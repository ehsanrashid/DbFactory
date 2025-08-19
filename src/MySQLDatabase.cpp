#include "MySQLDatabase.h"

#include <iostream>
#include <stdexcept>

MySQLDatabase::MySQLDatabase(const std::string& host, int port) noexcept
    : _host(host), _port(port == 0 ? 3306 : port), _connected(false) {}

std::string MySQLDatabase::connection_info() const noexcept {
    return "MySQL Database at " + _host + ":" + std::to_string(_port);
}

bool MySQLDatabase::connected() const noexcept { return _connected; }

void MySQLDatabase::connect() {
    if (_connected) {
        std::cout << "[MySQL] Already connected\n";
        return;
    }
    std::cout << "[MySQL] Connecting to " << _host << ":" << _port << "\n";
    // Simulate connection logic
    _connected = true;
    std::cout << "[MySQL] Successfully connected\n";
}

void MySQLDatabase::disconnect() {
    if (!_connected) {
        std::cout << "[MySQL] Already disconnected\n";
        return;
    }
    std::cout << "[MySQL] Disconnecting from database\n";
    _connected = false;
    std::cout << "[MySQL] Successfully disconnected\n";
}

std::unique_ptr<IResult> MySQLDatabase::exec(const std::string& sql) {
    if (!_connected) {
        throw std::runtime_error("[MySQL] Database not connected");
    }
    std::cout << "[MySQL] Executing query: " << sql << "\n";
    // Simulate query execution
    std::cout << "[MySQL] Query executed successfully\n";
    return nullptr;
}

std::unique_ptr<IResult> MySQLDatabase::exec_params(
    const std::string& sql, const std::vector<std::any>& args) {
    return nullptr;
}
