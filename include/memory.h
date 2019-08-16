#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>

#define MEMORY_PAGESIZE (1 << 12)

struct phys_memory_page_block {
    uint64_t base;
    uint64_t page_count;
} __attribute__ ((packed));
typedef struct phys_memory_page_block phys_memory_page_block_t;

typedef struct {
    uint16_t block_count;
    phys_memory_page_block_t pb[MAX_PAGE_FRAME_BLOCK];
} page_frame_mannager_t;

struct memory_map {
    unsigned long long mmap_size;
    unsigned char *mem_desc;
    unsigned long long map_key;
    unsigned long long mem_desc_unit_size;
    unsigned int desc_ver;
};
typedef struct memory_map mmap_t;

struct memory_descriptor {
    uint16_t type;
    uint64_t ps;
    uint64_t vs;
    uint64_t nop;
    uint64_t attr;
};
typedef struct memory_descriptor mdesc_t;

typedef enum {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistenMemory,
    EfiMaxMemoryType
} EFI_MEMORY_TYPE;

uint64_t init_phys_memory(uint64_t entry_count, uint64_t entry_size, mdesc_t *mdesc, page_frame_mannager_t *pfm);
void dump_phys_memory_page_block(page_frame_mannager_t *pfm);
void *pfalloc(uint64_t nr, page_frame_mannager_t *pfm);

#endif
