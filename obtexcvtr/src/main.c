#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*
    -x delete origin image
    -v verbose mode
    -l set log output path
    -f input file format
    -q quiet mode
*/

enum cli_flags {
    DELETE_ORIGIN_IMAGE = 0b000001,
    VERBOSE_MODE        = 0b000010,
    LOG_OUTPUT_PATH     = 0b000100,
    INPUT_FILE_FORMAT   = 0b001000,
    QUIET_MODE          = 0b010000,
    OUTPUT_PATH         = 0b100000,
};

enum input_format {
    FORMAT_PNG,
    FORMAT_JPG,
    FORMAT_BMP,
};

// TODO: develop a help page/man page for this tool

bool __ob_check_cli_argument(const uint32_t, uint32_t*, char**);

static enum input_format iformat = -1;
static bool verbose_mode = false;
static bool quiet_mode = false;
static bool delete_origin_image = false;
static char* input_path = NULL;
static char* output_path = NULL;
static char* log_output_path = NULL;

int main(int argc, char** argv)
{
    if (argc <= 1)
        return -1; // TODO: print usage/help page

    uint32_t index = 0;
    uint32_t offset = 1;
    while (argc != 0)
    {
        offset = 1;
        if (__ob_check_cli_argument(index, &offset, argv))
        {

        }
        index += offset;
        argc--;
    }

    /*
    static char* input_path = NULL;
static char* output_path = NULL;
static char* log_output_path = NULL;
    */

    free(input_path);
    free(output_path);
    if (log_output_path != NULL)
        free(log_output_path);

    return 0;
}

bool __ob_check_cli_argument(const uint32_t index, uint32_t* offset, char** argv)
{
    char* arg = *(argv + index);
    printf("%s\n", arg);

    if (strcmp(arg, "-o") == 0)
    {
        char* path = *(argv + (index + 1));
        (*offset) ++;
        output_path = calloc(strlen(path)+1, sizeof(char));
        if(output_path == NULL)
            return false;
        strcpy(output_path, path);
        return true;
    }

    if (strcmp(arg, "-x") == 0)
    {
        delete_origin_image = true;
        return true;
    }

    if (strcmp(arg, "-v") == 0 && quiet_mode == false)
    {
        verbose_mode = true;
        return true;
    }

    if (strcmp(arg, "-l") == 0)
    {
        char* path = *(argv + (index + 1));
        (*offset) ++;
        log_output_path = calloc(strlen(path)+1, sizeof(char));
        if (log_output_path == NULL)
            return false;
        strcpy(log_output_path, path);
        return true;
    }

    if (strcmp(arg, "-f") == 0)
    {
        char* format = *(argv + (index + 1));
        // TODO: segfault here
        (*offset) ++;
        if (strcmp(format, "PNG") == 0 || strcmp(format, "png") == 0)
            iformat = FORMAT_PNG;
        if (strcmp(format, "JPG") == 0 || strcmp(format, "jpg") == 0)
            iformat = FORMAT_JPG;
        if (strcmp(format, "BMP") == 0 || strcmp(format, "bmp") == 0)
            iformat = FORMAT_BMP;
        return true;
    }

    if (strcmp(arg, "-q") == 0 && verbose_mode == false)
    {
        quiet_mode = true;
        return true;
    }

    return false;
}