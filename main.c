#include <libdrm/drm_mode.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include "lvgl/lvgl.h"
#include "lvgl/src/core/lv_global.h"
#include "src/ui/demo.h"
#include "src/input/input.h"

lv_indev_t * indev_drv;
lv_group_t * group;
lv_obj_t * menu;

static void lv_sdl_disp_init(void)
{
    lv_sdl_window_create(800,480);
}

int main(int argc, char **argv)
{

    /* Initialize LVGL. */
    lv_init();

    /* Initialize driver */
    lv_sdl_disp_init();

    // create_virtual_keyboard
    indev_drv = create_virtual_keyboard();

    // Create an input group
    lv_group_set_default(lv_group_create());    

    menu = lv_example_menu_5(lv_group_get_default());

    lv_indev_set_group(indev_drv, lv_group_get_default());

    while (1) {

        // Handle keyboard input
        handle_keyboard_input();

        lv_task_handler();
        //ui_tick();
        usleep(5000);  // Sleep for a short period to allow LVGL to update
    }

    return 0;
}