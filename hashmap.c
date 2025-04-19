#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    if (map->size >= map->capacity * 0.7) {
        enlarge(map);
    }

    long i = hash(key, map->capacity);
    long original = i;

    while (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
        if (is_equal(map->buckets[i]->key, key)) return;  
        i = (i + 1) % map->capacity;
        if (i == original) return; 
    }

    Pair * newPair = createPair(strdup(key), value);
    map->buckets[i] = newPair;
    map->size++;
    map->current = i;
}


void enlarge(HashMap * map) {

    Pair ** bucketsAnt = map->buckets;
    long capacityAnt = map->capacity;

    map->capacity *= 2;
    map->buckets = (Pair**)calloc(map->capacity, sizeof(Pair*));
    map->size = 0;

    for(int i = 0 ; i < capacityAnt ; i++) {
        if (bucketsAnt[i] && bucketsAnt[i]->key) {
            insertMap(map, bucketsAnt[i]->key, bucketsAnt[i]->value);
            free(bucketsAnt[i]);
        }
    }
    free(bucketsAnt);

    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
    HashMap * map = (HashMap *)malloc(sizeof(HashMap));
    if (map == NULL) return NULL;

    map->capacity = capacity;
    map->size = 0;
    map->current = -1;
    map->buckets = (Pair **) calloc(capacity, sizeof(Pair*));


    return map;
}

void eraseMap(HashMap * map,  char * key) {
    long i = hash(key, map->capacity);
    long inicio = i;
    
    while (map->buckets[i]) {
        if (map->buckets[i]->key && is_equal(map->buckets[i]->key, key)) {
            map->buckets[i]->key = NULL;
            map->size--;
            return;
        }
        i = (i + 1) % map->capacity;
        if (i == inicio) break;

    }


}

Pair * searchMap(HashMap * map,  char * key) { 
    long i = hash(key, map->capacity);
    long inicio = i;
    
    while (map->buckets[i]) {
        if (map->buckets[i]->key && is_equal(map->buckets[i]->key, key)) {
            map->current = i;
            return map->buckets[i];    
        }
        i = (i + 1) % map->capacity;
        if (i == inicio) break;
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
