#include "video.h"
#include "../string.h"
#include "../time.h"

volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void putPixel(int x, int y, uint32_t pixel)
{
    *((uint32_t*)(buffer->address + 4 * (buffer->pitch / 4) * y + 4 * x)) = pixel;
}

uint32_t getPixel(uint32_t x, uint32_t y)
{
    return *(uint32_t *)(uint32_t*)(buffer->address + 4 * (buffer->pitch / 4) * y + 4 * x);
}

void drawRect(int startx, int starty, int width, int height, uint32_t VGA_COLOR)
{
    int endx = startx + width;
    int endy = starty + height;

    for (int x = startx; x < endx; x++)
    {
        for (int y = starty; y < endy; y++)
        {
            putPixel(x, y, VGA_COLOR);
        }
    }
}   

uint8_t drawImage(uint32_t *icon, int posx, int posy)
{
    int x = posx;
    int y = posy;
    for (int i = 0; icon[i] != 3; i++)
    {
        if(icon[i] == 2 || icon[i] == '2')
        {
            x = posx;
            y++;
        }
        else
        {
            putPixel(x, y, icon[i]);
            x++;
        }
    }
}

void Panic(const char *panicMessage)
{
    Clear(0);
    printf("Kernel Panic\n\n%s \n\nSeshOS has been shutdown to prevent damage to your computer", panicMessage);
    asm("hlt");
}

void Clear(uint32_t color)
{
    printf("%s", "\033[2J \033[H");
    //for (int x = 0; x < buffer->width; x++)
    //{
    //    for (int y = 0; y < buffer->height; y++)
    //    {
    //        putPixel(x, y, 0);
    //    }
    //}
}

void setCursorPos(int x, int y)
{
    printf("\033[%i;%iH", x, y);
    printf("%s", "\033[1;1H");
}

void getCursorPos()
{
    printf("%s","\033[6n");
}