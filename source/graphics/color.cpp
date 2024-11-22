#include <graphics/color.hpp>

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

RGBAColor RGBAColor::
normalize() const
{

    // TODO(Chris): SIMD could save us from this madness.
    const r32 rsq = this->red       * this->red;
    const r32 gsq = this->green     * this->green;
    const r32 bsq = this->blue      * this->blue;
    const r32 sum = rsq + gsq + bsq;
    const r32 div = sqrtf(sum);

    const r32 alpha_component = (this->alpha < 0.0) ? 0.0f: ((this->alpha > 1.0f) ? 1.0f: this->alpha);
    return { this->red / div, this->green / div, this->blue / div, alpha_component };

}

packed_color RGBAColor::
pack_to_abgr() const
{

    // TODO(Chris): We will want to epsilon shift and truncate at some point to
    //              enforce a true range of values.

    packed_color result = {0};
    result.component_abgr.red    = (u8)(this->red    * 255.0f);
    result.component_abgr.green  = (u8)(this->green  * 255.0f);
    result.component_abgr.blue   = (u8)(this->blue   * 255.0f);
    result.component_abgr.alpha  = (u8)(this->alpha  * 255.0f);
    return result;

}

packed_color RGBAColor::
pack_to_bgra() const
{

    // TODO(Chris): We will want to epsilon shift and truncate at some point to
    //              enforce a true range of values.

    packed_color result = {0};
    result.component_bgra.red    = (u8)(this->red    * 255.0f);
    result.component_bgra.green  = (u8)(this->green  * 255.0f);
    result.component_bgra.blue   = (u8)(this->blue   * 255.0f);
    result.component_bgra.alpha  = (u8)(this->alpha  * 255.0f);
    return result;

}

packed_color RGBAColor::
pack_to_rgba() const
{

    // TODO(Chris): We will want to epsilon shift and truncate at some point to
    //              enforce a true range of values.

    packed_color result = {0};
    result.component_rgba.red    = (u8)(this->red    * 255.0f);
    result.component_rgba.green  = (u8)(this->green  * 255.0f);
    result.component_rgba.blue   = (u8)(this->blue   * 255.0f);
    result.component_rgba.alpha  = (u8)(this->alpha  * 255.0f);
    return result;

}

packed_color RGBAColor::
pack_to_argb() const
{

    // TODO(Chris): We will want to epsilon shift and truncate at some point to
    //              enforce a true range of values.

    packed_color result = {0};
    result.component_argb.red    = (u8)(this->red    * 255.0f);
    result.component_argb.green  = (u8)(this->green  * 255.0f);
    result.component_argb.blue   = (u8)(this->blue   * 255.0f);
    result.component_argb.alpha  = (u8)(this->alpha  * 255.0f);
    return result;

}
