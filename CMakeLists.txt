cmake_minimum_required(VERSION 3.10)
project(gsmenu)
find_package(PkgConfig REQUIRED)

set("LV_USE_LINUX_DRM" "ON")

option(USE_SIMULATOR "Enable simulator mode" OFF)
if(USE_SIMULATOR)
    add_definitions(-DUSE_SIMULATOR)
    pkg_check_modules(LIBSDL REQUIRED sdl2)
else()
    pkg_check_modules(LIBGPIOD REQUIRED libgpiod)
endif()

# Uncomment if the program needs debugging
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb")

pkg_check_modules(LIBDRM REQUIRED libdrm)
include_directories(${LIBDRM_INCLUDE_DIRS})

set(CMAKE_C_STANDARD 99) # LVGL officially supports C99 and above
set(CMAKE_CXX_STANDARD 17) #C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(LV_CONF_BUILD_DISABLE_DEMOS 1)
set(LV_CONF_BUILD_DISABLE_EXAMPLES 1)

add_subdirectory(lvgl)
target_include_directories(lvgl PUBLIC ${PROJECT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/ui)
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.c ${PROJECT_SOURCE_DIR}/src/ui/*.c)

add_executable(gsmenu main.c ${SOURCES})

target_link_libraries(gsmenu lvgl ${LIBDRM_LIBRARIES} ${LIBSDL_LIBRARIES} ${LIBGPIOD_LIBRARIES})
