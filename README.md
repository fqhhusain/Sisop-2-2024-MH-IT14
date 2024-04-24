# Sisop-2-2024-MH-IT14
Laporan pengerjaan soal shift modul 2 Praktikum Sistem Operasi 2024 oleh Kelompok IT14
## Praktikan Sistem Operasi Kelompok IT14
1. Tsaldia Hukma Cita          : 5027231036
2. Muhammad Faqih Husain       : 5027231023
3. Muhammad Syahmi Ash Shidqi  : 5027231085

Soal Shift Modul 2

1. Gavriel adalah seorang cyber security enthusiast. Suatu hari, ia dikontrak oleh sebuah perusahaan ternama untuk membuat sebuah program yang cukup rumit dengan bayaran jutaan rupiah. Karena tergoda dengan nominal bayaran tersebut, Gavriel langsung menerima tawaran tersebut. Setelah mencoba membuat dan mengembangkan program tersebut selama seminggu, laptop yang digunakan Gavriel mengalami overheat dan mati total sehingga harus dilarikan ke tukang servis terdekat. Karena keterbatasan waktu dalam pembuatan program dan tidak ingin mengecewakan perusahaan, Gavriel meminta bantuan kalian untuk membuat program tersebut dengan ketentuan sebagai berikut:

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
Mencatat setiap peristiwa yang terjadi ke dalam file .log yang bernama “history.log” dengan ketentuan:
Format: [nama_user][HH:MM:SS] - <nama_file> - <action>
nama_user adalah username yang melakukan action terhadap file
Format action untuk setiap kode:
kode r3N4mE: Successfully renamed.
kode d3Let3: Successfully deleted.
mode backup: Successfully moved to backup.
mode restore: Successfully restored from backup.
Contoh pesan log:
[paul][00:00:00] - r3N4mE.ts - Successfully renamed.
[paul][00:00:00] - m0V3.xk1 - Successfully restored from backup.

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
Untuk setiap file dengan nama yang memuat kode d3Let3, hapus file tersebut. 
Sementara itu, untuk setiap file dengan nama yang memuat kode r3N4mE, lakukan hal berikut:
Jika ekstensi file adalah “.ts”, rename filenya menjadi “helper.ts”
Jika ekstensi file adalah “.py”, rename filenya menjadi “calculator.py”
Jika ekstensi file adalah “.go”, rename filenya menjadi “server.go”
Jika file tidak memuat salah satu dari ekstensi diatas, rename filenya menjadi “renamed.file”

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
Melakukan signaling untuk mengganti mode dari program ini tanpa menghentikannya terlebih dahulu. Oleh karena itu, bantulan Paul untuk mengintegrasikan kemampuan untuk mengganti mode ini dengan mengirim sinyal ke daemon, dengan ketentuan:
SIGRTMIN untuk mode default
SIGUSR1 untuk mode backup
SIGUSR2 untuk mode restore
Contoh penggunaan: kill -SIGUSR2 <pid_program>
```

volatile sig_atomic_t stop;

void inthand(int signum) {
    stop = 1;
}
```
Mengubah nilai stop menjadi 1 untuk menghentikan while loop pada main function sehingga daemon dapat berhenti dengan efisien
```
int main(int argc, char *argv[]) {
```
Main function
```
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
```
Menjalankan program secara daemon
```
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
```
```
if (argc == 3 && strcmp(argv[1], "-m") == 0) {
        if (strcmp(argv[2], "backup") == 0) {
        mode = 1;
        } else if (strcmp(argv[2], "restore") == 0) {
        mode = 2;
    }
}
```
Mengatur mode dari program berdasarkan input pengguna
default: program berjalan seperti biasa untuk me-rename dan menghapus file. Mode ini dieksekusi ketika program dijalankan tanpa argumen tambahan, yaitu dengan command ./management saja
backup: program memindahkan file dengan kode m0V3 ke sebuah folder bernama “backup”
restore: program mengembalikan file dengan kode m0V3 ke folder sebelum file tersebut dipindahkan
Contoh penggunaan: ./management -m backup
```
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
    sleep(1); //program berjalan setiap satu detik
  }
}
```
Berikut adalah struktur folder untuk pengerjaan nomor 2:
    soal_2/
    ├── history.log
    ├── management.c
    └── library/
        └── backup/
contoh penggunaan
![Screenshot_2024-04-24_03_23_55](https://github.com/fqhhusain/Sisop-2-2024-MH-IT14/assets/88548292/a4556d02-a688-4273-bf49-618dc5a31b29)



3. Pak Heze adalah seorang admin yang baik. Beliau ingin membuat sebuah program admin yang dapat memantau para pengguna sistemnya. Bantulah Pak Heze untuk membuat program  tersebut!

4. Salomo memiliki passion yang sangat dalam di bidang sistem operasi. Saat ini, dia ingin mengotomasi kegiatan-kegiatan yang ia lakukan agar dapat bekerja secara efisien. Bantulah Salomo untuk membuat program yang dapat mengotomasi kegiatan dia!
(NB: Soal di uji coba dan berhasil di sistem yang menggunakan MacOS dan linux yang menggunakan dual boot dan VM. Untuk teman-teman yang menggunakan WSL bisa mengerjakan soal yang lain ya)

Cara Bermain
```
./setup -o <app1> <num1> <app2> <num2>.....<appN> <numN>
./setup -o firefox 2 wireshark 2
./setup -f file.conf
./setup -k
./setup -k file.conf 
```
-o untuk membuka aplikasi
-f untuk membuka aplikasi berdasarkan file konfigurasi
-k untuk mematikan semua aplikasi yang dijalankan oleh program
-k <filename> untuk mematikan aplikasi berdasarkan file konfigurasi
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
```
Library yang digunakan
```
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
```
Fungsi untuk membuka window
```

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
```
Fungsi untuk membaca file konfigurasi
```

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
