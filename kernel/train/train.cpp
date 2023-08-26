#include <train/train.h>
#include <train/image.h>
#include <time.h>

int x = 0;

void train()
{
    x = buffer->width - g_image_data[0];
    drawImage(g_image_data, x, (buffer->height / 2) - (g_image_data[1] / 2));
    for (int i = 0; i < 75; i++)
    {
        sleep(1);
        x -= 10;
        drawImage(g_image_data, x, (buffer->height / 2) - (g_image_data[1] / 2));
    }
}