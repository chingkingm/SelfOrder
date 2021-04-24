#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int showImage(char *imageName, int w, int y, int x1, int y1)
{
    // 打开开发板
    int lcd = open("/dev/fb0", O_RDWR);
    if (lcd < 0)
    {
        printf("打开lcd失败！\n");
    }
    else
    {
        printf("打开lcd成功！\n");
    }
    // 打开图片
    int imagfd = open(imageName, O_RDWR, 777);
    if (imagfd < 0)
    {
        printf("图片%s打开失败！\n",imageName);
    }
    else
    {
        printf("图片%s打开成功！\n",imageName);
    }
    // 移动光标
    lseek(imagfd, 54, SEEK_SET);
    // 读取图片
    char *lcdBuf = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd, 0);
    char imagBuf[w * y * 3]; // 图片
    int x, y;
    read(imagfd, imagBuf, w * y * 3); // 读取
    for (x = 0; x < 800; x++)
    {
        for (y = 0; y < 480; y++)
        {
            lcdBuf[0 + 4 * x + 800 * y * 4] = imagBuf[0 + 3 * x + 800 * (479 - y) * 3]; // B
            lcdBuf[1 + 4 * x + 800 * y * 4] = imagBuf[1 + 3 * x + 800 * (479 - y) * 3]; // G
            lcdBuf[2 + 4 * x + 800 * y * 4] = imagBuf[2 + 3 * x + 800 * (479 - y) * 3]; // R
            lcdBuf[3 + 4 * x + 800 * y * 4] = 0;                                        // 透明度
        }
    }
    write(lcd, lcdBuf, 800 * 480 * 4);
    munmap(lcdBuf, 800 * 400 * 4);
    close(lcd);
    close(imagfd);
}

int main(){

}