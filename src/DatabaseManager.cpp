#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(std::unique_ptr<IDatabase> database) noexcept
    : _db(std::move(database)) {
    connect();
}

DatabaseManager::~DatabaseManager() noexcept { disconnect(); }

// Enable move constructor and assignment
DatabaseManager::DatabaseManager(DatabaseManager&& dbManager) noexcept
    : _db(std::move(dbManager._db)) {}
DatabaseManager& DatabaseManager::operator=(
    DatabaseManager&& dbManager) noexcept {
    if (this != &dbManager) {
        disconnect();

        _db = std::move(dbManager._db);
    }
    return *this;
}

IDatabase* DatabaseManager::operator->() const noexcept { return _db.get(); }
IDatabase& DatabaseManager::operator*() const noexcept { return *_db; }
IDatabase* DatabaseManager::get() const noexcept { return _db.get(); }

bool DatabaseManager::valid() const noexcept { return _db != nullptr; }

void DatabaseManager::connect() noexcept {
    if (_db && !_db->connected()) _db->connect();
}

void DatabaseManager::disconnect() noexcept {
    if (_db && _db->connected()) _db->disconnect();
}
