#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

/* The name of this program. */
const char* program_name;

/* Prints usage information for this program to STREAM (stdout or stderr), 
   and exits the program with EXIT_CODE. */
void print_usage(FILE* stream, int exit_code)
{
    fprintf(stream, "Usage: %s options [ inputfile ... ]\n", program_name);
    fprintf(stream,
        "  -h --help        Display this usage information.\n"
        "  -o --output      filename Write output to file.\n"
        "  -v --verbose     Print verbose messages.\n");
    exit(exit_code);
}

/* Main program entry point. */
int main(int argc, char* argv[])
{
    int next_option;

    /* A string listing valid short options letters. */
    const char* const short_options = "ho:v";

    /* An array describing valid long options. */
    const struct option long_options[] = {
        { "help",    0, NULL, 'h' },
        { "output",  1, NULL, 'o' },
        { "verbose", 0, NULL, 'v' },
        { NULL,      0, NULL, 0 } /* Required at end of array. */
    };

    /* The name of the file to receive program output, or NULL for stdout. */
    const char* output_filename = NULL;

    /* Whether to display verbose messages. */
    int verbose = 0;

    /* Store program name from argv[0]. */
    program_name = argv[0];

    /* Parse command-line options. */
    do {
        next_option = getopt_long(argc, argv, short_options, long_options, NULL);

        switch (next_option)
        {
            case 'h': /* -h or --help */
                print_usage(stdout, 0);
                break;
            case 'o': /* -o or --output */
                output_filename = optarg;
                break;
            case 'v': /* -v or --verbose */
                verbose = 1;
                break;
            case '?': /* Invalid option */
                print_usage(stderr, 1);
                break;
            case -1: /* Done with options */
                break;
            default: /* Unexpected */
                abort();
        }
    } while (next_option != -1);

    /* Print non-option arguments if verbose. */
    if (verbose) {
        int i;
        for (i = optind; i < argc; ++i)
            printf("Argument: %s\n", argv[i]);
    }

    /* Main program logic would go here. */
    if (output_filename != NULL) {
        printf("Output would be written to: %s\n", output_filename);
    }

    return 0;
}
