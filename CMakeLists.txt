cmake_minimum_required(VERSION 3.5)

project(TikTakToe LANGUAGES CXX)

find_package(
    snodec COMPONENTS http-server-express websocket-server net-in-stream-legacy
                      net-in-stream-tls
)

get_target_property(
    WEBSOCKET_SUBPROTOCOL_SERVER_INSTALL_LIBDIR snodec::websocket-server
    WEBSOCKET_SUBPROTOCOL_SERVER_INSTALL_LIBDIR
)

get_target_property(
    WEBSOCKET_SUBPROTOCOL_SERVER_INCLUDE_DIRS snodec::websocket-server
    WEBSOCKET_SUBPROTOCOL_SERVER_INCLUDE_DIRS
)

if(snodec_FOUND)
    configure_file(config.h.in config.h @ONLY)

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    find_program(iwyu_path NAMES include-what-you-use iwyu)

    if(iwyu_path)
        option(CHECK_INCLUDES "Check used headers")

        set(iwyu_path_and_options
            ${iwyu_path}
            -Xiwyu
            --verbose=1
            -Xiwyu
            --cxx17ns
            -Xiwyu
            --quoted_includes_first
            -Xiwyu
            --check_also='${PROJECT_SOURCE_DIR}/*'
        )

        if(CHECK_INCLUDES)
            set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path_and_options})
        endif(CHECK_INCLUDES)
    else()
        message(
            AUTHOR_WARNING "Could not find the program include-what-you-use"
        )
    endif()

    add_compile_options(
        -Werror
        -Wall
        -Wextra
        -Wno-psabi # needed for RaspberryPi
        -Wconversion
        -Wpedantic
        -pedantic-errors
        -fexec-charset=UTF-8
        # -fsanitize=address -fno-omit-frame-pointer
    )
    # set_property(GLOBAL PROPERTY LINK_FLAGS -fsanitize=address)
    # link_libraries(asan)

    set(TIKTAKTOE_SUBPROTOCOL_SOURCES
        TikTakToeSubProtocolFactory.cpp TikTakToeSubProtocol.cpp
        TikTakToeGameModel.cpp
    )

    # ##########################################################################
    #
    # Subprotocol as library
    #
    # ##########################################################################

    add_library(tiktaktoeplugin SHARED ${TIKTAKTOE_SUBPROTOCOL_SOURCES})

    set_target_properties(
        tiktaktoeplugin PROPERTIES OUTPUT_NAME "snodec-websocket-tiktaktoe"
                                   SOVERSION 1 # PREFIX "ssp"
    )

    target_include_directories(
        tiktaktoeplugin PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                                ${WEBSOCKET_SUBPROTOCOL_SERVER_INCLUDE_DIRS}
    )

    install(TARGETS tiktaktoeplugin
            LIBRARY DESTINATION ${WEBSOCKET_SUBPROTOCOL_SERVER_INSTALL_LIBDIR}
    )

    # ##########################################################################
    #
    # Inline linked target
    #
    # ##########################################################################

    add_executable(
        tiktaktoe-inline tiktaktoe.cpp ${TIKTAKTOE_SUBPROTOCOL_SOURCES}
    )

    target_compile_definitions(tiktaktoe-inline PRIVATE LINK_SUBPROTOCOL_STATIC)

    target_include_directories(
        tiktaktoe-inline PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
    )

    target_link_libraries(
        tiktaktoe-inline
        PUBLIC snodec::http-server-express snodec::websocket-server
               snodec::net-in-stream-legacy snodec::net-in-stream-tls
    )

    install(TARGETS tiktaktoe-inline RUNTIME DESTINATION bin)

    # ##########################################################################
    #
    # Full dynamically linked target
    #
    # ##########################################################################

    add_executable(tiktaktoe-dynamic tiktaktoe.cpp)

    target_compile_definitions(
        tiktaktoe-dynamic PRIVATE LINK_SUBPROTOCOL_STATIC
    )

    target_include_directories(
        tiktaktoe-dynamic PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
    )

    target_link_libraries(
        tiktaktoe-dynamic
        PUBLIC snodec::http-server-express snodec::websocket-server
               snodec::net-in-stream-legacy snodec::net-in-stream-tls
               tiktaktoeplugin
    )

    target_link_directories(
        tiktaktoe-dynamic PRIVATE
        ${WEBSOCKET_SUBPROTOCOL_SERVER_INSTALL_LIBDIR}
    )

    install(TARGETS tiktaktoe-dynamic RUNTIME DESTINATION bin)

    # ##########################################################################
    #
    # Half dynamically linked target - tiktaktoe library must be dlopened
    #
    # ##########################################################################

    add_executable(tiktaktoe-half tiktaktoe.cpp)

    target_compile_definitions(tiktaktoe-half PRIVATE LINK_WEBSOCKET_STATIC)

    target_include_directories(
        tiktaktoe-half PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
    )

    target_link_libraries(
        tiktaktoe-half
        PUBLIC snodec::http-server-express snodec::websocket-server
               snodec::net-in-stream-legacy snodec::net-in-stream-tls
    )

    install(TARGETS tiktaktoe-half RUNTIME DESTINATION bin)

    # ##########################################################################
    #
    # Non linked target - websocket and tiktaktoe libraries must be dlopened
    #
    # ##########################################################################

    add_executable(tiktaktoe tiktaktoe.cpp)

    target_compile_definitions(tiktaktoe PRIVATE)

    target_include_directories(
        tiktaktoe PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
    )

    target_link_libraries(
        tiktaktoe PUBLIC snodec::http-server-express
                         snodec::net-in-stream-legacy snodec::net-in-stream-tls
    )

    install(TARGETS tiktaktoe RUNTIME DESTINATION bin)

    add_custom_target(format DEPENDS format-cmds)
    add_custom_command(
        OUTPUT format-cmds
        COMMENT "Auto formatting of all source and all cmake files"
    )

    include("cmake/clang-cpp-checks.cmake")
    include("cmake/cmake-format.cmake")

    add_custom_target(
        uninstall
        COMMAND sudo xargs rm -v < install_manifest.txt
        COMMAND sudo xargs -L1 dirname < install_manifest.txt | sort | uniq |
                sudo xargs rmdir -v --ignore-fail-on-non-empty -p
        COMMENT "Uninstall project"
    )
endif(snodec_FOUND)

# xargs rm < install_manifest.txt 140
