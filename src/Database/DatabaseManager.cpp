#include <Database/DatabaseManager.hpp>

#include <fstream>

DatabaseManager::DatabaseManager()
{
    std::string db, user, password;

    // ENV_PATH comes from cmake's add_definitions
    // and refers to 'env' file in the source directory
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
