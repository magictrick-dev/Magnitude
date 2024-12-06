#ifndef MAGNITUDE_BINDINGS_HPP
#define MAGNITUDE_BINDINGS_HPP
#include <common.hpp>
#include <unordered_map>
#include <array>

// --- Keybind Interface -------------------------------------------------------
//
// The keybind interface is designed to be overridden by a class that defines
// a particular behavior.
//

class IKeybind
{

    public:
        inline                  IKeybind(bool shift, bool control, i32 key);
        inline                 ~IKeybind();

        inline virtual void     on_press();
        inline virtual void     on_release();

        inline void             check();

    protected:
        bool uses_shift;
        bool uses_control;
        i32 key_index;

};

IKeybind::
IKeybind(bool shift, bool control, i32 key)
{
    this->uses_shift = shift;
    this->uses_control = control;
    this->key_index = key;
}

IKeybind::
~IKeybind()
{

}

void IKeybind::
on_press()
{

}

void IKeybind::
on_release()
{

}

void IKeybind::
check()
{

    if (this->uses_shift && !input_key_is_down(MagKeyShift)) return;
    if (this->uses_control && !input_key_is_down(MagKeyControl)) return;

    if (input_key_is_pressed(this->key_index)) this->on_press();
    else if (input_key_is_released(this->key_index)) this->on_release();

}

// --- Shortcut Bindings Adapter -----------------------------------------------
//
// The shortcut bindings adapter is designed to centralized hot-key like functionality
// in a unified interface. Since multiple components want to inspect the state of
// the keyboard, but may produce different behaviors, this method allows each one
// to define their own bindings interface and interact as they see fit.
//
// The environment maintains its own bindings adapter and uses that in place of
// when a custom adapter is not set.
//

class ShortcutBindings
{

    public:
        inline          ShortcutBindings();
        inline virtual ~ShortcutBindings();

        inline void     update_all();
        inline bool     is_binding_set(bool control, bool shift, i32 key);

        template <class T> inline void binding_set(bool control, bool shift, i32 key);

    protected:
        std::unordered_map<i32, std::array<shared_ptr<IKeybind>,3>> map;

};

inline ShortcutBindings::
ShortcutBindings()
{

}

inline ShortcutBindings::
~ShortcutBindings()
{

}

inline void ShortcutBindings::
update_all()
{

    for (auto current_mapping : this->map)
    {
        auto bind_set = current_mapping.second;
        if (bind_set[0] != nullptr) bind_set[0]->check();
        if (bind_set[1] != nullptr) bind_set[1]->check();
        if (bind_set[2] != nullptr) bind_set[2]->check();
    }

}

template <class T> inline void ShortcutBindings::
binding_set(bool control, bool shift, i32 key)
{

    i32 offset = control + shift;
    auto bind_set = this->map[key];
    bind_set[offset] = make_shared<T>();

}

#endif
