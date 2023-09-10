#ifndef GREPJR_H_
#define GREPJR_H_
#define SIZE        256
#define COLOR_RED   33
#define COLOR_GREEN 32
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

typedef struct {
        char filepath[SIZE];
        char query[SIZE];
} Config;

typedef enum {
        Ok,
        ParseErr,
} ErrorCode;

ErrorCode Build(const int argc, char *argv[], Config *config);
char *readContent(const char *path);
void search(const char *query, char *content);
void Run(const Config *config);

#endif // GREPJR_H_

#ifdef GREPJR_IMPLEMENTATION 

ErrorCode Build(const int argc, char *argv[], Config *config) {
        if (argc != 3) {
                fprintf(stderr, "Usage is '%s' <query> <filename>.\n", argv[0]);
                return ParseErr;
        }

        if (*argv [1] == '\0') {
                fprintf(stderr, "A query has not been stated.\n");
                return ParseErr;
        }

        if (*argv [2] == '\0') {
                fprintf(stderr, "A filepath has not been stated.\n");
                return ParseErr;
        }

        strncpy(config->query, argv[1], sizeof(config->query));
        strncpy(config->filepath, argv[2], sizeof(config->filepath));
        config->query[sizeof(config->query) - 1] = '\0';
        config->filepath[sizeof(config->filepath) - 1] = '\0';

        return Ok;
}

char *readContent(const char *path) {
        FILE *file = fopen(path, "r");
        if (file == NULL) {
                fprintf(stderr, "[%s] Error opening '%s' file: %s.\n", __func__, path, strerror(errno));
                exit(EXIT_FAILURE);
        }

        fseek(file, 0, SEEK_END);
        size_t file_size = ftell(file);
        rewind(file);

        char *content = (char *)malloc(file_size + 1);

        if (content == NULL) {
                fclose(file);
                fprintf(stderr, "[%s] Error allocating memory for storing the content of the file: %s.\n", __func__, strerror(errno));
                exit(EXIT_FAILURE);
        }

        size_t bytes_read = fread(content, 1, file_size, file);
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

void search(const char *query, char *content) {
        char *line_ptr = strtok((char*)content, "\n");
        int line_num = 1;
        while (line_ptr != NULL) {
                char *sub_str_pointer = strstr(line_ptr, query);
                if (sub_str_pointer != NULL) {
                        printf("\x1b[%dm%d\x1b[0m: ", COLOR_GREEN, line_num);
                        while (sub_str_pointer != NULL) {
                            size_t startPosition = sub_str_pointer - line_ptr;

                            for (size_t i = 0; i < startPosition; i++) {
                                printf("%c", line_ptr[i]);
                            }
                            printf("\033[1m\x1b[%dm", COLOR_RED);
                            for (size_t i = 0; i < strlen(query); i++){
                                printf("%c", line_ptr[startPosition + i]);
                            }
                            printf("\x1b[0m\033[m");

                            line_ptr = sub_str_pointer + strlen(query);
                            sub_str_pointer = strstr(line_ptr, query);
                    }
                    printf("%s\n", line_ptr);
                }
                line_num += 1;
                line_ptr = strtok(NULL, "\n");
        }
}
void Run(const Config *config) {
        char *content = readContent(config->filepath);
        search(config->query, content);
        free(content);
}
#endif //GREPJR_IMPLEMENTATION
