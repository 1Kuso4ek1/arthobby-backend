#include <Controllers/SwaggerController.hpp>

void SwaggerController::swaggerPage(const HttpRequestPtr& req, Callback&& callback)
{
    auto resp = HttpResponse::newRedirectionResponse("/index.html");

    callback(resp);
}

void SwaggerController::swaggerFile(const HttpRequestPtr& req, Callback&& callback)
{
    auto resp = HttpResponse::newFileResponse("../static/swagger.json");

    resp->addHeader("Access-Control-Allow-Origin", "*");

    callback(resp);
}
