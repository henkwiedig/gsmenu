#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "screens.h"

//
// Style: Button
//

void init_style_button_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(theme_colors[0][0]));
    lv_style_set_radius(style, 5);
    lv_style_set_outline_width(style, 5);
};

lv_style_t *get_style_button_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_button_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_button_MAIN_FOCUS_KEY(lv_style_t *style) {
    lv_style_set_outline_color(style, lv_color_hex(theme_colors[0][1]));
    lv_style_set_outline_width(style, 5);
    lv_style_set_text_color(style, lv_color_hex(theme_colors[0][1]));
};

lv_style_t *get_style_button_MAIN_FOCUS_KEY() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_button_MAIN_FOCUS_KEY(style);
    }
    return style;
};

void init_style_button_MAIN_FOCUSED(lv_style_t *style) {
    lv_style_set_outline_color(style, lv_color_hex(theme_colors[0][1]));
    lv_style_set_outline_width(style, 5);
    lv_style_set_text_color(style, lv_color_hex(theme_colors[0][1]));
};

lv_style_t *get_style_button_MAIN_FOCUSED() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_button_MAIN_FOCUSED(style);
    }
    return style;
};

void add_style_button(lv_obj_t *obj) {
    lv_obj_add_style(obj, get_style_button_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_button_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(obj, get_style_button_MAIN_FOCUSED(), LV_PART_MAIN | LV_STATE_FOCUSED);
};

void remove_style_button(lv_obj_t *obj) {
    lv_obj_remove_style(obj, get_style_button_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_button_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_remove_style(obj, get_style_button_MAIN_FOCUSED(), LV_PART_MAIN | LV_STATE_FOCUSED);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_button,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_button,
    };
    remove_style_funcs[styleIndex](obj);
}

