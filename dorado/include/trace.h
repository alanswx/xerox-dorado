#ifndef DORADO_TRACE_H
#define DORADO_TRACE_H

/*
 * Cached boolean test of a trace environment variable (DORADO_*_TRACE etc.).
 *
 * Caches by string-literal POINTER, so pass only literals used as on/off
 * flags, never value-returning env vars. Implementation in src/cpu.c.
 *
 * 256 call sites ask this on the per-step path, and even as a memoized hash
 * lookup it cost 8% of the emulator's whole runtime (`sample`, 2026-07-31):
 * an out-of-line call, a multiply and a probe, per site per microinstruction,
 * to answer "no" 114 different ways. Every key is DORADO_-prefixed, so an
 * environment holding no DORADO_* variable at all answers `no` for all of
 * them, and the test collapses to an inlined load and branch.
 * dorado_trace_env_present is -1 until the first lookup probes the
 * environment, and is only ever set once.
 *
 * This lives in its own header because memory.c, display.c and disk.c each
 * used to carry their own `extern int dorado_trace_flag(...)` -- the same
 * diverging-copies trap that put two different ASCII key maps in the
 * frontends. One declaration, one place.
 */
extern int dorado_trace_env_present;
void dorado_trace_init(void);              /* probe the environment once */
int dorado_trace_flag_lookup(const char *name);

static inline int dorado_trace_flag(const char *name)
{
    return dorado_trace_env_present == 0 ? 0 : dorado_trace_flag_lookup(name);
}

#endif
