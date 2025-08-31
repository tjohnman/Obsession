cmake_minimum_required(VERSION 3.00)

project(Obsession)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Network Multimedia MultimediaWidgets Core5Compat)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES
	src/cdownload.cpp
	src/dialogfilebrowser.cpp
	src/downloadmanager.cpp
	src/chatlineedit.cpp
	src/dialogiconviewer.cpp
	src/listwidgetusers.cpp
	src/connectioncontroller.cpp
	src/dialoglinearnews.cpp
	src/main.cpp
	src/ctransaction.cpp
	src/dialognewnewsmessage.cpp
	src/mainwindow.cpp
	src/cupload.cpp
	src/dialogopenconnection.cpp
	src/threadiconloader.cpp
	src/dialogabout.cpp
	src/dialogpostlinearnews.cpp
	src/threaduploader.cpp
	src/dialogaddtracker.cpp
	src/dialogpreferences.cpp
	src/transactionparameter.cpp
	src/dialogagreement.cpp
	src/dialogprivatemessaging.cpp
	src/uploadmanager.cpp
	src/dialogbookmarks.cpp
	src/dialogprivileges.cpp
	src/widgetchat.cpp
	src/dialogbroadcast.cpp
	src/dialogrequestaccount.cpp
	src/widgetconsole.cpp
	src/dialogcreateaccount.cpp
	src/dialogtrackers.cpp
	src/widgetdownloaditem.cpp
	src/dialogdownloadqueue.cpp
	src/dialoguploadsqueue.cpp
	src/widgetnews.cpp
	src/dialogerror.cpp
	src/dialoguserinfo.cpp
)

set(HEADERS
	src/TextHelper.h
	src/cdownload.h
	src/chatlineedit.h
	src/connectioncontroller.h
	src/ctransaction.h
	src/cupload.h
	src/defines.h
	src/dialogabout.h
	src/dialogaddtracker.h
	src/dialogagreement.h
	src/dialogbookmarks.h
	src/dialogbroadcast.h
	src/dialogcreateaccount.h
	src/dialogdownloadqueue.h
	src/dialogerror.h
	src/dialogfilebrowser.h
	src/dialogiconviewer.h
	src/dialoglinearnews.h
	src/dialognewnewsmessage.h
	src/dialogopenconnection.h
	src/dialogpostlinearnews.h
	src/dialogpreferences.h
	src/dialogprivatemessaging.h
	src/dialogprivileges.h
	src/dialogrequestaccount.h
	src/dialogtrackers.h
	src/dialoguploadsqueue.h
	src/dialoguserinfo.h
	src/downloadmanager.h
	src/listwidgetusers.h
	src/mainwindow.h
	src/threadiconloader.h
	src/threaduploader.h
	src/transactionparameter.h
	src/uploadmanager.h
	src/version.h
	src/widgetchat.h
	src/widgetconsole.h
	src/widgetdownloaditem.h
	src/widgetnews.h
)

set(UI_FILES
	src/dialogabout.ui
	src/dialogaddtracker.ui
	src/dialogagreement.ui
	src/dialogbookmarks.ui
	src/dialogbroadcast.ui
	src/dialogcreateaccount.ui
	src/dialogdownloadqueue.ui
	src/dialogerror.ui
	src/dialogfilebrowser.ui
	src/dialogiconviewer.ui
	src/dialoglinearnews.ui
	src/dialognewnewsmessage.ui
	src/dialogopenconnection.ui
	src/dialogpostlinearnews.ui
	src/dialogpreferences.ui
	src/dialogprivatemessaging.ui
	src/dialogprivileges.ui
	src/dialogrequestaccount.ui
	src/dialogtrackers.ui
	src/dialoguploadsqueue.ui
	src/dialoguserinfo.ui
	src/mainwindow.ui
	src/widgetchat.ui
	src/widgetconsole.ui
	src/widgetdownloaditem.ui
	src/widgetnews.ui
)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.rc")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_FILES} ${app_icon_resource_windows}
	resources/icons.qrc
	resources/resources.qrc)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Widgets Qt6::Network Qt6::Multimedia Qt6::Core5Compat)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
