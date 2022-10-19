#include <stdint.h>
#include <stddef.h>
#include "../limine.h"
#include "../debugout.h"

/**
 * @brief startx but better
 * 
 */
void initGUI();

/**
 * @brief draws a rectangle at the given position
 * 
 * @param startx where on the screen the top left most corner will be on the X axis
 * @param starty where on the screen the top left most corner will be on the Y axis
 * @param width  how wide the rectangle will be
 * @param height how tall the rectangle will be
 * @param VGA_COLOR the color of the rectangle given as 0xAARRGGBB (alpha, red, green, blue)
 */
uint32_t* drawRect(int startx, int starty, int width, int height, uint32_t VGA_COLOR);
uint32_t* drawImage(uint32_t *icon, int posx, int posy);
uint32_t* Clear(uint32_t color);
void putPixel(int x, int y, uint32_t pixel);

void Panic(const char *panicMessage);
uint32_t getPixel(uint32_t x, uint32_t y);

void setCursorPos(int x, int y);
void getCursorPos();

extern volatile struct limine_framebuffer_request framebuffer_request;
extern struct limine_framebuffer* buffer;