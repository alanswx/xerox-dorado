#define _GNU_SOURCE
#include "dorado_ddr_mapping.h"
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int dorado_ddr_window_reserved(FILE *f) {
    if(!f)return EINVAL;
    char line[512],name[256];unsigned long long low,high,largest_ram=0;
    int saw_ram=0;
    const uint64_t begin=DORADO_DDR_BASE,end=begin+DORADO_DDR_SIZE-1;
    while(fgets(line,sizeof line,f)) {
        if(sscanf(line," %llx-%llx : %255[^\n]",&low,&high,name)!=3)continue;
        size_t n=strlen(name);while(n && (name[n-1]==' ' || name[n-1]=='\r'))name[--n]=0;
        if(high<low)return EPROTO;
        if(!strcmp(name,"System RAM")) {
            saw_ram=1;if(high>largest_ram)largest_ram=high;
            if(low<=end && high>=begin)return EBUSY;
        } else if(low<=end && high>=begin && strcmp(name,"reserved") &&
                  strcmp(name,"Reserved") && strcmp(name,"reserved memory"))return EBUSY;
    }
    if(ferror(f))return EIO;
    /* /proc/iomem may hide every address as zero from an unprivileged caller.
     * Absence of visible RAM is not evidence that an aperture is reserved. */
    return !saw_ram || largest_ram<0x1000000 ? EACCES : 0;
}
static int map_window(void *unused,volatile void **out) {
    (void)unused;*out=NULL;
    FILE *f=fopen("/proc/iomem","r");if(!f)return errno;
    int error=dorado_ddr_window_reserved(f);fclose(f);if(error)return error;
    int fd=open("/dev/mem",O_RDWR|O_SYNC|O_CLOEXEC);if(fd<0)return errno;
    void *mapping=mmap(NULL,DORADO_DDR_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,DORADO_DDR_BASE);
    error=mapping==MAP_FAILED?errno:0;close(fd);
    if(!error)*out=mapping;
    return error;
}
static int unmap_window(void *unused,volatile void *mapping) {
    (void)unused;return munmap((void *)mapping,DORADO_DDR_SIZE)?errno:0;
}
dorado_ddr_mapper dorado_ddr_physical_mapper(void) {
    dorado_ddr_mapper result={map_window,unmap_window,NULL};return result;
}
