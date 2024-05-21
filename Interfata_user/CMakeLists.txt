# Ensure minimum CMake version
cmake_minimum_required(VERSION 3.5)

# Project setup
project(DocumenteBatalionApp VERSION 0.1 LANGUAGES CXX)

# Enable Qt's automatic processing features
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt components, including Widgets and SQL
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Project source files
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Create executable depending on Qt version
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DocumenteBatalionApp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        meniu_studenti.h

    )
else()
    add_executable(DocumenteBatalionApp
        ${PROJECT_SOURCES}
    )
endif()

# Link required Qt components (including SQL)
target_link_libraries(DocumenteBatalionApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Set project properties for macOS/iOS and Windows
set_target_properties(DocumenteBatalionApp PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Set installation directories
install(TARGETS DocumenteBatalionApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize executable if using Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DocumenteBatalionApp)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Network)
target_link_libraries(DocumenteBatalionApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Network)
