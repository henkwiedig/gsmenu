#include "../../lvgl/lvgl.h"
#include "../input/input.h"
#include <stdio.h>
#include <stdlib.h>
#include "helper.h"

static void scan_wifi_event_handler(lv_event_t * e);
static void connect_wifi_event_handler(lv_event_t * e);
static void disconnect_wifi_event_handler(lv_event_t * e);
static void ta_event_cb(lv_event_t * e);

extern lv_obj_t * menu;
extern gsmenu_control_mode_t control_mode;

static lv_obj_t * ta_ssid;
static lv_obj_t * ta_password;

static int old_height;

static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e); 
    lv_obj_t * target_ta = lv_obj_get_user_data(btn); // Retrieve associated textarea

    if(code == LV_EVENT_CLICKED) {
        if (target_ta) {
            old_height = lv_obj_get_height(lv_obj_get_parent(kb));
            lv_keyboard_set_textarea(kb, target_ta);
            lv_obj_set_style_max_height(kb, LV_VER_RES * 2 / 3, 0);
            lv_obj_update_layout(lv_obj_get_parent(kb));  
            lv_obj_set_height(lv_obj_get_parent(kb), LV_VER_RES - lv_obj_get_height(kb));
            lv_obj_remove_flag(kb, LV_OBJ_FLAG_HIDDEN);
            lv_obj_scroll_to_view_recursive(target_ta, LV_ANIM_OFF);
            lv_indev_wait_release(lv_event_get_param(e));
            lv_group_focus_obj(kb);
        }
    }
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
        lv_obj_set_height(lv_obj_get_parent(kb), old_height);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, ta);   /*To forget the last clicked object to make it focusable again*/
        lv_group_focus_obj(lv_keyboard_get_textarea(kb));
        lv_obj_update_layout(lv_obj_get_parent(kb));
    }
}
void create_wifi_menu(lv_obj_t * parent) {
    
    lv_obj_t * btn_scan = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_scan, scan_wifi_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label_scan = lv_label_create(btn_scan);
    lv_label_set_text(label_scan, "Scan WiFi");
    
    lv_obj_t * btn_connect = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_connect, connect_wifi_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label_connect = lv_label_create(btn_connect);
    lv_label_set_text(label_connect, "Connect WiFi");
    
    lv_obj_t * btn_disconnect = lv_btn_create(parent);
    lv_obj_add_event_cb(btn_disconnect, disconnect_wifi_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label_disconnect = lv_label_create(btn_disconnect);
    lv_label_set_text(label_disconnect, "Disconnect WiFi");
    
    ta_ssid = create_textarea(parent, NULL, "SSID", false);
    lv_obj_add_state(ta_ssid, LV_STATE_DISABLED);
    lv_obj_t * ssid_button = lv_button_create(lv_obj_get_parent(ta_ssid));
    lv_obj_set_user_data(ssid_button, ta_ssid); // Associate button with text area
    lv_obj_t * obj = lv_label_create(ssid_button);
    lv_label_set_text(obj,LV_SYMBOL_KEYBOARD); 
   
    ta_password = create_textarea(parent, NULL, "Password", true);
    lv_obj_add_state(ta_password, LV_STATE_DISABLED);
    lv_obj_t * pw_button = lv_button_create(lv_obj_get_parent(ta_password));
    lv_obj_set_user_data(pw_button, ta_password); // Associate button with text area
    obj = lv_label_create(pw_button);
    lv_label_set_text(obj,LV_SYMBOL_KEYBOARD);     

    lv_obj_t * kb = lv_keyboard_create(parent);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(ssid_button, btn_event_cb, LV_EVENT_ALL, kb);
    lv_obj_add_event_cb(pw_button, btn_event_cb, LV_EVENT_ALL, kb);
    lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_ALL,kb);
    lv_keyboard_set_textarea(kb, NULL);    
}

static void scan_wifi_event_handler(lv_event_t * e) {
    system("echo nmcli dev wifi rescan");
    system("echo nmcli -t -f SSID dev wifi list > wifi_list.txt");
    LV_LOG_USER("WiFi scan initiated");
}

static void connect_wifi_event_handler(lv_event_t * e) {
    const char * ssid = lv_textarea_get_text(ta_ssid);
    const char * password = lv_textarea_get_text(ta_password);
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "echo nmcli dev wifi connect '%s' password '%s'", ssid, password);
    system(cmd);
    LV_LOG_USER("Connecting to WiFi: %s", ssid);
}

static void disconnect_wifi_event_handler(lv_event_t * e) {
    system("echo nmcli con down id $(nmcli -t -f NAME c show --active)");
    LV_LOG_USER("WiFi disconnected");
}
