#include <acpi.h>
#include <fbcon.h>

struct HPET_TABLE {
    struct SDTH header;
    uint32_t event_timer_block_id;
    struct ACPI_ADDRESS base_address;
    uint8_t hpet_number;
    uint16_t minimum_tick;
    uint8_t flags;
} __attribute__ ((packed));

uint64_t reg_base;

#define GCIDR_ADDR (reg_base)
#define GCIDR (*(volatile uint64_t *)GCIDR_ADDR)
union gcidr {
    uint64_t raw;
    struct {
        uint64_t rev_id:8;
        uint64_t num_tim_cap:5;
        uint64_t count_size_cap:1;
        uint64_t _reserved:1;
        uint64_t leg_rt_cap:1;
        uint64_t vendor_id:16;
        uint64_t counter_clk_period:32;
    } __attribute__ ((packed));
};

#define GCR_ADDR (reg_base + 0x10)
#define GCR (*(volatile uint64_t *)GCR_ADDR)
union gcr {
    uint64_t raw;
    struct {
        uint64_t enable_cnf:1;
        uint64_t leg_rt_cnf:1;
        uint64_t _reserved:62;
    } __attribute__ ((packed));
};

#define MCR_ADDR (reg_base + 0xf0)
#define MCR (*(volatile uint64_t *)MCR_ADDR)

#define US_TO_FS 1000000000

void
init_hpet(void)
{
    struct HPET_TABLE *hpet_table = (struct HPET_TABLE *)get_sdt("HPET");
    reg_base = hpet_table->base_address.address;

    union gcr gcr;
    gcr.raw = GCR;
    gcr.enable_cnf = 0;
    GCR = gcr.raw;
}

void
dump_gcidr(void)
{
    puts("GCIDR\r\n");

    union gcidr r;
    r.raw = GCIDR;

    puts("REV ID ");
    putd(r.rev_id, 3);
    puts("\r\n");
    puts("NUM TIM CAP ");
    putd(r.num_tim_cap, 2);
    puts("\r\n");
    puts("COUNT SIZE CAP ");
    putd(r.count_size_cap, 1);
    puts("\r\n");
    puts("LEG RT CAP ");
    putd(r.leg_rt_cap, 1);
    puts("\r\n");
    puts("COUNTER CLK PERIOD ");
    putd(r.counter_clk_period, 10);
    puts("\r\n");
}

void
dump_gcr(void)
{
    puts("GCR\r\n");

    union gcr r;
    r.raw = GCR;

    puts("ENABLE CNF ");
    putd(r.enable_cnf, 1);
    puts("\r\n");
    puts("LEG RT CNF ");
    putd(r.leg_rt_cnf, 1);
    puts("\r\n");
}

void
dump_mcr(void)
{
    puts("MCR ");
    puth(MCR, 16);
    puts("\r\n");
}

void
sleep(uint64_t us)
{
    uint64_t mc_now = MCR;
    uint64_t fs = us * US_TO_FS;

    union gcidr gcidr;
    gcidr.raw = GCIDR;
    uint64_t mc_duration = fs / gcidr.counter_clk_period;
    uint64_t mc_after = mc_now + mc_duration;

    union gcr gcr;
    gcr.raw = GCR;
    uint8_t to_disable = 0;
    if(!gcr.enable_cnf) {
        gcr.enable_cnf = 1;
        GCR = gcr.raw;

        to_disable = 1;
    }

    while(MCR < mc_after);

    if(to_disable) {
        gcr.raw = GCR;
        gcr.enable_cnf = 0;
        GCR = gcr.raw;
    }
}

