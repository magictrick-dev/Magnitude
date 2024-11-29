#include <renderer/framebuffer.hpp>

ccptr framebuffer_vertex_shader = R"(#version 430 core
layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_texcoords;

out vec2 tex_coords;

void main()
{
    gl_Position = vec4(a_position, 0.0f, 1.0f);
    tex_coords = a_texcoords;
}

)";

ccptr framebuffer_fragment_shader = R"(#version 430 core
out vec4 fragment_color;
in vec2 tex_coords;

uniform sampler2D screen_texture;

void main()
{
    fragment_color = texture(screen_texture, tex_coords);
}

)";

Framebuffer::
Framebuffer(i32 width, i32 height)
{

    MAG_ASSERT(width > 0 && height > 0);

    // Generate the vao.
    glGenVertexArrays(1, &this->vao_identifier);
    glBindVertexArray(this->vao_identifier);

    // Generate the frame buffer.
    glGenFramebuffers(1, &this->frame_identifier);
    glBindFramebuffer(GL_FRAMEBUFFER, this->frame_identifier);

    // Generate the texture and set the essential properties.
    glGenTextures(1, &this->texture_identifier);
    glBindTexture(GL_TEXTURE_2D, this->texture_identifier);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Generate the depth buffer.
    glGenRenderbuffers(1, &this->depth_identifier);
    glBindRenderbuffer(GL_RENDERBUFFER, this->depth_identifier);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER, this->depth_identifier);

    // Configure the frame buffer to our texture.
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->texture_identifier, 0);
    GLenum draw_buffer[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
    glBindVertexArray(NULL);

    this->width = width;
    this->height = height;

}

Framebuffer::
~Framebuffer()
{

    glDeleteFramebuffers(1, &this->frame_identifier);

}

bool Framebuffer::
bind()
{

    glBindVertexArray(this->vao_identifier);
    glBindFramebuffer(GL_FRAMEBUFFER, this->frame_identifier);
    return true;

}

bool Framebuffer::
unbind()
{

    glBindFramebuffer(GL_FRAMEBUFFER, NULL);
    glBindVertexArray(NULL);
    return true;

}

bool Framebuffer::
resize(i32 width, i32 height)
{

    MAG_ASSERT(width > 0 && height > 0);

    glBindTexture(GL_TEXTURE_2D, this->texture_identifier);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, this->depth_identifier);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depth_identifier);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->texture_identifier, 0);
    GLenum draw_buffer[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buffer);

    MAG_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    this->width = width;
    this->height = height;
    return true;

}

