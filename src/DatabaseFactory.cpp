#include "DatabaseFactory.h"

#include <stdexcept>

#include "MySQLDatabase.h"
#include "PostgreDatabase.h"
#include "RedisDatabase.h"
#include "SQLiteDatabase.h"

// Static member definition
std::unordered_map<std::string, DatabaseFactory::Creator>
    DatabaseFactory::creators;

// Register a database type with its creator function
void DatabaseFactory::register_database(const std::string& type,
                                        Creator creator) noexcept {
    creators[type] = creator;
}

// Initialize factory with default database types
void DatabaseFactory::initialize() noexcept {
    register_database(
        "mysql",
        [](const DatabaseConfig& config) -> std::unique_ptr<IDatabase> {
            return std::make_unique<MySQLDatabase>(
                config.host.empty() ? "localhost" : config.host,
                config.port == 0 ? 3306 : config.port);
        });

    register_database(
        "postgresql",
        [](const DatabaseConfig& config) -> std::unique_ptr<IDatabase> {
            return std::make_unique<PostgreDatabase>(
                config.host.empty() ? "localhost" : config.host,
                config.port == 0 ? 5432 : config.port,
                config.database.empty() ? "postgres" : config.database,
                config.username, config.password);
        });

    register_database(
        "postgres",
        [](const DatabaseConfig& config) -> std::unique_ptr<IDatabase> {
            return std::make_unique<PostgreDatabase>(
                config.host.empty() ? "localhost" : config.host,
                config.port == 0 ? 5432 : config.port,
                config.database.empty() ? "postgres" : config.database,
                config.username, config.password);
        });

    register_database(
        "sqlite",
        [](const DatabaseConfig& config) -> std::unique_ptr<IDatabase> {
            return std::make_unique<SQLiteDatabase>(
                config.filepath.empty() ? ":memory:" : config.filepath);
        });

    register_database(
        "redis",
        [](const DatabaseConfig& config) -> std::unique_ptr<IDatabase> {
            return std::make_unique<RedisDatabase>(
                config.host.empty() ? "localhost" : config.host,
                config.port == 0 ? 6379 : config.port);
        });
}

// Check if a database type is supported
bool DatabaseFactory::supported(const std::string& type) noexcept {
    return creators.find(type) != creators.end();
}

// Get list of available database types
std::vector<std::string> DatabaseFactory::available_types() noexcept {
    std::vector<std::string> types;
    for (const auto& pair : creators) types.push_back(pair.first);

    return types;
}

// Create database instance
std::unique_ptr<IDatabase> DatabaseFactory::create(
    const std::string& type, const DatabaseConfig& config) {
    auto itr = creators.find(type);
    if (itr == creators.end())
        throw std::runtime_error("Unknown database type: " + type);

    return itr->second(config);
}
