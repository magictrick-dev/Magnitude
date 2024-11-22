#ifndef MAGNITUDE_GRAPHICS_BITMAP_HPP
#define MAGNITUDE_GRAPHICS_BITMAP_HPP
#include <definitions.hpp>
#include <graphics/color.hpp>

// --- Bitmap ------------------------------------------------------------------
//
// The following is a conformant bitmap file standard layout. When serializing
// bitmap images, the structure bitmap_image is conformant up to the mask, which
// is partially standard V4 as defined by Microsoft. Since we don't care what
// comes afterwards, serializing this isn't really a problem given that the
// V1 standard is obeyed.
//
// You also probably don't want to deal with this structure set on your own,
// so a provided API called BitmapImageWrapper is provided for you.
//

struct bitmap_file_header
{
    u16     magic_constant; // 'BM'
    u32     file_size;
    u32     unused;
    u32     data_offset;
};

struct bitmap_info_header
{

    u32     structure_size;
    u16     width;
    u16     height;
    u8      planes;
    u8      bitcount;
    u32     compression;
    u32     image_size;
    u16     x_ppm;
    u16     y_ppm;
    u32     clear_used;
    u32     clear_important;
    u32     mask_red;
    u32     mask_green;
    u32     mask_blue;
    u32     mask_alpha;

};

struct bitmap_image
{
    bitmap_file_header file_header;
    bitmap_info_header info_header;
};

// --- Bitmap Image Wrapper ----------------------------------------------------
//
// For your bitmap manipulation pleasure. The main point of this class is to
// provide image manipulation tools and offload the initial booking keeping needed
// to properly serialize a bitmap image.
//

class BitmapImageWrapper
{

    public:
                            BitmapImageWrapper(i32 width, i32 height);
                            BitmapImageWrapper(const BitmapImageWrapper& copy);
        virtual            ~BitmapImageWrapper();

        BitmapImageWrapper& operator=(const BitmapImageWrapper& assignment);

        i32                 get_width() const;
        i32                 get_height() const;

    protected:
        bitmap_image        image;
        u32 *data;

};

#endif
