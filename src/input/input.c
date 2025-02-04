#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "../../lvgl/lvgl.h"
#include "../../lvgl/src/core/lv_global.h"

// Global or static variable to store the next key state
static lv_key_t next_key = LV_KEY_END;  // Default to no key
static bool next_key_pressed = false;    // Indicates if the next key should be pressed or released

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
                next_key = LV_KEY_UP;
                next_key_pressed = true;
                printf("Up\n");
                break;
            case 's':
            case 'S':
                next_key = LV_KEY_DOWN;
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
                next_key = LV_KEY_RIGHT;
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
    if (next_key != LV_KEY_END) {
        data->key = next_key;

        // Simulate the press/release alternating behavior
        if (next_key_pressed) {
            data->state = LV_INDEV_STATE_PR;  // Key is pressed
        } else {
            data->state = LV_INDEV_STATE_REL;  // Key is released
        }

        // After sending the event, toggle the key state for next time
        next_key_pressed = !next_key_pressed;  // Toggle the key state for alternating presses/releases
        next_key = LV_KEY_END;  // Reset next_key to indicate it's been processed
    }
}

// Function to create the virtual keyboard
lv_indev_t * create_virtual_keyboard(void) {
    lv_indev_t *indev_drv = lv_indev_create();
    lv_indev_set_type(indev_drv, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(indev_drv, virtual_keyboard_read);

    return indev_drv; // Return the new input device
}

