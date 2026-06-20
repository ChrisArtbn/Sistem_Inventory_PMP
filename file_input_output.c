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
    printf("8. Muat Data dari File\n");
    printf("9. Simpan Data ke File\n");
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

void muat_file(Memory_Pool_Inventaris *mp, const char* nama_file)
{
    FILE* file;
    char line[LINE_LEN];
    char* token;
    Inventaris_Lab data;
    Block* hasil;
    int field;
    int valid;

    file = fopen(nama_file, "r");

    if (file == NULL){
        printf("File %s tidak ditemukan\n", nama_file);
        return;
    }

    while (fgets(line, LINE_LEN, file) != NULL){
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\0'){
            continue;
        }
        memset(&data, 0, sizeof(Inventaris_Lab));

        field = 0;
        token = strtok(line, "|\n");

        while (token != NULL){
            if (field == 0) {
                data.id_barang = (uint16_t)atoi(token);
            } else if (field == 1){
                strncpy(data.nama, token, sizeof(data.nama) - 1);
            } else if (field == 2){
                data.id_kategori = (uint8_t)atoi(token);
            } else if (field == 3){
                data.jumlah_stock = (uint8_t)atoi(token);
            } else if (field == 4){
                data.id_lokasi = (uint8_t)atoi(token);
            } else if (field == 5){
                char temp_status = token[0];
                if (temp_status == 'T') data.status_barang = TERSEDIA;
                else if (temp_status == 'D') data.status_barang = DIPINJAM;
                else if (temp_status == 'R') data.status_barang = RUSAK;
                else if (temp_status == 'H') data.status_barang = HABIS;
            } else if (field == 6){
                strncpy(data.pemilik, token, sizeof(data.pemilik) - 1);
            } else if (field == 7){
                strncpy(data.pic, token, sizeof(data.pic) - 1);
            }

            field++;
            token = strtok(NULL, "|\n");
        }

        if (field != 8){
            printf("Format baris tidak valid, data dilewati\n");
        } 
        else{
            char status_char = 'H';
            if (data.status_barang == TERSEDIA) status_char = 'T';
            else if (data.status_barang == DIPINJAM) status_char = 'D';
            else if (data.status_barang == RUSAK) status_char = 'R';

            void normalisasi_status(char* status);
            void validasi_status(char status, int* valid);

            normalisasi_status(&status_char);
            validasi_status(status_char, &valid);
            hasil = cari_node(mp, data.id_barang);

            if (hasil != NULL){
                printf("ID %u duplikat, data dari file akan dilewati\n", data.id_barang);
            } else if (valid == 0){
                printf("Status ID %u tidak valid, data akan dilewati\n", data.id_barang);
            } else if (data.jumlah_stock < 0){
                printf("Stok ID %u tidak valid, data akan dilewati\n", data.id_barang);
            } else{
                if (data.jumlah_stock == 0){
                    data.status_barang = HABIS;
                }
                add_node(mp, data);
            }
        }
    }
    fclose(file);
    printf("Proses memuat data dari %s selesai\n", nama_file);
}

void simpan_ke_file(Memory_Pool_Inventaris *mp, const char* nama_file)
{
    FILE* file;
    Block* current;

    file = fopen(nama_file, "w");

    if (file == NULL){
        printf("File %s gagal dibuat\n", nama_file);
        return;
    }

    fprintf(file, "# ID|Nama|Kategori|Stok|Lokasi|Status|Pemilik|PIC\n");

    current = mp->activelist;

    while (current != NULL){
        char status_char = 'H';
        if (current->active.payload.status_barang == TERSEDIA) status_char = 'T';
        else if (current->active.payload.status_barang == DIPINJAM) status_char = 'D';
        else if (current->active.payload.status_barang == RUSAK) status_char = 'R';

        fprintf(file, "%u|%s|%d|%d|%d|%c|%s|%s\n",
                current->active.payload.id_barang,
                current->active.payload.nama,
                current->active.payload.id_kategori,
                current->active.payload.jumlah_stock,
                current->active.payload.id_lokasi,
                status_char,
                current->active.payload.pemilik,
                current->active.payload.pic);
        current = current->active.next_active;
    }
    fclose(file);
    printf("Data berhasil disimpan ke %s\n", nama_file);
}
