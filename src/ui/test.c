#include "../../lvgl/lvgl.h"
extern lv_obj_t * menu;


lv_obj_t * create_test_menu(lv_group_t * group)
{
    menu = lv_menu_create(lv_screen_active());
    lv_obj_set_size(menu, lv_display_get_horizontal_resolution(NULL), lv_display_get_vertical_resolution(NULL));
    lv_obj_center(menu);
    lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 10), 0);

    lv_obj_t * cont;
    lv_obj_t * label;
    lv_obj_t * section;

    /*Create a sub page*/
    lv_obj_t * sub_page = lv_menu_page_create(menu, NULL);

    cont = lv_menu_cont_create(sub_page);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Hello, I am hiding here");

    /*Create a main page*/
    lv_obj_t * main_page = lv_menu_page_create(menu, "Page 1");
    
    section = lv_menu_section_create(main_page);

    cont = lv_menu_cont_create(section);
    lv_group_add_obj(group,cont);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Item 1");

    cont = lv_menu_cont_create(section);
    lv_group_add_obj(group,cont);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Item 2");

    cont = lv_menu_cont_create(section);
    lv_group_add_obj(group,cont);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Item 3 (Click me!)");
    lv_menu_set_load_page_event(menu, cont, sub_page);

    label = lv_label_create(main_page);
    lv_label_set_text(label, "New Section");
    section = lv_menu_section_create(main_page);

    cont = lv_menu_cont_create(section);
    lv_group_add_obj(group,cont);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Item 1");

    cont = lv_menu_cont_create(section);
    lv_group_add_obj(group,cont);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Item 3 (Click me!)");
    lv_menu_set_load_page_event(menu, cont, sub_page);    

    lv_menu_set_page(menu, main_page);

    return menu;
}
