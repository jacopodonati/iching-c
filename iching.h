#ifndef ICHING_H
#define ICHING_H

typedef struct Hexagram {
  int index;
  unsigned char binary_representation;
} Hexagram;

extern const Hexagram iching[];

#endif
