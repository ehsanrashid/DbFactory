#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(std::unique_ptr<IDatabase> database)
    : _db(std::move(database)) {
    if (_db) _db->connect();
}

DatabaseManager::~DatabaseManager() {
    if (_db && _db->connected()) _db->disconnect();
}

// Enable move constructor and assignment
DatabaseManager::DatabaseManager(DatabaseManager&& other) noexcept
    : _db(std::move(other._db)) {}
DatabaseManager& DatabaseManager::operator=(DatabaseManager&& other) noexcept {
    if (this != &other) {
        if (_db && _db->connected()) {
            _db->disconnect();
        }
        _db = std::move(other._db);
    }
    return *this;
}

IDatabase* DatabaseManager::operator->() const noexcept { return _db.get(); }
IDatabase& DatabaseManager::operator*() const noexcept { return *_db; }
IDatabase* DatabaseManager::get() const noexcept { return _db.get(); }

bool DatabaseManager::valid() const noexcept { return _db != nullptr; }
