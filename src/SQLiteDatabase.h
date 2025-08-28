#pragma once

#include "IDatabase.h"

// SQLite Database implementation
class SQLiteDatabase : public IDatabase {
   public:
    SQLiteDatabase(const std::string& filepath = ":memory:") noexcept;

    std::string connection_info() const noexcept override;

    bool connected() const noexcept override;

    void connect() override;

    void disconnect() override;

    std::unique_ptr<IResult> exec(const std::string& sql) override;

    std::unique_ptr<IResult> exec_params(
        const std::string& sql, const std::vector<std::any>& args) override;

   private:
    std::string _filepath;
    bool _connected;
};
