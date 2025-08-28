#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "DatabaseConfig.h"
#include "IDatabase.h"

// Enhanced Database Factory with registration mechanism
class DatabaseFactory final {
   private:
    using Creator =
        std::function<std::unique_ptr<IDatabase>(const DatabaseConfig&)>;

    static std::unordered_map<std::string, Creator> _creators;

    DatabaseFactory() noexcept = delete;
    ~DatabaseFactory() noexcept = delete;

    DatabaseFactory(const DatabaseFactory&) noexcept = delete;
    DatabaseFactory& operator=(const DatabaseFactory&) noexcept = delete;
    DatabaseFactory(DatabaseFactory&&) noexcept = delete;
    DatabaseFactory& operator=(DatabaseFactory&&) noexcept = delete;

   public:
    // Register a database type with its creator function
    static void register_database(const std::string& dbType,
                                  Creator creator) noexcept;

    // Initialize factory with default database types
    static void initialize() noexcept;

    // Check if a database type is supported
    static bool supported(const std::string& dbType) noexcept;

    // Get list of available database types
    static std::vector<std::string> available_types() noexcept;

    // Create database instance
    static std::unique_ptr<IDatabase> create(
        const std::string& dbType,
        const DatabaseConfig& dbConfig = DatabaseConfig{});
};
