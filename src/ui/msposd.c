#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cairo/cairo.h>
#include <msposd.h>
#include "../../lvgl/lvgl.h"


cairo_t * cr;
cairo_surface_t * shm_surface = NULL;
unsigned char * shm_data = NULL;
extern lv_obj_t * fly_screen;

// Function to initialize shared memory
void init_shm(const char *shm_name, int width, int height) {
    printf("Creating shm region %s with size: %ix%i\n", shm_name, width, height);

    // Calculate total shared memory size
    size_t shm_size = sizeof(SharedMemoryRegion) + (width * height * 4); // Metadata + Image data

    // Create shared memory region
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Failed to create shared memory");
        return;
    }

    if (ftruncate(shm_fd, shm_size) == -1) {
        perror("Failed to set shared memory size");
        shm_unlink(shm_name);
        close(shm_fd);
        return;
    }

    // Map shared memory to process address space
    SharedMemoryRegion *shm_region = (SharedMemoryRegion *)mmap(
        0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0
    );
    if (shm_region == MAP_FAILED) {
        perror("Failed to map shared memory");
        shm_unlink(shm_name);
        close(shm_fd);
        return;
    }

    // Write metadata
    shm_region->width = width;
    shm_region->height = height;

    // Create Cairo surface for the image data
    shm_surface = cairo_image_surface_create_for_data(
        shm_region->data, CAIRO_FORMAT_ARGB32, width, height, width * 4
    );
	cr = cairo_create (shm_surface);    

    // Store pointer for cleanup (if needed)
    shm_data = (unsigned char *)shm_region->data;

    // Cleanup (optional, depending on your use case)
    // cairo_surface_destroy(shm_surface); // Only if you don't need it later
    // munmap(shm_region, shm_size); // Only if you don't need it later
    // close(shm_fd); // Only if you don't need it later
}



    // // Cleanup
    // cairo_destroy(cr);
    // cairo_surface_destroy(surface);
/**
 * Function to refresh LVGL canvas (forces redraw)
 */
static void refresh_lvgl_canvas(lv_obj_t * canvas) {
    //if ( fly_screen == lv_screen_active()) {
        if (shm_data) {
            lv_canvas_set_buffer(canvas, shm_data, lv_display_get_horizontal_resolution(NULL), lv_display_get_vertical_resolution(NULL), LV_COLOR_FORMAT_ARGB8888);
            lv_obj_invalidate(canvas);  // Request LVGL to redraw
        }
        printf("Refresh done ...\n");
    // }
}

/**
 * Timer callback to refresh the canvas periodically
 */
static void canvas_refresh_cb(lv_timer_t *timer) {
    refresh_lvgl_canvas(lv_timer_get_user_data(timer));
}

lv_obj_t * create_msposd_screen(lv_group_t * group)
{

    init_shm("msposd", lv_display_get_horizontal_resolution(NULL), lv_display_get_vertical_resolution(NULL));
    fly_screen = lv_obj_create(NULL);
    lv_obj_t * canvas = lv_canvas_create(lv_screen_active());
    lv_obj_set_pos(canvas, 0, 0);
    lv_obj_set_size(canvas, lv_display_get_horizontal_resolution(NULL), lv_display_get_vertical_resolution(NULL));
    lv_timer_create(canvas_refresh_cb, 100, canvas);  // Refresh every 33ms (~30 FPS)

    return canvas;
}
