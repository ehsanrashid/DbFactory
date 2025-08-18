cmake_minimum_required(VERSION 3.16)
project(DbFactory VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Collect source files
file(GLOB SOURCE_FILES "src/*.cpp")

# Create a library
add_library(${PROJECT_NAME} ${SOURCE_FILES})

# Include headers
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include   
)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include)
install(FILES ${HEADERS} DESTINATION include/)
