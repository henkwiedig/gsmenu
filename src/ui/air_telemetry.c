#include <stdio.h>
#include <stdlib.h>

#include "lvgl/lvgl.h"
#include "helper.h"

void create_air_telemetry_menu(lv_obj_t * parent) {

    lv_obj_t * cont;
    lv_obj_t * label;
    lv_obj_t * section;

    section = lv_menu_section_create(parent);
    cont = lv_menu_cont_create(section);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);    
    create_dropdown(cont,LV_SYMBOL_SETTINGS,"Serial Port","/dev/ttyS0\n/dev/ttyS3\n/dev/ttyS2\n");
    create_dropdown(cont,LV_SYMBOL_SETTINGS,"Bautrate","4800\n9600\n19200\n38400\n57600\n115200\n230400\n460800\n921600");
    create_dropdown(cont,LV_SYMBOL_SETTINGS,"Router","mavfwd\nmav-router\nmsposd");
    create_spinbox(cont,LV_SYMBOL_SETTINGS, "MCS Index", 0 , 9, 1);
    create_spinbox(cont,LV_SYMBOL_SETTINGS, "Aggregate", 0 , 15, 1);


    label = lv_label_create(parent);
    lv_label_set_text(label, "MSPOSD Settings");
    section = lv_menu_section_create(parent);
    cont = lv_menu_cont_create(section);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);    
    create_spinbox(cont,LV_SYMBOL_SETTINGS, "RC Channel", 0 , 15, 1);
    create_switch(cont,LV_SYMBOL_SETTINGS,"GS Rendering",false);



}