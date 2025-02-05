#include <stdio.h>
#include "../../lvgl/lvgl.h"
#include "../input/input.h"

extern gsmenu_control_mode_t control_mode;

typedef enum {
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2
} lv_menu_builder_variant_t;

static void back_event_handler(lv_event_t * e);
lv_obj_t * root_page;
static lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt,
                              lv_menu_builder_variant_t builder_variant,lv_group_t * group);
static lv_obj_t * create_slider(lv_obj_t * parent,
                                const char * icon, const char * txt, int32_t min, int32_t max, int32_t val);
static lv_obj_t * create_switch(lv_obj_t * parent,
                                const char * icon, const char * txt, bool chk);
static lv_obj_t * create_dropdown(lv_obj_t * parent, const char * icon, const char * label_txt, const char * txt);

lv_obj_t * lv_example_menu_5(lv_group_t * group)
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

    lv_obj_t * sub_sound_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_sound_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_sound_page);
    section = lv_menu_section_create(sub_sound_page);
    create_switch(section, LV_SYMBOL_AUDIO, "Sound", false);

    lv_obj_t * sub_display_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_display_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_display_page);
    section = lv_menu_section_create(sub_display_page);
    create_slider(section, LV_SYMBOL_SETTINGS, "Brightness", 0, 150, 100);

    lv_obj_t * sub_menu_mode_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_menu_mode_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_menu_mode_page);
    section = lv_menu_section_create(sub_menu_mode_page);
    cont = create_switch(section, LV_SYMBOL_AUDIO, "Sidebar enable", true);

    /*Create a root page*/
    root_page = lv_menu_page_create(menu, "Drone Settings");
    lv_obj_set_style_pad_hor(root_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    section = lv_menu_section_create(root_page);
    cont = create_text(section, LV_SYMBOL_WIFI, "WFB-NG", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_wfbng_page);
    cont = create_text(section, LV_SYMBOL_IMAGE, "Camera", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_sound_page);
    cont = create_text(section, LV_SYMBOL_DOWNLOAD, "Telemetry", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_display_page);

    create_text(root_page, NULL, "GS Settings", LV_MENU_ITEM_BUILDER_VARIANT_1,NULL);
    section = lv_menu_section_create(root_page);
    cont = create_text(section, LV_SYMBOL_DOWNLOAD, "Telemetry", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_menu_mode_page);
    cont = create_text(section, LV_SYMBOL_WIFI, "WLAN", LV_MENU_ITEM_BUILDER_VARIANT_1,group);
    lv_menu_set_load_page_event(menu, cont, sub_menu_mode_page);

    lv_menu_set_sidebar_page(menu, NULL);
    lv_menu_clear_history(menu);
    lv_menu_set_page(menu, root_page);

    return menu;
}

static void back_event_handler(lv_event_t * e)
{
    printf("This should close the menu\n");
}

static lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt,
                              lv_menu_builder_variant_t builder_variant, lv_group_t * group)
{
    lv_obj_t * obj = lv_menu_cont_create(parent);
    if (group)
        lv_group_add_obj(group, obj);
          

    lv_obj_t * img = NULL;
    lv_obj_t * label = NULL;

    if(icon) {
        img = lv_image_create(obj);
        lv_image_set_src(img, icon);
    }

    if(txt) {
        label = lv_label_create(obj);
        lv_label_set_text(label, txt);
        lv_label_set_long_mode(label, LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);
        lv_obj_set_flex_grow(label, 1);
    }

    if(builder_variant == LV_MENU_ITEM_BUILDER_VARIANT_2 && icon && txt) {
        lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
        lv_obj_swap(img, label);
    }

    return obj;
}

static lv_obj_t * create_slider(lv_obj_t * parent, const char * icon, const char * txt, int32_t min, int32_t max,
                                int32_t val)
{
    lv_obj_t * obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_2,NULL);

    lv_obj_t * slider = lv_slider_create(obj);
    lv_obj_set_flex_grow(slider, 1);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, val, LV_ANIM_OFF);

    if(icon == NULL) {
        lv_obj_add_flag(slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    }

    return obj;
}

static lv_obj_t * create_switch(lv_obj_t * parent, const char * icon, const char * txt, bool chk)
{
    lv_obj_t * obj = create_text(parent, NULL, "Hello World", LV_MENU_ITEM_BUILDER_VARIANT_1,NULL);

    lv_obj_t * sw = lv_switch_create(obj);
    lv_obj_add_state(sw, chk ? LV_STATE_CHECKED : 0);

    return obj;
}

static void dropdown_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    lv_key_t *key = lv_event_get_key(e);
    switch (code)
    {
    case LV_EVENT_CANCEL:
    case LV_EVENT_VALUE_CHANGED:
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        printf("Option: %s\n", buf);
        control_mode = GSMENU_CONTROL_MODE_NAV;
        break;
    case LV_EVENT_READY:
        printf("Switching control mode\n");
        control_mode = GSMENU_CONTROL_MODE_EDIT;    
    default:
        break;
    }
}

static lv_obj_t * create_dropdown(lv_obj_t * parent, const char * icon, const char * label_txt, const char * txt)
{

    lv_obj_t * obj = lv_menu_cont_create(parent);

    lv_obj_t * label = lv_label_create(obj);
    lv_label_set_text(label, label_txt);

    lv_obj_t * dd = lv_dropdown_create(obj); 
    lv_dropdown_set_options(dd, txt);
    lv_dropdown_set_dir(dd, LV_DIR_RIGHT);
    lv_dropdown_set_symbol(dd, LV_SYMBOL_RIGHT);

    lv_obj_add_event_cb(dd, dropdown_event_handler, LV_EVENT_ALL, NULL);
    return obj;
}