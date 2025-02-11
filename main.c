#include <libdrm/drm_mode.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include "lvgl/lvgl.h"
#include "lvgl/src/core/lv_global.h"
#include "src/ui/msposd.h"
#include "src/ui/ui.h"
#include "src/ui/test.h"
#include "src/input/input.h"

lv_indev_t * indev_drv;
lv_group_t * group;
lv_obj_t * menu;
lv_obj_t * menu_screen;
lv_obj_t * fly_screen;

#ifdef USE_SIMULATOR
static void lv_sdl_disp_init(void)
{
    lv_sdl_window_create(1920,1080);
    // lv_obj_set_style_bg_opa(lv_screen_active(), LV_OPA_TRANSP, LV_PART_MAIN);
    // lv_obj_set_style_bg_opa(lv_layer_bottom(), LV_OPA_TRANSP, LV_PART_MAIN);    
}
#else
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
    lv_display_t * disp = lv_linux_drm_create_res(1920, 1080, 60);

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

    lv_display_set_resolution(disp,1920,1080);
    //lv_display_set_offset(disp,200,200);
}
#endif

extern gsmenu_control_mode_t control_mode;

void my_log_cb(lv_log_level_t level, const char * buf)
{
  printf("%s\n", buf);
}


int main(int argc, char **argv)
{

    /* Initialize LVGL. */
    lv_init();

    lv_log_register_print_cb(my_log_cb);    

#ifdef USE_SIMULATOR
    /* Initialize sdl driver */
    lv_sdl_disp_init();
#else
    /* Initialize libDRM */
    lv_linux_disp_init();    
#endif
    // create_virtual_keyboard
    indev_drv = create_virtual_keyboard();

    // Create an input group
    lv_group_set_default(lv_group_create());    

    // menu_screen = create_menu(lv_group_get_default());
    fly_screen = create_msposd_screen(lv_group_get_default());
    //lv_obj_set_style_bg_opa(menu, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_indev_set_group(indev_drv, lv_group_get_default());

    // lv_screen_load_anim(fly_screen,LV_SCR_LOAD_ANIM_FADE_IN,200,0,false);

    while (1) {

        // Handle keyboard input
        handle_keyboard_input();

        lv_task_handler();
        //ui_tick();
        usleep(5000);  // Sleep for a short period to allow LVGL to update
    }

    return 0;
}