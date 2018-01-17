#ifndef SPM_API_H_
#define SPM_API_H_

#include "spm_types.h"
#include "../../../../util/m5/m5op.h"

#define SYNC_ROI() wakeCPU(0xFFFFFFFFFFFFFFFF);

static inline void SPM_ALLOC32(uint64_t start_v_address,
                               uint64_t end_v_address,
                               Approximation approximation,
                               uint64_t shared_data,
                               AllocationModes alloc_mode,
                               DataImportance d_importance,
                               ThreadPriority app_priority)
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

    m5_spm_alloc(encoded_start, encoded_end, 0);
}

static inline void SPM_ALLOC64(uint64_t start_v_address,
                               uint64_t end_v_address,
                               Approximation approximation,
                               uint64_t shared_data,
                               AllocationModes alloc_mode,
                               DataImportance d_importance,
                               ThreadPriority app_priority)
{
    uint64_t modes = 0x0000000000000000;
    modes = modes | (uint64_t)approximation;
    modes = modes | (uint64_t)shared_data << 8;
    modes = modes | (uint64_t)alloc_mode << 12;
    modes = modes | (uint64_t)d_importance << 16;
    modes = modes | (uint64_t)app_priority << 20;

    m5_spm_alloc(start_v_address, end_v_address, modes);
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

    m5_spm_free(encoded_start, encoded_end, 0);
}

static inline void SPM_FREE64(uint64_t start_v_address,
                              uint64_t end_v_address,
                              DeallocationModes dealloc_mode)
{
    uint64_t modes = 0x0000000000000000;
    modes = modes | (uint64_t)dealloc_mode;

    m5_spm_free(start_v_address, end_v_address, modes);
}

#endif /* SPM_API_H_ */
