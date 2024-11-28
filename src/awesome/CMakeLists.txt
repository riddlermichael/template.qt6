set(AWESOME_HOME ${ROOT_DIR} CACHE PATH "Awesome home" FORCE)
set(AWESOME_ROOT_DIR "${AWESOME_HOME}/awesome" CACHE PATH "Awesome root" FORCE)
set(PRODUCT_NAME awesome)

# generate version file
set(GENERATED_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include")
set(VERSION_FILE "${GENERATED_INCLUDE_DIR}/${PRODUCT_NAME}/version.hpp")
configure_file(version.hpp.in "${VERSION_FILE}")

# collect sources
file(GLOB_RECURSE HEADERS "*.hpp")
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE UIS "*.ui")

# create app
set(APP ${PROJECT_NAME})
add_executable(${APP} ${SOURCES} ${HEADERS} ${UIS})
target_include_directories(${APP} PUBLIC ${AWESOME_HOME})
target_link_libraries(${APP} PUBLIC Qt5::Core Qt5::Widgets)

include(GNUInstallDirs)

if(WIN32)
    set(CMAKE_INSTALL_BINDIR ".")
    set(CMAKE_INSTALL_LIBDIR ".")
    set(CMAKE_INSTALL_DATADIR ".")

    set_target_properties(${APP} PROPERTIES WIN32_EXECUTABLE ON)
endif()

if(UNIX)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

    target_link_options(${APP} PUBLIC "-Wl,--disable-new-dtags")
    target_link_options(${APP} PUBLIC "-Wl,-z,origin")
endif()

if(WIN32)
    # deploy Qt libs
    # if Qt's lib folder is not in PATH, set DEPLOY_QT_ON_BUILD to ON
    option(DEPLOY_QT_ON_BUILD "Deploy Qt on build")
    if(${VERBOSE})
        dump(DEPLOY_QT_ON_BUILD)
    endif()

    if(${DEPLOY_QT_ON_BUILD})
        set(BUILD_DEPLOY_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
        set(BUILD_DEPLOY_OPTS --no-compiler-runtime --no-angle --no-opengl-sw --no-system-d3d-compiler --no-translations)
        add_custom_command(TARGET ${APP} POST_BUILD
            COMMAND Qt5::windeployqt ${BUILD_DEPLOY_OPTS} --dir "${BUILD_DEPLOY_DIR}" "$<TARGET_FILE:${APP}>"
            COMMENT "Deploying Qt on build")
    endif()

    # deploy app
    option(DEPLOY_APP "Deploy app")
    if(DEFINED ENV{CI})
        set(DEPLOY_APP ON)
    endif()
    if(${VERBOSE})
        dump(DEPLOY_APP)
    endif()

    if(${DEPLOY_APP})
        set(DEPLOY_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/deploy" CACHE PATH "Deploy dir")
        set(DEPLOY_OPTS --compiler-runtime --no-angle --no-opengl-sw --no-system-d3d-compiler --no-translations)
        if(DEFINED ENV{CI})
            list(APPEND DEPLOY_OPTS --release) # force release versions of Qt libs
        endif()
        if(${VERBOSE})
            dump(DEPLOY_OPTS)
        endif()
        add_custom_command(TARGET ${APP} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${DEPLOY_DIR}"
            COMMAND Qt5::windeployqt ${DEPLOY_OPTS} --dir "${DEPLOY_DIR}" "$<TARGET_FILE:${APP}>"
            COMMENT "Deploying Qt")
    endif()
endif()
