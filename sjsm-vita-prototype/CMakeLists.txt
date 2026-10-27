cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "VitaSDK toolchain")
  else()
    message(FATAL_ERROR "Set VITASDK or pass -DCMAKE_TOOLCHAIN_FILE")
  endif()
endif()

project(SpookyCorridorVita C)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APPNAME "SpookyCorridorVita")
set(VITA_TITLEID "SPCV00001")
set(VITA_VERSION "01.00")

add_executable(${VITA_APPNAME} src/main.c)
target_compile_options(${VITA_APPNAME} PRIVATE -O2 -Wall -Wextra -DSDL_MAIN_HANDLED)
target_link_libraries(${VITA_APPNAME}
  SDL2
  SceCtrl_stub
  SceDisplay_stub
  SceGxm_stub
  SceSysmodule_stub
  m
)

vita_create_self(eboot.bin ${VITA_APPNAME} SAFE)
vita_create_vpk(${VITA_APPNAME}.vpk ${VITA_TITLEID} eboot.bin
  VERSION ${VITA_VERSION}
  NAME "Spooky Corridor Vita"
)
