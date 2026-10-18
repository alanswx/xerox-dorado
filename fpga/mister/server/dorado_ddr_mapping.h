#ifndef DORADO_DDR_MAPPING_H
#define DORADO_DDR_MAPPING_H
#include <stdio.h>
#include "dorado_ddr_control.h"
/* Pure parser is tested without opening /dev/mem. A redacted/missing RAM map
 * or any occupied aperture fails; known generic reserved entries may contain it. */
int dorado_ddr_window_reserved(FILE *);
dorado_ddr_mapper dorado_ddr_physical_mapper(void);
#endif
