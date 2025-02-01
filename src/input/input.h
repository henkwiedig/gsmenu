#include <stdint.h>
#include "../../lvgl/lvgl.h"
#include "../../lvgl/src/core/lv_global.h"

void simulate_key_press(uint32_t key_code);

// Function to make stdin non-blocking
void set_stdin_nonblock(void);
// Function to restore terminal settings
void restore_stdin(void);

// Handle WASD input and convert to LVGL key codes
void handle_keyboard_input(void);


// Custom function to simulate keyboard input
static void virtual_keyboard_read(lv_indev_t * indev, lv_indev_data_t * data);

// Function to create the virtual keyboard
lv_indev_t * create_virtual_keyboard(void);