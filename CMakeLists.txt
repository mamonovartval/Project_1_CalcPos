cmake_minimum_required(VERSION 3.5)

project(FFX_SW LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets Test Xml Core Sql REQUIRED)

# optionally you can also specify a specific version
# e.g. find_package(open62541 1.0.0)
find_package(open62541 REQUIRED COMPONENTS Events FullNamespace)



add_executable(FFX_SW
    main.cpp
    mainwindow.cpp
    mainwindow.h
    connection.h
    connection.cpp
    common.h
    plcdata.h
    plcdata.cpp
    clientopc.h
    clientopc.cpp
    process.h
    process.cpp
    testmodules.h
    mainwindow.ui
    resources.qrc
    control.h
    control.cpp
    standtablemodel.h
    standtablemodel.cpp
    database.h
    database.cpp
    calculation.h
    calculation.cpp
    productsize.h
    productsize.cpp
    files.h
    files.cpp
    diameterrolls.h
    diameterrolls.cpp
  )

target_link_libraries(FFX_SW PRIVATE Qt5::Widgets Qt5::Xml Qt5::Test Qt5::Core open62541::open62541 Qt5::Sql)
