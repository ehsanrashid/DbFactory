#include "Errors.h"

DatabaseError::DatabaseError(const std::string& msg)
    : std::runtime_error(msg) {}

ConnectionError::ConnectionError(const std::string& msg)
    : DatabaseError("Connection error: " + msg) {}

QueryError::QueryError(const std::string& msg)
    : DatabaseError("Query error: " + msg) {}
