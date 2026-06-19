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

void tambah_barang(Node** head)
{
    int sisa_slot;
    int valid;
    inventory_item data;
    Node* tempbarang;

    cek_memori(&sisa_slot);

    if (sisa_slot <= 0){
        printf("Memori penuh, barang gagal ditambahkan\n");
        return;
    }

    printf("Masukkan ID : ");
    scanf(" %49s", data.id);

    search_node(*head, data.id, &tempbarang);

    if (tempbarang != NULL){
        printf("Barang ini duplikat, ID sudah ada di inventaris\n");
        return;
    }

    printf("Masukkan Nama Barang : ");
    scanf(" %99[^\n]", data.nama);

    printf("Masukkan Lokasi : ");
    scanf(" %99[^\n]", data.lokasi);

    printf("Masukkan Kategori : ");
    scanf(" %49[^\n]", data.kategori);

    printf("Masukkan Jumlah Stok : ");
    scanf("%d", &data.stok);

    if (data.stok < 0){
        printf("Stok tidak boleh negatif\n");
        return;
    }

    printf("Masukkan Status Barang (T=tersedia, D=dipinjam, R=rusak, H=habis) : ");
    scanf(" %c", &data.status);

    normalisasi_status(&data.status);
    validasi_status(data.status, &valid);

    if (valid == 0) {
        printf("Status tidak valid\n");
        return;
    }

    printf("Masukkan Nama Pemilik : ");
    scanf(" %99[^\n]", data.pemilik);

    printf("Masukkan Nama PIC : ");
    scanf(" %99[^\n]", data.pic);

    if (data.stok == 0 && data.status != 'H'){
        printf("Stok 0, status otomatis diubah menjadi Habis\n");
        data.status = 'H';
    }

    if (data.stok > 0 && data.status == 'H'){
        printf("Stok lebih dari 0, status otomatis diubah menjadi Tersedia\n");
        data.status = 'T';
    }

    add_node(head, data);

    printf("Barang berhasil dimasukkan\n");

    cek_memori(&sisa_slot);

    if (sisa_slot <= MEMORY_WARNING_SLOT && sisa_slot > 0){
        printf("Peringatan: memori hampir penuh, sisa slot %d\n", sisa_slot);
    }
}

void hapus_barang(Node** head)
{
    char tempid[ID_LEN];
    char status_hapus;

    if (*head == NULL){
        printf("Inventory Kosong\n");
        return;
    }

    printf("Masukkan ID barang yang mau dihapus: ");
    scanf(" %49s", tempid);

    delete_node(head, tempid, &status_hapus);

    if (status_hapus == 'G'){
        printf("Barang tidak ditemukan\n");
    } else {
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

void cari_barang(Node* head)
{
    char tempid[ID_LEN];
    Node* tempor;

    if (head == NULL){
        printf("Inventory Kosong\n");
        return;
    }
    printf("Masukkan id: ");
    scanf(" %49s", tempid);

    search_node(head, tempid, &tempor);

    if (tempor != NULL){
        printf("Barang ditemukan\n");
        tampilkan_detail_barang(tempor);
    } else {
        printf("Barang tidak ditemukan\n");
    }
}

void update_stock(Node* head)
{
    char tempid[ID_LEN];
    Node* tempor;
    int tempangka;
    int pilihan;

    if (head == NULL){
        printf("Inventory Kosong\n");
        return;
    }

    printf("Masukkan id barang : ");
    scanf(" %49s", tempid);

    search_node(head, tempid, &tempor);

    if (tempor == NULL){
        printf("ID tidak ada dalam inventory\n");
        return;
    }

    printf("Stock sekarang: %d\n", tempor->data.stok);

    printf("Mau berubah berapa banyak kang? ");
    scanf("%d", &tempangka);

    if (tempangka < 0){
        printf("Jumlah perubahan stok tidak boleh negatif\n");
        return;
    }

    printf("Ketik 1 jika ingin menambah dan 2 jika ingin mengurangi: ");
    scanf("%d", &pilihan);

    if (pilihan == 1){
        tempor->data.stok += tempangka;
        printf("Stok telah ditambahkan\n");

        if (tempor->data.stok > 0 && tempor->data.status == 'H') {
            tempor->data.status = 'T';
        }
    } else if (pilihan == 2){
        if (tempangka > tempor->data.stok){
            printf("Jumlah melebihi stok saat ini\n");
        } else{
            tempor->data.stok -= tempangka;
            printf("Stok berhasil dikurangi\n");

            if (tempor->data.stok == 0) {
                tempor->data.status = 'H';
            }
        }
    } else{
        printf("Pilihan update stok tidak valid\n");
    }
}

void update_status(Node* head)
{
    char tempid[ID_LEN];
    char new_stats;
    int valid;
    Node* tempor;

    if (head == NULL){
        printf("Inventory Kosong\n");
        return;
    }
    printf("Masukkan id barang: ");
    scanf(" %49s", tempid);

    search_node(head, tempid, &tempor);

    if (tempor == NULL){
        printf("Barang tidak ditemukan\n");
        return;
    }
    printf("Status saat ini adalah : %c\n", tempor->data.status);
    printf("Pilih status (T=tersedia, D=dipinjam, R=rusak, H=habis) : ");
    scanf(" %c", &new_stats);

    normalisasi_status(&new_stats);
    validasi_status(new_stats, &valid);

    if (valid == 1){
        tempor->data.status = new_stats;

        if (new_stats == 'H'){
            tempor->data.stok = 0;
        }
        printf("Status berhasil diperbarui\n");
    } else {
        printf("Status tidak valid\n");
    }
}

void tampilkan_data(Node* head)
{
    Node* current;
    int i;

    if (head == NULL){
        printf("Inventory Kosong\n");
        return;
    }

    current = head;
    i = 1;

    while (current != NULL){
        printf("Item ke-%d\n", i);
        tampilkan_detail_barang(current);
        current = current->next;
        i++;
    }
}

void ringkasan(Node* head)
{
    int total_jenis;
    int total_stok;
    int count_stat_tersedia;
    int count_stat_dipinjam;
    int count_stat_habis;
    int count_stat_rusak;
    int sisa_slot;
    Node* current;

    if (head == NULL){
        printf("Data kosong\n");
        return;
    }

    total_jenis = 0;
    total_stok = 0;
    count_stat_tersedia = 0;
    count_stat_dipinjam = 0;
    count_stat_habis = 0;
    count_stat_rusak = 0;

    current = head;

    while (current != NULL){
        total_jenis++;
        total_stok += current->data.stok;

        if (current->data.status == 'T'){
            count_stat_tersedia++;
        } else if (current->data.status == 'H'){
            count_stat_habis++;
        } else if (current->data.status == 'D'){
            count_stat_dipinjam++;
        } else if (current->data.status == 'R'){
            count_stat_rusak++;
        }
        current = current->next;
    }

    cek_memori(&sisa_slot);

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
    Node* head;
    int pilihan;

    head = NULL;
    pilihan = -1;

    init_linked_list();

    while (pilihan != 0){
        cetak_menu_utama();
        baca_pilihan_menu(&pilihan);

        if (pilihan == 1){
            tambah_barang(&head);
        } else if (pilihan == 2){
            hapus_barang(&head);
        } else if (pilihan == 3){
            cari_barang(head);
        } else if (pilihan == 4){
            update_stok(head);
        } else if (pilihan == 5){
            update_status(head);
        } else if (pilihan == 6){
            tampilkan_data(head);
        } else if (pilihan == 7){
            ringkasan(head);
        } else if (pilihan == 8){
            muat_dari_file(&head, "input.txt");
        } else if (pilihan == 9){
            simpan_ke_file(head, "output.txt");
        } else if (pilihan == 0){
            printf("Program selesai\n");
        } else{
            printf("Pilihan tidak valid\n");
        }
    }
    clear_list(&head);
}
