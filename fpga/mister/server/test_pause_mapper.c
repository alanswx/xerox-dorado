/* Link-only fixture. The production executable never links this file and
 * has no environment option to replace its validated /dev/mem mapper. */
#ifndef DORADO_TEST_PAUSE_MAPPER
#error Test-only mapper requires the explicit test build
#endif
#define _GNU_SOURCE
#include "dorado_ddr_mapping.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

static int map_test(void *unused,volatile void **out){
    (void)unused;*out=NULL;
    const char *path=getenv("DORADO_TEST_DDR_MEMORY");if(!path)return EINVAL;
    int fd=open(path,O_RDWR);if(fd<0)return errno;
    struct stat st;if(fstat(fd,&st)){int error=errno;close(fd);return error;}
    if(st.st_size!=DORADO_DDR_SIZE){close(fd);return EINVAL;}
    void *p=mmap(NULL,DORADO_DDR_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    int error=p==MAP_FAILED?errno:0;close(fd);if(!error)*out=p;return error;
}
static int unmap_test(void *unused,volatile void *p){
    (void)unused;return munmap((void *)p,DORADO_DDR_SIZE)?errno:0;
}
dorado_ddr_mapper dorado_ddr_physical_mapper(void){
    dorado_ddr_mapper mapper={map_test,unmap_test,NULL};return mapper;
}
