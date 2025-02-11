// #include <cairo/cairo.h>
// #include "../../lvgl/lvgl.h"
// #include "msposd.h"

// extern unsigned char *shm_data;
// extern cairo_t * cr;
// extern lv_obj_t * fly_screen;

// /**
//  * Function to refresh LVGL canvas (forces redraw)
//  */
// static void refresh_lvgl_canvas(lv_obj_t * canvas) {
//     //if ( fly_screen == lv_screen_active()) {
//         if (shm_data) {
//             lv_canvas_set_buffer(canvas, shm_data, lv_display_get_horizontal_resolution(NULL), lv_display_get_vertical_resolution(NULL), LV_COLOR_FORMAT_ARGB8888);
//             lv_obj_invalidate(canvas);  // Request LVGL to redraw
//         }
//         printf("Refresh done ...\n");
//     // }
// }

// /**
//  * Timer callback to refresh the canvas periodically
//  */
// static void canvas_refresh_cb(lv_timer_t *timer) {
//     refresh_lvgl_canvas(lv_timer_get_user_data(timer));
// }

// lv_obj_t * create_test_menu(lv_group_t * group)
// {

//     init_shm("msposd", lv_display_get_horizontal_resolution(NULL), lv_display_get_vertical_resolution(NULL));
//     fly_screen = lv_obj_create(NULL);
//     lv_obj_t * canvas = lv_canvas_create(lv_screen_active());
//     lv_obj_set_pos(canvas, 0, 0);
//     lv_obj_set_size(canvas, lv_display_get_horizontal_resolution(NULL), lv_display_get_vertical_resolution(NULL));
//     lv_timer_create(canvas_refresh_cb, 100, canvas);  // Refresh every 33ms (~30 FPS)

//     return canvas;
// }
