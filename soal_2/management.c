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
#include <sys/wait.h>
#include <dirent.h>
#include <signal.h>
#include <time.h>
#include <pwd.h>

void log_action(char *username, char *filename, char *action) {
    FILE *f = fopen("/home/kali/Sisop/soal_2/history.log", "a+");
    if (f == NULL) {
        printf("Error opening file!\n");
        return;
    }
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(f, "[%s][%02d:%02d:%02d] - %s - %s\n", username, tm.tm_hour, tm.tm_min, tm.tm_sec, filename, action);
    fclose(f);
}

char rot19(char c) {
    if ('a' <= c && c <= 'z') {
        return 'a' + (c - 'a' + 7) % 26;
    } else if ('A' <= c && c <= 'Z') {
        return 'A' + (c - 'A' + 7) % 26;
    } else {
        return c;
    }
}

void rename_files()
{
    DIR *d;
    struct dirent *dir;
    char old_path[1024], new_path[1024];
    char new_filename[1024];

    d = opendir("/home/kali/Sisop/soal_2/library");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            // Check if the first character of the filename is a number between 1 and 6
            if (dir->d_name[0] >= '1' && dir->d_name[0] <= '6')
            {
                continue; // Skip this file
            }

            sprintf(old_path, "/home/kali/Sisop/soal_2/library/%s", dir->d_name);
            for(int i = 0; dir->d_name[i]; i++){
                new_filename[i] = rot19(dir->d_name[i]);
            }
            sprintf(new_path, "/home/kali/Sisop/soal_2/library/%s", new_filename);
            rename(old_path, new_path);

            log_action(getpwuid(getuid())->pw_name, dir->d_name, "Successfully renamed.");
        }
        closedir(d);
    }
}


void run_exec(char *path, char *argv[])
{
    pid_t child_id;
    int status;

    child_id = fork();

    if (child_id == 0)
    {
        execv(path, argv);
    }
    else
    {
        ((void)wait(&status));
    }
}

void download(char filename[], char url[])
{
    char *argv[] = {"wget", url, "-O", filename, "-q", NULL};
    run_exec("/usr/bin/wget", argv);
}

void extract_zip(char filename[], char output[])
{
    char *arg[] = {"unzip", "-q", filename, "-d", output, NULL};
    run_exec("/usr/bin/unzip", arg);
    remove(filename);
    log_action(getpwuid(getuid())->pw_name, filename, "Successfully extracted and deleted.");
}

void filtering() {
    DIR *d;
    struct dirent *dir;
    char old_path[1024], new_path[1024];
    char *ext;

    d = opendir("/home/kali/Sisop/soal_2/library");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            sprintf(old_path, "/home/kali/Sisop/soal_2/library/%s", dir->d_name);
            if (strstr(dir->d_name, "d3Let3")) {
                remove(old_path);
                log_action(getpwuid(getuid())->pw_name, dir->d_name, "Successfully deleted.");
            } else if (strstr(dir->d_name, "r3N4mE")) {
                ext = strrchr(dir->d_name, '.');
                if (ext != NULL) {
                    if (strcmp(ext, ".ts") == 0) {
                        sprintf(new_path, "/home/kali/Sisop/soal_2/library/helper.ts");
                    } else if (strcmp(ext, ".py") == 0) {
                        sprintf(new_path, "/home/kali/Sisop/soal_2/library/calculator.py");
                    } else if (strcmp(ext, ".go") == 0) {
                        sprintf(new_path, "/home/kali/Sisop/soal_2/library/server.go");
                    } else {
                        sprintf(new_path, "/home/kali/Sisop/soal_2/library/renamed.file");
                    }
                    rename(old_path, new_path);
                    log_action(getpwuid(getuid())->pw_name, dir->d_name, "Successfully renamed.");
                }
            }
        }
        closedir(d);
    }
}

void create_backup_folder() {
    mkdir("/home/kali/Sisop/soal_2/library/backup", 0777);
}

void move_files(char *source_folder, char *dest_folder, char *action) {
    DIR *d;
    struct dirent *dir;
    char old_path[1024], new_path[1024];

    d = opendir(source_folder);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strstr(dir->d_name, "m0V3")) {
                sprintf(old_path, "%s/%s", source_folder, dir->d_name);
                sprintf(new_path, "%s/%s", dest_folder, dir->d_name);
                rename(old_path, new_path);
                log_action(getpwuid(getuid())->pw_name, dir->d_name, action);
            }
        }
        closedir(d);
    }
}

int mode = 0; // 0 for default, 1 for backup, 2 for restore

void handle_signal(int sig) {
    if (sig == SIGRTMIN) {
        mode = 0;
    } else if (sig == SIGUSR1) {
        mode = 1;
    } else if (sig == SIGUSR2) {
        mode = 2;
    }
}

volatile sig_atomic_t stop;

void inthand(int signum) {
    stop = 1;
}


int main(int argc, char *argv[]) {
  
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

signal(SIGRTMIN, handle_signal);
signal(SIGUSR1, handle_signal);
signal(SIGUSR2, handle_signal);
signal(SIGTERM, inthand);

// Create the log file at the start
  FILE *f = fopen("/home/kali/Sisop/soal_2/history.log", "w");
  fclose(f);

if (argc == 3 && strcmp(argv[1], "-m") == 0) {
        if (strcmp(argv[2], "backup") == 0) {
        mode = 1;
        } else if (strcmp(argv[2], "restore") == 0) {
        mode = 2;
    }
}

  while (!stop) {
    if (mode == 0) {
            download("/home/kali/Sisop/soal_2/library.zip", "https://drive.google.com/uc?export=download&id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup");
            extract_zip("/home/kali/Sisop/soal_2/library.zip", "/home/kali/Sisop/soal_2");
            log_action(getpwuid(getuid())->pw_name, "library.zip", "Successfully downloaded and extracted.");
            rename_files();
            filtering();
            create_backup_folder();
        } else if (mode == 1) {
            move_files("/home/kali/Sisop/soal_2/library", "/home/kali/Sisop/soal_2/library/backup", "Successfully moved to backup.");
        } else if (mode == 2) {
            move_files("/home/kali/Sisop/soal_2/library/backup", "/home/kali/Sisop/soal_2/library", "Successfully restored from backup.");
        }
    sleep(1);
  }
}
