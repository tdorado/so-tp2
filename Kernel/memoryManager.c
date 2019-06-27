#include <memoryManager.h>
#include <defs.h>
#include <videoDriver.h>

enum STATUS {P = 0, F = 1};

struct nodeADT{ 
    struct nodeADT * next, *prev;
    void * memoryPointer;
    uint64_t size;
    enum STATUS status;
};

typedef struct nodeADT * node_t;

static void *memoryAddress;
static node_t root;
static uint64_t totalMemorySize;


/*función que implementa first fit en la lista de alocación,
**si el espacio que encuentra es más grande que el pedido y hay espacio
**para crear un nuevo nodo, crea un nuevo nodo que apunta a la nueva zona
**de memoria liberada y lo agrega a la lista*/
static node_t findFirstFit(node_t curr, uint64_t size);

//función para liberar un nodo de la lista (de ser necesario, elimilandolo) 
static node_t freeNode(node_t curr, void *memoryPointer);

static void printMemoryNode(node_t curr);


static node_t findFirstFit(node_t curr, uint64_t size){
    if(curr == NULL || size == 0){
        return NULL;
    }
    if(curr->status == F && curr->size == size){
        curr->status = P;
        return curr;
    }
    if(curr->status == F && (curr->size > size)){
        curr->status = P;
        uint64_t dif = curr->size - size;
        //si el espacio que sobra es suficiente, aloca un nuevo nodo
        if(dif > sizeof(struct nodeADT)){
            node_t aux = (node_t)((char *) curr->memoryPointer + size);

            aux->next = curr->next;
            aux->prev = curr;
            aux->memoryPointer = (void *) ((char *) curr->memoryPointer + size + sizeof(struct nodeADT));
            aux->size = dif - sizeof(struct nodeADT);
            aux->status = F;

            curr->size = size;
            curr->next = aux;        
        }
        return curr;
    }
    return findFirstFit(curr->next, size);
}

static node_t freeNode(node_t curr, void *memoryPointer){
    if(curr == NULL || memoryPointer == NULL){
        return NULL; //tal vez separar caso de error
    }
    if(curr->memoryPointer == memoryPointer){ //encontramos el puntero a liberar
        //4 casos de liberación y merge, ver libro de Tanenbaum 4ta edición
        if((curr->prev == NULL || curr->prev->status == P) && (curr->next == NULL || curr->next->status == P)){
            //caso ambos ocupados
            curr->status = F;
            return curr;
        }else if((curr->prev != NULL && curr->prev->status != P) && (curr->next == NULL || curr->next->status == P)){
            //caso libre izq -> se agranda izq
            curr->prev->size += curr->size + sizeof(struct nodeADT);
            if(curr->next != NULL) curr->next->prev = curr->prev;
            return curr->next;
        }else if((curr->prev == NULL || curr->prev->status == P)  && (curr->next != NULL && curr->next->status != P)){
            //caso libre der -> me agrando yo
            curr->status = F;
            curr->size += curr->next->size + sizeof(struct nodeADT);
            if(curr->next->next != NULL) curr->next->next->prev = curr;
            curr->next = curr->next->next;            
            return curr;
        }else if((curr->prev != NULL && curr->prev->status != P)  && (curr->next != NULL && curr->next->status != P)){
            //caso libre de ambos lados -> se agranda la izq el tamaño mío más el de la derecha
            curr->prev->size += curr->size + curr->next->size + 2 * sizeof(struct nodeADT);
            if(curr->next->next != NULL){
                curr->next->next->prev = curr->prev;
                return curr->next->next;
            }
            return NULL;
        }
    }
    curr->next = freeNode(curr->next, memoryPointer);
    return curr;
}

void loadMemoryManager(void *memoryAddressLocation, uint64_t totalSize){
    totalMemorySize = totalSize;
    root = memoryAddress = memoryAddressLocation;

    root->next = NULL;
    root->prev = NULL;
    root->memoryPointer =  (void *) ((char *)memoryAddressLocation + sizeof(struct nodeADT));
    root->size = totalSize - sizeof(struct nodeADT);
    root->status = F;
}

void *memoryBaseAddress(){
    return memoryAddress;
}

void *malloc(uint64_t size){
    node_t ret = findFirstFit(root, size);
    if(ret == NULL){
        return NULL;
    }
    return ret->memoryPointer;
}

void free(void *memoryPointer){
    root = freeNode(root, memoryPointer);
    if(root == NULL){
        printStringError("Error: free memory manager.\n");
    }
}

void printMemoryNodes(){
    printMemoryNode(root);
}

static void printMemoryNode(node_t curr){
    if(curr == NULL){
        return;
    }

    printString("Memory Pointer: ");
    printDec((uint64_t)curr->memoryPointer);
    printString(" Status: ");
    if(curr->status == P){
        printString("USED");
    }
    else{
        printString("FREE");
    }
    printString(" Size: ");
    printDec(curr->size);
    printString("\n");

    printMemoryNode(curr->next);
}