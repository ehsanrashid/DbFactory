#pragma once

#include "IDatabase.h"

class RedisDatabase : public IDatabase {
   public:
    RedisDatabase(const std::string& host, int port);

    std::string connection_info() const noexcept override;

    bool connected() const noexcept override;

    void connect() noexcept override;

    void disconnect() noexcept override;

    void query(const std::string& sql) override;

   private:
    std::string _host;
    int _port;
    bool _connected;
};
