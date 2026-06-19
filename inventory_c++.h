#ifndef INVENTORY_POOL_H
#define INVENTORY_POOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <avr/pgmspace.h> // <-- TAMBAHKAN INI UNTUK MENGHEMAT MEMORI RAM

#define MAX_ITEMS 50 // Target 50 data dipertahankan
#define MEMORY_WARNING_SLOT 5

typedef enum {
    LOKASI_Lab_1 = 0, LOKASI_Lab_2 = 1, LOKASI_Lab_3 = 2, LOKASI_Lab_4 = 3
} Lokasi;

typedef enum {
    KATEG_SENSOR = 0, KATEG_BOARD = 1, KATEG_MOTOR = 2, KATEG_KABEL = 3
} Kategori;

typedef enum {
    HABIS = 0, TERSEDIA = 1, DIPINJAM = 2, RUSAK = 3
} StatusBarang;

typedef struct __attribute__((packed)) {
    uint16_t id_barang;
    char nama[8];
    uint8_t id_kategori;
    uint8_t jumlah_stok;
    uint8_t id_lokasi;
    uint8_t status_barang;
    char pemilik[4];
    char pic[4];
} InventarisLab;

typedef union Block {
    union Block *next_free;  
    struct {
        InventarisLab payload;
        union Block *next_active; 
    } active;
} Block;

typedef struct {
    Block *freelist;
    Block *activelist;
    Block pool[MAX_ITEMS];
} MemoryPoolInventaris;

void mulaiProgram(void);
void initLinkedList(void);
void cekMemori(int* sisaSlot);
void addNode(Block** head, InventarisLab newItem);
void searchNode(Block* head, uint16_t id, Block** hasil);
void deletenode(Block** head, uint16_t id, char* statusHapus);
void clearList(Block** head);

void tambahBarang(Block** head);
void hapusBarang(Block** head);
void cariBarang(Block* head);
void updateStok(Block* head);
void updateStatus(Block* head);
void tampilkanData(Block* head);
void ringkasan(Block* head);

void cetakMenuUtama(void);
void bacapilmenu(int *pilihan);
void bacainputdata(InventarisLab *data);
void bacainputID(uint16_t *id);
void tampilkanDetailBarang(Block* current);
void muatDariFile(Block** head, const char* nama_file);
void simpanKeFile(Block* head, const char* nama_file);

#endif