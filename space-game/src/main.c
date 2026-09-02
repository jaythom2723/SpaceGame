#include <obsidian.h>
#include <display/ob_window.h>

int main(void) 
{
    OBinit();

    OBWNDsetTitle("Test Window");
    OBWNDsetSize(800, 600);
    OBWNDcreateWindow();

    while (OBWNDshouldClose() == false)
    {
        OBWNDpollEvents();

        OBWNDswapBuffers();
    }
 
    OBWNDdestroyWindow();
    OBclose();

    return 0;
}