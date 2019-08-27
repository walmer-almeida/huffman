#ifndef _AUXFUNCTIONS_H_
#define _AUXFUNCTIONS_H_

bool comparing(void *itemA,unsigned char itemB){
    return ((*(unsigned char*)itemA)== itemB);
}

unsigned char setBit( unsigned char c, int i){
    unsigned char mask = 1 << i;
    return mask | c;
}

int is_bit_i_set(unsigned char c, int i)
{
    unsigned char mask = 1 << i;
    return mask & c;
}

#endif