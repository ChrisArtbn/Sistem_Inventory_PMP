#ifndef INVENTORY_POOL_H
#define INVENTORY_POOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> // Dibutuhkan untuk tipe data hemat byte seperti uint8_t

#define MAX_ITEMS 50 // Target 50 data
#define MEMORY_WARNING_SLOT 5

// 1. PENGGUNAAN ENUM UNTUK MENGHEMAT STRING LOKASI, KATEGORI & STATUS
// (Masing-masing hanya memakan 1 byte)
typedef enum {
    LOKASI_RAK_A = 0,
    LOKASI_RAK_B = 1,
    LOKASI_LACI_1 = 2,
    LOKASI_LACI_2 = 3
} Lokasi;

typedef enum {
    KATEG_SENSOR = 0,
    KATEG_BOARD = 1,
    KATEG_MOTOR = 2,
    KATEG_KABEL = 3
} Kategori;

typedef enum {
    HABIS = 0,
    TERSEDIA = 1,
    DIPINJAM = 2,
    RUSAK = 3
} StatusBarang;

// 2. STRUKTUR DATA UTAMA (DIPADATKAN)
// Penggunaan __attribute__((packed)) memastikan kompilator tidak menambahkan
// byte "padding" kosong di antara variabel.
typedef struct __attribute__((packed)) {
    uint16_t id_barang;      // 2 byte (Menyimpan angka 0 - 65535, ganti char array jadi ID numerik)
    char nama[11];           // 12 byte (Maksimal 11 karakter + null terminator)
    uint8_t id_kategori;     // 1 byte (Disambungkan dengan enum Kategori)
    uint8_t jumlah_stok;     // 1 byte (Menyimpan angka 0 - 255)
    uint8_t id_lokasi;       // 1 byte (Disambungkan dengan enum Lokasi)
    uint8_t status_barang;   // 1 byte (Disambungkan dengan enum StatusBarang)
    char pemilik[4];         // 4 byte (Kode inisial 3 huruf + null terminator)
    char pic[4];             // 4 byte (Kode inisial 3 huruf + null terminator)
} InventarisLab;             // TOTAL: Hanya 26 byte per data!

// 3. IMPLEMENTASI MEMORY POOL MENGGUNAKAN UNION
// Union menumpuk memori secara bergantian.
// Jika kosong, slot dipakai untuk `next_free` (pointer ke slot kosong berikutnya).
// Jika terisi, slot dipakai untuk menyimpan `payload` dan pointer `next_active`.
typedef union Block {
    union Block *next_free;  
    struct {
        InventarisLab payload;
        union Block *next_active; 
    } active;
} Block;

// 4. STRUKTUR PENGELOLA POOL
typedef struct {
    Block *freelist;         // Kepala antrean slot memori yang kosong
    Block *activelist;       // Kepala antrean linked-list data yang sudah terisi
    Block pool[MAX_ITEMS];   // Array statis di SRAM yang menampung ke-50 data
} MemoryPoolInventaris;

// 5. DEKLARASI FUNGSI
// Syarat Tugas: Semua fungsi selain main harus void [cite: 15]

void mulaiProgram(void);
void initPool(MemoryPoolInventaris *mp);
void cekMemori(MemoryPoolInventaris *mp, int *sisaSlot);

// Fungsi Operasi CRUD Linked List
void tambahBarang(MemoryPoolInventaris *mp);
void hapusBarang(MemoryPoolInventaris *mp);
void cariBarangBerdasarkanID(MemoryPoolInventaris *mp);
void updateStok(MemoryPoolInventaris *mp);
void updateStatus(MemoryPoolInventaris *mp);
void tampilkanSemuaData(MemoryPoolInventaris *mp);
void tampilkanRingkasan(MemoryPoolInventaris *mp);

// Fungsi Utilitas Input
void bacaPilihanMenu(int *pilihan);
void bacaInputID(uint16_t *id);

#endif
