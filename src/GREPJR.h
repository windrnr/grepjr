#ifndef GREPJR_H_
#define GREPJR_H_
#include <stdint.h>
#define SIZE        256
#define COLOR_YELLOW 33
#define COLOR_GREEN 32
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

typedef char byte;
typedef int32_t i32;
typedef size_t usize;

struct Config {
        byte filepath[SIZE];
        byte query[SIZE];
};
typedef struct Config Config;

struct FileBuild{
        i32 ok;
};
typedef struct FileBuild FileBuild;

static FileBuild Build(i32 argc, byte *argv[], Config *config);
static byte *readContent(byte *path);
static void search(byte *query, byte *content);
static void Run(Config *config);

#endif // GREPJR_H_

#ifdef GREPJR_IMPLEMENTATION 

static FileBuild Build(i32 argc, byte *argv[], Config *config) {
        FileBuild r = {0};

        if (argc != 3) {
                fprintf(stderr, "Usage is '%s' <query> <filename>.\n", argv[0]);
                return r;
        }

        if (*argv [1] == '\0') {
                fprintf(stderr, "A query has not been stated.\n");
                return r;
        }

        if (*argv [2] == '\0') {
                fprintf(stderr, "A filepath has not been stated.\n");
                return r;
        }

        strncpy(config->query, argv[1], sizeof(config->query));
        strncpy(config->filepath, argv[2], sizeof(config->filepath));
        config->query[sizeof(config->query) - 1] = '\0';
        config->filepath[sizeof(config->filepath) - 1] = '\0';

        r.ok = 1;
        return r;
}

static byte *readContent(byte *path) {
        FILE *file = fopen(path, "r");
        if (file == NULL) {
                fprintf(stderr, "[%s] Error opening '%s' file: %s.\n", __func__, path, strerror(errno));
                exit(EXIT_FAILURE);
        }

        fseek(file, 0, SEEK_END);
        usize file_size = ftell(file);
        rewind(file);

        byte *content = (byte *)malloc(file_size + 1);

        if (content == NULL) {
                fclose(file);
                fprintf(stderr, "[%s] Error allocating memory for storing the content of the file: %s.\n", __func__, strerror(errno));
                exit(EXIT_FAILURE);
        }

        usize bytes_read = fread(content, 1, file_size, file);
        content[bytes_read] = '\0';

        if (bytes_read != file_size) {
                free(content);
                fclose(file);
                fprintf(stderr, "[%s] Error reading the file: %s.\n", __func__, strerror(errno));
                exit(EXIT_FAILURE);
        }
        fclose(file);
        return content;
}

static void print_text(byte *query, byte* line_ptr, usize startPosition) {
        for (usize i = 0; i < startPosition; i++) {
            printf("%c", line_ptr[i]);
        }
        printf("\033[1m\x1b[%dm", COLOR_YELLOW);
        for (usize i = 0; i < strlen(query); i++){
            printf("%c", line_ptr[startPosition + i]);
        }
        printf("\x1b[0m\033[m");
}


static void search(byte *query, byte *content) {
        byte *line_ptr = strtok((byte*)content, "\n");
        for (i32 line_num = 1;line_ptr != NULL;
             line_num++, line_ptr = strtok(NULL,"\n")) {
             
                byte *sub_str_pointer = strstr(line_ptr, query);
                if (sub_str_pointer != NULL) {
                        printf("\x1b[%dm%d\x1b[0m: ", COLOR_GREEN, line_num);
                        while (sub_str_pointer != NULL) {
                            usize startPosition = sub_str_pointer - line_ptr;
                            
                            print_text(query, line_ptr, startPosition);

                            line_ptr = sub_str_pointer + strlen(query);
                            sub_str_pointer = strstr(line_ptr, query);
                        }
                    printf("%s\n", line_ptr);
                }
        }
}

static void Run(Config *config) {
        byte *content = readContent(config->filepath);
        search(config->query, content);
        free(content);
}
#endif //GREPJR_IMPLEMENTATION
