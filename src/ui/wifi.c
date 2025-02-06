#include "lvgl/lvgl.h"
#include <stdio.h>
#include <stdlib.h>

static void scan_wifi_event_handler(lv_event_t * e);
static void connect_wifi_event_handler(lv_event_t * e);
static void disconnect_wifi_event_handler(lv_event_t * e);

static lv_obj_t * ta_ssid;
static lv_obj_t * ta_password;

void create_wifi_menu(lv_obj_t * parent) {
    lv_obj_t * menu = lv_menu_create(parent);
    lv_obj_set_size(menu, LV_HOR_RES, LV_VER_RES);
    
    lv_obj_t * cont_main = lv_menu_page_create(menu, NULL);
    lv_menu_set_page(menu, cont_main);
    
    lv_obj_t * btn_scan = lv_btn_create(cont_main);
    lv_obj_set_size(btn_scan, 100, 40);
    lv_obj_add_event_cb(btn_scan, scan_wifi_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label_scan = lv_label_create(btn_scan);
    lv_label_set_text(label_scan, "Scan WiFi");
    
    lv_obj_t * btn_connect = lv_btn_create(cont_main);
    lv_obj_set_size(btn_connect, 100, 40);
    lv_obj_add_event_cb(btn_connect, connect_wifi_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label_connect = lv_label_create(btn_connect);
    lv_label_set_text(label_connect, "Connect WiFi");
    
    lv_obj_t * btn_disconnect = lv_btn_create(cont_main);
    lv_obj_set_size(btn_disconnect, 100, 40);
    lv_obj_add_event_cb(btn_disconnect, disconnect_wifi_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label_disconnect = lv_label_create(btn_disconnect);
    lv_label_set_text(label_disconnect, "Disconnect WiFi");
    
    ta_ssid = lv_textarea_create(cont_main);
    lv_textarea_set_placeholder_text(ta_ssid, "SSID");
    
    ta_password = lv_textarea_create(cont_main);
    lv_textarea_set_placeholder_text(ta_password, "Password");
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