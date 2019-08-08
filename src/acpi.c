#include <types.h>
#include <acpi.h>
#include <common.h>
#include <fbcon.h>

struct RSDP {
    char signature[8];
    uint8_t checkSum;
    char OEMID[6];
    uint8_t revision;
    uint32_t rsdtAddress;
    uint32_t length;
    uint64_t xsdtAddress;
    uint8_t extendedCheckSum;
    uint8_t reserved[3];
} __attribute__ ((packed));

struct XSDT {
    struct SDTH header;
    struct SDTH *entry[0];
} __attribute__ ((packed));

struct XSDT *xsdt;
uint64_t num_sdts;

void
init_acpi(void *rsdp)
{
    xsdt = (struct XSDT *)((struct RSDP *)rsdp)->xsdtAddress;

    num_sdts = (xsdt->header.length - sizeof(struct SDTH)) / sizeof(struct SDTH *);
}

void
dump_sdth_sig(struct SDTH *h)
{
    unsigned char i;
    for(i = 0; i < 4; ++i)
        putc(h->signature[i]);
}

void
dump_xsdt(void)
{
    dump_sdth_sig(&xsdt->header);
    puts("\r\n");

    puts("NUM SDTS ");
    putd(num_sdts, 2);
    puts("\r\n");

    unsigned long long i;
    for(i = 0; i < num_sdts; ++i) {
        dump_sdth_sig(xsdt->entry[i]);
        putc(' ');
    }
    puts("\r\n");
}

struct SDTH *get_sdt(char *s)
{
    unsigned long long i;
    for(i = 0; i < num_sdts; ++i) {
        if(!strncmp(s, xsdt->entry[i]->signature, 4))
            return xsdt->entry[i];
    }
    return NULL;
}


