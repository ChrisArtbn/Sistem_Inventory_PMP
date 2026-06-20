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
    printf_P(PSTR("\nSISTEM INVENTARISASI BARANG LABORATORIUM\n"));
    printf_P(PSTR("Pilihan yang tersedia:\n"));
    printf_P(PSTR("1. Tambah Data Barang\n"));
    printf_P(PSTR("2. Hapus Data Barang\n"));
    printf_P(PSTR("3. Cari Data Barang berdasarkan ID\n"));
    printf_P(PSTR("4. Perbarui Stok Barang\n"));
    printf_P(PSTR("5. Perbarui Status Barang\n"));
    printf_P(PSTR("6. Tampilkan Seluruh Data Inventaris\n"));
    printf_P(PSTR("7. Tampilkan Ringkasan Inventaris\n"));
    printf_P(PSTR("Silakan masukkan pilihan Anda (0 untuk keluar): "));
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
    printf_P(PSTR("Masukkan ID (1-9999): "));
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
        printf_P(PSTR("Data masih kosong\n"));
        return;
    }
    printf_P(PSTR("ID Barang     : %u\n"), current->active.payload.id_barang);
    printf_P(PSTR("Nama Komponen : %s\n"), current->active.payload.nama);
    printf_P(PSTR("Kategori      : ")); cetak_kategori_teks(current->active.payload.id_kategori); printf_P(PSTR("\n"));
    printf_P(PSTR("Lokasi        : ")); cetak_lokasi_teks(current->active.payload.id_lokasi); printf_P(PSTR("\n"));
    printf_P(PSTR("Jumlah Stok   : %d unit\n"), current->active.payload.jumlah_stock);
    printf_P(PSTR("Pemilik       : %s\n"), current->active.payload.pemilik);
    printf_P(PSTR("PIC           : %s\n"), current->active.payload.pic);

    if (current->active.payload.status_barang == TERSEDIA){
        printf_P(PSTR("Status        : Tersedia\n"));
    } else if (current->active.payload.status_barang == DIPINJAM){
        printf_P(PSTR("Status        : Dipinjam\n"));
    } else if (current->active.payload.status_barang == RUSAK){
        printf_P(PSTR("Status        : Rusak\n"));
    } else if (current->active.payload.status_barang == HABIS){
        printf_P(PSTR("Status        : Habis\n"));
    } else{
        printf_P(PSTR("Status        : Kondisi tidak diketahui (%d)\n"), current->active.payload.status_barang);
    }
    printf_P(PSTR("----------------------------------------\n"));
}