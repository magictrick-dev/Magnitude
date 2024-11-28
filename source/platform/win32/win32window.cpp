#include <iostream>

#include <windows.h>
#include <wingdi.h>

#include <utilities/path.hpp>
#include <platform/system.hpp>
#include <platform/win32/win32window.hpp>
#include <platform/win32/inputhandler.hpp>

#include <glad/glad.h>
#include <imgui/imgui.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Win32Window::
Win32Window(std::string title, i32 width, i32 height)
{

    // Set the input intialization stuff.
    initialize_input_states();

    // Set the properties as provided by the user. Validate.
    // TODO(Chris): I know there's a minimum window size specification that we
    //              should probably enforce here. I should look this up and enforce
    //              a minimum size. Constants are provided for some good default
    //              values as a workaround.
    MAG_ASSERT(width > 0);
    MAG_ASSERT(height > 0);
    MAG_ASSERT(title.length() > 0);
    this->width     = width;
    this->height    = height;
    this->title     = title;

    // Create the display window class and register it.
    WNDCLASSEXW display_window_class    = {0};
    display_window_class.cbSize         = sizeof(display_window_class);
    display_window_class.lpfnWndProc    = &Win32Window::window_procedure;
    display_window_class.hInstance      = GetModuleHandleW(NULL);
    display_window_class.hIcon          = LoadIconA(NULL, IDI_APPLICATION);
    display_window_class.hCursor        = LoadCursorA(NULL, IDC_ARROW);
    display_window_class.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    display_window_class.lpszClassName  = L"displayWindowClass";
    display_window_class.style          = CS_OWNDC;
    RegisterClassExW(&display_window_class);

    // Calculate window size to accomodate frame size + client area.
    RECT client_rect    = {0};
    client_rect.right   = width;
    client_rect.bottom  = height;
    AdjustWindowRect(&client_rect, WS_OVERLAPPEDWINDOW, FALSE);
    i32 window_width    = client_rect.right  - client_rect.left;
    i32 window_height   = client_rect.bottom - client_rect.top;

    // Attempt to create the window. Normally, I would attempt to rationalize the
    // chaos that is Win32 programming, but alas, I leave the exercise up to the reader.
    HWND window_handle = CreateWindowExW(0, display_window_class.lpszClassName,
            L"Magnitude Rendering Utility", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            window_width, window_height, NULL, NULL, display_window_class.hInstance, this);
    MAG_ASSERT(window_handle != NULL);

    // Establish known properties about the window.
    this->window_handle     = window_handle;
    this->device_context    = GetDC(window_handle);
    this->focused           = true;
    this->minimized         = false;
    this->maximized         = false;
    this->resizable         = true;
    this->visible           = false;
    this->borderless        = false;
    this->was_resized       = false;
    this->was_focused       = false;
    this->was_closed        = false;

    // Now set the actual title.
    SetWindowTextA(window_handle, title.c_str());

    // Now set the icon.
    Filepath icon_directory = Filepath::cwd();
    icon_directory += "./assets/icons/app.ico";
    icon_directory.canonicalize();
    MAG_ASSERT(icon_directory.is_valid_file() && "Unable to locate icon file path.");

    HANDLE image_handle = LoadImageA(display_window_class.hInstance,
            icon_directory.c_str(), IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
    MAG_ASSERT(image_handle != NULL && "Unable to create icon handle.");
    SetClassLongPtr(window_handle, GCLP_HICON, (LONG_PTR)(HICON)image_handle);

}

Win32Window::
~Win32Window()
{

}

void Win32Window::
poll_events()
{

    // Reset our state-change flags.
    this->was_resized = false;
    this->was_focused = false;

    // Swap the input frames.
    swap_input_states();

    // Update the input states.
    input_state *previous_frame = get_previous_input_state();
    input_state *current_frame = get_current_input_state();
    for (u64 idx = 0; idx < 256; ++idx)
    {

        if (current_frame->keyboard[idx].is_released)
            current_frame->keyboard[idx].time = system_timestamp();
        if (current_frame->keyboard[idx].is_pressed)
            current_frame->keyboard[idx].time = system_timestamp();

        current_frame->keyboard[idx].is_released = false;
        current_frame->keyboard[idx].is_pressed = false;

    }

    for (u64 idx = 0; idx < 8; ++idx)
    {

        if (current_frame->mouse[idx].is_released)
            current_frame->mouse[idx].time = system_timestamp();
        if (current_frame->mouse[idx].is_pressed)
            current_frame->mouse[idx].time = system_timestamp();

        current_frame->mouse[idx].is_released = false;
        current_frame->mouse[idx].is_pressed = false;

    }

    current_frame->mouse_position.moved = false;
    current_frame->mouse_wheelie = false;
    current_frame->mouse_wheel.delta_x = 0.0;
    current_frame->mouse_wheel.delta_y = 0.0;

    // Pump the message loop.
    MSG current_message = {0};
    while (PeekMessageA(&current_message, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&current_message);
        DispatchMessageW(&current_message);
    }

}

void Win32Window::
swap_frames()
{

    SwapBuffers(this->device_context);

}

void Win32Window::
show()
{

    this->set_visibility(true);

}

bool Win32Window::
should_close() const
{
    return this->was_closed;
}

bool Win32Window::
did_size_change() const
{
    return this->was_resized;
}

bool Win32Window::
did_focus_change() const
{
    return this->was_focused;
}

bool Win32Window::
is_borderless() const
{
    return this->borderless;
}

bool Win32Window::
is_focused() const
{
    return this->focused;
}

bool Win32Window::
is_visible() const
{
    return this->visible;
}

bool Win32Window::
is_maximized() const
{
    return this->maximized;
}

bool Win32Window::
is_minimized() const
{
    return this->minimized;
}

i32 Win32Window::
get_height() const
{
    return this->height;
}

i32 Win32Window::
get_width() const
{
    return this->width;
}
 
std::string Win32Window::
get_title() const
{
    return this->title;
}

void Win32Window::
set_borderless(b32 enable, i32 exit_width, i32 exit_height)
{

    if (enable)
    {

        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        SetWindowLongPtr(this->window_handle, GWL_STYLE, WS_VISIBLE | WS_POPUP);
        SetWindowPos(this->window_handle, HWND_TOP, 0, 0, w, h, SWP_FRAMECHANGED);

    }
    
    else
    {

        SetWindowLongPtr(this->window_handle, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
        SetWindowPos(this->window_handle, NULL, 0, 0, 600, 400, SWP_FRAMECHANGED);   
        this->set_size(exit_width, exit_height);

    }

    this->borderless = enable;

}

void Win32Window::
set_title(const std::string& title)
{

    SetWindowTextA(this->window_handle, title.c_str());

}

void Win32Window::
set_visibility(b32 visible)    
{

    // This function will automatically trigger a window procedure event
    // to update the internal state.
    if (visible)
        ShowWindow(this->window_handle, SW_SHOWNORMAL);
    else
        ShowWindow(this->window_handle, SW_HIDE);

    this->visible = visible;

}

void Win32Window::
set_maximized(b32 maximize)    
{

    // This function will automatically trigger a window procedure event
    // to update the internal state.
    if (maximize) 
    {
        ShowWindow(this->window_handle, SW_MAXIMIZE);
    }
    else 
    {
        ShowWindow(this->window_handle, SW_RESTORE);
    }

}

void Win32Window::
set_size(i32 width, i32 height)
{

    // This function will automatically trigger a window procedure event
    // to update the internal state.
    RECT client_rect    = {0};
    client_rect.right   = width;
    client_rect.bottom  = height;

    AdjustWindowRect(&client_rect, WS_OVERLAPPEDWINDOW, FALSE);

    i32 window_width    = client_rect.right  - client_rect.left;
    i32 window_height   = client_rect.bottom - client_rect.top;

    SetWindowPos(this->window_handle, NULL, 0, 0,
        window_width, window_height, SWP_NOMOVE);

}

void Win32Window::
set_bitmap(i32 x, i32 y, bitmap_info_header *info, u32 *data)
{

    i32 result = StretchDIBits(this->device_context, x, y, this->width, this->height,
            0, 0, info->width, info->height, data,
            (BITMAPINFO*)info, DIB_RGB_COLORS, SRCCOPY);

}

void Win32Window::
set_pixel(i32 x, i32 y, packed_color what)
{

    if (x < 0.0f || x >= this->width || y < 0.0f || y >= this->height) return;
    SetPixel(this->device_context, x, y, (DWORD)what.pack);
    return;

}

packed_color Win32Window::
get_pixel(i32 x, i32 y)
{

    packed_color result = {0};
    if (x < 0.0f || x >= this->width || y < 0.0f || y >= this->height) return result;
    result.pack = (u32)GetPixel(this->device_context, x, y);
    return result;

}

LRESULT CALLBACK Win32Window::
window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{

    ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param);
    Win32Window *self = (Win32Window*)GetWindowLongPtr(window, GWLP_USERDATA);
    LRESULT ret_result = 0;   

    input_state *previous_frame = get_previous_input_state();
    input_state *current_frame = get_current_input_state();

    switch (message)
    {

        case WM_CREATE:
        {

            // We have to play pointer hopscotch just to get the Win32Window ptr.
            CREATESTRUCT *creation_context = (CREATESTRUCT*)l_param;
            Win32Window *window_context = (Win32Window*)creation_context->lpCreateParams;
            SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)window_context);

        } break;

        case WM_CLOSE:
        {

            self->was_closed = true;

        } break;

        case WM_SIZE:
        {

            i32 width   = (0xFFFF & l_param);
            i32 height  = (l_param >> 16);

            if (w_param == SIZE_MINIMIZED)
            {
                self->minimized = true;
            }
            else if (w_param == SIZE_MAXIMIZED)
            {
                self->maximized = true;
            }
            else if (w_param == SIZE_RESTORED && self->width == 0 && self->height == 0)
            {
                self->minimized = false;
                self->maximized = false;
            }

            self->width         = width;
            self->height        = height;
            self->was_resized   = true;

        } break;

        case WM_KEYDOWN:
        {

            u32 mapping = convert_keycode((u32)w_param);

            if (!previous_frame->keyboard[mapping].is_down) 
            {
                current_frame->keyboard[mapping].is_pressed = true;
            }

            current_frame->keyboard[mapping].is_down = true;

        } break;

        case WM_KEYUP:
        {

            u32 mapping = convert_keycode((u32)w_param);

            if (previous_frame->keyboard[mapping].is_down)
            {
                current_frame->keyboard[mapping].is_released = true;
            }

            current_frame->keyboard[mapping].is_down = false;

            break;

        } break;

        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDOWN:
        {
            
            if (w_param == MK_LBUTTON)
            {

                u32 mapping = MagMouseLeft;

                if (!previous_frame->mouse[mapping].is_down) 
                {
                    current_frame->mouse[mapping].is_pressed = true;
                }

                current_frame->mouse[mapping].is_down = true;

            }

            else if (w_param == MK_RBUTTON)
            {

                u32 mapping = MagMouseRight;

                if (!previous_frame->mouse[mapping].is_down) 
                {
                    current_frame->mouse[mapping].is_pressed = true;
                }

                current_frame->mouse[mapping].is_down = true;

            }

            else if (w_param == MK_MBUTTON)
            {

                u32 mapping = MagMouseMiddle;

                if (!previous_frame->mouse[mapping].is_down) 
                {
                    current_frame->mouse[mapping].is_pressed = true;
                }

                current_frame->mouse[mapping].is_down = true;

            }

        } break;

        case WM_MBUTTONUP:
        {

            u32 mapping = MagMouseMiddle;

            if (previous_frame->mouse[mapping].is_down)
            {
                current_frame->mouse[mapping].is_released = true;
            }

            current_frame->mouse[mapping].is_down = false;

        } break;

        case WM_RBUTTONUP:
        {

            u32 mapping = MagMouseRight;

            if (previous_frame->mouse[mapping].is_down)
            {
                current_frame->mouse[mapping].is_released = true;
            }

            current_frame->mouse[mapping].is_down = false;

        } break;

        case WM_LBUTTONUP:
        {

            u32 mapping = MagMouseLeft;

            if (previous_frame->mouse[mapping].is_down)
            {
                current_frame->mouse[mapping].is_released = true;
            }

            current_frame->mouse[mapping].is_down = false;

        } break;

        case WM_ACTIVATEAPP:
        {

            b32 is_active = (w_param == TRUE);
            self->focused = is_active;
            self->was_focused = true;

            if (!is_active) input_release_all();

        } break;

        default:
        {
            ret_result = DefWindowProcW(window, message, w_param, l_param);
        };

    };

    return ret_result;

}

