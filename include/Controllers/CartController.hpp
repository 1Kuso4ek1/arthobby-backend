#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

class CartController : public HttpController<CartController>
{
public:
    void getCart(const HttpRequestPtr& req, Callback&& callback);

    void addToCart(const HttpRequestPtr& req, Callback&& callback);
    void removeFromCart(const HttpRequestPtr& req, Callback&& callback);
    void clearCart(const HttpRequestPtr& req, Callback&& callback);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(CartController::getCart, "/getCart", Get, "JwtFilter");
        
        ADD_METHOD_TO(CartController::addToCart, "/addToCart", Post, "JwtFilter");
        ADD_METHOD_TO(CartController::removeFromCart, "/removeFromCart", Post, "JwtFilter");
        ADD_METHOD_TO(CartController::clearCart, "/clearCart", Post, "JwtFilter");

    METHOD_LIST_END

};
