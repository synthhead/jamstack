#ifndef MEMSPACE_H_
#define MEMSPACE_H_

// #include <inttypes.h>

#define FLASHFUNC __attribute__((section(".flashfunc")))
#define FLASHMEM  __attribute__((section(".flashmem")))
#define OCMEM     __attribute__((section(".dmabuffers")))
#define DMAMEM    __attribute__((section(".dmabuffers")))

#endif
