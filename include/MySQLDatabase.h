#pragma once

#include "IDatabase.h"

// MySQL Database implementation
class MySQLDatabase : public IDatabase {
   public:
    MySQLDatabase(const std::string& host = "localhost",
                  int port = 3306) noexcept;

    std::string connection_info() const noexcept override;

    bool connected() const noexcept override;

    void connect() override;

    void disconnect() override;

    IResult exec(const std::string& sql) override;

    IResult exec_params(const std::string& sql,
                        const std::vector<std::any>& args) override;

   private:
    std::string _host;
    int _port;
    bool _connected;
};
