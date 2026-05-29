#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[50];
    char nama[100];
    char kategori[50];
    int stok;
    char lokasi[100];
    char status[50];
    char pemilik[100];
    char pic[100];
} InventoryItem;

typedef struct Node {
    InventoryItem data;
    struct Node* next;
} Node;

// void tambahNode(Node** head, InventoryItem newItem);
// void hapusNode(Node** head, const char* id);
// void bersihkanMemori(Node** head);

// void tambahData(Node** head, InventoryItem newItem);
// void hapusData(Node** head, const char* id);
// void cariData(const Node* head, const char* id);
// void perbaruiStok(Node* head, const char* id, int perubahan_stok);
// void perbaruiStatus(Node* head, const char* id, const char* status_baru);
// void tampilkanSemuaData(const Node* head);
// void tampilkanRingkasan(const Node* head);

// void muatDariFile(Node** head, const char* nama_file);
// void simpanKeFile(const Node* head, const char* nama_file);

#endif