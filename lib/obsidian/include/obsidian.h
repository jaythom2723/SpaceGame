#ifndef OBISIDIAN_H
#define OBISIDIAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

struct obsidian_context {
    GLFWwindow* window;
};

struct obsidian_context* OBcreateContext(void);
void OBdestroyContext(struct obsidian_context* ctx);

#endif