#include <Controllers/CartController.hpp>
#include <Database/DatabaseManager.hpp>
#include <Models/Product.hpp>
#include <Models/User.hpp>
#include <Models/Cart.hpp>

namespace models = drogon_model::arthobby;

void CartController::getCart(const HttpRequestPtr& req, Callback&& callback)
{
    if(req->getParameter("user_id").empty())
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(drogon::k401Unauthorized);

        callback(response);

        return;
    }

    static auto dbClient = DatabaseManager::get().getDbClient();
    static auto& mapper = DatabaseManager::get().getMapper<models::Cart>();

    int userId = std::stoi(req->getParameter("user_id"));

    auto cart = mapper.findBy(
        orm::Criteria(
            "user_id",
            orm::CompareOperator::EQ,
            userId
        )
    );

    Json::Value json;
    int counter = 0;
    for(auto& i : cart)
    {
        json[counter]["product"] = i.getProduct(dbClient).toJson();
        json[counter++]["quantity"] = *i.getQuantity();
    }

    auto response = HttpResponse::newHttpJsonResponse(json);

    callback(response);
}

void CartController::addToCart(const HttpRequestPtr& req, Callback&& callback)
{
    if(req->getParameter("user_id").empty())
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(drogon::k401Unauthorized);

        callback(response);

        return;
    }

    static auto dbClient = DatabaseManager::get().getDbClient();
    static auto& mapper = DatabaseManager::get().getMapper<models::Cart>();

    int userId = std::stoi(req->getParameter("user_id"));
    int productId = (*req->getJsonObject())["product_id"].asInt();

    // If we already have such cart entry, just update the quantity
    try
    {
        auto cart = mapper.findOne(
            orm::Criteria(
                orm::CustomSql("user_id = $? AND product_id = $?"),
                userId, productId
            )
        );

        cart.setQuantity(*cart.getQuantity().get() + 1);
        mapper.update(cart);
    }
    catch(...) // Otherwise, we will create a new one
    {
        models::Cart cart;
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
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(drogon::k401Unauthorized);

        callback(response);

        return;
    }

    static auto dbClient = DatabaseManager::get().getDbClient();
    static auto& mapper = DatabaseManager::get().getMapper<models::Cart>();

    int userId = std::stoi(req->getParameter("user_id"));
    int productId = (*req->getJsonObject())["product_id"].asInt();

    try
    {
        auto cart = mapper.findOne(
            orm::Criteria(
                orm::CustomSql("user_id = $? AND product_id = $?"),
                userId, productId
            )
        );

        if(*cart.getQuantity().get() > 1)
        {
            cart.setQuantity(*cart.getQuantity().get() - 1);
            mapper.update(cart);
        }
        else
            mapper.deleteByPrimaryKey(*cart.getId());
    }
    catch(...)
    {
        // Nothing to delete
    }

    callback(HttpResponse::newHttpResponse());
}

void CartController::clearCart(const HttpRequestPtr& req, Callback&& callback)
{
    if(req->getParameter("user_id").empty())
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(drogon::k401Unauthorized);

        callback(response);

        return;
    }

    static auto dbClient = DatabaseManager::get().getDbClient();
    static auto& mapper = DatabaseManager::get().getMapper<models::Cart>();

    int userId = std::stoi(req->getParameter("user_id"));

    // May throw an exception, but I won't pay attention!!!
    mapper.deleteBy(
        orm::Criteria(
            "user_id",
            orm::CompareOperator::EQ,
            userId
        )
    );

    callback(HttpResponse::newHttpResponse());
}
