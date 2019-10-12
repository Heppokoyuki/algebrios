#include <pci.h>
#include <asmfunc.h>
#include <stdint.h>
#include <fbcon.h>

uint32_t
get_pci_conf_reg(uint8_t bus, uint8_t dev, uint8_t func, uint8_t reg)
{
    union pci_config_address conf_addr;
    conf_addr.raw = 0;
    conf_addr.bus_num = bus;
    conf_addr.dev_num = dev;
    conf_addr.func_num = func;
    conf_addr.reg_addr = reg;
    conf_addr.enable_bit = 1;
    outl(CONFIG_ADDRESS, conf_addr.raw);

    return inl(CONFIG_DATA);
}

void
dump_vid_did(uint8_t bus, uint8_t dev, uint8_t func)
{
    uint32_t conf_data = get_pci_conf_reg(bus, dev, func, PCI_CONF_DID_VID);
    uint16_t vendor_id = conf_data & 0x0000ffff;
    uint16_t device_id = conf_data >> 16;

    puts("VENDOR ID ");
    puth(vendor_id, 4);
    puts("\r\n");
    puts("DEVICE ID ");
    puth(device_id, 4);
    puts("\r\n");
}

void
dump_command_status(uint8_t bus, uint8_t dev, uint8_t func)
{
    uint32_t conf_data = get_pci_conf_reg(bus, dev, func, PCI_CONF_STATUS_COMMAND);
    uint16_t command = conf_data & 0x0000fffff;
    uint16_t status = conf_data >> 16;

    puts("COMMAND ");
    puth(command, 4);
    puts("\r\n");

	if (command & PCI_COM_IO_EN)
		puts("IO_EN ");
	if (command & PCI_COM_MEM_EN)
		puts("MEM_EN ");
	if (command & PCI_COM_BUS_MASTER_EN)
		puts("BUS_MASTER_EN ");
	if (command & PCI_COM_SPECIAL_CYCLE)
		puts("SPECIAL_CYCLE ");
	if (command & PCI_COM_MEMW_INV_EN)
		puts("MEMW_INV_EN ");
	if (command & PCI_COM_VGA_PAL_SNP)
		puts("VGA_PAL_SNP ");
	if (command & PCI_COM_PARITY_ERR_RES)
		puts("PARITY_ERR_RES ");
	if (command & PCI_COM_SERR_EN)
		puts("SERR_EN ");
	if (command & PCI_COM_FAST_BACK2BACK_EN)
		puts("FAST_BACK2BACK_EN ");
	if (command & PCI_COM_INTR_DIS)
		puts("INTR_DIS");
	puts("\r\n");

    puts("STATUS ");
    puth(status, 4);
    puts("\r\n");

    if (status & PCI_STAT_INTR)
		puts("INTR ");
	if (status & PCI_STAT_MULT_FUNC)
		puts("MULT_FUNC ");
	if (status & PCI_STAT_66MHZ)
		puts("66MHZ ");
	if (status & PCI_STAT_FAST_BACK2BACK)
		puts("FAST_BACK2BACK ");
	if (status & PCI_STAT_DATA_PARITY_ERR)
		puts("DATA_PARITY_ERR ");

	switch (status & PCI_STAT_DEVSEL_MASK) {
	case PCI_STAT_DEVSEL_FAST:
		puts("DEVSEL_FAST ");
		break;
	case PCI_STAT_DEVSEL_MID:
		puts("DEVSEL_MID ");
		break;
	case PCI_STAT_DEVSEL_LOW:
		puts("DEVSEL_LOW ");
		break;
	}

	if (status & PCI_STAT_SND_TARGET_ABORT)
		puts("SND_TARGET_ABORT ");
	if (status & PCI_STAT_RCV_TARGET_ABORT)
		puts("RCV_TARGET_ABORT ");
	if (status & PCI_STAT_RCV_MASTER_ABORT)
		puts("RCV_MASTER_ABORT ");
	if (status & PCI_STAT_SYS_ERR)
		puts("SYS_ERR ");
	if (status & PCI_STAT_PARITY_ERR)
		puts("PARITY_ERR");
	puts("\r\n");
}

