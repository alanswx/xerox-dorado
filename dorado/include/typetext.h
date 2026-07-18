#ifndef DORADO_TYPETEXT_H
#define DORADO_TYPETEXT_H

/* Shared synthetic-typing support for the frontends: the canonical
 * ASCII -> Alto key map (one copy; dorado.c and dorado_sdl.c used to
 * carry diverging statics) and a non-blocking paced typing queue used
 * for clipboard paste. The queue lives OUTSIDE dorado_machine on
 * purpose: it is transient frontend state and must not perturb the
 * snapshot ABI. */

#include <stddef.h>
#include <stdint.h>
#include "display.h"

struct dorado_machine;

/* Map an ASCII char to an Alto key (+ whether shift is needed).
 * Returns DORADO_KEY_NONE if unmapped. */
dorado_display_key dorado_char_to_key(char c, int *shift);

typedef struct dorado_typequeue {
    char text[8192];
    size_t len;
    size_t pos;
    int phase;              /* 0 = start char, 1 = press, 2 = release */
    int active;
    int shift, ctrl;
    int nkeys;
    dorado_display_key key;
    uint64_t key_hold;      /* cycles per matrix transition */
    uint64_t due;           /* next transition not before this cycle */
} dorado_typequeue;

/* Queue `text` for paced typing (appends if already active). key_hold
 * is the per-transition hold in cycles; 1600000 is reliable for the
 * Cedar KeyBits watcher and fine for the Alto worlds. */
void dorado_typequeue_start(dorado_typequeue *q, const char *text,
                            uint64_t key_hold, uint64_t now_cycles);

/* Apply at most one due key-matrix transition. Call once or twice per
 * frontend frame (before/after the frame's run slice). Returns nonzero
 * while typing is still in progress. */
int dorado_typequeue_pump(dorado_typequeue *q, struct dorado_machine *m);

#endif /* DORADO_TYPETEXT_H */
