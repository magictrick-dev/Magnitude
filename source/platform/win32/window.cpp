#include <memory>
#include <platform/window.hpp>
#include <platform/win32/win32window.hpp>

std::shared_ptr<Window> Window::
create(std::string title, i32 width, i32 height)
{

    Win32Window *window = new Win32Window(title, width, height);
    std::shared_ptr<Window> shared(window);
    return shared;

}
