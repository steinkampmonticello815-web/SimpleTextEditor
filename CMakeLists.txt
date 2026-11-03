cmake_minimum_required(VERSION 3.19)
project(SimpleTextEditor LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

qt_add_executable(SimpleTextEditor
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    action_icons.qrc
    dialog_assets.qrc
    aboutdialog.h aboutdialog.cpp aboutdialog.ui
    finddialog.h finddialog.cpp finddialog.ui
    replacedialog.h replacedialog.cpp replacedialog.ui
)

target_link_libraries(SimpleTextEditor
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS SimpleTextEditor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET SimpleTextEditor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
