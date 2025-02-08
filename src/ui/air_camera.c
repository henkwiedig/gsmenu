#include <stdio.h>
#include <stdlib.h>

#include "lvgl/lvgl.h"
#include "helper.h"

extern lv_obj_t * menu;

void create_air_camera_menu(lv_obj_t * parent) {

    lv_obj_t * cont;
    lv_obj_t * label;
    lv_obj_t * section;

    lv_obj_t * sub_image_page = lv_menu_page_create(menu, "Image");
    lv_obj_set_style_pad_hor(sub_image_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(parent), 0), 0);
    lv_menu_separator_create(sub_image_page);
    section = lv_menu_section_create(sub_image_page);
    cont = lv_menu_cont_create(section);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    create_switch(cont,LV_SYMBOL_SETTINGS,"Mirror",false);
    create_switch(cont,LV_SYMBOL_SETTINGS,"Flip",false);
    create_spinbox(cont,LV_SYMBOL_SETTINGS,"Contrast",0,100,11);
    create_spinbox(cont,LV_SYMBOL_SETTINGS,"Hue",0,100,11);
    create_spinbox(cont,LV_SYMBOL_SETTINGS,"Saturation",0,100,11);
    create_spinbox(cont,LV_SYMBOL_SETTINGS,"Luminance",0,100,11);
    create_spinbox(cont,LV_SYMBOL_SETTINGS,"Rotate",0,360,0);

    lv_obj_t * sub_video_page = lv_menu_page_create(menu, "Video");
    lv_obj_set_style_pad_hor(sub_video_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_video_page);
    section = lv_menu_section_create(sub_video_page);
    cont = lv_menu_cont_create(section);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);    
    create_dropdown(cont,LV_SYMBOL_SETTINGS, "Size", "1280x720\n1456x816\n1920x1080\n2720x1528\n2944x1656\n3200x1800\n3840x2160");
    create_dropdown(cont,LV_SYMBOL_SETTINGS, "FPS", "20\n30\n40\n50\n60\n70\n80\n90\n100\n120");
    create_dropdown(cont,LV_SYMBOL_SETTINGS, "Bitrate", "1024\n2048\n3072\n4096\n5120\n6144\n7168\n8192\n9216\n10240\n11264\n12288\n13312\n14336\n15360\n16384\n17408\n18432\n19456\n20480\n21504\n22528\n23552\n24576\n25600\n26624\n27648\n28672\n29692\n30720");
    create_dropdown(cont,LV_SYMBOL_SETTINGS, "Codec", "h264\nh265");
    create_spinbox(cont,LV_SYMBOL_SETTINGS,"Gopsize",0,20,1);
    create_dropdown(cont,LV_SYMBOL_SETTINGS, "RC Mode", "VBR\nCBR");

    lv_obj_t * sub_recording_page = lv_menu_page_create(menu, "Recording");
    lv_obj_set_style_pad_hor(sub_recording_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_recording_page);
    section = lv_menu_section_create(sub_recording_page);
    cont = lv_menu_cont_create(section);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);    
    create_switch(cont,LV_SYMBOL_SETTINGS,"Enabled",false);
    create_spinbox(cont,LV_SYMBOL_SETTINGS,"Split",0,50,25);
    create_spinbox(cont,LV_SYMBOL_SETTINGS,"Maxusage",0,99,90);

    lv_obj_t * sub_isp_page = lv_menu_page_create(menu, "ISP");
    lv_obj_set_style_pad_hor(sub_isp_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_isp_page);
    section = lv_menu_section_create(sub_isp_page);
    cont = lv_menu_cont_create(section);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);    
    create_spinbox(cont,LV_SYMBOL_SETTINGS,"Exposure",5,55,11);
    create_dropdown(cont,LV_SYMBOL_SETTINGS, "Antiflicker", "DISABLED\n50\n60");
    create_dropdown(cont,LV_SYMBOL_SETTINGS, "Sensor File", "/etc/sensors/imx415_fpv.bin\n/etc/sensors/imx415_milos10.bin\n/etc/sensors/imx415_milos15.bin\n/etc/sensors/imx335_milos12tweak.bin\n/etc/sensors/imx335_greg15.bin");
      
    lv_obj_t * sub_fpv_page = lv_menu_page_create(menu, "FPV");
    lv_obj_set_style_pad_hor(sub_fpv_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_fpv_page);
    section = lv_menu_section_create(sub_fpv_page);
    cont = lv_menu_cont_create(section);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);    
    create_switch(cont,LV_SYMBOL_SETTINGS,"Enabled",false);
    create_spinbox(cont,LV_SYMBOL_SETTINGS,"Noiselevel",0,1,0);

    // label = lv_label_create(parent);
    // lv_label_set_text(label, "Recording");
    // section = lv_menu_section_create(parent);

    // cont = lv_menu_cont_create(section);
    // lv_group_add_obj(lv_group_get_default(),cont);
    // label = lv_label_create(cont);
    // lv_label_set_text(label, "Item 1");

    // label = lv_label_create(parent);
    // lv_label_set_text(label, "ISP");
    // section = lv_menu_section_create(parent);

    // cont = lv_menu_cont_create(section);
    // lv_group_add_obj(lv_group_get_default(),cont);
    // label = lv_label_create(cont);
    // lv_label_set_text(label, "Item 1");

    // label = lv_label_create(parent);
    // lv_label_set_text(label, "FPV");
    // section = lv_menu_section_create(parent);

    // cont = lv_menu_cont_create(section);
    // lv_group_add_obj(lv_group_get_default(),cont);
    // label = lv_label_create(cont);
    // lv_label_set_text(label, "Item 1");

    // lv_obj_t * btn_connect = lv_btn_create(section);
    // //lv_obj_add_event_cb(btn_connect, connect_wifi_event_handler, LV_EVENT_CLICKED, NULL);
    // lv_obj_t * label_connect = lv_label_create(btn_connect);
    // lv_label_set_text(label_connect, "Restart Majestic");


    section = lv_menu_section_create(parent);
    cont = create_text(parent, LV_SYMBOL_IMAGE, "Image", LV_MENU_ITEM_BUILDER_VARIANT_1,lv_group_get_default());
    lv_menu_set_load_page_event(menu, cont, sub_image_page);
    cont = create_text(parent, LV_SYMBOL_VIDEO, "Video", LV_MENU_ITEM_BUILDER_VARIANT_1,lv_group_get_default());
    lv_menu_set_load_page_event(menu, cont, sub_video_page);
    cont = create_text(parent, LV_SYMBOL_SD_CARD, "Recording", LV_MENU_ITEM_BUILDER_VARIANT_1,lv_group_get_default());
    lv_menu_set_load_page_event(menu, cont, sub_recording_page);   
    cont = create_text(parent, LV_SYMBOL_SETTINGS, "ISP", LV_MENU_ITEM_BUILDER_VARIANT_1,lv_group_get_default());
    lv_menu_set_load_page_event(menu, cont, sub_isp_page);
    cont = create_text(parent, LV_SYMBOL_SETTINGS, "FPV", LV_MENU_ITEM_BUILDER_VARIANT_1,lv_group_get_default());
    lv_menu_set_load_page_event(menu, cont, sub_fpv_page); 
      
}