#include <stdio.h>
#include <stdlib.h>

#include "lvgl/lvgl.h"
#include "helper.h"

void create_gs_system_menu(lv_obj_t * parent) {


    lv_obj_t * cont;
    lv_obj_t * label;
    lv_obj_t * section;


    label = lv_label_create(parent);
    lv_label_set_text(label, "General");
    section = lv_menu_section_create(parent);
    cont = lv_menu_cont_create(section);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);    
    create_switch(cont,LV_SYMBOL_SETTINGS,"GS Rendering",false);
    create_dropdown(cont,LV_SYMBOL_SETTINGS, "Resolution", "1280x720@60\n1280x720@90\n1920x1080@90\n");


    label = lv_label_create(parent);
    lv_label_set_text(label, "Recording");
    section = lv_menu_section_create(parent);
    cont = lv_menu_cont_create(section);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    create_switch(cont,LV_SYMBOL_SETTINGS,"Enabled",false);
    create_dropdown(parent,LV_SYMBOL_SETTINGS, "Recording FPS", "60\n90");    
}