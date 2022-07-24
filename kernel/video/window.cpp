#include "window.h"
#include "video.h"
#include "../string.h"
#include "../shell.h"
#include "../userinput/mouse.h"

uint32_t closeIcon[] = 
{
    0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 2,
    0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 2,
    0x00ff0000, 0x00ff0000, 0xffffffff, 0xffffffff, 0x00ff0000, 0x00ff0000, 0xffffffff, 0xffffffff, 0x00ff0000, 0x00ff0000, 2,
    0x00ff0000, 0x00ff0000, 0xffffffff, 0xffffffff, 0x00ff0000, 0x00ff0000, 0xffffffff, 0xffffffff, 0x00ff0000, 0x00ff0000, 2,
    0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0xffffffff, 0xffffffff, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 2,
    0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0xffffffff, 0xffffffff, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 2,
    0x00ff0000, 0x00ff0000, 0xffffffff, 0xffffffff, 0x00ff0000, 0x00ff0000, 0xffffffff, 0xffffffff, 0x00ff0000, 0x00ff0000, 2,
    0x00ff0000, 0x00ff0000, 0xffffffff, 0xffffffff, 0x00ff0000, 0x00ff0000, 0xffffffff, 0xffffffff, 0x00ff0000, 0x00ff0000, 2,
    0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 2,
    0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 0x00ff0000, 3,
};

void Window::DrawWindow()
{
    drawRect(m_Startx, m_Starty, m_Width, m_Height, 0xffa4a4a4);
    drawRect(m_Startx + 1, m_Starty, m_Width - 2, m_Height - 1, m_BackgroundColor);
    drawRect(m_Startx, m_Starty, m_Width, 16, 0xff0000ff);
    drawImage(closeIcon, m_Startx + m_Width - 12, m_Starty + 2);
    gPrint(m_Name, m_Startx + 2, m_Starty, 0xffffffff);
}

Window::Window(int x, int y, int width, int height, const char* name, uint32_t backgroundColor)
{   
    m_Width = width;
    m_Height = height;
    m_Startx = x;
    m_Starty = y;

    int i = 0;
    for (; name[i] != '\0'; i++)
    {
        m_Name[i] = name[i];
    }
    m_Name[i] = '\0';

    m_BackgroundColor = backgroundColor;
    DrawWindow();
}