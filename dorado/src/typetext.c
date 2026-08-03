/* Shared synthetic typing: canonical char map + paced paste queue.
 * The pacing mirrors the frontends' scripted type_text: modifiers get
 * their own matrix transition before the base key (LLKEY applies
 * simultaneous transitions in key-number order, so an unpaced shifted
 * '(' decodes as '9'), each transition holds key_hold cycles, and
 * every 5th key adds a settle gap for the guest's type-ahead. */

#include <string.h>
#include "typetext.h"

#include <stdio.h>
#include "machine.h"

dorado_display_key dorado_char_to_key(char c, int *shift)
{
    *shift = 0;
    if (c >= 'A' && c <= 'Z') { *shift = 1; c = (char)(c - 'A' + 'a'); }
    if (c >= 'a' && c <= 'z') {
        static const dorado_display_key L[26] = {
            DORADO_KEY_A, DORADO_KEY_B, DORADO_KEY_C, DORADO_KEY_D,
            DORADO_KEY_E, DORADO_KEY_F, DORADO_KEY_G, DORADO_KEY_H,
            DORADO_KEY_I, DORADO_KEY_J, DORADO_KEY_K, DORADO_KEY_L,
            DORADO_KEY_M, DORADO_KEY_N, DORADO_KEY_O, DORADO_KEY_P,
            DORADO_KEY_Q, DORADO_KEY_R, DORADO_KEY_S, DORADO_KEY_T,
            DORADO_KEY_U, DORADO_KEY_V, DORADO_KEY_W, DORADO_KEY_X,
            DORADO_KEY_Y, DORADO_KEY_Z };
        return L[c - 'a'];
    }
    switch (c) {
    case '0': return DORADO_KEY_0;  case '1': return DORADO_KEY_1;
    case '2': return DORADO_KEY_2;  case '3': return DORADO_KEY_3;
    case '4': return DORADO_KEY_4;  case '5': return DORADO_KEY_5;
    case '6': return DORADO_KEY_6;  case '7': return DORADO_KEY_7;
    case '8': return DORADO_KEY_8;  case '9': return DORADO_KEY_9;
    case ' ':  return DORADO_KEY_SPACE;
    case '\n': case '\r': return DORADO_KEY_RETURN;
    case '?': *shift = 1; return DORADO_KEY_FSLASH;
    case '/': return DORADO_KEY_FSLASH;
    /* Alto II digit-row shifts.  The whole row, not just the three the Lisp
     * forms happened to need: an unmapped character used to vanish without a
     * word, and `&` going missing turned `IRM.HOST&DIR` into `IRM.HOSTDIR`
     * and reported it as an unbound variable -- which reads as a broken init
     * file rather than a broken keyboard.
     *
     * Note 6: the Alto puts `~` there, where a modern keyboard has `^`.
     * `^` is deliberately left unmapped rather than guessed at; an explicit
     * gap that warns is better than a wrong key. */
    case '!': *shift = 1; return DORADO_KEY_1;
    case '@': *shift = 1; return DORADO_KEY_2;
    case '#': *shift = 1; return DORADO_KEY_3;
    case '$': *shift = 1; return DORADO_KEY_4;
    case '%': *shift = 1; return DORADO_KEY_5;
    case '~': *shift = 1; return DORADO_KEY_6;
    case '&': *shift = 1; return DORADO_KEY_7;
    case '(': *shift = 1; return DORADO_KEY_9;
    case ')': *shift = 1; return DORADO_KEY_0;
    case '*': *shift = 1; return DORADO_KEY_8;
    case '<': *shift = 1; return DORADO_KEY_COMMA;
    case '>': *shift = 1; return DORADO_KEY_PERIOD;
    case '.': return DORADO_KEY_PERIOD;
    case ',': return DORADO_KEY_COMMA;
    case '-': return DORADO_KEY_MINUS;
    case '_': *shift = 1; return DORADO_KEY_MINUS;   /* Cedar's <- */
    case '=': return DORADO_KEY_PLUS;
    case '+': *shift = 1; return DORADO_KEY_PLUS;
    case '[': return DORADO_KEY_LBRACKET;
    case '{': *shift = 1; return DORADO_KEY_LBRACKET;
    case ']': return DORADO_KEY_RBRACKET;
    case '}': *shift = 1; return DORADO_KEY_RBRACKET;
    case ';': return DORADO_KEY_SEMICOLON;
    case ':': *shift = 1; return DORADO_KEY_SEMICOLON;
    case '\'': return DORADO_KEY_QUOTE;
    case '"': *shift = 1; return DORADO_KEY_QUOTE;
    case '\\': return DORADO_KEY_BSLASH;
    case '|': *shift = 1; return DORADO_KEY_BSLASH;
    default: return DORADO_KEY_NONE;
    }
}

void dorado_typequeue_start(dorado_typequeue *q, const char *text,
                            uint64_t key_hold, uint64_t now_cycles)
{
    if (!q || !text) return;
    if (!q->active) {
        q->len = q->pos = 0;
        q->phase = 0;
        q->nkeys = 0;
        q->due = now_cycles;
        q->key_hold = key_hold;
        q->active = 1;
    }
    for (const char *p = text; *p && q->len < sizeof q->text; p++) {
        if (*p == '\n' && p > text && p[-1] == '\r')
            continue;                     /* CRLF -> one Return */
        q->text[q->len++] = *p;
    }
    if (q->len == 0) q->active = 0;
}

int dorado_typequeue_pump(dorado_typequeue *q, struct dorado_machine *m)
{
    if (!q || !q->active || !m) return 0;
    uint64_t now = dorado_machine_cycles(m);
    if (now < q->due) return 1;

    if (q->phase == 0) {
        int shift = 0, ctrl = 0;
        char tc;
        do {
            if (q->pos >= q->len) { q->active = 0; return 0; }
            tc = q->text[q->pos];
            if (tc > 0 && tc <= 0x1A && tc != '\n' && tc != '\r' &&
                tc != '\t') {
                ctrl = 1;
                tc = (char)(tc - 1 + 'a');
            }
            q->key = dorado_char_to_key(tc, &shift);
            if (q->key == DORADO_KEY_NONE) {
                /* Say so. A dropped character produces a subtly wrong form
                 * in the guest -- a missing `&` cost a full debugging cycle
                 * by turning a set variable into an unbound one. */
                fprintf(stderr, "dorado: cannot type '%c' (0x%02x), skipped\n",
                        (q->text[q->pos] >= 040 && q->text[q->pos] < 0177)
                            ? q->text[q->pos] : '?',
                        (unsigned char)q->text[q->pos]);
                q->pos++; ctrl = 0;
            }
        } while (q->key == DORADO_KEY_NONE);
        q->shift = shift;
        q->ctrl = ctrl;
        if (ctrl) dorado_machine_set_key(m, DORADO_KEY_CTRL, 1);
        if (shift) dorado_machine_set_key(m, DORADO_KEY_LSHIFT, 1);
        q->phase = 1;
        if (ctrl || shift) {              /* modifier observed first */
            q->due = now + q->key_hold;
            return 1;
        }
    }
    if (q->phase == 1) {
        dorado_machine_set_key(m, q->key, 1);
        q->phase = 2;
        q->due = now + q->key_hold;
        return 1;
    }
    /* phase 2: release */
    dorado_machine_set_key(m, q->key, 0);
    if (q->shift) dorado_machine_set_key(m, DORADO_KEY_LSHIFT, 0);
    if (q->ctrl) dorado_machine_set_key(m, DORADO_KEY_CTRL, 0);
    q->pos++;
    q->phase = 0;
    q->due = now + q->key_hold;
    if (++q->nkeys % 5 == 0) q->due += 3000000ull;
    if (q->pos >= q->len && q->phase == 0) {
        /* release delivered; queue drains on the next pump */
    }
    return 1;
}
