#ifndef MAGNITUDE_GRAPHICS_COLOR_HPP
#define MAGNITUDE_GRAPHICS_COLOR_HPP
#include <definitions.hpp>

// --- Packed Color ------------------------------------------------------------
//
// Packed color is a structure which represents a 32-bit integer with components
// for each color. Since the format changes (particularly Windows) dependening
// on the system, it's the responsibility of the user to ensure that it's packed
// in the correct bit order.
//

union packed_color 
{

    u32 pack;

    struct
    {
        u8 alpha;
        u8 blue;
        u8 green;
        u8 red;
    } component_abgr;

    struct
    {
        u8 red;
        u8 green;
        u8 blue;
        u8 alpha;
    } component_rgba;

    struct
    {
        u8 alpha;
        u8 red;
        u8 green;
        u8 blue;
    } component_argb;

    struct
    {
        u8 blue;
        u8 green;
        u8 red;
        u8 alpha;
    } component_bgra;
    
};

// --- Floating-point RGBA Color -----------------------------------------------
//
// A floating point RGBA color format for your pleasure. The default clamp
// places the color range between [0.0, 1.0], otherwise providing a range will
// clamp color to that range. Normalize will compress it [0.0, 1.0] for you.
//

class RGBAColor
{

    public:
        RGBAColor       clamp() const;
        RGBAColor       clamp(r32 lower_bound, r32 upper_bound) const;
        RGBAColor       normalize() const;

        packed_color    pack_to_bgra() const;
        packed_color    pack_to_abgr() const;
        packed_color    pack_to_argb() const;
        packed_color    pack_to_rgba() const;

    public:
        r32 red;
        r32 green;
        r32 blue;
        r32 alpha;

};

#endif
