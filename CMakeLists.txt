cmake_minimum_required(VERSION 3.16)

# Project setup
set(PROJECT_NAME DbFactory)
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Find PostgreSQL first (required by libpqxx)
find_package(PostgreSQL REQUIRED)

# Find libpqxx
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

# Collect source files
file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)
# Collect header files
file(GLOB HEADERS CONFIGURE_DEPENDS src/*.h)

# Create a single executable
#add_executable(${PROJECT_NAME} ${SOURCES})
# Create a library
add_library(${PROJECT_NAME} ${SOURCES})

# Include headers
target_include_directories(${PROJECT_NAME} PRIVATE
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
