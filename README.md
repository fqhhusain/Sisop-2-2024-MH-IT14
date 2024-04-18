# Sisop-2-2024-MH-IT14
Laporan pengerjaan soal shift modul 2 Praktikum Sistem Operasi 2024 oleh Kelompok IT14
## Praktikan Sistem Operasi Kelompok IT14
1. Tsaldia Hukma Cita          : 5027231036
2. Muhammad Faqih Husain       : 5027231023
3. Muhammad Syahmi Ash Shidqi  : 5027231085

Soal Shift Modul 2

5. Salomo memiliki passion yang sangat dalam di bidang sistem operasi. Saat ini, dia ingin mengotomasi kegiatan-kegiatan yang ia lakukan agar dapat bekerja secara efisien. Bantulah Salomo untuk membuat program yang dapat mengotomasi kegiatan dia!
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
