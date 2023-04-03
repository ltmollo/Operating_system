#ifndef ZAD2_DL_MANAGER_H
#define ZAD2_DL_MANAGER_H

#ifdef USE_DYNAMIC_LIBRARY
#include <dlfcn.h>
#include <stdio.h>


void load_dynamic_library(){
    void* handle = dlopen("./library.so", RTLD_LAZY);
    if(handle == NULL){
        fprintf(stderr, "Dynamic library not found\n");
        return;
    }
    *(void**) (&Result_init) = dlsym(handle, "Result_init");  // function pointer

    *(void**) (&Result_push) = dlsym(handle, "Result_push");

    *(void**) (&Result_pop) = dlsym(handle, "Result_pop");

    *(void**) (&Result_get) = dlsym(handle, "Result_get");

    *(void**) (&Result_clear) = dlsym(handle, "Result_clear");

    *(void**) (&Result_destructor) = dlsym(handle, "Result_destructor");
}

#else
void load_dynamic_library(){};  // if we don't use dll we can to nothing

#endif
#endif //ZAD2_DL_MANAGER_H
