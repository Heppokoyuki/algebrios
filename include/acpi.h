#ifndef _ACPI_H_
#define _ACPI_H_

#include <types.h>

struct SDTH {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checkSum;
    char OEMID[6];
    char OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t creatorID;
    uint32_t creatorRevision;
} __attribute__ ((packed));

struct ACPI_ADDRESS {
    uint8_t address_space_id;
    uint8_t register_bit_width;
    uint8_t register_bit_offset;
    uint8_t _reserved;
    uint64_t address;
} __attribute__ ((packed));

void init_acpi(void *rsdp);
void dump_sdth_sig(struct SDTH *h);
void dump_xsdt(void);
struct SDTH *get_sdt(char *s);

#endif
