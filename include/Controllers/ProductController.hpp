#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

namespace Controllers
{

class ProductController final : public HttpController<ProductController>
{
public:
    static void getProduct(const HttpRequestPtr& req, Callback&& callback, int id);
    static void getAllProducts(const HttpRequestPtr& req, Callback&& callback);
    static void getPopularProducts(const HttpRequestPtr& req, Callback&& callback);
    static void getNewProducts(const HttpRequestPtr& req, Callback&& callback);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(getProduct, "/getProduct/{id}", Get);
        ADD_METHOD_TO(getAllProducts, "/getAllProducts", Get);
        ADD_METHOD_TO(getPopularProducts, "/getPopularProducts", Get);
        ADD_METHOD_TO(getNewProducts, "/getNewProducts", Get);

    METHOD_LIST_END

};

}
