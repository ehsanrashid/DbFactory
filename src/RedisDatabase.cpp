#include "RedisDatabase.h"

#include <iostream>
#include <stdexcept>

RedisDatabase::RedisDatabase(const std::string& host, int port)
    : _host(host), _port(port), _connected(false) {}

std::string RedisDatabase::connection_info() const noexcept {
    return "Redis at " + _host + ":" + std::to_string(_port);
}

bool RedisDatabase::connected() const noexcept { return _connected; }

void RedisDatabase::connect() {
    std::cout << "[Redis] Connecting to " << _host << ":" << _port << "\n";
    _connected = true;
}

void RedisDatabase::disconnect() {
    if (_connected) {
        std::cout << "[Redis] Disconnecting\n";
        _connected = false;
    }
}

std::unique_ptr<IResult> RedisDatabase::exec(const std::string& sql) {
    if (!_connected) {
        throw std::runtime_error("[Redis] Database not connected");
    }
    std::cout << "[Redis] Executing query: " << sql << "\n";
    // Simulate query execution
    std::cout << "[Redis] Query executed successfully\n";
    return nullptr;
}

std::unique_ptr<IResult> RedisDatabase::exec_params(
    const std::string& sql, const std::vector<std::any>& args) {
    return nullptr;
}
