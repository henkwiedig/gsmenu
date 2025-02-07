#include <libdrm/drm_mode.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include "lvgl/lvgl.h"
#include "lvgl/src/core/lv_global.h"
#include "src/ui/ui.h"
#include "src/input/input.h"

lv_indev_t * indev_drv;
lv_group_t * group;
lv_obj_t * menu;

#ifdef USE_SIMULATOR
static void lv_sdl_disp_init(void)
{
    lv_sdl_window_create(800,480);
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
#endif

extern gsmenu_control_mode_t control_mode;

static lv_obj_t * ta_ssid;

static void ta_event_cb(lv_event_t * e);
static void kb_event_cb(lv_event_t * e);


static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);
    
    if(code == LV_EVENT_CLICKED) {
        lv_keyboard_set_textarea(kb, ta_ssid);
        lv_obj_set_style_max_height(kb, LV_VER_RES * 2 / 3, 0);
        lv_obj_update_layout(lv_obj_get_parent(lv_obj_get_parent(kb)));   /*Be sure the sizes are recalculated*/
        lv_obj_set_height(lv_obj_get_parent(lv_obj_get_parent(kb)), LV_VER_RES - lv_obj_get_height(kb));
        lv_obj_remove_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_scroll_to_view_recursive(ta_ssid, LV_ANIM_OFF);
        lv_indev_wait_release(lv_event_get_param(e));
        lv_group_focus_obj(kb);
    }
}

void menux(lv_obj_t * parent) {
    
    lv_obj_t *obj = lv_textarea_create(parent);
    lv_textarea_set_one_line(obj, true);
    ta_ssid = lv_textarea_create(parent);
    lv_textarea_set_one_line(ta_ssid, true);
    lv_obj_t * button = lv_button_create(parent);
    obj = lv_label_create(button);
    lv_label_set_text(obj,LV_SYMBOL_KEYBOARD);
    obj = lv_textarea_create(parent);
    lv_textarea_set_one_line(obj, true);

    lv_obj_t * kb = lv_keyboard_create(parent);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(button, btn_event_cb, LV_EVENT_ALL, kb);    
    lv_keyboard_set_textarea(kb, ta_ssid);
    lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_ALL,kb);

}
static void kb_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);

    if (code == LV_EVENT_FOCUSED) {
        printf("forcus\n");
        control_mode = GSMENU_CONTROL_MODE_EDIT;
    }
    else if (code == LV_EVENT_DEFOCUSED)
    {
        printf("de-forcus\n");
        control_mode = GSMENU_CONTROL_MODE_NAV;
    }
    else if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_set_height(lv_obj_get_parent(lv_obj_get_parent(kb)), LV_VER_RES);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, ta);   /*To forget the last clicked object to make it focusable again*/
        lv_group_focus_obj(lv_keyboard_get_textarea(kb));
}
    
}
// static void ta_event_cb(lv_event_t * e)
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     lv_obj_t * ta = lv_event_get_target(e);
//     lv_obj_t * kb = lv_event_get_user_data(e);
//     // if(code == LV_EVENT_FOCUSED) {
//     //     lv_keyboard_set_textarea(kb, ta);
//     //     lv_obj_set_style_max_height(kb, LV_VER_RES * 2 / 3, 0);
//     //     lv_obj_update_layout(lv_obj_get_parent(lv_obj_get_parent(kb)));   /*Be sure the sizes are recalculated*/
//     //     lv_obj_set_height(lv_obj_get_parent(lv_obj_get_parent(kb)), LV_VER_RES - lv_obj_get_height(kb));
//     //     lv_obj_remove_flag(kb, LV_OBJ_FLAG_HIDDEN);
//     //     lv_obj_scroll_to_view_recursive(ta, LV_ANIM_OFF);
//     //     lv_indev_wait_release(lv_event_get_param(e));
//     //     lv_group_focus_obj(kb);
//     // }
//     // else
//     if(code == LV_EVENT_DEFOCUSED && control_mode == GSMENU_CONTROL_MODE_EDIT) {
//         printf("here\n");
//         lv_keyboard_set_textarea(kb, NULL);
//         lv_obj_set_height(lv_obj_get_parent(lv_obj_get_parent(kb)), LV_VER_RES);
//         lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
//         lv_indev_reset(NULL, ta);
//     }
//     else if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
//         lv_obj_set_height(lv_obj_get_parent(lv_obj_get_parent(kb)), LV_VER_RES);
//         lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
//         lv_indev_reset(NULL, ta);   /*To forget the last clicked object to make it focusable again*/
//     }
// }

int main(int argc, char **argv)
{

    /* Initialize LVGL. */
    lv_init();

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

    menu = create_menu(lv_group_get_default());
    // lv_obj_t * root = lv_obj_create(lv_screen_active());
    // lv_obj_add_flag(root, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    // lv_obj_set_size(root,800,480);
    // lv_obj_set_layout(root, LV_LAYOUT_FLEX);
    // lv_obj_set_flex_flow(root, LV_FLEX_FLOW_COLUMN);
    // menux(root);

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