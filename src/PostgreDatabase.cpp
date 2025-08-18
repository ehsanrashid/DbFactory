#include "PostgreDatabase.h"

#include <iostream>
#include <stdexcept>

PostgreDatabase::PostgreDatabase(const std::string& host, int port,
                                 const std::string& database,
                                 const std::string& username,
                                 const std::string& password) noexcept
    : _host(host),
      _port(port == 0 ? 5432 : port),
      _database(database),
      _username(username),
      _password(password),
      _connected(false) {}

PostgreDatabase::PostgreDatabase(const std::string& host, int port,
                                 const std::string& database) noexcept
    : _host(host),
      _port(port == 0 ? 5432 : port),
      _database(database),
      _connected(false) {}

std::string PostgreDatabase::connection_info() const noexcept {
    return "Postgre Database at " + _host + ":" + std::to_string(_port) + "/" +
           _database;
}

bool PostgreDatabase::connected() const noexcept { return _connected; }

void PostgreDatabase::connect() noexcept {
    if (_connected) {
        std::cout << "[Postgre] Already connected\n";
        return;
    }
    std::cout << "[Postgre] Connecting to " << _host << ":" << _port << "/"
              << _database << "\n";
    // Simulate connection logic
    _connected = true;
    std::cout << "[Postgre] Successfully connected\n";
}

void PostgreDatabase::disconnect() noexcept {
    if (!_connected) {
        std::cout << "[Postgre] Already disconnected\n";
        return;
    }
    std::cout << "[Postgre] Disconnecting from database\n";
    _connected = false;
    std::cout << "[Postgre] Successfully disconnected\n";
}

void PostgreDatabase::query(const std::string& sql) {
    if (!_connected) {
        throw std::runtime_error("[Postgre] Database not connected");
    }
    std::cout << "[Postgre] Executing query: " << sql << "\n";
    // Simulate query execution
    std::cout << "[Postgre] Query executed successfully\n";
}
