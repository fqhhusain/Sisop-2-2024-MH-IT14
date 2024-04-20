/*
__________                             .__  __     ____   _____  
\______   \__ __  ____ ___.__._____    |__|/  |_  /_   | /  |  | 
 |     ___/  |  \/    <   |  |\__  \   |  \   __\  |   |/   |  |_
 |    |   |  |  /   |  \___  | / __ \_ |  ||  |    |   /    ^   /
 |____|   |____/|___|  / ____|(____  / |__||__|    |___\____   | 
                     \/\/          \/                       |__| 
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

#define SUSPICIOUS_STRINGS_COUNT 3
#define LOG_FILE "/home/kali/Sisop/soal_1/virus.log"

const char *SUSPICIOUS_STRINGS[] = {"m4LwAr3", "5pYw4R3", "R4nS0mWaR3"};
const char *REPLACEMENT_STRINGS[] = {"[MALWARE]", "[SPYWARE]", "[RANSOMWARE]"};

void replace_string(const char *filename, const char *string_to_replace, const char *replacement_string) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char *contents = NULL;
    long file_size;
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    contents = (char *)malloc(file_size + 1);
    if (contents == NULL) {
        fclose(file);
        perror("Memory allocation failed");
        return;
    }

    fread(contents, 1, file_size, file);
    contents[file_size] = '\0';

    char *found = strstr(contents, string_to_replace);
    while (found != NULL) {
        strncpy(found, replacement_string, strlen(replacement_string));
        found = strstr(found + strlen(replacement_string), string_to_replace);
    }

    rewind(file);
    fwrite(contents, 1, file_size, file);

    free(contents);
    fclose(file);

    printf("Suspicious string at %s successfully replaced!\n", filename);
}

void write_log(const char *filename, const char *message) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }

    time_t now;
    struct tm *local_time;
    char timestamp[20];

    time(&now);
    local_time = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%d-%m-%Y %H:%M:%S", local_time);

    fprintf(log_file, "[%s] Suspicious string at %s successfully replaced! %s\n", timestamp, filename, message);
    fclose(log_file);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <folder_path>\n", argv[0]);
        return 1;
    }

    const char *folder_path = argv[1];

    pid_t pid, sid;
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        for (int i = 0; i < SUSPICIOUS_STRINGS_COUNT; i++) {
            replace_string(argv[1], SUSPICIOUS_STRINGS[i], REPLACEMENT_STRINGS[i]);
            write_log(argv[1], "Suspicious string replaced in files");
        }
        sleep(15);
    }

    exit(EXIT_SUCCESS);
}
