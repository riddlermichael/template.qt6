set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
if(NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif()

option(VERBOSE "Enable additional output for developer" OFF)

# see https://doc.qt.io/qt-6/cmake-get-started.html
# find Qt
set(QT_COMPONENTS
    Core
    Widgets)
find_package(Qt6 REQUIRED COMPONENTS ${QT_COMPONENTS})
# see https://doc.qt.io/qt-6/cmake-qt5-and-qt6-compatibility.html#pitfalls-when-using-versionless-targets
if(${VERBOSE})
    foreach(X ${QT_COMPONENTS})
        if(TARGET Qt6::${X})
            get_target_property(LOC Qt6::${X} LOCATION)
            message(STATUS "Found Qt6::${X} at ${LOC}")
        endif()
    endforeach()
endif()

# see https://doc.qt.io/qt-6/qt-standard-project-setup.html
qt_standard_project_setup()

if(WIN32)
    get_target_property(QT_QMAKE_LOCATION Qt::qmake IMPORTED_LOCATION)
    execute_process(
        COMMAND "${QT_QMAKE_LOCATION}" -query QT_INSTALL_PREFIX
        OUTPUT_VARIABLE QT_INSTALL_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(WIN_DEPLOY_QT_LOCATION "${QT_INSTALL_PREFIX}/bin/windeployqt.exe")
    if(EXISTS "${WIN_DEPLOY_QT_LOCATION}")
        if(${VERBOSE})
            message(STATUS "Found windeployqt at ${WIN_DEPLOY_QT_LOCATION}")
        endif()
    else()
        message(FATAL_ERROR "windeployqt not found")
    endif()
    add_executable(Qt::windeployqt IMPORTED)
    set_target_properties(Qt::windeployqt PROPERTIES IMPORTED_LOCATION "${WIN_DEPLOY_QT_LOCATION}")
endif()

# app
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(awesome)
