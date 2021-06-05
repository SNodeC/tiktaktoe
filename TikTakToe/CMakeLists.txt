cmake_minimum_required(VERSION 3.5)

configure_file(config.h.in config.h @ONLY)

project(TikTakToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SnodeC REQUIRED)
find_package(OpenSSL REQUIRED)

add_compile_options(-Wno-psabi)

add_executable(tiktaktoe tiktaktoe.cpp TikTakToeSubProtocolInterface.cpp TikTakToeSubProtocol.cpp TikTakToeGameModel.cpp)

target_include_directories(tiktaktoe PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${SNODEC_EXPRESS_INCLUDES}
    ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(tiktaktoe PUBLIC
    ${SNODEC_EXPRESS_LIBRARIES}
    ${OPENSSL_LIBRARIES}
)
