#pragma once
#include <cairo/cairo.h>
#include <stdint.h>

typedef struct {
    uint16_t width;
    uint16_t height;
    unsigned char data[]; // Flexible array member for image data
} SharedMemoryRegion;

void init_shm(const char *shm_name, int width, int height);