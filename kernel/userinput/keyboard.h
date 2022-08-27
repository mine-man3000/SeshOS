#include <stdint.h>
#include "../string.h"
#include "../shell.h"

#define us_qwerty_left_shift	0x2A
#define us_qwerty_right_shift	0x36
#define us_qwerty_enter		0x1C
#define us_qwerty_backspace	0x0E


extern uint8_t is_us_qwerty_left_shift_pressed;
extern uint8_t is_us_qwerty_right_shift_pressed;

extern char us_qwerty_ascii_ps2_mapping[];

char get_us_qwerty_char_from_scan_code(uint8_t scan_code);
void HandleKeyboard(uint8_t scancode);