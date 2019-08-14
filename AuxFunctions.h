#ifndef _AUXFUNCTIONS_H_
#define _AUXFUNCTIONS_H_

bool comparing(void *itemA,unsigned char itemB){
    return (*((unsigned char*)itemA)== itemB);
}

#endif