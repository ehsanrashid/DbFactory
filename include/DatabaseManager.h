#pragma once

#include <memory>

#include "IDatabase.h"

// RAII Database Connection Manager
class DatabaseManager final {
   public:
    DatabaseManager(std::unique_ptr<IDatabase> database) noexcept;

    // Disable copy constructor and assignment
    DatabaseManager(const DatabaseManager&) noexcept = delete;
    DatabaseManager& operator=(const DatabaseManager&) noexcept = delete;

    // Enable move constructor and assignment
    DatabaseManager(DatabaseManager&& dbManager) noexcept;
    DatabaseManager& operator=(DatabaseManager&& dbManager) noexcept;

    ~DatabaseManager() noexcept;

    IDatabase* operator->() const noexcept;
    IDatabase& operator*() const noexcept;
    IDatabase* get() const noexcept;

    bool valid() const noexcept;

   private:
    void connect() noexcept;
    void disconnect() noexcept;

    std::unique_ptr<IDatabase> _db;
};
