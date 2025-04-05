#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

class ProductController : public HttpController<ProductController>
{
public:
    void getProduct(const HttpRequestPtr& req, Callback&& callback, int id);
    void getProducts(const HttpRequestPtr& req, Callback&& callback);

    METHOD_LIST_BEGIN

        ADD_METHOD_TO(ProductController::getProduct, "/product/{id}", Get);
        ADD_METHOD_TO(ProductController::getProducts, "/products/", Get);

    METHOD_LIST_END

};
