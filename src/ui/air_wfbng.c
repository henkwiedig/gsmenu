#include <stdio.h>
#include <stdlib.h>

#include "lvgl/lvgl.h"
#include "helper.h"

void create_air_wfbng_menu(lv_obj_t * parent) {

    create_slider(parent, LV_SYMBOL_SETTINGS, "Velocity", 0, 150, 120);
    create_slider(parent, LV_SYMBOL_SETTINGS, "Acceleration", 0, 150, 50);
    create_slider(parent, LV_SYMBOL_SETTINGS, "Weight limit", 0, 150, 80);
    create_dropdown(parent,LV_SYMBOL_SETTINGS, "Frequency", "Foo\nBar\nBuzz");

}