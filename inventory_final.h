#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ITEMS 20
#define MEMORY_WARNING_SLOT 3

#define ID_LEN 50
#define NAMA_LEN 100
#define KATEGORI_LEN 50
#define LOKASI_LEN 100
#define PEMILIK_LEN 100
#define PIC_LEN 100
#define LINE_LEN 512

typedef struct {
    char id[ID_LEN];
    char nama[NAMA_LEN];
    char kategori[KATEGORI_LEN];
    int stok;
    char lokasi[LOKASI_LEN];
    char status;
    char pemilik[PEMILIK_LEN];
    char pic[PIC_LEN];
} InventoryItem;

typedef struct Node {
    InventoryItem data;
    struct Node* next;
} Node;

void mulaiProgram(void);

void initLinkedList(void);
void addNode(Node** head, InventoryItem newItem);
void searchNode(Node* head, const char* id, Node** hasil);
void deletenode(Node** head, const char* id, char* statusHapus);
void clearList(Node** head);
void cekMemori(int* sisaSlot);

void cetakMenuUtama(void);
void bacapilmenu(int *pilihan);
void bacainputdata(char *id, char *nama, char *kategori, int *stok, char *lokasi, char *status, char *pemilik, char *pic);
void bacainputID(char *id);
void bacainputstoknew(int *stokBaru);
void bacainputstatusnew(char *statusBaru);
void tampilkanDetailBarang(Node* current);
void muatDariFile(Node** head, const char* nama_file);
void simpanKeFile(Node* head, const char* nama_file);

void tambahBarang(Node** head);
void hapusBarang(Node** head);
void cariBarang(Node* head);
void updateStok(Node* head);
void updateStatus(Node* head);
void tampilkanData(Node* head);
void ringkasan(Node* head);
void normalisasiStatus(char* status);
void validasiStatus(char status, int* valid);

#endif
