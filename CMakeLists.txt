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
        "source/platform/win32/win32window.hpp"
        "source/platform/win32/win32window.cpp"
        "source/platform/win32/opengl.cpp"
        "source/platform/win32/win32opengl.hpp"
        "source/platform/win32/win32opengl.cpp"
        "source/platform/win32/filesystem.cpp"
        "source/platform/win32/system.cpp"

        "vendor/imgui/backends/imgui_impl_opengl3.cpp"
        "vendor/imgui/backends/imgui_impl_win32.cpp"
    )
endif (WIN32)

# --- Executable File Listings -------------------------------------------------
#
# Defines the executable and the files that need to be added to compile it.
#
 
add_executable(Magnitude
    "source/main.cpp"
    "source/definitions.hpp"
    "source/linear.hpp"
    "source/linear.cpp"

    "source/graphics/color.hpp"
    "source/graphics/color.cpp"
    "source/graphics/bitmap.hpp"
    "source/graphics/bitmap.cpp"

    "source/editor/editor.hpp"
    "source/editor/editor.cpp"
    "source/editor/component.hpp"
    "source/editor/component.cpp"
    "source/editor/mainmenu.hpp"
    "source/editor/mainmenu.cpp"
    "source/editor/rdviewer.hpp"
    "source/editor/rdviewer.cpp"

    "source/utilities/path.hpp"
    "source/utilities/path.cpp"
    "source/utilities/cli.hpp"
    "source/utilities/cli.cpp"
    "source/utilities/resourceman.hpp"
    "source/utilities/resourceman.cpp"
    "source/utilities/rdtokenizer.hpp"
    "source/utilities/rdtokenizer.cpp"
    "source/utilities/logging.hpp"
    "source/utilities/logging.cpp"

    "source/platform/window.hpp"
    "source/platform/opengl.hpp"
    "source/platform/filesystem.hpp"
    "source/platform/system.hpp"

    "vendor/stb/stb_image.h"
    "vendor/glad/glad.c"
    "vendor/glad/glad.h"
    "vendor/glad/glad_wgl.c"
    "vendor/glad/glad_wgl.h"
    "vendor/glad/khrplatform.h"
    "vendor/imgui/imgui.h"
    "vendor/imgui/imgui.cpp"
    "vendor/imgui/imgui_demo.cpp"
    "vendor/imgui/imgui_draw.cpp"
    "vendor/imgui/imgui_internal.h"
    "vendor/imgui/imgui_tables.cpp"
    "vendor/imgui/imgui_widgets.cpp"
    "vendor/imgui/imstb_rectpack.h"
    "vendor/imgui/imstb_textedit.h"
    "vendor/imgui/imstb_truetype.h"
    "vendor/balazedit/texteditor.h"
    "vendor/balazedit/texteditor.cpp"

    ${PLATFORM_FILES}
)

# --- Include Directories ------------------------------------------------------
#
# Defines which directories that are allowed to be bracket-included versus
# relative pathing.
#

find_package(OpenGL REQUIRED)
target_include_directories(Magnitude PRIVATE "source" "vendor")
set_property(TARGET Magnitude PROPERTY CXX_STANDARD 20)

if (WIN32)
    TARGET_LINK_LIBRARIES(Magnitude 
        opengl32.lib 
        Shlwapi.lib 
        winmm.lib 
        Xinput.lib 
        Xaudio2.lib
    )
endif(WIN32)
