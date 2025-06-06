#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <gpiod.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#include "../../lvgl/lvgl.h"
#include "../../lvgl/src/core/lv_global.h"
#include "input.h"

#ifndef USE_SIMULATOR
#define MAX_GPIO_BUTTONS 10  // Adjust based on your hardware
#define MAX_GPIO_CHIPS 5     // Max number of GPIO chips to scan
#define GPIO_PATH "/dev/"    // Base path for GPIO chips
#define DEBOUNCE_DELAY_MS 50 // Debounce delay in milliseconds

typedef struct {
    const char *chip_name;  // GPIO chip device (e.g., "/dev/gpiochip0")
    int line_num;           // GPIO pin number
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int last_state;         // Last state of the GPIO (pressed or released)
    long last_time;         // Last time the state was updated (for debouncing)
} gpio_button_t;

// Define all GPIO buttons
gpio_button_t gpio_buttons[] = {
    {"/dev/gpiochip3", 9,  NULL, NULL, -1, 0},  // Up
    {"/dev/gpiochip3", 10,  NULL, NULL, -1, 0}, // Down
    {"/dev/gpiochip3", 2,  NULL, NULL, -1, 0},  // Left
    {"/dev/gpiochip3", 1,  NULL, NULL, -1, 0}, // Right
    {"/dev/gpiochip3", 18,  NULL, NULL, -1, 0}, // OK
};
#endif

extern lv_obj_t * menu_screen;
extern lv_obj_t * fly_screen;

// Global or static variable to store the next key state
static lv_key_t next_key = LV_KEY_END;  // Default to no key
static bool next_key_pressed = false;    // Indicates if the next key should be pressed or released
gsmenu_control_mode_t control_mode = GSMENU_CONTROL_MODE_NAV;

#ifndef USE_SIMULATOR
// Function to initialize GPIO buttons
void setup_gpio(void) {
    for (size_t i = 0; i < sizeof(gpio_buttons) / sizeof(gpio_buttons[0]); i++) {
        gpio_buttons[i].chip = gpiod_chip_open(gpio_buttons[i].chip_name);
        if (!gpio_buttons[i].chip) {
            perror("Failed to open GPIO chip");
            continue;
        }

        gpio_buttons[i].line = gpiod_chip_get_line(gpio_buttons[i].chip, gpio_buttons[i].line_num);
        if (!gpio_buttons[i].line) {
            perror("Failed to get GPIO line");
            gpiod_chip_close(gpio_buttons[i].chip);
            continue;
        }

        if (gpiod_line_request_input(gpio_buttons[i].line, "lvgl_input") < 0) {
            perror("Failed to request GPIO input");
            gpiod_chip_close(gpio_buttons[i].chip);
            gpio_buttons[i].chip = NULL;
            gpio_buttons[i].line = NULL;
        }
    }
}

// Function to debounce and read GPIO inputs
void handle_gpio_input(void) {
    struct timespec ts;
    for (size_t i = 0; i < sizeof(gpio_buttons) / sizeof(gpio_buttons[0]); i++) {
        if (gpio_buttons[i].chip && gpio_buttons[i].line) {
            int current_state = gpiod_line_get_value(gpio_buttons[i].line);
            
            // Get current time in milliseconds
            clock_gettime(CLOCK_MONOTONIC, &ts);
            long current_time = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;

            // Check if the state has changed and is stable for the debounce period
            if (current_state != gpio_buttons[i].last_state &&
                (current_time - gpio_buttons[i].last_time) > DEBOUNCE_DELAY_MS) {
                gpio_buttons[i].last_state = current_state;
                gpio_buttons[i].last_time = current_time;

                if (current_state == 1) { // Active high: pressed when the value is 1
                    // Adjust for control_mode
                    switch (control_mode)
                    {
                        case GSMENU_CONTROL_MODE_NAV:
                            switch (gpio_buttons[i].line_num) {
                                case 9:  // Up
                                    next_key = LV_KEY_PREV;
                                    break;
                                case 10: // Down
                                    next_key = LV_KEY_NEXT;
                                    break;
                                case 2:  // Left
                                    next_key = LV_KEY_LEFT;
                                    break;
                                case 1:  // Right
                                    next_key = LV_KEY_ENTER;
                                    break;
                                case 18: // OK
                                    next_key = LV_KEY_ENTER;
                                    break;
                            }
                            break;
                        case GSMENU_CONTROL_MODE_EDIT:
                            switch (gpio_buttons[i].line_num) {
                                case 9:  // Up
                                    next_key = LV_KEY_UP;
                                    break;
                                case 10: // Down
                                    next_key = LV_KEY_DOWN;
                                    break;
                                case 2:  // Left
                                    next_key = LV_KEY_LEFT;
                                    break;
                                case 1:  // Right
                                    next_key = LV_KEY_RIGHT;
                                    break;
                                case 18: // OK
                                    next_key = LV_KEY_ENTER;
                                    break;
                            }
                            break;
                        case GSMENU_CONTROL_MODE_SLIDER:
                            switch (gpio_buttons[i].line_num) {
                                case 9:  // Up
                                    next_key = LV_KEY_PREV;
                                    break;
                                case 10: // Down
                                    next_key = LV_KEY_NEXT;
                                    break;
                                case 2:  // Left
                                    next_key = LV_KEY_LEFT;
                                    break;
                                case 1:  // Right
                                    next_key = LV_KEY_RIGHT;
                                    break;
                                case 18: // OK
                                    next_key = LV_KEY_ENTER;
                                    break;
                            }
                            break; 
                        default:
                            break;
                    }
                    next_key_pressed = true;
                    printf("GPIO Pressed: %d (Chip: %s)\n", gpio_buttons[i].line_num, gpio_buttons[i].chip_name);
                    break;
                }
            }
        }
    }
}


// Cleanup function for GPIO
void cleanup_gpio(void) {
    for (size_t i = 0; i < sizeof(gpio_buttons) / sizeof(gpio_buttons[0]); i++) {
        if (gpio_buttons[i].chip) {
            gpiod_chip_close(gpio_buttons[i].chip);
            gpio_buttons[i].chip = NULL;
            gpio_buttons[i].line = NULL;
        }
    }
}
#endif

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
                switch (control_mode)
                {
                case GSMENU_CONTROL_MODE_NAV:                    
                case GSMENU_CONTROL_MODE_SLIDER:
                    next_key = LV_KEY_PREV;
                    break;
                case GSMENU_CONTROL_MODE_EDIT:
                    next_key = LV_KEY_UP;
                    break;
                
                default:
                    break;
                }                
                next_key_pressed = true;
                printf("Up\n");
                break;
            case 's':
            case 'S':
                switch (control_mode)
                {
                case GSMENU_CONTROL_MODE_SLIDER:
                case GSMENU_CONTROL_MODE_NAV:
                    next_key = LV_KEY_NEXT;
                    break;
                case GSMENU_CONTROL_MODE_EDIT:
                    next_key = LV_KEY_DOWN;
                    break;
                
                default:
                    break;
                } 
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
                switch (control_mode)
                {
                case GSMENU_CONTROL_MODE_NAV:
                    next_key = LV_KEY_ENTER;
                    break;
                case GSMENU_CONTROL_MODE_SLIDER:
                case GSMENU_CONTROL_MODE_EDIT:
                    next_key = LV_KEY_RIGHT;
                    break;
                
                default:
                    break;
                }                 
                next_key_pressed = true;
                printf("Right\n");
                break;
            case '\n':
                next_key = LV_KEY_ENTER;
                next_key_pressed = true;
                printf("Enter\n");
                break;
            case 'm':
            case 'M':
                printf("OpenMenu...\n");
                lv_screen_load_anim(menu_screen,LV_SCR_LOAD_ANIM_FADE_IN,200,0,false);
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

#ifndef USE_SIMULATOR
    handle_gpio_input(); // Check GPIO state separately from keyboard input
#endif

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

    set_stdin_nonblock(); // setup keyboard input from stdin
#ifndef USE_SIMULATOR 
    setup_gpio();          // Initialize GPIO
#endif
    lv_indev_t * indev_drv = lv_indev_create();
    lv_indev_set_type(indev_drv, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(indev_drv, virtual_keyboard_read);

    lv_indev_enable(indev_drv, true);

    return indev_drv;
}
