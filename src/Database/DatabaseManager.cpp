#include <Database/DatabaseManager.hpp>

#include <fstream>

DatabaseManager::DatabaseManager()
{
    std::string host, port, db, user, password;

    // ENV_PATH comes from cmake's add_definitions
    // and refers to 'env' file in the source directory
    std::ifstream env(ENV_PATH);
    env >> host >> port >> db >> user >> password;

    dbClient = orm::DbClient::newPgClient(
        "host=" + host + " "
        "port=" + port + " "
        "dbname=" + db + " "
        "connect_timeout=10 "
        "password=" + password + " "
        "user=" + user,
        5
    );
}

orm::DbClientPtr DatabaseManager::getDbClient()
{
    return dbClient;
}
