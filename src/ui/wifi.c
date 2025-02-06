#include "../../lvgl/lvgl.h"
#include "../input/input.h"
#include <stdio.h>
#include <stdlib.h>
#include "helper.h"

static void scan_wifi_event_handler(lv_event_t * e);
static void connect_wifi_event_handler(lv_event_t * e);
static void disconnect_wifi_event_handler(lv_event_t * e);
static void ta_event_cb(lv_event_t * e);

extern gsmenu_control_mode_t control_mode;

static lv_obj_t * ta_ssid;
static lv_obj_t * ta_password;

void create_wifi_menu(lv_obj_t * parent) {
    
    // lv_obj_t * cont_main = lv_menu_page_create(parent, NULL);
    // lv_menu_set_page(menu, cont_main);
    
    // lv_obj_t * btn_scan = lv_btn_create(parent);
    // //lv_obj_set_size(btn_scan, 100, 40);
    // lv_obj_add_event_cb(btn_scan, scan_wifi_event_handler, LV_EVENT_CLICKED, NULL);
    // lv_obj_t * label_scan = lv_label_create(btn_scan);
    // lv_label_set_text(label_scan, "Scan WiFi");
    
    // lv_obj_t * btn_connect = lv_btn_create(parent);
    // //lv_obj_set_size(btn_connect, 100, 40);
    // lv_obj_add_event_cb(btn_connect, connect_wifi_event_handler, LV_EVENT_CLICKED, NULL);
    // lv_obj_t * label_connect = lv_label_create(btn_connect);
    // lv_label_set_text(label_connect, "Connect WiFi");
    
    // lv_obj_t * btn_disconnect = lv_btn_create(parent);
    // //lv_obj_set_size(btn_disconnect, 100, 40);
    // lv_obj_add_event_cb(btn_disconnect, disconnect_wifi_event_handler, LV_EVENT_CLICKED, NULL);
    // lv_obj_t * label_disconnect = lv_label_create(btn_disconnect);
    // lv_label_set_text(label_disconnect, "Disconnect WiFi");
    
    ta_ssid = create_textarea(parent, NULL, "SSID", false);
    ta_password = create_textarea(parent, NULL, "Password", true);

    lv_obj_t * kb = lv_keyboard_create(parent);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(ta_ssid, ta_event_cb, LV_EVENT_ALL, kb);
    lv_keyboard_set_textarea(kb, ta_ssid);
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


static void ta_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * kb = lv_event_get_user_data(e);
    if(code == LV_EVENT_KEY) {
        printf("Event fired\n");        
        lv_key_t key = lv_event_get_key(e);        
    }    

    // printf("Event fired, code: %i, key: %i\n",code,key);

    // if(code == LV_EVENT_FOCUSED) {
    //     printf("Switching keybaord input mode\n");
    //     lv_keyboard_set_textarea(kb, ta);
    //     control_mode = GSMENU_CONTROL_MODE_EDIT;
    // }

    // if(code == LV_EVENT_DEFOCUSED) {
    //     printf("Switching keybaord input out\n");
    //     lv_keyboard_set_textarea(kb, NULL);
    //     lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    //     control_mode = GSMENU_CONTROL_MODE_NAV;
    // }
    // switch (key)
    // {
    //     case LV_KEY_ENTER: {
    //         printf("Switching control mode\n");
    //         lv_obj_remove_flag(kb, LV_OBJ_FLAG_HIDDEN);
    //         control_mode = GSMENU_CONTROL_MODE_EDIT;    
    //         break;
    //     }
    //     default:
    //         break;
    // }    
}