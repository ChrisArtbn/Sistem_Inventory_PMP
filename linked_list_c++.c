#include "inventory_c++.h"

static MemoryPoolInventaris mp_global;

static void ambilNodeKosong(Block** nodeKosong) {
    if (mp_global.freelist == NULL) {
        *nodeKosong = NULL;
        return;
    }
    *nodeKosong = mp_global.freelist;
    mp_global.freelist = mp_global.freelist->next_free;
}

static void lepasNode(Block* node) {
    memset(node, 0, sizeof(Block));
    node->next_free = mp_global.freelist;
    mp_global.freelist = node;
}

void initLinkedList(void) {
    int i;
    mp_global.freelist = NULL;
    mp_global.activelist = NULL;
    for (i = 0; i < MAX_ITEMS; i++) {
        mp_global.pool[i].next_free = mp_global.freelist;
        mp_global.freelist = &mp_global.pool[i];
    }
}

void cekMemori(int* sisaSlot) {
    Block* current = mp_global.freelist;
    *sisaSlot = 0;
    while (current != NULL) {
        (*sisaSlot)++;
        current = current->next_free;
    }
}

void addNode(Block** head, InventarisLab newItem) {
    Block* newNode;
    Block* current;
    ambilNodeKosong(&newNode);

    if (newNode == NULL) {
        printf_P(PSTR("Memori Arduino penuh, node baru gagal dibuat\n"));
        return;
    }

    newNode->active.payload = newItem;
    newNode->active.next_active = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    current = *head;
    while (current->active.next_active != NULL) {
        current = current->active.next_active;
    }
    current->active.next_active = newNode;
}

void searchNode(Block* head, uint16_t id, Block** hasil) {
    Block* current;
    *hasil = NULL;
    current = head;
    while (current != NULL) {
        if (current->active.payload.id_barang == id) {
            *hasil = current;
            return;
        }
        current = current->active.next_active;
    }
}

void deletenode(Block** head, uint16_t id, char* statusHapus) {
    Block* current;
    Block* previous;

    *statusHapus = 'G'; 

    if (*head == NULL) return;

    current = *head;
    previous = NULL;

    while (current != NULL && current->active.payload.id_barang != id) {
        previous = current;
        current = current->active.next_active;
    }

    if (current == NULL) return;

    if (previous == NULL) {
        *head = current->active.next_active; 
        *statusHapus = 'A';
    } else {
        previous->active.next_active = current->active.next_active;
        if (current->active.next_active == NULL) {
            *statusHapus = 'K'; 
        } else {
            *statusHapus = 'T'; 
        }
    }
    lepasNode(current);
}

void clearList(Block** head) {
    Block* current;
    Block* nextNode;
    current = *head;

    while (current != NULL) {
        nextNode = current->active.next_active;
        lepasNode(current); 
        current = nextNode;
    }
    *head = NULL;
}