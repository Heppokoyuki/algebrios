#ifndef _MEMORY_H_
#define _MEMORY_H_

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


#endif
