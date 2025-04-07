#include <Controllers/LoginController.hpp>
#include <Database/DatabaseManager.hpp>
#include <Models/User.hpp>

using namespace std::chrono_literals;

namespace models = drogon_model::arthobby;

void LoginController::registerUser(const HttpRequestPtr& req, Callback&& callback)
{
    auto request = req->getJsonObject();

    if(!request || !request->isMember("username") || !request->isMember("password"))
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k400BadRequest);

        callback(response);
        return;
    }

    static auto& mapper = DatabaseManager::get().getMapper<models::User>();

    models::User user;
    user.setUsername((*request)["username"].asString());
    user.setPassword((*request)["password"].asString());

    try
    {
        mapper.insert(user);
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;

        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(drogon::k409Conflict);

        callback(response);
        return;
    }

    login(req, std::move(callback));
}

void LoginController::login(const HttpRequestPtr& req, Callback&& callback)
{
    auto request = req->getJsonObject();

    if(validateUser(request))
    {
        auto userId = (*request)["user_id"].asInt();
        auto username = (*request)["username"].asString();
        
        auto accessToken = makeAccessToken(userId, username);
        auto refreshToken = makeRefreshToken(userId, username);

        Json::Value json;
        json["token"] = accessToken;

        auto response = HttpResponse::newHttpJsonResponse(json);

        saveRefreshToCookie(refreshToken, response);

        callback(response);
    }
    else
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        callback(response);
    }
}

void LoginController::refresh(const HttpRequestPtr& req, Callback&& callback)
{
    auto refreshToken = req->getCookie("refreshToken");

    if(refreshToken.empty())
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        callback(response);
        return;
    }

    try
    {
        auto decoded = jwt::decode(refreshToken);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256("refreshSecret"))
            .with_issuer("auth0");

        verifier.verify(decoded);

        auto accessToken =
            makeAccessToken(
                decoded.get_payload_claim("user_id").as_integer(),
                decoded.get_payload_claim("username").as_string()
            );

        Json::Value json;
        json["token"] = accessToken;

        auto response = HttpResponse::newHttpJsonResponse(json);

        callback(response);
    }
    catch(const std::exception& e)
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        callback(response);
    }
}

void LoginController::logout(const HttpRequestPtr& req, Callback&& callback)
{
    auto response = HttpResponse::newHttpResponse();

    // Removing cookie
    saveRefreshToCookie("", response, 0);

    callback(response);
}

void LoginController::saveAccessToCookie(const std::string& token, const HttpResponsePtr& resp, int maxAge)
{
    /* Cookie cookie("accessToken", token);
    cookie.setHttpOnly(true);
    cookie.setSecure(true);
    cookie.setSameSite(Cookie::SameSite::kStrict);
    cookie.setPath("/access");
    cookie.setMaxAge(maxAge);

    resp->addCookie(cookie); */
}

void LoginController::saveRefreshToCookie(const std::string& token, const HttpResponsePtr& resp, int maxAge)
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
    static auto& mapper = DatabaseManager::get().getMapper<drogon_model::arthobby::User>();

    if(!json || !json->isMember("username") || !json->isMember("password"))
        return false;

    try
    {
        auto user = mapper.findOne(
            orm::Criteria(
                "username",
                orm::CompareOperator::EQ,
                (*json)["username"].asString()
            )
        );

        (*json)["user_id"] = *user.getId();

        if(user.getPassword())
            return *user.getPassword().get() == (*json)["password"].asString();
    }
    catch(...)
    {
        return false;
    }

    return false;
}

std::string LoginController::makeAccessToken(int id, const std::string& username)
{
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
        .sign(jwt::algorithm::hs256("secret")); // Change the secret to something *really* secret

    return token;
}

std::string LoginController::makeRefreshToken(int id, const std::string& username)
{
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
        .sign(jwt::algorithm::hs256("refreshSecret"));

    return token;
}
