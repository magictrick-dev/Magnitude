#include <graphics/color.hpp>

#define MAG_DEF_GAMMA 2.2f
#define MAG_TRUNC_EPSILON 0.5f

RGBAColor RGBAColor::
clamp() const
{
    
    return this->clamp(0.0f, 1.0f);

}

RGBAColor RGBAColor::
clamp(r32 lower_bound, r32 upper_bound) const
{

    // Ternary soup.
    const r32 red_component = (this->red < lower_bound) ? lower_bound : 
        ((this->red > upper_bound) ? upper_bound : this->red);
    const r32 green_component = (this->green < lower_bound) ? lower_bound : 
        ((this->green > upper_bound) ? upper_bound : this->green);
    const r32 blue_component = (this->blue < lower_bound) ? lower_bound : 
        ((this->blue > upper_bound) ? upper_bound : this->blue);
    const r32 alpha_component = (this->alpha < lower_bound) ? lower_bound : 
        ((this->alpha > upper_bound) ? upper_bound : this->alpha);

    return { red_component, green_component, blue_component, alpha_component };

}

packed_color RGBAColor::
pack_to_abgr() const
{

    r32 result_red      = powf(this->red,   (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    r32 result_green    = powf(this->green, (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    r32 result_blue     = powf(this->blue,  (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    r32 result_alpha    = powf(this->alpha, (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    result_red         += MAG_TRUNC_EPSILON;
    result_green       += MAG_TRUNC_EPSILON;
    result_blue        += MAG_TRUNC_EPSILON;
    result_alpha       += MAG_TRUNC_EPSILON;

    packed_color result = {0};
    result.component_abgr.red    = (u8)(result_red);
    result.component_abgr.green  = (u8)(result_green);
    result.component_abgr.blue   = (u8)(result_blue);
    result.component_abgr.alpha  = (u8)(result_alpha);
    return result;

}

packed_color RGBAColor::
pack_to_bgra() const
{

    r32 result_red      = powf(this->red,   (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    r32 result_green    = powf(this->green, (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    r32 result_blue     = powf(this->blue,  (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    r32 result_alpha    = powf(this->alpha, (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    result_red         += MAG_TRUNC_EPSILON;
    result_green       += MAG_TRUNC_EPSILON;
    result_blue        += MAG_TRUNC_EPSILON;
    result_alpha       += MAG_TRUNC_EPSILON;

    packed_color result = {0};
    result.component_bgra.red    = (u8)(result_red);
    result.component_bgra.green  = (u8)(result_green);
    result.component_bgra.blue   = (u8)(result_blue);
    result.component_bgra.alpha  = (u8)(result_alpha);
    return result;

}

packed_color RGBAColor::
pack_to_rgba() const
{

    r32 result_red      = powf(this->red,   (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    r32 result_green    = powf(this->green, (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    r32 result_blue     = powf(this->blue,  (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    r32 result_alpha    = powf(this->alpha, (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    result_red         += MAG_TRUNC_EPSILON;
    result_green       += MAG_TRUNC_EPSILON;
    result_blue        += MAG_TRUNC_EPSILON;
    result_alpha       += MAG_TRUNC_EPSILON;

    packed_color result = {0};
    result.component_rgba.red    = (u8)(result_red);
    result.component_rgba.green  = (u8)(result_green);
    result.component_rgba.blue   = (u8)(result_blue);
    result.component_rgba.alpha  = (u8)(result_alpha);
    return result;

}

packed_color RGBAColor::
pack_to_argb() const
{

    r32 result_red      = powf(this->red,   (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    r32 result_green    = powf(this->green, (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    r32 result_blue     = powf(this->blue,  (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    r32 result_alpha    = powf(this->alpha, (1.0f / MAG_DEF_GAMMA)) * 255.0f;
    result_red         += MAG_TRUNC_EPSILON;
    result_green       += MAG_TRUNC_EPSILON;
    result_blue        += MAG_TRUNC_EPSILON;
    result_alpha       += MAG_TRUNC_EPSILON;

    packed_color result = {0};
    result.component_argb.red    = (u8)(result_red);
    result.component_argb.green  = (u8)(result_green);
    result.component_argb.blue   = (u8)(result_blue);
    result.component_argb.alpha  = (u8)(result_alpha);
    return result;

}
