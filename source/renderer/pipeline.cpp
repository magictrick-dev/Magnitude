#include <renderer/pipeline.hpp>

// --- Point Pipeline ----------------------------------------------------------
//
// Basically, it just renders a single point to the screen.
//

void RendererPipeline::
point_pipeline(vec3 position, r32 radius)
{

    RendererPipeline::point_objects.push_back({
        .position = position, 
        .radius = radius,
    });

    if (RendererPipeline::point_objects.size() >= PIPELINE_POINTS_FLUSH_LIMIT)
        RendererPipeline::flush_point_pipeline();

}

void RendererPipeline::
flush_point_pipeline()
{

    // Where the rendering occurs.

}

// --- Line Pipeline -----------------------------------------------------------
//
// Lines pipelines render lines to the screen.
//

void RendererPipeline::
point_pipeline(vec3 a, vec3 b, r32 width)
{

    RendererPipeline::line_objects.push_back({
        .a = a, 
        .b = b,
        .width = width,
    });

    if (RendererPipeline::line_objects.size() >= PIPELINE_LINE_FLUSH_LIMIT)
        RendererPipeline::flush_line_pipeline();

}

void RendererPipeline::
flush_point_pipeline()
{

    // Where the rendering occurs.

}

// --- Helpers -----------------------------------------------------------------

void RendererPipeline::
flush_all_pipelines()
{

    RendererPipeline::flush_point_pipeline();
    RendererPipeline::flush_line_pipeline();

}
