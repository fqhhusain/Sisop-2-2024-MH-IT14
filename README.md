# Sisop-2-2024-MH-IT14
Laporan pengerjaan soal shift modul 2 Praktikum Sistem Operasi 2024 oleh Kelompok IT14
## Praktikan Sistem Operasi Kelompok IT14
1. Tsaldia Hukma Cita          : 5027231036
2. Muhammad Faqih Husain       : 5027231023
3. Muhammad Syahmi Ash Shidqi  : 5027231085

Soal Shift Modul 2

2. Paul adalah seorang mahasiswa semester 4 yang diterima magang di perusahaan XYZ. Pada hari pertama magang, ia diberi tugas oleh atasannya untuk membuat program manajemen file sederhana. Karena kurang terbiasa dengan bahasa C dan environment Linux, ia meminta bantuan kalian untuk mengembangkan program tersebut.

Cara Bermain
```
code . management.c
gcc management.c -o management 
./management
./management -m restore
./management -m backup
kill -SIGRTMIN <pid_program>
kill -SIGUSR1 <pid_program>
kill -SIGUSR2 <pid_program>
kill -TERM pid
```
Code Lengkap
Library yang digunakan
```
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
```
Penjelasan dari setiap fungsi
```

void log_action(char *username, char *filename, char *action) {
    FILE *f = fopen("/home/kali/Sisop/soal_2/history.log", "a");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(f, "[%s][%02d:%02d:%02d] - %s - %s\n", username, tm.tm_hour, tm.tm_min, tm.tm_sec, filename, action);
    fclose(f);
}
```
Melakukan pencatatan setiap proses yang terjadi pada file history.log
```

char rot19(char c) {
    if ('a' <= c && c <= 'z') {
        return 'a' + (c - 'a' + 7) % 26;
    } else if ('A' <= c && c <= 'Z') {
        return 'A' + (c - 'A' + 7) % 26;
    } else {
        return c;
    }
}
```
Melakukan dekripsi ROT19
```

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
```
Mulai file ke 7 dan seterusnya dilakukan rename berdasarkan hasil dekripsi ROT19
```

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
```
Fungsi untuk melakukan command exec
```

void download(char filename[], char url[])
{
    char *argv[] = {"wget", url, "-O", filename, "-q", NULL};
    run_exec("/usr/bin/wget", argv);
}
```
Melakukan download file 
```

void extract_zip(char filename[], char output[])
{
    char *arg[] = {"unzip", "-q", filename, "-d", output, NULL};
    run_exec("/usr/bin/unzip", arg);
    remove(filename);
    log_action(getpwuid(getuid())->pw_name, filename, "Successfully extracted and deleted.");
}
```
Melakukan unzip file
```
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
```
Mengganti nama file berdasarkan filter tertentu
```

void create_backup_folder() {
    mkdir("/home/kali/Sisop/soal_2/library/backup", 0777);
}
```
Membuat folder untuk menyimpan backup
```

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
```
Melakukan pemindahan file untuk fungsi opsi backup dan restore
```

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
```
Melakukan signaling untuk mengubah option dari program
```

volatile sig_atomic_t stop;

void inthand(int signum) {
    stop = 1;
}
```
Melakukan pengamanan untuk menghentikan program
```

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
```

4) Salomo memiliki passion yang sangat dalam di bidang sistem operasi. Saat ini, dia ingin mengotomasi kegiatan-kegiatan yang ia lakukan agar dapat bekerja secara efisien. Bantulah Salomo untuk membuat program yang dapat mengotomasi kegiatan dia!
(NB: Soal di uji coba dan berhasil di sistem yang menggunakan MacOS dan linux yang menggunakan dual boot dan VM. Untuk teman-teman yang menggunakan WSL bisa mengerjakan soal yang lain ya)

a. Salomo ingin dapat membuka berbagai macam aplikasi dengan banyak jendela aplikasi dalam satu command. Namai file program tersebut setup.c

b.Program dapat membuka berbagai macam aplikasi dan banyak jendela aplikasi sekaligus (bisa membuka 1 atau lebih aplikasi dengan 1 atau lebih window (kecuali aplikasi yang tidak bisa dibuka dengan banyak window seperti discord)) dengan menjalankan: 
setup -o <app1> <num1> <app2> <num2>.....<appN> <numN>
Contoh penggunaannya adalah sebagai berikut: 
setup -o firefox 2 wireshark 2
Program akan membuka 2 jendela aplikasi firefox dan 2 jendela aplikasi wireshark.

c. Program juga dapat membuka aplikasi dengan menggunakan file konfigurasi dengan menggunakan 
setup -f file.conf 
Format file konfigurasi dibebaskan, namun pastikan dapat menjalankan fitur dari poin 2.
Contoh isi file.conf:
Firefox 2
Wireshark 3
Ketika menjalankan command contoh, program akan membuka 2 jendela aplikasi firefox dan 3 jendela aplikasi wireshark.

d. Program dapat mematikan semua aplikasi yg dijalankan oleh program tersebut dengan: 
setup -k

e. Program juga dapat mematikan aplikasi yang dijalankan sesuai dengan file konfigurasi. 
Contohnya: 
setup -k file.conf 
Command ini hanya mematikan aplikasi yang dijalankan dengan 
setup -f file.conf

NB: Mohon untuk testing, adaptasikan dengan kemampuan device masing-masing. Jangan menjalankan program jika device anda tidak kuat (contohnya laptop anda menggunakan RAM hanya 2gb namun mentesting dengan menjalankan setup -o firefox 10 wireshark 8). Jika terdapat kerusakan dikarenakan testing dari soal, ASISTEN LAB tidak memiliki tanggung jawab apapun terhadap kerusakan tersebut.

NB: Semua soal tidak boleh dikerjakan menggunakan fungsi system(). Di utamakan menggunakan fungsi fork() atau exec().
```
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

```
