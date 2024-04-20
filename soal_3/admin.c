/*

__________                             .__  __     ____   _____  
\______   \__ __  ____ ___.__._____    |__|/  |_  /_   | /  |  | 
 |     ___/  |  \/    <   |  |\__  \   |  \   __\  |   |/   |  |_
 |    |   |  |  /   |  \___  | / __ \_ |  ||  |    |   /    ^   /
 |____|   |____/|___|  / ____|(____  / |__||__|    |___\____   | 
                     \/\/          \/                       |__| 
*/

include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#define LOG_FILE_EXTENSION ".log"
#define MAX_FILENAME_LENGTH 256
#define MAX_COMMAND_LENGTH 512

int monitor_user(const char *user);
void stop_monitoring(const char *user);
void block_processes(const char *user);
void unblock_processes(const char *user);
void log_event(const char *user, const char *pid, const char *process, const char *status);
void signal_handler(int signum);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s [-m/-s/-c/-a] <user>\n", argv[0]);
        return 1;
    }

    char *user = argv[2];

    if (strcmp(argv[1], "-m") == 0) {
        // Start monitoring
        if (monitor_user(user) == -1) {
            perror("Failed to start monitoring");
            return 1;
        }
    } else if (strcmp(argv[1], "-s") == 0) {
        // Stop monitoring
        stop_monitoring(user);
    } else if (strcmp(argv[1], "-c") == 0) {
        // Block processes
        block_processes(user);
    } else if (strcmp(argv[1], "-a") == 0) {
        // Unblock processes
        unblock_processes(user);
    } else {
        printf("Invalid option\n");
        return 1;
    }

    return 0;
}

int monitor_user(const char *user) {
    // Forking to create a daemon process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    } else if (pid > 0) {
        // Parent process
        printf("Monitoring started for user: %s\n", user);
        return 0;
    }

    // Child process (daemon)
    umask(0);
    setsid();

    // Register signal handler for SIGTERM
    signal(SIGTERM, signal_handler);

    // Monitoring loop
    char log_filename[MAX_FILENAME_LENGTH];
    snprintf(log_filename, MAX_FILENAME_LENGTH, "%s%s", user, LOG_FILE_EXTENSION);
    int log_fd = open(log_filename, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (log_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char command[MAX_COMMAND_LENGTH];
    while (1) {
        snprintf(command, MAX_COMMAND_LENGTH, "ps -u %s", user);
        FILE *ps_output = popen(command, "r");
        if (ps_output == NULL) {
            perror("popen");
            exit(EXIT_FAILURE);
        }

        char line[MAX_COMMAND_LENGTH];
        while (fgets(line, sizeof(line), ps_output)) {
            if (strstr(line, "PID") != NULL) continue; // Skip header line
            char *pid = strtok(line, " ");
            char *process = strtok(NULL, "\n");
            if (process != NULL) {
                // Log the process
                log_event(user, pid, process, "JALAN");
            }
        }

        pclose(ps_output);
        sleep(1); // Monitoring interval
    }

    close(log_fd);
    return 0;
}

void stop_monitoring(const char *user) {
    // Find the PID of the monitoring process and send SIGTERM to it
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, MAX_COMMAND_LENGTH, "ps -ef | grep './admin -m %s' | grep -v grep | awk '{print $2}'", user);
    FILE *pid_output = popen(command, "r");
    if (pid_output == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    char pid_str[16];
    if (fgets(pid_str, sizeof(pid_str), pid_output) != NULL) {
        pid_t pid = atoi(pid_str);
        if (pid > 0) {
            kill(pid, SIGTERM);
            printf("Monitoring stopped for user: %s\n", user);
        }
    }

    pclose(pid_output);
}

void block_processes(const char *user) {
    // Implement actual process blocking logic here
    // For the sake of simplicity, let's just log that processes are blocked
    printf("Blocking processes for user: %s\n", user);

    // Log the blocking action
    log_event(user, "-", "Blocked processes", "GAGAL");
}

void unblock_processes(const char *user) {
    // Implement process unblocking
    // For the sake of simplicity, let's say it's a placeholder
    printf("Unblocking processes for user: %s\n", user);
}

void log_event(const char *user, const char *pid, const char *process, const char *status) {
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);

    char log_filename[MAX_FILENAME_LENGTH];
    snprintf(log_filename, MAX_FILENAME_LENGTH, "%s%s", user, LOG_FILE_EXTENSION);

    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "[%d:%m:%Y]-[%H:%M:%S]", local_time);

    int log_fd = open(log_filename, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (log_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char log_entry[MAX_COMMAND_LENGTH];
    snprintf(log_entry, MAX_COMMAND_LENGTH, "%s-%s-%s-%s_%s\n", timestamp, pid, process, user, status);
    write(log_fd, log_entry, strlen(log_entry));

    close(log_fd);
}

void signal_handler(int signum) {
    // Signal handler for SIGTERM
    if (signum == SIGTERM) {
        printf("Received SIGTERM signal. Exiting...\n");
        exit(EXIT_SUCCESS);
    }
}
