#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "../../lvgl/lvgl.h"
#include "../../lvgl/src/core/lv_global.h"

#include "input.h"

// Global or static variable to store the next key state
static lv_key_t next_key = LV_KEY_END;  // Default to no key
static bool next_key_pressed = false;    // Indicates if the next key should be pressed or released
gsmenu_control_mode_t control_mode = GSMENU_CONTROL_MODE_NAV;

// Function to make stdin non-blocking
void set_stdin_nonblock(void) {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    
    // Disable canonical mode and echo
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Function to restore terminal settings
void restore_stdin(void) {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Handle WASD input and convert to LVGL key codes
void handle_keyboard_input(void) {
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0) {
        switch(c) {
            case 'w':
            case 'W':
                next_key = control_mode == GSMENU_CONTROL_MODE_NAV ? LV_KEY_PREV : LV_KEY_UP;
                next_key_pressed = true;
                printf("Up\n");
                break;
            case 's':
            case 'S':
                next_key = control_mode == GSMENU_CONTROL_MODE_NAV ? LV_KEY_NEXT : LV_KEY_DOWN;
                next_key_pressed = true;
                printf("Down\n");
                break;
            case 'a':
            case 'A':
                next_key = LV_KEY_LEFT;
                next_key_pressed = true;
                printf("Left\n");
                break;
            case 'd':
            case 'D':
                next_key = control_mode == GSMENU_CONTROL_MODE_NAV ? LV_KEY_ENTER : LV_KEY_RIGHT;
                next_key_pressed = true;
                printf("Right\n");
                break;
            case '\n':
                next_key = LV_KEY_ENTER;
                next_key_pressed = true;
                printf("Enter\n");
                break;
            case 'q':
            case 'Q':
                printf("Exiting...\n");
                restore_stdin();
                exit(0);
                break;
        }
    }
}

// Custom function to simulate keyboard input
static void virtual_keyboard_read(lv_indev_t * indev, lv_indev_data_t * data) {
    static bool key_sent = false;  // Track if a key event was sent
    
    if (next_key != LV_KEY_END) {
        data->key = next_key;
        data->state = next_key_pressed ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;

        next_key_pressed = !next_key_pressed;  // Toggle state

        if (!next_key_pressed) {  
            next_key = LV_KEY_END;  // Reset key after release event
        }

        key_sent = true;  // Mark that a key was sent
    } else if (key_sent) {
        data->state = LV_INDEV_STATE_REL;  // Ensure release event is sent
        key_sent = false;  // Reset the flag
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

// Function to create the virtual keyboard
lv_indev_t * create_virtual_keyboard() {

    // setup keybard inout from stdin
    set_stdin_nonblock();

    lv_indev_t * indev_drv = lv_indev_create();
    lv_indev_set_type(indev_drv, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(indev_drv, virtual_keyboard_read);

    lv_indev_enable(indev_drv, true);

    return indev_drv;
}

