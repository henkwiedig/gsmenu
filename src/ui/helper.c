#include "../../lvgl/lvgl.h"
#include "../input/input.h"
#include "helper.h"

extern gsmenu_control_mode_t control_mode;

lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt,
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

lv_obj_t * create_slider(lv_obj_t * parent, const char * icon, const char * txt, int32_t min, int32_t max,
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



static void lv_spinbox_increment_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(lv_event_get_user_data(e));
    }
}

static void lv_spinbox_decrement_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(lv_event_get_user_data(e));
    }
}

lv_obj_t * create_spinbox(lv_obj_t * parent, const char * icon, const char * txt, int32_t min, int32_t max,
                                int32_t val)
{
    lv_obj_t * obj = lv_menu_cont_create(parent);
    lv_obj_t * img = NULL;

    if(icon) {
        img = lv_image_create(obj);
        lv_image_set_src(img, icon);
    }    

    lv_obj_t * label = lv_label_create(obj);
    lv_label_set_text(label, txt);
    lv_label_set_text(label, txt);

    lv_obj_t * spinbox = lv_spinbox_create(obj);
    lv_obj_add_state(spinbox, LV_STATE_DISABLED);
    lv_spinbox_set_digit_format(spinbox, 2, 0);
    lv_group_remove_obj(spinbox);
    lv_spinbox_set_value(spinbox, val);
    lv_spinbox_set_range(spinbox, min, max);
    lv_obj_t * btn = lv_button_create(obj);
    lv_obj_set_style_bg_image_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_increment_event_cb, LV_EVENT_ALL,  spinbox);

    btn = lv_button_create(obj);
    lv_obj_set_style_bg_image_src(btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_decrement_event_cb, LV_EVENT_ALL, spinbox);


    return obj;
}

lv_obj_t * create_switch(lv_obj_t * parent, const char * icon, const char * txt, bool chk)
{
    lv_obj_t * obj = lv_menu_cont_create(parent);
    lv_obj_t * img = NULL;

    if(icon) {
        img = lv_image_create(obj);
        lv_image_set_src(img, icon);
    }

    lv_obj_t * label = lv_label_create(obj);
    lv_label_set_text(label, txt);

    lv_obj_t * sw = lv_switch_create(obj);
    lv_obj_add_state(sw, chk ? LV_STATE_CHECKED : 0);

    return obj;
}

void dropdown_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    lv_key_t key = lv_event_get_key(e);
    switch (code)
    {
    case LV_EVENT_CANCEL:
    case LV_EVENT_VALUE_CHANGED: {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        printf("Option: %s\n", buf);
        control_mode = GSMENU_CONTROL_MODE_NAV;
        break;
    }
    case LV_EVENT_READY:
        printf("Switching control mode\n");
        control_mode = GSMENU_CONTROL_MODE_EDIT;    
    default:
        break;
    }
}

lv_obj_t * create_dropdown(lv_obj_t * parent, const char * icon, const char * label_txt, const char * txt)
{

    lv_obj_t * obj = lv_menu_cont_create(parent);

    lv_obj_t * img = NULL;

    if(icon) {
        img = lv_image_create(obj);
        lv_image_set_src(img, icon);
    }    

    lv_obj_t * label = lv_label_create(obj);
    lv_label_set_text(label, label_txt);

    lv_obj_t * dd = lv_dropdown_create(obj); 
    lv_dropdown_set_options(dd, txt);
    lv_dropdown_set_dir(dd, LV_DIR_RIGHT);
    lv_dropdown_set_symbol(dd, LV_SYMBOL_RIGHT);

    lv_obj_add_event_cb(dd, dropdown_event_handler, LV_EVENT_ALL, NULL);
    return obj;
}

lv_obj_t * create_textarea(lv_obj_t * parent, const char * icon, const char * label_txt, bool password)
{
    //lv_obj_t * obj = create_text(parent, icon, label_txt, LV_MENU_ITEM_BUILDER_VARIANT_2,NULL);

    lv_obj_t * obj = lv_menu_cont_create(parent);

    lv_obj_t * label = lv_label_create(obj);
    lv_label_set_text(label, label_txt);    

    lv_obj_t * ta = lv_textarea_create(obj);
    //lv_textarea_set_placeholder_text(ta_ssid, "SSID");
    lv_textarea_set_one_line(ta,true);
    //lv_textarea_set_password_mode(ta, password);

    // if(icon == NULL) {
    //     lv_obj_add_flag(ta, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    // }

    return ta;
}