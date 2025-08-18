#include "PostgreDatabase.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "Errors.h"

PostgreRow::PostgreRow(const pqxx::row& row) : _row(row) {}

// Get value by column index
template <typename T>
T PostgreRow::get(int col) const {
    if (col >= _row.size()) {
        throw std::out_of_range("Column index out of range");
    }
    return _row[col].as<T>();
}

// Get value by column name
template <typename T>
T PostgreRow::get(const std::string& colName) const {
    return _row[colName].as<T>();
}

// Get optional value (returns nullopt if NULL)
template <typename T>
std::optional<T> PostgreRow::get_optional(int col) const {
    if (col >= _row.size()) {
        throw std::out_of_range("Column index out of range");
    }
    return _row[col].is_null() ? std::nullopt
                               : std::make_optional(_row[col].as<T>());
}

template <typename T>
std::optional<T> PostgreRow::get_optional(const std::string& colName) const {
    auto field = _row[colName];
    return field.is_null() ? std::nullopt : std::make_optional(field.as<T>());
}

// Check if column is NULL
bool PostgreRow::is_null(int col) const {
    return col < _row.size() && _row[col].is_null();
}

bool PostgreRow::is_null(const std::string& colName) const {
    return _row[colName].is_null();
}

// Get number of columns
size_t PostgreRow::size() const { return _row.size(); }

// // Column name access
// std::string PostgreRow::column_name(size_t col) const {
//     return _row.column_name(col);
// }

PostgreResult::iterator::iterator(pqxx::result::const_iterator itr)
    : _itr(itr) {}

PostgreRow PostgreResult::iterator::operator*() const {
    return PostgreRow(*_itr);
}

bool PostgreResult::iterator::operator==(
    const PostgreResult::iterator& itr) const {
    return _itr == itr._itr;
}

bool PostgreResult::iterator::operator!=(
    const PostgreResult::iterator& itr) const {
    return _itr != itr._itr;
}

PostgreResult::iterator& PostgreResult::iterator::operator++() {
    ++_itr;
    return *this;
}

PostgreResult::iterator PostgreResult::iterator::operator++(int) {
    auto itr = *this;
    ++_itr;
    return itr;
}

PostgreResult::PostgreResult(const pqxx::result& result) : _result(result) {}

PostgreResult::iterator PostgreResult::begin() const {
    return PostgreResult::iterator(_result.begin());
}
PostgreResult::iterator PostgreResult::end() const {
    return PostgreResult::iterator(_result.end());
}

// Access rows
PostgreRow PostgreResult::operator[](size_t rowNum) const {
    if (rowNum >= _result.size()) {
        throw std::out_of_range("Row index out of range");
    }
    return PostgreRow(_result[rowNum]);
}

PostgreRow PostgreResult::at(size_t rowNum) const { return (*this)[rowNum]; }

// Get first row (throws if empty)
PostgreRow PostgreResult::front() const {
    if (_result.empty()) {
        throw std::runtime_error("Result is empty");
    }
    return PostgreRow(_result.front());
}

// Get first row as optional
std::optional<PostgreRow> PostgreResult::front_optional() const {
    return _result.empty() ? std::nullopt
                           : std::make_optional(PostgreRow(_result.front()));
}

// Result properties
size_t PostgreResult::size() const { return _result.size(); }
bool PostgreResult::empty() const { return _result.empty(); }
size_t PostgreResult::columns() const { return _result.columns(); }
size_t PostgreResult::affected_rows() const { return _result.affected_rows(); }

// Column information
std::string PostgreResult::column_name(size_t col) const {
    return _result.column_name(col);
}

// Convert all rows to vector
template <typename T>
std::vector<T> PostgreResult::to_vector(
    std::function<T(const PostgreRow&)> converter) const {
    std::vector<T> vec;
    vec.reserve(size());
    for (const auto& row : *this) {
        vec.push_back(converter(row));
    }
    return vec;
}

PostgreTransaction::PostgreTransaction(pqxx::connection& conn)
    : _txn(std::make_unique<pqxx::work>(conn)), _committed(false) {}

PostgreTransaction::~PostgreTransaction() {
    if (!_committed) {
        try {
            _txn->abort();
        } catch (...) {
            // Ignore exceptions in destructor
        }
    }
}

// Execute query
PostgreResult PostgreTransaction::exec(const std::string& sql) {
    try {
        return PostgreResult(_txn->exec(sql));
    } catch (const pqxx::sql_error& e) {
        throw QueryError(e.what());
    } catch (const std::exception& e) {
        throw DatabaseError(e.what());
    }
}

// Execute parameterized query
template <typename... Args>
PostgreResult PostgreTransaction::exec_params(const std::string& sql,
                                              Args&&... args) {
    try {
        return PostgreResult(
            _txn->exec_params(sql, std::forward<Args>(args)...));
    } catch (const pqxx::sql_error& e) {
        throw QueryError(e.what());
    } catch (const std::exception& e) {
        throw DatabaseError(e.what());
    }
}

// Execute prepared statement
template <typename... Args>
PostgreResult PostgreTransaction::exec_prepared(const std::string& name,
                                                Args&&... args) {
    try {
        return PostgreResult(
            _txn->exec_prepared(name, std::forward<Args>(args)...));
    } catch (const pqxx::sql_error& e) {
        throw QueryError(e.what());
    } catch (const std::exception& e) {
        throw DatabaseError(e.what());
    }
}

// Commit transaction
void PostgreTransaction::commit() {
    if (_committed) {
        throw std::runtime_error("Transaction already committed");
    }
    try {
        _txn->commit();
        _committed = true;
    } catch (const std::exception& e) {
        throw DatabaseError(e.what());
    }
}

// Abort transaction
void PostgreTransaction::abort() {
    if (!_committed) {
        _txn->abort();
        _committed = true;  // Mark as completed to avoid double-abort
    }
}

// Quote and escape values
std::string PostgreTransaction::quote(const std::string& value) {
    return _txn->quote(value);
}

std::string PostgreTransaction::quote_name(const std::string& name) {
    return _txn->quote_name(name);
}

// Constructor with connection string
PostgreDatabase::PostgreDatabase(const std::string& connectionString) noexcept
    : _connectionString(connectionString) {}

PostgreDatabase::PostgreDatabase(const std::string& host, int port,
                                 const std::string& database,
                                 const std::string& username,
                                 const std::string& password) noexcept {
    std::ostringstream oss;
    oss << "host=" << host << " port=" << (port == 0 ? 5432 : port)
        << " dbname=" << database << " user=" << username
        << " password=" << password;
    _connectionString = oss.str();
}

PostgreDatabase::PostgreDatabase(const std::string& host, int port,
                                 const std::string& database) noexcept {
    std::ostringstream oss;
    oss << "host=" << host << " port=" << (port == 0 ? 5432 : port)
        << " dbname=" << database;
    _connectionString = oss.str();
}

// Get connection info
std::string PostgreDatabase::port() const noexcept { return _conn->port(); }
std::string PostgreDatabase::hostname() const noexcept {
    return _conn->hostname();
}
std::string PostgreDatabase::dbname() const noexcept { return _conn->dbname(); }
std::string PostgreDatabase::username() const noexcept {
    return _conn->username();
}

std::string PostgreDatabase::connection_info() const noexcept {
    return "Postgre Database at " + _connectionString;
}

bool PostgreDatabase::connected() const noexcept {
    return _conn && _conn->is_open();
}

void PostgreDatabase::connect() {
    if (connected()) {
        std::cout << "[Postgre] Already connected\n";
        return;
    }

    try {
        std::cout << "[Postgre] Connecting to " << _connectionString << "\n";
        _conn = std::make_unique<pqxx::connection>(_connectionString);
        std::cout << "[Postgre] Successfully connected\n";
    } catch (const std::exception& e) {
        throw ConnectionError(e.what());
    }
}

void PostgreDatabase::disconnect() {
    if (!connected()) {
        std::cout << "[Postgre] Already disconnected\n";
        return;
    }
    std::cout << "[Postgre] Disconnecting from database\n";
    if (_conn) {
        _conn.reset();  // _conn->close();
    }
    std::cout << "[Postgre] Successfully disconnected\n";
}

// Create transaction
PostgreTransaction PostgreDatabase::begin_transaction() {
    if (!connected()) {
        throw ConnectionError("Connection is not open");
    }

    return PostgreTransaction(*_conn);
}

// Execute query without transaction (auto-commit)
IResult PostgreDatabase::exec(const std::string& sql) {
    if (!connected()) {
        throw ConnectionError("[Postgre] Database not connected");
    }

    try {
        auto txn = begin_transaction();
        auto result = txn.exec(sql);
        txn.commit();
        return result;
    } catch (const std::exception& e) {
        throw QueryError(e.what());
    }
    return IResult{};
}

// Execute parameterized query without transaction
template <typename... Args>
IResult PostgreDatabase::exec_params(const std::string& sql, Args&&... args) {
    if (!connected()) {
        throw ConnectionError("[Postgre] Database not connected");
    }

    try {
        auto txn = begin_transaction();
        auto result = txn.exec_params(sql, std::forward<Args>(args)...);
        txn.commit();
        return result;
    } catch (const std::exception& e) {
        throw QueryError(e.what());
    }
    return IResult{};
}
