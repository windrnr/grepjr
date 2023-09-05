#ifndef GREPJR_H_
#define GREPJR_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
        char filepath[100];
        char query[100];
} Config;

typedef enum {
        Ok,
        ParseErr,
} ErrorCode;

ErrorCode Build(const int argc, char *argv[], Config *config);
void Run(const Config *config);
char *readContent(const char *path);
void search(char *query, char *content);

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
                fprintf(stderr, "Error opening '%s' file. Aborting.\n", path);
                exit(1);
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        char *content = (char *)malloc(file_size + 1);

        if (content == NULL) {
                fclose(file);
                fprintf(stderr, "Error allocating memory for storing the content of the file. Aborting\n");
                exit(1);
        }

        size_t bytes_read = fread(content, 1, file_size, file);
        content[bytes_read] = '\0';

        if (bytes_read != file_size) {
                free(content);
                fclose(file);
                fprintf(stderr, "Error reading the file. Aborting");
                exit(1);
        }
        fclose(file);
        return content;
}

void search(char *query, char *content) {
    char *line = strtok((char*)content, "\n");
    while (line != NULL) {
        char *substringPosition = line;
        while ((substringPosition = strstr(substringPosition, query)) != NULL) {
            /* char *substringPosition = strstr(line, query); */
            /* if (substringPosition != NULL) { */ 
            int startPosition = substringPosition - line;
            int endPosition = startPosition + strlen(query);

            char *substring = (char *)malloc(strlen(query) + 1);
            strncpy(substring, line + startPosition, endPosition - startPosition);
            substring[endPosition - startPosition] = '\0';

            printf("%.*s", startPosition, line);
            printf("\e[1m\x1b[%dm%s\x1b[0m\e[m", 33, substring);
            printf("%s\n", line + endPosition);
            free(substring);
            substringPosition++;
        }
        line = strtok(NULL, "\n");
    }
}

void Run(const Config *config) {
        char *content = readContent(&config->filepath);
        search(&config->query, content);
        free(content);
}
#endif //GREPJR_IMPLEMENTATION
