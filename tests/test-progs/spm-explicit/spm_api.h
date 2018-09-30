#ifndef SPM_API_H_
#define SPM_API_H_

#include <gem5/m5ops.h>

#include "spm_types.h"

#define SYNC_ROI() wakeCPU(0xFFFFFFFFFFFFFFFF);

void spm_alloc(uint64_t start, uint64_t end, uint64_t metadata){
  register uint64_t a asm ("19");
  register uint64_t b asm ("20");
  register uint64_t c asm ("21");
  a = start;
  b = end;
  c = metadata;

  //printf("rd: %lu rs1: %lu rs2: %lu \n", a, b, c);
  asm volatile
  (
    "spmal   %[z], %[x], %[y]\n\t"
    : [z] "=r" (a)
    : [x] "r" (b), [y] "r" (c)
  );
}

void spm_free(uint64_t start, uint64_t end, uint64_t metadata){
  register uint64_t a asm ("16");
  register uint64_t b;
  register uint64_t c;
  a = start;
  b = end;
  c = metadata;
  asm volatile
  (
    "spmal   %[z], %[x], %[y]\n\t"
    : [z] "=r" (a)
    : [x] "r" (b), [y] "r" (c)
  );
}

static inline void SPM_ALLOC32(uint64_t start_v_address,
                               uint64_t end_v_address,
                               AllocationModes alloc_mode,
                               //only used for explicitly addressed SPMs
                               uint64_t start_spm_address,
                               uint64_t shared_data,
                               DataImportance d_importance,
                               ThreadPriority app_priority,
                               Approximation approximation)
{
    uint64_t modes = 0x0000000000000000;
    modes = modes | (uint64_t)approximation;
    modes = modes | (uint64_t)shared_data << 8;
    modes = modes | (uint64_t)alloc_mode << 12;
    modes = modes | (uint64_t)d_importance << 16;
    modes = modes | (uint64_t)app_priority << 20;

    uint64_t encoded_start =  ( 0x0000000000000000 | start_v_address) |
                              ( modes & 0xFFFFFFFF00000000);
    uint64_t encoded_end   =  ( 0x0000000000000000 | end_v_address)   |
                              ( modes & 0x00000000FFFFFFFF) << 32;

    /*
     * for consistency with 3rd argument of 64b pseudo inst call,
     * we need to use the upper 32 bits to the address
     */
    start_spm_address = (uint64_t)start_spm_address << 32;

        spm_alloc(encoded_start, encoded_end, start_spm_address);
    //m5_spm_alloc(encoded_start, encoded_end, start_spm_address);
}

static inline void SPM_ALLOC64(uint64_t start_v_address,
                               uint64_t end_v_address,
                               AllocationModes alloc_mode,
                               //only used for explicitly addressed SPMs
                               uint64_t start_spm_address,
                               uint64_t shared_data,
                               DataImportance d_importance,
                               ThreadPriority app_priority,
                               Approximation approximation,
                               uint64_t spm_host)
{
    uint64_t modes = 0x0000000000000000;
    modes = modes | (uint64_t)approximation;
    modes = modes | (uint64_t)shared_data << 8;
    modes = modes | (uint64_t)alloc_mode << 12;
    modes = modes | (uint64_t)d_importance << 16;
    modes = modes | (uint64_t)app_priority << 20;
        modes = modes | (uint64_t)spm_host << 24;
    /*
     * we limit the physically addressable SPM space to 4GB, dedicating
     * the top 32b of mode to the address
     */

    modes = modes | (uint64_t)start_spm_address << 32;

        spm_alloc(start_v_address, end_v_address, modes);
    //m5_spm_alloc(encoded_start, encoded_end, modes);
}

/*************************************************************/

static inline void SPM_FREE32(uint64_t start_v_address,
                              uint64_t end_v_address,
                              DeallocationModes dealloc_mode)
{
    uint64_t modes = 0x0000000000000000;
    modes = modes | (uint64_t)dealloc_mode;

    uint64_t encoded_start =  ( 0x0000000000000000 | start_v_address) |
                              ( modes & 0xFFFFFFFF00000000);
    uint64_t encoded_end   =  ( 0x0000000000000000 | end_v_address)   |
                              ( modes & 0x00000000FFFFFFFF) << 32;

    //m5_spm_free(encoded_start, encoded_end, 0);
    spm_free(encoded_start, encoded_end,0);
}

static inline void SPM_FREE64(uint64_t start_v_address,
                              uint64_t end_v_address,
                              DeallocationModes dealloc_mode)
{
    uint64_t modes = 0x0000000000000000;
    modes = modes | (uint64_t)dealloc_mode;

    //m5_spm_free(start_v_address, end_v_address, modes);
    spm_free(start_v_address, end_v_address, modes);
}

#endif /* SPM_API_H_ */
