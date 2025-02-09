#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lvgl/lvgl.h"
#include "helper.h"
#include "air_wfbng.h"

FrequencyEntry Frequency24GHz[] = {
    {0, ""}, {1, "2412 MHz [1]"}, {2, "2417 MHz [2]"}, {3, "2422 MHz [3]"},
    {4, "2427 MHz [4]"}, {5, "2432 MHz [5]"}, {6, "2437 MHz [6]"}, {7, "2442 MHz [7]"},
    {8, "2447 MHz [8]"}, {9, "2452 MHz [9]"}, {10, "2457 MHz [10]"}, {11, "2462 MHz [11]"},
    {12, "2467 MHz [12]"}, {13, "2472 MHz [13]"}, {14, "2484 MHz [14]"}
};

int Frequency24GHzSize = sizeof(Frequency24GHz) / sizeof(Frequency24GHz[0]);

FrequencyEntry Frequency58GHz[] = {
    {0, ""}, {36, "5180 MHz [36]"}, {40, "5200 MHz [40]"}, {44, "5220 MHz [44]"},
    {48, "5240 MHz [48]"}, {52, "5260 MHz [52]"}, {56, "5280 MHz [56]"}, {60, "5300 MHz [60]"},
    {64, "5320 MHz [64]"}, {100, "5500 MHz [100]"}, {104, "5520 MHz [104]"}, {108, "5540 MHz [108]"},
    {112, "5560 MHz [112]"}, {116, "5580 MHz [116]"}, {120, "5600 MHz [120]"}, {124, "5620 MHz [124]"},
    {128, "5640 MHz [128]"}, {132, "5660 MHz [132]"}, {136, "5680 MHz [136]"}, {140, "5700 MHz [140]"},
    {144, "5720 MHz [144]"}, {149, "5745 MHz [149]"}, {153, "5765 MHz [153]"}, {157, "5785 MHz [157]"},
    {161, "5805 MHz [161]"}, {165, "5825 MHz [165]"}, {169, "5845 MHz [169]"}, {173, "5865 MHz [173]"},
    {177, "5885 MHz [177]"}
};

int Frequency58GHzSize = sizeof(Frequency58GHz) / sizeof(Frequency58GHz[0]);


char *get_frequencies_string(FrequencyEntry *entries, int size) {
    int buffer_size = 1024;
    char *buffer = malloc(buffer_size);
    if (!buffer) return NULL;
    buffer[0] = '\0';
    
    for (int i = 0; i < size; i++) {
        if (strlen(entries[i].frequency) > 0) {
            strncat(buffer, entries[i].frequency, buffer_size - strlen(buffer) - 1);
            strncat(buffer, "\n", buffer_size - strlen(buffer) - 1);
        }
    }
    return buffer;
}

void create_air_wfbng_menu(lv_obj_t * parent) {

    create_dropdown(parent,LV_SYMBOL_SETTINGS, "Frequency", get_frequencies_string(Frequency58GHz, Frequency58GHzSize));
    create_dropdown(parent,LV_SYMBOL_SETTINGS, "Bandwith", "20\n40");
    create_slider(parent,LV_SYMBOL_SETTINGS, "MCS Index", 0 , 11, 1);
    create_switch(parent,LV_SYMBOL_SETTINGS,"STBC",true);
    create_switch(parent,LV_SYMBOL_SETTINGS,"LDPC",true);
    create_slider(parent,LV_SYMBOL_SETTINGS, "FEC_K", 0 , 12, 12);
    create_slider(parent,LV_SYMBOL_SETTINGS, "FEC_N", 0 , 12, 8);

}
