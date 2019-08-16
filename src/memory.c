#include <stdint.h>
#include <algebrios.h>
#include <memory.h>
#include <fbcon.h>
#include <stddef.h>

static uint64_t
phys_memory_add_region(uint64_t base, uint64_t next, page_frame_mannager_t *pfm)
{
    uint64_t page_count;
    phys_memory_page_block_t *pb;

    page_count = (next - base) / MEMORY_PAGESIZE;

    if(!page_count) return page_count;

    pb = pfm->pb + pfm->block_count;

    pb->base = base;
    pb->page_count = page_count;
    pfm->block_count++;

    return page_count;
}

static void
merge_continuous_region(page_frame_mannager_t *pfm)
{
    phys_memory_page_block_t *pb, *npb;
    uint16_t i, j;

    /* remove empty region */
    for(i = 0; i < pfm->block_count; ++i) {
        pb = pfm->pb + i;

        if(!pb->page_count) {
            for(j = i; j < pfm->block_count - 1; ++j) {
                pb = pfm->pb + j;
                npb = pb + 1;

                pb->base = npb->base;
                pb->page_count = npb->page_count;
            }
            pfm->block_count--;
        }
    }

    for(i = 0; i < pfm->block_count - 1; ++i) {
        pb = pfm->pb + i;
        npb = pb + 1;

        /* merge */
        if(pb->base + pb->page_count * MEMORY_PAGESIZE == npb->base) {
            pb->page_count += npb->page_count;
            for(j = i + 1; j < pfm->block_count - 1; ++j) {
                pb = pfm->pb + j;
                npb = pb + 1;

                pb->base = npb->base;
                pb->page_count = npb->page_count;
            }
            pfm->block_count--;
            /* Maybe 3 regions can be merged continuosly */
            i--;
        }
    }
}

/* page frame allocator */
void *
pfalloc(uint64_t nr, page_frame_mannager_t *pfm)
{
    phys_memory_page_block_t *pb;
    void *result = NULL;
    uint16_t i;

    for(i = 0; i < pfm->block_count; ++i) {
        pb = pfm->pb + i;

        if(pb->page_count >= nr) {
            result = (void *)pb->base;
            pb->base += nr * MEMORY_PAGESIZE;
            pb->page_count -= nr;
        }
    }

    merge_continuous_region(pfm);
    return result;
}

/* page frame free */
//TODO

void
dump_phys_memory_page_block(page_frame_mannager_t *pfm)
{
    uint16_t i;

    puts("--------------------\r\n");
    for(i = 0; i < pfm->block_count; ++i) {
        puts("BASE ADDRESS ");
        puth((pfm->pb + i)->base, 10);
        puts("\r\nBLOCK SIZE ");
        putd((pfm->pb + i)->page_count * MEMORY_PAGESIZE / 1024 / 1024, 5);
        puts("MiB");
        puts("\r\nPAGE COUNT ");
        puth((pfm->pb + i)->page_count, 10);
        puts("\r\n");
        puts("--------------------\r\n");
    }
}


uint64_t
init_phys_memory(uint64_t entry_count, uint64_t entry_size, mdesc_t *mdesc, page_frame_mannager_t *pfm)
{
    mdesc_t *p;
    mdesc_t *next_p;
    uint64_t base;
    uint64_t next;
    uint64_t free_page = 0;

    p = mdesc;
    pfm->block_count = 0;

    for(uint64_t i = 0; i < entry_count; ++i) {
        next_p = (mdesc_t *)((unsigned char *)p + entry_size);
        base = p->ps;
        next = next_p->ps;

        if(p->type != EfiConventionalMemory &&
           p->type != EfiBootServicesCode &&
           p->type != EfiBootServicesData) {
            p = next_p;
            continue;
        }

        /* Kernel static reserved address */
        if(base < KERNEL_RESERVED)
            base = KERNEL_RESERVED;
        if(next < KERNEL_RESERVED)
            next = KERNEL_RESERVED;

        /* Round up for page alignment */
        base = (base + (MEMORY_PAGESIZE - 1)) & ~(MEMORY_PAGESIZE - 1);
        next = next & ~(MEMORY_PAGESIZE - 1);

        /* Is aligned address valid? */
        if(base >= MEMORY_ALLOCATE_LIMIT) {
            p = next_p;
            continue;
        }
        if(next > MEMORY_ALLOCATE_LIMIT) {
            next = MEMORY_ALLOCATE_LIMIT;
        }

        free_page += phys_memory_add_region(base, next, pfm);
        p = next_p;
    }

    merge_continuous_region(pfm);

    return free_page;
}
