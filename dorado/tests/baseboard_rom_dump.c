/*
 * baseboard_rom_dump -- print the BaseBoard ROM as the C emulator loads it.
 *
 * The RTL's eight 2716 sockets need contents, and the authority on what is in
 * them is not firmware/*.BIN -- those 1987 chip dumps are a different set,
 * and their tails hold no plausible 6502 vector triple. It is
 * `chm/dorado/doradobaserom.mb!13`, the archive's base ROM, which is what
 * `baseboard_load_rom()` reads and therefore what the emulator actually boots.
 *
 * Using the emulator's own loader keeps the two models comparable: whatever
 * the RTL's 6502 fetches is byte-for-byte what the C one fetches.
 *
 * Output: "ADDR VALUE" in hex, one per present byte.
 */

#include "baseboard.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    const char *path = argc > 1 ? argv[1] : "../chm/dorado/doradobaserom.mb!13";
    static dorado_baseboard bb;
    baseboard_init(&bb);
    if (baseboard_load_rom(&bb, path) != 0) {
        fprintf(stderr, "%s\n", bb.halt_msg);
        return 1;
    }
    fprintf(stderr, "%s\n", bb.halt_msg);
    for (int a = 0xC000; a <= 0xFFFF; a++)
        printf("%04X %02X\n", a, bb.mem[a]);
    return 0;
}
