#include <stdlib.h>
#define GREPJR_IMPLEMENTATION
#include "GREPJR.h"

int main(int argc, char *argv[]) {
        Config config;
        ErrorCode result = Build(argc, argv, &config);
        if (result == ParseErr) {
                fprintf(stderr, "[%s] A parse error has ocurred: %s.\n", __func__, strerror(errno));
                exit(EXIT_FAILURE);
        }
        Run(&config);
        return 0;
}

