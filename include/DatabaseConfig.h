#pragma once

#include <string>

// Database configuration structure
struct DatabaseConfig {
    DatabaseConfig() = default;
    DatabaseConfig(const std::string& h, int p, const std::string& db = "")
        : host(h), port(p), database(db) {}
    DatabaseConfig(const std::string& fp) : filepath(fp) {}

    std::string host = "localhost";
    int port = 0;
    std::string database = "";
    std::string filepath = "";
};
