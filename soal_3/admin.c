/*

__________                             .__  __     ____   _____  
\______   \__ __  ____ ___.__._____    |__|/  |_  /_   | /  |  | 
 |     ___/  |  \/    <   |  |\__  \   |  \   __\  |   |/   |  |_
 |    |   |  |  /   |  \___  | / __ \_ |  ||  |    |   /    ^   /
 |____|   |____/|___|  / ____|(____  / |__||__|    |___\____   | 
                     \/\/          \/                       |__| 
*/

//library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>

#define LOG_FILE_EXTENSION ".log"
#define MAX_USERNAME_LENGTH 100
#define MAX_LOG_ENTRY_LENGTH 200
#define MAX_COMMAND_LENGTH 200 

// Fungsi untuk mendapatkan waktu saat ini
char* get_current_time() {
    time_t rawtime;
    struct tm *info;
    char *buffer = (char*)malloc(sizeof(char) * 30);

    time(&rawtime);
    info = localtime(&rawtime);
    strftime(buffer, 30, "%d:%m:%Y-%H:%M:%S", info);

    return buffer;
}

// Fungsi untuk mendapatkan nama proses berdasarkan PID
char* proc_name(char *pid) {
    char proc_path[20];
    FILE *fp;
    static char proc_name[100];

    snprintf(proc_path, sizeof(proc_path), "/proc/%s/comm", pid);
    fp = fopen(proc_path, "r");
    if (fp != NULL) {
        fscanf(fp, "%s", proc_name);
        fclose(fp);
    } else {
        strcpy(proc_name, "?");
    }

    return proc_name;
}

// Fungsi untuk melakukan logging event(mencatat proses)
void log_event(char *username, char *pid, char *process_name, int success) {
    char filename[MAX_USERNAME_LENGTH + strlen(LOG_FILE_EXTENSION) + 1];
    snprintf(filename, sizeof(filename), "%s%s", username, LOG_FILE_EXTENSION);

    FILE *file = fopen(filename, "a");
    if (file != NULL) {
        char *status = success ? "JALAN" : "GAGAL";
        char *time_str = get_current_time();
        char *proc = proc_name(pid);
        char day_month_year[12], hour_minute_second[9];
        sscanf(time_str, "%10[^-]-%9s", day_month_year, hour_minute_second);
        fprintf(file, "[%s]-[%s]-%s_%s_%s_%s\n", day_month_year, hour_minute_second, pid, proc, "process", status);
        free(time_str);
        fclose(file);
    }
}

// Fungsi untuk memonitor proses yang dilakukan oleh user
void monitor_processes(char *username) {
    char command[MAX_COMMAND_LENGTH + MAX_USERNAME_LENGTH + 3]; // "-u username\0"
    snprintf(command, sizeof(command), "ps -u %s", username);

    while (1) {
        FILE *ps_output = popen(command, "r");
        if (ps_output != NULL) {
            char line[256];
            fgets(line, sizeof(line), ps_output); // Skip header line
            while (fgets(line, sizeof(line), ps_output) != NULL) {
                char pid[10], process_name[100];
                sscanf(line, "%s %s", pid, process_name);
                log_event(username, pid, process_name, 1);
            }
            pclose(ps_output);
        }
        sleep(1);
    }
}

// Fungsi untuk menghentikan pemantauan proses
void stop_monitoring(const char *username) {
    char command[MAX_COMMAND_LENGTH]; // Perbesar ukuran buffer
    snprintf(command, sizeof(command), "pkill -f './admin -m %s'", username);
    int result = system(command);
    if (result == -1) {
        perror("Error Gagal menghentikan");
    } else {
        printf("Monitoring berhasil dihentikan untuk user: %s\n", username);
    }
}

// Fungsi untuk menangani perintah yang diberikan kepada program
void handle_command(char *username, char *option) {
    char filename[MAX_USERNAME_LENGTH + strlen(LOG_FILE_EXTENSION) + 1];
    snprintf(filename, sizeof(filename), "%s%s", username, LOG_FILE_EXTENSION);

    static pid_t monitor_pid = -1; // Variabel untuk menyimpan PID proses monitor

    if (strncmp(option, "-m", 2) == 0) {
        printf("Monitoring berhasil dimulai untuk user: %s\n", username);
        if (monitor_pid == -1) {
            monitor_pid = fork();
            if (monitor_pid == 0) {
                monitor_processes(username);
            } else if (monitor_pid < 0) {
                perror("Error forking");
            }
        } else {
            printf("Monitoring telah dilakukan untuk user: %s\n", username);
        }
    } else if (strncmp(option, "-s", 2) == 0) {
        stop_monitoring(username);
        if (remove(filename) != 0) {
            perror("Error removing log file");
        } else {
            printf("Log file removed for user: %s\n", username);
        }
    } else if (strncmp(option, "-c", 2) == 0) {
        log_event(username, "N/A", "Proses digagalkan", 0);
        printf("Proses berhasil digagalkan untuk user: %s\n", username);
    } else if (strncmp(option, "-a", 2) == 0) {
        printf("Berhasil menghentikan proses penggagalan untuk user: %s\n", username);
        if (monitor_pid != -1) {
            if (kill(monitor_pid, SIGKILL) == -1) {
                perror("Error! Gagal menghentikan penggagalan proses");
            } else {
                printf("Berhasil menghentikan penggagalan user: %s\n", username);
            }
            monitor_pid = -1;
        }
    } else {
        printf("Opsi tidak valid: %s\n", option);
    }
}

// Fungsi utama program
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <option> <username>\n", argv[0]);
        return 1;
    }

    char *option = argv[1];
    char *username = argv[2];
    handle_command(username, option);

    return 0;
}

