#pragma once
#include <drogon/HttpController.h>
#include <drogon/orm/DbClient.h>

#include <jwt-cpp/jwt.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

namespace Controllers
{

class LoginController final : public HttpController<LoginController>
{
public:
    static void registerUser(const HttpRequestPtr& req, Callback&& callback);
    static void login(const HttpRequestPtr& req, Callback&& callback);

    static void refresh(const HttpRequestPtr& req, Callback&& callback);
    static void logout(const HttpRequestPtr& req, Callback&& callback);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(registerUser, "/register", Post);
        ADD_METHOD_TO(login, "/login", Post);
        ADD_METHOD_TO(refresh, "/refresh", Post);
        ADD_METHOD_TO(logout, "/logout", Post);

    METHOD_LIST_END

private:
    static void saveRefreshToCookie(const std::string& token, const HttpResponsePtr& resp, int maxAge = 604800); // 7 days
    static bool validateUser(const std::shared_ptr<Json::Value>& json);

    static std::string makeAccessToken(int id, const std::string& username);
    static std::string makeRefreshToken(int id, const std::string& username);
};

}
