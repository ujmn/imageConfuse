#ifndef IMAGEOBFUSCATER_H
#define IMAGEOBFUSCATER_H

#include <string>
#include <regex>

#include <cstdint>
#include <vector>

struct rect {
    uint32_t x,y;
    uint32_t width,height;
};

class ImageObfuscater
{
public:
    ImageObfuscater();
    ImageObfuscater( uint32_t * data, uint32_t img_width, uint32_t img_height, uint32_t cell_size );
    void obfuscate();
    bool valid();

private:
    void swap_pixel( uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2 );
    void swap_cell( rect a, rect b );
private:
    std::vector<std::vector<rect>> 		main_cells;
        std::vector<rect> 					bottom_sides;
        std::vector<rect> 					right_sides;
        rect 								corner;
        ////////////////////////////////
        uint32_t*							bitmap_data;
        uint32_t                            bitmap_width;
        uint32_t                            bitmap_height;
};

#endif // IMAGEOBFUSCATER_H
