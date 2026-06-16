#include "inventory_final.h"

static void clearInputBuffer(void)
{
    int c;
    c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

void cetakMenuUtama(void)
{
    printf("SISTEM INVENTARIS DENGAN ARDUINO UNO\n");
    printf("Berikut pilihan yang tersedia:\n");
    printf("1. Tambah Data Barang\n");
    printf("2. Hapus Data Barang\n");
    printf("3. Cari Data Barang berdasarkan ID\n");
    printf("4. Perbarui Stok Barang\n");
    printf("5. Perbarui Status Barang\n");
    printf("6. Tampilkan Seluruh Data Inventaris\n");
    printf("7. Tampilkan Ringkasan Inventaris\n");
    printf("8. Muat data dari input.txt\n");
    printf("9. Simpan data ke output.txt\n");
    printf("Silakan masukkan pilihan Anda (0 untuk keluar): ");
}

void bacapilmenu(int *pilihan)
{
    if (scanf("%d", pilihan) != 1) {
        *pilihan = -1;
        clearInputBuffer();
    }
}

void bacainputdata(char *id, char *nama, char *kategori, int *stok, char *lokasi, char *status, char *pemilik, char *pic)
{
    printf("Masukkan ID: ");
    scanf(" %49s", id);

    printf("Masukkan Nama Barang: ");
    scanf(" %99[^\n]", nama);

    printf("Masukkan Kategori Barang: ");
    scanf(" %49[^\n]", kategori);

    printf("Masukkan Jumlah Stok Awal: ");
    scanf("%d", stok);

    printf("Masukkan Lokasi Fisik: ");
    scanf(" %99[^\n]", lokasi);

    printf("Masukkan Status Barang (T/D/R/H): ");
    scanf(" %c", status);

    printf("Masukkan Pemilik Barang: ");
    scanf(" %99[^\n]", pemilik);

    printf("Masukkan PIC: ");
    scanf(" %99[^\n]", pic);
}

void bacainputID(char *id)
{
    printf("Masukkan ID: ");
    scanf(" %49s", id);
}

void bacainputstoknew(int *stokBaru)
{
    printf("Masukkan Jumlah Stok: ");
    scanf("%d", stokBaru);
}

void bacainputstatusnew(char *statusBaru)
{
    printf("Masukkan Status (T/D/R/H): ");
    scanf(" %c", statusBaru);
}

void tampilkanDetailBarang(Node* current)
{
    if (current == NULL) {
        printf("Data masih kosong\n");
        return;
    }
    printf("ID Barang     : %s\n", current->data.id);
    printf("Nama Komponen : %s\n", current->data.nama);
    printf("Kategori      : %s\n", current->data.kategori);
    printf("Lokasi Fisik  : %s\n", current->data.lokasi);
    printf("Jumlah Stok   : %d unit\n", current->data.stok);
    printf("Pemilik       : %s\n", current->data.pemilik);
    printf("PIC           : %s\n", current->data.pic);

    if (current->data.status == 'T') {
        printf("Status        : Tersedia\n");
    } else if (current->data.status == 'D') {
        printf("Status        : Dipinjam\n");
    } else if (current->data.status == 'R') {
        printf("Status        : Rusak\n");
    } else if (current->data.status == 'H') {
        printf("Status        : Habis\n");
    } else {
        printf("Status        : Kondisi tidak diketahui (%c)\n", current->data.status);
    }

    printf("----------------------------------------\n");
}

void muatDariFile(Node** head, const char* nama_file)
{
    FILE* file;
    char line[LINE_LEN];
    char* token;
    InventoryItem data;
    Node* hasil;
    int field;
    int valid;

    file = fopen(nama_file, "r");

    if (file == NULL) {
        printf("File %s tidak ditemukan\n", nama_file);
        return;
    }

    while (fgets(line, LINE_LEN, file) != NULL) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\0') {
            continue;
        }

        memset(&data, 0, sizeof(InventoryItem));

        field = 0;
        token = strtok(line, "|\n");

        while (token != NULL) {
            if (field == 0) {
                strncpy(data.id, token, ID_LEN - 1);
            } else if (field == 1) {
                strncpy(data.nama, token, NAMA_LEN - 1);
            } else if (field == 2) {
                strncpy(data.kategori, token, KATEGORI_LEN - 1);
            } else if (field == 3) {
                data.stok = atoi(token);
            } else if (field == 4) {
                strncpy(data.lokasi, token, LOKASI_LEN - 1);
            } else if (field == 5) {
                data.status = token[0];
            } else if (field == 6) {
                strncpy(data.pemilik, token, PEMILIK_LEN - 1);
            } else if (field == 7) {
                strncpy(data.pic, token, PIC_LEN - 1);
            }

            field++;
            token = strtok(NULL, "|\n");
        }

        if (field != 8) {
            printf("Format baris tidak valid, data dilewati\n");
        } else {
            normalisasiStatus(&data.status);
            validasiStatus(data.status, &valid);
            searchNode(*head, data.id, &hasil);

            if (hasil != NULL) {
                printf("ID %s duplikat, data dari file dilewati\n", data.id);
            } else if (valid == 0) {
                printf("Status ID %s tidak valid, data dilewati\n", data.id);
            } else if (data.stok < 0) {
                printf("Stok ID %s tidak valid, data dilewati\n", data.id);
            } else {
                if (data.stok == 0) {
                    data.status = 'H';
                }

                addNode(head, data);
            }
        }
    }

    fclose(file);
    printf("Proses muat data dari %s selesai\n", nama_file);
}

void simpanKeFile(Node* head, const char* nama_file)
{
    FILE* file;
    Node* current;

    file = fopen(nama_file, "w");

    if (file == NULL) {
        printf("File %s gagal dibuat\n", nama_file);
        return;
    }

    fprintf(file, "# ID|Nama|Kategori|Stok|Lokasi|Status|Pemilik|PIC\n");

    current = head;

    while (current != NULL) {
        fprintf(file, "%s|%s|%s|%d|%s|%c|%s|%s\n",
                current->data.id,
                current->data.nama,
                current->data.kategori,
                current->data.stok,
                current->data.lokasi,
                current->data.status,
                current->data.pemilik,
                current->data.pic);

        current = current->next;
    }

    fclose(file);
    printf("Data berhasil disimpan ke %s\n", nama_file);
}
