#include <curl/curl.h>

struct curl_result_data {
  char *memory;
  size_t size;
};
 
static size_t
write_result_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    const char *error;
    size_t realsize = size * nmemb;
    struct curl_result_data *mem = (struct curl_result_data *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        // case treated by curl_easy_strerror
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

static int bwa_curl_get(bwa_context *ctx, int argc, bwa_value **argv) {
    CURL *curl_handle = NULL;
    struct curl_slist *list = NULL;

    const char *error, *url, *header_option;
    int i, len;
    bwa_value *result = NULL;
    struct curl_result_data curl_res;

    if (argc < 1) {
        error = "Missing argument";
        bwa_result_string(ctx, error, strlen(error));
        return BWA_OK;
    }

    for (i = 0; i < argc; ++i) {
        if (!bwa_value_is_string(argv[i])) {
            error = "Invalid argument(s) (all should be string)";
            bwa_result_string(ctx, error, strlen(error));
            return BWA_OK;
        }
    }

    url = bwa_value_to_string(argv[0], &len);
    for (int i = 1; i < argc; ++i) {
        list = curl_slist_append(list, bwa_value_to_string(argv[i], &len));
    }
    
    curl_handle = curl_easy_init();
    if (curl_handle == NULL) {
        error = "Could not create CURL handle";
        bwa_result_string(ctx, error, strlen(error));
        return BWA_OK;
    }

    // Init curl result value
    curl_res.memory = malloc(1); /* will be grown as needed by the realloc above */ 
    curl_res.size = 0; /* no data at this point */ 

    // Create GET request
    CURLcode res;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_result_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&curl_res);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, list);
    res = curl_easy_perform(curl_handle);

    if (res != CURLE_OK) {
        error = curl_easy_strerror(res);
        bwa_result_string(ctx, error, strlen(error));
        return BWA_OK;
    }

    // Set result data
    result = bwa_context_new_scalar(ctx);
    bwa_value_string(result, curl_res.memory, curl_res.size);
    bwa_result_value(ctx,result);

    // Cleanup
    free(curl_res.memory);
    curl_easy_cleanup(curl_handle);
	
	return BWA_OK;
}