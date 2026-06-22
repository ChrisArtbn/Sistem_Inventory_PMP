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
        printf_P(PSTR("Memori penuh, barang gagal ditambahkan\n"));
        return;
    }
    baca_input_ID(&data.id_barang);

    cari_node(mp, data.id_barang, &temp_barang);

    if (temp_barang != NULL){
        printf_P(PSTR("Barang ini duplikat, ID sudah ada di inventaris\n"));
        return;
    }

    printf_P(PSTR("Masukkan Nama Barang : (0-11 char)"));
    scanf(" %10[^\n]", data.nama);

    printf_P(PSTR("Masukkan Lokasi : (0:LAB 1, 1:LAB 2, 2:LAB 3, 3:LAB 4) "));
    scanf("%d", &temp_lokasi);
    data.id_lokasi = (uint8_t)temp_lokasi;

    printf_P(PSTR("Masukkan Kategori : (0:Sensor, 1:Board, 2:Motor , 3:Kabel) "));
    scanf("%d", &temp_kategori);
    data.id_kategori = (uint8_t)temp_kategori;

    printf_P(PSTR("Masukkan Jumlah Stok (0-255) :  "));
    scanf("%d", &temp_stok);

    if (temp_stok < 0){
        printf_P(PSTR("Stok tidak boleh negatif\n"));
        return;
    }
    if(temp_stok > 255){
        printf_P(PSTR("Stok melebihi kapasitas maksimum\n"));
        return;
    }
    data.jumlah_stock = (uint8_t)temp_stok;

    printf_P(PSTR("Masukkan Status Barang (T=tersedia, D=dipinjam, R=rusak, H=habis) : "));
    scanf(" %c", &temp_status);
    while (getchar() != '\n');

    normalisasi_status(&temp_status);
    validasi_status(temp_status, &valid);

    if (valid == 0){
        printf_P(PSTR("Status tidak valid\n"));
        return;
    }

    if (temp_status == 'T') data.status_barang = TERSEDIA;
    else if (temp_status == 'D') data.status_barang = DIPINJAM;
    else if (temp_status == 'R') data.status_barang = RUSAK;
    else if (temp_status == 'H') data.status_barang = HABIS;

    printf_P(PSTR("Masukkan Nama Pemilik : "));
    scanf(" %4[^\n]", data.pemilik); 
    while (getchar() != '\n'); // Membersihkan buffer

    printf_P(PSTR("Masukkan Nama PIC : "));
    scanf(" %4[^\n]", data.pic);
    while (getchar() != '\n');

    if (data.jumlah_stock == 0 && data.status_barang != HABIS){
        printf_P(PSTR("Stok 0, status otomatis diubah menjadi Habis\n"));
        data.status_barang = HABIS;
    }

    if (data.jumlah_stock > 0 && data.status_barang == HABIS){
        printf_P(PSTR("Stok lebih dari 0, status otomatis diubah menjadi Tersedia\n"));
        data.status_barang = TERSEDIA;
    }

    add_node(mp, data);

    printf_P(PSTR("Barang berhasil dimasukkan\n"));

    cek_memori(mp, &sisa_slot);

    if (sisa_slot <= MEMORY_WARNING_SLOT && sisa_slot > 0){
        printf_P(PSTR("Peringatan: memori hampir penuh, sisa slot %d\n"), sisa_slot);
    }
}

void hapus_barang(Memory_Pool_Inventaris *mp)
{
    uint16_t tempid;
    char status_hapus;

    if (mp->activelist == NULL){
        printf_P(PSTR("Inventory Kosong\n"));
        return;
    }

    baca_input_ID(&tempid);
    delete_node(mp, tempid, &status_hapus);

    if (status_hapus == 'G'){
        printf_P(PSTR("Barang tidak ditemukan\n"));
    } 
    else{
        if (status_hapus == 'A'){
            printf_P(PSTR("Node pertama berhasil dihapus\n"));
        } else if (status_hapus == 'T'){
            printf_P(PSTR("Node tengah berhasil dihapus\n"));
        } else if (status_hapus == 'K'){
            printf_P(PSTR("Node akhir berhasil dihapus\n"));
        }
        printf_P(PSTR("Barang berhasil dihapus\n"));
    }
}

void cari_barang_berdasarkan_ID(Memory_Pool_Inventaris *mp)
{
    uint16_t tempid;
    Block* tempor;

    if (mp->activelist == NULL){
        printf_P(PSTR("Inventory Kosong\n"));
        return;
    }
    
    baca_input_ID(&tempid);
    cari_node(mp, tempid, &tempor);

    if (tempor != NULL){
        printf_P(PSTR("Barang ditemukan\n"));
        tampilkan_detail_barang(tempor);
    } 
    else{
        printf_P(PSTR("Barang tidak ditemukan\n"));
    }
}

void update_stock(Memory_Pool_Inventaris *mp)
{
    uint16_t tempid;
    Block* tempor;
    int tempangka;
    int pilihan;

    if (mp->activelist == NULL){
        printf_P(PSTR("Inventory Kosong\n"));
        return;
    }

    baca_input_ID(&tempid);
    cari_node(mp, tempid, &tempor);

    if (tempor == NULL){
        printf_P(PSTR("ID tidak ada dalam inventory\n"));
        return;
    }

    printf_P(PSTR("Stock sekarang: %d\n"), tempor->active.payload.jumlah_stock);
    printf_P(PSTR("Mau Ditambah/Dikurang berapa? "));
    scanf("%d", &tempangka);

    if (tempangka < 0){
        printf_P(PSTR("Jumlah perubahan stok tidak boleh negatif\n"));
        return;
    }

    printf_P(PSTR("Ketik 1 jika ingin menambah dan 2 jika ingin mengurangi : "));
    scanf("%d", &pilihan);

    if (pilihan == 1){
        tempor->active.payload.jumlah_stock += (uint8_t)tempangka;
        printf_P(PSTR("Stok telah ditambahkan\n"));

        if (tempor->active.payload.jumlah_stock > 0 && tempor->active.payload.status_barang == HABIS){
            tempor->active.payload.status_barang = TERSEDIA;
        }
    } else if (pilihan == 2){
        if (tempangka > tempor->active.payload.jumlah_stock){
            printf_P(PSTR("Jumlah melebihi stok saat ini\n"));
        } 
        else{
            tempor->active.payload.jumlah_stock -= (uint8_t)tempangka;
            printf_P(PSTR("Stok berhasil dikurangi\n"));

            if (tempor->active.payload.jumlah_stock == 0){
                tempor->active.payload.status_barang = HABIS;
            }
        }
    } 
    else{
        printf_P(PSTR("Pilihan update stok tidak valid\n"));
    }
}

void update_status(Memory_Pool_Inventaris *mp)
{
    uint16_t tempid;
    char new_stats;
    int valid;
    Block* tempor;

    if (mp->activelist == NULL){
        printf_P(PSTR("Inventory Kosong\n"));
        return;
    }
    
    baca_input_ID(&tempid);
    cari_node(mp, tempid, &tempor);

    if (tempor == NULL){
        printf_P(PSTR("Barang tidak ditemukan\n"));
        return;
    }
    
    printf_P(PSTR("Pilih status baru (T=tersedia, D=dipinjam, R=rusak, H=habis) : "));
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
        printf_P(PSTR("Status berhasil diperbarui\n"));
    }
    else{
        printf_P(PSTR("Status tidak valid\n"));
    }
}

void tampilkan_semua_data(Memory_Pool_Inventaris *mp)
{
    Block* current;
    int i;

    if (mp->activelist == NULL){
        printf_P(PSTR("Inventory Kosong\n"));
        return;
    }

    current = mp->activelist;
    i = 1;

    while (current != NULL){
        printf_P(PSTR("Item ke-%d\n"), i);
        tampilkan_detail_barang(current);
        current = current->active.next_active;
        i++;
    }
}

void tampilkan_ringkasan(Memory_Pool_Inventaris *mp)
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
        printf_P(PSTR("Data kosong\n"));
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

    printf_P(PSTR("\nRingkasan Inventaris\n"));
    printf_P(PSTR("Total Jenis Barang : %d\n"), total_jenis);
    printf_P(PSTR("Total Stok Barang  : %d\n"), total_stok);
    printf_P(PSTR("Jumlah Tersedia    : %d\n"), count_stat_tersedia);
    printf_P(PSTR("Jumlah Dipinjam    : %d\n"), count_stat_dipinjam);
    printf_P(PSTR("Jumlah Rusak       : %d\n"), count_stat_rusak);
    printf_P(PSTR("Jumlah Habis       : %d\n"), count_stat_habis);
    printf_P(PSTR("Kapasitas Maksimum : %d item\n"), MAX_ITEMS);
    printf_P(PSTR("Sisa Slot Memori   : %d item\n"), sisa_slot);

    if (sisa_slot <= MEMORY_WARNING_SLOT && sisa_slot > 0){
        printf_P(PSTR("Peringatan: memori hampir penuh\n"));
    }
}

void mulai_program()
{
    Memory_Pool_Inventaris mp;
    int pilihan;

    pilihan = -1;
    in_it_pool(&mp);

    isi_data_dummy(&mp);
    printf_P(PSTR("50 Data dummy berhasil dimuat!\n"));

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
        } else if (pilihan == 0){
            printf_P(PSTR("Program selesai\n"));
        } else{
            printf_P(PSTR("Pilihan tidak valid\n"));
        }
    }
    clear_list(&mp);
}

void cetak_kategori_teks(uint8_t kat) {
    if (kat == KATEG_SENSOR) printf_P(PSTR("Sensor"));
    else if (kat == KATEG_BOARD) printf_P(PSTR("Board"));
    else if (kat == KATEG_MOTOR) printf_P(PSTR("Motor"));
    else if (kat == KATEG_KABEL) printf_P(PSTR("Kabel"));
}

void cetak_lokasi_teks(uint8_t lok) {
    if (lok == LOKASI_LAB_1) printf_P(PSTR("Lab 1"));
    else if (lok == LOKASI_LAB_2) printf_P(PSTR("Lab 2"));
    else if (lok == LOKASI_LAB_3) printf_P(PSTR("Lab 3"));
    else if (lok == LOKASI_LAB_4) printf_P(PSTR("Lab 4"));
}

void isi_data_dummy(Memory_Pool_Inventaris *mp)
{
    Inventaris_Lab dummy;
    int i;

    for (i = 1; i <= 50; i++) {
        dummy.id_barang = (uint16_t)i;
        
        sprintf(dummy.nama, "Brg-%d", i); 
        
        dummy.id_kategori = (uint8_t)(i % 4);
        
        dummy.id_lokasi = (uint8_t)((i / 4) % 4); 
        
        dummy.jumlah_stock = (uint8_t)(i % 10 + 1);
        
        dummy.status_barang = TERSEDIA;
        
        strcpy(dummy.pemilik, "LUIS");
        strcpy(dummy.pic, "Ken");
        
        add_node(mp, dummy);
    }
}