#include <Controllers/CartController.hpp>

#include <Models/Cart.hpp>
#include <Models/Product.hpp>

namespace Models = drogon_model::arthobby;

namespace Controllers
{

void CartController::getCart(const HttpRequestPtr& req, Callback&& callback)
{
    if(req->getParameter("user_id").empty())
    {
        const auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        callback(response);

        return;
    }

    static auto dbClient = app().getDbClient();
    static auto mapper = orm::Mapper<Models::Cart>(dbClient);

    int userId = std::stoi(req->getParameter("user_id"));

    const auto cart = mapper.findBy({ "user_id", userId });

    Json::Value json;
    int counter = 0;
    for(auto& i : cart)
    {
        json[counter]["product"] = i.getProduct(dbClient).toJson();
        json[counter++]["quantity"] = *i.getQuantity();
    }

    const auto response = HttpResponse::newHttpJsonResponse(json);

    callback(response);
}

void CartController::addToCart(const HttpRequestPtr& req, Callback&& callback)
{
    if(req->getParameter("user_id").empty())
    {
        const auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        callback(response);

        return;
    }

    static auto dbClient = app().getDbClient();
    static auto mapper = orm::Mapper<Models::Cart>(dbClient);

    int userId = std::stoi(req->getParameter("user_id"));
    int productId = (*req->getJsonObject())["product_id"].asInt();

    // If we already have such cart entry, update the quantity
    try
    {
        auto cart = mapper.findOne(
            orm::Criteria("user_id", userId)
            && orm::Criteria("product_id", productId)
        );

        cart.setQuantity(*cart.getQuantity() + 1);
        mapper.update(cart);
    }
    catch(...) // Otherwise, we will create a new one
    {
        Models::Cart cart;
        cart.setUserId(userId);
        cart.setProductId(productId);
        cart.setQuantity(1);

        mapper.insert(cart);
    }

    callback(HttpResponse::newHttpResponse());
}

void CartController::removeFromCart(const HttpRequestPtr& req, Callback&& callback)
{
    if(req->getParameter("user_id").empty())
    {
        const auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        callback(response);

        return;
    }

    static auto dbClient = app().getDbClient();
    static auto mapper = orm::Mapper<Models::Cart>(dbClient);

    int userId = std::stoi(req->getParameter("user_id"));
    int productId = (*req->getJsonObject())["product_id"].asInt();

    try
    {
        auto cart = mapper.findOne(
            orm::Criteria("user_id", userId)
            && orm::Criteria("product_id", productId)
        );

        if(*cart.getQuantity() > 1)
        {
            cart.setQuantity(*cart.getQuantity() - 1);
            mapper.update(cart);
        }
        else
            mapper.deleteByPrimaryKey(*cart.getId());
    }
    catch(...)
    {
        // Something we can ignore
    }

    callback(HttpResponse::newHttpResponse());
}

void CartController::clearCart(const HttpRequestPtr& req, Callback&& callback)
{
    if(req->getParameter("user_id").empty())
    {
        const auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        callback(response);

        return;
    }

    static auto dbClient = app().getDbClient();
    static auto mapper = orm::Mapper<Models::Cart>(dbClient);

    int userId = std::stoi(req->getParameter("user_id"));

    try
    {
        mapper.deleteBy({ "user_id", userId });
    }
    catch(...)
    {
        // No need to handle this I guess
    }

    callback(HttpResponse::newHttpResponse());
}

}
