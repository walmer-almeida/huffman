#ifndef _AUXFUNCTIONS_H_
#define _AUXFUNCTIONS_H_

/*
 * Recieves a void pointer (with something inside) and a unsigned char byte;
 * Compares the recieved unsigned char with the (unsigned char)casted void pointer content;
 * Returns true if (unsigned char)itemA == itemB, false if don't.
*/
bool comparing(void *itemA,unsigned char itemB){
    return ((*(unsigned char*)itemA)== itemB);
}

/*
 * Recieves a unsigned char byte and a integer;
 * Sets a bit at i position;
 * Returns the recieved byte, now with a bit setted at i position.
*/
unsigned char setBit( unsigned char c, int i){
    unsigned char mask = 1 << i;
    return mask | c;
}

/*
 * Recieves a unsigned char byte and a integer;
 * Checks if a bit is setted at i position;
 * Returns 1 if the bit is setted at i position, false if it isn't.
*/
int isBitISet(unsigned char c, int i)
{
    unsigned char mask = 1 << i;
    return mask & c;
}

#endif