# Minimal Nova/Alto disassembler
FUNC=['COM','NEG','MOV','INC','ADC','SUB','ADD','AND']
SH=['','L','R','S']; CY=['','Z','O','C']
SKIP=['','SKP','SZC','SNC','SZR','SNR','SEZ','SBN']
IDX=['','+1@PC','+2@AC2','+3@AC3']  # index modes (page0 if 0)
def dis(w,pc):
    if w & 0x8000:  # ALC
        acs=(w>>13)&3; acd=(w>>11)&3; f=(w>>8)&7; sh=(w>>6)&3
        cy=(w>>4)&3; nl=(w>>3)&1; sk=w&7
        return f"{FUNC[f]}{SH[sh]}{CY[cy]}{'#' if nl else ''} {acs},{acd}{','+SKIP[sk] if sk else ''}"
    grp=(w>>13)&3; ind='@' if (w>>10)&1 else ''
    idx=(w>>8)&3; disp=w&0xFF
    # effective address
    if idx==0:
        ea=f"{disp:o}"          # page zero (absolute)
    else:
        mag = disp if disp < 128 else 256 - disp
        sign = '+' if disp < 128 else '-'
        if idx==1:
            ea=f".{sign}{mag:o}"
        else:
            ea=f"{sign}{mag:o}(AC{idx})"
    if grp==0:
        sub=(w>>11)&3; op=['JMP','JSR','ISZ','DSZ'][sub]
        return f"{op} {ind}{ea}"
    if grp==1: return f"LDA {(w>>11)&3},{ind}{ea}"
    if grp==2: return f"STA {(w>>11)&3},{ind}{ea}"
    return f"IO/{w:06o}"
# MC loop region 2760-3020 (from VM dump, ours)
mem={0o2760:[0o34762,0o61002,0o54434,0o61003,0o26433,0o122400,0o34432,0o162033],
     0o2770:[0o424,0o167000,0o46425,0o34424,0o25401,0o20424,0o107022,0o11400],
     0o3000:[0o101000,0o45401,0o25777,0o20417,0o107022,0o11776,0o101000,0o45777],
     0o3010:[0o11775,0o403,0o11774,0o101000,0o34402,0o1401,0o1537,0o576]}
for base in sorted(mem):
    for i,w in enumerate(mem[base]):
        a=base+i
        print(f"  {a:o}: {w:06o}  {dis(w,a)}")
