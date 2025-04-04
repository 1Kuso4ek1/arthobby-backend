#include <Controllers/ProductController.hpp>
#include <Database/DatabaseManager.hpp>
#include <Models/Product.hpp>

namespace models = drogon_model::arthobby;

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
