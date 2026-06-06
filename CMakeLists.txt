cmake_minimum_required(VERSION 3.21)
project(my_application LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)
include(CMakePrintHelpers)
include(FetchContent)

set(SLINT_STYLE "cosmic-dark" CACHE STRING "The style used by Slint")

find_package(Slint QUIET)
if (NOT Slint_FOUND)
  message("Slint could not be located in the CMake module search path. Downloading it from Git and building it locally")
  include(FetchContent)
  FetchContent_Declare(
    Slint
    GIT_REPOSITORY https://github.com/slint-ui/slint.git
    # `release/1` will auto-upgrade to the latest Slint >= 1.0.0 and < 2.0.0
    # `release/1.0` will auto-upgrade to the latest Slint >= 1.0.0 and < 1.1.0
    GIT_TAG release/1
    SOURCE_SUBDIR api/cpp
  )
  FetchContent_MakeAvailable(Slint)
endif (NOT Slint_FOUND)

add_subdirectory(json-develop)

# -----------------------------------------------#
#                 Dependecies                    #
# -----------------------------------------------#
# find_package(fmt QUIET)
# if (NOT fmt_FOUND)
#   FetchContent_Declare(
#       fmt
#       GIT_REPOSITORY https://github.com/fmtlib/fmt.git
#       GIT_TAG        11.1.4
#       FIND_PACKAGE_ARGS 
#   )
#   FetchContent_MakeAvailable(fmt)
#   if(TARGET fmt::fmt)
#     message("fmt was found")
#   else()
#     message(FATAL_ERROR "fmt was not found")
#   endif()
# endif (NOT fmt_FOUND)

add_executable(my_application src/main.cpp)
set_target_properties(my_application
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS ON
        CXX_EXCEPTIONS ON
        #COMPILE_WARNING_AS_ERROR YES
)
target_compile_features(my_application PRIVATE cxx_std_20 )
target_link_libraries(my_application PRIVATE Slint::Slint nlohmann_json::nlohmann_json)


slint_target_sources(my_application ui/app-window.slint)
# On Windows, copy the Slint DLL next to the application binary so that it's found.
if (WIN32)
    add_custom_command(TARGET my_application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:my_application> $<TARGET_FILE_DIR:my_application> COMMAND_EXPAND_LISTS)
endif()


file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)

