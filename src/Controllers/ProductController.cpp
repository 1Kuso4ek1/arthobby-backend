#include <Controllers/ProductController.hpp>
#include <Models/Product.hpp>

namespace Models = drogon_model::arthobby;

namespace Controllers
{

void ProductController::getProduct(const HttpRequestPtr& req, Callback&& callback, int id)
{
    static auto dbClient = app().getDbClient();
    static auto mapper = orm::Mapper<Models::Product>(dbClient);

    try
    {
        const auto queryResult = mapper.findByPrimaryKey(id);

        const auto response = HttpResponse::newHttpJsonResponse(queryResult.toJson());

        callback(response);
    }
    catch(...) // mapper.findByPrimaryKey throws an exception when nothing was found
    {
        // An empty json
        const auto response = HttpResponse::newHttpJsonResponse({});

        callback(response);
    }
}

void ProductController::getAllProducts(const HttpRequestPtr& req, Callback&& callback)
{
    static auto dbClient = app().getDbClient();
    static auto mapper = orm::Mapper<Models::Product>(dbClient);

    const auto queryResult = mapper.findAll();

    Json::Value json;
    for(auto& i : queryResult)
        json.append(i.toJson());

    const auto response = HttpResponse::newHttpJsonResponse(json);

    callback(response);
}

void ProductController::getPopularProducts(const HttpRequestPtr& req, Callback&& callback)
{
    static auto dbClient = app().getDbClient();
    static auto mapper = orm::Mapper<Models::Product>(dbClient);

    const auto queryResult =
        mapper
            .orderBy("popularity", orm::SortOrder::DESC)
            .limit(10)
            .findAll();

    Json::Value json;
    for(auto& i : queryResult)
        json.append(i.toJson());

    const auto response = HttpResponse::newHttpJsonResponse(json);

    callback(response);
}

void ProductController::getNewProducts(const HttpRequestPtr& req, Callback&& callback)
{
    static auto dbClient = app().getDbClient();
    static auto mapper = orm::Mapper<Models::Product>(dbClient);

    const auto queryResult =
        mapper
            .orderBy("dateofcreation", orm::SortOrder::DESC)
            .limit(10)
            .findAll();

    Json::Value json;
    for(auto& i : queryResult)
        json.append(i.toJson());

    const auto response = HttpResponse::newHttpJsonResponse(json);

    callback(response);
}

}
