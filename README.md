# DbFactory

A small, modern C++17 database factory and RAII connection manager with a pluggable architecture. It provides a common `IDatabase` interface, a `DatabaseFactory` for creating concrete database clients by type, and a `DatabaseManager` that automatically opens/closes connections.

This repository currently builds a reusable library target named `DbFactory`. A small usage example is included in `src/main.cpp_` that you can adapt into your own executable.

## Features
- **Pluggable factory**: Register database types at runtime via `DatabaseFactory::register_database`.
- **RAII connection manager**: `DatabaseManager` opens on construction and closes on destruction.
- **Unified interface**: All databases implement `IDatabase` with `connect`, `disconnect`, `exec`, and `exec_params`.
- **PostgreSQL support (real)**: Backed by `libpqxx`, with transactions and typed row/result helpers.
- **SQLite, MySQL, Redis (mock/demo)**: Lightweight illustrative implementations for demos and extension examples.

## Supported database types
- `postgresql` and `postgres` (alias) — real implementation using `libpqxx`
- `sqlite` — illustrative, in-memory by default
- `mysql` — illustrative
- `redis` — illustrative

Note: Only PostgreSQL performs real queries using `libpqxx`. The other backends simulate connections and query execution for demonstration; they print actions to stdout and often return `nullptr` for results.

## Repository layout
- `src/IDatabase.h` — common database interface and `IResult`
- `src/DatabaseFactory.h|.cpp` — registration-based factory
- `src/DatabaseManager.h|.cpp` — RAII manager wrapper
- `src/DatabaseConfig.h` — simple configuration struct
- `src/PostgreDatabase.h|.cpp` — PostgreSQL implementation using `libpqxx`
- `src/SQLiteDatabase.h|.cpp` — demo implementation
- `src/MySQLDatabase.h|.cpp` — demo implementation
- `src/RedisDatabase.h|.cpp` — demo implementation
- `src/Errors.h|.cpp` — exception types
- `src/main.cpp_` — example program (not built by default)

## Requirements
- CMake ≥ 3.16
- A C++17 compiler (GCC 9+, Clang 10+, MSVC 2019+)
- pkg-config
- PostgreSQL client libraries and `libpqxx` (required to build the library)

### Install dependencies
- Ubuntu/Debian:
```bash
sudo apt update
sudo apt install -y build-essential cmake pkg-config libpq-dev libpqxx-dev
```
- Fedora:
```bash
sudo dnf install -y gcc-c++ cmake pkgconf-pkg-config libpq-devel libpqxx-devel
```
- macOS (Homebrew):
```bash
brew install cmake pkg-config libpqxx
```

## Build
```bash
git clone https://github.com/ehsanrashid/DbFactory.git
cd DbFactory
mkdir build && cd build
cmake ..
cmake --build .
# optionally install
sudo cmake --install .
```
This builds the library target `DbFactory`. Headers are installed to `include/` and the library to `lib/` if you run the install step.

## Using the library in your project
The recommended way is to add this repo as a subdirectory and link against the target:

```cmake
# CMakeLists.txt in your application
add_subdirectory(DbFactory)
add_executable(my_app src/main.cpp)
target_link_libraries(my_app PRIVATE DbFactory)
```

If you installed the library system-wide, you can instead manually set include and link paths (a dedicated CMake package config is not yet provided):
- Add `${CMAKE_INSTALL_PREFIX}/include` to your include dirs
- Link `${CMAKE_INSTALL_PREFIX}/lib/libDbFactory.{a,so,dylib}` and `libpqxx`

## Quick start
Minimal example using PostgreSQL:
```cpp
#include "DatabaseFactory.h"
#include "DatabaseManager.h"

int main() {
  DatabaseFactory::initialize();

  DatabaseConfig cfg("localhost", 5432, "postgres", "postgres", "password");
  auto db = DatabaseFactory::create("postgresql", cfg);

  DatabaseManager mgr(std::move(db));
  mgr->exec("CREATE TABLE IF NOT EXISTS users(id INT, name TEXT)");
  mgr->exec("INSERT INTO users VALUES (1, 'John Doe')");
  mgr->exec("SELECT * FROM users WHERE id = 1");
}
```

A more complete demo is in `src/main.cpp_`. If you want to build and run it, create an executable target in your local CMakeLists and link `DbFactory`:
```cmake
add_executable(demo src/main.cpp_)  # note the underscore file name in this repo
target_link_libraries(demo PRIVATE DbFactory)
```

## API overview
- **`struct DatabaseConfig`** (`src/DatabaseConfig.h`)
  - Fields: `host`, `port`, `database`, `filepath`, `username`, `password`
  - Defaults: `host="localhost"`, `port=0`, `database=""`, `filepath=""`
  - Factory defaults if `port == 0`:
    - `postgresql`: 5432, db `postgres` if empty
    - `mysql`: 3306
    - `redis`: 6379
    - `sqlite`: `":memory:"` if `filepath` empty

- **`class DatabaseFactory`** (`src/DatabaseFactory.h|.cpp`)
  - `static void initialize()` — registers built-in types
  - `static bool supported(const std::string& type)`
  - `static std::vector<std::string> available_types()`
  - `static std::unique_ptr<IDatabase> create(const std::string& type, const DatabaseConfig& cfg = {})`
  - `static void register_database(const std::string& type, Creator)` — register custom creators

- **`class DatabaseManager`** (`src/DatabaseManager.h|.cpp`)
  - RAII wrapper around `std::unique_ptr<IDatabase>`
  - Connects in constructor, disconnects in destructor
  - Operators: `operator->`, `operator*`, `get()`, `valid()`

- **`class IDatabase`** (`src/IDatabase.h`)
  - `std::string connection_info() const noexcept`
  - `bool connected() const noexcept`
  - `void connect()` / `void disconnect()`
  - `std::unique_ptr<IResult> exec(const std::string& sql)`
  - `std::unique_ptr<IResult> exec_params(const std::string& sql, const std::vector<std::any>& args)`

- **PostgreSQL extras** (`src/PostgreDatabase.h|.cpp`)
  - `PostgreTransaction begin_transaction()` with `commit()`/`abort()`
  - `PostgreResult` with iteration, `front()`, `size()`, `columns()`, `affected_rows()`, `column_name()`
  - `PostgreRow` with typed getters: `get<T>(index|name)`, `get_optional<T>()`, `is_null()`
  - Helpers: `table_exists(name)`, `get_columns(table)`, `insert(table, columns, values...)`

## Extending with a custom database
Register any type at runtime:
```cpp
DatabaseFactory::register_database(
  "mydb",
  [](const DatabaseConfig& cfg) -> std::unique_ptr<IDatabase> {
    class MyDb : public IDatabase { /* implement methods */ };
    return std::make_unique<MyDb>();
  }
);
```
After registration, `DatabaseFactory::create("mydb", cfg)` will produce your implementation.

## Error handling
Exceptions derive from `DatabaseError` (`src/Errors.h`):
- `ConnectionError` — connection/open/close issues
- `QueryError` — query/transaction issues

Catch `std::exception` (or `DatabaseError`) around operations.

## Notes & limitations
- Only the PostgreSQL backend performs real database operations (via `libpqxx`).
- The `sqlite`, `mysql`, and `redis` backends are illustrative; `exec_params` may return `nullptr` and no real driver is linked.
- The library ships as a single CMake target `DbFactory`; no CMake package config (`find_package(DbFactory)`) is provided yet.
- The example file is named `src/main.cpp_` to avoid being built by default. Rename to `main.cpp` or add a custom executable target if you want to build it.
