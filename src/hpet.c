#include <acpi.h>
#include <fbcon.h>
#include <pic.h>
#include <hpet.h>
#include <common.h>
#include <idt.h>
#include <handler.h>
#include <serial.h>

#define TIMER_N 0
#define US_TO_FS 1000000000

struct HPET_TABLE {
    struct SDTH header;
    uint32_t event_timer_block_id;
    struct ACPI_ADDRESS base_address;
    uint8_t hpet_number;
    uint16_t minimum_tick;
    uint8_t flags;
} __attribute__ ((packed));

uint64_t reg_base;
uint32_t counter_clk_period;

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

#define TNCCR_ADDR(n) (reg_base + (0x20 * (n)) + 0x100)
#define TNCCR(n) (*(volatile uint64_t *)(TNCCR_ADDR(n)))
#define TNCCR_INT_TYPE_EDGE 0
#define TNCCR_INT_TYPE_LEVEL 1
#define TNCCR_TYPE_NON_PERIODIC 0
#define TNCCR_TYPE_PERIODIC 1
union tnccr {
    uint64_t raw;
    struct {
        uint64_t _reserved1:1;
        uint64_t int_type_cnf:1;
        uint64_t int_enb_cnf:1;
        uint64_t type_cnf:1;
        uint64_t per_int_cap:1;
        uint64_t size_cap:1;
        uint64_t val_set_cnf:1;
        uint64_t _reserved2:1;
        uint64_t mode32_cnf:1;
        uint64_t int_route_cnf:5;
        uint64_t fsb_en_cnf:1;
        uint64_t fsb_int_del_cap:1;
        uint64_t _reserved3:16;
        uint64_t int_route_cap:32;
    } __attribute__ ((packed));
};

#define TNCR_ADDR(n) (reg_base + (0x20 * (n)) + 0x108)
#define TNCR(n) (*(volatile uint64_t *)(TNCR_ADDR(n)))

void (*user_handler)(uint64_t current_rsp) = NULL;

void
init_hpet(void)
{
    struct HPET_TABLE *hpet_table = (struct HPET_TABLE *)get_sdt("HPET");
    reg_base = hpet_table->base_address.address;

    union gcr gcr;
    gcr.raw = GCR;
    gcr.enable_cnf = 0;
    gcr.leg_rt_cnf = 1;
    GCR = gcr.raw;

    union gcidr gcidr;
    gcidr.raw = GCIDR;
    counter_clk_period = gcidr.counter_clk_period;

    union tnccr tnccr;
    tnccr.raw = TNCCR(TIMER_N);
    tnccr.int_type_cnf = TNCCR_INT_TYPE_EDGE;
    tnccr.int_enb_cnf = 0;
    tnccr.type_cnf = TNCCR_TYPE_NON_PERIODIC;
    tnccr.val_set_cnf = 0;
    tnccr.mode32_cnf = 0;
    tnccr.fsb_en_cnf = 0;
    tnccr._reserved1 = 0;
    tnccr._reserved2 = 0;
    tnccr._reserved3 = 0;
    TNCCR(TIMER_N) = tnccr.raw;

    set_intr_gate(HPET_INTR_NO, hpet_handler);

    enable_pic_intr(HPET_INTR_NO);
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

void
do_hpet_interrupt(uint64_t current_rsp)
{
    puts_serial("debug");
    union gcr gcr;
    gcr.raw = GCR;
    gcr.enable_cnf = 0;
    GCR = gcr.raw;

    union tnccr tnccr;
    tnccr.raw = TNCCR(TIMER_N);
    tnccr.int_enb_cnf = 0;
    tnccr._reserved1 = 0;
    tnccr._reserved2 = 0;
    tnccr._reserved3 = 0;
    TNCCR(TIMER_N) = tnccr.raw;

    if(user_handler)
        user_handler(current_rsp);

    set_pic_eoi(HPET_INTR_NO);
}

void
alert(uint64_t us, void *handler)
{
    user_handler = handler;
    union tnccr tnccr;
    tnccr.raw = TNCCR(TIMER_N);
    tnccr.int_enb_cnf = 1;
    tnccr.type_cnf = TNCCR_TYPE_NON_PERIODIC;
    tnccr._reserved1 = 0;
    tnccr._reserved2 = 0;
    tnccr._reserved3 = 0;
    TNCCR(TIMER_N) = tnccr.raw;

    MCR = (uint64_t)0;

    uint64_t femt_sec = us * US_TO_FS;
    uint64_t clk_counts = femt_sec / counter_clk_period;
    TNCR(TIMER_N) = clk_counts;

    union gcr gcr;
    gcr.raw = GCR;
    gcr.enable_cnf = 1;
    GCR = gcr.raw;
}

