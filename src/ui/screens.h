#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _groups_t {
    lv_group_t *mainmenu;
    lv_group_t *wfb_ng;
} groups_t;

extern groups_t groups;

void ui_create_groups();

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *mainmenu;
    lv_obj_t *b_wfb_ng;
    lv_obj_t *wfb_ng;
    lv_obj_t *button_x;
    lv_obj_t *obj0;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
};

void create_screen_main();
void tick_screen_main();

enum Themes {
    THEME_ID_DJI,
};
enum Colors {
    COLOR_ID_BACKGROUND,
    COLOR_ID_SELECTED,
};
void change_color_theme(uint32_t themeIndex);
extern uint32_t theme_colors[1][2];

void create_screens();
void tick_screen(int screen_index);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/