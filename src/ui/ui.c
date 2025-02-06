#include <stdio.h>
#include "../../lvgl/lvgl.h"

#include "helper.h"
#include "wifi.h"

static void back_event_handler(lv_event_t * e);
lv_obj_t * root_page;

lv_obj_t * create_menu(lv_group_t * group)
{
    lv_obj_t * menu = lv_menu_create(lv_screen_active());

    lv_color_t bg_color = lv_obj_get_style_bg_color(menu, 0);
    if(lv_color_brightness(bg_color) > 127) {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 10), 0);
    }
    else {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 50), 0);
    }
    //lv_menu_set_mode_root_back_button(menu, LV_MENU_ROOT_BACK_BUTTON_ENABLED);
    lv_obj_add_event_cb(menu, back_event_handler, LV_EVENT_CLICKED, menu);
    lv_obj_set_size(menu, lv_display_get_horizontal_resolution(NULL), lv_display_get_vertical_resolution(NULL));
    lv_obj_center(menu);

    lv_obj_t * cont;
    lv_obj_t * section;

    /*Create sub pages*/
    lv_obj_t * sub_wfbng_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_wfbng_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_wfbng_page);
    section = lv_menu_section_create(sub_wfbng_page);
    create_slider(section, LV_SYMBOL_SETTINGS, "Velocity", 0, 150, 120);
    create_slider(section, LV_SYMBOL_SETTINGS, "Acceleration", 0, 150, 50);
    create_slider(section, LV_SYMBOL_SETTINGS, "Weight limit", 0, 150, 80);
    create_dropdown(section,LV_SYMBOL_SETTINGS, "Frequency", "Foo\nBar\nBuzz");

    lv_obj_t * sub_camera_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_camera_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_camera_page);
    section = lv_menu_section_create(sub_camera_page);
    create_switch(section, LV_SYMBOL_AUDIO, "FPV", false);

    lv_obj_t * sub_telemetry_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_telemetry_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_telemetry_page);
    section = lv_menu_section_create(sub_telemetry_page);
    create_slider(section, LV_SYMBOL_SETTINGS, "Brightness", 0, 150, 100);

    lv_obj_t * sub_wlan_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_wlan_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_wlan_page);
    section = lv_menu_section_create(sub_wlan_page);
    create_wifi_menu(section);

    /*Create a root page*/
    root_page = lv_menu_page_create(menu, "Drone Settings");
    lv_obj_set_style_pad_hor(root_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    section = lv_menu_section_create(root_page);
    cont = create_text(section, LV_SYMBOL_WIFI, "WFB-NG", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_wfbng_page);
    cont = create_text(section, LV_SYMBOL_IMAGE, "Camera", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_camera_page);
    cont = create_text(section, LV_SYMBOL_DOWNLOAD, "Telemetry", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_telemetry_page);

    create_text(root_page, NULL, "GS Settings", LV_MENU_ITEM_BUILDER_VARIANT_1,NULL);
    section = lv_menu_section_create(root_page);
    cont = create_text(section, LV_SYMBOL_DOWNLOAD, "Telemetry", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_wlan_page);
    cont = create_text(section, LV_SYMBOL_WIFI, "WLAN", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_wlan_page);

    lv_menu_set_sidebar_page(menu, NULL);
    lv_menu_clear_history(menu);
    lv_menu_set_page(menu, root_page);

    return menu;
}

static void back_event_handler(lv_event_t * e)
{
    printf("This should close the menu\n");
}

