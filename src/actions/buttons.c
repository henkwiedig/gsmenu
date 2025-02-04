#include <stdio.h>
#include "../../lvgl/lvgl.h"
#include "../../lvgl/src/core/lv_global.h"

void action_set_focus(lv_event_t *e) {
    int *target = lv_event_get_user_data(e);

    if (target) {
        printf("action_set_focus called. Userdata: %i\n", target);
    } else {
        printf("action_set_focus called. Userdata is NULL\n");
    }
}
