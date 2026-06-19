#include "inventory_final.h"
#include <ctype.h>

void tambahBarang(Block** head) {
    int sisaSlot;
    InventarisLab data;
    Block* tempbarang;

    cekMemori(&sisaSlot);

    if (sisaSlot <= 0) {
        printf_P(PSTR("Memori penuh, barang gagal ditambahkan\n"));
        return;
    }

    bacainputdata(&data);
    searchNode(*head, data.id_barang, &tempbarang);

    if (tempbarang != NULL) {
        printf_P(PSTR("Barang duplikat, ID %u sudah ada di inventaris\n"), data.id_barang);
        return;
    }

    if (data.jumlah_stok == 0 && data.status_barang != HABIS) {
        printf_P(PSTR("Stok 0, Status diubah menjadi Habis\n"));
        data.status_barang = HABIS;
    }

    if (data.jumlah_stok > 0 && data.status_barang == HABIS) {
        printf_P(PSTR("Stok lebih dari 0, Status diubah menjadi Tersedia\n"));
        data.status_barang = TERSEDIA;
    }

    addNode(head, data);
    printf_P(PSTR("Barang berhasil dimasukkan\n"));

    cekMemori(&sisaSlot);
    if (sisaSlot <= MEMORY_WARNING_SLOT && sisaSlot > 0) {
        printf_P(PSTR("Memori hampir penuh, sisa slot %d\n"), sisaSlot);
    }
}

void hapusBarang(Block** head) {
    uint16_t tempid;
    char statusHapus;

    if (*head == NULL) {
        printf_P(PSTR("Inventory Kosong\n"));
        return;
    }

    bacainputID(&tempid);
    deletenode(head, tempid, &statusHapus);

    if (statusHapus == 'G') {
        printf_P(PSTR("Barang tidak ditemukan\n"));
    } else {
        if (statusHapus == 'A') {
            printf_P(PSTR("Node pertama berhasil dihapus\n"));
        } else if (statusHapus == 'T') {
            printf_P(PSTR("Node tengah berhasil dihapus\n"));
        } else if (statusHapus == 'K') {
            printf_P(PSTR("Node akhir berhasil dihapus\n"));
        }
        printf_P(PSTR("Barang berhasil dihapus\n"));
    }
}

void cariBarang(Block* head) {
    uint16_t tempid;
    Block* tempor;

    if (head == NULL) {
        printf_P(PSTR("Inventory Kosong\n"));
        return;
    }

    bacainputID(&tempid);
    searchNode(head, tempid, &tempor);

    if (tempor != NULL) {
        printf_P(PSTR("Barang ditemukan\n"));
        tampilkanDetailBarang(tempor);
    } else {
        printf_P(PSTR("Barang tidak ditemukan\n"));
    }
}

void updateStok(Block* head) {
    uint16_t tempid;
    Block* tempor;
    int tempangka;
    int pilihan;

    if (head == NULL) {
        printf_P(PSTR("Inventory Kosong\n"));
        return;
    }

    bacainputID(&tempid);
    searchNode(head, tempid, &tempor);

    if (tempor == NULL) {
        printf_P(PSTR("ID tidak ada dalam inventory\n"));
        return;
    }

    printf_P(PSTR("Stok sekarang: %u\n"), tempor->active.payload.jumlah_stok);
    printf_P(PSTR("Masukkan jumlah perubahan stok(Maks stok 255):  "));
    scanf("%d", &tempangka);

    if (tempangka < 0) {
        printf_P(PSTR("Jumlah perubahan stok tidak boleh negatif\n"));
        return;
    }

    printf_P(PSTR("Ketik 1 jika ingin menambah dan 2 jika ingin mengurangi: "));
    scanf("%d", &pilihan);

    if (pilihan == 1) {
        if ((int)tempor->active.payload.jumlah_stok + tempangka > 255) {
            printf_P(PSTR("Kapasitas memori stok maksimum adalah 255 unit\n"));
            return;
        }
        tempor->active.payload.jumlah_stok += tempangka;
        printf_P(PSTR("Stok telah ditambahkan\n"));

        if (tempor->active.payload.jumlah_stok > 0 && tempor->active.payload.status_barang == HABIS) {
            tempor->active.payload.status_barang = TERSEDIA;
        }
    } else if (pilihan == 2) {
        if (tempangka > tempor->active.payload.jumlah_stok) {
            printf_P(PSTR("Jumlah pengurangan melebihi stok saat ini\n"));
        } else {
            tempor->active.payload.jumlah_stok -= tempangka;
            printf_P(PSTR("Stok berhasil dikurangi\n"));

            if (tempor->active.payload.jumlah_stok == 0) {
                tempor->active.payload.status_barang = HABIS;
            }
        }
    } else {
        printf_P(PSTR("Pilihan update stok tidak valid\n"));
    }
}

void updateStatus(Block* head) {
    uint16_t tempid;
    char newstats;
    Block* tempor;
    int valid = 0;

    if (head == NULL) {
        printf_P(PSTR("Inventory Kosong\n"));
        return;
    }

    bacainputID(&tempid);
    searchNode(head, tempid, &tempor);

    if (tempor == NULL) {
        printf_P(PSTR("Barang tidak ditemukan\n"));
        return;
    }

    printf_P(PSTR("Status saat ini adalah: "));
    if (tempor->active.payload.status_barang == TERSEDIA) printf_P(PSTR("Tersedia\n"));
    else if (tempor->active.payload.status_barang == DIPINJAM) printf_P(PSTR("Dipinjam\n"));
    else if (tempor->active.payload.status_barang == RUSAK) printf_P(PSTR("Rusak\n"));
    else printf_P(PSTR("Habis\n"));

    printf_P(PSTR("Pilih status (T=Tersedia, D=Dipinjam, R=Rusak, H=Habis): "));
    scanf(" %c", &newstats);
    
    if (newstats == 'T' || newstats == 't') { tempor->active.payload.status_barang = TERSEDIA; valid = 1; }
    else if (newstats == 'D' || newstats == 'd') { tempor->active.payload.status_barang = DIPINJAM; valid = 1; }
    else if (newstats == 'R' || newstats == 'r') { tempor->active.payload.status_barang = RUSAK; valid = 1; }
    else if (newstats == 'H' || newstats == 'h') { tempor->active.payload.status_barang = HABIS; valid = 1; }

    if (valid == 1) {
        if (tempor->active.payload.status_barang == HABIS) {
            tempor->active.payload.jumlah_stok = 0;
        }
        printf_P(PSTR("Status berhasil diperbarui\n"));
    } else {
        printf_P(PSTR("Status tidak valid\n"));
    }
}

void tampilkanData(Block* head) {
    Block* current;
    int i;

    if (head == NULL) {
        printf_P(PSTR("Inventory Kosong\n"));
        return;
    }

    current = head;
    i = 1;

    while (current != NULL) {
        printf_P(PSTR("Item ke-%d\n"), i);
        tampilkanDetailBarang(current);
        current = current->active.next_active;
        i++;
    }
}

void ringkasan(Block* head) {
    int totalJenis = 0, totalStok = 0;
    int countstattersedia = 0, countstatdipinjam = 0;
    int countstathabis = 0, countstatrusak = 0;
    int sisaSlot;
    Block* current;

    if (head == NULL) {
        printf_P(PSTR("Data masih kosong\n"));
        return;
    }

    current = head;

    while (current != NULL) {
        totalJenis++;
        totalStok += current->active.payload.jumlah_stok;

        if (current->active.payload.status_barang == TERSEDIA) countstattersedia++;
        else if (current->active.payload.status_barang == HABIS) countstathabis++;
        else if (current->active.payload.status_barang == DIPINJAM) countstatdipinjam++;
        else if (current->active.payload.status_barang == RUSAK) countstatrusak++;

        current = current->active.next_active;
    }

    cekMemori(&sisaSlot);

    printf_P(PSTR("\nRingkasan Inventaris\n"));
    printf_P(PSTR("Total Jenis Barang: %d\n"), totalJenis);
    printf_P(PSTR("Total Stok Barang : %d\n"), totalStok);
    printf_P(PSTR("Jumlah Tersedia   : %d\n"), countstattersedia);
    printf_P(PSTR("Jumlah Dipinjam   : %d\n"), countstatdipinjam);
    printf_P(PSTR("Jumlah Rusak      : %d\n"), countstatrusak);
    printf_P(PSTR("Jumlah Habis      : %d\n"), countstathabis);
    printf_P(PSTR("Kapasitas Maksimum: %d item\n"), MAX_ITEMS);
    printf_P(PSTR("Sisa Slot Memori  : %d item\n"), sisaSlot);

    if (sisaSlot <= MEMORY_WARNING_SLOT && sisaSlot > 0) {
        printf_P(PSTR("Memori hampir penuh\n"));
    }
}


void mulaiProgram(void) {
    Block* head = NULL; 
    int pilihan = -1;

    initLinkedList(); 


    printf_P(PSTR("\nAuto-Fill 50 Data Dummy\n"));
    for(int i = 1; i <= 50; i++) {
        InventarisLab dummy;
        dummy.id_barang = i;
        strcpy(dummy.nama, "Dummy");
        dummy.id_kategori = 0;
        dummy.jumlah_stok = 10;
        dummy.id_lokasi = 0;
        dummy.status_barang = 1; // Tersedia
        strcpy(dummy.pemilik, "AAA");
        strcpy(dummy.pic, "BBB");

        addNode(&head, dummy);
    }
    printf_P(PSTR("50 Data berhasil dimuat\n"));

    while (pilihan != 0) {
        cetakMenuUtama();
        bacapilmenu(&pilihan);

        if (pilihan == 1) {
            tambahBarang(&head);
        } else if (pilihan == 2) {
            hapusBarang(&head);
        } else if (pilihan == 3) {
            cariBarang(head);
        } else if (pilihan == 4) {
            updateStok(head);
        } else if (pilihan == 5) {
            updateStatus(head);
        } else if (pilihan == 6) {
            tampilkanData(head);
        } else if (pilihan == 7) {
            ringkasan(head);
        } else if (pilihan == 0) {
            printf_P(PSTR("Program selesai\n"));
        } else {
            printf_P(PSTR("Pilihan tidak valid\n"));
        }
    }

    clearList(&head); 
}
