#ifndef MAGNITUDE_PLATFORM_INPUT_HPP
#define MAGNITUDE_PLATFORM_INPUT_HPP
#include <definitions.hpp>

void    input_release_all();

bool    input_key_is_pressed(u32 key_code);
bool    input_key_is_released(u32 key_code);
bool    input_key_is_down(u32 key_code);
r64     input_key_time_down(u32 key_code);
r64     input_key_time_up(u32 key_code);

bool    input_mouse_button_is_pressed(u32 mouse_code);
bool    input_mouse_button_is_released(u32 mouse_code);
bool    input_mouse_button_is_down(u32 mouse_code);
r64     input_mouse_button_time_down(u32 mouse_code);
r64     input_mouse_button_time_up(u32 mouse_code);

b32     input_mouse_position_moved();
void    input_mouse_position_relative_unbounded(i32 *x, i32 *y);
void    input_mouse_position_relative_bounded(i32 *x, i32 *y);
void    input_mouse_position_relative_delta(i32 *x, i32 *y);
void    input_mouse_position_absolute(i32 *x, i32 *y);
r32     input_mouse_scroll_delta_y();
b32     input_mouse_scrolled();

#define MagMouseLeft        0
#define MagMouseRight       1
#define MagMouseMiddle      2

#define MagKeyA             0
#define MagKeyB             1
#define MagKeyC             2
#define MagKeyD             3
#define MagKeyE             4
#define MagKeyF             5
#define MagKeyG             6
#define MagKeyH             7
#define MagKeyI             8
#define MagKeyJ             9
#define MagKeyK             10
#define MagKeyL             11
#define MagKeyM             12
#define MagKeyN             13
#define MagKeyO             14
#define MagKeyP             15
#define MagKeyQ             16
#define MagKeyR             17
#define MagKeyS             18
#define MagKeyT             19
#define MagKeyU             20
#define MagKeyV             21
#define MagKeyW             22
#define MagKeyX             23
#define MagKeyY             24
#define MagKeyZ             25

#define MagKey0             26
#define MagKey1             27
#define MagKey2             28
#define MagKey3             29
#define MagKey4             30
#define MagKey5             31
#define MagKey6             32
#define MagKey7             33
#define MagKey8             34
#define MagKey9             35

#define MagKeyShift         42
#define MagKeyControl       43
#define MagKeyAlt           44
#define MagKeySpace         45
#define MagKeyBack          46
#define MagKeyDelete        47
#define MagKeyTab           48
#define MagKeyComma         49
#define MagKeyPeriod        50
#define MagKeyRightSlash    51
#define MagKeyLeftSlash     52
#define MagKeySemicolon     53
#define MagKeyQuote         54
#define MagKeyLeftBracket   55
#define MagKeyRightBracket  56
#define MagKeyTilde         57
#define MagKeyMinus         58
#define MagKeyEqual         59
#define MagKeyEnter         60
#define MagKeyEscape        61

#define MagKeyF1            63
#define MagKeyF2            64
#define MagKeyF3            65
#define MagKeyF4            66
#define MagKeyF5            67
#define MagKeyF6            68
#define MagKeyF7            69
#define MagKeyF8            70
#define MagKeyF9            71
#define MagKeyF10           72
#define MagKeyF11           73
#define MagKeyF12           74

#define MagKeyLeft          80
#define MagKeyRight         81
#define MagKeyUp            82
#define MagKeyDown          83

#define MagKeyNull          255


#endif
