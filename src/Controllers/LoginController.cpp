#include <Controllers/LoginController.hpp>

#include <Models/User.hpp>

using namespace std::chrono_literals;

namespace Models = drogon_model::arthobby;

namespace Controllers
{

void LoginController::registerUser(const HttpRequestPtr& req, Callback&& callback)
{
    const auto request = req->getJsonObject();

    if(!request || !request->isMember("username") || !request->isMember("password"))
    {
        const auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k400BadRequest);

        callback(response);
        return;
    }

    static auto mapper = orm::Mapper<Models::User>(app().getDbClient());

    Models::User user;
    user.setUsername((*request)["username"].asString());
    user.setPassword((*request)["password"].asString());

    try
    {
        mapper.insert(user);
    }
    catch(const std::exception& e)
    {
        LOG_ERROR << e.what();

        const auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k409Conflict);

        callback(response);

        return;
    }

    login(req, std::move(callback));
}

void LoginController::login(const HttpRequestPtr& req, Callback&& callback)
{
    if(const auto request = req->getJsonObject();
        validateUser(request))
    {
        const auto userId = (*request)["user_id"].asInt();
        const auto username = (*request)["username"].asString();
        
        const auto accessToken = makeAccessToken(userId, username);
        const auto refreshToken = makeRefreshToken(userId, username);

        Json::Value json;
        json["token"] = accessToken;

        const auto response = HttpResponse::newHttpJsonResponse(json);

        saveRefreshToCookie(refreshToken, response);

        callback(response);
    }
    else
    {
        const auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        callback(response);
    }
}

void LoginController::refresh(const HttpRequestPtr& req, Callback&& callback)
{
    static auto refreshSecret = app().getCustomConfig()["jwt"]["refresh_secret"].asString();

    const auto refreshToken = req->getCookie("refreshToken");

    if(refreshToken.empty())
    {
        const auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        callback(response);
        return;
    }

    try
    {
        const auto decoded = jwt::decode(refreshToken);
        const auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256(refreshSecret))
            .with_issuer("auth0");

        verifier.verify(decoded);

        const auto accessToken =
            makeAccessToken(
                decoded.get_payload_claim("user_id").as_integer(),
                decoded.get_payload_claim("username").as_string()
            );

        Json::Value json;
        json["token"] = accessToken;

        const auto response = HttpResponse::newHttpJsonResponse(json);

        callback(response);
    }
    catch(...)
    {
        const auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        callback(response);
    }
}

void LoginController::logout(const HttpRequestPtr& req, Callback&& callback)
{
    const auto response = HttpResponse::newHttpResponse();

    // Removing cookie
    saveRefreshToCookie("", response, 0);

    callback(response);
}

void LoginController::saveRefreshToCookie(const std::string& token, const HttpResponsePtr& resp, const int maxAge)
{
    Cookie cookie("refreshToken", token);
    cookie.setHttpOnly(true);
    cookie.setSecure(true);
    cookie.setSameSite(Cookie::SameSite::kStrict);
    cookie.setPath("/refresh");
    cookie.setMaxAge(maxAge);

    resp->addCookie(cookie);
}

bool LoginController::validateUser(const std::shared_ptr<Json::Value>& json)
{
    static auto mapper = orm::Mapper<Models::User>(app().getDbClient());

    if(!json || !json->isMember("username") || !json->isMember("password"))
        return false;

    try
    {
        const auto user = mapper.findOne({ "username", (*json)["username"].asString() });

        (*json)["user_id"] = *user.getId();

        if(user.getPassword())
            return *user.getPassword() == (*json)["password"].asString();
    }
    catch(...)
    {
        return false;
    }

    return false;
}

std::string LoginController::makeAccessToken(const int id, const std::string& username)
{
    static auto accessSecret = app().getCustomConfig()["jwt"]["access_secret"].asString();

    auto token = jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_expires_in(2592000s) // 1 month
        .set_payload_claim(
            "user_id",
            jwt::claim(std::to_string(id))
        )
        .set_payload_claim(
            "username",
            jwt::claim(username)
        )
        .sign(jwt::algorithm::hs256(accessSecret)); // Change the secret to something *really* secret

    return token;
}

std::string LoginController::makeRefreshToken(const int id, const std::string& username)
{
    static auto refreshSecret = drogon::app().getCustomConfig()["jwt"]["refresh_secret"].asString();

    auto token = jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_expires_in(604800s) // 7 days
        .set_payload_claim(
            "user_id",
            jwt::claim(std::to_string(id))
        )
        .set_payload_claim(
            "username",
            jwt::claim(username)
        )
        .sign(jwt::algorithm::hs256(refreshSecret));

    return token;
}

}
