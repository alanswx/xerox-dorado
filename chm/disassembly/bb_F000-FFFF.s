
MIDASCODE:
F000: D8        CLD                
F001: 48        PHA                
F002: A5 7C     LDA $7C            
F004: F0 06     BEQ $F00C          
F006: A9 FF     LDA #$FF           
F008: 85 7D     STA $7D            
F00A: 68        PLA                
F00B: 40        RTI                
F00C: E6 7C     INC $7C            
F00E: 58        CLI                
F00F: 68        PLA                
F010: 85 64     STA $64            
F012: 86 65     STX $65            
F014: 84 66     STY $66            
F016: 68        PLA                
F017: 85 63     STA $63            
F019: 68        PLA                
F01A: 85 60     STA $60            
F01C: 68        PLA                
F01D: 85 61     STA $61            
F01F: BA        TSX                
F020: 86 62     STX $62            
F022: A9 58     LDA #$58           
F024: C5 60     CMP $60            
F026: D0 06     BNE $F02E          
F028: A9 F1     LDA #$F1           
F02A: C5 61     CMP $61            
F02C: F0 E8     BEQ $F016          
F02E: A2 FF     LDX #$FF           
F030: 8E 01 04  STX $0401          
F033: E8        INX                
F034: 8E 01 05  STX $0501          
F037: 8E 03 05  STX $0503          
F03A: A2 C0     LDX #$C0           
F03C: 8E 03 04  STX $0403          
F03F: 20 F2 F1  JSR $F1F2          
F042: A9 00     LDA #$00           
F044: 85 7D     STA $7D            
F046: AD 00 05  LDA $0500          
F049: 4D 02 04  EOR $0402          
F04C: 10 F8     BPL $F046          
F04E: AD 00 05  LDA $0500          
F051: 29 07     AND #$07           
F053: AA        TAX                
F054: AD 02 05  LDA $0502          
F057: CA        DEX                
F058: 30 11     BMI $F06B          
F05A: F0 4A     BEQ $F0A6          
F05C: CA        DEX                
F05D: F0 4C     BEQ $F0AB          
F05F: CA        DEX                
F060: F0 4E     BEQ $F0B0          
F062: CA        DEX                
F063: F0 57     BEQ $F0BC          
F065: CA        DEX                
F066: F0 5E     BEQ $F0C6          
F068: CA        DEX                
F069: F0 68     BEQ $F0D3          
F06B: AD 00 05  LDA $0500          
F06E: AA        TAX                
F06F: A9 80     LDA #$80           
F071: 4D 02 04  EOR $0402          
F074: 8D 02 04  STA $0402          
F077: 8A        TXA                
F078: 29 08     AND #$08           
F07A: F0 09     BEQ $F085          
F07C: 20 92 F6  JSR $F692            ; PACIFYWATCHDOG
F07F: A5 7D     LDA $7D            
F081: F0 F9     BEQ $F07C          
F083: D0 BD     BNE $F042          
F085: 20 1C F2  JSR $F21C          
F088: A6 62     LDX $62            
F08A: 9A        TXS                
F08B: A5 61     LDA $61            
F08D: 48        PHA                
F08E: A5 60     LDA $60            
F090: 48        PHA                
F091: A5 63     LDA $63            
F093: 48        PHA                
F094: A4 66     LDY $66            
F096: A6 65     LDX $65            
F098: A5 64     LDA $64            
F09A: 48        PHA                
F09B: A5 7D     LDA $7D            
F09D: F0 03     BEQ $F0A2          
F09F: 4C 0F F0  JMP $F00F          
F0A2: 68        PLA                
F0A3: C6 7C     DEC $7C            
F0A5: 40        RTI                
F0A6: 85 5F     STA $5F            
F0A8: 4C 6B F0  JMP $F06B          
F0AB: 85 5E     STA $5E            
F0AD: 4C 6B F0  JMP $F06B          
F0B0: 85 5E     STA $5E            
F0B2: A0 00     LDY #$00           
F0B4: B1 5E     LDA ($5E),Y        
F0B6: 8D 00 04  STA $0400          
F0B9: 4C 6B F0  JMP $F06B          
F0BC: A0 00     LDY #$00           
F0BE: B1 5E     LDA ($5E),Y        
F0C0: 8D 00 04  STA $0400          
F0C3: 4C CA F0  JMP $F0CA          
F0C6: A0 00     LDY #$00           
F0C8: 91 5E     STA ($5E),Y        
F0CA: E6 5E     INC $5E            
F0CC: D0 9D     BNE $F06B          
F0CE: E6 5F     INC $5F            
F0D0: 4C 6B F0  JMP $F06B          
F0D3: AA        TAX                
F0D4: F0 95     BEQ $F06B          
F0D6: CA        DEX                
F0D7: F0 05     BEQ $F0DE          
F0D9: CA        DEX                
F0DA: F0 33     BEQ $F10F          
F0DC: D0 F2     BNE $F0D0          
F0DE: BA        TSX                
F0DF: 8A        TXA                
F0E0: 38        SEC                
F0E1: E9 40     SBC #$40           
F0E3: AA        TAX                
F0E4: 9A        TXS                
F0E5: 20 E9 F0  JSR $F0E9          
F0E8: 00        BRK                
F0E9: 20 12 F1  JSR $F112          

MIDASRETURNPOINT:
F0EC: 08        PHP                
F0ED: E6 7C     INC $7C            
F0EF: C6 7E     DEC $7E            
F0F1: 85 6D     STA $6D            
F0F3: 68        PLA                
F0F4: 85 6C     STA $6C            
F0F6: D8        CLD                
F0F7: 18        CLC                
F0F8: 68        PLA                
F0F9: 69 01     ADC #$01           
F0FB: 85 69     STA $69            
F0FD: 68        PLA                
F0FE: 69 00     ADC #$00           
F100: 85 6A     STA $6A            
F102: 86 6E     STX $6E            
F104: 84 6F     STY $6F            
F106: BA        TSX                
F107: 86 6B     STX $6B            
F109: A6 62     LDX $62            
F10B: 9A        TXS                
F10C: 4C 6B F0  JMP $F06B          
F10F: A6 6B     LDX $6B            
F111: 9A        TXS                
F112: A5 6A     LDA $6A            
F114: 48        PHA                
F115: A5 69     LDA $69            
F117: 48        PHA                
F118: A5 6C     LDA $6C            
F11A: 48        PHA                
F11B: A4 6F     LDY $6F            
F11D: A6 6E     LDX $6E            
F11F: A5 6D     LDA $6D            
F121: C6 7C     DEC $7C            
F123: E6 7E     INC $7E            
F125: 40        RTI                

TRAPENCOUNTERED:
F126: 20 62 F2  JSR $F262            ; INHIBITMIDAS
F129: A9 FE     LDA #$FE           
F12B: A6 7E     LDX $7E            
F12D: F0 02     BEQ $F131          
F12F: A9 FD     LDA #$FD           
F131: D8        CLD                
F132: 18        CLC                
F133: BA        TSX                
F134: 7D 04 01  ADC $0104,X        
F137: 9D 04 01  STA $0104,X        
F13A: BD 05 01  LDA $0105,X        
F13D: E9 00     SBC #$00           
F13F: 9D 05 01  STA $0105,X        
F142: 68        PLA                
F143: AA        TAX                
F144: A5 7E     LDA $7E            
F146: F0 04     BEQ $F14C          
F148: 68        PLA                
F149: 4C ED F0  JMP $F0ED          
F14C: A9 BF     LDA #$BF           
F14E: 2D 02 04  AND $0402          
F151: 8D 02 04  STA $0402          
F154: 68        PLA                
F155: 20 67 F2  JSR $F267            ; ENABLEMIDAS
F158: 20 8B F6  JSR $F68B            ; PACIFYWATCHDOGIFJUMPER
F15B: 4C 58 F1  JMP $F158          
F15E: A6 97     LDX $97            
F160: 10 08     BPL $F16A          
F162: A2 3A     LDX #$3A           
F164: A0 02     LDY #$02           
F166: 86 71     STX $71            
F168: D0 22     BNE $F18C          
F16A: B5 81     LDA $81,X          
F16C: 85 71     STA $71            
F16E: A0 02     LDY #$02           
F170: B1 69     LDA ($69),Y        
F172: 99 71 00  STA $0071,Y        
F175: 88        DEY                
F176: D0 F8     BNE $F170          
F178: A2 EA     LDX #$EA           
F17A: A0 03     LDY #$03           
F17C: A5 71     LDA $71            
F17E: C9 20     CMP #$20           
F180: F0 10     BEQ $F192          
F182: 29 08     AND #$08           
F184: F0 09     BEQ $F18F          
F186: A5 71     LDA $71            
F188: 29 05     AND #$05           
F18A: D0 06     BNE $F192          
F18C: 88        DEY                
F18D: 86 72     STX $72            
F18F: 88        DEY                
F190: 86 73     STX $73            
F192: 98        TYA                
F193: 18        CLC                
F194: 65 69     ADC $69            
F196: 85 75     STA $75            
F198: A5 6A     LDA $6A            
F19A: 69 00     ADC #$00           
F19C: 85 76     STA $76            
F19E: A5 72     LDA $72            
F1A0: 18        CLC                
F1A1: 65 75     ADC $75            
F1A3: 85 78     STA $78            
F1A5: A5 72     LDA $72            
F1A7: 29 80     AND #$80           
F1A9: 10 02     BPL $F1AD          
F1AB: A9 FF     LDA #$FF           
F1AD: 65 76     ADC $76            
F1AF: 85 79     STA $79            
F1B1: A9 4C     LDA #$4C           
F1B3: 85 74     STA $74            
F1B5: 85 77     STA $77            
F1B7: A5 71     LDA $71            
F1B9: 29 1F     AND #$1F           
F1BB: C9 10     CMP #$10           
F1BD: D0 04     BNE $F1C3          
F1BF: A9 03     LDA #$03           
F1C1: 85 72     STA $72            
F1C3: 60        RTS                
F1C4: A5 71     LDA $71            
F1C6: C9 20     CMP #$20           
F1C8: D0 17     BNE $F1E1          
F1CA: A5 76     LDA $76            
F1CC: 48        PHA                
F1CD: A5 6D     LDA $6D            
F1CF: 48        PHA                
F1D0: BA        TSX                
F1D1: BD 01 01  LDA $0101,X        
F1D4: DE 01 01  DEC $0101,X        
F1D7: A8        TAY                
F1D8: D0 03     BNE $F1DD          
F1DA: DE 02 01  DEC $0102,X        
F1DD: A9 4C     LDA #$4C           
F1DF: 85 71     STA $71            
F1E1: A5 6C     LDA $6C            
F1E3: 48        PHA                
F1E4: A4 6F     LDY $6F            
F1E6: A6 6E     LDX $6E            
F1E8: A5 6D     LDA $6D            
F1EA: C6 7C     DEC $7C            
F1EC: E6 7E     INC $7E            
F1EE: 28        PLP                
F1EF: 4C 71 00  JMP $0071          
F1F2: A9 40     LDA #$40           
F1F4: 0D 02 04  ORA $0402          
F1F7: 8D 02 04  STA $0402          
F1FA: A2 15     LDX #$15           
F1FC: A9 FF     LDA #$FF           
F1FE: D5 7F     CMP $7F,X          
F200: D0 04     BNE $F206          
F202: D5 80     CMP $80,X          
F204: F0 10     BEQ $F216          
F206: A1 7F     LDA ($7F,X)        
F208: F0 08     BEQ $F212          
F20A: A9 FF     LDA #$FF           
F20C: 95 7F     STA $7F,X          
F20E: 95 80     STA $80,X          
F210: D0 04     BNE $F216          
F212: B5 81     LDA $81,X          
F214: 81 7F     STA ($7F,X)        
F216: CA        DEX                
F217: CA        DEX                
F218: CA        DEX                
F219: 10 E1     BPL $F1FC          
F21B: 60        RTS                
F21C: A9 FF     LDA #$FF           
F21E: 85 97     STA $97            
F220: A2 15     LDX #$15           
F222: A9 FF     LDA #$FF           
F224: D5 7F     CMP $7F,X          
F226: D0 04     BNE $F22C          
F228: D5 80     CMP $80,X          
F22A: F0 16     BEQ $F242          
F22C: A5 69     LDA $69            
F22E: D5 7F     CMP $7F,X          
F230: D0 08     BNE $F23A          
F232: A5 6A     LDA $6A            
F234: D5 80     CMP $80,X          
F236: D0 02     BNE $F23A          
F238: 86 97     STX $97            
F23A: A1 7F     LDA ($7F,X)        
F23C: 95 81     STA $81,X          
F23E: A9 00     LDA #$00           
F240: 81 7F     STA ($7F,X)        
F242: CA        DEX                
F243: CA        DEX                
F244: CA        DEX                
F245: 10 DB     BPL $F222          
F247: 60        RTS                

MIDASSETUP:
F248: A9 40     LDA #$40           
F24A: 0D 02 04  ORA $0402          
F24D: 8D 02 04  STA $0402          
F250: A2 17     LDX #$17           
F252: A9 FF     LDA #$FF           
F254: 95 7F     STA $7F,X          
F256: CA        DEX                
F257: 10 FB     BPL $F254          
F259: A9 00     LDA #$00           
F25B: 85 7D     STA $7D            
F25D: 85 7C     STA $7C            
F25F: 85 7E     STA $7E            
F261: 60        RTS                

INHIBITMIDAS:
F262: 08        PHP                
F263: E6 7C     INC $7C            
F265: 28        PLP                
F266: 60        RTS                

ENABLEMIDAS:
F267: 08        PHP                
F268: 48        PHA                
F269: A9 01     LDA #$01           
F26B: C5 7C     CMP $7C            
F26D: D0 12     BNE $F281          
F26F: A9 00     LDA #$00           
F271: 8D 80 05  STA $0580          
F274: A9 10     LDA #$10           
F276: 8D 82 05  STA $0582          
F279: C6 7C     DEC $7C            
F27B: A5 7D     LDA $7D            
F27D: D0 07     BNE $F286          
F27F: F0 02     BEQ $F283          
F281: C6 7C     DEC $7C            
F283: 68        PLA                
F284: 28        PLP                
F285: 60        RTS                
F286: 78        SEI                
F287: 8A        TXA                
F288: 48        PHA                
F289: BA        TSX                
F28A: 18        CLC                
F28B: D8        CLD                
F28C: BD 04 01  LDA $0104,X        
F28F: 69 01     ADC #$01           
F291: 9D 04 01  STA $0104,X        
F294: BD 05 01  LDA $0105,X        
F297: 69 00     ADC #$00           
F299: 9D 05 01  STA $0105,X        
F29C: 68        PLA                
F29D: AA        TAX                
F29E: 68        PLA                
F29F: 4C 00 F0  JMP $F000            ; MIDASCODE

RESETCODE:
F2A2: D8        CLD                
F2A3: 48        PHA                
F2A4: 8A        TXA                
F2A5: 48        PHA                
F2A6: BA        TSX                
F2A7: BD 03 01  LDA $0103,X        
F2AA: 29 10     AND #$10           
F2AC: F0 03     BEQ $F2B1          
F2AE: 4C 26 F1  JMP $F126            ; TRAPENCOUNTERED
F2B1: 6C 98 00  JMP ($0098)        
F2B4: A5 52     LDA $52            
F2B6: 48        PHA                
F2B7: 20 CF F2  JSR $F2CF            ; UPDATETIMER
F2BA: 20 11 F3  JSR $F311          
F2BD: 68        PLA                
F2BE: C5 52     CMP $52            
F2C0: F0 07     BEQ $F2C9          
F2C2: 58        CLI                
F2C3: 20 3D F3  JSR $F33D          
F2C6: 20 5A F3  JSR $F35A          
F2C9: 68        PLA                
F2CA: AA        TAX                
F2CB: 68        PLA                
F2CC: 40        RTI                
F2CD: 00        BRK                
F2CE: 08        PHP                

UPDATETIMER:
F2CF: AD 85 05  LDA $0585          
F2D2: 29 80     AND #$80           
F2D4: F0 3A     BEQ $F310          
F2D6: A2 02     LDX #$02           
F2D8: A9 04     LDA #$04           
F2DA: 38        SEC                
F2DB: ED 94 05  SBC $0594          
F2DE: 8E 9F 05  STX $059F          
F2E1: 65 9A     ADC $9A            
F2E3: 85 9A     STA $9A            
F2E5: A9 00     LDA #$00           
F2E7: 65 9B     ADC $9B            
F2E9: 85 9B     STA $9B            
F2EB: 90 0E     BCC $F2FB          
F2ED: A5 9A     LDA $9A            
F2EF: ED CD F2  SBC $F2CD          
F2F2: 85 9A     STA $9A            
F2F4: A5 9B     LDA $9B            
F2F6: ED CE F2  SBC $F2CE          
F2F9: 85 9B     STA $9B            
F2FB: A9 01     LDA #$01           
F2FD: 65 9C     ADC $9C            
F2FF: 85 9C     STA $9C            
F301: E9 31     SBC #$31           
F303: 90 0B     BCC $F310          
F305: 85 9C     STA $9C            
F307: A2 00     LDX #$00           
F309: F6 52     INC $52,X          
F30B: D0 03     BNE $F310          
F30D: E8        INX                
F30E: D0 F9     BNE $F309          
F310: 60        RTS                
F311: AD 82 04  LDA $0482          
F314: 29 40     AND #$40           
F316: D0 15     BNE $F32D          
F318: A9 00     LDA #$00           
F31A: 85 A4     STA $A4            
F31C: 85 A3     STA $A3            
F31E: A5 A1     LDA $A1            
F320: C9 03     CMP #$03           
F322: D0 02     BNE $F326          
F324: E6 A5     INC $A5            
F326: C9 04     CMP #$04           
F328: 10 02     BPL $F32C          
F32A: E6 A1     INC $A1            
F32C: 60        RTS                
F32D: A5 A3     LDA $A3            
F32F: C9 14     CMP #$14           
F331: 10 03     BPL $F336          
F333: E6 A3     INC $A3            
F335: 60        RTS                
F336: A9 00     LDA #$00           
F338: 85 A2     STA $A2            
F33A: 85 A1     STA $A1            
F33C: 60        RTS                
F33D: A5 A1     LDA $A1            
F33F: C9 01     CMP #$01           
F341: 30 0E     BMI $F351          
F343: A5 A2     LDA $A2            
F345: C9 14     CMP #$14           
F347: 10 03     BPL $F34C          
F349: E6 A2     INC $A2            
F34B: 60        RTS                
F34C: A9 00     LDA #$00           
F34E: 85 A5     STA $A5            
F350: 60        RTS                
F351: A5 A4     LDA $A4            
F353: C9 64     CMP #$64           
F355: 10 02     BPL $F359          
F357: E6 A4     INC $A4            
F359: 60        RTS                
F35A: 20 95 F3  JSR $F395          
F35D: 8A        TXA                
F35E: F0 26     BEQ $F386          
F360: E4 9D     CPX $9D            
F362: 30 02     BMI $F366          
F364: 86 9D     STX $9D            
F366: E6 9E     INC $9E            
F368: 30 13     BMI $F37D          
F36A: A5 9E     LDA $9E            
F36C: 4A        LSR A              
F36D: 4A        LSR A              
F36E: B0 0D     BCS $F37D          
F370: C5 9D     CMP $9D            
F372: 30 18     BMI $F38C          
F374: 20 95 F3  JSR $F395          
F377: 86 9D     STX $9D            
F379: A9 F3     LDA #$F3           
F37B: 85 9E     STA $9E            
F37D: A9 80     LDA #$80           
F37F: 0D 82 04  ORA $0482          
F382: 8D 82 04  STA $0482          
F385: 60        RTS                
F386: 86 9D     STX $9D            
F388: A9 F3     LDA #$F3           
F38A: 85 9E     STA $9E            
F38C: A9 7F     LDA #$7F           
F38E: 2D 82 04  AND $0482          
F391: 8D 82 04  STA $0482          
F394: 60        RTS                
F395: A2 00     LDX #$00           
F397: 38        SEC                
F398: A5 68     LDA $68            
F39A: E9 01     SBC #$01           
F39C: 49 FF     EOR #$FF           
F39E: 25 68     AND $68            
F3A0: F0 04     BEQ $F3A6          
F3A2: E8        INX                
F3A3: 0A        ASL A              
F3A4: D0 FC     BNE $F3A2          
F3A6: 60        RTS                
F3A7: D8        CLD                
F3A8: A2 00     LDX #$00           
F3AA: A9 00     LDA #$00           
F3AC: 95 00     STA $00,X          
F3AE: CA        DEX                
F3AF: D0 FB     BNE $F3AC          
F3B1: A9 0F     LDA #$0F           
F3B3: 85 00     STA $00            
F3B5: A2 FF     LDX #$FF           
F3B7: 9A        TXS                
F3B8: A9 B4     LDA #$B4           
F3BA: 85 98     STA $98            
F3BC: A9 F2     LDA #$F2           
F3BE: 85 99     STA $99            
F3C0: 20 48 F2  JSR $F248            ; MIDASSETUP
F3C3: A2 FF     LDX #$FF           
F3C5: 8E 01 04  STX $0401          
F3C8: 8E 81 05  STX $0581          
F3CB: E8        INX                
F3CC: 8E 03 05  STX $0503          
F3CF: 8E 01 05  STX $0501          
F3D2: A2 E0     LDX #$E0           
F3D4: 8E 81 04  STX $0481          
F3D7: A2 C0     LDX #$C0           
F3D9: 8E 03 04  STX $0403          
F3DC: A2 80     LDX #$80           
F3DE: 8E 82 04  STX $0482          
F3E1: 8E 83 04  STX $0483          
F3E4: A2 F1     LDX #$F1           
F3E6: 8E 83 05  STX $0583          
F3E9: A2 40     LDX #$40           
F3EB: 8E 01 06  STX $0601          
F3EE: A9 A3     LDA #$A3           
F3F0: 8D 7B 00  STA $007B          
F3F3: A9 01     LDA #$01           
F3F5: 8D 9C 05  STA $059C          
F3F8: 58        CLI                
F3F9: 20 05 F7  JSR $F705            ; INITENVIRONMENT
F3FC: AD 82 05  LDA $0582          
F3FF: 29 02     AND #$02           
F401: F0 15     BEQ $F418            ; REBOOTDORADO
F403: A2 30     LDX #$30           
F405: 20 86 F9  JSR $F986            ; READMUFFLER
F408: D0 0E     BNE $F418            ; REBOOTDORADO
F40A: A9 08     LDA #$08           
F40C: 20 E7 F4  JSR $F4E7            ; SETPROBLEM
F40F: 20 9F F5  JSR $F59F            ; CHECKBOOTBUTTON
F412: 20 21 F7  JSR $F721            ; DOALLCONVERSIONS
F415: 4C 0F F4  JMP $F40F          

REBOOTDORADO:
F418: 20 92 F6  JSR $F692            ; PACIFYWATCHDOG
F41B: A9 80     LDA #$80           
F41D: 20 E7 F4  JSR $F4E7            ; SETPROBLEM
F420: A9 7F     LDA #$7F           
F422: 20 EC F4  JSR $F4EC            ; CLEARPROBLEM
F425: 20 21 F7  JSR $F721            ; DOALLCONVERSIONS
F428: A2 10     LDX #$10           
F42A: 20 5A F9  JSR $F95A            ; SETMANIFOLD
F42D: 20 48 F9  JSR $F948            ; STOPDORADO
F430: AD 01 00  LDA $0001          
F433: 29 44     AND #$44           
F435: F0 0E     BEQ $F445          
F437: 20 92 F6  JSR $F692            ; PACIFYWATCHDOG
F43A: 20 05 F7  JSR $F705            ; INITENVIRONMENT
F43D: A9 10     LDA #$10           
F43F: 20 E7 F4  JSR $F4E7            ; SETPROBLEM
F442: 4C 28 F4  JMP $F428          
F445: A9 10     LDA #$10           
F447: 20 EC F4  JSR $F4EC            ; CLEARPROBLEM
F44A: A2 2C     LDX #$2C           
F44C: 20 86 F9  JSR $F986            ; READMUFFLER
F44F: F0 14     BEQ $F465          
F451: A2 00     LDX #$00           
F453: 20 5A F9  JSR $F95A            ; SETMANIFOLD
F456: A2 14     LDX #$14           
F458: 20 B3 F4  JSR $F4B3          
F45B: A2 02     LDX #$02           
F45D: 20 5A F9  JSR $F95A            ; SETMANIFOLD
F460: A2 14     LDX #$14           
F462: 20 B3 F4  JSR $F4B3          
F465: 20 21 F7  JSR $F721            ; DOALLCONVERSIONS
F468: AD 01 00  LDA $0001          
F46B: F0 22     BEQ $F48F          
F46D: A2 04     LDX #$04           
F46F: 20 5A F9  JSR $F95A            ; SETMANIFOLD
F472: A2 04     LDX #$04           
F474: 20 BA F4  JSR $F4BA            ; DELAY
F477: A2 06     LDX #$06           
F479: 20 5A F9  JSR $F95A            ; SETMANIFOLD
F47C: AD 01 00  LDA $0001          
F47F: F0 0E     BEQ $F48F          
F481: 20 92 F6  JSR $F692            ; PACIFYWATCHDOG
F484: 20 05 F7  JSR $F705            ; INITENVIRONMENT
F487: A9 10     LDA #$10           
F489: 20 E7 F4  JSR $F4E7            ; SETPROBLEM
F48C: 4C 7C F4  JMP $F47C          
F48F: A9 10     LDA #$10           
F491: 20 EC F4  JSR $F4EC            ; CLEARPROBLEM
F494: A2 01     LDX #$01           
F496: 20 BA F4  JSR $F4BA            ; DELAY
F499: A2 12     LDX #$12           
F49B: 20 5A F9  JSR $F95A            ; SETMANIFOLD
F49E: A2 14     LDX #$14           
F4A0: 20 5A F9  JSR $F95A            ; SETMANIFOLD
F4A3: A2 02     LDX #$02           
F4A5: 20 BA F4  JSR $F4BA            ; DELAY
F4A8: 20 AE FA  JSR $FAAE            ; LOADDORADOCODE
F4AB: A9 80     LDA #$80           
F4AD: 20 EC F4  JSR $F4EC            ; CLEARPROBLEM
F4B0: 4C F3 F4  JMP $F4F3            ; CONTINUOUSCODE
F4B3: AD 82 05  LDA $0582          
F4B6: 29 02     AND #$02           
F4B8: F0 2C     BEQ $F4E6          

DELAY:
F4BA: 78        SEI                
F4BB: A5 52     LDA $52            
F4BD: A4 53     LDY $53            
F4BF: 58        CLI                
F4C0: 85 9F     STA $9F            
F4C2: 84 A0     STY $A0            
F4C4: A0 09     LDY #$09           
F4C6: 8A        TXA                
F4C7: 18        CLC                
F4C8: 65 9F     ADC $9F            
F4CA: 85 9F     STA $9F            
F4CC: A9 00     LDA #$00           
F4CE: 65 A0     ADC $A0            
F4D0: 85 A0     STA $A0            
F4D2: 88        DEY                
F4D3: 10 F1     BPL $F4C6          
F4D5: 20 92 F6  JSR $F692            ; PACIFYWATCHDOG
F4D8: A5 53     LDA $53            
F4DA: C5 A0     CMP $A0            
F4DC: 30 F7     BMI $F4D5          
F4DE: D0 06     BNE $F4E6          
F4E0: A5 52     LDA $52            
F4E2: C5 9F     CMP $9F            
F4E4: 90 EF     BCC $F4D5          
F4E6: 60        RTS                

SETPROBLEM:
F4E7: 05 68     ORA $68            
F4E9: 85 68     STA $68            
F4EB: 60        RTS                

CLEARPROBLEM:
F4EC: 49 FF     EOR #$FF           
F4EE: 25 68     AND $68            
F4F0: 85 68     STA $68            
F4F2: 60        RTS                

CONTINUOUSCODE:
F4F3: 20 21 F7  JSR $F721            ; DOALLCONVERSIONS
F4F6: A9 00     LDA #$00           
F4F8: 8D A5 00  STA $00A5          
F4FB: 8D C0 00  STA $00C0          
F4FE: 8D BC 00  STA $00BC          
F501: A9 00     LDA #$00           
F503: 8D BD 00  STA $00BD          
F506: A9 20     LDA #$20           
F508: 8D BE 00  STA $00BE          

SPIN:
F50B: AD BC 00  LDA $00BC          
F50E: F0 10     BEQ $F520          
F510: 20 16 F5  JSR $F516          
F513: 4C 0B F5  JMP $F50B            ; SPIN
F516: AD BE 00  LDA $00BE          
F519: 48        PHA                
F51A: AD BD 00  LDA $00BD          
F51D: 48        PHA                
F51E: 08        PHP                
F51F: 40        RTI                
F520: 20 9F F5  JSR $F59F            ; CHECKBOOTBUTTON
F523: 20 21 F7  JSR $F721            ; DOALLCONVERSIONS
F526: A5 01     LDA $01            
F528: F0 1B     BEQ $F545          
F52A: A9 10     LDA #$10           
F52C: 20 E7 F4  JSR $F4E7            ; SETPROBLEM
F52F: A9 20     LDA #$20           
F531: 20 E7 F4  JSR $F4E7            ; SETPROBLEM
F534: 8D 67 00  STA $0067          
F537: A2 05     LDX #$05           
F539: 78        SEI                
F53A: B5 52     LDA $52,X          
F53C: 95 58     STA $58,X          
F53E: CA        DEX                
F53F: 10 F9     BPL $F53A          
F541: 58        CLI                
F542: 4C 4A F5  JMP $F54A          
F545: A9 10     LDA #$10           
F547: 20 EC F4  JSR $F4EC            ; CLEARPROBLEM
F54A: A5 0B     LDA $0B            
F54C: C9 FF     CMP #$FF           
F54E: F0 BB     BEQ $F50B            ; SPIN
F550: CD 7B 00  CMP $007B          
F553: 90 B6     BCC $F50B            ; SPIN
F555: A9 04     LDA #$04           
F557: 20 E7 F4  JSR $F4E7            ; SETPROBLEM
F55A: 4C 66 F5  JMP $F566          
F55D: 20 21 F7  JSR $F721            ; DOALLCONVERSIONS

ONCEONLY:
F560: A9 00     LDA #$00           
F562: 8D BC 00  STA $00BC          
F565: 60        RTS                
F566: A9 08     LDA #$08           
F568: 20 E7 F4  JSR $F4E7            ; SETPROBLEM
F56B: 20 48 F9  JSR $F948            ; STOPDORADO
F56E: A2 16     LDX #$16           
F570: 20 5A F9  JSR $F95A            ; SETMANIFOLD
F573: A2 10     LDX #$10           
F575: 20 5A F9  JSR $F95A            ; SETMANIFOLD
F578: A2 08     LDX #$08           
F57A: 20 5A F9  JSR $F95A            ; SETMANIFOLD
F57D: A2 14     LDX #$14           
F57F: 20 BA F4  JSR $F4BA            ; DELAY
F582: A2 0A     LDX #$0A           
F584: 20 5A F9  JSR $F95A            ; SETMANIFOLD
F587: A2 0C     LDX #$0C           
F589: 20 5A F9  JSR $F95A            ; SETMANIFOLD
F58C: A2 02     LDX #$02           
F58E: 20 BA F4  JSR $F4BA            ; DELAY
F591: A2 0E     LDX #$0E           
F593: 20 5A F9  JSR $F95A            ; SETMANIFOLD
F596: 20 9F F5  JSR $F59F            ; CHECKBOOTBUTTON
F599: 20 21 F7  JSR $F721            ; DOALLCONVERSIONS
F59C: 4C 96 F5  JMP $F596          

CHECKBOOTBUTTON:
F59F: 20 92 F6  JSR $F692            ; PACIFYWATCHDOG
F5A2: AD 68 00  LDA $0068          
F5A5: 29 08     AND #$08           
F5A7: D0 5A     BNE $F603          
F5A9: A9 00     LDA #$00           
F5AB: 8D C0 00  STA $00C0          
F5AE: AD A4 00  LDA $00A4          
F5B1: C9 0F     CMP #$0F           
F5B3: 30 0C     BMI $F5C1          
F5B5: A9 00     LDA #$00           
F5B7: AE A5 00  LDX $00A5          
F5BA: 8D A5 00  STA $00A5          
F5BD: E0 01     CPX #$01           
F5BF: 10 1F     BPL $F5E0          
F5C1: AD 82 04  LDA $0482          
F5C4: 29 0F     AND #$0F           
F5C6: 8D BF 00  STA $00BF          
F5C9: AD 82 04  LDA $0482          
F5CC: 29 0F     AND #$0F           
F5CE: CD BF 00  CMP $00BF          
F5D1: D0 EE     BNE $F5C1          
F5D3: C9 02     CMP #$02           
F5D5: F0 18     BEQ $F5EF          
F5D7: C9 03     CMP #$03           
F5D9: F0 11     BEQ $F5EC          
F5DB: C9 04     CMP #$04           
F5DD: F0 20     BEQ $F5FF          
F5DF: 60        RTS                
F5E0: CA        DEX                
F5E1: F0 FC     BEQ $F5DF          
F5E3: CA        DEX                
F5E4: F0 15     BEQ $F5FB          
F5E6: E0 05     CPX #$05           
F5E8: 90 15     BCC $F5FF          
F5EA: B0 0A     BCS $F5F6          
F5EC: 20 29 F6  JSR $F629          
F5EF: AD 82 05  LDA $0582          
F5F2: 29 02     AND #$02           
F5F4: F0 E9     BEQ $F5DF          
F5F6: 68        PLA                
F5F7: 68        PLA                
F5F8: 4C 66 F5  JMP $F566          
F5FB: 20 4A FB  JSR $FB4A            ; STARTDORADOATINITMAP
F5FE: 60        RTS                
F5FF: 20 AE FA  JSR $FAAE            ; LOADDORADOCODE
F602: 60        RTS                
F603: AD A4 00  LDA $00A4          
F606: C9 0F     CMP #$0F           
F608: 30 10     BMI $F61A          
F60A: A9 00     LDA #$00           
F60C: AE A5 00  LDX $00A5          
F60F: 8D A5 00  STA $00A5          
F612: E0 01     CPX #$01           
F614: 30 04     BMI $F61A          
F616: E0 07     CPX #$07           
F618: 30 05     BMI $F61F          
F61A: 20 78 F6  JSR $F678          
F61D: 90 C0     BCC $F5DF          
F61F: A9 00     LDA #$00           
F621: 8D C0 00  STA $00C0          
F624: 68        PLA                
F625: 68        PLA                
F626: 4C 18 F4  JMP $F418            ; REBOOTDORADO
F629: 20 48 F9  JSR $F948            ; STOPDORADO
F62C: A2 05     LDX #$05           
F62E: A9 00     LDA #$00           
F630: 9D C1 00  STA $00C1,X        
F633: CA        DEX                
F634: 10 FA     BPL $F630          
F636: A9 03     LDA #$03           
F638: 8D BF 00  STA $00BF          
F63B: AD BF 00  LDA $00BF          
F63E: 20 AC F8  JSR $F8AC            ; READBNIBBLE
F641: 48        PHA                
F642: AD BF 00  LDA $00BF          
F645: 4A        LSR A              
F646: 29 01     AND #$01           
F648: 49 01     EOR #$01           
F64A: AA        TAX                
F64B: 68        PLA                
F64C: A0 03     LDY #$03           
F64E: 4A        LSR A              
F64F: 7E C2 00  ROR $00C2,X        
F652: 88        DEY                
F653: 10 F9     BPL $F64E          
F655: CE BF 00  DEC $00BF          
F658: 10 E1     BPL $F63B          
F65A: A2 05     LDX #$05           
F65C: 8E BF 00  STX $00BF          
F65F: A2 00     LDX #$00           
F661: 18        CLC                
F662: 78        SEI                
F663: B5 52     LDA $52,X          
F665: 7D C1 00  ADC $00C1,X        
F668: 9D C1 00  STA $00C1,X        
F66B: E8        INX                
F66C: CE BF 00  DEC $00BF          
F66F: 10 F2     BPL $F663          
F671: A9 01     LDA #$01           
F673: 8D C0 00  STA $00C0          
F676: 58        CLI                
F677: 60        RTS                
F678: 18        CLC                
F679: AD C0 00  LDA $00C0          
F67C: F0 0C     BEQ $F68A          
F67E: A2 05     LDX #$05           
F680: B5 52     LDA $52,X          
F682: DD C1 00  CMP $00C1,X        
F685: 90 03     BCC $F68A          
F687: CA        DEX                
F688: 10 F6     BPL $F680          
F68A: 60        RTS                

PACIFYWATCHDOGIFJUMPER:
F68B: AD 82 05  LDA $0582          
F68E: 29 02     AND #$02           
F690: D0 16     BNE $F6A8          

PACIFYWATCHDOG:
F692: AD 00 06  LDA $0600          
F695: 29 80     AND #$80           
F697: 6A        ROR A              
F698: 8D C7 00  STA $00C7          
F69B: 78        SEI                
F69C: AD 00 06  LDA $0600          
F69F: 29 BF     AND #$BF           
F6A1: 0D C7 00  ORA $00C7          
F6A4: 8D 00 06  STA $0600          
F6A7: 58        CLI                
F6A8: 60        RTS                

MUFFLERTABLE:
F6A9: 40        RTI                
F6AA: 4C 60 4C  JMP $4C60          
F6AD: E0 4C     CPX #$4C           
F6AF: F0 4C     BEQ $F6FD          
F6B1: D0 4C     BNE $F6FF          
F6B3: 90 4C     BCC $F701          
F6B5: 80        ???  ; illegal $80 
F6B6: 4C 00 4C  JMP $4C00          
F6B9: 10 4A     BPL $F705            ; INITENVIRONMENT
F6BB: 30 48     BMI $F705            ; INITENVIRONMENT
F6BD: F0 49     BEQ $F708          
F6BF: 00        BRK                
F6C0: 4B        ???  ; illegal $4B 
F6C1: 00        BRK                
F6C2: 04        ???  ; illegal $04 
F6C3: 90 4A     BCC $F70F          
F6C5: 00        BRK                
F6C6: 03        ???  ; illegal $03 
F6C7: 00        BRK                
F6C8: 05 00     ORA $00            
F6CA: 08        PHP                
F6CB: 00        BRK                
F6CC: 0C        ???  ; illegal $0C 
F6CD: 00        BRK                
F6CE: 10 00     BPL $F6D0          
F6D0: 14        ???  ; illegal $14 
F6D1: 00        BRK                
F6D2: 18        CLC                
F6D3: 00        BRK                
F6D4: 1E A0 49  ASL $49A0,X        
F6D7: 80        ???  ; illegal $80 
F6D8: 48        PHA                
F6D9: 80        ???  ; illegal $80 
F6DA: 49 00     EOR #$00           
F6DC: 48        PHA                
F6DD: 10 00     BPL $F6DF            ; THERMOMMUFADDR

THERMOMMUFADDR:
F6DF: D0 48     BNE $F729          
F6E1: 00        BRK                
F6E2: 07        ???  ; illegal $07 
F6E3: 20 0C 80  JSR $800C          
F6E6: 18        CLC                
F6E7: 00        BRK                
F6E8: 18        CLC                
F6E9: A0 56     LDY #$56           
F6EB: 10 20     BPL $F70D          
F6ED: 70 3C     BVS $F72B          
F6EF: 70 28     BVS $F719          
F6F1: 00        BRK                
F6F2: 40        RTI                
F6F3: 00        BRK                
F6F4: 00        BRK                

ANALOGCODE:
F6F5: E0 E0     CPX #$E0           
F6F7: E0 E0     CPX #$E0           
F6F9: A0 A0     LDY #$A0           
F6FB: A0 A0     LDY #$A0           
F6FD: FF        ???  ; illegal $FF 
F6FE: FF        ???  ; illegal $FF 
F6FF: FF        ???  ; illegal $FF 
F700: FF        ???  ; illegal $FF 
F701: 00        BRK                
F702: 00        BRK                
F703: 00        BRK                
F704: 00        BRK                

INITENVIRONMENT:
F705: A2 13     LDX #$13           
F707: A9 00     LDA #$00           
F709: 95 3E     STA $3E,X          
F70B: A9 FF     LDA #$FF           
F70D: 95 2A     STA $2A,X          
F70F: 95 02     STA $02,X          
F711: CA        DEX                
F712: 10 F3     BPL $F707          
F714: 20 21 F7  JSR $F721            ; DOALLCONVERSIONS
F717: A2 13     LDX #$13           
F719: B5 02     LDA $02,X          
F71B: 95 16     STA $16,X          
F71D: CA        DEX                
F71E: 10 F9     BPL $F719          
F720: 60        RTS                

DOALLCONVERSIONS:
F721: 20 4C F7  JSR $F74C            ; VOLTAGECONVERSIONS
F724: 20 7A F7  JSR $F77A            ; CURRENTCONVERSIONS
F727: 20 CB F7  JSR $F7CB            ; TEMPERATURECONVERSIONS
F72A: A2 13     LDX #$13           
F72C: B5 02     LDA $02,X          
F72E: D5 2A     CMP $2A,X          
F730: B0 02     BCS $F734          
F732: 95 2A     STA $2A,X          
F734: C9 FF     CMP #$FF           
F736: F0 06     BEQ $F73E          
F738: D5 3E     CMP $3E,X          
F73A: 90 02     BCC $F73E          
F73C: 95 3E     STA $3E,X          
F73E: CA        DEX                
F73F: 10 EB     BPL $F72C          
F741: A5 AD     LDA $AD            
F743: 4A        LSR A              
F744: 4A        LSR A              
F745: 4A        LSR A              
F746: 4A        LSR A              
F747: 05 AE     ORA $AE            
F749: 85 01     STA $01            
F74B: 60        RTS                

VOLTAGECONVERSIONS:
F74C: A9 00     LDA #$00           
F74E: 85 AE     STA $AE            
F750: A9 01     LDA #$01           
F752: 20 6D F7  JSR $F76D            ; VICONVSETUP
F755: 20 3B F8  JSR $F83B            ; ADCONVERT
F758: 95 02     STA $02,X          
F75A: DD F9 F6  CMP $F6F9,X        
F75D: 90 05     BCC $F764          
F75F: DD F5 F6  CMP $F6F5,X        
F762: 90 01     BCC $F765          
F764: 38        SEC                
F765: 66 AE     ROR $AE            
F767: 06 A8     ASL $A8            
F769: CA        DEX                
F76A: 10 E9     BPL $F755          
F76C: 60        RTS                

VICONVSETUP:
F76D: 85 A8     STA $A8            
F76F: A9 04     LDA #$04           
F771: 85 A7     STA $A7            
F773: A9 80     LDA #$80           
F775: 85 A6     STA $A6            
F777: A2 03     LDX #$03           
F779: 60        RTS                

CURRENTCONVERSIONS:
F77A: A9 00     LDA #$00           
F77C: 85 AD     STA $AD            
F77E: A9 10     LDA #$10           
F780: 20 6D F7  JSR $F76D            ; VICONVSETUP
F783: A9 A0     LDA #$A0           
F785: 20 BC F7  JSR $F7BC            ; DOCURRENTCONVERSION
F788: 85 AB     STA $AB            
F78A: A9 C0     LDA #$C0           
F78C: 20 BC F7  JSR $F7BC            ; DOCURRENTCONVERSION
F78F: 85 AC     STA $AC            
F791: A9 80     LDA #$80           
F793: 20 BC F7  JSR $F7BC            ; DOCURRENTCONVERSION
F796: 38        SEC                
F797: E5 AC     SBC $AC            
F799: B0 02     BCS $F79D          
F79B: A9 00     LDA #$00           
F79D: 95 06     STA $06,X          
F79F: DD 01 F7  CMP $F701,X        
F7A2: 90 05     BCC $F7A9          
F7A4: DD FD F6  CMP $F6FD,X        
F7A7: 90 01     BCC $F7AA          
F7A9: 38        SEC                
F7AA: 66 AD     ROR $AD            
F7AC: A5 AB     LDA $AB            
F7AE: 85 AC     STA $AC            
F7B0: AD 80 04  LDA $0480          
F7B3: 29 E0     AND #$E0           
F7B5: 38        SEC                
F7B6: E9 20     SBC #$20           
F7B8: CA        DEX                
F7B9: 10 D8     BPL $F793          
F7BB: 60        RTS                

DOCURRENTCONVERSION:
F7BC: 8D 80 04  STA $0480          
F7BF: A9 0B     LDA #$0B           
F7C1: 85 AA     STA $AA            
F7C3: C6 AA     DEC $AA            
F7C5: D0 FC     BNE $F7C3          
F7C7: 20 3B F8  JSR $F83B            ; ADCONVERT
F7CA: 60        RTS                

TEMPERATURECONVERSIONS:
F7CB: A5 AE     LDA $AE            
F7CD: 29 70     AND #$70           
F7CF: D0 5E     BNE $F82F          
F7D1: A9 04     LDA #$04           
F7D3: 85 A7     STA $A7            
F7D5: A9 82     LDA #$82           
F7D7: 85 A6     STA $A6            
F7D9: A9 10     LDA #$10           
F7DB: 20 39 F8  JSR $F839          
F7DE: 85 0A     STA $0A            
F7E0: A9 20     LDA #$20           
F7E2: 20 39 F8  JSR $F839          
F7E5: 85 0B     STA $0B            
F7E7: A9 04     LDA #$04           
F7E9: 85 A7     STA $A7            
F7EB: A9 02     LDA #$02           
F7ED: 85 A6     STA $A6            
F7EF: A9 01     LDA #$01           
F7F1: 85 A8     STA $A8            
F7F3: A9 0A     LDA #$0A           
F7F5: 85 AF     STA $AF            
F7F7: A2 2E     LDX #$2E           
F7F9: 20 86 F9  JSR $F986            ; READMUFFLER
F7FC: B0 2D     BCS $F82B          
F7FE: F0 2B     BEQ $F82B          
F800: A5 AF     LDA $AF            
F802: 0A        ASL A              
F803: A8        TAY                
F804: B9 E0 F6  LDA $F6E0,Y        
F807: 8D BA 00  STA $00BA          
F80A: B9 DF F6  LDA $F6DF,Y        
F80D: 8D B9 00  STA $00B9          
F810: 20 D0 F9  JSR $F9D0            ; SETMUFFLERADDRESS
F813: B0 13     BCS $F828          
F815: 20 3B F8  JSR $F83B            ; ADCONVERT
F818: 20 67 F2  JSR $F267            ; ENABLEMIDAS
F81B: A6 AF     LDX $AF            
F81D: 95 0A     STA $0A,X          
F81F: C6 AF     DEC $AF            
F821: A5 AF     LDA $AF            
F823: C9 02     CMP #$02           
F825: 10 D0     BPL $F7F7          
F827: 60        RTS                
F828: 20 67 F2  JSR $F267            ; ENABLEMIDAS
F82B: A9 00     LDA #$00           
F82D: F0 EC     BEQ $F81B          
F82F: A2 0A     LDX #$0A           
F831: A9 00     LDA #$00           
F833: 95 0A     STA $0A,X          
F835: CA        DEX                
F836: 10 FB     BPL $F833          
F838: 60        RTS                
F839: 85 A8     STA $A8            

ADCONVERT:
F83B: 20 62 F2  JSR $F262            ; INHIBITMIDAS
F83E: AD 00 04  LDA $0400          
F841: 48        PHA                
F842: A9 00     LDA #$00           
F844: A8        TAY                
F845: 85 A9     STA $A9            
F847: 38        SEC                
F848: B0 18     BCS $F862          
F84A: 45 A9     EOR $A9            
F84C: 8D 00 04  STA $0400          
F84F: A9 02     LDA #$02           
F851: 85 AA     STA $AA            
F853: C6 AA     DEC $AA            
F855: D0 FC     BNE $F853          
F857: B1 A6     LDA ($A6),Y        
F859: 25 A8     AND $A8            
F85B: F0 02     BEQ $F85F          
F85D: A5 A9     LDA $A9            
F85F: 4D 00 04  EOR $0400          
F862: 66 A9     ROR $A9            
F864: D0 E4     BNE $F84A          
F866: A8        TAY                
F867: 68        PLA                
F868: 8D 00 04  STA $0400          
F86B: 98        TYA                
F86C: 4C 67 F2  JMP $F267            ; ENABLEMIDAS

SETCPREG~:
F86F: A5 B6     LDA $B6            
F871: 49 FF     EOR #$FF           
F873: 20 95 F8  JSR $F895            ; SETCPREG0
F876: A5 B5     LDA $B5            
F878: 49 FF     EOR #$FF           
F87A: 38        SEC                
F87B: 4C 86 F8  JMP $F886            ; SETCPREG1

SETCPREG:
F87E: A5 B6     LDA $B6            
F880: 20 95 F8  JSR $F895            ; SETCPREG0
F883: A5 B5     LDA $B5            
F885: 38        SEC                

SETCPREG1:
F886: 8D 80 05  STA $0580          
F889: A9 60     LDA #$60           
F88B: 6A        ROR A              
F88C: D0 14     BNE $F8A2          

DOCLOCK:
F88E: 8D 80 05  STA $0580          
F891: A9 10     LDA #$10           
F893: D0 0D     BNE $F8A2          

SETCPREG0:
F895: 8D 80 05  STA $0580          
F898: A9 20     LDA #$20           
F89A: D0 06     BNE $F8A2          

DOCONTROL:
F89C: 8D 80 05  STA $0580          
F89F: A9 00     LDA #$00           
F8A1: 6A        ROR A              
F8A2: 8D 82 05  STA $0582          
F8A5: EE 82 05  INC $0582          
F8A8: CE 82 05  DEC $0582          
F8AB: 60        RTS                

READBNIBBLE:
F8AC: 20 1F FA  JSR $FA1F            ; WAITFORCPCONTROL
F8AF: 0A        ASL A              
F8B0: 0A        ASL A              
F8B1: 0A        ASL A              
F8B2: 0A        ASL A              
F8B3: 0A        ASL A              
F8B4: 8D 80 05  STA $0580          
F8B7: AD 02 04  LDA $0402          
F8BA: 29 1E     AND #$1E           
F8BC: 4A        LSR A              
F8BD: 4C 67 F2  JMP $F267            ; ENABLEMIDAS

RUNDORADOINSTRUCTIONSTREAM:
F8C0: 20 1F FA  JSR $FA1F            ; WAITFORCPCONTROL
F8C3: A9 00     LDA #$00           
F8C5: 20 8E F8  JSR $F88E            ; DOCLOCK
F8C8: A9 00     LDA #$00           
F8CA: F0 0A     BEQ $F8D6          

DODORADOMICROINST:
F8CC: 20 1F FA  JSR $FA1F            ; WAITFORCPCONTROL
F8CF: A9 21     LDA #$21           
F8D1: 20 8E F8  JSR $F88E            ; DOCLOCK
F8D4: A9 01     LDA #$01           
F8D6: 85 B2     STA $B2            
F8D8: A9 4E     LDA #$4E           
F8DA: 18        CLC                
F8DB: 20 9C F8  JSR $F89C            ; DOCONTROL
F8DE: A9 00     LDA #$00           
F8E0: 38        SEC                
F8E1: 20 9C F8  JSR $F89C            ; DOCONTROL
F8E4: A0 00     LDY #$00           
F8E6: B1 B0     LDA ($B0),Y        
F8E8: 85 B3     STA $B3            
F8EA: C8        INY                
F8EB: B1 B0     LDA ($B0),Y        
F8ED: 8D 80 05  STA $0580          
F8F0: A9 80     LDA #$80           
F8F2: 06 B3     ASL $B3            
F8F4: 6A        ROR A              
F8F5: 8D 82 05  STA $0582          
F8F8: EE 82 05  INC $0582          
F8FB: CE 82 05  DEC $0582          
F8FE: C8        INY                
F8FF: B1 B0     LDA ($B0),Y        
F901: 8D 80 05  STA $0580          
F904: A9 A0     LDA #$A0           
F906: 06 B3     ASL $B3            
F908: 6A        ROR A              
F909: 8D 82 05  STA $0582          
F90C: EE 82 05  INC $0582          
F90F: CE 82 05  DEC $0582          
F912: C8        INY                
F913: B1 B0     LDA ($B0),Y        
F915: 8D 80 05  STA $0580          
F918: A9 C0     LDA #$C0           
F91A: 06 B3     ASL $B3            
F91C: 6A        ROR A              
F91D: 8D 82 05  STA $0582          
F920: EE 82 05  INC $0582          
F923: CE 82 05  DEC $0582          
F926: C8        INY                
F927: B1 B0     LDA ($B0),Y        
F929: 8D 80 05  STA $0580          
F92C: A9 E0     LDA #$E0           
F92E: 06 B3     ASL $B3            
F930: 6A        ROR A              
F931: 8D 82 05  STA $0582          
F934: EE 82 05  INC $0582          
F937: CE 82 05  DEC $0582          
F93A: A5 B2     LDA $B2            
F93C: 4A        LSR A              
F93D: A9 01     LDA #$01           
F93F: 20 9C F8  JSR $F89C            ; DOCONTROL
F942: A5 B2     LDA $B2            
F944: F0 11     BEQ $F957          
F946: D0 03     BNE $F94B          

STOPDORADO:
F948: 20 1F FA  JSR $FA1F            ; WAITFORCPCONTROL
F94B: A9 01     LDA #$01           
F94D: 38        SEC                
F94E: 20 9C F8  JSR $F89C            ; DOCONTROL
F951: A9 00     LDA #$00           
F953: 38        SEC                
F954: 20 9C F8  JSR $F89C            ; DOCONTROL
F957: 4C 73 F9  JMP $F973          

SETMANIFOLD:
F95A: BD A9 F6  LDA $F6A9,X        
F95D: 85 B9     STA $B9            
F95F: BD AA F6  LDA $F6AA,X        
F962: 85 BA     STA $BA            
F964: 20 D0 F9  JSR $F9D0            ; SETMUFFLERADDRESS
F967: B0 0B     BCS $F974          
F969: A9 04     LDA #$04           
F96B: 20 8E F8  JSR $F88E            ; DOCLOCK
F96E: A9 00     LDA #$00           
F970: 20 8E F8  JSR $F88E            ; DOCLOCK
F973: 18        CLC                
F974: 4C 67 F2  JMP $F267            ; ENABLEMIDAS

DATUMTOMANIFOLD:
F977: 85 BA     STA $BA            
F979: A5 B4     LDA $B4            
F97B: 0A        ASL A              
F97C: 26 BA     ROL $BA            
F97E: 0A        ASL A              
F97F: 26 BA     ROL $BA            
F981: 85 B9     STA $B9            
F983: 4C 64 F9  JMP $F964          

READMUFFLER:
F986: BD A9 F6  LDA $F6A9,X        
F989: 85 B9     STA $B9            
F98B: BD AA F6  LDA $F6AA,X        
F98E: 85 BA     STA $BA            
F990: 20 D0 F9  JSR $F9D0            ; SETMUFFLERADDRESS
F993: A9 01     LDA #$01           
F995: 2D 02 04  AND $0402          
F998: 4C 67 F2  JMP $F267            ; ENABLEMIDAS

READMUFFLERFIELD:
F99B: 20 0E FA  JSR $FA0E            ; TRYGETTINGMUFMANCONTROL
F99E: B0 D4     BCS $F974          
F9A0: BD A9 F6  LDA $F6A9,X        
F9A3: 85 B7     STA $B7            
F9A5: BD AA F6  LDA $F6AA,X        
F9A8: 85 B8     STA $B8            
F9AA: A9 00     LDA #$00           
F9AC: 48        PHA                
F9AD: 18        CLC                
F9AE: A5 B7     LDA $B7            
F9B0: 85 B9     STA $B9            
F9B2: 7D AB F6  ADC $F6AB,X        
F9B5: 85 B7     STA $B7            
F9B7: A5 B8     LDA $B8            
F9B9: 85 BA     STA $BA            
F9BB: 7D AC F6  ADC $F6AC,X        
F9BE: 85 B8     STA $B8            
F9C0: 20 D5 F9  JSR $F9D5          
F9C3: AD 02 04  LDA $0402          
F9C6: 4A        LSR A              
F9C7: 68        PLA                
F9C8: 2A        ROL A              
F9C9: 88        DEY                
F9CA: 10 E0     BPL $F9AC          
F9CC: 18        CLC                
F9CD: 4C 67 F2  JMP $F267            ; ENABLEMIDAS

SETMUFFLERADDRESS:
F9D0: 20 0E FA  JSR $FA0E            ; TRYGETTINGMUFMANCONTROL
F9D3: B0 20     BCS $F9F5          
F9D5: 48        PHA                
F9D6: 98        TYA                
F9D7: 48        PHA                
F9D8: A9 10     LDA #$10           
F9DA: 8D 82 05  STA $0582          
F9DD: A0 07     LDY #$07           
F9DF: 06 BA     ASL $BA            
F9E1: 20 F6 F9  JSR $F9F6          
F9E4: 88        DEY                
F9E5: 10 F8     BPL $F9DF          
F9E7: A0 03     LDY #$03           
F9E9: 06 B9     ASL $B9            
F9EB: 20 F6 F9  JSR $F9F6          
F9EE: 88        DEY                
F9EF: 10 F8     BPL $F9E9          
F9F1: 68        PLA                
F9F2: A8        TAY                
F9F3: 68        PLA                
F9F4: 18        CLC                
F9F5: 60        RTS                
F9F6: A9 80     LDA #$80           
F9F8: 6A        ROR A              
F9F9: 8D 80 05  STA $0580          
F9FC: EE 82 05  INC $0582          
F9FF: CE 82 05  DEC $0582          
FA02: 29 BF     AND #$BF           
FA04: 8D 80 05  STA $0580          
FA07: EE 82 05  INC $0582          
FA0A: CE 82 05  DEC $0582          
FA0D: 60        RTS                

TRYGETTINGMUFMANCONTROL:
FA0E: 48        PHA                
FA0F: 20 4E FA  JSR $FA4E          
FA12: 20 62 F2  JSR $F262            ; INHIBITMIDAS
FA15: AD 82 05  LDA $0582          
FA18: 29 04     AND #$04           
FA1A: F0 23     BEQ $FA3F          
FA1C: 38        SEC                
FA1D: 68        PLA                
FA1E: 60        RTS                

WAITFORCPCONTROL:
FA1F: 48        PHA                
FA20: 20 4E FA  JSR $FA4E          
FA23: 4C 3C FA  JMP $FA3C          
FA26: 20 67 F2  JSR $F267            ; ENABLEMIDAS
FA29: 20 92 F6  JSR $F692            ; PACIFYWATCHDOG
FA2C: A5 BB     LDA $BB            
FA2E: CD 52 00  CMP $0052          
FA31: D0 05     BNE $FA38          
FA33: A9 02     LDA #$02           
FA35: 20 E7 F4  JSR $F4E7            ; SETPROBLEM
FA38: A5 7C     LDA $7C            
FA3A: D0 FE     BNE $FA3A          
FA3C: 20 62 F2  JSR $F262            ; INHIBITMIDAS
FA3F: AD 00 05  LDA $0500          
FA42: 29 20     AND #$20           
FA44: D0 E0     BNE $FA26          
FA46: A9 02     LDA #$02           
FA48: 20 EC F4  JSR $F4EC            ; CLEARPROBLEM
FA4B: 18        CLC                
FA4C: 68        PLA                
FA4D: 60        RTS                
FA4E: AD 52 00  LDA $0052          
FA51: 18        CLC                
FA52: 69 0A     ADC #$0A           
FA54: 85 BB     STA $BB            
FA56: 60        RTS                

LEFT7:
FA57: 0A        ASL A              
FA58: 26 B4     ROL $B4            

LEFT6:
FA5A: 0A        ASL A              
FA5B: 26 B4     ROL $B4            

LEFT5:
FA5D: 0A        ASL A              
FA5E: 26 B4     ROL $B4            

LEFT4:
FA60: 0A        ASL A              
FA61: 26 B4     ROL $B4            

LEFT3:
FA63: 0A        ASL A              
FA64: 26 B4     ROL $B4            

LEFT2:
FA66: 0A        ASL A              
FA67: 26 B4     ROL $B4            

LEFT1:
FA69: 0A        ASL A              
FA6A: 26 B4     ROL $B4            
FA6C: 60        RTS                

BOOTCODE:
FA6D: 70 01     BVS $FA70          
FA6F: 0F        ???  ; illegal $0F 
FA70: 4C 40 60  JMP $6040          
FA73: 02        ???  ; illegal $02 
FA74: 0F        ???  ; illegal $0F 
FA75: 44        ???  ; illegal $44 
FA76: 40        RTI                

CPREGTOLINK#:
FA77: 30 13     BMI $FA8C          
FA79: EF        ???  ; illegal $EF 
FA7A: 04        ???  ; illegal $04 
FA7B: 40        RTI                

IMLHRSTK.0IS0#:
FA7C: 60        RTS                
FA7D: 33        ???  ; illegal $33 
FA7E: EF        ???  ; illegal $EF 
FA7F: 03        ???  ; illegal $03 
FA80: 4F        ???  ; illegal $4F 

IMLHRSTK.0IS1#:
FA81: 20 73 EF  JSR $EF73          
FA84: 03        ???  ; illegal $03 
FA85: 4F        ???  ; illegal $4F 

IMRHBLOCKIS0#:
FA86: 20 13 EF  JSR $EF13          
FA89: 03        ???  ; illegal $03 
FA8A: 4F        ???  ; illegal $4F 

IMRHBLOCKIS1#:
FA8B: 60        RTS                
FA8C: 53        ???  ; illegal $53 
FA8D: EF        ???  ; illegal $EF 
FA8E: 03        ???  ; illegal $03 
FA8F: 4F        ???  ; illegal $4F 

RETURN#:
FA90: 60        RTS                
FA91: 13        ???  ; illegal $13 
FA92: E1 42     SBC ($42,X)        
FA94: 43        ???  ; illegal $43 

QFROMCPREG#:
FA95: 30 13     BMI $FAAA          
FA97: EF        ???  ; illegal $EF 
FA98: C4 40     CPY $40            
FA9A: 70 03     BVS $FA9F            ; ALUFM[0]FROMQ#
FA9C: 0F        ???  ; illegal $0F 
FA9D: 04        ???  ; illegal $04 
FA9E: C0 30     CPY #$30           
FAA0: 05 09     ORA $09            
FAA2: C4 40     CPY $40            
FAA4: 30 02     BMI $FAA8          
FAA6: 0B        ???  ; illegal $0B 
FAA7: 84 60     STY $60            

SETHOLDTASKSIM#:
FAA9: 70 13     BVS $FABE          
FAAB: EF        ???  ; illegal $EF 
FAAC: 84 40     STY $40            

LOADDORADOCODE:
FAAE: 20 1F FA  JSR $FA1F            ; WAITFORCPCONTROL
FAB1: 20 48 F9  JSR $F948            ; STOPDORADO
FAB4: A0 07     LDY #$07           
FAB6: A2 32     LDX #$32           
FAB8: 20 9B F9  JSR $F99B            ; READMUFFLERFIELD
FABB: C9 3F     CMP #$3F           
FABD: F0 11     BEQ $FAD0          
FABF: 90 0F     BCC $FAD0          
FAC1: A2 12     LDX #$12           
FAC3: 20 5A F9  JSR $F95A            ; SETMANIFOLD
FAC6: A2 14     LDX #$14           
FAC8: 20 5A F9  JSR $F95A            ; SETMANIFOLD
FACB: A2 02     LDX #$02           
FACD: 20 BA F4  JSR $F4BA            ; DELAY
FAD0: A2 1A     LDX #$1A           
FAD2: A9 08     LDA #$08           
FAD4: 85 CC     STA $CC            
FAD6: 20 5A F9  JSR $F95A            ; SETMANIFOLD
FAD9: E8        INX                
FADA: E8        INX                
FADB: C6 CC     DEC $CC            
FADD: 10 F7     BPL $FAD6          
FADF: 20 67 FB  JSR $FB67          
FAE2: A2 15     LDX #$15           
FAE4: A9 28     LDA #$28           
FAE6: 20 A3 FB  JSR $FBA3            ; SETCPANDDOIRTABLEINST
FAE9: A9 32     LDA #$32           
FAEB: 20 B7 FB  JSR $FBB7            ; DOIRTABLEINST
FAEE: A9 05     LDA #$05           
FAF0: 20 B7 FB  JSR $FBB7            ; DOIRTABLEINST
FAF3: A2 00     LDX #$00           
FAF5: A9 00     LDA #$00           
FAF7: 20 CB FB  JSR $FBCB            ; SENDIMBLOCKTODORADO
FAFA: A2 16     LDX #$16           
FAFC: 20 5A F9  JSR $F95A            ; SETMANIFOLD
FAFF: A0 0F     LDY #$0F           
FB01: A2 E0     LDX #$E0           
FB03: A9 0A     LDA #$0A           
FB05: 20 94 FB  JSR $FB94            ; SETCP~ANDDOIRTABLEINST
FB08: A9 00     LDA #$00           
FB0A: 85 B5     STA $B5            
FB0C: A9 80     LDA #$80           
FB0E: 85 B6     STA $B6            
FB10: 20 7E F8  JSR $F87E            ; SETCPREG
FB13: A2 18     LDX #$18           
FB15: 20 5A F9  JSR $F95A            ; SETMANIFOLD
FB18: A9 FA     LDA #$FA           
FB1A: 85 B1     STA $B1            
FB1C: A9 90     LDA #$90           
FB1E: 85 B0     STA $B0            
FB20: 20 C0 F8  JSR $F8C0            ; RUNDORADOINSTRUCTIONSTREAM
FB23: A2 0B     LDX #$0B           
FB25: A9 01     LDA #$01           
FB27: 20 CB FB  JSR $FBCB            ; SENDIMBLOCKTODORADO
FB2A: A9 40     LDA #$40           
FB2C: 20 EC F4  JSR $F4EC            ; CLEARPROBLEM
FB2F: A0 41     LDY #$41           
FB31: A2 00     LDX #$00           
FB33: AD 82 04  LDA $0482          
FB36: 29 0F     AND #$0F           
FB38: C9 01     CMP #$01           
FB3A: F0 0B     BEQ $FB47          
FB3C: CA        DEX                
FB3D: D0 F4     BNE $FB33          
FB3F: 88        DEY                
FB40: D0 F1     BNE $FB33          
FB42: A9 40     LDA #$40           
FB44: 20 E7 F4  JSR $F4E7            ; SETPROBLEM
FB47: 4C 67 F2  JMP $F267            ; ENABLEMIDAS

STARTDORADOATINITMAP:
FB4A: 20 1F FA  JSR $FA1F            ; WAITFORCPCONTROL
FB4D: 20 67 FB  JSR $FB67          
FB50: A0 02     LDY #$02           
FB52: A2 3E     LDX #$3E           
FB54: A9 0A     LDA #$0A           
FB56: 20 94 FB  JSR $FB94            ; SETCP~ANDDOIRTABLEINST
FB59: A9 FA     LDA #$FA           
FB5B: 85 B1     STA $B1            
FB5D: A9 90     LDA #$90           
FB5F: 85 B0     STA $B0            
FB61: 20 C0 F8  JSR $F8C0            ; RUNDORADOINSTRUCTIONSTREAM
FB64: 4C 67 F2  JMP $F267            ; ENABLEMIDAS
FB67: 20 48 F9  JSR $F948            ; STOPDORADO
FB6A: A2 1A     LDX #$1A           
FB6C: 20 5A F9  JSR $F95A            ; SETMANIFOLD
FB6F: A9 28     LDA #$28           
FB71: 85 CD     STA $CD            
FB73: A2 00     LDX #$00           
FB75: A0 00     LDY #$00           
FB77: A9 3C     LDA #$3C           
FB79: 20 A3 FB  JSR $FBA3            ; SETCPANDDOIRTABLEINST
FB7C: C6 CD     DEC $CD            
FB7E: D0 F3     BNE $FB73          
FB80: A2 43     LDX #$43           
FB82: A0 00     LDY #$00           
FB84: A9 2D     LDA #$2D           
FB86: 20 A3 FB  JSR $FBA3            ; SETCPANDDOIRTABLEINST
FB89: A9 37     LDA #$37           
FB8B: 20 B7 FB  JSR $FBB7            ; DOIRTABLEINST
FB8E: A9 23     LDA #$23           
FB90: 20 B7 FB  JSR $FBB7            ; DOIRTABLEINST
FB93: 60        RTS                

SETCP~ANDDOIRTABLEINST:
FB94: 86 B5     STX $B5            
FB96: 84 B6     STY $B6            
FB98: 48        PHA                
FB99: 20 1F FA  JSR $FA1F            ; WAITFORCPCONTROL
FB9C: 20 6F F8  JSR $F86F            ; SETCPREG~
FB9F: 68        PLA                
FBA0: 4C AF FB  JMP $FBAF          

SETCPANDDOIRTABLEINST:
FBA3: 86 B5     STX $B5            
FBA5: 84 B6     STY $B6            
FBA7: 48        PHA                
FBA8: 20 1F FA  JSR $FA1F            ; WAITFORCPCONTROL
FBAB: 20 7E F8  JSR $F87E            ; SETCPREG
FBAE: 68        PLA                
FBAF: 20 B7 FB  JSR $FBB7            ; DOIRTABLEINST
FBB2: A9 00     LDA #$00           
FBB4: 4C BA FB  JMP $FBBA          

DOIRTABLEINST:
FBB7: 20 1F FA  JSR $FA1F            ; WAITFORCPCONTROL
FBBA: 18        CLC                
FBBB: 69 6D     ADC #$6D           
FBBD: 85 B0     STA $B0            
FBBF: A9 00     LDA #$00           
FBC1: 69 FA     ADC #$FA           
FBC3: 85 B1     STA $B1            
FBC5: 20 CC F8  JSR $F8CC            ; DODORADOMICROINST
FBC8: 4C 67 F2  JMP $F267            ; ENABLEMIDAS

SENDIMBLOCKTODORADO:
FBCB: 85 D9     STA $D9            
FBCD: BD 02 C0  LDA $C002,X        
FBD0: 85 C9     STA $C9            
FBD2: BD 03 C0  LDA $C003,X        
FBD5: 85 C8     STA $C8            
FBD7: BD 04 C0  LDA $C004,X        
FBDA: 85 CB     STA $CB            
FBDC: BD 05 C0  LDA $C005,X        
FBDF: 85 CA     STA $CA            
FBE1: BD 06 C0  LDA $C006,X        
FBE4: 85 D1     STA $D1            
FBE6: BD 07 C0  LDA $C007,X        
FBE9: 85 D2     STA $D2            
FBEB: BD 08 C0  LDA $C008,X        
FBEE: 85 D3     STA $D3            
FBF0: BD 09 C0  LDA $C009,X        
FBF3: 85 D6     STA $D6            
FBF5: BD 0A C0  LDA $C00A,X        
FBF8: 85 D7     STA $D7            
FBFA: 20 4A FC  JSR $FC4A          
FBFD: A5 D9     LDA $D9            
FBFF: F0 16     BEQ $FC17          
FC01: A9 00     LDA #$00           
FC03: 85 CF     STA $CF            
FC05: A9 C8     LDA #$C8           
FC07: 85 CE     STA $CE            
FC09: A0 00     LDY #$00           
FC0B: 84 D0     STY $D0            
FC0D: A9 40     LDA #$40           
FC0F: 85 D8     STA $D8            
FC11: 20 AF FC  JSR $FCAF          
FC14: 20 AF FC  JSR $FCAF          
FC17: BD 00 C0  LDA $C000,X        
FC1A: 85 CE     STA $CE            
FC1C: BD 01 C0  LDA $C001,X        
FC1F: 85 CF     STA $CF            
FC21: 20 5E FC  JSR $FC5E            ; SENDAHUNK
FC24: 20 92 F6  JSR $F692            ; PACIFYWATCHDOG
FC27: A5 CE     LDA $CE            
FC29: 18        CLC                
FC2A: 69 11     ADC #$11           
FC2C: 85 CE     STA $CE            
FC2E: 90 02     BCC $FC32          
FC30: E6 CF     INC $CF            
FC32: C6 CB     DEC $CB            
FC34: D0 EB     BNE $FC21          
FC36: C6 CA     DEC $CA            
FC38: 10 E7     BPL $FC21          
FC3A: A5 D9     LDA $D9            
FC3C: F0 0B     BEQ $FC49          
FC3E: A5 D1     LDA $D1            
FC40: 38        SEC                
FC41: 20 86 F8  JSR $F886            ; SETCPREG1
FC44: A5 D2     LDA $D2            
FC46: 20 95 F8  JSR $F895            ; SETCPREG0
FC49: 60        RTS                
FC4A: 98        TYA                
FC4B: 48        PHA                
FC4C: A4 D3     LDY $D3            
FC4E: 88        DEY                
FC4F: 98        TYA                
FC50: 0A        ASL A              
FC51: A8        TAY                
FC52: B1 D6     LDA ($D6),Y        
FC54: 85 D4     STA $D4            
FC56: C8        INY                
FC57: B1 D6     LDA ($D6),Y        
FC59: 85 D5     STA $D5            
FC5B: 68        PLA                
FC5C: A8        TAY                
FC5D: 60        RTS                

SENDAHUNK:
FC5E: A5 D9     LDA $D9            
FC60: F0 03     BEQ $FC65          
FC62: 20 1F FA  JSR $FA1F            ; WAITFORCPCONTROL
FC65: A0 00     LDY #$00           
FC67: B1 CE     LDA ($CE),Y        
FC69: C8        INY                
FC6A: 85 D0     STA $D0            
FC6C: A9 40     LDA #$40           
FC6E: 85 D8     STA $D8            
FC70: A5 C9     LDA $C9            
FC72: C5 D5     CMP $D5            
FC74: D0 10     BNE $FC86          
FC76: A5 C8     LDA $C8            
FC78: C5 D4     CMP $D4            
FC7A: D0 0A     BNE $FC86          
FC7C: A5 D3     LDA $D3            
FC7E: F0 06     BEQ $FC86          
FC80: A9 02     LDA #$02           
FC82: 05 D8     ORA $D8            
FC84: 85 D8     STA $D8            
FC86: 20 AF FC  JSR $FCAF          
FC89: A5 D8     LDA $D8            
FC8B: 09 01     ORA #$01           
FC8D: 85 D8     STA $D8            
FC8F: 20 AF FC  JSR $FCAF          
FC92: A5 D8     LDA $D8            
FC94: 29 02     AND #$02           
FC96: F0 05     BEQ $FC9D          
FC98: C6 D3     DEC $D3            
FC9A: 20 4A FC  JSR $FC4A          
FC9D: E6 C9     INC $C9            
FC9F: D0 02     BNE $FCA3          
FCA1: E6 C8     INC $C8            
FCA3: C0 11     CPY #$11           
FCA5: D0 C5     BNE $FC6C          
FCA7: A5 D9     LDA $D9            
FCA9: F0 03     BEQ $FCAE          
FCAB: 20 67 F2  JSR $F267            ; ENABLEMIDAS
FCAE: 60        RTS                
FCAF: A5 D9     LDA $D9            
FCB1: F0 4A     BEQ $FCFD            ; SENDVIAMIR
FCB3: B1 CE     LDA ($CE),Y        
FCB5: C8        INY                
FCB6: 8D 80 05  STA $0580          
FCB9: A9 B0     LDA #$B0           
FCBB: 8D 82 05  STA $0582          
FCBE: EE 82 05  INC $0582          
FCC1: CE 82 05  DEC $0582          
FCC4: A9 00     LDA #$00           
FCC6: 8D 80 05  STA $0580          
FCC9: A9 20     LDA #$20           
FCCB: 8D 82 05  STA $0582          
FCCE: EE 82 05  INC $0582          
FCD1: CE 82 05  DEC $0582          
FCD4: B1 CE     LDA ($CE),Y        
FCD6: C8        INY                
FCD7: 8D 80 05  STA $0580          
FCDA: A9 B0     LDA #$B0           
FCDC: 8D 82 05  STA $0582          
FCDF: EE 82 05  INC $0582          
FCE2: CE 82 05  DEC $0582          
FCE5: A5 D8     LDA $D8            
FCE7: 06 D0     ASL $D0            
FCE9: 2A        ROL A              
FCEA: 8D 80 05  STA $0580          
FCED: A9 20     LDA #$20           
FCEF: 8D 82 05  STA $0582          
FCF2: EE 82 05  INC $0582          
FCF5: CE 82 05  DEC $0582          
FCF8: 60        RTS                
FCF9: 0F        ???  ; illegal $0F 
FCFA: 14        ???  ; illegal $14 
FCFB: 19 1E 98  ORA $981E,Y        
FCFE: 48        PHA                
FCFF: A6 C9     LDX $C9            
FD01: A4 C8     LDY $C8            
FD03: A9 0A     LDA #$0A           
FD05: 20 94 FB  JSR $FB94            ; SETCP~ANDDOIRTABLEINST
FD08: 68        PLA                
FD09: A8        TAY                
FD0A: B1 CE     LDA ($CE),Y        
FD0C: C8        INY                
FD0D: 85 B6     STA $B6            
FD0F: B1 CE     LDA ($CE),Y        
FD11: C8        INY                
FD12: 85 B5     STA $B5            
FD14: 98        TYA                
FD15: 48        PHA                
FD16: 20 6F F8  JSR $F86F            ; SETCPREG~
FD19: A5 D8     LDA $D8            
FD1B: 29 01     AND #$01           
FD1D: 06 D0     ASL $D0            
FD1F: 2A        ROL A              
FD20: AA        TAX                
FD21: BD F9 FC  LDA $FCF9,X        
FD24: 20 B7 FB  JSR $FBB7            ; DOIRTABLEINST
FD27: 68        PLA                
FD28: A8        TAY                
FD29: 60        RTS                
FD2A: 00        BRK                
FD2B: 00        BRK                
FD2C: 00        BRK                
FD2D: 00        BRK                
FD2E: 00        BRK                
FD2F: 00        BRK                
FD30: 00        BRK                
FD31: 00        BRK                
FD32: 00        BRK                
FD33: 00        BRK                
FD34: 00        BRK                
FD35: 00        BRK                
FD36: 00        BRK                
FD37: 00        BRK                
FD38: 00        BRK                
FD39: 00        BRK                
FD3A: 00        BRK                
FD3B: 00        BRK                
FD3C: 00        BRK                
FD3D: 00        BRK                
FD3E: 00        BRK                
FD3F: 00        BRK                
FD40: 00        BRK                
FD41: 00        BRK                
FD42: 00        BRK                
FD43: 00        BRK                
FD44: 00        BRK                
FD45: 00        BRK                
FD46: 00        BRK                
FD47: 00        BRK                
FD48: 00        BRK                
FD49: 00        BRK                
FD4A: 00        BRK                
FD4B: 00        BRK                
FD4C: 00        BRK                
FD4D: 00        BRK                
FD4E: 00        BRK                
FD4F: 00        BRK                
FD50: 00        BRK                
FD51: 00        BRK                
FD52: 00        BRK                
FD53: 00        BRK                
FD54: 00        BRK                
FD55: 00        BRK                
FD56: 00        BRK                
FD57: 00        BRK                
FD58: 00        BRK                
FD59: 00        BRK                
FD5A: 00        BRK                
FD5B: 00        BRK                
FD5C: 00        BRK                
FD5D: 00        BRK                
FD5E: 00        BRK                
FD5F: 00        BRK                
FD60: 00        BRK                
FD61: 00        BRK                
FD62: 00        BRK                
FD63: 00        BRK                
FD64: 00        BRK                
FD65: 00        BRK                
FD66: 00        BRK                
FD67: 00        BRK                
FD68: 00        BRK                
FD69: 00        BRK                
FD6A: 00        BRK                
FD6B: 00        BRK                
FD6C: 00        BRK                
FD6D: 00        BRK                
FD6E: 00        BRK                
FD6F: 00        BRK                
FD70: 00        BRK                
FD71: 00        BRK                
FD72: 00        BRK                
FD73: 00        BRK                
FD74: 00        BRK                
FD75: 00        BRK                
FD76: 00        BRK                
FD77: 00        BRK                
FD78: 00        BRK                
FD79: 00        BRK                
FD7A: 00        BRK                
FD7B: 00        BRK                
FD7C: 00        BRK                
FD7D: 00        BRK                
FD7E: 00        BRK                
FD7F: 00        BRK                
FD80: 00        BRK                
FD81: 00        BRK                
FD82: 00        BRK                
FD83: 00        BRK                
FD84: 00        BRK                
FD85: 00        BRK                
FD86: 00        BRK                
FD87: 00        BRK                
FD88: 00        BRK                
FD89: 00        BRK                
FD8A: 00        BRK                
FD8B: 00        BRK                
FD8C: 00        BRK                
FD8D: 00        BRK                
FD8E: 00        BRK                
FD8F: 00        BRK                
FD90: 00        BRK                
FD91: 00        BRK                
FD92: 00        BRK                
FD93: 00        BRK                
FD94: 00        BRK                
FD95: 00        BRK                
FD96: 00        BRK                
FD97: 00        BRK                
FD98: 00        BRK                
FD99: 00        BRK                
FD9A: 00        BRK                
FD9B: 00        BRK                
FD9C: 00        BRK                
FD9D: 00        BRK                
FD9E: 00        BRK                
FD9F: 00        BRK                
FDA0: 00        BRK                
FDA1: 00        BRK                
FDA2: 00        BRK                
FDA3: 00        BRK                
FDA4: 00        BRK                
FDA5: 00        BRK                
FDA6: 00        BRK                
FDA7: 00        BRK                
FDA8: 00        BRK                
FDA9: 00        BRK                
FDAA: 00        BRK                
FDAB: 00        BRK                
FDAC: 00        BRK                
FDAD: 00        BRK                
FDAE: 00        BRK                
FDAF: 00        BRK                
FDB0: 00        BRK                
FDB1: 00        BRK                
FDB2: 00        BRK                
FDB3: 00        BRK                
FDB4: 00        BRK                
FDB5: 00        BRK                
FDB6: 00        BRK                
FDB7: 00        BRK                
FDB8: 00        BRK                
FDB9: 00        BRK                
FDBA: 00        BRK                
FDBB: 00        BRK                
FDBC: 00        BRK                
FDBD: 00        BRK                
FDBE: 00        BRK                
FDBF: 00        BRK                
FDC0: 00        BRK                
FDC1: 00        BRK                
FDC2: 00        BRK                
FDC3: 00        BRK                
FDC4: 00        BRK                
FDC5: 00        BRK                
FDC6: 00        BRK                
FDC7: 00        BRK                
FDC8: 00        BRK                
FDC9: 00        BRK                
FDCA: 00        BRK                
FDCB: 00        BRK                
FDCC: 00        BRK                
FDCD: 00        BRK                
FDCE: 00        BRK                
FDCF: 00        BRK                
FDD0: 00        BRK                
FDD1: 00        BRK                
FDD2: 00        BRK                
FDD3: 00        BRK                
FDD4: 00        BRK                
FDD5: 00        BRK                
FDD6: 00        BRK                
FDD7: 00        BRK                
FDD8: 00        BRK                
FDD9: 00        BRK                
FDDA: 00        BRK                
FDDB: 00        BRK                
FDDC: 00        BRK                
FDDD: 00        BRK                
FDDE: 00        BRK                
FDDF: 00        BRK                
FDE0: 00        BRK                
FDE1: 00        BRK                
FDE2: 00        BRK                
FDE3: 00        BRK                
FDE4: 00        BRK                
FDE5: 00        BRK                
FDE6: 00        BRK                
FDE7: 00        BRK                
FDE8: 00        BRK                
FDE9: 00        BRK                
FDEA: 00        BRK                
FDEB: 00        BRK                
FDEC: 00        BRK                
FDED: 00        BRK                
FDEE: 00        BRK                
FDEF: 00        BRK                
FDF0: 00        BRK                
FDF1: 00        BRK                
FDF2: 00        BRK                
FDF3: 00        BRK                
FDF4: 00        BRK                
FDF5: 00        BRK                
FDF6: 00        BRK                
FDF7: 00        BRK                
FDF8: 00        BRK                
FDF9: 00        BRK                
FDFA: 00        BRK                
FDFB: 00        BRK                
FDFC: 00        BRK                
FDFD: 00        BRK                
FDFE: 00        BRK                
FDFF: 00        BRK                

BOOT0:
FE00: 00        BRK                
FE01: 00        BRK                
FE02: C4 7F     CPY $7F            
FE04: A7        ???  ; illegal $A7 
FE05: 90 B6     BCC $FDBD          
FE07: 3F        ???  ; illegal $3F 
FE08: FF        ???  ; illegal $FF 
FE09: 69 F7     ADC #$F7           
FE0B: 08        PHP                
FE0C: FF        ???  ; illegal $FF 
FE0D: 05 CF     ORA $CF            
FE0F: 80        ???  ; illegal $80 
FE10: 89        ???  ; illegal $89 
FE11: 00        BRK                
FE12: 00        BRK                
FE13: 00        BRK                
FE14: 00        BRK                
FE15: 00        BRK                
FE16: 00        BRK                
FE17: 44        ???  ; illegal $44 
FE18: 3F        ???  ; illegal $3F 
FE19: FF        ???  ; illegal $FF 
FE1A: 29 F7     AND #$F7           
FE1C: 08        PHP                
FE1D: FF        ???  ; illegal $FF 
FE1E: 45 F7     EOR $F7            
FE20: D8        CLD                
FE21: AA        TAX                
FE22: 00        BRK                
FE23: 00        BRK                
FE24: 00        BRK                
FE25: 00        BRK                
FE26: 00        BRK                
FE27: 70 B4     BVS $FDDD          
FE29: 3F        ???  ; illegal $3F 
FE2A: FF        ???  ; illegal $FF 
FE2B: 29 F7     AND #$F7           
FE2D: 08        PHP                
FE2E: FF        ???  ; illegal $FF 
FE2F: 05 CF     ORA $CF            
FE31: 80        ???  ; illegal $80 
FE32: B2        ???  ; illegal $B2 
FE33: 00        BRK                
FE34: 00        BRK                
FE35: 00        BRK                
FE36: 00        BRK                
FE37: 00        BRK                
FE38: 10 B4     BPL $FDEE          
FE3A: 3F        ???  ; illegal $3F 
FE3B: 47        ???  ; illegal $47 
FE3C: 00        BRK                
FE3D: 44        ???  ; illegal $44 
FE3E: 3F        ???  ; illegal $3F 
FE3F: 85 00     STA $00            
FE41: 44        ???  ; illegal $44 
FE42: 3F        ???  ; illegal $3F 
FE43: FD 00 20  SBC $2000,X        
FE46: 84 3F     STY $3F            
FE48: 7F        ???  ; illegal $7F 
FE49: 00        BRK                
FE4A: 44        ???  ; illegal $44 
FE4B: 3F        ???  ; illegal $3F 
FE4C: AF        ???  ; illegal $AF 
FE4D: 60        RTS                
FE4E: 84 3F     STY $3F            
FE50: 7F        ???  ; illegal $7F 
FE51: 00        BRK                
FE52: 44        ???  ; illegal $44 
FE53: 3F        ???  ; illegal $3F 
FE54: AF        ???  ; illegal $AF 
FE55: 00        BRK                
FE56: 00        BRK                
FE57: 84 3F     STY $3F            
FE59: 7F        ???  ; illegal $7F 
FE5A: 74        ???  ; illegal $74 
FE5B: 74        ???  ; illegal $74 
FE5C: 3F        ???  ; illegal $3F 
FE5D: AF        ???  ; illegal $AF 
FE5E: 40        RTI                
FE5F: 84 3F     STY $3F            
FE61: 7F        ???  ; illegal $7F 
FE62: 74        ???  ; illegal $74 
FE63: 74        ???  ; illegal $74 
FE64: 3F        ???  ; illegal $3F 
FE65: AF        ???  ; illegal $AF 
FE66: 00        BRK                
FE67: A0 84     LDY #$84           
FE69: 3F        ???  ; illegal $3F 
FE6A: 7F        ???  ; illegal $7F 
FE6B: 00        BRK                
FE6C: 44        ???  ; illegal $44 
FE6D: 3F        ???  ; illegal $3F 
FE6E: AF        ???  ; illegal $AF 
FE6F: E0 84     CPX #$84           
FE71: 3F        ???  ; illegal $3F 
FE72: 7F        ???  ; illegal $7F 
FE73: 00        BRK                
FE74: 44        ???  ; illegal $44 
FE75: 3F        ???  ; illegal $3F 
FE76: AF        ???  ; illegal $AF 
FE77: 00        BRK                
FE78: 80        ???  ; illegal $80 
FE79: 84 3F     STY $3F            
FE7B: 7F        ???  ; illegal $7F 
FE7C: 74        ???  ; illegal $74 
FE7D: 74        ???  ; illegal $74 
FE7E: 3F        ???  ; illegal $3F 
FE7F: AF        ???  ; illegal $AF 
FE80: C0 84     CPY #$84           
FE82: 3F        ???  ; illegal $3F 
FE83: 7F        ???  ; illegal $7F 
FE84: 74        ???  ; illegal $74 
FE85: 74        ???  ; illegal $74 
FE86: 3F        ???  ; illegal $3F 
FE87: AF        ???  ; illegal $AF 
FE88: 00        BRK                
FE89: 00        BRK                
FE8A: 44        ???  ; illegal $44 
FE8B: 62        ???  ; illegal $62 
FE8C: B3        ???  ; illegal $B3 
FE8D: 00        BRK                
FE8E: 4C 7E 59  JMP $597E          
FE91: 90 84     BCC $FE17          
FE93: 3F        ???  ; illegal $3F 
FE94: A1 60     LDA ($60,X)        
FE96: 44        ???  ; illegal $44 
FE97: 67        ???  ; illegal $67 
FE98: 90 00     BCC $FE9A          
FE9A: 00        BRK                
FE9B: 00        BRK                
FE9C: 00        BRK                
FE9D: 00        BRK                
FE9E: 00        BRK                
FE9F: 00        BRK                
FEA0: 00        BRK                
FEA1: 00        BRK                
FEA2: 9C        ???  ; illegal $9C 
FEA3: F4        ???  ; illegal $F4 
FEA4: 67        ???  ; illegal $67 
FEA5: 8D 00 4C  STA $4C00          
FEA8: 7E A2 00  ROR $00A2,X        
FEAB: 00        BRK                
FEAC: 00        BRK                
FEAD: 00        BRK                
FEAE: 00        BRK                
FEAF: 40        RTI                
FEB0: 44        ???  ; illegal $44 
FEB1: 38        SEC                
FEB2: A3        ???  ; illegal $A3 
FEB3: 49 37     EOR #$37           
FEB5: 01 AB     ORA ($AB,X)        
FEB7: 05 CF     ORA $CF            
FEB9: 80        ???  ; illegal $80 
FEBA: AD 00 00  LDA $0000          
FEBD: 00        BRK                
FEBE: 00        BRK                
FEBF: 00        BRK                
FEC0: 30 4E     BMI $FF10          
FEC2: 3F        ???  ; illegal $3F 
FEC3: A9 20     LDA #$20           
FEC5: 44        ???  ; illegal $44 
FEC6: 66 AF     ROR $AF            
FEC8: 00        BRK                
FEC9: 44        ???  ; illegal $44 
FECA: 33        ???  ; illegal $33 
FECB: 8E 00 00  STX $0000          
FECE: 00        BRK                
FECF: 00        BRK                
FED0: 00        BRK                
FED1: 29 37     AND #$37           
FED3: 03        ???  ; illegal $03 
FED4: AE 30 B4  LDX $B430          
FED7: 3F        ???  ; illegal $3F 
FED8: B1 01     LDA ($01),Y        
FEDA: 0C        ???  ; illegal $0C 
FEDB: 13        ???  ; illegal $13 
FEDC: B9 00 00  LDA $0000,Y        
FEDF: 00        BRK                
FEE0: 00        BRK                
FEE1: 00        BRK                
FEE2: 00        BRK                
FEE3: 00        BRK                
FEE4: 00        BRK                
FEE5: 00        BRK                
FEE6: 00        BRK                
FEE7: 00        BRK                
FEE8: 00        BRK                
FEE9: 00        BRK                
FEEA: 00        BRK                
FEEB: 00        BRK                
FEEC: 00        BRK                
FEED: 00        BRK                
FEEE: 00        BRK                
FEEF: 00        BRK                
FEF0: 00        BRK                
FEF1: 00        BRK                
FEF2: 00        BRK                
FEF3: 10 84     BPL $FE79          
FEF5: B2        ???  ; illegal $B2 
FEF6: BA        TSX                
FEF7: 01 0C     ORA ($0C,X)        
FEF9: 80        ???  ; illegal $80 
FEFA: BB        ???  ; illegal $BB 
FEFB: 14        ???  ; illegal $14 
FEFC: 84 B2     STY $B2            
FEFE: BD 00 00  LDA $0000,X        
FF01: 00        BRK                
FF02: 00        BRK                
FF03: 00        BRK                
FF04: 01 0C     ORA ($0C,X)        
FF06: 01 BE     ORA ($BE,X)        
FF08: 1C        ???  ; illegal $1C 
FF09: 84 B2     STY $B2            
FF0B: BF        ???  ; illegal $BF 
FF0C: 10 B6     BPL $FEC4          
FF0E: 3F        ???  ; illegal $3F 
FF0F: 81 00     STA ($00,X)        
FF11: 00        BRK                
FF12: 00        BRK                
FF13: 00        BRK                
FF14: 00        BRK                
FF15: 00        BRK                
FF16: 00        BRK                
FF17: 00        BRK                
FF18: 00        BRK                
FF19: 00        BRK                
FF1A: 00        BRK                
FF1B: 00        BRK                
FF1C: 00        BRK                
FF1D: 00        BRK                
FF1E: 00        BRK                
FF1F: 00        BRK                
FF20: 00        BRK                
FF21: 00        BRK                
FF22: 00        BRK                
FF23: 00        BRK                
FF24: 00        BRK                
FF25: 00        BRK                
FF26: 00        BRK                
FF27: 00        BRK                
FF28: 00        BRK                
FF29: 00        BRK                
FF2A: 00        BRK                
FF2B: 00        BRK                
FF2C: 00        BRK                
FF2D: 00        BRK                
FF2E: 00        BRK                
FF2F: 00        BRK                
FF30: 00        BRK                
FF31: 00        BRK                
FF32: 00        BRK                
FF33: 00        BRK                
FF34: 00        BRK                
FF35: 00        BRK                
FF36: 00        BRK                
FF37: 00        BRK                
FF38: 00        BRK                
FF39: 00        BRK                
FF3A: 00        BRK                
FF3B: 00        BRK                
FF3C: 00        BRK                
FF3D: 00        BRK                
FF3E: 00        BRK                
FF3F: 00        BRK                
FF40: 00        BRK                
FF41: 00        BRK                
FF42: 00        BRK                
FF43: 00        BRK                
FF44: 00        BRK                
FF45: 00        BRK                
FF46: 00        BRK                
FF47: 00        BRK                
FF48: 00        BRK                
FF49: 00        BRK                
FF4A: 00        BRK                
FF4B: 00        BRK                
FF4C: 00        BRK                
FF4D: 00        BRK                
FF4E: 00        BRK                
FF4F: 00        BRK                
FF50: 00        BRK                
FF51: 00        BRK                
FF52: 00        BRK                
FF53: 00        BRK                
FF54: 00        BRK                
FF55: 00        BRK                
FF56: 00        BRK                
FF57: 00        BRK                
FF58: 00        BRK                
FF59: 00        BRK                
FF5A: 00        BRK                
FF5B: 00        BRK                
FF5C: 00        BRK                
FF5D: 00        BRK                
FF5E: 00        BRK                
FF5F: 00        BRK                
FF60: 00        BRK                
FF61: 00        BRK                
FF62: 00        BRK                
FF63: 00        BRK                
FF64: 00        BRK                
FF65: 00        BRK                
FF66: 00        BRK                
FF67: 00        BRK                
FF68: 00        BRK                
FF69: 00        BRK                
FF6A: 00        BRK                
FF6B: 00        BRK                
FF6C: 00        BRK                
FF6D: 00        BRK                
FF6E: 00        BRK                
FF6F: 00        BRK                
FF70: 00        BRK                
FF71: 00        BRK                
FF72: 00        BRK                
FF73: 00        BRK                
FF74: 00        BRK                
FF75: 00        BRK                
FF76: 00        BRK                
FF77: 00        BRK                
FF78: 00        BRK                
FF79: 00        BRK                
FF7A: 00        BRK                
FF7B: 00        BRK                
FF7C: 00        BRK                
FF7D: 00        BRK                
FF7E: 00        BRK                
FF7F: 00        BRK                
FF80: 00        BRK                
FF81: 00        BRK                
FF82: 00        BRK                
FF83: 00        BRK                
FF84: 00        BRK                
FF85: 00        BRK                
FF86: 00        BRK                
FF87: 00        BRK                
FF88: 00        BRK                
FF89: 00        BRK                
FF8A: 00        BRK                
FF8B: 00        BRK                
FF8C: 00        BRK                
FF8D: 00        BRK                
FF8E: 00        BRK                
FF8F: 00        BRK                
FF90: 00        BRK                
FF91: 00        BRK                
FF92: 00        BRK                
FF93: 00        BRK                
FF94: 00        BRK                
FF95: 00        BRK                
FF96: 00        BRK                
FF97: 00        BRK                
FF98: 00        BRK                
FF99: 00        BRK                
FF9A: 00        BRK                
FF9B: 00        BRK                
FF9C: 00        BRK                
FF9D: 00        BRK                
FF9E: 00        BRK                
FF9F: 00        BRK                
FFA0: 00        BRK                
FFA1: 00        BRK                
FFA2: 00        BRK                
FFA3: 00        BRK                
FFA4: 00        BRK                
FFA5: 00        BRK                
FFA6: 00        BRK                
FFA7: 00        BRK                
FFA8: 00        BRK                
FFA9: 00        BRK                
FFAA: 00        BRK                
FFAB: 00        BRK                
FFAC: 00        BRK                
FFAD: 00        BRK                
FFAE: 00        BRK                
FFAF: 00        BRK                
FFB0: 00        BRK                
FFB1: 00        BRK                
FFB2: 00        BRK                
FFB3: 00        BRK                
FFB4: 00        BRK                
FFB5: 00        BRK                
FFB6: 00        BRK                
FFB7: 00        BRK                
FFB8: 00        BRK                
FFB9: 00        BRK                
FFBA: 00        BRK                
FFBB: 00        BRK                
FFBC: 00        BRK                
FFBD: 00        BRK                
FFBE: 00        BRK                
FFBF: 00        BRK                
FFC0: 00        BRK                
FFC1: 00        BRK                
FFC2: 00        BRK                
FFC3: 00        BRK                
FFC4: 00        BRK                
FFC5: 00        BRK                
FFC6: 00        BRK                
FFC7: 00        BRK                
FFC8: 00        BRK                
FFC9: 00        BRK                
FFCA: 00        BRK                
FFCB: 00        BRK                
FFCC: 00        BRK                
FFCD: 00        BRK                
FFCE: 00        BRK                
FFCF: 00        BRK                
FFD0: 00        BRK                
FFD1: 00        BRK                
FFD2: 00        BRK                
FFD3: 00        BRK                
FFD4: 00        BRK                
FFD5: 00        BRK                
FFD6: 00        BRK                
FFD7: 00        BRK                
FFD8: 00        BRK                
FFD9: 00        BRK                
FFDA: 00        BRK                
FFDB: 00        BRK                
FFDC: 00        BRK                
FFDD: 00        BRK                
FFDE: 00        BRK                
FFDF: 00        BRK                
FFE0: 00        BRK                
FFE1: 00        BRK                
FFE2: 00        BRK                
FFE3: 00        BRK                
FFE4: 00        BRK                
FFE5: 00        BRK                
FFE6: 00        BRK                
FFE7: 00        BRK                
FFE8: 00        BRK                
FFE9: 00        BRK                
FFEA: 00        BRK                
FFEB: 00        BRK                
FFEC: 00        BRK                
FFED: 00        BRK                
FFEE: 00        BRK                
FFEF: 00        BRK                
FFF0: 00        BRK                
FFF1: 00        BRK                
FFF2: 00        BRK                
FFF3: 00        BRK                
FFF4: 00        BRK                
FFF5: 00        BRK                
FFF6: 00        BRK                
FFF7: 00        BRK                
FFF8: 00        BRK                
FFF9: 00        BRK                
FFFA: 00        BRK                
FFFB: F0 A7     BEQ $FFA4          
FFFD: F3        ???  ; illegal $F3 
FFFE: A2 F2     LDX #$F2           
