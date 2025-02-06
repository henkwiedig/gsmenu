#include <stdio.h>
#include <stdlib.h>

#include "lvgl/lvgl.h"
#include "helper.h"

void create_air_telemetry_menu(lv_obj_t * parent) {

    create_slider(parent, LV_SYMBOL_SETTINGS, "Brightness", 0, 150, 100);

}