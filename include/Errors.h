#pragma once

#include <stdexcept>
#include <string>

// Exception classes
class DatabaseError : public std::runtime_error {
   public:
    explicit DatabaseError(const std::string& msg);
};

class ConnectionError final : public DatabaseError {
   public:
    explicit ConnectionError(const std::string& msg);
};

class QueryError final : public DatabaseError {
   public:
    explicit QueryError(const std::string& msg);
};
