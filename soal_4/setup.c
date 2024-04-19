/*

__________                             .__  __     ____   _____  
\______   \__ __  ____ ___.__._____    |__|/  |_  /_   | /  |  | 
 |     ___/  |  \/    <   |  |\__  \   |  \   __\  |   |/   |  |_
 |    |   |  |  /   |  \___  | / __ \_ |  ||  |    |   /    ^   /
 |____|   |____/|___|  / ____|(____  / |__||__|    |___\____   | 
                     \/\/          \/                       |__| 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void open_windows(char* app, int num, FILE* pid_file) {
    for (int i = 0; i < num; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            execlp(app, app, NULL);
            perror("Error executing program");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("Error forking process");
            exit(EXIT_FAILURE);
        } else {
            // Parent process: write the child's PID to the file
            fprintf(pid_file, "%d\n", pid);
        }
    }
}

void read_config_file(const char* file_path, char* apps[], int* num_apps) {
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char app[50];
    int num;
    int i = 0;
    while (fscanf(file, "%s %d", app, &num) == 2) {
        for (int j = 0; j < num; j++) {
            apps[i] = strdup(app);
            i++;
        }
    }
    *num_apps = i;

    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("  ./setup -o <app1> <num1> <app2> <num2> ... <appN> <numN>\n");
        printf("  ./setup -f <config_file>\n");
        printf("  ./setup -k <config_file>\n");
        printf("  ./setup -k\n");
        return EXIT_FAILURE;
    }

    char* option = argv[1];

    FILE* pid_file;
    if (strcmp(option, "-o") == 0 || strcmp(option, "-f") == 0) {
        pid_file = fopen("pids.txt", "w");
        if (pid_file == NULL) {
            perror("Error opening PID file");
            exit(EXIT_FAILURE);
        }
    }

    if (strcmp(option, "-o") == 0) {
        if (argc % 2 != 0) {
            printf("Invalid number of arguments. Usage: ./setup -o <app1> <num1> <app2> <num2> ... <appN> <numN>\n");
            return EXIT_FAILURE;
        }
        for (int i = 2; i < argc; i += 2) {
            char* app = argv[i];
            int num = atoi(argv[i + 1]);
            open_windows(app, num, pid_file);
        }
    } else if (strcmp(option, "-f") == 0) {
        if (argc != 3) {
            printf("Usage: ./setup -f <config_file>\n");
            return EXIT_FAILURE;
        }
        char* config_file = argv[2];
        int num_apps;
        char* apps[100];
        read_config_file(config_file, apps, &num_apps);
        for (int i = 0; i < num_apps; i++) {
            open_windows(apps[i], 1, pid_file);
        }
    } else if (strcmp(option, "-k") == 0) {
        if (argc == 2) {
            // New code: kill all processes listed in the PID file
            pid_file = fopen("pids.txt", "r");
            if (pid_file == NULL) {
                perror("Error opening PID file");
                exit(EXIT_FAILURE);
            }
            int pid;
            while (fscanf(pid_file, "%d", &pid) == 1) {
                kill(pid, SIGTERM);
            }
            fclose(pid_file);
        } else if (argc == 3) {
            char* config_file = argv[2];
            int num_apps;
            char* apps[100];
            read_config_file(config_file, apps, &num_apps);
            for (int i = 0; i < num_apps; i++) {
                pid_t pid = fork();
                if (pid == 0) {
                    // Child process
                    execlp("pkill", "pkill", apps[i], NULL);
                    perror("Error executing pkill");
                    exit(EXIT_FAILURE);
                } else if (pid < 0) {
                    perror("Error forking process");
                    exit(EXIT_FAILURE);
                }
            }
        } else {
            printf("Usage: ./setup -k <config_file> or ./setup -k\n");
            return EXIT_FAILURE;
        }
    } else {
        printf("Invalid option. Use -o, -f, or -k.\n");
        return EXIT_FAILURE;
    }

    if (strcmp(option, "-o") == 0 || strcmp(option, "-f") == 0) {
        fclose(pid_file);
    }

    return EXIT_SUCCESS;
}
