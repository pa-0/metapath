cmake_minimum_required(VERSION 3.15)

project(Metapath VERSION 4.0.13)

add_compile_definitions(
    WIN32
    _WINDOWS
    NDEBUG
    _UNICODE
    UNICODE)

add_executable(Metapath
  "src/metapath.c")

target_sources(Metapath
  PUBLIC "src/Dialogs.c"
  PUBLIC "src/Dialogs.h"
  PUBLIC "src/Dlapi.c"
  PUBLIC "src/Dlapi.h"
  PUBLIC "src/Dropsource.cpp"
  PUBLIC "src/Dropsource.h"
  PUBLIC "src/Helpers.c"
  PUBLIC "src/Helpers.h"
  PUBLIC "src/metapath.h"
  PUBLIC "src/metapath.rc"
  PUBLIC "src/metapath.ver"
  PUBLIC "src/resource.h"
  PUBLIC "src/version.h")

target_compile_features(Metapath
  PUBLIC cxx_std_17
  )

target_link_libraries(Metapath
  PUBLIC kernel32.lib
  PUBLIC user32.lib
  PUBLIC gdi32.lib
  PUBLIC Msimg32.lib
  PUBLIC advapi32.lib
  PUBLIC shell32.lib
  PUBLIC shlwapi.lib
  PUBLIC comdlg32.lib
  PUBLIC comctl32.lib
  PUBLIC winspool.lib
  PUBLIC imm32.lib
  PUBLIC ole32.lib
  PUBLIC oleaut32.lib
  PUBLIC psapi.lib)
