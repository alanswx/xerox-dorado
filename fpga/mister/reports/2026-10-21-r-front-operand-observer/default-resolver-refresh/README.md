# Default local resolver refreshed

The ignored `dorado/build/mbdis` executable still used the old physical-neighbor
resolver: CIA239 was labeled BootOrStart+1 and6c1 JLoadMiniMixer+1. Rebuilding
`make -C dorado build/mbdis` from the already-committed source corrects it.
Six requested PCs now match the previously reviewed private resolver exactly:
221=MTrap,238=BootOrStart,239/23a/6c1/6c2=RequeueSearchSQ offsets1/2/3/0.

This7-member receipt includes before/after hashes/output, original MB, source
and the build log, including its mixed-SDK linker warnings. Lossless readback
passes with the standard receipt verifier. No executable is archived. The
native simulators are statically linked to their frozen packet service and
are unaffected by rebuilding this separate analysis utility. No guest RTL,
microcode, memory or firmware image was changed.
