#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdint.h>

/*función para inicializar la memoria dado un puntero inicial y el tamaño
**total de la memoria*/
void loadMemoryManager(void *memoryAddressLocation, uint64_t totalSize);

/*función que llama a find_first_fit con el nodo raíz y 
**retorna el puntero del nodo adecuado*/
void *malloc(uint64_t size);

//función que llama a liberar el nodo que tenga el puntero apuntando a ptr
void free(void *memoryPointer);

//devuelve un puntero al inicio de la memoria
void *memoryBaseAddress();

void printMemoryNodes();

#endif /* MEMORY_MANAGER_H */