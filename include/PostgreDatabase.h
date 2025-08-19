#pragma once

#include <optional>
#include <pqxx/pqxx>

#include "IDatabase.h"

// Forward declarations
class PostgreRow;
class PostgreResult;
class PostgreTransaction;
class PostgreDatabase;

// PostgreRow class - represents a single row from query results
class PostgreRow {
   public:
    explicit PostgreRow(const pqxx::row& row);

    // Get value by column index
    template <typename T>
    T get(int col) const;

    // Get value by column name
    template <typename T>
    T get(const std::string& colName) const;

    // Get optional value (returns nullopt if NULL)
    template <typename T>
    std::optional<T> get_optional(int col) const;

    template <typename T>
    std::optional<T> get_optional(const std::string& colName) const;

    // Check if column is NULL
    bool is_null(int col) const;

    bool is_null(const std::string& colName) const;

    // Get number of columns
    size_t size() const;

    // // Column name access
    // std::string column_name(size_t col) const;

   private:
    pqxx::row _row;
};

// PostgreResult class - represents query results
class PostgreResult : public IResult {
   public:
    explicit PostgreResult(const pqxx::result& result);

    // Iterator support
    class iterator {
       public:
        explicit iterator(pqxx::result::const_iterator itr);

        PostgreRow operator*() const;

        bool operator==(const iterator& itr) const;
        bool operator!=(const iterator& itr) const;

        iterator& operator++();
        iterator operator++(int);

       private:
        pqxx::result::const_iterator _itr;
    };

    iterator begin() const;
    iterator end() const;

    // Access rows
    PostgreRow operator[](size_t rowNum) const;

    PostgreRow at(size_t rowNum) const;

    // Get first row (throws if empty)
    PostgreRow front() const;

    // Get first row as optional
    std::optional<PostgreRow> front_optional() const;

    // Result properties
    size_t size() const;
    bool empty() const;
    size_t columns() const;
    size_t affected_rows() const;

    // Column information
    std::string column_name(size_t col) const;

    // Convert all rows to vector
    template <typename T>
    std::vector<T> to_vector(
        std::function<T(const PostgreRow&)> converter) const;

   private:
    pqxx::result _result;
};

// PostgreTransaction class
class PostgreTransaction {
   public:
    explicit PostgreTransaction(pqxx::connection& conn);

    ~PostgreTransaction();

    // Execute query
    PostgreResult exec(const std::string& sql);

    // Execute parameterized query
    template <typename... Args>
    PostgreResult exec_params(const std::string& sql, Args&&... args);

    // Execute prepared statement
    template <typename... Args>
    PostgreResult exec_prepared(const std::string& name, Args&&... args);

    // Commit transaction
    void commit();

    // Abort transaction
    void abort();

    // Quote and escape values
    std::string quote(const std::string& value);

    std::string quote_name(const std::string& name);

   private:
    std::unique_ptr<pqxx::work> _txn;
    bool _committed;
};

// PostgreSQL Database implementation
class PostgreDatabase : public IDatabase {
   public:
    // Constructor with connection string
    explicit PostgreDatabase(const std::string& connectionString) noexcept;
    PostgreDatabase(const std::string& host, int port,
                    const std::string& database, const std::string& username,
                    const std::string& password) noexcept;
    PostgreDatabase(const std::string& host, int port = 5432,
                    const std::string& database = "postgres") noexcept;

    // Get connection info
    std::string port() const noexcept;
    std::string hostname() const noexcept;
    std::string dbname() const noexcept;
    std::string username() const noexcept;

    std::string connection_info() const noexcept override;

    bool connected() const noexcept override;

    void connect() override;

    void disconnect() override;

    // Create transaction
    PostgreTransaction begin_transaction();

    // Execute query without transaction (auto-commit)
    IResult exec(const std::string& sql) override;

    // Execute parameterized query without transaction
    IResult exec_params(const std::string& sql,
                        const std::vector<std::any>& args) override;

    // Execute parameterized query without transaction
    template <typename... Args>
    IResult exec_params(const std::string& sql, Args&&... args);

    // Utility methods for common operations

    // Check if table exists
    bool table_exists(const std::string& tableName);

    // Get table column names
    std::vector<std::string> get_columns(const std::string& tableName);

    // Simple insert helper
    template <typename... Args>
    void insert(const std::string& table,
                const std::vector<std::string>& columns, Args&&... values);

   private:
    std::string _connectionString;
    std::unique_ptr<pqxx::connection> _conn;
};
