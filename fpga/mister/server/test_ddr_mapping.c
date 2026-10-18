#include "dorado_ddr_mapping.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void check(const char *map,int expected) {
    FILE *f=tmpfile();if(!f){perror("tmpfile");exit(23);}
    fwrite(map,1,strlen(map),f);rewind(f);
    int result=dorado_ddr_window_reserved(f);fclose(f);
    if(result!=expected){fprintf(stderr,"DDR MAP FAIL: expected%d got%d: %s\n",expected,result,map);exit(23);}
}
int main(void) {
    check("00000000-1fefffff : System RAM\n  01000000-01149b4f : Kernel data\nfff00000-fff00fff : spi\n",0);
    check("00000000-1fefffff : System RAM\n1ff00000-3fffffff : reserved\n",0);
    check("00000000-1fefffff : System RAM\n1ff10000-1fffffff : other\n",0);
    check("00000000-1fffffff : System RAM\n",EBUSY);
    check("00000000-1fefffff : System RAM\n1ff00000-1ff00000 : other driver\n",EBUSY);
    check("00000000-1fefffff : System RAM\n1ff0ffff-1ff20000 : other driver\n",EBUSY);
    check("00000000-1fefffff : System RAM\n1ff00000-1ff0ffff : System RAM\n",EBUSY);
    check("00000000-00000000 : System RAM\n",EACCES);
    check("fff00000-fff00fff : spi\n",EACCES);
    check("1ff00000-1fefffff : reserved\n",EPROTO);
    puts("DDR MAP PARSER PASS: actual reservation shape, overlaps/boundaries/driver claims, redacted/missing/malformed maps; no physical mapping attempted");
}
