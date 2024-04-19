# Sisop-2-2024-MH-IT14
Laporan pengerjaan soal shift modul 2 Praktikum Sistem Operasi 2024 oleh Kelompok IT14
## Praktikan Sistem Operasi Kelompok IT14
1. Tsaldia Hukma Cita          : 5027231036
2. Muhammad Faqih Husain       : 5027231023
3. Muhammad Syahmi Ash Shidqi  : 5027231085

Soal Shift Modul 2

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
