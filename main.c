/*
 * Extremely minimal civetweb(formerly mongoose) + BWA scripted web server for embedded use,
 * or just a lightweight alternative to your usual PHP and Apache, nginx, lighttpd.
 * Serves up files and BWA scripts from the directory run.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>   // stat
#include <stdbool.h>    // bool type
#include <pthread.h>
#include "config.h"
#include "utils.c"
#include "civetweb.h"
#include "bwa.h"

#include "extensions/sqlite3.inl"

static struct mg_context *ctx; /* Set by start_civetweb() */
static bwa *bEngine; /* BWA engine */
static bwa_vm *bVm;  /* Compiled BWA program */
int exit_flag = 0;

// Display an error message and exit.
static void Fatal(const char *zMsg){
    puts(zMsg);
    bwa_lib_shutdown();
    mg_stop(ctx);
    exit(0);
}

#define STDOUT_FILENO   1
/*
 * VM output consumer callback.
 * Each time the virtual machine generates some outputs,the following
 * function gets called by the underlying virtual machine to consume
 * the generated output.
 * All this function does is redirecting the VM output to STDOUT.
 * This function is registered later via a call to bwa_vm_config()
 * with a configuration verb set to: BWA_VM_CONFIG_OUTPUT.
 */
static int Output_Consumer(
        const void *pOutput,
        unsigned int nOutputLen,
        void *pUserData // Unused
){

    ssize_t nWr;
    nWr = printf("%.*s", nOutputLen, (char*)pOutput);

    if( nWr < 0 ){
        return BWA_ABORT;
    }

    return BWA_OK;
}

int index_handler(struct mg_connection *conn, void *cbdata){
    mg_printf(conn,"HTTP/1.1 303 See Other\r\n"
                   "Location: /"INDEX_FILE"\r\n\r\n");

    return 1;
}
int bwa_script(struct mg_connection *conn, void *cbdata){

    int rc;
    const void *pOut;
    unsigned int nLen;
    char *buff = NULL;
    int c = 0,i = 1;

    /* Handler may access the request info using mg_get_request_info */
    const struct mg_request_info * req_info = mg_get_request_info(conn);
    const char * path = concat(HOME, req_info->local_uri);

    if (!file_exists(path)){
        mg_send_http_error(conn, 404, "Not Found");
        return 1;
    }

    buff = (char *)malloc(req_info->length);
    memcpy(buff,req_info->buffer, req_info->length);

    for (i = 0; i < req_info->length - 1; i++){
        if (buff[i] == '\0'){
            if (buff[i+1] != '\0'){
                if (c == 1 && buff[i + 1] != 'H'){
                    buff[i] = '?';
                }else{
                    buff[i] = ' ';
                }
                c++;
            }
        }
    }

    /* Now,it's time to compile our PHP file */
    rc = bwa_compile_file(
            bEngine, /* BWA Engine */
            path, /* Path to the BWA file to compile */
            &bVm,    /* OUT: Compiled BWA program */
            0        /* IN: Compile flags */
    );

    if( rc != BWA_OK ){ /* Compile error */
        if( rc == BWA_IO_ERR ){
            Fatal("IO Error");
        }else if( rc == BWA_VM_ERR ){
            Fatal("VM initialization error");
        }else{
            /* Compile-time error, your output (STDOUT) should display the error messages */
            Fatal("Compile error");
        }
    }

    rc = bwa_vm_config(bVm, BWA_VM_CONFIG_HTTP_REQUEST, buff, req_info->length);
    if( rc != BWA_OK ){
        mg_send_http_error( conn, 500, "Error while transferring the HTTP request to BWA.");
        bwa_lib_shutdown();
        return 1;
    }

    //SQLite Functions
    rc = bwa_create_function(bVm,"sqlite_query", bwa_sqlite_query,NULL);
    if( rc != BWA_OK ){
        mg_send_http_error( conn, 500, "sqlite_query function failed");
        bwa_lib_shutdown();
        return 1;
    }

    rc = bwa_vm_config(bVm, BWA_VM_CONFIG_IMPORT_PATH, HOME);
    if( rc != BWA_OK ){
        mg_send_http_error( conn, 500, "Cannot get Import Path");
        bwa_lib_shutdown();
        return 1;
    }

    rc = bwa_vm_exec(bVm,0);
    if( rc != BWA_OK ){
        Fatal("VM execution error");
    }

    /*
     * Now we have our script compiled,it's time to configure our VM.
     * We will install the VM output consumer callback defined above
     * so that we can consume the VM output and redirect it to STDOUT.
     */

    /* Extract the output */
    rc = bwa_vm_config(bVm,
                       BWA_VM_CONFIG_EXTRACT_OUTPUT,
                       &pOut,
                       &nLen
    );

    if(nLen > 0){
        mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
        mg_write(conn, pOut, nLen);
    }

#if REPORT_ERRORS+1
    /* Report script run-time errors */
    bwa_vm_config(bVm,BWA_VM_CONFIG_ERR_REPORT);
#endif

    bwa_vm_reset(bVm);

    return 1;
}

int bwa_script_json(struct mg_connection *conn, void *cbdata){
    int rc;
    const void *pOut;
    unsigned int nLen;
    char *buff = NULL;
    int c = 0,i = 1;

    /* Handler may access the request info using mg_get_request_info */
    const struct mg_request_info * req_info = mg_get_request_info(conn);
    const char * path = concat(HOME, req_info->local_uri);

    if (!file_exists(path)){
        mg_send_http_error(conn, 404, "Not Found");
        return 1;
    }

    buff = (char *)malloc(req_info->length);
    memcpy(buff,req_info->buffer, req_info->length);

    for (i = 0; i < req_info->length - 1; i++){
        if (buff[i] == '\0'){
            if (buff[i+1] != '\0'){
                if (c == 1 && buff[i + 1] != 'H'){
                    buff[i] = '?';
                }else{
                    buff[i] = ' ';
                }
                c++;
            }
        }
    }

    /* Now,it's time to compile our PHP file */
    rc = bwa_compile_file(
            bEngine, /* BWA Engine */
            path, /* Path to the BWA file to compile */
            &bVm,    /* OUT: Compiled BWA program */
            0        /* IN: Compile flags */
    );

    if( rc != BWA_OK ){ /* Compile error */
        if( rc == BWA_IO_ERR ){
            Fatal("IO Error");
        }else if( rc == BWA_VM_ERR ){
            Fatal("VM initialization error");
        }else{
            /* Compile-time error, your output (STDOUT) should display the error messages */
            Fatal("Compile error");
        }
    }

    rc = bwa_vm_config(bVm, BWA_VM_CONFIG_HTTP_REQUEST, buff, req_info->length);
    if( rc != BWA_OK ){
        mg_send_http_error( conn, 500, "Error while transferring the HTTP request to BWA.");
        bwa_lib_shutdown();
        return 1;
    }

    //SQLite Functions
    rc = bwa_create_function(bVm,"sqlite_query", bwa_sqlite_query,NULL);
    if( rc != BWA_OK ){
        mg_send_http_error( conn, 500, "sqlite_query function failed");
        bwa_lib_shutdown();
        return 1;
    }

    rc = bwa_vm_config(bVm, BWA_VM_CONFIG_IMPORT_PATH, HOME);
    if( rc != BWA_OK ){
        mg_send_http_error( conn, 500, "Cannot get Import Path");
        bwa_lib_shutdown();
        return 1;
    }

    rc = bwa_vm_exec(bVm,0);
    if( rc != BWA_OK ){
        Fatal("VM execution error");
    }

    /*
     * Now we have our script compiled,it's time to configure our VM.
     * We will install the VM output consumer callback defined above
     * so that we can consume the VM output and redirect it to STDOUT.
     */

    /* Extract the output */
    rc = bwa_vm_config(bVm,
                       BWA_VM_CONFIG_EXTRACT_OUTPUT,
                       &pOut,
                       &nLen
    );

    if(nLen > 0){
        mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
        mg_write(conn, pOut, nLen);
    }

#if REPORT_ERRORS+1
    /* Report script run-time errors */
    bwa_vm_config(bVm,BWA_VM_CONFIG_ERR_REPORT);
#endif

    bwa_vm_reset(bVm);

    return 1;
}


int main(int argc, char **argv){

    int rc;
    const char * options[] = {
            "document_root", HOME,
            "listening_ports", PORT,
            "index_files", INDEX_FILE,
            "error_log_file", concat(ERROR_PATH, "/error.log"),
            0
    };

    /* Allocate a new BWA engine instance */
    rc = bwa_init(&bEngine);
    if( rc != BWA_OK ){
        Fatal("Error while allocating a new BWA engine instance");
    }
    /* Set an error log consumer callback. This callback [Output_Consumer()] will
     * redirect all compile-time error messages to STDOUT.
     */
    bwa_config(bEngine,BWA_CONFIG_ERR_OUTPUT,
               Output_Consumer, // Error log consumer
               0 // NULL: Callback Private data
    );

    // Start the web server.
    ctx = mg_start(NULL, NULL, options);

    mg_set_request_handler(ctx, "**.bwa$", bwa_script, 0);
    mg_set_request_handler(ctx, "**.jbwa$", bwa_script_json, 0);
    mg_set_request_handler(ctx, "/$", index_handler, 0);

    printf("Cyanogale started on port(s) %s\n",
           mg_get_option(ctx, "listening_ports"));

    while (exit_flag == 0) {
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
    }

    bwa_vm_release(bVm);
    bwa_release(bEngine);
    mg_stop(ctx);

    return 0;
}
