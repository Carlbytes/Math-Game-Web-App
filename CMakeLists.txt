cmake_minimum_required(VERSION 3.15)
project(CrowMainMenu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Crow depends on Asio and Threads
find_package(asio CONFIG REQUIRED)
find_package(Threads REQUIRED)

add_executable(CrowMainMenu main.cpp)

add_custom_command(TARGET CrowMainMenu POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:CrowMainMenu>/www
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/www $<TARGET_FILE_DIR:CrowMainMenu>/www
)


# Add Crow headers manually (header-only)
target_include_directories(CrowMainMenu PRIVATE
        ${VCPKG_INSTALLED_DIR}/x64-windows/include
)

# Link Asio and Threads (targets, not include paths)
target_link_libraries(CrowMainMenu PRIVATE asio::asio Threads::Threads)

