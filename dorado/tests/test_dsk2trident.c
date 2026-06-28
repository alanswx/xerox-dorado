#define main dsk2trident_tool_main
#include "../src/dsk2trident.c"
#undef main

#include <stdint.h>
#include <stdio.h>

#define FAIL(msg, ...) do { \
    fprintf(stderr, "FAIL: %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    return 1; \
} while (0)

#define EXPECT(cond, msg, ...) do { \
    if (!(cond)) FAIL(msg, ##__VA_ARGS__); \
} while (0)

static int test_cross_drive_rda_links_preserve_target_drive(void)
{
    uint16_t drive0_last = 0156254u;
    uint16_t drive1_first = 000002u;

    EXPECT(translate_rda_preserve_drive(drive0_last, 406, 14, 406, 14) ==
               drive0_last,
           "drive 0 RDA gained a drive bit");
    EXPECT(translate_rda_preserve_drive(drive1_first, 406, 14, 406, 14) ==
               drive1_first,
           "drive 1 RDA lost its drive bit");

    uint16_t remapped = translate_rda_preserve_drive(drive1_first,
                                                     406, 12, 406, 14);
    EXPECT((remapped & 000002u) != 0, "12->14 remap lost drive bit");

    printf("PASS  test_cross_drive_rda_links_preserve_target_drive\n");
    return 0;
}

int main(void)
{
    int rc = 0;
    rc |= test_cross_drive_rda_links_preserve_target_drive();
    return rc ? 1 : 0;
}
