#include "SQLiteDatabase.h"

#include <iostream>
#include <stdexcept>

SQLiteDatabase::SQLiteDatabase(const std::string& filepath) noexcept
    : _filepath(filepath), _connected(false) {}

std::string SQLiteDatabase::connection_info() const noexcept {
    return "SQLite Database at " + _filepath;
}

bool SQLiteDatabase::connected() const noexcept { return _connected; }

void SQLiteDatabase::connect() {
    if (_connected) {
        std::cout << "[SQLite] Already connected\n";
        return;
    }
    std::cout << "[SQLite] Opening database: " << _filepath << "\n";
    // Simulate connection logic
    _connected = true;
    std::cout << "[SQLite] Successfully opened\n";
}

void SQLiteDatabase::disconnect() {
    if (!_connected) {
        std::cout << "[SQLite] Already disconnected\n";
        return;
    }
    std::cout << "[SQLite] Closing database\n";
    _connected = false;
    std::cout << "[SQLite] Successfully closed\n";
}

std::unique_ptr<IResult> SQLiteDatabase::exec(const std::string& sql) {
    if (!_connected) {
        throw std::runtime_error("[SQLite] Database not connected");
    }
    std::cout << "[SQLite] Executing query: " << sql << "\n";
    // Simulate query execution
    std::cout << "[SQLite] Query executed successfully\n";
    return nullptr;
}

std::unique_ptr<IResult> SQLiteDatabase::exec_params(
    const std::string& sql, const std::vector<std::any>& args) {
    return nullptr;
}
