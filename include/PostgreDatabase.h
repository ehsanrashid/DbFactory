#pragma once

#include "IDatabase.h"

// PostgreSQL Database implementation
class PostgreDatabase : public IDatabase {
   public:
    PostgreDatabase(const std::string& host, int port,
                    const std::string& database, const std::string& username,
                    const std::string& password) noexcept;
    PostgreDatabase(const std::string& host = "localhost", int port = 5432,
                    const std::string& database = "postgres") noexcept;

    std::string connection_info() const noexcept override;

    bool connected() const noexcept override;

    void connect() noexcept override;

    void disconnect() noexcept override;

    void query(const std::string& sql) override;

   private:
    std::string _host;
    int _port;
    std::string _database;
    std::string _username;
    std::string _password;
    bool _connected;
};
