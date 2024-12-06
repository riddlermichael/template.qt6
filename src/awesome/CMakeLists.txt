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
# see https://doc.qt.io/qt-6/qt-wrap-ui.html
file(GLOB_RECURSE UIS "*.ui")

# collect resources
set(RESOURCES_ROOT "${PROJECT_SOURCE_DIR}/resources")
set(RESOURCES "${RESOURCES_ROOT}/main.qrc")

# create app
set(APP ${PROJECT_NAME})
# see https://doc.qt.io/qt-6/resources.html
set(CMAKE_AUTORCC ON)
# see https://doc.qt.io/qt-6/qt-add-executable.html
# see https://doc.qt.io/qt-6/qt-add-ui.html (since Qt 6.8)
qt_add_executable(${APP} ${SOURCES} ${HEADERS} ${UIS} ${RESOURCES})
target_include_directories(${APP} PUBLIC ${AWESOME_HOME})
target_link_libraries(${APP} PUBLIC Qt::Widgets)
if(WIN32)
    set_target_properties(${APP} PROPERTIES WIN32_EXECUTABLE ON)
endif()

# another approach is compilation of binary resources into source code; remove this if-block in production!
if(FALSE)
    # see https://doc.qt.io/qt-6/qt-add-resources.html
    qt_add_resources(RESOURCES_CPP "${RESOURCES}")
    qt_add_executable(${APP} ${SOURCES} ${HEADERS} ${UIS} ${RESOURCES_CPP})
endif()

# deploy
if(WIN32)
    # deploy Qt libs
    # if Qt's lib folder is not in PATH, set DEPLOY_QT_ON_BUILD to ON
    option(DEPLOY_QT_ON_BUILD "Deploy Qt on build")
    if(${VERBOSE})
        dump(DEPLOY_QT_ON_BUILD)
    endif()

    if(${DEPLOY_QT_ON_BUILD})
        set(BUILD_DEPLOY_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
        set(BUILD_DEPLOY_OPTS --no-compiler-runtime --no-opengl-sw --no-system-d3d-compiler --no-translations)
        add_custom_command(TARGET ${APP} POST_BUILD
            COMMAND Qt::windeployqt ${BUILD_DEPLOY_OPTS} --dir "${BUILD_DEPLOY_DIR}" "$<TARGET_FILE:${APP}>"
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
        set(DEPLOY_OPTS --compiler-runtime --no-opengl-sw --no-system-d3d-compiler --no-translations)
        if(DEFINED ENV{CI})
            list(APPEND DEPLOY_OPTS --release) # force release versions of Qt libs
        endif()
        if(${VERBOSE})
            dump(DEPLOY_OPTS)
        endif()
        add_custom_command(TARGET ${APP} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${DEPLOY_DIR}"
            COMMAND Qt::windeployqt ${DEPLOY_OPTS} --dir "${DEPLOY_DIR}" "$<TARGET_FILE:${APP}>"
            COMMENT "Deploying Qt")
    endif()
endif()
