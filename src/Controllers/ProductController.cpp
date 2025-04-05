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
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k404NotFound);

        callback(response);
    }
}

void ProductController::getProducts(const HttpRequestPtr& req, Callback&& callback)
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
