#pragma once
#include <cairo/cairo.h>
#include <stdint.h>
#include "../../lvgl/lvgl.h"


typedef struct {
    uint16_t width;
    uint16_t height;
    unsigned char data[]; // Flexible array member for image data
} SharedMemoryRegion;

lv_obj_t * create_msposd_screen(lv_group_t * group);