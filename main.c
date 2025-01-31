#include <unistd.h>
#include "lvgl/lvgl.h"
#include "lvgl/src/core/lv_global.h"

#include "src/ui/ui.h"

static void lv_linux_disp_init(void)
{
    const char *device = "/dev/dri/card0";
    lv_display_t * disp = lv_linux_drm_create();

    lv_linux_drm_set_file(disp, device, -1);
}

int main(int argc, char **argv)
{

    /* Initialize LVGL. */
    lv_init();

    /* Initialize libDRM */
    lv_linux_disp_init();

    ui_init();

    while (1) {
        lv_task_handler();
        //ui_tick();
        usleep(5000);  // Sleep for a short period to allow LVGL to update
    }

    //lv_linux_run_loop();
    //sleep(50);

    return 0;
}
