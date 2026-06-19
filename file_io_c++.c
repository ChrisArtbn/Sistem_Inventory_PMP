#include "inventory_final.h"

static uint8_t stringKeKategori(const char* str) {
    if (strstr(str, "Board")) return KATEG_BOARD;
    if (strstr(str, "Motor")) return KATEG_MOTOR;
    if (strstr(str, "Kabel")) return KATEG_KABEL;
    return KATEG_SENSOR; 
}

static const char* kategoriKeString(uint8_t k) {
    if (k == KATEG_BOARD) return "Board";
    if (k == KATEG_MOTOR) return "Motor";
    if (k == KATEG_KABEL) return "Kabel";
    return "Sensor";
}

static uint8_t stringKeLokasi(const char* str) {
    if (strstr(str, "Lab 2")) return LOKASI_Lab_2;
    if (strstr(str, "Lab 3")) return LOKASI_Lab_3;
    if (strstr(str, "Lab 4")) return LOKASI_Lab_4;
    return LOKASI_Lab_1; 
}

static const char* lokasiKeString(uint8_t l) {
    if (l == LOKASI_Lab_2) return "Lab 2";
    if (l == LOKASI_Lab_3) return "Lab 3";
    if (l == LOKASI_Lab_4) return "Lab 4";
    return "Lab 1";
}

static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != '\r' && c != EOF);
}

void cetakMenuUtama(void) {
    printf_P(PSTR("\n=== SISTEM INVENTARIS LAB (MEMORY POOL) ===\n"));
    printf_P(PSTR("1. Tambah Data Barang\n"));
    printf_P(PSTR("2. Hapus Data Barang\n"));
    printf_P(PSTR("3. Cari Data Barang berdasarkan ID\n"));
    printf_P(PSTR("4. Perbarui Stok Barang\n"));
    printf_P(PSTR("5. Perbarui Status Barang\n"));
    printf_P(PSTR("6. Tampilkan Seluruh Data Inventaris\n"));
    printf_P(PSTR("7. Tampilkan Ringkasan Inventaris\n"));
    printf_P(PSTR("Silakan masukkan pilihan Anda (0 untuk keluar): "));
}

void bacapilmenu(int *pilihan) {
    if (scanf("%d", pilihan) != 1) {
        *pilihan = -1;
        clearInputBuffer();
    }
}

void bacainputdata(InventarisLab *data) {
    char temp_kategori[15], temp_lokasi[15];
    char status_char;

    printf_P(PSTR("\nMasukkan ID (Angka, maks 65535): "));
    scanf("%hu", &data->id_barang);
    clearInputBuffer();

    printf_P(PSTR("Masukkan Nama Barang (Maks 11 huruf): "));
    scanf(" %11[^\n]", data->nama);

    printf_P(PSTR("Masukkan Kategori (Sensor/Board/Motor/Kabel): "));
    scanf(" %14s", temp_kategori);
    data->id_kategori = stringKeKategori(temp_kategori);

    printf_P(PSTR("Masukkan Jumlah Stok Awal (0-255): "));
    int stok;
    scanf("%d", &stok);
    data->jumlah_stok = (uint8_t)stok;

    printf_P(PSTR("Masukkan Lokasi Fisik (Lab 1/Lab 2/Lab 3/Lab 4): "));
    scanf(" %14[^\n]", temp_lokasi);
    data->id_lokasi = stringKeLokasi(temp_lokasi);

    printf_P(PSTR("Masukkan Status (T=Tersedia/D=Dipinjam/R=Rusak/H=Habis): "));
    scanf(" %c", &status_char);
    if (status_char == 'T' || status_char == 't') data->status_barang = TERSEDIA;
    else if (status_char == 'D' || status_char == 'd') data->status_barang = DIPINJAM;
    else if (status_char == 'R' || status_char == 'r') data->status_barang = RUSAK;
    else data->status_barang = HABIS;

    printf_P(PSTR("\nMasukkan Pemilik Barang (Inisial 3 huruf): "));
    scanf(" %3s", data->pemilik);

    printf_P(PSTR("\nMasukkan PIC (Inisial 3 huruf): "));
    scanf(" %3s", data->pic);
}

void bacainputID(uint16_t *id) {
    printf_P(PSTR("\nMasukkan ID Barang (Angka): "));
    scanf("%hu", id);
}

void tampilkanDetailBarang(Block* current) {
    if (current == NULL) {
        printf_P(PSTR("Data tidak ditemukan.\n"));
        return;
    }
    
    InventarisLab* item = &current->active.payload;

    printf_P(PSTR("ID Barang     : %u\n"), item->id_barang);
    printf_P(PSTR("Nama Komponen : %s\n"), item->nama);
    printf_P(PSTR("Kategori      : %s\n"), kategoriKeString(item->id_kategori));
    printf_P(PSTR("Lokasi Fisik  : %s\n"), lokasiKeString(item->id_lokasi));
    printf_P(PSTR("Jumlah Stok   : %u unit\n"), item->jumlah_stok);
    printf_P(PSTR("Pemilik       : %s\n"), item->pemilik);
    printf_P(PSTR("PIC           : %s\n"), item->pic);

    printf_P(PSTR("Status        : "));
    switch (item->status_barang) {
        case TERSEDIA: printf_P(PSTR("Tersedia\n")); break;
        case DIPINJAM: printf_P(PSTR("Dipinjam\n")); break;
        case RUSAK: printf_P(PSTR("Rusak\n")); break;
        case HABIS: printf_P(PSTR("Habis\n")); break;
        default: printf_P(PSTR("Kondisi tidak diketahui\n"));
    }
    printf_P(PSTR("----------------------------------------\n"));
}

void muatDariFile(Block** head, const char* nama_file) {
    printf_P(PSTR("\n[ERROR] Maaf, Arduino Uno tidak memiliki memori file bawaan.\n"));
    printf_P(PSTR("Fitur muat data dari '%s' ini membutuhkan Modul SD Card.\n"), nama_file);
}

void simpanKeFile(Block* head, const char* nama_file) {
    printf_P(PSTR("\n[ERROR] Maaf, Arduino Uno tidak memiliki memori file bawaan.\n"));
    printf_P(PSTR("Fitur simpan data ke '%s' ini membutuhkan Modul SD Card.\n"), nama_file);
}
