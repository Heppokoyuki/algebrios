#ifndef _PCI_H_
#define _PCI_H_

#include <stdint.h>

#define PCI_CONF_DID_VID 0x00
#define PCI_CONF_STATUS_COMMAND 0x04
#define PCI_COM_IO_EN (1U << 0)
#define PCI_COM_MEM_EN (1U << 1)
#define PCI_COM_BUS_MASTER_EN (1U << 2)
#define PCI_COM_SPECIAL_CYCLE (1U << 3)
#define PCI_COM_MEMW_INV_EN (1U << 4)
#define PCI_COM_VGA_PAL_SNP (1U << 5)
#define PCI_COM_PARITY_ERR_RES (1U << 6)
#define PCI_COM_SERR_EN (1U << 8)
#define PCI_COM_FAST_BACK2BACK_EN (1U << 9)
#define PCI_COM_INTR_DIS (1U << 10)
#define PCI_STAT_INTR (1U << 3)
#define PCI_STAT_MULT_FUNC (1U << 4)
#define PCI_STAT_66MHZ (1U << 5)
#define PCI_STAT_FAST_BACK2BACK (1U << 7)
#define PCI_STAT_DATA_PARITY_ERR (1U << 8)
#define PCI_STAT_DEVSEL_MASK (3U << 9)
#define PCI_STAT_DEVSEL_FAST (0b00 << 9)
#define PCI_STAT_DEVSEL_MID  (0b01 << 9)
#define PCI_STAT_DEVSEL_LOW  (0b10 << 9)
#define PCI_STAT_SND_TARGET_ABORT (1U << 11)
#define PCI_STAT_RCV_TARGET_ABORT (1U << 12)
#define PCI_STAT_RCV_MASTER_ABORT (1U << 13)
#define PCI_STAT_SYS_ERR (1U << 14)
#define PCI_STAT_PARITY_ERR (1U << 15)

#define CONFIG_ADDRESS 0x0cf8
#define CONFIG_DATA 0x0cfc

union pci_config_address {
    unsigned int raw;
    struct {
        unsigned int reg_addr:8;
        unsigned int func_num:3;
        unsigned int dev_num:5;
        unsigned int bus_num:8;
        unsigned int _reserved:7;
        unsigned int enable_bit:1;
    } __attribute__ ((packed));
};

#define NIC_BUS_NUM 0x03
#define NIC_DEV_NUM 0x00
#define NIC_FUNC_NUM 0x0

uint32_t get_pci_conf_reg(uint8_t bus, uint8_t dev, uint8_t func, uint8_t reg);

void dump_vid_did(uint8_t bus, uint8_t dev, uint8_t func);
void dump_command_status(uint8_t bus, uint8_t dev, uint8_t func);
#endif
