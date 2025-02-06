#pragma once
#include <stdio.h>

#include "../../lvgl/lvgl.h"

typedef enum {
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2
} lv_menu_builder_variant_t;


lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt,
                              lv_menu_builder_variant_t builder_variant, lv_group_t * group);

lv_obj_t * create_slider(lv_obj_t * parent, const char * icon, const char * txt, int32_t min, int32_t max,
                                int32_t val);
lv_obj_t * create_switch(lv_obj_t * parent, const char * icon, const char * txt, bool chk);

void dropdown_event_handler(lv_event_t * e);

lv_obj_t * create_dropdown(lv_obj_t * parent, const char * icon, const char * label_txt, const char * txt);