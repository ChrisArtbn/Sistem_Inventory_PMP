#include "sistem_inventory.h"

void in_it_pool(Memory_Pool_Inventaris *mp)
{
    int i;
    mp->activelist = NULL;
    mp->freelist = NULL;

    for (i = MAX_ITEMS - 1; i >= 0; i--){
        memset(&mp->pool[i].active.payload, 0, sizeof(Inventaris_Lab));
        mp->pool[i].next_free = mp->freelist;
        mp->freelist = &mp->pool[i];
    }
}

void cek_memori(Memory_Pool_Inventaris *mp, int* sisa_slot)
{
    int count = 0;
    Block* current = mp->freelist;
    while (current != NULL){
        count++;
        current = current->next_free;
    }
    *sisa_slot = count;
}

void add_node(Memory_Pool_Inventaris *mp, Inventaris_Lab new_item)
{
    Block* new_node;
    Block* current;

    if (mp->freelist == NULL){
        printf("Memori penuh, node baru gagal dibuat\n");
        return;
    }

    new_node = mp->freelist;
    mp->freelist = new_node->next_free;

    new_node->active.payload = new_item;
    new_node->active.next_active = NULL;

    if (mp->activelist == NULL){
        mp->activelist = new_node;
        return;
    }
    current = mp->activelist;

    while (current->active.next_active != NULL){
        current = current->active.next_active;
    }
    current->active.next_active = new_node;
}

Block* cari_node(Memory_Pool_Inventaris *mp, uint16_t id)
{
    Block* current = mp->activelist;

    while (current != NULL){
        if (current->active.payload.id_barang == id){
            return current;
        }
        current = current->active.next_active;
    }
    return NULL;
}

void delete_node(Memory_Pool_Inventaris *mp, uint16_t id, char* status_hapus)
{
    Block* current;
    Block* previous;

    *status_hapus = 'G';

    if (mp->activelist == NULL){
        return;
    }

    current = mp->activelist;
    previous = NULL;

    while (current != NULL && current->active.payload.id_barang != id){
        previous = current;
        current = current->active.next_active;
    }

    if (current == NULL){
        return;
    }

    if (previous == NULL){
        mp->activelist = current->active.next_active;
        *status_hapus = 'A';
    } 
    else{
        previous->active.next_active = current->active.next_active;

        if (current->active.next_active == NULL){
            *status_hapus = 'K';
        } 
        else{
            *status_hapus = 'T';
        }
    }
    
    memset(&current->active.payload, 0, sizeof(Inventaris_Lab));
    current->next_free = mp->freelist;
    mp->freelist = current;
}

void clear_list(Memory_Pool_Inventaris *mp)
{
    Block* current = mp->activelist;
    Block* next_node;

    while (current != NULL){
        next_node = current->active.next_active;
        memset(&current->active.payload, 0, sizeof(Inventaris_Lab));
        current->next_free = mp->freelist;
        mp->freelist = current;
        current = next_node;
    }
    mp->activelist = NULL;
}
