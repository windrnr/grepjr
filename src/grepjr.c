#define GREPJR_IMPLEMENTATION
#include "GREPJR.h"

int main(int argc, char *argv[]) {
        Config config;
        ErrorCode result = Build(argc, argv, &config);
        if (result == ParseErr) {
                fprintf(stderr, "A parse error has ocurred, exiting.\n");
                exit(1);
        }
        Run(&config);


        return 0;
}

