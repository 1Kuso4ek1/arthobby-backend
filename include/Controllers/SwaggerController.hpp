#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

class SwaggerController : public HttpController<SwaggerController>
{
public:
    void swaggerPage(const HttpRequestPtr& req, Callback&& callback);
    void swaggerFile(const HttpRequestPtr& req, Callback&& callback);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(SwaggerController::swaggerPage, "/swagger", Get);
        ADD_METHOD_TO(SwaggerController::swaggerFile, "/swagger/swagger.json", Get);

    METHOD_LIST_END

};
