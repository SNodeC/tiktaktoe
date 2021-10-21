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
        message(FATAL_ERROR "Could not find the program include-what-you-use")
    endif()

    add_compile_options(-Wno-psabi)

    option(
        STATIC_LINKING
        "Link subprotocoll plugin to executable instead of using it as a dlopened plugin"
        ON
    )

    if(STATIC_LINKING)
        add_executable(
            tiktaktoe tiktaktoe.cpp TikTakToeSubProtocolFactory.cpp
                      TikTakToeSubProtocol.cpp TikTakToeGameModel.cpp
        )

        get_target_property(
            INCLUDES snodec::websocketserver INTERFACE_INCLUDE_DIRECTORIES
        )

        target_include_directories(
            tiktaktoe PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
        )

        target_link_libraries(
            tiktaktoe PUBLIC snodec::express snodec::websocketserver
        )

        target_compile_definitions(tiktaktoe PRIVATE LINK_STATIC)
    else(STATIC_LINKING)
        add_executable(tiktaktoe tiktaktoe.cpp)

        get_target_property(
            INCLUDES snodec::websocketserver INTERFACE_INCLUDE_DIRECTORIES
        )

        target_include_directories(
            tiktaktoe PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
        )

        target_link_libraries(tiktaktoe PUBLIC snodec::express)

        add_library(
            tiktaktoeplugin SHARED
            TikTakToeSubProtocolFactory.cpp TikTakToeSubProtocol.cpp
            TikTakToeGameModel.cpp
        )

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
    endif(STATIC_LINKING)

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
