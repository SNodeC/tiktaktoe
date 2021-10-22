cmake_minimum_required(VERSION 3.5)

project(TikTakToe LANGUAGES CXX)

find_package(snodec COMPONENTS express websocketserver)

if(snodec_FOUND)
    configure_file(config.h.in config.h @ONLY)

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

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

    add_compile_options(-Wno-psabi)

    set(TIKTAKTOE_SUBPROTOCOL_SOURCES
        TikTakToeSubProtocolFactory.cpp TikTakToeSubProtocol.cpp
        TikTakToeGameModel.cpp
    )

    # ##########################################################################
    #
    # Static linked target
    #
    # ##########################################################################

    add_executable(
        tiktaktoe-static tiktaktoe.cpp ${TIKTAKTOE_SUBPROTOCOL_SOURCES}
    )

    target_compile_definitions(tiktaktoe-static PRIVATE LINK_SUBPROTOCOL_STATIC)

    target_include_directories(
        tiktaktoe-static PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                                 ${SNODEC_SUBPROTOCOL_SERVER_INCLUDE_DIRS}
    )

    target_link_libraries(
        tiktaktoe-static PUBLIC snodec::express snodec::websocketserver
    )

    install(TARGETS tiktaktoe-static RUNTIME DESTINATION bin)

    # ##########################################################################
    #
    # Half-static linked target
    #
    # ##########################################################################

    add_executable(tiktaktoe-half-static tiktaktoe.cpp)

    target_compile_definitions(
        tiktaktoe-half-static PRIVATE LINK_WEBSOCKET_STATIC
    )

    target_include_directories(
        tiktaktoe-half-static PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                                      ${SNODEC_SUBPROTOCOL_SERVER_INCLUDE_DIRS}
    )

    target_link_libraries(
        tiktaktoe-half-static PUBLIC snodec::express snodec::websocketserver
    )

    install(TARGETS tiktaktoe-half-static RUNTIME DESTINATION bin)

    # ##########################################################################
    #
    # Dynamic linked target
    #
    # ##########################################################################

    add_executable(tiktaktoe-dynamic tiktaktoe.cpp)

    target_compile_definitions(tiktaktoe-dynamic PRIVATE)

    target_include_directories(
        tiktaktoe-dynamic PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
    )

    target_link_libraries(tiktaktoe-dynamic PUBLIC snodec::express)

    install(TARGETS tiktaktoe-dynamic RUNTIME DESTINATION bin)

    # ##########################################################################
    #
    # Subprotocol as plugin
    #
    # ##########################################################################

    add_library(tiktaktoeplugin SHARED ${TIKTAKTOE_SUBPROTOCOL_SOURCES})

    set_target_properties(
        tiktaktoeplugin PROPERTIES OUTPUT_NAME "snodec-websocket-tiktaktoe"
                                   SOVERSION 1 # PREFIX "ssp"
    )

    target_include_directories(
        tiktaktoeplugin PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
                                ${SNODEC_SUBPROTOCOL_SERVER_INCLUDE_DIRS}
    )

    install(TARGETS tiktaktoeplugin
            LIBRARY DESTINATION ${SNODEC_SUBPROTOCOL_SERVER_INSTALL_PATH}
    )

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
