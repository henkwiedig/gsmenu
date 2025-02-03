#include <libdrm/drm_mode.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include "lvgl/lvgl.h"
#include "lvgl/src/core/lv_global.h"

#include "src/ui/ui.h"
#include "src/ui/screens.h"
#include "src/input/input.h"

#define BUFFER_CNT 2

typedef struct {
    uint32_t handle;
    uint32_t pitch;
    uint32_t offset;
    unsigned long int size;
    uint8_t * map;
    uint32_t fb_handle;
} drm_buffer_t;

typedef struct {
    int fd;
    uint32_t conn_id, enc_id, crtc_id, plane_id, crtc_idx;
    uint32_t width, height;
    uint32_t mmWidth, mmHeight;
    uint32_t fourcc;
    drmModeModeInfo mode;
    uint32_t blob_id;
    drmModeCrtc * saved_crtc;
    drmModeAtomicReq * req;
    drmEventContext drm_event_ctx;
    drmModePlane * plane;
    drmModeCrtc * crtc;
    drmModeConnector * conn;
    uint32_t count_plane_props;
    uint32_t count_crtc_props;
    uint32_t count_conn_props;
    drmModePropertyPtr plane_props[128];
    drmModePropertyPtr crtc_props[128];
    drmModePropertyPtr conn_props[128];
    drm_buffer_t drm_bufs[BUFFER_CNT];
    drm_buffer_t * act_buf;
} drm_dev_t;

static void lv_linux_disp_init(void)
{
    const char *device = "/dev/dri/card0";
    lv_display_t * disp = lv_linux_drm_create();

    lv_obj_set_style_bg_opa(lv_screen_active(), LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(lv_layer_bottom(), LV_OPA_TRANSP, LV_PART_MAIN);
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_ARGB8888);  // seems to be ignored in drm driver, currently patched manually
    //lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);

    lv_linux_drm_set_file(disp, device, -1);

    drm_dev_t *drm_dev = (drm_dev_t *) lv_display_get_driver_data(disp);
    //set zpos
    if (drmModeObjectSetProperty(drm_dev->fd, 54, DRM_MODE_OBJECT_PLANE, 59, 6) < 0) {
        printf("Failed to set zpos property\n");
    }

    lv_display_set_resolution(disp,800,480);
    lv_display_set_offset(disp,200,200);
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
    // ui_create_groups();

    // set group for your input device
    // lv_indev_set_group(kb_indev, groups.mainGroup);

    ui_init();

    // lv_obj_t * label = lv_label_create(lv_screen_active());
    // lv_label_set_text(label, "Hello world");
    // //lv_obj_set_style_text_font(label,LV_FONT_MONTSERRAT_48,LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
    // lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    // //lv_obj_set_style_opa(label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);    


    // lv_obj_t *obj = lv_obj_create(0);
    // lv_obj_set_pos(obj, 0, 0);
    // lv_obj_set_size(obj, 800, 480);
    // lv_obj_set_style_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // lv_obj_t * label = lv_label_create(obj);
    // lv_obj_t * label = lv_label_create(lv_screen_active());
    // lv_label_set_text(label, "Hello world");

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