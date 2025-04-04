#pragma once
#include <drogon/HttpController.h>
#include <drogon/orm/DbClient.h>

#include <jwt-cpp/jwt.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

class LoginController : public HttpController<LoginController>
{
public:
    void login(const HttpRequestPtr& req, Callback&& callback);
    void refresh(const HttpRequestPtr& req, Callback&& callback);
    void logout(const HttpRequestPtr& req, Callback&& callback);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(LoginController::login, "/login", Post);
        ADD_METHOD_TO(LoginController::refresh, "/refresh", Post);
        ADD_METHOD_TO(LoginController::logout, "/logout", Post);

    METHOD_LIST_END

private:
    void saveRefreshToCookie(const std::string& token, const HttpResponsePtr& resp, int maxAge = 604800); // 7 days

    bool validateUser(const std::shared_ptr<Json::Value>& json);

    std::string makeAccessToken(const std::string& claim);
    std::string makeRefreshToken(const std::string& claim);
};

