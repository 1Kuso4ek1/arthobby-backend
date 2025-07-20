#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

namespace Controllers
{

class CartController final : public HttpController<CartController>
{
public:
    static void getCart(const HttpRequestPtr& req, Callback&& callback);

    static void addToCart(const HttpRequestPtr& req, Callback&& callback);
    static void removeFromCart(const HttpRequestPtr& req, Callback&& callback);
    static void clearCart(const HttpRequestPtr& req, Callback&& callback);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(getCart, "/getCart", Get, "JwtFilter");

        ADD_METHOD_TO(addToCart, "/addToCart", Post, "JwtFilter");
        ADD_METHOD_TO(removeFromCart, "/removeFromCart", Post, "JwtFilter");
        ADD_METHOD_TO(clearCart, "/clearCart", Post, "JwtFilter");

    METHOD_LIST_END

};

}
