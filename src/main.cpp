#include <drogon/drogon.h>

using namespace drogon;
using namespace trantor;

using namespace std::chrono_literals;

int main()
{
    app()
        .setLogPath("./")
        .setLogLevel(Logger::kWarn)
        //.setSSLFiles("../cert/cert.pem", "../cert/key.pem")
        .addListener("0.0.0.0", 8080/* , true */)
        .setThreadNum(16)
        // .enableRunAsDaemon()
        .enableSession(1min)
        .setDocumentRoot("../static/swagger")
        .run();
}
