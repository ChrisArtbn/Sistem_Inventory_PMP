#ifndef INVENTORY_POOL_H
#define INVENTORY_POOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_ITEMS 50
#define MEMORY_WARNING_SLOT 5

typedef enum{
    LOKASI_RAK_A = 0,
    LOKASI_RAK_B = 1,
    LOKASI_LACI_1 = 2,
    LOKASI_LACI_2 = 3
} Lokasi;

typedef enum{
    KATEG_SENSOR = 0,
    KATEG_BOARD = 1,
    KATEG_MOTOR = 2,
    KATEG_KABEL = 3
} Kategori;

typedef enum{
    HABIS = 0,
    TERSEDIA = 1,
    DIPINJAM = 2,
    RUSAK = 3
} Status_Barang;

typedef struct __attribute__((packed)){
    uint16_t id_barang;  
    char nama[11];        
    uint8_t id_kategori;     
    uint8_t jumlah_stock;    
    uint8_t id_lokasi;     
    uint8_t status_barang;   
    char pemilik[4];         
    char pic[4];             
} Inventaris_Lab;            

typedef union Block{
    union Block *next_free;  
    struct {
        Inventaris_Lab payload;
        union Block *next_active; 
    } active;
} Block;

typedef struct{
    Block *freelist;       
    Block *activelist;    
    Block pool[MAX_ITEMS];   
} Memory_Pool_Inventaris;

void mulai_program();
void in_it_pool(Memory_Pool_Inventaris *mp);
void cek_memori(Memory_Pool_Inventaris *mp, int *sisa_slot);

void tambah_barang(Memory_Pool_Inventaris *mp);
void hapus_barang(Memory_Pool_Inventaris *mp);
void cari_barang_berdasarkan_ID(Memory_Pool_Inventaris *mp);
void update_stock(Memory_Pool_Inventaris *mp);
void update_status(Memory_Pool_Inventaris *mp);
void tampilkan_Semua_Data(MemoryPoolInventaris *mp);
void tampilkanRingkasan(MemoryPoolInventaris *mp);

// Fungsi Utilitas Input
void bacaPilihanMenu(int *pilihan);
void bacaInputID(uint16_t *id);

#endif
