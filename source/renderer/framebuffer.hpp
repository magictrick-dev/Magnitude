#ifndef MAGNITUDE_RENDERER_FRAMEBUFFER_HPP
#define MAGNITUDE_RENDERER_FRAMEBUFFER_HPP
#include <definitions.hpp>
#include <glad/glad.h>

// --- Framebuffer -------------------------------------------------------------
//
// Have you ever wanted to render to something other than a screen? The framebuffer
// class is the class for you!
//
// If you bind it, you're able to make all subsequent render calls go to the
// framebuffer itself. You can then take the texture it produces and render it
// to the screen, if you so choose.
//

class Framebuffer
{

    public:
                            Framebuffer();
                            Framebuffer(i32 width, i32 height);
        virtual            ~Framebuffer();

        bool                bind();
        bool                unbind();
        bool                resize(i32 width, i32 height);

        inline u32          get_texture() const { return this->texture_identifier; }
        inline i32          get_width() const { return this->width; }
        inline i32          get_height() const { return this->height; }

    protected:
        i32 width;
        i32 height;

        u32 vao_identifier;
        u32 frame_identifier;
        u32 depth_identifier;
        u32 texture_identifier;


};

#endif
