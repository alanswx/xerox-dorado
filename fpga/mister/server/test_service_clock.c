#include "dorado_service_clock.h"
#include <stdio.h>
#include <stdlib.h>

static void check(int ok,const char *why){if(!ok){fprintf(stderr,"SERVICE CLOCK FAIL: %s\n",why);exit(23);}}
int main(void){
    dorado_service_clock clock={0};
    check(dorado_service_clock_sample(&clock,100,0)==0,"initial epoch");
    check(dorado_service_clock_sample(&clock,132,0)==32,"ordinary elapsed time");
    check(dorado_service_clock_sample(&clock,180,1)==80,"elapsed before pause lost");
    for(uint64_t t=1000;t<1000000;t+=997)
        check(dorado_service_clock_sample(&clock,t,1)==80,"paused wall time advanced retries");
    check(dorado_service_clock_sample(&clock,UINT64_C(5000000000),0)==80,"continue jumped retry time");
    check(dorado_service_clock_sample(&clock,UINT64_C(5000000032),0)==112,"continued elapsed time differs");
    check(dorado_service_clock_sample(&clock,UINT64_C(5000000050),1)==130,"second pause elapsed time");
    /* A paused state can rebase on a different process's monotonic origin. */
    dorado_service_clock restored=clock;
    check(dorado_service_clock_sample(&restored,10,1)==130,"restore rebasing advanced retries");
    check(dorado_service_clock_sample(&restored,20,0)==130,"restored continue jumped time");
    check(dorado_service_clock_sample(&restored,52,0)==162,"restored elapsed time differs");
    puts("SERVICE CLOCK PASS: ordinary elapsed time, two pauses, pending pre-pause time, no paused-time jump, paused-state rebase");
    return 0;
}
