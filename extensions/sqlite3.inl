#include <sqlite3.h>

static int bwa_sqlite_query(bwa_context *ctx, int argc, bwa_value **argv) {
    sqlite3 *db;
    sqlite3_stmt *stmt = NULL;
    const char *query, *error, *name;
    int qlen,rc,done,retry,i,count,row,len ;
    bwa_value *result = NULL,*arr = NULL,*val = NULL;
    char idx[16];

    if (argc < 2) {
        error = "Missing argument(s)";
        bwa_result_string(ctx, error, strlen(error));
        goto FAIL;
    }

    if (!bwa_value_is_string(argv[0]) || !bwa_value_is_string(argv[1])) {
        error = "Invalid argument(s) (want int, string)";
        bwa_result_string(ctx, error, strlen(error));
        goto FAIL;
    }

    name = concat("/", bwa_value_to_string(argv[0],&len));
    const char * db_path = concat(HOME, name);

    if (len < 1) {
        error = "Invalid argument: string must not be blank";
        bwa_result_string(ctx, error, strlen(error));
        goto FAIL;
    }

    if (sqlite3_open(db_path, &db) != SQLITE_OK) {
        error = sqlite3_errmsg(db);
        bwa_result_string(ctx, error, strlen(error));
        db = NULL;
        goto FAIL;
    }

    if (db == NULL || (void *)db - (void *)0x8000000 < 1) {
        error = "Invalid sqlite3 database handle";
        bwa_result_string(ctx, error, strlen(error));
        goto FAIL;
    }

    query = bwa_value_to_string(argv[1],&qlen);
    if (qlen < 7) {
        error = "Invalid sqlite3 query";
        bwa_result_string(ctx, error, strlen(error));
        goto FAIL;
    }

    if (sqlite3_prepare(db,query,qlen,&stmt,NULL) != SQLITE_OK) {
        error = sqlite3_errmsg(db);
        bwa_result_string(ctx, error, strlen(error));
        goto FAIL;
    }

    done = 0;
    retry = 5;
    row = 0;
    count = 0;
    result = bwa_context_new_array(ctx);
    val = bwa_context_new_scalar(ctx);
    while (!done) {
        switch (sqlite3_step(stmt)) {
            case SQLITE_BUSY:
                if (--retry == 0) {
                    error = sqlite3_errmsg(db);
                    bwa_result_string(ctx, error, strlen(error));
                } else {
                    usleep(5000);
                    continue;
                }
                break;
            case SQLITE_DONE:
                done = 1;
                break;
            case SQLITE_ROW:
                arr = bwa_context_new_array(ctx);
                count = sqlite3_column_count(stmt);

                if (count == 0) {
                    bwa_array_add_intkey_elem(result,0,arr);
                    done = 1;
                    break;
                }

                for (i = 0; i < count; i++) {
                    switch (sqlite3_column_type(stmt,i)) {
                        case SQLITE_INTEGER:
                            bwa_value_int64(val,sqlite3_column_int64(stmt,i));
                            break;
                        case SQLITE_FLOAT:
                            bwa_value_double(val,sqlite3_column_double(stmt,i));
                            break;
                        case SQLITE_TEXT:
                            bwa_value_string(val,sqlite3_column_text(stmt,i),-1);
                            break;
                        default: // note: blobs are not handled
                            bwa_value_null(val);
                    }
                    bwa_array_add_strkey_elem(arr,sqlite3_column_name(stmt,i),val);
                }
                bwa_array_add_intkey_elem(result,row++,arr);
                break;

            default:
                error = sqlite3_errmsg(db);
                bwa_result_string(ctx, error, strlen(error));

            bwa_value_reset_string_cursor(val);
        }
    }

    sqlite3_finalize(stmt);
    bwa_result_value(ctx,result);
    return BWA_OK;

FAIL:
    if (stmt != NULL) sqlite3_finalize(stmt);
    return BWA_OK;
}
