cmake_minimum_required(VERSION 3.21)

project(Obsession VERSION 109.5 LANGUAGES CXX)

# Set C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Find required packages
find_package(Qt6 6.10 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Network
    Multimedia
)

# Find ICU for extended codec support (Shift-JIS, Apple Roman, etc.)
find_package(ICU REQUIRED COMPONENTS uc i18n)

# Use Qt's standard project setup (enables automoc, autouic, autorcc)
qt_standard_project_setup()

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/ctransaction.cpp
    src/connectioncontroller.cpp
    src/dialogopenconnection.cpp
    src/dialogpreferences.cpp
    src/dialogagreement.cpp
    src/listwidgetusers.cpp
    src/UserIconDelegate.cpp
    src/dialogprivatemessaging.cpp
    src/dialogfilebrowser.cpp
    src/dialogtrackers.cpp
    src/dialogaddtracker.cpp
    src/dialogerror.cpp
    src/dialogiconviewer.cpp
    src/dialoglinearnews.cpp
    src/chatlineedit.cpp
    src/downloadmanager.cpp
    src/dialogdownloadqueue.cpp
    src/dialogabout.cpp
    src/transactionparameter.cpp
    src/widgetchat.cpp
    src/widgetnews.cpp
    src/dialogbookmarks.cpp
    src/cdownload.cpp
    src/widgetdownloaditem.cpp
    src/uploadmanager.cpp
    src/cupload.cpp
    src/dialoguploadsqueue.cpp
    src/widgetconsole.cpp
    src/dialoguserinfo.cpp
    src/threadiconloader.cpp
    src/threaduploader.cpp
    src/dialogprivileges.cpp
    src/dialogrequestaccount.cpp
    src/dialogcreateaccount.cpp
    src/dialogbroadcast.cpp
    src/dialognewnewsmessage.cpp
    src/dialogpostlinearnews.cpp
)

# Header files
set(HEADERS
    src/mainwindow.h
    src/ctransaction.h
    src/defines.h
    src/connectioncontroller.h
    src/dialogopenconnection.h
    src/dialogpreferences.h
    src/dialogagreement.h
    src/listwidgetusers.h
    src/UserIconDelegate.h
    src/dialogprivatemessaging.h
    src/dialogfilebrowser.h
    src/dialogtrackers.h
    src/dialogaddtracker.h
    src/dialogerror.h
    src/dialogiconviewer.h
    src/dialoglinearnews.h
    src/chatlineedit.h
    src/downloadmanager.h
    src/dialogdownloadqueue.h
    src/dialogabout.h
    src/transactionparameter.h
    src/widgetchat.h
    src/widgetnews.h
    src/dialogbookmarks.h
    src/cdownload.h
    src/widgetdownloaditem.h
    src/uploadmanager.h
    src/cupload.h
    src/dialoguploadsqueue.h
    src/widgetconsole.h
    src/dialoguserinfo.h
    src/threadiconloader.h
    src/version.h
    src/threaduploader.h
    src/TextHelper.h
    src/SettingsManager.h
    src/FileTypeMapper.h
    src/HotlineProtocol.h
    src/dialogprivileges.h
    src/dialogrequestaccount.h
    src/dialogcreateaccount.h
    src/dialogbroadcast.h
    src/dialognewnewsmessage.h
    src/dialogpostlinearnews.h
)

# UI form files
set(FORMS
    src/mainwindow.ui
    src/dialogopenconnection.ui
    src/dialogpreferences.ui
    src/dialogagreement.ui
    src/dialogprivatemessaging.ui
    src/dialogfilebrowser.ui
    src/dialogtrackers.ui
    src/dialogaddtracker.ui
    src/dialogerror.ui
    src/dialogiconviewer.ui
    src/dialoglinearnews.ui
    src/dialogdownloadqueue.ui
    src/dialogabout.ui
    src/widgetchat.ui
    src/widgetnews.ui
    src/dialogbookmarks.ui
    src/widgetdownloaditem.ui
    src/dialoguploadsqueue.ui
    src/widgetconsole.ui
    src/dialoguserinfo.ui
    src/dialogprivileges.ui
    src/dialogrequestaccount.ui
    src/dialogcreateaccount.ui
    src/dialogbroadcast.ui
    src/dialognewnewsmessage.ui
    src/dialogpostlinearnews.ui
)

# Resource files
set(RESOURCES
    resources/resources.qrc
    resources/icons1.qrc
    resources/icons2.qrc
)

# Compile resources
qt_add_resources(RESOURCE_FILES ${RESOURCES})

# Create executable
qt_add_executable(Obsession
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
    ${RESOURCE_FILES}
)

# Include directories
target_include_directories(Obsession PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(Obsession PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Multimedia
    ICU::uc
    ICU::i18n
)

# Compiler definitions matching original QMake config
target_compile_definitions(Obsession PRIVATE
    _CRT_SECURE_NO_WARNINGS
    QT_NO_CAST_FROM_ASCII
)

# Disable exceptions and RTTI (matching original config)
if(MSVC)
    target_compile_options(Obsession PRIVATE /EHs-c- /GR-)
else()
    target_compile_options(Obsession PRIVATE -fno-exceptions -fno-rtti)
endif()

# Platform-specific configurations
if(WIN32)
    # Windows: Add RC file for icon and version info
    target_sources(Obsession PRIVATE Obsession.rc)
    
    # Set application icon
    set_target_properties(Obsession PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
elseif(APPLE)
    # macOS: Set application icon
    set(MACOSX_BUNDLE_ICON_FILE icon.icns)
    set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/resources/icon.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )
    target_sources(Obsession PRIVATE ${APP_ICON_MACOSX})
    
    set_target_properties(Obsession PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
    )
endif()

# Installation rules
include(GNUInstallDirs)

install(TARGETS Obsession
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Qt deployment
if(WIN32)
    qt_generate_deploy_app_script(
        TARGET Obsession
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
    )
    install(SCRIPT ${deploy_script})
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "Obsession Configuration Summary:")
message(STATUS "  Version:        ${PROJECT_VERSION}")
message(STATUS "  Qt Version:     ${Qt6_VERSION}")
message(STATUS "  C++ Standard:   C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "  ICU Found:      ${ICU_FOUND}")
message(STATUS "  ICU Version:    ${ICU_VERSION}")
message(STATUS "")
