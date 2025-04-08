#include <drogon/drogon.h>

using namespace drogon;
using namespace trantor;

using namespace std::chrono_literals;

int main(int argc, char* argv[])
{
    std::string configPath = "../config/sample.json";

    if(argc > 1)
        configPath = argv[1];

    app()
        .loadConfigFile(configPath)
        .registerPostHandlingAdvice(
            [](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp) {
                resp->addHeader("Access-Control-Allow-Origin", "*");
            }
        )
        .run();
}
