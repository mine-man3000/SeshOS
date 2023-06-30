#include "keyboard.h"

bool isLeftShiftPressed;
bool isRightShiftPressed;

extern bool xRunning;

char input[100];
int i = 0;

void HandleKeyboard(uint8_t scancode)
{
	char c = get_us_qwerty_char_from_scan_code(scancode);

	if (scancode == us_qwerty_left_shift)
	{
		is_us_qwerty_left_shift_pressed = 1;
		c = '\0';
	}
	else if (scancode == us_qwerty_right_shift)
	{
		is_us_qwerty_right_shift_pressed = 1;
		c = '\0';
	}

	if (scancode == us_qwerty_left_shift + 0x80)
	{
		is_us_qwerty_left_shift_pressed = 0;
		c = '\0';
	}
	else if (scancode == us_qwerty_right_shift + 0x80)
	{
		is_us_qwerty_right_shift_pressed = 0;
		c = '\0';
	}

	if (scancode == us_qwerty_enter && xRunning == false)
	{
		printf("\n");
		c = '\0';
		i = 0;
		//newShell.TestCMD(input);
		//newShell.PrintPrompt();
		
		for (int i = 0; i < 100; i ++) {
		    input[i] = '\0';
		}
	}
	else if (scancode == us_qwerty_backspace)
	{
		printf("\b \b");
		c = '\0';
		i--;
		input[i] = '\0';
		if (i < 0)
		{
			printf(" ");
			i = 0;
		}
	}

	if (c != '\0' && xRunning == false)
	{
		char str[2] = {c, '\0'};
		input[i] = c;
		i++;
		printf(str);
	}

	uint16_t port = 0x20;
	uint8_t value = 0x20;

    asm volatile("outb %0, %1" :: "a"(value), "Nd"(port));
}

uint8_t is_us_qwerty_left_shift_pressed;
uint8_t is_us_qwerty_right_shift_pressed;

char us_qwerty_ascii_ps2_mapping[] = {
	'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', 
	'9', '0', '-', '=', '\0', '\0', 'q', 'w', 'e', 'r', 
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\0', '\0', 
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', 
	'\'', '`', '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 
	'm', ',', '.', '/', '\0', '*', '\0', ' '
};

char get_us_qwerty_char_from_scan_code(uint8_t scancode)
{
	if (scancode > 58) return '\0';
	
	char c = us_qwerty_ascii_ps2_mapping[scancode];
	
	if ((is_us_qwerty_left_shift_pressed || is_us_qwerty_right_shift_pressed))
	{
		// letters
		if (c > 96 && c < 123) c -= 32;
		
		// numbers
		if		(c == '1')	c = '!';
		else if (c == '2')	c = '@';
		else if (c == '3')	c = '#';
		else if (c == '4')	c = '$';
		else if (c == '5')	c = '%';
		else if (c == '6')	c = '^';
		else if (c == '7')	c = '&';
		else if (c == '8')	c = '*';
		else if (c == '9')	c = '(';
		else if (c == '0')	c = ')';
		
		// misc
		if		(c == '`' )	c = '~';
		else if (c == '-' )	c = '_';
		else if (c == '=' )	c = '+';
		else if (c == '[' )	c = '{';
		else if (c == ']' )	c = '}';
		else if (c == '\\')	c = '|';
		else if (c == ';' )	c = ':';
		else if (c == '\'')	c = '"';
		else if (c == ',' )	c = '<';
		else if (c == '.' )	c = '>';
		else if (c == '/' )	c = '?';
	}
	
	return c;
}