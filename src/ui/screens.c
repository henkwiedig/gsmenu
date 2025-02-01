#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

groups_t groups;
static bool groups_created = false;

objects_t objects;
lv_obj_t *tick_value_change_obj;

static void event_handler_cb_main_main(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_SCREEN_LOAD_START) {
        // group: mainGroup
        lv_group_remove_all_objs(groups.mainGroup);
        lv_group_add_obj(groups.mainGroup, objects.obj0);
        lv_group_add_obj(groups.mainGroup, objects.obj1);
        lv_group_add_obj(groups.mainGroup, objects.obj2);
        lv_group_add_obj(groups.mainGroup, objects.obj3);
        lv_group_add_obj(groups.mainGroup, objects.obj4);
    }
}

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_add_event_cb(obj, event_handler_cb_main_main, LV_EVENT_ALL, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff252323), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_tabview_create(parent_obj);
            lv_tabview_set_tab_bar_position(obj, LV_DIR_LEFT);
            lv_tabview_set_tab_bar_size(obj, 80);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 32, 39);
            lv_obj_set_size(obj, 742, 406);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff595353), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Settings");
                    objects.obj1 = obj;
                }
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "WFB-NG");
                    objects.obj2 = obj;
                }
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Tab");
                    objects.obj3 = obj;
                }
                {
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Tab");
                    objects.obj4 = obj;
                }
            }
        }
    }
}

void tick_screen_main() {
}


void ui_create_groups() {
    if (!groups_created) {
        groups.mainGroup = lv_group_create();
        groups_created = true;
    }
}

void create_screens() {
    ui_create_groups();
    
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
}

typedef void (*tick_screen_func_t)();

tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};

void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
