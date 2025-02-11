#include <stdio.h>
#include "../../lvgl/lvgl.h"

#include "helper.h"
#include "air_wfbng.h"
#include "air_camera.h"
#include "air_telemetry.h"
#include "gs_system.h"
#include "wifi.h"

static void back_event_handler(lv_event_t * e);
extern lv_obj_t * menu;
extern lv_obj_t * menu_screen;
extern lv_obj_t * fly_screen;
lv_obj_t * root_page;

static void exit_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED) {
        lv_screen_load_anim(fly_screen,LV_SCR_LOAD_ANIM_FADE_IN,200,0,false);
    }
}

lv_obj_t * create_menu(lv_group_t * group)
{
    menu_screen = lv_obj_create(NULL);
    menu = lv_menu_create(menu_screen);

    lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 10), 0);
    lv_obj_add_event_cb(menu, back_event_handler, LV_EVENT_CLICKED, menu);
    lv_obj_set_size(menu, lv_display_get_horizontal_resolution(NULL), lv_display_get_vertical_resolution(NULL));
    lv_obj_center(menu);

    lv_obj_t * cont;
    lv_obj_t * section;

    /*Create sub pages*/
    lv_obj_t * sub_air_wfbng_page = lv_menu_page_create(menu, LV_SYMBOL_WIFI" WFB-NG");
    lv_obj_set_style_pad_hor(sub_air_wfbng_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_air_wfbng_page);
    section = lv_menu_section_create(sub_air_wfbng_page);
    create_air_wfbng_menu(section);

    lv_obj_t * sub_air_camera_page = lv_menu_page_create(menu, LV_SYMBOL_IMAGE" Camera");
    lv_obj_set_style_pad_hor(sub_air_camera_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_air_camera_page);
    section = lv_menu_section_create(sub_air_camera_page);
    create_air_camera_menu(section);

    lv_obj_t * sub_air_telemetry_page = lv_menu_page_create(menu, LV_SYMBOL_DOWNLOAD" Drone Telemetry");
    lv_obj_set_style_pad_hor(sub_air_telemetry_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_air_telemetry_page);
    section = lv_menu_section_create(sub_air_telemetry_page);
    create_air_telemetry_menu(section);

    lv_obj_t * sub_gs_system_page = lv_menu_page_create(menu, LV_SYMBOL_SETTINGS" System");
    lv_obj_set_style_pad_hor(sub_gs_system_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_gs_system_page);
    section = lv_menu_section_create(sub_gs_system_page);
    create_gs_system_menu(section);

    lv_obj_t * sub_wlan_page = lv_menu_page_create(menu, LV_SYMBOL_WIFI" WLAN");
    lv_obj_set_style_pad_hor(sub_wlan_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_wlan_page);
    section = lv_menu_section_create(sub_wlan_page);
    create_wifi_menu(section);

    /*Create a root page*/
    root_page = lv_menu_page_create(menu, "OpenIPC");
    lv_obj_set_style_pad_hor(root_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    create_text(root_page, NULL, "Drone Settings", LV_MENU_ITEM_BUILDER_VARIANT_1,NULL);    
    section = lv_menu_section_create(root_page);
    cont = create_text(section, LV_SYMBOL_WIFI, "WFB-NG", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_air_wfbng_page);
    cont = create_text(section, LV_SYMBOL_IMAGE, "Camera", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_air_camera_page);
    cont = create_text(section, LV_SYMBOL_DOWNLOAD, "Telemetry", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_air_telemetry_page);

    create_text(root_page, NULL, "GS Settings", LV_MENU_ITEM_BUILDER_VARIANT_1,NULL);
    section = lv_menu_section_create(root_page);
    cont = create_text(section, LV_SYMBOL_SETTINGS, "System Settings", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_gs_system_page);
    cont = create_text(section, LV_SYMBOL_WIFI, "WLAN", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_wlan_page);
    cont = create_text(section, LV_SYMBOL_CLOSE, "Exit", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_obj_add_event_cb(cont, exit_handler, LV_EVENT_CLICKED, NULL);
    

    lv_menu_set_sidebar_page(menu, NULL);
    lv_menu_clear_history(menu);
    lv_menu_set_page(menu, root_page);

    return menu_screen;
}

static void back_event_handler(lv_event_t * e)
{
    printf("This should close the menu\n");
}

