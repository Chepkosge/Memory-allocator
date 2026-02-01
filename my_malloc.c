#include <stdlib.h>
#include <unistd.h>

struct chunk{
    int size;
    void* next;
};

// Forward declaration
void add(void** list, int type, void* new, int num_chunks);

void* list_32 = NULL;
void* list_64 = NULL;
void* list_128 = NULL;
void* list_256 = NULL;
void* list_512 = NULL;
void* list_1024 = NULL;

void add(void** list, int type, void* new, int num_chunks){
    struct chunk* temp = NULL;
    if(*list){
        temp = (struct chunk*)*list - 1;
        while((temp -> next) != NULL){
            temp = temp -> next;
            temp = temp - 1;
        }
        temp -> next = (struct chunk*)new + 1;
    }
    else{
        *list = (struct chunk*)new + 1;
        temp = (struct chunk*)new;
        temp -> next = *list;
    }
    for(int i = 0; i < num_chunks; i++){
        temp = temp -> next;
        temp = temp - 1;
        temp -> size = type;
        temp -> next = (i < (num_chunks - 1)) ? ((char*)temp + (sizeof(struct chunk) + type)) : NULL;
    }
}

void* malloc(size_t size){
    if(size == 0){
        return NULL;
    }

    void* memory = NULL;
    
    if(size <= 20){
        if(list_32){
            memory = list_32;
            struct chunk* temp = (struct chunk*)list_32 - 1;
            list_32 = temp -> next;
        }
        else if(list_64){
            struct chunk* temp = (struct chunk*)list_64 - 1;
            list_64 = temp -> next;
            add(&list_32, 32, temp, 2);
            memory = list_32;
            temp = (struct chunk*)list_32 - 1;
            list_32 = temp -> next;
        }
        else if(list_128){
            struct chunk* temp = (struct chunk*)list_128 - 1;
            list_128 = temp -> next;
            add(&list_32, 32, temp, 2);
            temp = (struct chunk*)((char*)temp + 64);
            add(&list_64, 64, temp, 1);
            memory = list_32;
            temp = (struct chunk*)list_32 - 1;
            list_32 = temp -> next;
        }
        else if(list_256){
            struct chunk* temp = (struct chunk*)list_256 - 1;
            list_256 = temp -> next;
            add(&list_32, 32, temp, 2);
            temp = (struct chunk*)((char*)temp + 64);
            add(&list_64, 64, temp, 1);
            temp = (struct chunk*)((char*)temp + 64);
            add(&list_128, 128, temp, 1);
            memory = list_32;
            temp = (struct chunk*)list_32 - 1;
            list_32 = temp -> next;
        }
        else if(list_512){
            struct chunk* temp = (struct chunk*)list_512 - 1;
            list_512 = temp -> next;
            add(&list_32, 32, temp, 2);
            temp = (struct chunk*)((char*)temp + 64);
            add(&list_64, 64, temp, 1);
            temp = (struct chunk*)((char*)temp + 64);
            add(&list_128, 128, temp, 1);
            temp = (struct chunk*)((char*)temp + 128);
            add(&list_256, 256, temp, 1);
            memory = list_32;
            temp = (struct chunk*)list_32 - 1;
            list_32 = temp -> next;
        }
        else if(list_1024){
            struct chunk* temp = (struct chunk*)list_1024 - 1;
            list_1024 = temp -> next;
            add(&list_32, 32, temp, 2);
            temp = (struct chunk*)((char*)temp + 64);
            add(&list_64, 64, temp, 1);
            temp = (struct chunk*)((char*)temp + 64);
            add(&list_128, 128, temp, 1);
            temp = (struct chunk*)((char*)temp + 128);
            add(&list_256, 256, temp, 1);
            temp = (struct chunk*)((char*)temp + 512);
            add(&list_512, 512, temp, 1);
            memory = list_32;
            temp = (struct chunk*)list_32 - 1;
            list_32 = temp -> next;
        }
        else{
            void* initial = sbrk(0);
            if(sbrk(4096) == (void*)-1){
                return NULL;
            }
            add(&list_32, 32, initial, 8);
            initial = (char*)initial + 256;
            add(&list_64, 64, initial, 8);
            initial = (char*)initial + 512;
            add(&list_128, 128, initial, 4);
            initial = (char*)initial + 512;
            add(&list_256, 256, initial, 3);
            initial = (char*)initial + 768;
            add(&list_512, 512, initial, 2);
            initial = (char*)initial + 1024;
            add(&list_1024, 1024, initial, 1);
            memory = list_32;
            struct chunk* temp = (struct chunk*)list_32 - 1;
            list_32 = temp -> next;
        }
    }

    else if(size <= 52){
        if(list_64){
            memory = list_64;
            struct chunk* temp = (struct chunk*)list_64 - 1;
            list_64 = temp -> next; 
        }
        else if(list_128){
            struct chunk* temp = (struct chunk*)list_128 - 1;
            list_128 = temp -> next;
            add(&list_64, 64, temp, 2);
            memory = list_64;
            temp = (struct chunk*)list_64 - 1;
            list_64 = temp -> next;
        }
        else if(list_256){
            struct chunk* temp = (struct chunk*)list_256 - 1;
            list_256 = temp -> next;
            add(&list_64, 64, temp, 2);
            temp = (struct chunk*)((char*)temp + 128);
            add(&list_128, 128, temp, 1);
            memory = list_64;
            temp = (struct chunk*)list_64 - 1;
            list_64 = temp -> next;
        }
        else if(list_512){
            struct chunk* temp = (struct chunk*)list_512 - 1;
            list_512 = temp -> next;
            add(&list_64, 64, temp, 2);
            temp = (struct chunk*)((char*)temp + 128);
            add(&list_128, 128, temp, 1);
            temp = (struct chunk*)((char*)temp + 128);
            add(&list_256, 256, temp, 1);
            memory = list_64;
            temp = (struct chunk*)list_64 - 1;
            list_64 = temp -> next;
        }
        else if(list_1024){
            struct chunk* temp = (struct chunk*)list_1024 - 1;
            list_1024 = temp -> next;
            add(&list_64, 64, temp, 2);
            temp = (struct chunk*)((char*)temp + 128);
            add(&list_128, 128, temp, 1);
            temp = (struct chunk*)((char*)temp + 128);
            add(&list_256, 256, temp, 1);
            temp = (struct chunk*)((char*)temp + 256);
            add(&list_512, 512, temp, 1);
            memory = list_64;
            temp = (struct chunk*)list_64 - 1;
            list_64 = temp -> next;
        }
        else{
            void* initial = sbrk(0);
            if(sbrk(4096) == (void*)-1){
                return NULL;
            }
            add(&list_32, 32, initial, 8);
            initial = (char*)initial + 256;
            add(&list_64, 64, initial, 8);
            initial = (char*)initial + 512;
            add(&list_128, 128, initial, 4);
            initial = (char*)initial + 512;
            add(&list_256, 256, initial, 3);
            initial = (char*)initial + 768;
            add(&list_512, 512, initial, 2);
            initial = (char*)initial + 1024;
            add(&list_1024, 1024, initial, 1);
            memory = list_64;
            struct chunk* temp = (struct chunk*)list_64 - 1;
            list_64 = temp -> next;
        }
    }

    else if(size <= 116){
        if(list_128){
            memory = list_128;
            struct chunk* temp = (struct chunk*)list_128 - 1;
            list_128 = temp -> next;
        }
        else if(list_256){
            struct chunk* temp = (struct chunk*)list_256 - 1;
            list_256 = temp -> next;
            add(&list_128, 128, temp, 2);
            memory = list_128;
            temp = (struct chunk*)list_128 - 1;
            list_128 = temp -> next;
        }
        else if(list_512){
            struct chunk* temp = (struct chunk*)list_512 - 1;
            list_512 = temp -> next;
            add(&list_128, 128, temp, 2);
            temp = (struct chunk*)((char*)temp + 256);
            add(&list_256, 256, temp, 1);
            memory = list_128;
            temp = (struct chunk*)list_128 - 1;
            list_128 = temp -> next;
        }
        else if(list_1024){
            struct chunk* temp = (struct chunk*)list_1024 - 1;
            list_1024 = temp -> next;
            add(&list_128, 128, temp, 2);
            temp = (struct chunk*)((char*)temp + 256);
            add(&list_256, 256, temp, 1);
            temp = (struct chunk*)((char*)temp + 256);
            add(&list_512, 512, temp, 1);
            memory = list_128;
            temp = (struct chunk*)list_128 - 1;
            list_128 = temp -> next;
        }
        else{
            void* initial = sbrk(0);
            if(sbrk(4096) == (void*)-1){
                return NULL;
            }
            add(&list_32, 32, initial, 8);
            initial = (char*)initial + 256;
            add(&list_64, 64, initial, 8);
            initial = (char*)initial + 512;
            add(&list_128, 128, initial, 4);
            initial = (char*)initial + 512;
            add(&list_256, 256, initial, 3);
            initial = (char*)initial + 768;
            add(&list_512, 512, initial, 2);
            initial = (char*)initial + 1024;
            add(&list_1024, 1024, initial, 1);
            memory = list_128;
            struct chunk* temp = (struct chunk*)list_128 - 1;
            list_128 = temp -> next;
        }
    }
    else if(size <= 244){
        if(list_256){
            memory = list_256;
            struct chunk* temp = (struct chunk*)list_256 - 1;
            list_256 = temp -> next;
        }
        else if(list_512){
            struct chunk* temp = (struct chunk*)list_512 - 1;
            list_512 = temp -> next;
            add(&list_256, 256, temp, 2);
            memory = list_256;
            temp = (struct chunk*)list_256 - 1;
            list_256 = temp -> next;
        }
        else if(list_1024){
            struct chunk* temp = (struct chunk*)list_1024 - 1;
            list_1024 = temp -> next;
            add(&list_256, 256, temp, 2);
            temp = (struct chunk*)((char*)temp + 512);
            add(&list_512, 512, temp, 1);
            memory = list_256;
            temp = (struct chunk*)list_256 - 1;
            list_256 = temp -> next;
        }
        else{
            void* initial = sbrk(0);
            if(sbrk(4096) == (void*)-1){
                return NULL;
            }
            add(&list_32, 32, initial, 8);
            initial = (char*)initial + 256;
            add(&list_64, 64, initial, 8);
            initial = (char*)initial + 512;
            add(&list_128, 128, initial, 4);
            initial = (char*)initial + 512;
            add(&list_256, 256, initial, 3);
            initial = (char*)initial + 768;
            add(&list_512, 512, initial, 2);
            initial = (char*)initial + 1024;
            add(&list_1024, 1024, initial, 1);
            memory = list_256;
            struct chunk* temp = (struct chunk*)list_256 - 1;
            list_256 = temp -> next;
        }
    }

    else if(size <= 500){
        if(list_512){
            memory = list_512;
            struct chunk* temp = (struct chunk*)list_512 - 1;
            list_512 = temp -> next;
        }
        else if(list_1024){
            struct chunk* temp = (struct chunk*)list_1024 - 1;
            list_1024 = temp -> next;
            add(&list_512, 512, temp, 2);
            memory = list_512;
            temp = (struct chunk*)list_512 - 1;
            list_512 = temp -> next;
        }
        else{
            void* initial = sbrk(0);
            if(sbrk(4096) == (void*)-1){
                return NULL;
            }
            add(&list_32, 32, initial, 8);
            initial = (char*)initial + 256;
            add(&list_64, 64, initial, 8);
            initial = (char*)initial + 512;
            add(&list_128, 128, initial, 4);
            initial = (char*)initial + 512;
            add(&list_256, 256, initial, 3);
            initial = (char*)initial + 768;
            add(&list_512, 512, initial, 2);
            initial = (char*)initial + 1024;
            add(&list_1024, 1024, initial, 1);
            memory = list_512;
            struct chunk* temp = (struct chunk*)list_512 - 1;
            list_512 = temp -> next;
        }
    }

    else if(size <= 1012){
        if(list_1024){
            memory = list_1024;
            struct chunk* temp = (struct chunk*)list_1024 - 1;
            list_1024 = temp -> next;
        }
        else{
            void* initial = sbrk(0);
            if(sbrk(4096) == (void*)-1){
                return NULL;
            }
            add(&list_32, 32, initial, 8);
            initial = (char*)initial + 256;
            add(&list_64, 64, initial, 8);
            initial = (char*)initial + 512;
            add(&list_128, 128, initial, 4);
            initial = (char*)initial + 512;
            add(&list_256, 256, initial, 3);
            initial = (char*)initial + 768;
            add(&list_512, 512, initial, 2);
            initial = (char*)initial + 1024;
            add(&list_1024, 1024, initial, 1);
            memory = list_1024;
            struct chunk* temp = (struct chunk*)list_1024 - 1;
            list_1024 = temp -> next;
        }
    }
    return memory;
}

void free(void* memory){
    if(!memory){
        return;
    }   

    struct chunk* temp = (struct chunk*)memory - 1;
    int type = temp -> size;

    switch (type)
    {
    case 32:
        add(&list_32, 32, temp, 1);
        break;

    case 64:
        add(&list_64, 64, temp, 1);
        break;
    
    case 128:
        add(&list_128, 128, temp, 1);
        break;

    case 256:
        add(&list_256, 256, temp, 1);
        break;

    case 512:
        add(&list_512, 512, temp, 1);
        break;

    case 1024:
        add(&list_1024, 1024, temp, 1);
        break;
    }
}
