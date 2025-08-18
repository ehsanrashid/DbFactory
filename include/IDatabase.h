#pragma once

#include <string>

class IResult {};

// Database interface
class IDatabase {
   public:
    virtual ~IDatabase() = default;

    virtual std::string connection_info() const noexcept = 0;
    virtual bool connected() const noexcept = 0;

    virtual void connect() = 0;
    virtual void disconnect() = 0;

    virtual IResult exec(const std::string& sql) = 0;

    template <typename... Args>
    IResult exec_params(const std::string& sql, Args&&... args);
};
