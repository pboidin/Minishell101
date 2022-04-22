#include <libc.h>
#include <dirent.h>

/* Print all the dirs starting from <path> [maybe recursive]. */
int print_dirs(const char *path, int recursive)
{
    struct dirent	*de;
    DIR 			*dr;
    size_t			path_len;
	struct stat		fstat;
	char			full_name[_POSIX_PATH_MAX + 1];

    if (!path) /* Check that input params */
        return -1;
    path_len = strlen(path);  
    if (!path || !path_len || (path_len > _POSIX_PATH_MAX))
        return -1;
    dr = opendir(path); /* open directory */
    if (dr == NULL)
        return -1;
    while ((de = readdir(dr)) != NULL)
    {
        /* Calculate full name, check we are in file length limts */
        if ((path_len + strlen(de->d_name) + 1) > _POSIX_PATH_MAX)
            continue;

        strcpy(full_name, path);
        if (full_name[path_len - 1] != '/')
            strcat(full_name, "/");
        strcat(full_name, de->d_name);

        if ((strcmp(de->d_name, ".") == 0) || /* Ignore special directories */
            (strcmp(de->d_name, "..") == 0))
            continue;

        if (stat(full_name, &fstat) < 0) /* Print only if it is really directory */
            continue;
        if (S_ISDIR(fstat.st_mode))
        {
            printf("%s\n", full_name);
            if (recursive)
                print_dirs(full_name, 1);
        }
    }

    /* Finalize resources. */
    (void)closedir(dr);
    return 0;
}

/* We are taking first argument as initial path name. */
int main(int argc, const char* argv[])
{
    if (argc < 2)
        return -1;

    print_dirs(argv[1], 1);
    return 0;
}
