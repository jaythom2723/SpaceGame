#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb-image/stb_image.h>

#define DEPTH_RGBA                  32
#define DEPTH_RGB                   24
#define DEPTH_RGBA16                16
#define DEPTH_RGB16                 16

#define DEFAULT_FILE_EXT            "obtf"
#define DEFAULT_COLOR_BIT_DEPTH     DEPTH_RGBA;
#define DEFAULT_OUTPUT_PATH         "./"

#define FLAG_SET_OUTPUT             "-o"
#define FLAG_SHOW_HELP              "-h"
#define FLAG_SET_DEPTH              "-d"

// static bool display_help = false;
static char* input_path = NULL;
static char* output_path = NULL;
// static uint32_t bit_depth = DEFAULT_COLOR_BIT_DEPTH;

void __ob_gen_output_path(void);
void __ob_clone_string(char** restrict dest, const char* restrict src);
void __ob_process_clargs(int argc, char** argv);

int main(int argc, char** argv)
{
    argv++;
    argc--;

    __ob_process_clargs(argc, argv);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(input_path, &width, &height, &nrChannels, 4);
    if (data == NULL)
    {
        printf("Could not find file!\n");
        return -1;
    }

    if (output_path == NULL)
        __ob_gen_output_path();

    FILE* fp = fopen(output_path, "wb");
    if (fp == NULL)
    {
        printf("Failed to open new file!\n");
        return -2;
    }

    printf("%d\n%d\n", width, height);

    fwrite(&width, sizeof(int), 1, fp);
    fwrite(&height, sizeof(int), 1, fp);
    fwrite(&nrChannels, sizeof(int), 1, fp);
    fwrite(data , sizeof(char), width * height * 4, fp);

    fclose(fp);
    fp = NULL;

    stbi_image_free(data);

    free(output_path);
    free(input_path);

    output_path = NULL;
    input_path = NULL;

    return 0;
}

void __ob_gen_output_path(void)
{
    if (input_path == NULL)
        return;
    
    output_path = calloc(strlen(input_path)+2, sizeof(char));
    if (output_path == NULL)
        return;

    memcpy(output_path, input_path, strlen(input_path)-3 * sizeof(char));
    strcat(output_path, DEFAULT_FILE_EXT);

    printf("%s\n", output_path);
}

void __ob_clone_string(char** restrict dest, const char* restrict src)
{
    (*dest) = calloc(strlen(src)+1, sizeof(char));
    if (dest == NULL)
        return;
    strcpy(*dest, src);
}

// TODO: come up with a better way to do this bullshit
void __ob_process_clargs(int argc, char** argv)
{
    for (int i = 0; i < argc; i++)
    {
        char* arg = *(argv + i);

        if (strcmp(arg, FLAG_SET_OUTPUT) == 0)
        {
            char* op = *(argv + (++i));
            __ob_clone_string(&output_path, op);
        } else
        {
            // This should be the input file, if its not, someone fucked up lol
            __ob_clone_string(&input_path, arg);
        }
    }
}