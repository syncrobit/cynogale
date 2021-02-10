bool file_exists (const char *filename) {
    struct stat   buffer;
    return (stat (filename, &buffer) == 0);
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

