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
    void *flowState = lv_event_get_user_data(e);
    
    if (event == LV_EVENT_SCREEN_LOAD_START) {
        // group: mainmenu
        lv_group_remove_all_objs(groups.mainmenu);
        lv_group_add_obj(groups.mainmenu, objects.mainmenu);
        lv_group_add_obj(groups.mainmenu, objects.b_wfb_ng);
        lv_group_add_obj(groups.mainmenu, objects.wfb_ng);
        lv_group_add_obj(groups.mainmenu, objects.button_x);
        // group: wfb_ng
        lv_group_remove_all_objs(groups.wfb_ng);
    }
}

static void event_handler_cb_main_b_wfb_ng(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    
    if (event == LV_EVENT_KEY) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 2, 0, e);
    }
}

static void event_handler_cb_main_button_x(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    
    if (event == LV_EVENT_KEY) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 7, 0, e);
    }
}

static void event_handler_cb_main_obj0(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    
    if (event == LV_EVENT_KEY) {
        e->user_data = (void *)0;
        flowPropagateValueLVGLEvent(flowState, 9, 0, e);
    }
}

void create_screen_main() {
    void *flowState = getFlowState(0, 0);
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 800, 480);
    lv_obj_add_event_cb(obj, event_handler_cb_main_main, LV_EVENT_ALL, flowState);
    lv_obj_set_style_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(obj, 0, LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_opa(obj, 0, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_opa(obj, 0, LV_PART_MAIN | LV_STATE_CHECKED | LV_STATE_PRESSED);
    lv_obj_set_style_opa(obj, 0, LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_opa(obj, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_opa(obj, 0, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_set_style_opa(obj, 0, LV_PART_MAIN | LV_STATE_EDITED);
    lv_obj_set_style_opa(obj, 0, LV_PART_MAIN | LV_STATE_SCROLLED);
    lv_obj_set_style_opa(obj, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(obj, 0, LV_PART_SCROLLBAR | LV_STATE_CHECKED);
    lv_obj_set_style_opa(obj, 0, LV_PART_SCROLLBAR | LV_STATE_PRESSED);
    lv_obj_set_style_opa(obj, 0, LV_PART_SCROLLBAR | LV_STATE_CHECKED | LV_STATE_PRESSED);
    lv_obj_set_style_opa(obj, 0, LV_PART_SCROLLBAR | LV_STATE_DISABLED);
    lv_obj_set_style_opa(obj, 0, LV_PART_SCROLLBAR | LV_STATE_FOCUSED);
    lv_obj_set_style_opa(obj, 0, LV_PART_SCROLLBAR | LV_STATE_FOCUS_KEY);
    lv_obj_set_style_opa(obj, 0, LV_PART_SCROLLBAR | LV_STATE_EDITED);
    lv_obj_set_style_opa(obj, 0, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    {
        lv_obj_t *parent_obj = obj;
        {
            // mainmenu
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.mainmenu = obj;
            lv_obj_set_pos(obj, 20, 29);
            lv_obj_set_size(obj, 100, 230);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff242729), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff242729), LV_PART_ANY | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // bWFB-NG
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.b_wfb_ng = obj;
                    lv_obj_set_pos(obj, 0, 5);
                    lv_obj_set_size(obj, 100, 50);
                    lv_obj_add_event_cb(obj, event_handler_cb_main_b_wfb_ng, LV_EVENT_ALL, flowState);
                    add_style_button(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "WFB-NG");
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 92);
                    lv_obj_set_size(obj, 100, 50);
                    add_style_button(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Button");
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
            }
        }
        {
            // wfb_ng
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.wfb_ng = obj;
            lv_obj_set_pos(obj, 128, 29);
            lv_obj_set_size(obj, 211, 452);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // buttonX
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.button_x = obj;
                    lv_obj_set_pos(obj, 10, 58);
                    lv_obj_set_size(obj, 100, 50);
                    lv_obj_add_event_cb(obj, event_handler_cb_main_button_x, LV_EVENT_ALL, flowState);
                    add_style_button(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Button");
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
                {
                    lv_obj_t *obj = lv_btn_create(parent_obj);
                    objects.obj0 = obj;
                    lv_obj_set_pos(obj, 10, 141);
                    lv_obj_set_size(obj, 100, 50);
                    lv_obj_add_event_cb(obj, event_handler_cb_main_obj0, LV_EVENT_ALL, flowState);
                    add_style_button(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_text(obj, "Button");
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
            }
        }
    }
}

void tick_screen_main() {
    void *flowState = getFlowState(0, 0);
    {
        bool new_val = evalBooleanProperty(flowState, 6, 3, "Failed to evaluate Hidden flag");
        bool cur_val = lv_obj_has_flag(objects.wfb_ng, LV_OBJ_FLAG_HIDDEN);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.wfb_ng;
            if (new_val) lv_obj_add_flag(objects.wfb_ng, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_clear_flag(objects.wfb_ng, LV_OBJ_FLAG_HIDDEN);
            tick_value_change_obj = NULL;
        }
    }
}

void change_color_theme(uint32_t theme_index) {
    lv_style_set_bg_color(get_style_button_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][0]));
    
    lv_style_set_outline_color(get_style_button_MAIN_FOCUS_KEY(), lv_color_hex(theme_colors[theme_index][1]));
    
    lv_style_set_text_color(get_style_button_MAIN_FOCUS_KEY(), lv_color_hex(theme_colors[theme_index][1]));
    
    lv_style_set_outline_color(get_style_button_MAIN_FOCUSED(), lv_color_hex(theme_colors[theme_index][1]));
    
    lv_style_set_text_color(get_style_button_MAIN_FOCUSED(), lv_color_hex(theme_colors[theme_index][1]));
    
    lv_obj_invalidate(objects.main);
}

extern void add_style(lv_obj_t *obj, int32_t styleIndex);
extern void remove_style(lv_obj_t *obj, int32_t styleIndex);

void ui_create_groups() {
    if (!groups_created) {
        groups.mainmenu = lv_group_create();
        groups.wfb_ng = lv_group_create();
        eez_flow_init_groups((lv_group_t **)&groups, sizeof(groups) / sizeof(lv_group_t *));
        groups_created = true;
    }
}

static const char *screen_names[] = { "Main" };
static const char *object_names[] = { "main", "mainmenu", "b_wfb_ng", "wfb_ng", "button_x", "obj0" };
static const char *group_names[] = { "mainmenu", "wfb_ng" };
static const char *style_names[] = { "Button" };
static const char *theme_names[] = { "DJI" };

uint32_t theme_colors[1][2] = {
    { 0xff181b1d, 0xfff8e223 },
};

void create_screens() {
    ui_create_groups();
    
    eez_flow_init_styles(add_style, remove_style);
    
    eez_flow_init_screen_names(screen_names, sizeof(screen_names) / sizeof(const char *));
    eez_flow_init_object_names(object_names, sizeof(object_names) / sizeof(const char *));
    eez_flow_init_group_names(group_names, sizeof(group_names) / sizeof(const char *));
    eez_flow_init_style_names(style_names, sizeof(style_names) / sizeof(const char *));
    eez_flow_init_themes(theme_names, sizeof(theme_names) / sizeof(const char *), change_color_theme);
    
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
