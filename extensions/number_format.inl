#include <assert.h>
#include <stdio.h>
#include <math.h>

/*
 * Number Format functions
 */
static int bwa_number_format(bwa_context *ctx, int argc, bwa_value **argv) {
    if (argc < 1) {
        bwa_result_string(ctx, "ERROR: Missing argument", -1);
    }

    double value = bwa_value_to_double(argv[0]);
    int  dp = 0;
    int len_dec;
    int len_sep;
    char dec_point = '.';
    char thousand_sep = ',';

    if(argc > 1){
        dp = bwa_value_to_double(argv[1]);
    }

    if(argc > 2){
        dec_point = *strdup(bwa_value_to_string(argv[2], &len_dec));
    }

    if(argc > 3){
        thousand_sep = *strdup(bwa_value_to_string(argv[3], &len_sep));
    }

    

    bwa_result_string(ctx, number, -1);
    return BWA_OK;
}