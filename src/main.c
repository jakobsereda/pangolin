#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * TODO:
 *  - tokenizer/lexer 
 *  - symbol table
 *  - parser (recursive descent parser on an absract syntax tree)
 *  - code gen
 *  - test
 */

void print_help() {
    printf("USAGE: pangolin [OPTIONS] <FILEPATH>\n");
    printf("OPTIONS:\n");
    printf("  --help, -h           display help message\n");
    printf("  --debug, -d          enable debug mode\n");
    printf("  --output, -o <file>  specify output file\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "USAGE: pangolin [OPTIONS] <FILEPATH>\n");
        return 1;
    }

    char *input_filepath = NULL;
    char *output_filepath = NULL;
    int debug = 0;

    // parse args
    for (int i = 1; i < argc; i++) {
        // -h, --help
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            print_help();
            return 0;
        // -d, --debug
        } else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")) {
            debug = 1;
        // -o, --output <file>
        } else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) {
            if (i + 1 >= argc) {
                fprintf(stderr, "ERROR: no ouput file specified\n");
                fprintf(stderr, "USAGE: pangolin --output <OUTFILE> <FILEPATH>\n");
                return 1;
            }
            output_filepath = argv[++i];
        // handle unknown flags
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "ERROR: unknown flag: %s\n", argv[i]);
            return 1;
        // <FILEPATH>
        } else {
            if (!input_filepath) {
                input_filepath = argv[i];
            } else {
                fprintf(stderr, "ERROR: only one input file expected\n");
                return 1;
            }
        }
    }

    if (!input_filepath) {
        fprintf(stderr, "ERROR: no input file provided\n");
        return 1;
    }

    // open file
    FILE *input_file = fopen(input_filepath, "r");
    if (!input_file) {
        fprintf(stderr, "ERROR: could not open provided input file: %s\n", input_filepath);
        return 1;
    }

    // TODO: output file opening, when we get codegen working to write to it

    // get length of input file
    fseek(input_file, 0, SEEK_END);
    long input_length = ftell(input_file);
    rewind(input_file);

    // malloc memory for input content
    char *content = malloc(input_length + 1);
    if (!content) {
        fprintf(stderr, "ERROR: memory alloc for input failed\n");
        fclose(input_file);
        return 1;
    }

    // populate content
    fread(content, 1, input_length, input_file);
    content[input_length] = '\0';
    fclose(input_file);

    printf("File Content: \n%s\n", content);

    // free content
    free(content);
    content = NULL;
}