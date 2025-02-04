#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif


enum {
    ACTION_SET_FOCUS_PROPERTY_TARGET,
};
extern void action_set_focus(lv_event_t * e);



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/