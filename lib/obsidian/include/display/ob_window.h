#ifndef OB_WINDOW_H
#define OB_WINDOW_H

#include <stdbool.h>
#include <stdint.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct obsidian_window {
    char* title;
    uint32_t width, height;
    GLFWwindow* handle;
};

bool OBWNDcreateWindow(void);
bool OBWNDdestroyWindow(void);

void OBWNDpollEvents(void);
void OBWNDswapBuffers(void);

bool OBWNDsetTitle(const char* buffer);
bool OBWNDsetSize(const uint32_t width, const uint32_t height);

bool OBWNDshouldClose(void);

const char* OBWNDgetTitle(void);
uint32_t OBWNDgetWidth(void);
uint32_t OBWNDgetHeight(void);
void OBWNDgetSize(uint32_t* restrict width, uint32_t* restrict height);
const GLFWwindow* OBWNDgetHandle(void);

#endif