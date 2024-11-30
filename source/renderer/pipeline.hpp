#ifndef MAGNITUDE_RENDERER_PIPELINE_HPP
#define MAGNITUDE_RENDERER_PIPELINE_HPP
#include <definitions.hpp>
#include <linear.hpp>
#include <vector>

class RendererPipeline
{

    public:
        static inline std::vector<mat4> transform_stack;

        static inline vec4      camera_at       = { 0.0f,  0.0f, -1.0f,  1.0f };
        static inline vec4      camera_eye      = { 0.0f,  0.0f,  0.0f,  1.0f };
        static inline vec4      camera_up       = { 0.0f,  1.0f,  0.0f,  1.0f };
        static inline r32       camera_fov      = 90.0f;
        static inline r32       clipping_near   = 1.0f;
        static inline r32       clipping_far    = 1'000'000'000.0f;

};

#endif
