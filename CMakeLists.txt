cmake_minimum_required(VERSION 3.16)
project(DbFactory VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Find PostgreSQL first (required by libpqxx)
find_package(PostgreSQL REQUIRED)

# Find libpqxx
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

# Collect source files
file(GLOB SOURCES "src/*.cpp")
# Collect header files
file(GLOB HEADERS "include/*.h")

# Create a library
#add_executable(${PROJECT_NAME} ${SOURCES})
add_library(${PROJECT_NAME} ${SOURCES})

# Include headers
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${PQXX_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${PQXX_LIBRARIES}
)

# Compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE ${PQXX_CFLAGS_OTHER})

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include)
install(FILES ${HEADERS} DESTINATION include/)
