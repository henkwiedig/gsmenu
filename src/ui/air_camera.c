#include <stdio.h>
#include <stdlib.h>

#include "lvgl/lvgl.h"
#include "helper.h"

void create_air_camera_menu(lv_obj_t * parent) {

    create_switch(parent, LV_SYMBOL_AUDIO, "FPV", false);

}