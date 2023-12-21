cmake_minimum_required(VERSION 3.24)
project(FruitySerial)

set(CMAKE_CXX_STANDARD 17)

set(imgui_SOURCE_DIR ${imgui_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/imGui)
set(tools_PATH ${tools_PATH} ${CMAKE_SOURCE_DIR}/Tools)
set(uiDisplays_PATH ${uiDisplays_PATH} ${CMAKE_SOURCE_DIR}/UIDisplays)
set(data_PATH ${data_PATH} ${CMAKE_SOURCE_DIR}/Data)
set(manager_PATH ${manager_PATH} ${CMAKE_SOURCE_DIR}/Manager)
set(serial_PATH ${serial_PATH} ${CMAKE_SOURCE_DIR}/Serial)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2")
set(SDL2_IMAGE_PATH "SDL2")
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc" Tools/SendLog.cpp Tools/SendLog.h)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_subdirectory(Serial)

add_library(imgui
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imconfig.h
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imstb_rectpack.h
        ${imgui_SOURCE_DIR}/imstb_textedit.h
        ${imgui_SOURCE_DIR}/imstb_truetype.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.h)

include_directories(${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${imgui_SOURCE_DIR}
        ${tools_PATH}
        ${uiDisplays_PATH}
        ${data_PATH}
        ${manager_PATH}
        ${serial_PATH})

#link_directories(${SDL2_LIBRARY})

add_executable(FruitySerial main.cpp
                            UIDisplays/Hud.cpp
                            UIDisplays/Hud.h
                            Data/DTypes.h
                            Tools/stb_image.h
                            Tools/FunctionTools.cpp
                            Tools/FunctionTools.h
                            UIDisplays/SubWindow.cpp
                            UIDisplays/SubWindow.h
                            UIDisplays/MainWindow.cpp
                            UIDisplays/MainWindow.h
                            UIDisplays/TabSerialWindow.cpp
                            UIDisplays/TabSerialWindow.h
                            Manager/SerialManager.cpp
                            Manager/SerialManager.h
                            Manager/SerialConnection.cpp
                            Manager/SerialConnection.h
                            Data/LineData.cpp
                            Data/LineData.h
                            Tools/ClockTime.cpp
                            Tools/ClockTime.h
                            Tools/UndoRedo.cpp
                            Tools/UndoRedo.h
                            ${APP_ICON_RESOURCE_WINDOWS})


target_include_directories(${PROJECT_NAME} PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

target_link_libraries(${PROJECT_NAME}
                        OpenGL::GL
                        ${SDL2_LIBRARY}
                        ${SDL2_IMAGE_LIBRARY}
                        imgui
                        serial
                        #mingw32
                        )

