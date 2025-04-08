#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

class ProductController : public HttpController<ProductController>
{
public:
    void getProduct(const HttpRequestPtr& req, Callback&& callback, int id);
    void getAllProducts(const HttpRequestPtr& req, Callback&& callback);
    void getPopularProducts(const HttpRequestPtr& req, Callback&& callback);
    void getNewProducts(const HttpRequestPtr& req, Callback&& callback);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(ProductController::getProduct, "/getProduct/{id}", Get);
        ADD_METHOD_TO(ProductController::getAllProducts, "/getAllProducts", Get);
        ADD_METHOD_TO(ProductController::getPopularProducts, "/getPopularProducts", Get);
        ADD_METHOD_TO(ProductController::getNewProducts, "/getNewProducts", Get);

    METHOD_LIST_END

};
