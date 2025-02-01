#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lvgl/lvgl.h"
#include "lvgl/src/core/lv_global.h"

#include "src/ui/ui.h"
#include "src/ui/screens.h"
#include "src/input/input.h"

static void lv_linux_disp_init(void)
{
    const char *device = "/dev/dri/card0";
    lv_display_t * disp = lv_linux_drm_create();

    lv_linux_drm_set_file(disp, device, -1);
}

int main(int argc, char **argv)
{
    // setup keybard inout from stdin
    set_stdin_nonblock();
    printf("WASD Navigation active (Q to quit)\n");

    /* Initialize LVGL. */
    lv_init();

    /* Initialize libDRM */
    lv_linux_disp_init();

    // Create a virtual keyboard input device
    lv_indev_t * kb_indev = create_virtual_keyboard();

    // Check if the virtual keyboard was created successfully
    if (kb_indev) {
        printf("Virtual keyboard created successfully!\n");
    } else {
        printf("Error: Could not create virtual keyboard!\n");
    }

    // call this before "ui_init()"
    ui_create_groups();

    // set group for your input device
    lv_indev_set_group(kb_indev, groups.mainGroup);

    ui_init();

    while (1) {

        // Handle keyboard input
        handle_keyboard_input();

        lv_task_handler();
        //ui_tick();
        usleep(5000);  // Sleep for a short period to allow LVGL to update
    }

    //lv_linux_run_loop();
    //sleep(50);

    return 0;
}
