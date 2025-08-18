#pragma once

#include <string>

// Database interface
class IDatabase {
   public:
    virtual ~IDatabase() = default;

    virtual std::string connection_info() const noexcept = 0;
    virtual bool connected() const noexcept = 0;

    virtual void connect() noexcept = 0;
    virtual void disconnect() noexcept = 0;
    virtual void query(const std::string& sql) = 0;
};
