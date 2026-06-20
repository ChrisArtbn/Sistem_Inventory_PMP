#include "sistem_inventory.h"

static Node node_pool[MAX_ITEMS];
static int node_dipakai[MAX_ITEMS];

static void ambil_node_kosong(Node** node_kosong){
    int i;
    *node_kosong = NULL;

    for (i = 0; i < MAX_ITEMS; i++){
        if (node_dipakai[i] == 0) {
            node_dipakai[i] = 1;
            node_pool[i].next = NULL;

            memset(&node_pool[i].data, 0, sizeof(inventory_item));
            *node_kosong = &node_pool[i];
            return;
        }
    }
}

static void lepas_node(Node* node){
    int i;

    for (i = 0; i < MAX_ITEMS; i++){
        if (&node_pool[i] == node){
            node_dipakai[i] = 0;
            node_pool[i].next = NULL;

            memset(&node_pool[i].data, 0, sizeof(inventory_item));
            return;
        }
    }
}

void init_linked_list(void){
    int i;

    for (i = 0; i < MAX_ITEMS; i++){
        node_dipakai[i] = 0;
        node_pool[i].next = NULL;

        memset(&node_pool[i].data, 0, sizeof(inventory_item));
    }
}

void cek_memori(int* sisa_slot){
    int i;
    *sisa_slot = 0;

    for (i = 0; i < MAX_ITEMS; i++){
        if (node_dipakai[i] == 0){
            (*sisa_slot)++;
        }
    }
}

void add_node(Node** head, inventory_item new_item){
    Node* new_node;
    Node* current;

    ambil_node_kosong(&new_node);

    if (new_node == NULL){
        printf("Memori penuh, node baru gagal dibuat\n");
        return;
    }
    new_node->data = new_item;
    new_node->next = NULL;

    if (*head == NULL){
        *head = new_node;
        return;
    }
    current = *head;

    while (current->next != NULL){
        current = current->next;
    }
    current->next = new_node;
}

void search_node(Node* head, const char* id, Node** hasil)
{
    Node* current;

    *hasil = NULL;
    current = head;

    while (current != NULL){
        if (strcmp(current->data.id, id) == 0){
            *hasil = current;
            return;
        }
        current = current->next;
    }
}

void delete_node(Node** head, const char* id, char* status_hapus)
{
    Node* current;
    Node* previous;

    *status_hapus = 'G';

    if (*head == NULL){
        return;
    }

    current = *head;
    previous = NULL;

    while (current != NULL && strcmp(current->data.id, id) != 0){
        previous = current;
        current = current->next;
    }

    if (current == NULL){
        return;
    }

    if (previous == NULL){
        *head = current->next;
        *status_hapus = 'A';
    } 
    else{
        previous->next = current->next;

        if (current->next == NULL){
            *status_hapus = 'K';
        } 
        else{
            *status_hapus = 'T';
        }
    }
    lepas_node(current);
}

void clear_list(Node** head)
{
    Node* current;
    Node* next_node;

    current = *head;

    while (current != NULL) {
        next_node = current->next;
        lepas_node(current);
        current = next_node;
    }
    *head = NULL;
}
