cmake_minimum_required(VERSION 3.5)

configure_file(config.h.in config.h @ONLY)

project(TikTakToe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

find_package(
  snodec
  COMPONENTS logger
             utils
             net
             web
             express
             websocketbase
             websocketserver)

find_program(iwyu_path NAMES include-what-you-use iwyu)
if(NOT iwyu_path)
  message(FATAL_ERROR "Could not find the program include-what-you-use")
endif()

option(CHECK_INCLUDES "Check used headers")

set(iwyu_path_and_options ${iwyu_path} -Xiwyu --verbose=1)

add_compile_options(-Wno-psabi)

add_executable(tiktaktoe tiktaktoe.cpp TikTakToeSubProtocolFactory.cpp
                         TikTakToeSubProtocol.cpp TikTakToeGameModel.cpp)

target_include_directories(tiktaktoe PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(tiktaktoe PUBLIC snodec::websocketserver snodec::express)

install(TARGETS tiktaktoe RUNTIME DESTINATION bin)

if(CHECK_INCLUDES)
  set_property(TARGET tiktaktoe PROPERTY CXX_INCLUDE_WHAT_YOU_USE
                                         ${iwyu_path_and_options})
endif(CHECK_INCLUDES)

add_custom_target(format DEPENDS format-cmds)
add_custom_command(OUTPUT format-cmds
                   COMMENT "Auto formatting of all source and all cmake files")

include("cmake/clang-cpp-checks.cmake")
include("cmake/cmake-format.cmake")
