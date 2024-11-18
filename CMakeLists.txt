cmake_minimum_required(VERSION 3.21)

# --- Project Setup ------------------------------------------------------------
#
# Here, we designate the project name and the output directory for the build.
# Additionally, you can toggle the debug flag for production builds as well.
#

project(Magnitude)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")
add_compile_definitions(MAGNITUDE_DEBUG_BUILD=1)

# --- Platform Detection -------------------------------------------------------
#
# Determines which platform specific files to compile for the current target
# platform. Generally, you shouldn't list any non-platform specific source files
# here and instead you should list them in the "executable file listings".
#

set(PLATFORM_FILES)
if (WIN32)
    set(PLATFORM_FILES
        "source/platform/win32/window.cpp"
        "source/platform/win32/filesystem.cpp"
        "source/platform/win32/system.cpp"
    )
endif (WIN32)

# --- Executable File Listings -------------------------------------------------
#
# Defines the executable and the files that need to be added to compile it.
#
 
add_executable(Magnitude
    "source/main.cpp"
    "source/definitions.hpp"

    "source/utilities/path.hpp"
    "source/utilities/path.cpp"
    "source/utilities/cli.hpp"
    "source/utilities/cli.cpp"

    "source/platform/window.hpp"
    "source/platform/filesystem.hpp"
    "source/platform/system.hpp"

    ${PLATFORM_FILES}
)

# --- Include Directories ------------------------------------------------------
#
# Defines which directories that are allowed to be bracket-included versus
# relative pathing.
#

target_include_directories(Magnitude PUBLIC "source")
set_property(TARGET Magnitude PROPERTY CXX_STANDARD 20)

if (WIN32)
    TARGET_LINK_LIBRARIES(Magnitude Shlwapi.lib)
endif(WIN32)
