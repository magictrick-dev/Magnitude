#include <graphics/bitmap.hpp>

BitmapImageWrapper::
BitmapImageWrapper(i32 width, i32 height)
{

    // Ensure that the dimensions are actually sane.
    MAG_ASSERT(width > 0 && height > 0);

    // Calculate the sizes.
    u64 pixel_buffer_size = width * height * sizeof(pcolor);
    u64 total_size = sizeof(bitmap_image) + pixel_buffer_size;

    // Describe the file header.
    this->image.file_header.magic_constant  = 'BM';
    this->image.file_header.file_size       = total_size;
    this->image.file_header.unused          = 0;
    this->image.file_header.data_offset     = sizeof(bitmap_image);

    // Describe the info header.
    this->image.info_header.structure_size  = sizeof(bitmap_info_header);
    this->image.info_header.width           = width;
    this->image.info_header.height          = height;
    this->image.info_header.planes          = 1;
    this->image.info_header.bitcount        = 32;
    this->image.info_header.compression     = 0;
    this->image.info_header.image_size      = pixel_buffer_size;
    this->image.info_header.x_ppm           = 0;
    this->image.info_header.y_ppm           = 0;
    this->image.info_header.clear_used      = 0;
    this->image.info_header.clear_important = 0;
    this->image.info_header.mask_red        = 0x000000FF;
    this->image.info_header.mask_green      = 0x0000FF00;
    this->image.info_header.mask_blue       = 0x00FF0000;
    this->image.info_header.mask_alpha      = 0xFF000000;

    // Allocate the buffer and then clear it out.
    this->data = (u32*)MAG_MEMORY_ALLOC(this->image.info_header.image_size);
    memset(this->data, 0x00, width*height*sizeof(pcolor));

}

BitmapImageWrapper::
BitmapImageWrapper(const BitmapImageWrapper& copy)
{

    this->image = copy.image;
    this->data = (u32*)MAG_MEMORY_ALLOC(this->image.info_header.image_size);
    memcpy(this->data, copy.data, this->image.info_header.image_size);

}

BitmapImageWrapper::
~BitmapImageWrapper()
{

    if (this->data != nullptr) MAG_MEMORY_FREE(this->data);

}

BitmapImageWrapper& BitmapImageWrapper::
operator=(const BitmapImageWrapper& assignment)
{

    if (this->data != nullptr) MAG_MEMORY_FREE(this->data);
    this->image = assignment.image;
    this->data = (u32*)MAG_MEMORY_ALLOC(this->image.info_header.image_size);
    memcpy(this->data, assignment.data, this->image.info_header.image_size);
    return *this;

}

void BitmapImageWrapper::
resize(i32 width, i32 height)
{

    // TODO(Chris): We are basically wiping out the old image. We should do a
    //              leetcode medium and actually copy the old image over to the
    //              new buffer when we resize.
    //
    //              If we wanted it to be a leet code *extreme*, we could offer
    //              multiple methods of resizing, such as truncation, stretching,
    //              or interpolating... perhaps when I have the passion for it.

    MAG_ASSERT(width > 0 && height > 0);

    u64 pixel_buffer_size = width * height * sizeof(pcolor);
    u64 total_size = sizeof(bitmap_image) + pixel_buffer_size;

    if (this->data != nullptr) MAG_MEMORY_FREE(this->data);
    this->image.file_header.file_size       = total_size;
    this->image.info_header.width           = width;
    this->image.info_header.height          = height;
    this->image.info_header.image_size      = pixel_buffer_size;

    this->data = (u32*)MAG_MEMORY_ALLOC(this->image.info_header.image_size);
    memset(this->data, 0x00, width*height*sizeof(pcolor));

}

void BitmapImageWrapper::
set_pixel(i32 x, i32 y, pcolor pixel)
{

    if (x < 0 && x >= this->image.info_header.width &&
        y < 0 && y >= this->image.info_header.height) return;

    u64 offset = y * this->image.info_header.width + x;
    this->data[offset] = pixel.pack;

}

pcolor BitmapImageWrapper::
get_pixel(i32 x, i32 y) const
{

    pcolor result = {0};
    if (x < 0 && x >= this->image.info_header.width &&
        y < 0 && y >= this->image.info_header.height) return result;

    u64 offset = y * this->image.info_header.width + x;
    result.pack = this->data[offset];
    return result;


}

u32* BitmapImageWrapper::
get_data() const
{

    return this->data;

}

bitmap_image BitmapImageWrapper::
get_image_format() const
{

    return this->image;

}
