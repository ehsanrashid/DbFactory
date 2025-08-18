#include "RedisDatabase.h"

#include <iostream>
#include <stdexcept>

RedisDatabase::RedisDatabase(const std::string& host, int port)
    : _host(host), _port(port), _connected(false) {}

std::string RedisDatabase::get_connection_info() const noexcept {
    return "Redis at " + _host + ":" + std::to_string(_port);
}

bool RedisDatabase::connected() const noexcept { return _connected; }

void RedisDatabase::connect() noexcept {
    std::cout << "[Redis] Connecting to " << _host << ":" << _port << "\n";
    _connected = true;
}

void RedisDatabase::disconnect() noexcept {
    if (_connected) {
        std::cout << "[Redis] Disconnecting\n";
        _connected = false;
    }
}

void RedisDatabase::query(const std::string& sql) {
    if (!_connected) {
        throw std::runtime_error("[Redis] Database not connected");
    }
    std::cout << "[Redis] Executing query: " << sql << "\n";
    // Simulate query execution
    std::cout << "[Redis] Query executed successfully\n";
}
