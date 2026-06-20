#include "sistem_inventory.h"

void normalisasi_status(char* status)
{
    *status = (char)toupper((unsigned char)*status);
}

void validasi_status(char status, int* valid)
{
    *valid = 0;

    if (status == 'T' || status == 'D' || status == 'R' || status == 'H'){
        *valid = 1;
    }
}

void tambah_barang(Memory_Pool_Inventaris *mp)
{
    int sisa_slot;
    int valid;
    Inventaris_Lab data;
    Block* temp_barang;
    char temp_status;
    int temp_lokasi, temp_kategori, temp_stok;

    cek_memori(mp, &sisa_slot);

    if (sisa_slot <= 0){
        printf("Memori penuh, barang gagal ditambahkan\n");
        return;
    }
    baca_input_ID(&data.id_barang);

    temp_barang = cari_node(mp, data.id_barang);

    if (temp_barang != NULL){
        printf("Barang ini duplikat, ID sudah ada di inventaris\n");
        return;
    }

    printf("Masukkan Nama Barang : ");
    scanf(" %10[^\n]", data.nama);

    printf("Masukkan Lokasi : ");
    scanf("%d", &temp_lokasi);
    data.id_lokasi = (uint8_t)temp_lokasi;

    printf("Masukkan Kategori : ");
    scanf("%d", &temp_kategori);
    data.id_kategori = (uint8_t)temp_kategori;

    printf("Masukkan Jumlah Stok : ");
    scanf("%d", &temp_stok);

    if (temp_stok < 0){
        printf("Stok tidak boleh negatif\n");
        return;
    }
    data.jumlah_stock = (uint8_t)temp_stok;

    printf("Masukkan Status Barang (T=tersedia, D=dipinjam, R=rusak, H=habis) : ");
    scanf(" %c", &temp_status);

    normalisasi_status(&temp.status);
    validasi_status(temp.status, &valid);

    if (valid == 0){
        printf("Status tidak valid\n");
        return;
    }

    if (temp_status == 'T') data.status_barang = TERSEDIA;
    else if (temp_status == 'D') data.status_barang = DIPINJAM;
    else if (temp_status == 'R') data.status_barang = RUSAK;
    else if (temp_status == 'H') data.status_barang = HABIS;

    printf("Masukkan Nama Pemilik : ");
    scanf(" %3[^\n]", data.pemilik);

    printf("Masukkan Nama PIC : ");
    scanf(" %3[^\n]", data.pic);

    if (data.jumlah_stock == 0 && data.status_barang != HABIS){
        printf("Stok 0, status otomatis diubah menjadi Habis\n");
        data.status_barang = HABIS;
    }

    if (data.jumlah_stock > 0 && data.status_barang == HABIS){
        printf("Stok lebih dari 0, status otomatis diubah menjadi Tersedia\n");
        data.status_barang = TERSEDIA;
    }

    add_node(mp, data);

    printf("Barang berhasil dimasukkan\n");

    cek_memori(mp, &sisa_slot);

    if (sisa_slot <= MEMORY_WARNING_SLOT && sisa_slot > 0){
        printf("Peringatan: memori hampir penuh, sisa slot %d\n", sisa_slot);
    }
}

void hapus_barang(Memory_Pool_Inventaris *mp)
{
    uint16_t tempid;
    char status_hapus;

    if (mp->activelist == NULL){
        printf("Inventory Kosong\n");
        return;
    }

    baca_input_ID(&tempid);
    delete_node(mp, tempid, &status_hapus);

    if (status_hapus == 'G'){
        printf("Barang tidak ditemukan\n");
    } 
    else{
        if (status_hapus == 'A'){
            printf("Node pertama berhasil dihapus\n");
        } else if (status_hapus == 'T'){
            printf("Node tengah berhasil dihapus\n");
        } else if (status_hapus == 'K'){
            printf("Node akhir berhasil dihapus\n");
        }
        printf("Barang berhasil dihapus\n");
    }
}

void cari_barang_berdasarkan_ID(Memory_Pool_Inventaris *mp)
{
    uint16_t tempid;
    Block* tempor;

    if (mp->activelist == NULL){
        printf("Inventory Kosong\n");
        return;
    }
    
    baca_input_ID(&tempid);
    tempor = cari_node(mp, tempid);

    if (tempor != NULL){
        printf("Barang ditemukan\n");
        tampilkan_detail_barang(tempor);
    } 
    else{
        printf("Barang tidak ditemukan\n");
    }
}

void update_stock(Memory_Pool_Inventaris *mp)
{
    uint16_t tempid;
    Block* tempor;
    int tempangka;
    int pilihan;

    if (mp->activelist == NULL){
        printf("Inventory Kosong\n");
        return;
    }

    baca_input_ID(&tempid);
    tempor = cari_node(mp, tempid);

    if (tempor == NULL){
        printf("ID tidak ada dalam inventory\n");
        return;
    }

    printf("Stock sekarang: %d\n", tempor->active.payload.jumlah_stock);
    printf("Mau berubah berapa banyak kang? ");
    scanf("%d", &tempangka);

    if (tempangka < 0){
        printf("Jumlah perubahan stok tidak boleh negatif\n");
        return;
    }

    printf("Ketik 1 jika ingin menambah dan 2 jika ingin mengurangi : ");
    scanf("%d", &pilihan);

    if (pilihan == 1){
        tempor->active.payload.jumlah_stock += (uint8_t)tempangka;
        printf("Stok telah ditambahkan\n");

        if (tempor->active.payload.jumlah_stock > 0 && tempor->active.payload.status_barang == HABIS){
            tempor->active.payload.status_barang = TERSEDIA;
        }
    } else if (pilihan == 2){
        if (tempangka > tempor->active.payload.jumlah_stock){
            printf("Jumlah melebihi stok saat ini\n");
        } 
        else{
            tempor->active.payload.jumlah_stock -= (uint8_t)tempangka;
            printf("Stok berhasil dikurangi\n");

            if (tempor->active.payload.jumlah_stock == 0){
                tempor->active.payload.status_barang = HABIS;
            }
        }
    } 
    else{
        printf("Pilihan update stok tidak valid\n");
    }
}

void update_status(Memory_Pool_Inventaris *mp)
{
    uint16_t tempid;
    char new_stats;
    int valid;
    Block* tempor;

    if (mp->activelist == NULL){
        printf("Inventory Kosong\n");
        return;
    }
    
    baca_input_ID(&tempid);
    tempor = cari_node(mp, tempid);

    if (tempor == NULL){
        printf("Barang tidak ditemukan\n");
        return;
    }
    
    printf("Pilih status baru (T=tersedia, D=dipinjam, R=rusak, H=habis) : ");
    scanf(" %c", &new_stats);

    normalisasi_status(&new_stats);
    validasi_status(new_stats, &valid);

    if (valid == 1){
        if (new_stats == 'T') tempor->active.payload.status_barang = TERSEDIA;
        else if (new_stats == 'D') tempor->active.payload.status_barang = DIPINJAM;
        else if (new_stats == 'R') tempor->active.payload.status_barang = RUSAK;
        else if (new_stats == 'H') tempor->active.payload.status_barang = HABIS;

        if (new_stats == 'H'){
            tempor->active.payload.jumlah_stock = 0;
        }
        printf("Status berhasil diperbarui\n");
    }
    else{
        printf("Status tidak valid\n");
    }
}

void tampilkan_semua_data(MemoryPoolInventaris *mp)
{
    Block* current;
    int i;

    if (mp->activelist == NULL){
        printf("Inventory Kosong\n");
        return;
    }

    current = mp->activelist;
    i = 1;

    while (current != NULL){
        printf("Item ke-%d\n", i);
        tampilkan_detail_barang(current);
        current = current->active.next_active;
        i++;
    }
}

void tampilkan_ringkasan(MemoryPoolInventaris *mp)
{
    int total_jenis = 0;
    int total_stok = 0;
    int count_stat_tersedia = 0;
    int count_stat_dipinjam = 0;
    int count_stat_habis = 0;
    int count_stat_rusak = 0;
    int sisa_slot;
    Block* current;

    if (mp->activelist == NULL){
        printf("Data kosong\n");
        return;
    }

    current = mp->activelist;

    while (current != NULL){
        total_jenis++;
        total_stok += current->active.payload.jumlah_stock;

        if (current->active.payload.status_barang == TERSEDIA) count_stat_tersedia++;
        else if (current->active.payload.status_barang == HABIS) count_stat_habis++;
        else if (current->active.payload.status_barang == DIPINJAM) count_stat_dipinjam++;
        else if (current->active.payload.status_barang == RUSAK) count_stat_rusak++;
        
        current = current->active.next_active;
    }

    cek_memori(mp, &sisa_slot);

    printf("\nRingkasan Inventaris\n");
    printf("Total Jenis Barang : %d\n", total_jenis);
    printf("Total Stok Barang  : %d\n", total_stok);
    printf("Jumlah Tersedia    : %d\n", count_stat_tersedia);
    printf("Jumlah Dipinjam    : %d\n", count_stat_dipinjam);
    printf("Jumlah Rusak       : %d\n", count_stat_rusak);
    printf("Jumlah Habis       : %d\n", count_stat_habis);
    printf("Kapasitas Maksimum : %d item\n", MAX_ITEMS);
    printf("Sisa Slot Memori   : %d item\n", sisa_slot);

    if (sisa_slot <= MEMORY_WARNING_SLOT && sisa_slot > 0){
        printf("Peringatan: memori hampir penuh\n");
    }
}

void mulai_program(void)
{
    Memory_Pool_Inventaris mp;
    int pilihan;

    pilihan = -1;
    in_it_pool(&mp);

    while (pilihan != 0){
        menu_utama();
        baca_pilihan_menu(&pilihan);

        if (pilihan == 1){
            tambah_barang(&mp);
        } else if (pilihan == 2){
            hapus_barang(&mp);
        } else if (pilihan == 3){
            cari_barang_berdasarkan_ID(&mp);
        } else if (pilihan == 4){
            update_stock(&mp);
        } else if (pilihan == 5){
            update_status(&mp);
        } else if (pilihan == 6){
            tampilkan_semua_data(&mp);
        } else if (pilihan == 7){
            tampilkan_ringkasan(&mp);
        } else if (pilihan == 8){
            muat_file(&mp, "input.txt");
        } else if (pilihan == 9){
            simpan_ke_file(&mp, "output.txt");
        } else if (pilihan == 0){
            printf("Program selesai\n");
        } else{
            printf("Pilihan tidak valid\n");
        }
    }
    clear_list(&mp);
}
