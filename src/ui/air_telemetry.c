#include <stdio.h>
#include <stdlib.h>

#include "lvgl/lvgl.h"
#include "helper.h"

void create_air_telemetry_menu(lv_obj_t * parent) {

    create_dropdown(parent,NULL,"Serial Port","/dev/ttyS0\n/dev/ttyS3\n/dev/ttyS2\n");
    create_dropdown(parent,NULL,"Bautrate","4800\n9600\n19200\n38400\n57600\n115200\n230400\n460800\n921600");
    create_dropdown(parent,NULL,"Router","mavfwd\nmav-router\nmsposd");
    create_spinbox(parent,NULL, "MCS Index", 0 , 9, 1);
    create_spinbox(parent,NULL, "Aggregate", 0 , 15, 1);
    create_text(parent, LV_SYMBOL_SETTINGS, "MSPOSD Settings", LV_MENU_ITEM_BUILDER_VARIANT_1,NULL);
    create_spinbox(parent,NULL, "RC Channel", 0 , 15, 1);
    create_switch(parent,NULL,"GS Rendering",false);



}