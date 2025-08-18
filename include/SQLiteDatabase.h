#pragma once

#include "IDatabase.h"

// SQLite Database implementation
class SQLiteDatabase : public IDatabase {
   public:
    SQLiteDatabase(const std::string& filepath = ":memory:") noexcept;

    std::string connection_info() const noexcept override;

    bool connected() const noexcept override;

    void connect() noexcept override;

    void disconnect() noexcept override;

    void query(const std::string& sql) override;

   private:
    std::string _filepath;
    bool _connected;
};
