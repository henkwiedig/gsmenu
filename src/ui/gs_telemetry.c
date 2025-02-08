#include <stdio.h>
#include <stdlib.h>

#include "lvgl/lvgl.h"
#include "helper.h"

void create_gs_telemetry_menu(lv_obj_t * parent) {

    create_switch(parent,LV_SYMBOL_SETTINGS,"GS Rendering",false);

}