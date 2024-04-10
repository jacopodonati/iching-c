#ifndef ICHING_H
#define ICHING_H

typedef struct {
    int index;
    unsigned long long int binary_representation;
} Hexagram;

extern const Hexagram iching[];

#endif

