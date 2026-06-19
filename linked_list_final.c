#include "sistem_inventory.h"

static Node nodePool[MAX_ITEMS];
static int nodeDipakai[MAX_ITEMS];

static void ambilNodeKosong(Node** nodeKosong)
{
    int i;

    *nodeKosong = NULL;

    for (i = 0; i < MAX_ITEMS; i++) {
        if (nodeDipakai[i] == 0) {
            nodeDipakai[i] = 1;
            nodePool[i].next = NULL;
            memset(&nodePool[i].data, 0, sizeof(InventoryItem));
            *nodeKosong = &nodePool[i];
            return;
        }
    }
}

static void lepasNode(Node* node)
{
    int i;

    for (i = 0; i < MAX_ITEMS; i++) {
        if (&nodePool[i] == node) {
            nodeDipakai[i] = 0;
            nodePool[i].next = NULL;
            memset(&nodePool[i].data, 0, sizeof(InventoryItem));
            return;
        }
    }
}

void initLinkedList(void)
{
    int i;

    for (i = 0; i < MAX_ITEMS; i++) {
        nodeDipakai[i] = 0;
        nodePool[i].next = NULL;
        memset(&nodePool[i].data, 0, sizeof(InventoryItem));
    }
}

void cekMemori(int* sisaSlot)
{
    int i;

    *sisaSlot = 0;

    for (i = 0; i < MAX_ITEMS; i++) {
        if (nodeDipakai[i] == 0) {
            (*sisaSlot)++;
        }
    }
}

void addNode(Node** head, InventoryItem newItem)
{
    Node* newNode;
    Node* current;

    ambilNodeKosong(&newNode);

    if (newNode == NULL) {
        printf("Memori penuh, node baru gagal dibuat\n");
        return;
    }

    newNode->data = newItem;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    current = *head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNode;
}

void searchNode(Node* head, const char* id, Node** hasil)
{
    Node* current;

    *hasil = NULL;
    current = head;

    while (current != NULL) {
        if (strcmp(current->data.id, id) == 0) {
            *hasil = current;
            return;
        }

        current = current->next;
    }
}

void deletenode(Node** head, const char* id, char* statusHapus)
{
    Node* current;
    Node* previous;

    *statusHapus = 'G';

    if (*head == NULL) {
        return;
    }

    current = *head;
    previous = NULL;

    while (current != NULL && strcmp(current->data.id, id) != 0) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return;
    }

    if (previous == NULL) {
        *head = current->next;
        *statusHapus = 'A';
    } else {
        previous->next = current->next;

        if (current->next == NULL) {
            *statusHapus = 'K';
        } else {
            *statusHapus = 'T';
        }
    }

    lepasNode(current);
}

void clearList(Node** head)
{
    Node* current;
    Node* nextNode;

    current = *head;

    while (current != NULL) {
        nextNode = current->next;
        lepasNode(current);
        current = nextNode;
    }

    *head = NULL;
}
