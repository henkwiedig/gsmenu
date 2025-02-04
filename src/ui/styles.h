#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: Button
lv_style_t *get_style_button_MAIN_DEFAULT();
lv_style_t *get_style_button_MAIN_FOCUS_KEY();
lv_style_t *get_style_button_MAIN_FOCUSED();
void add_style_button(lv_obj_t *obj);
void remove_style_button(lv_obj_t *obj);



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/