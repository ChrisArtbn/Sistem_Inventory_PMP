#include "sistem_inventory.h"

static void clear_buffer()
{
    int c;
    c = getchar();
    while (c != '\n' && c != EOF){
        c = getchar();
    }
}

void menu_utama()
{
    printf("\nSISTEM INVENTARISASI BARANG LABORATORIUM\n");
    printf("Pilihan yang tersedia:\n");
    printf("1. Tambah Data Barang\n");
    printf("2. Hapus Data Barang\n");
    printf("3. Cari Data Barang berdasarkan ID\n");
    printf("4. Perbarui Stok Barang\n");
    printf("5. Perbarui Status Barang\n");
    printf("6. Tampilkan Seluruh Data Inventaris\n");
    printf("7. Tampilkan Ringkasan Inventaris\n");
    printf("Silakan masukkan pilihan Anda (0 untuk keluar): ");
}

void baca_pilihan_menu(int *pilihan)
{
    if (scanf("%d", pilihan) != 1){
        *pilihan = -1;
        clear_buffer();
    }
}

void baca_input_ID(uint16_t *id)
{
    unsigned int temp_id;
    printf("Masukkan ID : ");
    if (scanf("%u", &temp_id) == 1){
        *id = (uint16_t)temp_id;
    } 
    else{
        *id = 0;
        clear_buffer();
    }
}

void tampilkan_detail_barang(Block* current)
{
    if (current == NULL){
        printf("Data masih kosong\n");
        return;
    }
    printf("ID Barang     : %u\n", current->active.payload.id_barang);
    printf("Nama Komponen : %s\n", current->active.payload.nama);
    printf("Kategori      : %d\n", current->active.payload.id_kategori);
    printf("Lokasi        : %d\n", current->active.payload.id_lokasi);
    printf("Jumlah Stok   : %d unit\n", current->active.payload.jumlah_stock);
    printf("Pemilik       : %s\n", current->active.payload.pemilik);
    printf("PIC           : %s\n", current->active.payload.pic);

    if (current->active.payload.status_barang == TERSEDIA){
        printf("Status        : Tersedia\n");
    } else if (current->active.payload.status_barang == DIPINJAM){
        printf("Status        : Dipinjam\n");
    } else if (current->active.payload.status_barang == RUSAK){
        printf("Status        : Rusak\n");
    } else if (current->active.payload.status_barang == HABIS){
        printf("Status        : Habis\n");
    } else{
        printf("Status        : Kondisi tidak diketahui (%d)\n", current->active.payload.status_barang);
    }
    printf("----------------------------------------\n");
}
