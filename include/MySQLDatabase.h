#pragma once

#include "IDatabase.h"

// MySQL Database implementation
class MySQLDatabase : public IDatabase {
   public:
    MySQLDatabase(const std::string& host = "localhost",
                  int port = 3306) noexcept;

    std::string get_connection_info() const noexcept override;

    bool connected() const noexcept override;

    void connect() noexcept override;

    void disconnect() noexcept override;

    void query(const std::string& sql) override;

   private:
    std::string _host;
    int _port;
    bool _connected;
};
