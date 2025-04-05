#include <Controllers/ProductController.hpp>
#include <Database/DatabaseManager.hpp>
#include <Models/Product.hpp>

namespace models = drogon_model::arthobby;

void ProductController::getProduct(const HttpRequestPtr& req, Callback&& callback, int id)
{
    static auto dbClient = DatabaseManager::get().getDbClient();
    static auto& mapper = DatabaseManager::get().getMapper<models::Product>();

    try
    {
        auto queryResult = mapper.findByPrimaryKey(id);

        auto response = HttpResponse::newHttpJsonResponse(queryResult.toJson());

        callback(response);
    }
    catch(...) // mapper.findByPrimaryKey throws an exception when nothing was found
    {
        // An empty json
        auto response = HttpResponse::newHttpJsonResponse({});

        callback(response);
    }
}

void ProductController::getAllProducts(const HttpRequestPtr& req, Callback&& callback)
{
    static auto dbClient = DatabaseManager::get().getDbClient();
    static auto& mapper = DatabaseManager::get().getMapper<models::Product>();

    auto queryResult = mapper.findAll();

    Json::Value json;
    for(auto& i : queryResult)
        json.append(i.toJson());

    auto response = HttpResponse::newHttpJsonResponse(json);

    callback(response);
}

void ProductController::getPopularProducts(const HttpRequestPtr& req, Callback&& callback)
{
    static auto dbClient = DatabaseManager::get().getDbClient();
    static auto& mapper = DatabaseManager::get().getMapper<models::Product>();

    auto queryResult =
        mapper
            .orderBy("popularity", orm::SortOrder::DESC)
            .limit(10)
            .findAll();

    Json::Value json;
    for(auto& i : queryResult)
        json.append(i.toJson());

    auto response = HttpResponse::newHttpJsonResponse(json);

    callback(response);
}
