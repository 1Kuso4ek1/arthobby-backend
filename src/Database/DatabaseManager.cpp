#include <Database/DatabaseManager.hpp>

#include <fstream>

DatabaseManager::DatabaseManager()
{
    std::string db, user, password;

    std::ifstream env(ENV_PATH);
    env >> db >> user >> password;

    dbClient = orm::DbClient::newPgClient(
        "host=localhost "
        "port=5432 "
        "dbname=" + db +" "
        "connect_timeout=10 "
        "password=" + password + " "
        "user=" + user,
        1
    );
}

orm::DbClientPtr DatabaseManager::getDbClient()
{
    return dbClient;
}
