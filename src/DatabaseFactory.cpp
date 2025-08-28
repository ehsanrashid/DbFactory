#include "DatabaseFactory.h"

#include <stdexcept>

#include "MySQLDatabase.h"
#include "PostgreDatabase.h"
#include "RedisDatabase.h"
#include "SQLiteDatabase.h"

// Static member definition
std::unordered_map<std::string, DatabaseFactory::Creator>
    DatabaseFactory::_creators;

// Register a database type with its creator function
void DatabaseFactory::register_database(const std::string& dbType,
                                        Creator creator) noexcept {
    _creators[dbType] = creator;
}

// Initialize factory with default database types
void DatabaseFactory::initialize() noexcept {
    register_database(
        "mysql",
        [](const DatabaseConfig& dbConfig) -> std::unique_ptr<IDatabase> {
            return std::make_unique<MySQLDatabase>(
                dbConfig.host.empty() ? "localhost" : dbConfig.host,
                dbConfig.port == 0 ? 3306 : dbConfig.port);
        });

    register_database(
        "postgresql",
        [](const DatabaseConfig& dbConfig) -> std::unique_ptr<IDatabase> {
            return std::make_unique<PostgreDatabase>(
                dbConfig.host.empty() ? "localhost" : dbConfig.host,
                dbConfig.port == 0 ? 5432 : dbConfig.port,
                dbConfig.database.empty() ? "postgres" : dbConfig.database,
                dbConfig.username, dbConfig.password);
        });

    register_database(
        "postgres",
        [](const DatabaseConfig& dbConfig) -> std::unique_ptr<IDatabase> {
            return std::make_unique<PostgreDatabase>(
                dbConfig.host.empty() ? "localhost" : dbConfig.host,
                dbConfig.port == 0 ? 5432 : dbConfig.port,
                dbConfig.database.empty() ? "postgres" : dbConfig.database,
                dbConfig.username, dbConfig.password);
        });

    register_database(
        "sqlite",
        [](const DatabaseConfig& dbConfig) -> std::unique_ptr<IDatabase> {
            return std::make_unique<SQLiteDatabase>(
                dbConfig.filepath.empty() ? ":memory:" : dbConfig.filepath);
        });

    register_database(
        "redis",
        [](const DatabaseConfig& dbConfig) -> std::unique_ptr<IDatabase> {
            return std::make_unique<RedisDatabase>(
                dbConfig.host.empty() ? "localhost" : dbConfig.host,
                dbConfig.port == 0 ? 6379 : dbConfig.port);
        });
}

// Check if a database type is supported
bool DatabaseFactory::supported(const std::string& dbType) noexcept {
    return _creators.find(dbType) != _creators.end();
}

// Get list of available database types
std::vector<std::string> DatabaseFactory::available_types() noexcept {
    std::vector<std::string> dbTypes;
    for (const auto& creator : _creators) dbTypes.push_back(creator.first);

    return dbTypes;
}

// Create database instance
std::unique_ptr<IDatabase> DatabaseFactory::create(
    const std::string& dbType, const DatabaseConfig& dbConfig) {
    auto itr = _creators.find(dbType);

    if (itr == _creators.end())
        throw std::runtime_error("Unknown database type: " + dbType);

    return itr->second(dbConfig);
}
