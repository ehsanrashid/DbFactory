#pragma once

#include <memory>

#include "IDatabase.h"

// RAII Database Connection Manager
class DatabaseManager final {
   public:
    DatabaseManager(std::unique_ptr<IDatabase> database);

    ~DatabaseManager();

    // Disable copy constructor and assignment
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // Enable move constructor and assignment
    DatabaseManager(DatabaseManager&& other) noexcept;
    DatabaseManager& operator=(DatabaseManager&& other) noexcept;

    IDatabase* operator->() const noexcept;
    IDatabase& operator*() const noexcept;
    IDatabase* get() const noexcept;

    bool valid() const noexcept;

   private:
    std::unique_ptr<IDatabase> _db;
};
