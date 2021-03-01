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

    const char *error, *url;
    int i, len;
    struct curl_result_data curl_res;

    if (argc < 1) {
        error = "Missing argument(s)";
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
    for (i = 1; i < argc; ++i) {
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

    // Set result data
    if (res == CURLE_OK) {
        bwa_result_string(ctx, curl_res.memory, curl_res.size);
    }
    else
    {
        // Something went wrong
        error = curl_easy_strerror(res);
        bwa_result_string(ctx, error, strlen(error));
    }

    // Cleanup
    free(curl_res.memory);
    curl_slist_free_all(list);
    curl_easy_cleanup(curl_handle);
	
    return BWA_OK;
}

static int bwa_curl_post(bwa_context *ctx, int argc, bwa_value **argv) {
    CURL *curl_handle = NULL;
    const char *error, *url, *postfields;
    int len;
    struct curl_result_data curl_res;

    if (argc < 2) {
        error = "Missing argument(s)";
        bwa_result_string(ctx, error, strlen(error));
        return BWA_OK;
    }

    if (!bwa_value_is_string(argv[0]) || !bwa_value_is_string(argv[1])) {
        error = "Invalid argument(s) (want url, postfields)";
        bwa_result_string(ctx, error, strlen(error));
        return BWA_OK;
    }

    url = bwa_value_to_string(argv[0], &len);
    postfields = bwa_value_to_string(argv[1], &len);
    
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
    curl_easy_setopt(curl_handle, CURLOPT_COPYPOSTFIELDS, postfields);
    res = curl_easy_perform(curl_handle);

    // Set result data
    if (res == CURLE_OK) {
        bwa_result_string(ctx, curl_res.memory, curl_res.size);
    }
    else
    {
        // Something went wrong
        error = curl_easy_strerror(res);
        bwa_result_string(ctx, error, strlen(error));
    }

    // Cleanup
    free(curl_res.memory);
    curl_easy_cleanup(curl_handle);
	
	return BWA_OK;
}