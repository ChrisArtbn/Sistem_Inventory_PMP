#ifndef SISTEM_INVENTORY_H
#define SISTEM_INVENTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <avr/pgmspace.h>

#define MAX_ITEMS 50
#define MEMORY_WARNING_SLOT 5
#define LINE_LEN 256

typedef enum{
    LOKASI_LAB_1 = 0,
    LOKASI_LAB_2 = 1,
    LOKASI_LAB_3 = 2,
    LOKASI_LAB_4 = 3
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
    char pemilik[5];         
    char pic[5];             
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
void tampilkan_semua_data(Memory_Pool_Inventaris *mp);
void tampilkan_ringkasan(Memory_Pool_Inventaris *mp);

void menu_utama();
void baca_pilihan_menu(int *pilihan);
void baca_input_ID(uint16_t *id);
void tampilkan_detail_barang(Block* current);

void add_node(Memory_Pool_Inventaris *mp, Inventaris_Lab new_item);
Block* cari_node(Memory_Pool_Inventaris *mp, uint16_t id);
void delete_node(Memory_Pool_Inventaris *mp, uint16_t id, char* status_hapus);
void clear_list(Memory_Pool_Inventaris *mp);
void isi_data_dummy(Memory_Pool_Inventaris *mp);
void cetak_kategori_teks(uint8_t kat);
void cetak_lokasi_teks(uint8_t lok);

#endif
