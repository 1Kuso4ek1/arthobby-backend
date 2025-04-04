#include <Controllers/LoginController.hpp>

using namespace std::chrono_literals;

void LoginController::login(const HttpRequestPtr& req, Callback&& callback)
{
    auto request = req->getJsonObject();

    if(validateUser(request))
    {
        auto claim = (*request)["username"].asString();
        
        auto accessToken = makeAccessToken(claim);
        auto refreshToken = makeRefreshToken(claim);

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

        auto accessToken = makeAccessToken(decoded.get_payload_claim("username").as_string());

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
    if(!json)
        return false;

    // Just a placeholder
    return ((*json)["username"] == "amogus" && (*json)["password"] == "test");
}

std::string LoginController::makeAccessToken(const std::string& claim)
{
    auto token = jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_expires_in(900s) // 15 min
        .set_payload_claim(
            "username",
            jwt::claim(claim)
        )
        .sign(jwt::algorithm::hs256("secret"));

    return token;
}

std::string LoginController::makeRefreshToken(const std::string& claim)
{
    auto token = jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_expires_in(604800s) // 7 days
        .set_payload_claim(
            "username",
            jwt::claim(claim)
        )
        .sign(jwt::algorithm::hs256("refreshSecret"));

    return token;
}
