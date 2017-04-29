#ifndef UTILS_H
#define UTILS_H

typedef struct {
  unsigned int value : 1;
} Bit;

class Utils{
   public:
      static int binToDec(Bit* data, int size);
      static int binToDec(Bit* data, int size, int start);
      static int power(int a, int b);
      static char charOfBin(Bit* data, int shift);
     /* static int max(int a, int b);
      static int min(int a, int b); */
};


#endif
