#ifndef MAGNITUDE_RENDERER_PIPELINE_HPP
#define MAGNITUDE_RENDERER_PIPELINE_HPP
#include <definitions.hpp>
#include <linear.hpp>
#include <vector>
#include <platform/opengl.hpp>

#define PIPELINE_POINTS_FLUSH_LIMIT     128
#define PIPELINE_LINE_FLUSH_LIMIT       128

struct point_object
{
    vec3 position;
    r32 radius;
};

struct line_object
{
    vec3 a;
    vec3 b;
    r32 width;
};

// --- Render Pipeline ---------------------------------------------------------
//
// The render pipeline is where the render magic happens. Since we are aiming for
// a consistent system of shading, each pipeline uses a variant of the same shader
// with the same inputs. This allows each one access to all the necessary information
// needed to perform their functions, even if they don't need all the information to
// perform the actual rendering.
//
// 
//

class RendererPipeline
{

    public:
        static inline vec3      camera_at       = { 0.0f,  0.0f, -1.0f };
        static inline vec3      camera_eye      = { 0.0f,  0.0f,  0.0f };
        static inline vec3      camera_up       = { 0.0f,  1.0f,  0.0f };
        static inline r32       camera_fov      = 90.0f;
        static inline r32       clipping_near   = 1.0f;
        static inline r32       clipping_far    = 1'000'000'000.0f;

        static inline mat4      projection;
        static inline mat4      view;

        // Pipeline rendering for each pipeline type.
        static void             point_pipeline(vec3 position, r32 radius);
        static void             line_pipeline(vec3 a, vec3 b, r32 width);
        static void             flush_point_pipeline();
        static void             flush_line_pipeline();
        static void             flush_all_pipelines();

        // Batching storage.
        static inline std::vector<point_object> point_objects;
        static inline std::vector<line_object> line_objects;

};

#endif
