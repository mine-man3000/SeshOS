#include <video/video.h>

class Window
{   public:
        Window(int x, int y, int width, int height, const char* name, uint32_t backgroundColor);
        bool m_LeftMouseDown;
        int  m_MouseX;
        int  m_MouseY;
        int m_Width;
        int m_Height;
        int m_Startx;
        int m_Starty;
        uint32_t m_BackgroundColor;
        char m_Name[20];
        void DrawWindow();
        void UpdateWindow();
};  