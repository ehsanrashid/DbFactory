#pragma once

#include <any>
#include <memory>
#include <string>
#include <vector>

class IResult {
   public:
    virtual ~IResult() = default;
};

// Database interface
class IDatabase {
   public:
    virtual ~IDatabase() = default;

    virtual std::string connection_info() const noexcept = 0;
    virtual bool connected() const noexcept = 0;

    // Opem connection
    virtual void connect() = 0;
    // Close connection
    virtual void disconnect() = 0;

    // Execute query without transaction (auto-commit)
    virtual std::unique_ptr<IResult> exec(const std::string& sql) = 0;
    // Execute parameterized query without transaction
    virtual std::unique_ptr<IResult> exec_params(
        const std::string& sql, const std::vector<std::any>& args) = 0;
};
