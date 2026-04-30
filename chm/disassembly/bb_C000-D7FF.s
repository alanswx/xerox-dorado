
BOOTBLOCKTABLE:
C000: 00        BRK                
C001: FE C0 0F  INC $0FC0,X        
C004: 10 00     BPL $C006          
C006: 22        ???  ; illegal $22 
C007: 00        BRK                
C008: 00        BRK                
C009: 10 FF     BPL $C00A          

BOOT1BLOCK:
C00B: 16 C0     ASL $C0,X          
C00D: 40        RTI                
C00E: 0C        ???  ; illegal $0C 
C00F: E0 00     CPX #$00           
C011: 13        ???  ; illegal $13 
C012: 00        BRK                
C013: 07        ???  ; illegal $07 
C014: F6 CE     INC $CE,X          

BOOT1:
C016: 30 00     BMI $C018          
C018: 44        ???  ; illegal $44 
C019: E6 82     INC $82            
C01B: 00        BRK                
C01C: 44        ???  ; illegal $44 
C01D: 67        ???  ; illegal $67 
C01E: 8B        ???  ; illegal $8B 
C01F: 00        BRK                
C020: 44        ???  ; illegal $44 
C021: 33        ???  ; illegal $33 
C022: 82        ???  ; illegal $82 
C023: 12        ???  ; illegal $12 
C024: 86 56     STX $56            
C026: 8A        TXA                
C027: 04        ???  ; illegal $04 
C028: 94 7A     STY $7A,X          
C02A: E7        ???  ; illegal $E7 
C02B: B6 00     LDX $00,Y          
C02D: 46 53     LSR $53            
C02F: 90 20     BCC $C051          
C031: 44        ???  ; illegal $44 
C032: 6A        ROR A              
C033: 86 52     STX $52            
C035: 74        ???  ; illegal $74 
C036: D2        ???  ; illegal $D2 
C037: 00        BRK                
C038: 00        BRK                
C039: 01 0C     ORA ($0C,X)        
C03B: 40        RTI                
C03C: 3B        ???  ; illegal $3B 
C03D: 00        BRK                
C03E: 44        ???  ; illegal $44 
C03F: 55 8D     EOR $8D,X          
C041: 16 4E     ASL $4E,X          
C043: 30 8A     BMI $BFCF          
C045: 00        BRK                
C046: 44        ???  ; illegal $44 
C047: 3F        ???  ; illegal $3F 
C048: 47        ???  ; illegal $47 
C049: 04        ???  ; illegal $04 
C04A: 15 8A     ORA $8A,X          
C04C: 02        ???  ; illegal $02 
C04D: A8        TAY                
C04E: 01 0C     ORA ($0C,X)        
C050: 32        ???  ; illegal $32 
C051: F2        ???  ; illegal $F2 
C052: E0 44     CPX #$44           
C054: 70 81     BVS $BFD7          
C056: 01 0C     ORA ($0C,X)        
C058: 00        BRK                
C059: 9B        ???  ; illegal $9B 
C05A: 20 40 74  JSR $7440          
C05D: 7F        ???  ; illegal $7F 
C05E: 8F        ???  ; illegal $8F 
C05F: 05 0F     ORA $0F            
C061: EE 9D 0F  INC $0F9D          
C064: 0E C1 F1  ASL $F1C1          
C067: 01 09     ORA ($09,X)        
C069: 20 F2 00  JSR $00F2          
C06C: 01 09     ORA ($09,X)        
C06E: 20 F2 32  JSR $32F2          
C071: 74        ???  ; illegal $74 
C072: E8        INX                
C073: 90 32     BCC $C0A7          
C075: 74        ???  ; illegal $74 
C076: 3F        ???  ; illegal $3F 
C077: 87        ???  ; illegal $87 
C078: 0F        ???  ; illegal $0F 
C079: 0E 01 F1  ASL $F101          
C07C: 08        PHP                
C07D: 00        BRK                
C07E: 44        ???  ; illegal $44 
C07F: 70 F3     BVS $C074          
C081: 01 0C     ORA ($0C,X)        
C083: 01 F1     ORA ($F1,X)        
C085: 18        CLC                
C086: 74        ???  ; illegal $74 
C087: 76 91     ROR $91,X          
C089: 0F        ???  ; illegal $0F 
C08A: 0E E1 F1  ASL $F1E1          
C08D: 00        BRK                
C08E: 20 40 CF  JSR $CF40          
C091: 05 0D     ORA $0D            
C093: CF        ???  ; illegal $CF 
C094: 59 9E CA  EOR $CA9E,Y        
C097: B6 6F     LDX $6F,Y          
C099: 9F        ???  ; illegal $9F 
C09A: E1 B4     SBC ($B4,X)        
C09C: 01 A1     ORA ($A1,X)        
C09E: 40        RTI                
C09F: 20 74 7F  JSR $7F74          
C0A2: A2 E0     LDX #$E0           
C0A4: 77        ???  ; illegal $77 
C0A5: CF        ???  ; illegal $CF 
C0A6: 0F        ???  ; illegal $0F 
C0A7: 01 8C     ORA ($8C,X)        
C0A9: 40        RTI                
C0AA: 92        ???  ; illegal $92 
C0AB: 00        BRK                
C0AC: 84 47     STY $47            
C0AE: A9 00     LDA #$00           
C0B0: F5 3A     SBC $3A,X          
C0B2: 01 55     ORA ($55,X)        
C0B4: F3        ???  ; illegal $F3 
C0B5: 72        ???  ; illegal $72 
C0B6: FF        ???  ; illegal $FF 
C0B7: B2        ???  ; illegal $B2 
C0B8: 6A        ROR A              
C0B9: 84 81     STY $81            
C0BB: 70 02     BVS $C0BF          
C0BD: 44        ???  ; illegal $44 
C0BE: 81 B9     STA ($B9,X)        
C0C0: 02        ???  ; illegal $02 
C0C1: 15 0A     ORA $0A,X          
C0C3: 16 B8     ASL $B8,X          
C0C5: 22        ???  ; illegal $22 
C0C6: 76 54     ROR $54,X          
C0C8: 85 08     STA $08            
C0CA: 86 3F     STX $3F            
C0CC: B9 32 74  LDA $7432,Y        
C0CF: DA        ???  ; illegal $DA 
C0D0: 00        BRK                
C0D1: 00        BRK                
C0D2: 40        RTI                
C0D3: 74        ???  ; illegal $74 
C0D4: 1A        ???  ; illegal $1A 
C0D5: BD 74 4C  LDA $4C74,X        
C0D8: 81 B1     STA ($B1,X)        
C0DA: D4        ???  ; illegal $D4 
C0DB: 74        ???  ; illegal $74 
C0DC: 34        ???  ; illegal $34 
C0DD: AA        TAX                
C0DE: F0 C4     BEQ $C0A4          
C0E0: 8B        ???  ; illegal $8B 
C0E1: 5C        ???  ; illegal $5C 
C0E2: 00        BRK                
C0E3: 54        ???  ; illegal $54 
C0E4: 4A        LSR A              
C0E5: FF        ???  ; illegal $FF 
C0E6: A4 F4     LDY $F4            
C0E8: 4E 3B AF  LSR $AF3B          
C0EB: 01 CC     ORA ($CC,X)        
C0ED: F6 F5     INC $F5,X          
C0EF: 01 CC     ORA ($CC,X)        
C0F1: 4F        ???  ; illegal $4F 
C0F2: F5 00     SBC $00,X          
C0F4: 00        BRK                
C0F5: 44        ???  ; illegal $44 
C0F6: 8B        ???  ; illegal $8B 
C0F7: AD 00 44  LDA $4400          
C0FA: 31 AE     AND ($AE),Y        
C0FC: F5 3A     SBC $3A,X          
C0FE: 00        BRK                
C0FF: 7D 15 0A  ADC $0A15,X        
C102: 04        ???  ; illegal $04 
C103: B0 20     BCS $C125          
C105: 15 0A     ORA $0A,X          
C107: B4 BC     LDY $BC,X          
C109: 00        BRK                
C10A: 44        ???  ; illegal $44 
C10B: 67        ???  ; illegal $67 
C10C: BA        TSX                
C10D: 00        BRK                
C10E: 44        ???  ; illegal $44 
C10F: 32        ???  ; illegal $32 
C110: 47        ???  ; illegal $47 
C111: C1 74     CMP ($74,X)        
C113: F4        ???  ; illegal $F4 
C114: AB        ???  ; illegal $AB 
C115: 00        BRK                
C116: 15 0A     ORA $0A,X          
C118: 01 84     ORA ($84,X)        
C11A: 45 37     EOR $37            
C11C: 88        DEY                
C11D: BE 61 BC  LDX $BC61,Y        
C120: FF        ???  ; illegal $FF 
C121: BF        ???  ; illegal $BF 
C122: 54        ???  ; illegal $54 
C123: 4A        LSR A              
C124: FF        ???  ; illegal $FF 
C125: 8C 0A 00  STY $000A          
C128: 84 C4     STY $C4            
C12A: 0A        ASL A              
C12B: 00        BRK                
C12C: B4 5E     LDY $5E,X          
C12E: 8F        ???  ; illegal $8F 
C12F: E0 B4     CPX #$B4           
C131: 46 81     LSR $81            
C133: E1 3C     SBC ($3C,X)        
C135: FF        ???  ; illegal $FF 
C136: 8A        TXA                
C137: 00        BRK                
C138: 00        BRK                
C139: 44        ???  ; illegal $44 
C13A: 63        ???  ; illegal $63 
C13B: 99 21 74  STA $7421,Y        
C13E: F1 A0     SBC ($A0),Y        
C140: 24 84     BIT $84            
C142: 3F        ???  ; illegal $3F 
C143: 6F        ???  ; illegal $6F 
C144: 14        ???  ; illegal $14 
C145: 4E 31 84  LSR $8431          
C148: 00        BRK                
C149: 00        BRK                
C14A: 44        ???  ; illegal $44 
C14B: 70 8D     BVS $C0DA          
C14D: 01 8C     ORA ($8C,X)        
C14F: 04        ???  ; illegal $04 
C150: A9 16     LDA #$16           
C152: 8E 64 B4  STX $B464          
C155: 21 F4     AND ($F4,X)        
C157: 83        ???  ; illegal $83 
C158: A5 80     LDA $80            
C15A: B6 7E     LDX $7E,Y          
C15C: FD 98 01  SBC $0198,X        
C15F: 0C        ???  ; illegal $0C 
C160: 20 F2 34  JSR $34F2          
C163: 70 F3     BVS $C158          
C165: 88        DEY                
C166: 20 B4 F1  JSR $F1B4          
C169: 9F        ???  ; illegal $9F 
C16A: 50 20     BVC $C18C          
C16C: 74        ???  ; illegal $74 
C16D: 7F        ???  ; illegal $7F 
C16E: AB        ???  ; illegal $AB 
C16F: 60        RTS                
C170: 44        ???  ; illegal $44 
C171: 5B        ???  ; illegal $5B 
C172: F3        ???  ; illegal $F3 
C173: 00        BRK                
C174: 44        ???  ; illegal $44 
C175: C6 00     DEC $00            
C177: 00        BRK                
C178: 44        ???  ; illegal $44 
C179: FE F3 00  INC $00F3,X        
C17C: 41 34     EOR ($34,X)        
C17E: 80        ???  ; illegal $80 
C17F: 90 00     BCC $C181          
C181: 44        ???  ; illegal $44 
C182: FF        ???  ; illegal $FF 
C183: F3        ???  ; illegal $F3 
C184: 00        BRK                
C185: 44        ???  ; illegal $44 
C186: D3        ???  ; illegal $D3 
C187: 00        BRK                
C188: 32        ???  ; illegal $32 
C189: 74        ???  ; illegal $74 
C18A: EF        ???  ; illegal $EF 
C18B: 88        DEY                
C18C: 80        ???  ; illegal $80 
C18D: C0 B4     CPY #$B4           
C18F: 31 82     AND ($82),Y        
C191: 01 0C     ORA ($0C,X)        
C193: 01 F1     ORA ($F1,X)        
C195: 21 BC     AND ($BC,X)        
C197: 04        ???  ; illegal $04 
C198: 9B        ???  ; illegal $9B 
C199: 0F        ???  ; illegal $0F 
C19A: 0E B1 B0  ASL $B0B1          
C19D: 00        BRK                
C19E: 2F        ???  ; illegal $2F 
C19F: 0C        ???  ; illegal $0C 
C1A0: AB        ???  ; illegal $AB 
C1A1: B0 01     BCS $C1A4          
C1A3: 0C        ???  ; illegal $0C 
C1A4: 3F        ???  ; illegal $3F 
C1A5: B0 00     BCS $C1A7          
C1A7: 44        ???  ; illegal $44 
C1A8: CD 02 40  CMP $4002          
C1AB: B4 F2     LDY $F2,X          
C1AD: AF        ???  ; illegal $AF 
C1AE: 00        BRK                
C1AF: 00        BRK                
C1B0: 44        ???  ; illegal $44 
C1B1: 6A        ROR A              
C1B2: 47        ???  ; illegal $47 
C1B3: 12        ???  ; illegal $12 
C1B4: 4C C6 03  JMP $03C6          
C1B7: 00        BRK                
C1B8: 8C BC 5B  STY $5BBC          
C1BB: 0A        ASL A              
C1BC: 86 30     STX $30            
C1BE: A2 40     LDX #$40           
C1C0: 60        RTS                
C1C1: 84 57     STY $57            
C1C3: 91 20     STA ($20),Y        
C1C5: 44        ???  ; illegal $44 
C1C6: 58        CLI                
C1C7: B5 12     LDA $12,X          
C1C9: 84 61     STY $61            
C1CB: A3        ???  ; illegal $A3 
C1CC: 00        BRK                
C1CD: 44        ???  ; illegal $44 
C1CE: 60        RTS                
C1CF: 47        ???  ; illegal $47 
C1D0: 00        BRK                
C1D1: 00        BRK                
C1D2: 84 5D     STY $5D            
C1D4: B8        CLV                
C1D5: 0F        ???  ; illegal $0F 
C1D6: 0E A9 B0  ASL $B0A9          
C1D9: 14        ???  ; illegal $14 
C1DA: 4C 3F 85  JMP $853F          
C1DD: 01 0C     ORA ($0C,X)        
C1DF: 32        ???  ; illegal $32 
C1E0: F2        ???  ; illegal $F2 
C1E1: 00        BRK                
C1E2: 01 0C     ORA ($0C,X)        
C1E4: 01 F1     ORA ($F1,X)        
C1E6: 12        ???  ; illegal $12 
C1E7: 4C EB 00  JMP $00EB          
C1EA: 40        RTI                
C1EB: 42        ???  ; illegal $42 
C1EC: C4 09     CPY $09            
C1EE: 60        RTS                
C1EF: B4 F3     LDY $F3,X          
C1F1: B1 00     LDA ($00),Y        
C1F3: 00        BRK                
C1F4: 44        ???  ; illegal $44 
C1F5: EB        ???  ; illegal $EB 
C1F6: A6 80     LDX $80            
C1F8: B4 F4     LDY $F4,X          
C1FA: B2        ???  ; illegal $B2 
C1FB: A0 B4     LDY #$B4           
C1FD: F5 B3     SBC $B3,X          
C1FF: C0 B4     CPY #$B4           
C201: F6 B9     INC $B9,X          
C203: 40        RTI                
C204: 00        BRK                
C205: 74        ???  ; illegal $74 
C206: 31 8A     AND ($8A),Y        
C208: 29 0F     AND #$0F           
C20A: 08        PHP                
C20B: B6 36     LDX $36,Y          
C20D: B4 5F     LDY $5F,X          
C20F: B7        ???  ; illegal $B7 
C210: 12        ???  ; illegal $12 
C211: 7C        ???  ; illegal $7C 
C212: 3F        ???  ; illegal $3F 
C213: A8        TAY                
C214: 0A        ASL A              
C215: 00        BRK                
C216: 84 5C     STY $5C            
C218: BC E0 B4  LDY $B4E0,X        
C21B: F7        ???  ; illegal $F7 
C21C: BA        TSX                
C21D: 00        BRK                
C21E: B4 F8     LDY $F8,X          
C220: BB        ???  ; illegal $BB 
C221: 20 B4 F9  JSR $F9B4          
C224: BD 2A 32  LDA $322A,X        
C227: 8C 34 A4  STY $A434          
C22A: 40        RTI                
C22B: B4 FA     LDY $FA,X          
C22D: BE 60 B4  LDX $B460,Y        
C230: FB        ???  ; illegal $FB 
C231: BF        ???  ; illegal $BF 
C232: 80        ???  ; illegal $80 
C233: B4 FC     LDY $FC,X          
C235: 8C 10 A1  STY $A110          
C238: 34        ???  ; illegal $34 
C239: 3F        ???  ; illegal $3F 
C23A: 81 20     STA ($20,X)        
C23C: 74        ???  ; illegal $74 
C23D: 7F        ???  ; illegal $7F 
C23E: 8A        TXA                
C23F: 89        ???  ; illegal $89 
C240: 0F        ???  ; illegal $0F 
C241: 06 8D     ASL $8D            
C243: B6 74     LDX $74,Y          
C245: 31 8A     AND ($8A),Y        
C247: 40        RTI                
C248: E0 4C     CPX #$4C           
C24A: 81 8E     STA ($8E,X)        
C24C: 08        PHP                
C24D: 84 31     STY $31            
C24F: 84 11     STY $11            
C251: 4C 1C BE  JMP $BE1C          
C254: 0F        ???  ; illegal $0F 
C255: 0E 93 BD  ASL $BD93          
C258: 81 0D     STA ($0D,X)        
C25A: B4 F0     LDY $F0,X          
C25C: 8C B6 44  STY $44B6          
C25F: 31 82     AND ($82),Y        
C261: 81 34     STA ($34,X)        
C263: 03        ???  ; illegal $03 
C264: 82        ???  ; illegal $82 
C265: E0 44     CPX #$44           
C267: C4 01     CPY $01            
C269: 05 4A     ORA $4A            
C26B: B6 30     LDX $30,Y          
C26D: A6 AE     LDX $AE            
C26F: 4E D6 01  LSR $01D6          
C272: E4 8C     CPX $8C            
C274: 33        ???  ; illegal $33 
C275: 8E 41 8C  STX $8C41          
C278: 5D 18 00  EOR $0018,X        
C27B: 01 0C     ORA ($0C,X)        
C27D: 02        ???  ; illegal $02 
C27E: 47        ???  ; illegal $47 
C27F: 0A        ASL A              
C280: 8E 8B B3  STX $B38B          
C283: 00        BRK                
C284: 44        ???  ; illegal $44 
C285: 81 93     STA ($93,X)        
C287: 01 B4     ORA ($B4,X)        
C289: 01 9F     ORA ($9F,X)        
C28B: 00        BRK                
C28C: CF        ???  ; illegal $CF 
C28D: B6 80     LDX $80,Y          
C28F: 47        ???  ; illegal $47 
C290: CD 36 01  CMP $0136          
C293: 47        ???  ; illegal $47 
C294: 15 0A     ORA $0A,X          
C296: 00        BRK                
C297: 38        SEC                
C298: 00        BRK                
C299: 44        ???  ; illegal $44 
C29A: 62        ???  ; illegal $62 
C29B: A0 00     LDY #$00           
C29D: 00        BRK                
C29E: 44        ???  ; illegal $44 
C29F: E1 00     SBC ($00,X)        
C2A1: 00        BRK                
C2A2: 84 8B     STY $8B            
C2A4: 6F        ???  ; illegal $6F 
C2A5: 00        BRK                
C2A6: 44        ???  ; illegal $44 
C2A7: E6 00     INC $00            
C2A9: 00        BRK                
C2AA: 84 FC     STY $FC            
C2AC: 6F        ???  ; illegal $6F 
C2AD: 00        BRK                
C2AE: 00        BRK                
C2AF: 44        ???  ; illegal $44 
C2B0: F2        ???  ; illegal $F2 
C2B1: 90 00     BCC $C2B3          
C2B3: 84 63     STY $63            
C2B5: 6F        ???  ; illegal $6F 
C2B6: 05 8C     ORA $8C            
C2B8: 0C        ???  ; illegal $0C 
C2B9: BB        ???  ; illegal $BB 
C2BA: 12        ???  ; illegal $12 
C2BB: 4C D9 A1  JMP $A1D9          
C2BE: 00        BRK                
C2BF: 00        BRK                
C2C0: 44        ???  ; illegal $44 
C2C1: 87        ???  ; illegal $87 
C2C2: A3        ???  ; illegal $A3 
C2C3: 00        BRK                
C2C4: 46 53     LSR $53            
C2C6: A2 14     LDX #$14           
C2C8: 46 54     LSR $54            
C2CA: 96 01     STX $01,Y          
C2CC: 8C F0 A9  STY $A9F0          
C2CF: 00        BRK                
C2D0: 00        BRK                
C2D1: 44        ???  ; illegal $44 
C2D2: 3F        ???  ; illegal $3F 
C2D3: 47        ???  ; illegal $47 
C2D4: 00        BRK                
C2D5: 44        ???  ; illegal $44 
C2D6: 77        ???  ; illegal $77 
C2D7: 51 00     EOR ($00),Y        
C2D9: 44        ???  ; illegal $44 
C2DA: D2        ???  ; illegal $D2 
C2DB: 0F        ???  ; illegal $0F 
C2DC: 00        BRK                
C2DD: 44        ???  ; illegal $44 
C2DE: 3F        ???  ; illegal $3F 
C2DF: A7        ???  ; illegal $A7 
C2E0: 00        BRK                
C2E1: 20 41 FF  JSR $FF41          
C2E4: 47        ???  ; illegal $47 
C2E5: 00        BRK                
C2E6: 84 6A     STY $6A            
C2E8: AA        TAX                
C2E9: C0 74     CPY #$74           
C2EB: 1B        ???  ; illegal $1B 
C2EC: AB        ???  ; illegal $AB 
C2ED: 01 8C     ORA ($8C,X)        
C2EF: FF        ???  ; illegal $FF 
C2F0: AD 00 09  LDA $0900          
C2F3: 0E 80 B8  ASL $B880          
C2F6: 00        BRK                
C2F7: 84 6A     STY $6A            
C2F9: AE 01 8C  LDX $8C01          
C2FC: 46 AF     LSR $AF            
C2FE: 00        BRK                
C2FF: 84 1E     STY $1E            
C301: B1 00     LDA ($00),Y        
C303: 12        ???  ; illegal $12 
C304: 4C EB 03  JMP $03EB          
C307: C0 44     CPY #$44           
C309: A0 B2     LDY #$B2           
C30B: 00        BRK                
C30C: 4C 1A 30  JMP $301A          
C30F: 14        ???  ; illegal $14 
C310: B4 44     LDY $44,X          
C312: B9 00 00  LDA $0000,Y        
C315: 44        ???  ; illegal $44 
C316: 1A        ???  ; illegal $1A 
C317: B7        ???  ; illegal $B7 
C318: 01 0C     ORA ($0C,X)        
C31A: 08        PHP                
C31B: F2        ???  ; illegal $F2 
C31C: 40        RTI                
C31D: 44        ???  ; illegal $44 
C31E: 67        ???  ; illegal $67 
C31F: A8        TAY                
C320: 00        BRK                
C321: 44        ???  ; illegal $44 
C322: DF        ???  ; illegal $DF 
C323: 06 05     ASL $05            
C325: 04        ???  ; illegal $04 
C326: 0E E9 04  ASL $04E9          
C329: 62        ???  ; illegal $62 
C32A: 74        ???  ; illegal $74 
C32B: 15 BA     ORA $BA,X          
C32D: 00        BRK                
C32E: 44        ???  ; illegal $44 
C32F: 3F        ???  ; illegal $3F 
C330: B3        ???  ; illegal $B3 
C331: AA        TAX                
C332: 86 E1     STX $E1            
C334: 85 92     STA $92            
C336: 45 0F     EOR $0F            
C338: 88        DEY                
C339: AC F0 A4  LDY $A4F0          
C33C: D4        ???  ; illegal $D4 
C33D: 0D 14 4B  ORA $4B14          
C340: DF        ???  ; illegal $DF 
C341: 07        ???  ; illegal $07 
C342: 09 37     ORA #$37           
C344: 08        PHP                
C345: 88        DEY                
C346: AA        TAX                
C347: 40        RTI                
C348: 4C A0 81  JMP $81A0          
C34B: 2D 04 80  AND $8004          
C34E: 89        ???  ; illegal $89 
C34F: 32        ???  ; illegal $32 
C350: 74        ???  ; illegal $74 
C351: 34        ???  ; illegal $34 
C352: 8A        TXA                
C353: 2D 0C 0F  AND $0F0C          
C356: 1C        ???  ; illegal $1C 
C357: AA        TAX                
C358: 40        RTI                
C359: B4 34     LDY $34,X          
C35B: 82        ???  ; illegal $82 
C35C: 41 77     EOR ($77,X)        
C35E: 01 8D     ORA ($8D,X)        
C360: 20 74 7F  JSR $7F74          
C363: 8F        ???  ; illegal $8F 
C364: 74        ???  ; illegal $74 
C365: 44        ???  ; illegal $44 
C366: 3F        ???  ; illegal $3F 
C367: AA        TAX                
C368: 2A        ROL A              
C369: 60        RTS                
C36A: 44        ???  ; illegal $44 
C36B: 53        ???  ; illegal $53 
C36C: 8C 40 74  STY $7440          
C36F: 1B        ???  ; illegal $1B 
C370: 10 52     BPL $C3C4          
C372: 74        ???  ; illegal $74 
C373: 3F        ???  ; illegal $3F 
C374: A6 69     LDX $69            
C376: 04        ???  ; illegal $04 
C377: D2        ???  ; illegal $D2 
C378: 8E 28 20  STX $2028          
C37B: 44        ???  ; illegal $44 
C37C: 54        ???  ; illegal $54 
C37D: A8        TAY                
C37E: 21 B7     AND ($B7,X)        
C380: 01 A6     ORA ($A6,X)        
C382: 41 B4     EOR ($B4,X)        
C384: 80        ???  ; illegal $80 
C385: 5A        ???  ; illegal $5A 
C386: 00        BRK                
C387: 84 38     STY $38            
C389: A1 80     LDA ($80,X)        
C38B: 52        ???  ; illegal $52 
C38C: 74        ???  ; illegal $74 
C38D: 3F        ???  ; illegal $3F 
C38E: A6 51     LDX $51            
C390: 4C 1C A3  JMP $A31C          
C393: 51 4C     EOR ($4C),Y        
C395: 1D A3 51  ORA $51A3,X        
C398: 4C 1E A3  JMP $A31E          
C39B: 02        ???  ; illegal $02 
C39C: 51 4C     EOR ($4C),Y        
C39E: 1F        ???  ; illegal $1F 
C39F: A3        ???  ; illegal $A3 
C3A0: 51 4C     EOR ($4C),Y        
C3A2: 1B        ???  ; illegal $1B 
C3A3: A3        ???  ; illegal $A3 
C3A4: 4F        ???  ; illegal $4F 
C3A5: 0C        ???  ; illegal $0C 
C3A6: 14        ???  ; illegal $14 
C3A7: A2 52     LDX #$52           
C3A9: 74        ???  ; illegal $74 
C3AA: 3F        ???  ; illegal $3F 
C3AB: A6 0A     LDX $0A            
C3AD: 51 4C     EOR ($4C),Y        
C3AF: 20 A3 51  JSR $51A3          
C3B2: 4C 21 A3  JMP $A321          
C3B5: 52        ???  ; illegal $52 
C3B6: 74        ???  ; illegal $74 
C3B7: 3F        ???  ; illegal $3F 
C3B8: A6 52     LDX $52            
C3BA: 74        ???  ; illegal $74 
C3BB: 3F        ???  ; illegal $3F 
C3BC: A6 AA     LDX $AA            
C3BE: 52        ???  ; illegal $52 
C3BF: 74        ???  ; illegal $74 
C3C0: 3F        ???  ; illegal $3F 
C3C1: A6 52     LDX $52            
C3C3: 74        ???  ; illegal $74 
C3C4: 3F        ???  ; illegal $3F 
C3C5: A6 52     LDX $52            
C3C7: 74        ???  ; illegal $74 
C3C8: 3F        ???  ; illegal $3F 
C3C9: A6 61     LDX $61            
C3CB: 34        ???  ; illegal $34 
C3CC: 01 A9     ORA ($A9,X)        
C3CE: 6A        ROR A              
C3CF: 20 74 7F  JSR $7F74          
C3D2: B5 32     LDA $32,X          
C3D4: 74        ???  ; illegal $74 
C3D5: 67        ???  ; illegal $67 
C3D6: 90 22     BCC $C3FA          
C3D8: 73        ???  ; illegal $73 
C3D9: CF        ???  ; illegal $CF 
C3DA: 0C        ???  ; illegal $0C 
C3DB: 52        ???  ; illegal $52 
C3DC: 72        ???  ; illegal $72 
C3DD: FF        ???  ; illegal $FF 
C3DE: A6 02     LDX $02            
C3E0: E0 4C     CPX #$4C           
C3E2: 30 AC     BMI $C390          
C3E4: E0 44     CPX #$44           
C3E6: 66 B3     ROR $B3            
C3E8: 36 74     ROL $74,X          
C3EA: A6 47     LDX $47            
C3EC: 72        ???  ; illegal $72 
C3ED: 74        ???  ; illegal $74 
C3EE: 3F        ???  ; illegal $3F 
C3EF: A6 28     LDX $28            
C3F1: 00        BRK                
C3F2: 41 11     EOR ($11,X)        
C3F4: B8        CLV                
C3F5: 2A        ROL A              
C3F6: B6 3F     LDX $3F,Y          
C3F8: A6 74     LDX $74            
C3FA: 74        ???  ; illegal $74 
C3FB: 15 A9     ORA $A9,X          
C3FD: CA        DEX                
C3FE: 4E 3F BD  LSR $BD3F          
C401: 02        ???  ; illegal $02 
C402: 44        ???  ; illegal $44 
C403: B4 3F     LDY $3F,X          
C405: AF        ???  ; illegal $AF 
C406: 01 8C     ORA ($8C,X)        
C408: 40        RTI                
C409: AB        ???  ; illegal $AB 
C40A: 54        ???  ; illegal $54 
C40B: 74        ???  ; illegal $74 
C40C: 3F        ???  ; illegal $3F 
C40D: B1 00     LDA ($00),Y        
C40F: 74        ???  ; illegal $74 
C410: BC 54 00  LDY $0054,X        
C413: 0A        ASL A              
C414: 86 CE     STX $CE            
C416: 05 D4     ORA $D4            
C418: 7C        ???  ; illegal $7C 
C419: EB        ???  ; illegal $EB 
C41A: 00        BRK                
C41B: 32        ???  ; illegal $32 
C41C: 74        ???  ; illegal $74 
C41D: 3F        ???  ; illegal $3F 
C41E: A0 00     LDY #$00           
C420: 44        ???  ; illegal $44 
C421: 33        ???  ; illegal $33 
C422: AE 01 2A  LDX $2A01          
C425: B6 15     LDX $15,Y          
C427: 88        DEY                
C428: 20 44 6A  JSR $6A44          
C42B: B0 40     BCS $C46D          
C42D: 40        RTI                
C42E: 40        RTI                
C42F: B0 E0     BCS $C411          
C431: 44        ???  ; illegal $44 
C432: C4 01     CPY $01            
C434: 00        BRK                
C435: 00        BRK                
C436: 46 52     LSR $52            
C438: BC D4 7C  LDY $7CD4,X        
C43B: EB        ???  ; illegal $EB 
C43C: 00        BRK                
C43D: 21 B4     AND ($B4,X)        
C43F: C0 A0     CPY #$A0           
C441: 00        BRK                
C442: 44        ???  ; illegal $44 
C443: 33        ???  ; illegal $33 
C444: BE 01 96  LDX $9601,Y        
C447: 74        ???  ; illegal $74 
C448: CC 09 4A  CPY $4A09          
C44B: B6 66     LDX $66,Y          
C44D: BB        ???  ; illegal $BB 
C44E: 00        BRK                
C44F: 44        ???  ; illegal $44 
C450: 3F        ???  ; illegal $3F 
C451: B9 E0 44  LDA $44E0,Y        
C454: C4 01     CPY $01            
C456: 02        ???  ; illegal $02 
C457: 00        BRK                
C458: 84 1E     STY $1E            
C45A: 47        ???  ; illegal $47 
C45B: 21 B4     AND ($B4,X)        
C45D: 02        ???  ; illegal $02 
C45E: A0 00     LDY #$00           
C460: 44        ???  ; illegal $44 
C461: 3F        ???  ; illegal $3F 
C462: 84 52     STY $52            
C464: 74        ???  ; illegal $74 
C465: 3F        ???  ; illegal $3F 
C466: B0 02     BCS $C46A          
C468: 00        BRK                
C469: 44        ???  ; illegal $44 
C46A: CD 02 01  CMP $0102          
C46D: 22        ???  ; illegal $22 
C46E: 00        BRK                
C46F: B8        CLV                
C470: 21 B4     AND ($B4,X)        
C472: 02        ???  ; illegal $02 
C473: 92        ???  ; illegal $92 
C474: A9 07     LDA #$07           
C476: 07        ???  ; illegal $07 
C477: 90 A8     BCC $C421          
C479: 69 07     ADC #$07           
C47B: 04        ???  ; illegal $04 
C47C: 90 69     BCC $C4E7          
C47E: 07        ???  ; illegal $07 
C47F: 0A        ASL A              
C480: 90 89     BCC $C40B          
C482: 07        ???  ; illegal $07 
C483: 05 90     ORA $90            
C485: 12        ???  ; illegal $12 
C486: 4C 3F 86  JMP $863F          
C489: 32        ???  ; illegal $32 
C48A: B6 7B     LDX $7B,Y          
C48C: D9 0C 00  CMP $000C,Y        
C48F: 4C 30 A2  JMP $A230          
C492: 80        ???  ; illegal $80 
C493: B4 C7     LDY $C7,X          
C495: 0B        ???  ; illegal $0B 
C496: 52        ???  ; illegal $52 
C497: 6C 2D 1D  JMP ($1D2D)        
C49A: 2A        ROL A              
C49B: 00        BRK                
C49C: 44        ???  ; illegal $44 
C49D: 31 9A     AND ($9A),Y        
C49F: 44        ???  ; illegal $44 
C4A0: 34        ???  ; illegal $34 
C4A1: 33        ???  ; illegal $33 
C4A2: 9E        ???  ; illegal $9E 
C4A3: 89        ???  ; illegal $89 
C4A4: 07        ???  ; illegal $07 
C4A5: 0C        ???  ; illegal $0C 
C4A6: 90 A9     BCC $C451          
C4A8: 07        ???  ; illegal $07 
C4A9: 0C        ???  ; illegal $0C 
C4AA: 90 A8     BCC $C454          
C4AC: A9 07     LDA #$07           
C4AE: 0A        ASL A              
C4AF: 90 A9     BCC $C45A          
C4B1: 07        ???  ; illegal $07 
C4B2: 0B        ???  ; illegal $0B 
C4B3: 90 A9     BCC $C45E          
C4B5: 07        ???  ; illegal $07 
C4B6: 01 90     ORA ($90,X)        
C4B8: 00        BRK                
C4B9: 44        ???  ; illegal $44 
C4BA: DB        ???  ; illegal $DB 
C4BB: 00        BRK                
C4BC: 2C 12 4C  BIT $4C12          
C4BF: 3F        ???  ; illegal $3F 
C4C0: 9A        TXS                
C4C1: 52        ???  ; illegal $52 
C4C2: 74        ???  ; illegal $74 
C4C3: 3F        ???  ; illegal $3F 
C4C4: B0 05     BCS $C4CB          
C4C6: 3E 48 A3  ROL $A348,X        
C4C9: 14        ???  ; illegal $14 
C4CA: 4E 3F 47  LSR $473F          
C4CD: 00        BRK                
C4CE: 05 0C     ORA $0C            
C4D0: 04        ???  ; illegal $04 
C4D1: 8C 2A B4  STY $B42A          
C4D4: 66 9E     ROR $9E            
C4D6: 34        ???  ; illegal $34 
C4D7: 71 FF     ADC ($FF),Y        
C4D9: 91 01     STA ($01),Y        
C4DB: 8C 02 78  STY $7802          
C4DE: 80        ???  ; illegal $80 
C4DF: 00        BRK                
C4E0: 74        ???  ; illegal $74 
C4E1: 7F        ???  ; illegal $7F 
C4E2: 8E 69 F7  STX $F769          
C4E5: 04        ???  ; illegal $04 
C4E6: A9 00     LDA #$00           
C4E8: 44        ???  ; illegal $44 
C4E9: CC 06 21  CPY $2106          
C4EC: B4 02     LDY $02,X          
C4EE: A0 2C     LDY #$2C           
C4F0: 00        BRK                
C4F1: 44        ???  ; illegal $44 
C4F2: 3F        ???  ; illegal $3F 
C4F3: 84 52     STY $52            
C4F5: 74        ???  ; illegal $74 
C4F6: 3F        ???  ; illegal $3F 
C4F7: B0 00     BCS $C4F9          
C4F9: 4C 3F 81  JMP $813F          
C4FC: 01 0C     ORA ($0C,X)        
C4FE: 0A        ASL A              
C4FF: F2        ???  ; illegal $F2 
C500: 00        BRK                
C501: 8D 0F 3B  STA $3B0F          
C504: B3        ???  ; illegal $B3 
C505: 12        ???  ; illegal $12 
C506: 4C EB 00  JMP $00EB          
C509: 8D 0F 3B  STA $3B0F          
C50C: AF        ???  ; illegal $AF 
C50D: 0F        ???  ; illegal $0F 
C50E: 0E E1 F1  ASL $F1E1          
C511: 00        BRK                
C512: 61 00     ADC ($00,X)        
C514: 00        BRK                
C515: AD 01 0C  LDA $0C01          
C518: 0A        ASL A              
C519: F2        ???  ; illegal $F2 
C51A: 61 00     ADC ($00,X)        
C51C: 00        BRK                
C51D: A7        ???  ; illegal $A7 
C51E: 0F        ???  ; illegal $0F 
C51F: 8E 60 AB  STX $AB60          
C522: 00        BRK                
C523: 2B        ???  ; illegal $2B 
C524: 8C 02 9D  STY $9D02          
C527: 0F        ???  ; illegal $0F 
C528: 0E 61 F1  ASL $F161          
C52B: 01 0C     ORA ($0C,X)        
C52D: B0 B9     BCS $C4E8          
C52F: 0F        ???  ; illegal $0F 
C530: 8E 60 B1  STX $B160          
C533: 22        ???  ; illegal $22 
C534: 00        BRK                
C535: 4C 1A B5  JMP $B51A          
C538: 14        ???  ; illegal $14 
C539: B0 EE     BCS $C529          
C53B: 03        ???  ; illegal $03 
C53C: 12        ???  ; illegal $12 
C53D: 4C 3F 47  JMP $473F          
C540: 52        ???  ; illegal $52 
C541: 76 53     ROR $53,X          
C543: FE 0A 01  INC $010A,X        
C546: 4C 51 BC  JMP $BC51          
C549: 68        PLA                
C54A: B4 D3     LDY $D3,X          
C54C: 04        ???  ; illegal $04 
C54D: 49 34     EOR #$34           
C54F: 80        ???  ; illegal $80 
C550: BB        ???  ; illegal $BB 
C551: 44        ???  ; illegal $44 
C552: 34        ???  ; illegal $34 
C553: D2        ???  ; illegal $D2 
C554: 03        ???  ; illegal $03 
C555: 22        ???  ; illegal $22 
C556: 16 4B     ASL $4B,X          
C558: F1 0C     SBC ($0C),Y        
C55A: 45 37     EOR $37            
C55C: 80        ???  ; illegal $80 
C55D: BA        TSX                
C55E: CA        DEX                
C55F: B6 E3     LDX $E3,Y          
C561: 0A        ASL A              
C562: EF        ???  ; illegal $EF 
C563: 2E 2F BE  ROL $BE2F          
C566: 20 00 44  JSR $4400          
C569: A1 81     LDA ($81,X)        
C56B: 80        ???  ; illegal $80 
C56C: 44        ???  ; illegal $44 
C56D: 1E 82 00  ASL $0082,X        
C570: 44        ???  ; illegal $44 
C571: F3        ???  ; illegal $F3 
C572: 08        PHP                
C573: 01 CC     ORA ($CC,X)        
C575: F0 F5     BEQ $C56C          
C577: 00        BRK                
C578: 01 CC     ORA ($CC,X)        
C57A: 0F        ???  ; illegal $0F 
C57B: F5 00     SBC $00,X          
C57D: 44        ???  ; illegal $44 
C57E: DD 00 00  CMP $0000,X        
C581: 84 F7     STY $F7            
C583: 6F        ???  ; illegal $6F 
C584: 00        BRK                
C585: 44        ???  ; illegal $44 
C586: F6 90     INC $90,X          
C588: 03        ???  ; illegal $03 
C589: 00        BRK                
C58A: 84 63     STY $63            
C58C: 6F        ???  ; illegal $6F 
C58D: 00        BRK                
C58E: C4 67     CPY $67            
C590: 90 00     BCC $C592          
C592: 44        ???  ; illegal $44 
C593: A6 9A     LDX $9A            
C595: B6 34     LDX $34,Y          
C597: 1E 5C 80  ASL $805C,X        
C59A: 80        ???  ; illegal $80 
C59B: 42        ???  ; illegal $42 
C59C: D5 0C     CMP $0C,X          
C59E: 01 8C     ORA ($8C,X)        
C5A0: 0E 8E 00  ASL $008E          
C5A3: 84 6A     STY $6A            
C5A5: 8F        ???  ; illegal $8F 
C5A6: 00        BRK                
C5A7: 44        ???  ; illegal $44 
C5A8: 62        ???  ; illegal $62 
C5A9: 83        ???  ; illegal $83 
C5AA: 02        ???  ; illegal $02 
C5AB: 01 0C     ORA ($0C,X)        
C5AD: 06 47     ASL $47            
C5AF: 00        BRK                
C5B0: 44        ???  ; illegal $44 
C5B1: E2        ???  ; illegal $E2 
C5B2: 00        BRK                
C5B3: 16 4C     ASL $4C,X          
C5B5: 8B        ???  ; illegal $8B 
C5B6: A2 81     LDX #$81           
C5B8: B4 FF     LDY $FF,X          
C5BA: 99 18 00  STA $0018,Y        
C5BD: 84 1E     STY $1E            
C5BF: 9F        ???  ; illegal $9F 
C5C0: 00        BRK                
C5C1: 84 1E     STY $1E            
C5C3: 95 B6     STA $B6,X          
C5C5: 76 30     ROR $30,X          
C5C7: A4 01     LDY $01            
C5C9: CC 0F 95  CPY $950F          
C5CC: A0 94     LDY #$94           
C5CE: 6A        ROR A              
C5CF: F2        ???  ; illegal $F2 
C5D0: BE A1 34  LDX $34A1,Y        
C5D3: 0B        ???  ; illegal $0B 
C5D4: A6 01     LDX $01            
C5D6: CC 0F 94  CPY $940F          
C5D9: 01 CC     ORA ($CC,X)        
C5DB: 6F        ???  ; illegal $6F 
C5DC: 9D 90 85  STA $8590,X        
C5DF: 2B        ???  ; illegal $2B 
C5E0: 10 A8     BPL $C58A          
C5E2: 00        BRK                
C5E3: 84 1E     STY $1E            
C5E5: 9E        ???  ; illegal $9E 
C5E6: 01 CC     ORA ($CC,X)        
C5E8: 0F        ???  ; illegal $0F 
C5E9: 36 01     ROL $01,X          
C5EB: CC 4F A1  CPY $A14F          
C5EE: 12        ???  ; illegal $12 
C5EF: 00        BRK                
C5F0: C4 7F     CPY $7F            
C5F2: 8D 00 84  STA $8400          
C5F5: 1E 92 0C  ASL $0C92,X        
C5F8: 8C 81 A3  STY $A381          
C5FB: 2C 8C BC  BIT $BC8C          
C5FE: 3C        ???  ; illegal $3C 
C5FF: 28        PLP                
C600: 01 CC     ORA ($CC,X)        
C602: 5F        ???  ; illegal $5F 
C603: A9 26     LDA #$26           
C605: 74        ???  ; illegal $74 
C606: A5 93     LDA $93            
C608: 94 71     STY $71,X          
C60A: F6 8A     INC $8A,X          
C60C: 00        BRK                
C60D: 44        ???  ; illegal $44 
C60E: A6 9B     LDX $9B            
C610: B0 80     BCS $C592          
C612: 64        ???  ; illegal $64 
C613: 34        ???  ; illegal $34 
C614: B4 B6     LDY $B6,X          
C616: B4 1E     LDY $1E,X          
C618: 54        ???  ; illegal $54 
C619: C4 B4     CPY $B4            
C61B: DC        ???  ; illegal $DC 
C61C: 0C        ???  ; illegal $0C 
C61D: E2        ???  ; illegal $E2 
C61E: 9C        ???  ; illegal $9C 
C61F: 1E AD 02  ASL $02AD,X        
C622: 05 0C     ORA $0C            
C624: 19 B8 CD  ORA $CDB8,Y        
C627: 36 FF     ROL $FF,X          
C629: 78        SEI                
C62A: 07        ???  ; illegal $07 
C62B: 0C        ???  ; illegal $0C 
C62C: 05 AF     ORA $AF            
C62E: 85 33     STA $33            
C630: 17        ???  ; illegal $17 
C631: B1 00     LDA ($00),Y        
C633: 60        RTS                
C634: 74        ???  ; illegal $74 
C635: 7F        ???  ; illegal $7F 
C636: AC C0 64  LDY $64C0          
C639: 3F        ???  ; illegal $3F 
C63A: BA        TSX                
C63B: C0 42     CPY #$42           
C63D: F3        ???  ; illegal $F3 
C63E: 0E 17 0A  ASL $0A17          
C641: 00        BRK                
C642: BD 88 96  LDA $9688,X        
C645: 4C E3 03  JMP $03E3          
C648: 34        ???  ; illegal $34 
C649: 4A        LSR A              
C64A: F1 08     SBC ($08),Y        
C64C: ED 0F B3  SBC $B30F          
C64F: AA        TAX                
C650: 00        BRK                
C651: 44        ???  ; illegal $44 
C652: D9 AE 00  CMP $00AE,Y        
C655: 40        RTI                
C656: B4 E6     LDY $E6,X          
C658: 0C        ???  ; illegal $0C 
C659: D4        ???  ; illegal $D4 
C65A: 74        ???  ; illegal $74 
C65B: 12        ???  ; illegal $12 
C65C: B2        ???  ; illegal $B2 
C65D: D1 04     CMP ($04),Y        
C65F: 1B        ???  ; illegal $1B 
C660: BB        ???  ; illegal $BB 
C661: D4        ???  ; illegal $D4 
C662: 74        ???  ; illegal $74 
C663: 30 B2     BMI $C617          
C665: 8A        TXA                
C666: 94 6B     STY $6B,X          
C668: D4        ???  ; illegal $D4 
C669: 0F        ???  ; illegal $0F 
C66A: 00        BRK                
C66B: 43        ???  ; illegal $43 
C66C: FF        ???  ; illegal $FF 
C66D: B9 85 B4  LDA $B485,Y        
C670: 02        ???  ; illegal $02 
C671: BF        ???  ; illegal $BF 
C672: 8D 74 F8  STA $F874          
C675: 8C 0A 00  STY $000A          
C678: 44        ???  ; illegal $44 
C679: A1 81     LDA ($81,X)        
C67B: 00        BRK                
C67C: 84 1E     STY $1E            
C67E: 47        ???  ; illegal $47 
C67F: 4D 36 FF  EOR $FF36          
C682: A0 45     LDY #$45           
C684: 31 03     AND ($03),Y        
C686: AE 02 F0  LDX $F002          
C689: 04        ???  ; illegal $04 
C68A: 3F        ???  ; illegal $3F 
C68B: A0 60     LDY #$60           
C68D: 44        ???  ; illegal $44 
C68E: 34        ???  ; illegal $34 
C68F: 9C        ???  ; illegal $9C 
C690: 00        BRK                
C691: 44        ???  ; illegal $44 
C692: A6 82     LDX $82            
C694: 76 74     ROR $74,X          
C696: 3F        ???  ; illegal $3F 
C697: AD 20 10  LDA $1020          
C69A: 06 3F     ASL $3F            
C69C: A0 60     LDY #$60           
C69E: 49 C5     EOR #$C5           
C6A0: 84 00     STY $00            
C6A2: 4C 1A 1E  JMP $1E1A          
C6A5: 00        BRK                
C6A6: 44        ???  ; illegal $44 
C6A7: A6 A9     LDX $A9            
C6A9: 28        PLP                
C6AA: C9 04     CMP #$04           
C6AC: 1C        ???  ; illegal $1C 
C6AD: 98        TYA                
C6AE: 6D 84 FF  ADC $FF84          
C6B1: A0 41     LDY #$41           
C6B3: 09 B5     ORA #$B5           
C6B5: 88        DEY                
C6B6: 04        ???  ; illegal $04 
C6B7: 0E B8 9B  ASL $9BB8          
C6BA: 0C        ???  ; illegal $0C 
C6BB: 01 0C     ORA ($0C,X)        
C6BD: 07        ???  ; illegal $07 
C6BE: 47        ???  ; illegal $47 
C6BF: 00        BRK                
C6C0: 44        ???  ; illegal $44 
C6C1: E2        ???  ; illegal $E2 
C6C2: 00        BRK                
C6C3: 52        ???  ; illegal $52 
C6C4: 74        ???  ; illegal $74 
C6C5: 3F        ???  ; illegal $3F 
C6C6: A3        ???  ; illegal $A3 
C6C7: 00        BRK                
C6C8: 44        ???  ; illegal $44 
C6C9: 3F        ???  ; illegal $3F 
C6CA: 47        ???  ; illegal $47 
C6CB: 28        PLP                
C6CC: 00        BRK                
C6CD: 84 1E     STY $1E            
C6CF: BF        ???  ; illegal $BF 
C6D0: A1 34     LDA ($34,X)        
C6D2: 02        ???  ; illegal $02 
C6D3: B1 54     LDA ($54),Y        
C6D5: 71 CC     ADC ($CC),Y        
C6D7: 9E        ???  ; illegal $9E 
C6D8: 01 CC     ORA ($CC,X)        
C6DA: F0 A1     BEQ $C67D          
C6DC: 08        PHP                
C6DD: 00        BRK                
C6DE: 44        ???  ; illegal $44 
C6DF: 31 A4     AND ($A4),Y        
C6E1: 70 84     BVS $C667          
C6E3: 3F        ???  ; illegal $3F 
C6E4: A0 41     LDY #$41           
C6E6: 49 E8     EOR #$E8           
C6E8: 8F        ???  ; illegal $8F 
C6E9: 00        BRK                
C6EA: 84 66     STY $66            
C6EC: 39 88 41  AND $4188,Y        
C6EF: 34        ???  ; illegal $34 
C6F0: 02        ???  ; illegal $02 
C6F1: 99 60 B4  STA $B460,Y        
C6F4: 3F        ???  ; illegal $3F 
C6F5: 9C        ???  ; illegal $9C 
C6F6: 54        ???  ; illegal $54 
C6F7: 51 FF     EOR ($FF),Y        
C6F9: A2 F4     LDX #$F4           
C6FB: 74        ???  ; illegal $74 
C6FC: 3F        ???  ; illegal $3F 
C6FD: 92        ???  ; illegal $92 
C6FE: 10 00     BPL $C700          
C700: 44        ???  ; illegal $44 
C701: 30 92     BMI $C695          
C703: EA        NOP                
C704: B6 1E     LDX $1E,Y          
C706: A1 34     LDA ($34,X)        
C708: B0 F3     BCS $C6FD          
C70A: 9E        ???  ; illegal $9E 
C70B: 05 0C     ORA $0C            
C70D: 10 AB     BPL $C6BA          
C70F: 00        BRK                
C710: C9 34     CMP #$34           
C712: 1C        ???  ; illegal $1C 
C713: A5 00     LDA $00            
C715: 44        ???  ; illegal $44 
C716: F3        ???  ; illegal $F3 
C717: 00        BRK                
C718: 00        BRK                
C719: 44        ???  ; illegal $44 
C71A: EA        NOP                
C71B: 09 E0     ORA #$E0           
C71D: 44        ???  ; illegal $44 
C71E: 1E F8 8E  ASL $8EF8,X        
C721: 81 B4     STA ($B4,X)        
C723: 02        ???  ; illegal $02 
C724: AC 01 CC  LDY $CC01          
C727: F4        ???  ; illegal $F4 
C728: AA        TAX                
C729: 52        ???  ; illegal $52 
C72A: B4 1E     LDY $1E,X          
C72C: A3        ???  ; illegal $A3 
C72D: 60        RTS                
C72E: B4 A5     LDY $A5,X          
C730: 8A        TXA                
C731: B2        ???  ; illegal $B2 
C732: 80        ???  ; illegal $80 
C733: 44        ???  ; illegal $44 
C734: 1E B8 54  ASL $54B8,X        
C737: B0 F1     BCS $C72A          
C739: 8A        TXA                
C73A: 49 0F     EOR #$0F           
C73C: 08        PHP                
C73D: AF        ???  ; illegal $AF 
C73E: 70 36     BVS $C776          
C740: 3F        ???  ; illegal $3F 
C741: 8D 9A C0  STA $C09A          
C744: 74        ???  ; illegal $74 
C745: 7F        ???  ; illegal $7F 
C746: BE E1 37  LDX $37E1,Y        
C749: 0A        ASL A              
C74A: F8        SED                
C74B: 01 74     ORA ($74,X)        
C74D: FE B0 A1  INC $A1B0,X        
C750: 34        ???  ; illegal $34 
C751: 08        PHP                
C752: BB        ???  ; illegal $BB 
C753: 82        ???  ; illegal $82 
C754: 74        ???  ; illegal $74 
C755: 6B        ???  ; illegal $6B 
C756: DB        ???  ; illegal $DB 
C757: 0C        ???  ; illegal $0C 
C758: 12        ???  ; illegal $12 
C759: 44        ???  ; illegal $44 
C75A: EF        ???  ; illegal $EF 
C75B: 0C        ???  ; illegal $0C 
C75C: 00        BRK                
C75D: 4C 1A B5  JMP $B51A          
C760: 54        ???  ; illegal $54 
C761: 6B        ???  ; illegal $6B 
C762: FF        ???  ; illegal $FF 
C763: B4 22     LDY $22,X          
C765: D0 84     BNE $C6EB          
C767: 3F        ???  ; illegal $3F 
C768: BD 01 B4  LDA $B401,X        
C76B: FF        ???  ; illegal $FF 
C76C: B0 00     BCS $C76E          
C76E: 44        ???  ; illegal $44 
C76F: DB        ???  ; illegal $DB 
C770: 07        ???  ; illegal $07 
C771: 01 74     ORA ($74,X)        
C773: F6 B0     INC $B0,X          
C775: 80        ???  ; illegal $80 
C776: A1 B4     LDA ($B4,X)        
C778: 01 B9     ORA ($B9,X)        
C77A: 12        ???  ; illegal $12 
C77B: 4C A0 30  JMP $30A0          
C77E: B4 71     LDY $71,X          
C780: F2        ???  ; illegal $F2 
C781: 08        PHP                
C782: 00        BRK                
C783: 84 1E     STY $1E            
C785: A5 00     LDA $00            
C787: 00        BRK                
C788: 44        ???  ; illegal $44 
C789: A1 81     LDA ($81,X)        
C78B: 01 0C     ORA ($0C,X)        
C78D: 02        ???  ; illegal $02 
C78E: 82        ???  ; illegal $82 
C78F: 74        ???  ; illegal $74 
C790: 84 1E     STY $1E            
C792: 83        ???  ; illegal $83 
C793: 01 0C     ORA ($0C,X)        
C795: 01 89     ORA ($89,X)        
C797: 20 12 4C  JSR $4C12          
C79A: E5 00     SBC $00            
C79C: 32        ???  ; illegal $32 
C79D: 74        ???  ; illegal $74 
C79E: E5 9E     SBC $9E            
C7A0: 60        RTS                
C7A1: 4C EA 0E  JMP $0EEA          
C7A4: 74        ???  ; illegal $74 
C7A5: 7C        ???  ; illegal $7C 
C7A6: 15 8B     ORA $8B,X          
C7A8: 68        PLA                
C7A9: 00        BRK                
C7AA: 44        ???  ; illegal $44 
C7AB: D9 47 80  CMP $8047,Y        
C7AE: 74        ???  ; illegal $74 
C7AF: 1A        ???  ; illegal $1A 
C7B0: 8A        TXA                
C7B1: 80        ???  ; illegal $80 
C7B2: B4 1E     LDY $1E,X          
C7B4: 1D 90 84  ORA $8490,X        
C7B7: EA        NOP                
C7B8: 0D 80 4D  ORA $4D80          
C7BB: 74        ???  ; illegal $74 
C7BC: F0 9C     BEQ $C75A          
C7BE: 00        BRK                
C7BF: 44        ???  ; illegal $44 
C7C0: F3        ???  ; illegal $F3 
C7C1: 47        ???  ; illegal $47 
C7C2: 01 8C     ORA ($8C,X)        
C7C4: FF        ???  ; illegal $FF 
C7C5: 8F        ???  ; illegal $8F 
C7C6: 00        BRK                
C7C7: 84 6A     STY $6A            
C7C9: 9B        ???  ; illegal $9B 
C7CA: 00        BRK                
C7CB: C1 0C     CMP ($0C,X)        
C7CD: 04        ???  ; illegal $04 
C7CE: 3C        ???  ; illegal $3C 
C7CF: 01 0C     ORA ($0C,X)        
C7D1: 03        ???  ; illegal $03 
C7D2: F5 01     SBC $01,X          
C7D4: 0C        ???  ; illegal $0C 
C7D5: 02        ???  ; illegal $02 
C7D6: F5 C0     SBC $C0,X          
C7D8: 44        ???  ; illegal $44 
C7D9: D9 9D 00  CMP $009D,Y        
C7DC: 15 4A     ORA $4A,X          
C7DE: FF        ???  ; illegal $FF 
C7DF: 33        ???  ; illegal $33 
C7E0: 01 34     ORA ($34,X)        
C7E2: 04        ???  ; illegal $04 
C7E3: A4 01     LDY $01            
C7E5: 0C        ???  ; illegal $0C 
C7E6: 03        ???  ; illegal $03 
C7E7: 8D 00 44  STA $4400          
C7EA: F4        ???  ; illegal $F4 
C7EB: 47        ???  ; illegal $47 
C7EC: 04        ???  ; illegal $04 
C7ED: 01 8C     ORA ($8C,X)        
C7EF: F7        ???  ; illegal $F7 
C7F0: 8F        ???  ; illegal $8F 
C7F1: 00        BRK                
C7F2: 44        ???  ; illegal $44 
C7F3: 3F        ???  ; illegal $3F 
C7F4: F4        ???  ; illegal $F4 
C7F5: 0D 04 02  ORA $0204          
C7F8: 58        CLI                
C7F9: 00        BRK                
C7FA: 44        ???  ; illegal $44 
C7FB: 87        ???  ; illegal $87 
C7FC: 91 88     STA ($88),Y        
C7FE: 4E BC F2  LSR $F2BC          
C801: 0D 41 B4  ORA $B441          
C804: 01 11     ORA ($11,X)        
C806: 52        ???  ; illegal $52 
C807: 74        ???  ; illegal $74 
C808: A6 9F     LDX $9F            
C80A: 51 4C     EOR ($4C),Y        
C80C: 1C        ???  ; illegal $1C 
C80D: 94 00     STY $00,X          
C80F: 01 8C     ORA ($8C,X)        
C811: 0E BE 01  ASL $01BE          
C814: AC 02 F7  LDY $F702          
C817: 14        ???  ; illegal $14 
C818: 4C 35 AE  JMP $AE35          
C81B: 60        RTS                
C81C: 44        ???  ; illegal $44 
C81D: 34        ???  ; illegal $34 
C81E: AA        TAX                
C81F: 00        BRK                
C820: 21 34     AND ($34,X)        
C822: A1 A6     LDA ($A6,X)        
C824: 21 34     AND ($34,X)        
C826: 14        ???  ; illegal $14 
C827: A6 00     LDX $00            
C829: 44        ???  ; illegal $44 
C82A: 1E 99 11  ASL $1199,X        
C82D: 34        ???  ; illegal $34 
C82E: 02        ???  ; illegal $02 
C82F: 50 01     BVC $C832          
C831: 85 36     STA $36            
C833: 0A        ASL A              
C834: B8        CLV                
C835: AA        TAX                
C836: B6 80     LDX $80,Y          
C838: B1 B2     LDA ($B2),Y        
C83A: 74        ???  ; illegal $74 
C83B: F2        ???  ; illegal $F2 
C83C: 00        BRK                
C83D: 00        BRK                
C83E: 44        ???  ; illegal $44 
C83F: 3F        ???  ; illegal $3F 
C840: F8        SED                
C841: 08        PHP                
C842: 01 4C     ORA ($4C,X)        
C844: 50 B9     BVC $C7FF          
C846: 14        ???  ; illegal $14 
C847: 49 D4     EOR #$D4           
C849: 05 4D     ORA $4D            
C84B: 4C EF A3  JMP $A3EF          
C84E: A5 33     LDA $33            
C850: 03        ???  ; illegal $03 
C851: 7D 00 60  ADC $6000,X        
C854: 74        ???  ; illegal $74 
C855: 7F        ???  ; illegal $7F 
C856: 8C 0E 34  STY $340E          
C859: 5A        ???  ; illegal $5A 
C85A: B2        ???  ; illegal $B2 
C85B: 00        BRK                
C85C: 44        ???  ; illegal $44 
C85D: 85 AC     STA $AC            
C85F: B4 B0     LDY $B0,X          
C861: F0 AC     BEQ $C80F          
C863: 08        PHP                
C864: 96 2B     STX $2B,Y          
C866: FB        ???  ; illegal $FB 
C867: B5 8E     LDA $8E,X          
C869: 04        ???  ; illegal $04 
C86A: F7        ???  ; illegal $F7 
C86B: 0E 40 4C  ASL $4C40          
C86E: F2        ???  ; illegal $F2 
C86F: 00        BRK                
C870: B4 51     LDY $51,X          
C872: E6 09     INC $09            
C874: 2A        ROL A              
C875: 80        ???  ; illegal $80 
C876: 42        ???  ; illegal $42 
C877: F7        ???  ; illegal $F7 
C878: 03        ???  ; illegal $03 
C879: C5 33     CMP $33            
C87B: 03        ???  ; illegal $03 
C87C: A1 E8     LDA ($E8,X)        
C87E: B4 3F     LDY $3F,X          
C880: BD E4 B4  LDA $B4E4,X        
C883: F3        ???  ; illegal $F3 
C884: 0B        ???  ; illegal $0B 
C885: 00        BRK                
C886: 45 0C     EOR $0C            
C888: 0E A8 D4  ASL $D4A8          
C88B: 74        ???  ; illegal $74 
C88C: 31 BA     AND ($BA),Y        
C88E: 00        BRK                
C88F: 84 6A     STY $6A            
C891: BF        ???  ; illegal $BF 
C892: 00        BRK                
C893: 44        ???  ; illegal $44 
C894: 81 88     STA ($88,X)        
C896: 00        BRK                
C897: 80        ???  ; illegal $80 
C898: 74        ???  ; illegal $74 
C899: 7F        ???  ; illegal $7F 
C89A: 8A        TXA                
C89B: 00        BRK                
C89C: 84 1E     STY $1E            
C89E: F9 20 A3  SBC $A320,Y        
C8A1: A3        ???  ; illegal $A3 
C8A2: 06 14     ASL $14            
C8A4: 73        ???  ; illegal $73 
C8A5: 0B        ???  ; illegal $0B 
C8A6: 02        ???  ; illegal $02 
C8A7: 00        BRK                
C8A8: 83        ???  ; illegal $83 
C8A9: 4B        ???  ; illegal $4B 
C8AA: B3        ???  ; illegal $B3 
C8AB: 00        BRK                
C8AC: 21 A4     AND ($A4,X)        
C8AE: A4 01     LDY $01            
C8B0: 86 3B     STX $3B            
C8B2: A3        ???  ; illegal $A3 
C8B3: 00        BRK                
C8B4: 11 83     ORA ($83),Y        
C8B6: 1B        ???  ; illegal $1B 
C8B7: 02        ???  ; illegal $02 
C8B8: 00        BRK                
C8B9: 00        BRK                
C8BA: 00        BRK                
C8BB: 00        BRK                
C8BC: 06 60     ASL $60            
C8BE: 44        ???  ; illegal $44 
C8BF: 67        ???  ; illegal $67 
C8C0: F5 C4     SBC $C4,X          
C8C2: 4C 62 8E  JMP $8E62          
C8C5: 01 8C     ORA ($8C,X)        
C8C7: C0 89     CPY #$89           
C8C9: 00        BRK                
C8CA: 8F        ???  ; illegal $8F 
C8CB: 34        ???  ; illegal $34 
C8CC: AC A8 94  LDY $94A8          
C8CF: 74        ???  ; illegal $74 
C8D0: 30 8A     BMI $C85C          
C8D2: 00        BRK                
C8D3: 84 39     STY $39            
C8D5: 8F        ???  ; illegal $8F 
C8D6: 80        ???  ; illegal $80 
C8D7: 44        ???  ; illegal $44 
C8D8: 67        ???  ; illegal $67 
C8D9: 98        TYA                
C8DA: 00        BRK                
C8DB: 60        RTS                
C8DC: 74        ???  ; illegal $74 
C8DD: 7F        ???  ; illegal $7F 
C8DE: 91 00     STA ($00),Y        
C8E0: 44        ???  ; illegal $44 
C8E1: A5 92     LDA $92            
C8E3: 01 8C     ORA ($8C,X)        
C8E5: 60        RTS                
C8E6: 93        ???  ; illegal $93 
C8E7: 00        BRK                
C8E8: 84 1E     STY $1E            
C8EA: 81 00     STA ($00,X)        
C8EC: 00        BRK                
C8ED: 84 1E     STY $1E            
C8EF: F5 00     SBC $00,X          
C8F1: 44        ???  ; illegal $44 
C8F2: A4 A2     LDY $A2            
C8F4: B6 B4     LDX $B4,Y          
C8F6: 1E 3C 8D  ASL $8D3C,X        
C8F9: 04        ???  ; illegal $04 
C8FA: 07        ???  ; illegal $07 
C8FB: 8D 00 01  STA $0100          
C8FE: 0C        ???  ; illegal $0C 
C8FF: 04        ???  ; illegal $04 
C900: 77        ???  ; illegal $77 
C901: B8        CLV                
C902: 74        ???  ; illegal $74 
C903: 7F        ???  ; illegal $7F 
C904: 8E 41 0C  STX $0C41          
C907: 04        ???  ; illegal $04 
C908: 77        ???  ; illegal $77 
C909: B8        CLV                
C90A: 74        ???  ; illegal $74 
C90B: 7F        ???  ; illegal $7F 
C90C: 8E 00 60  STX $6000          
C90F: 44        ???  ; illegal $44 
C910: 63        ???  ; illegal $63 
C911: 77        ???  ; illegal $77 
C912: 18        CLC                
C913: 4C 7F 96  JMP $967F          
C916: 41 0C     EOR ($0C,X)        
C918: 04        ???  ; illegal $04 
C919: 77        ???  ; illegal $77 
C91A: B8        CLV                
C91B: 74        ???  ; illegal $74 
C91C: 7F        ???  ; illegal $7F 
C91D: 8E 00 01  STX $0100          
C920: 0C        ???  ; illegal $0C 
C921: 0C        ???  ; illegal $0C 
C922: 47        ???  ; illegal $47 
C923: 12        ???  ; illegal $12 
C924: 4C 85 B1  JMP $B185          
C927: 01 8C     ORA ($8C,X)        
C929: 80        ???  ; illegal $80 
C92A: F5 12     SBC $12,X          
C92C: 4C 3F F5  JMP $F53F          
C92F: 20 01 8C  JSR $8C01          
C932: 0F        ???  ; illegal $0F 
C933: F7        ???  ; illegal $F7 
C934: A0 44     LDY #$44           
C936: 67        ???  ; illegal $67 
C937: BB        ???  ; illegal $BB 
C938: 94 4A     STY $4A,X          
C93A: F2        ???  ; illegal $F2 
C93B: 07        ???  ; illegal $07 
C93C: 81 B4     STA ($B4,X)        
C93E: 69 B8     ADC #$B8           
C940: 00        BRK                
C941: 80        ???  ; illegal $80 
C942: 42        ???  ; illegal $42 
C943: FF        ???  ; illegal $FF 
C944: A7        ???  ; illegal $A7 
C945: F1 E6     SBC ($E6),Y        
C947: AA        TAX                
C948: AA        TAX                
C949: A5 31     LDA $31            
C94B: 02        ???  ; illegal $02 
C94C: 70 01     BVS $C94F          
C94E: 4C 50 AD  JMP $AD50          
C951: 20 01 4C  JSR $4C01          
C954: 53        ???  ; illegal $53 
C955: BD 80 42  LDA $4280,X        
C958: E2        ???  ; illegal $E2 
C959: 0C        ???  ; illegal $0C 
C95A: B6 30     LDX $30,Y          
C95C: D7        ???  ; illegal $D7 
C95D: 0F        ???  ; illegal $0F 
C95E: 01 8C     ORA ($8C,X)        
C960: 08        PHP                
C961: B9 A2 A0  LDA $A0A2,Y        
C964: 74        ???  ; illegal $74 
C965: 7F        ???  ; illegal $7F 
C966: AF        ???  ; illegal $AF 
C967: 61 B4     ADC ($B4,X)        
C969: 01 B0     ORA ($B0,X)        
C96B: 00        BRK                
C96C: 44        ???  ; illegal $44 
C96D: 3F        ???  ; illegal $3F 
C96E: FC        ???  ; illegal $FC 
C96F: 40        RTI                
C970: 4C F2 00  JMP $00F2          
C973: 80        ???  ; illegal $80 
C974: 92        ???  ; illegal $92 
C975: 74        ???  ; illegal $74 
C976: D9 AB 01  CMP $01AB,Y        
C979: 8C 03 AE  STY $AE03          
C97C: 20 B4 32  JSR $32B4          
C97F: 47        ???  ; illegal $47 
C980: 60        RTS                
C981: 5C        ???  ; illegal $5C 
C982: 67        ???  ; illegal $67 
C983: B6 02     LDX $02,Y          
C985: 8F        ???  ; illegal $8F 
C986: 34        ???  ; illegal $34 
C987: DA        ???  ; illegal $DA 
C988: BC 00 84  LDY $8400,X        
C98B: 6A        ROR A              
C98C: BA        TSX                
C98D: 01 8C     ORA ($8C,X)        
C98F: 04        ???  ; illegal $04 
C990: 94 41     STY $41,X          
C992: 34        ???  ; illegal $34 
C993: 10 47     BPL $C9DC          
C995: 00        BRK                
C996: 45 0C     EOR $0C            
C998: 1B        ???  ; illegal $1B 
C999: A6 99     LDX $99            
C99B: F2        ???  ; illegal $F2 
C99C: 7F        ???  ; illegal $7F 
C99D: BE 45 0C  LDX $0C45,Y        
C9A0: 03        ???  ; illegal $03 
C9A1: BF        ???  ; illegal $BF 
C9A2: 15 0A     ORA $0A,X          
C9A4: 01 8C     ORA ($8C,X)        
C9A6: 80        ???  ; illegal $80 
C9A7: A0 74     LDY #$74           
C9A9: 7F        ???  ; illegal $7F 
C9AA: 9E        ???  ; illegal $9E 
C9AB: CF        ???  ; illegal $CF 
C9AC: 8C 20 8F  STY $8F20          
C9AF: A5 31     LDA $31            
C9B1: 02        ???  ; illegal $02 
C9B2: 58        CLI                
C9B3: 01 8C     ORA ($8C,X)        
C9B5: 10 A1     BPL $C958          
C9B7: 20 00 44  JSR $4400          
C9BA: 3F        ???  ; illegal $3F 
C9BB: 97        ???  ; illegal $97 
C9BC: A0 44     LDY #$44           
C9BE: 67        ???  ; illegal $67 
C9BF: F5 12     SBC $12,X          
C9C1: 4C A0 8D  JMP $8DA0          
C9C4: B4 74     LDY $74,X          
C9C6: 3F        ???  ; illegal $3F 
C9C7: 47        ???  ; illegal $47 
C9C8: 00        BRK                
C9C9: 80        ???  ; illegal $80 
C9CA: 42        ???  ; illegal $42 
C9CB: E3        ???  ; illegal $E3 
C9CC: 0C        ???  ; illegal $0C 
C9CD: 0F        ???  ; illegal $0F 
C9CE: 0E 08 FA  ASL $FA08          
C9D1: 01 8C     ORA ($8C,X)        
C9D3: 01 F7     ORA ($F7,X)        
C9D5: 17        ???  ; illegal $17 
C9D6: 30 00     BMI $C9D8          
C9D8: AB        ???  ; illegal $AB 
C9D9: 00        BRK                
C9DA: 47        ???  ; illegal $47 
C9DB: 0C        ???  ; illegal $0C 
C9DC: 05 B9     ORA $B9            
C9DE: 14        ???  ; illegal $14 
C9DF: B4 1E     LDY $1E,X          
C9E1: F5 01     SBC $01,X          
C9E3: 8C 10 89  STY $8910          
C9E6: B4 30     LDY $30,X          
C9E8: DA        ???  ; illegal $DA 
C9E9: 0B        ???  ; illegal $0B 
C9EA: 8A        TXA                
C9EB: C0 74     CPY #$74           
C9ED: 7F        ???  ; illegal $7F 
C9EE: 91 0F     STA ($0F),Y        
C9F0: 8E 40 FA  STX $FA40          
C9F3: 81 34     STA ($34,X)        
C9F5: 15 F6     ORA $F6,X          
C9F7: 80        ???  ; illegal $80 
C9F8: 44        ???  ; illegal $44 
C9F9: 35 9A     AND $9A,X          
C9FB: 88        DEY                
C9FC: 34        ???  ; illegal $34 
C9FD: 88        DEY                
C9FE: F3        ???  ; illegal $F3 
C9FF: 0A        ASL A              
CA00: 01 8C     ORA ($8C,X)        
CA02: 02        ???  ; illegal $02 
CA03: F5 80     SBC $80,X          
CA05: 4C A4 F5  JMP $F5A4          
CA08: 00        BRK                
CA09: 44        ???  ; illegal $44 
CA0A: 3F        ???  ; illegal $3F 
CA0B: F8        SED                
CA0C: EB        ???  ; illegal $EB 
CA0D: 81 34     STA ($34,X)        
CA0F: 03        ???  ; illegal $03 
CA10: F6 81     INC $81,X          
CA12: 8C 02 15  STY $1502          
CA15: D2        ???  ; illegal $D2 
CA16: 4C 67 F5  JMP $F567          
CA19: 32        ???  ; illegal $32 
CA1A: 74        ???  ; illegal $74 
CA1B: 3F        ???  ; illegal $3F 
CA1C: F8        SED                
CA1D: 2A        ROL A              
CA1E: 00        BRK                
CA1F: 44        ???  ; illegal $44 
CA20: 3F        ???  ; illegal $3F 
CA21: 92        ???  ; illegal $92 
CA22: D0 84     BNE $C9A8          
CA24: 34        ???  ; illegal $34 
CA25: 82        ???  ; illegal $82 
CA26: 80        ???  ; illegal $80 
CA27: B4 A1     LDY $A1,X          
CA29: 95 C0     STA $C0,X          
CA2B: 44        ???  ; illegal $44 
CA2C: 67        ???  ; illegal $67 
CA2D: 18        CLC                
CA2E: 02        ???  ; illegal $02 
CA2F: 45 0C     EOR $0C            
CA31: 09 B1     ORA #$B1           
CA33: 0F        ???  ; illegal $0F 
CA34: 0E 0A FA  ASL $FA0A          
CA37: 01 8C     ORA ($8C,X)        
CA39: 0E F7 60  ASL $60F7          
CA3C: 4C A0 F5  JMP $F5A0          
CA3F: 48        PHA                
CA40: 6A        ROR A              
CA41: B6 3F     LDX $3F,Y          
CA43: 82        ???  ; illegal $82 
CA44: 20 44 54  JSR $5444          
CA47: 47        ???  ; illegal $47 
CA48: E0 4C     CPX #$4C           
CA4A: 3F        ???  ; illegal $3F 
CA4B: 90 00     BCC $CA4D          
CA4D: 44        ???  ; illegal $44 
CA4E: A4 81     LDY $81            
CA50: 00        BRK                
CA51: 01 C2     ORA ($C2,X)        
CA53: 7F        ???  ; illegal $7F 
CA54: AC A5 0C  LDY $0CA5          
CA57: 04        ???  ; illegal $04 
CA58: AA        TAX                
CA59: 01 02     ORA ($02,X)        
CA5B: 01 86     ORA ($86,X)        
CA5D: 16 74     ASL $74,X          
CA5F: D7        ???  ; illegal $D7 
CA60: 08        PHP                
CA61: 2A        ROL A              
CA62: C5 3E     CMP $3E            
CA64: 10 BD     BPL $CA23          
CA66: 8F        ???  ; illegal $8F 
CA67: B4 04     LDY $04,X          
CA69: 78        SEI                
CA6A: 80        ???  ; illegal $80 
CA6B: 74        ???  ; illegal $74 
CA6C: 1A        ???  ; illegal $1A 
CA6D: AF        ???  ; illegal $AF 
CA6E: 81 B4     STA ($B4,X)        
CA70: 0C        ???  ; illegal $0C 
CA71: 75 04     ADC $04,X          
CA73: 29 F7     AND #$F7           
CA75: 08        PHP                
CA76: B8        CLV                
CA77: 89        ???  ; illegal $89 
CA78: 36 02     ROL $02,X          
CA7A: 88        DEY                
CA7B: 00        BRK                
CA7C: 44        ???  ; illegal $44 
CA7D: CF        ???  ; illegal $CF 
CA7E: 04        ???  ; illegal $04 
CA7F: 16 4E     ASL $4E,X          
CA81: 30 B2     BMI $CA35          
CA83: 2A        ROL A              
CA84: 60        RTS                
CA85: B4 3F     LDY $3F,X          
CA87: 47        ???  ; illegal $47 
CA88: 92        ???  ; illegal $92 
CA89: 4C 1E B4  JMP $B41E          
CA8C: 80        ???  ; illegal $80 
CA8D: 44        ???  ; illegal $44 
CA8E: 1E AE 45  ASL $45AE,X        
CA91: 0F        ???  ; illegal $0F 
CA92: F5 B6     SBC $B6,X          
CA94: 00        BRK                
CA95: 20 44 53  JSR $5344          
CA98: BC 01 02  LDY $0201,X        
CA9B: 04        ???  ; illegal $04 
CA9C: A0 08     LDY #$08           
CA9E: 86 E7     STX $E7            
CAA0: 07        ???  ; illegal $07 
CAA1: 45 0C     EOR $0C            
CAA3: 03        ???  ; illegal $03 
CAA4: A8        TAY                
CAA5: 00        BRK                
CAA6: 25 F7     AND $F7            
CAA8: 88        DEY                
CAA9: A5 20     LDA $20            
CAAB: 42        ???  ; illegal $42 
CAAC: FF        ???  ; illegal $FF 
CAAD: A0 08     LDY #$08           
CAAF: 86 E7     STX $E7            
CAB1: 07        ???  ; illegal $07 
CAB2: C2        ???  ; illegal $C2 
CAB3: 41 E7     EOR ($E7,X)        
CAB5: 07        ???  ; illegal $07 
CAB6: F8        SED                
CAB7: A1 34     LDA ($34,X)        
CAB9: 03        ???  ; illegal $03 
CABA: BC 16 74  LDY $7416,X        
CABD: 3F        ???  ; illegal $3F 
CABE: F6 80     INC $80,X          
CAC0: 44        ???  ; illegal $44 
CAC1: A2 1D     LDX #$1D           
CAC3: 00        BRK                
CAC4: 44        ???  ; illegal $44 
CAC5: 3F        ???  ; illegal $3F 
CAC6: F6 82     INC $82,X          
CAC8: 81 34     STA ($34,X)        
CACA: 10 3D     BPL $CB09          
CACC: 00        BRK                
CACD: 84 1E     STY $1E            
CACF: A6 00     LDX $00            
CAD1: 44        ???  ; illegal $44 
CAD2: 3F        ???  ; illegal $3F 
CAD3: 90 00     BCC $CAD5          
CAD5: 4C 1A B8  JMP $B81A          
CAD8: AA        TAX                
CAD9: 14        ???  ; illegal $14 
CADA: 88        DEY                
CADB: DA        ???  ; illegal $DA 
CADC: B8        CLV                
CADD: 32        ???  ; illegal $32 
CADE: 74        ???  ; illegal $74 
CADF: CF        ???  ; illegal $CF 
CAE0: 00        BRK                
CAE1: 8E 84 D9  STX $D984          
CAE4: AA        TAX                
CAE5: 08        PHP                
CAE6: 4E D8 99  LSR $99D8          
CAE9: 0A        ASL A              
CAEA: 00        BRK                
CAEB: 44        ???  ; illegal $44 
CAEC: DB        ???  ; illegal $DB 
CAED: 07        ???  ; illegal $07 
CAEE: 01 0C     ORA ($0C,X)        
CAF0: 81 B2     STA ($B2,X)        
CAF2: 81 34     STA ($34,X)        
CAF4: 04        ???  ; illegal $04 
CAF5: AB        ???  ; illegal $AB 
CAF6: 81 34     STA ($34,X)        
CAF8: 04        ???  ; illegal $04 
CAF9: 81 AA     STA ($AA,X)        
CAFB: 21 B4     AND ($B4,X)        
CAFD: 02        ???  ; illegal $02 
CAFE: A8        TAY                
CAFF: 32        ???  ; illegal $32 
CB00: 74        ???  ; illegal $74 
CB01: A2 94     LDX #$94           
CB03: 2F        ???  ; illegal $2F 
CB04: 34        ???  ; illegal $34 
CB05: 02        ???  ; illegal $02 
CB06: 93        ???  ; illegal $93 
CB07: 94 72     STY $72,X          
CB09: E5 B4     SBC $B4            
CB0B: 2A        ROL A              
CB0C: 00        BRK                
CB0D: 4C 1A B6  JMP $B61A          
CB10: 16 74     ASL $74,X          
CB12: D7        ???  ; illegal $D7 
CB13: 04        ???  ; illegal $04 
CB14: 2E BC F2  ROL $F2BC          
CB17: 0C        ???  ; illegal $0C 
CB18: 80        ???  ; illegal $80 
CB19: 4C F1 00  JMP $00F1          
CB1C: 01 0D     ORA ($0D,X)        
CB1E: 4E FC 96  LSR $96FC          
CB21: 20 96 53  JSR $5396          
CB24: A1 00     LDA ($00,X)        
CB26: 44        ???  ; illegal $44 
CB27: A1 58     LDA ($58,X)        
CB29: 00        BRK                
CB2A: 44        ???  ; illegal $44 
CB2B: D7        ???  ; illegal $D7 
CB2C: 04        ???  ; illegal $04 
CB2D: B2        ???  ; illegal $B2 
CB2E: 21 34     AND ($34,X)        
CB30: 40        RTI                
CB31: B5 14     LDA $14,X          
CB33: 71 F0     ADC ($F0),Y        
CB35: 94 12     STY $12,X          
CB37: 44        ???  ; illegal $44 
CB38: 30 A2     BMI $CADC          
CB3A: B6 74     LDX $74,Y          
CB3C: 30 9C     BMI $CADA          
CB3E: 8A        TXA                
CB3F: 81 34     STA ($34,X)        
CB41: 1F        ???  ; illegal $1F 
CB42: 83        ???  ; illegal $83 
CB43: 16 4E     ASL $4E,X          
CB45: 15 AD     ORA $AD,X          
CB47: 21 34     AND ($34,X)        
CB49: 40        RTI                
CB4A: 9A        TXS                
CB4B: 20 44 31  JSR $3144          
CB4E: 9A        TXS                
CB4F: A4 80     LDY $80            
CB51: 4C F3 0C  JMP $0CF3          
CB54: 8E 84 3F  STX $3F84          
CB57: 47        ???  ; illegal $47 
CB58: 00        BRK                
CB59: 44        ???  ; illegal $44 
CB5A: A2 A9     LDX #$A9           
CB5C: 01 8C     ORA ($8C,X)        
CB5E: 20 85 A3  JSR $A385          
CB61: 92        ???  ; illegal $92 
CB62: 7C        ???  ; illegal $7C 
CB63: 3F        ???  ; illegal $3F 
CB64: B7        ???  ; illegal $B7 
CB65: 32        ???  ; illegal $32 
CB66: 74        ???  ; illegal $74 
CB67: D9 FB B4  CMP $B4FB,Y        
CB6A: 88        DEY                
CB6B: F0 92     BEQ $CAFF          
CB6D: 14        ???  ; illegal $14 
CB6E: 71 FF     ADC ($FF),Y        
CB70: F6 82     INC $82,X          
CB72: 80        ???  ; illegal $80 
CB73: 44        ???  ; illegal $44 
CB74: 35 90     AND $90,X          
CB76: E0 44     CPX #$44           
CB78: 39 B1 00  AND $00B1,Y        
CB7B: 04        ???  ; illegal $04 
CB7C: 1E BA 14  ASL $14BA,X        
CB7F: 31 DE     AND ($DE),Y        
CB81: B9 02 00  LDA $0002,Y        
CB84: 00        BRK                
CB85: 00        BRK                
CB86: 00        BRK                
CB87: 54        ???  ; illegal $54 
CB88: 76 54     ROR $54,X          
CB8A: 8C 00 44  STY $4400          
CB8D: A2 B3     LDX #$B3           
CB8F: 14        ???  ; illegal $14 
CB90: B1 DE     LDA ($DE),Y        
CB92: AF        ???  ; illegal $AF 
CB93: 28        PLP                
CB94: E1 37     SBC ($37,X)        
CB96: 02        ???  ; illegal $02 
CB97: A0 A1     LDY #$A1           
CB99: F4        ???  ; illegal $F4 
CB9A: 7F        ???  ; illegal $7F 
CB9B: 9D 10 06  STA $0610,X        
CB9E: 34        ???  ; illegal $34 
CB9F: 9E        ???  ; illegal $9E 
CBA0: F2        ???  ; illegal $F2 
CBA1: 74        ???  ; illegal $74 
CBA2: D9 AA DA  CMP $DAAA,Y        
CBA5: 14        ???  ; illegal $14 
CBA6: 74        ???  ; illegal $74 
CBA7: 30 88     BMI $CB31          
CBA9: 00        BRK                
CBAA: 44        ???  ; illegal $44 
CBAB: 31 AE     AND ($AE),Y        
CBAD: 32        ???  ; illegal $32 
CBAE: 74        ???  ; illegal $74 
CBAF: A0 BB     LDY #$BB           
CBB1: 81 34     STA ($34,X)        
CBB3: 1E BD 81  ASL $81BD,X        
CBB6: 80        ???  ; illegal $80 
CBB7: 74        ???  ; illegal $74 
CBB8: 1A        ???  ; illegal $1A 
CBB9: 50 E1     BVC $CB9C          
CBBB: 37        ???  ; illegal $37 
CBBC: 02        ???  ; illegal $02 
CBBD: BE B4 74  LDX $74B4,Y        
CBC0: D9 BF B4  CMP $B4BF,Y        
CBC3: 74        ???  ; illegal $74 
CBC4: 3F        ???  ; illegal $3F 
CBC5: 83        ???  ; illegal $83 
CBC6: 80        ???  ; illegal $80 
CBC7: C0 74     CPY #$74           
CBC9: 7F        ???  ; illegal $7F 
CBCA: 8F        ???  ; illegal $8F 
CBCB: 12        ???  ; illegal $12 
CBCC: 4C E3 00  JMP $00E3          
CBCF: 00        BRK                
CBD0: 44        ???  ; illegal $44 
CBD1: A3        ???  ; illegal $A3 
CBD2: 83        ???  ; illegal $83 
CBD3: 01 0C     ORA ($0C,X)        
CBD5: 01 F5     ORA ($F5,X)        
CBD7: 0A        ASL A              
CBD8: 01 0C     ORA ($0C,X)        
CBDA: 09 F5     ORA #$F5           
CBDC: 01 0C     ORA ($0C,X)        
CBDE: FF        ???  ; illegal $FF 
CBDF: 89        ???  ; illegal $89 
CBE0: B4 74     LDY $74,X          
CBE2: 1E 9E 8E  ASL $8E9E,X        
CBE5: BC 3F 47  LDY $473F,X        
CBE8: 80        ???  ; illegal $80 
CBE9: 94 74     STY $74,X          
CBEB: D9 08 12  CMP $1208,Y        
CBEE: 8C 1E F5  STY $F51E          
CBF1: 01 0C     ORA ($0C,X)        
CBF3: 44        ???  ; illegal $44 
CBF4: F5 01     SBC $01,X          
CBF6: 0C        ???  ; illegal $0C 
CBF7: 84 F5     STY $F5            
CBF9: 00        BRK                
CBFA: 01 0C     ORA ($0C,X)        
CBFC: 04        ???  ; illegal $04 
CBFD: 91 00     STA ($00),Y        
CBFF: 84 1E     STY $1E            
CC01: F5 01     SBC $01,X          
CC03: 8C 0F 9D  STY $9D0F          
CC06: 81 34     STA ($34,X)        
CC08: 1E 99 80  ASL $8099,X        
CC0B: A0 B4     LDY #$B4           
CC0D: A1 A1     LDA ($A1,X)        
CC0F: 12        ???  ; illegal $12 
CC10: 8C 1E F5  STY $F51E          
CC13: 01 0C     ORA ($0C,X)        
CC15: 1B        ???  ; illegal $1B 
CC16: F5 01     SBC $01,X          
CC18: 0C        ???  ; illegal $0C 
CC19: 06 F5     ASL $F5            
CC1B: 03        ???  ; illegal $03 
CC1C: 01 0C     ORA ($0C,X)        
CC1E: 09 F5     ORA #$F5           
CC20: 01 0C     ORA ($0C,X)        
CC22: 02        ???  ; illegal $02 
CC23: F5 16     SBC $16,X          
CC25: 8E 1E 9B  STX $9B1E          
CC28: B6 74     LDX $74,Y          
CC2A: EF        ???  ; illegal $EF 
CC2B: 0C        ???  ; illegal $0C 
CC2C: 28        PLP                
CC2D: 40        RTI                
CC2E: 42        ???  ; illegal $42 
CC2F: DB        ???  ; illegal $DB 
CC30: 04        ???  ; illegal $04 
CC31: 81 34     STA ($34,X)        
CC33: 03        ???  ; illegal $03 
CC34: 9A        TXS                
CC35: 4F        ???  ; illegal $4F 
CC36: B4 08     LDY $08,X          
CC38: 81 12     STA ($12,X)        
CC3A: 8C 1E 8D  STY $8D1E          
CC3D: 28        PLP                
CC3E: 00        BRK                
CC3F: 42        ???  ; illegal $42 
CC40: E2        ???  ; illegal $E2 
CC41: 09 C0     ORA #$C0           
CC43: 44        ???  ; illegal $44 
CC44: 67        ???  ; illegal $67 
CC45: F7        ???  ; illegal $F7 
CC46: 8E B4 BC  STX $BCB4          
CC49: 9F        ???  ; illegal $9F 
CC4A: 00        BRK                
CC4B: 4C 1A 19  JMP $191A          
CC4E: A8        TAY                
CC4F: 80        ???  ; illegal $80 
CC50: 74        ???  ; illegal $74 
CC51: 7F        ???  ; illegal $7F 
CC52: A9 81     LDA #$81           
CC54: 3C        ???  ; illegal $3C 
CC55: 01 86     ORA ($86,X)        
CC57: 8E 8C 3F  STX $3F8C          
CC5A: A0 0F     LDY #$0F           
CC5C: 0E 20 A0  ASL $A020          
CC5F: 20 01 8C  JSR $8C01          
CC62: 0E F7 60  ASL $60F7          
CC65: 4C A0 F5  JMP $F5A0          
CC68: 01 8C     ORA ($8C,X)        
CC6A: 04        ???  ; illegal $04 
CC6B: AF        ???  ; illegal $AF 
CC6C: 80        ???  ; illegal $80 
CC6D: 42        ???  ; illegal $42 
CC6E: E8        INX                
CC6F: 0C        ???  ; illegal $0C 
CC70: 00        BRK                
CC71: B4 51     LDY $51,X          
CC73: EC 0B 00  CPX $000B          
CC76: 44        ???  ; illegal $44 
CC77: A4 F5     LDY $F5            
CC79: 0F        ???  ; illegal $0F 
CC7A: 8E 80 F5  STX $F580          
CC7D: 0D CE 0F  ORA $0FCE          
CC80: AE 28 A0  LDX $A028          
CC83: 80        ???  ; illegal $80 
CC84: E9 0F     SBC #$0F           
CC86: 89        ???  ; illegal $89 
CC87: 37        ???  ; illegal $37 
CC88: 06 A2     ASL $A2            
CC8A: 80        ???  ; illegal $80 
CC8B: 44        ???  ; illegal $44 
CC8C: 67        ???  ; illegal $67 
CC8D: F5 6A     SBC $6A,X          
CC8F: 74        ???  ; illegal $74 
CC90: 67        ???  ; illegal $67 
CC91: F7        ???  ; illegal $F7 
CC92: A0 A0     LDY #$A0           
CC94: 74        ???  ; illegal $74 
CC95: 7F        ???  ; illegal $7F 
CC96: B1 C1     LDA ($C1),Y        
CC98: 74        ???  ; illegal $74 
CC99: C0 B3     CPY #$B3           
CC9B: 00        BRK                
CC9C: 44        ???  ; illegal $44 
CC9D: 3F        ???  ; illegal $3F 
CC9E: 47        ???  ; illegal $47 
CC9F: 01 8C     ORA ($8C,X)        
CCA1: 02        ???  ; illegal $02 
CCA2: F7        ???  ; illegal $F7 
CCA3: 0C        ???  ; illegal $0C 
CCA4: 00        BRK                
CCA5: 44        ???  ; illegal $44 
CCA6: 3F        ???  ; illegal $3F 
CCA7: F8        SED                
CCA8: D0 84     BNE $CC2E          
CCAA: A0 B9     LDY #$B9           
CCAC: D4        ???  ; illegal $D4 
CCAD: 74        ???  ; illegal $74 
CCAE: 34        ???  ; illegal $34 
CCAF: B2        ???  ; illegal $B2 
CCB0: 00        BRK                
CCB1: 44        ???  ; illegal $44 
CCB2: 30 47     BMI $CCFB          
CCB4: A2 80     LDX #$80           
CCB6: 7C        ???  ; illegal $7C 
CCB7: 1A        ???  ; illegal $1A 
CCB8: 47        ???  ; illegal $47 
CCB9: B2        ???  ; illegal $B2 
CCBA: 44        ???  ; illegal $44 
CCBB: 67        ???  ; illegal $67 
CCBC: 78        SEI                
CCBD: 00        BRK                
CCBE: 43        ???  ; illegal $43 
CCBF: D7        ???  ; illegal $D7 
CCC0: 0D C0 59  ORA $59C0          
CCC3: FF        ???  ; illegal $FF 
CCC4: BD 2A 00  LDA $002A,X        
CCC7: 44        ???  ; illegal $44 
CCC8: 3F        ???  ; illegal $3F 
CCC9: 97        ???  ; illegal $97 
CCCA: C0 A0     CPY #$A0           
CCCC: E9 0D     SBC #$0D           
CCCE: 94 49     STY $49,X          
CCD0: FF        ???  ; illegal $FF 
CCD1: BF        ???  ; illegal $BF 
CCD2: 96 6B     STX $6B,Y          
CCD4: FF        ???  ; illegal $FF 
CCD5: 88        DEY                
CCD6: 00        BRK                
CCD7: 00        BRK                
CCD8: 44        ???  ; illegal $44 
CCD9: 3F        ???  ; illegal $3F 
CCDA: FF        ???  ; illegal $FF 
CCDB: 09 0F     ORA #$0F           
CCDD: 01 82     ORA ($82,X)        
CCDF: 00        BRK                
CCE0: 84 5E     STY $5E            
CCE2: 8B        ???  ; illegal $8B 
CCE3: 10 B6     BPL $CC9B          
CCE5: 62        ???  ; illegal $62 
CCE6: 81 00     STA ($00,X)        
CCE8: 00        BRK                
CCE9: 44        ???  ; illegal $44 
CCEA: 81 B7     STA ($B7,X)        
CCEC: 6A        ROR A              
CCED: 84 30     STY $30            
CCEF: 92        ???  ; illegal $92 
CCF0: 00        BRK                
CCF1: 44        ???  ; illegal $44 
CCF2: 3F        ???  ; illegal $3F 
CCF3: 86 30     STX $30            
CCF5: B6 56     LDX $56,Y          
CCF7: 8D 00 45  STA $4500          
CCFA: 0C        ???  ; illegal $0C 
CCFB: 20 38 11  JSR $1138          
CCFE: 0E 44 90  ASL $9044          
CD01: 00        BRK                
CD02: 44        ???  ; illegal $44 
CD03: B2        ???  ; illegal $B2 
CD04: A0 01     LDY #$01           
CD06: 0C        ???  ; illegal $0C 
CD07: 41 18     EOR ($18,X)        
CD09: 02        ???  ; illegal $02 
CD0A: 74        ???  ; illegal $74 
CD0B: 4C 3E 92  JMP $923E          
CD0E: 01 34     ORA ($34,X)        
CD10: 15 8E     ORA $8E,X          
CD12: 2D 0F 39  AND $390F          
CD15: 89        ???  ; illegal $89 
CD16: D5 3A     CMP $3A,X          
CD18: 00        BRK                
CD19: 9D 08 20  STA $2008,X        
CD1C: 84 3F     STY $3F            
CD1E: 7F        ???  ; illegal $7F 
CD1F: 34        ???  ; illegal $34 
CD20: 74        ???  ; illegal $74 
CD21: 38        SEC                
CD22: 8A        TXA                
CD23: C0 01     CPY #$01           
CD25: F1 84     SBC ($84),Y        
CD27: 00        BRK                
CD28: 44        ???  ; illegal $44 
CD29: 81 9C     STA ($9C,X)        
CD2B: 00        BRK                
CD2C: 01 02     ORA ($02,X)        
CD2E: 00        BRK                
CD2F: BB        ???  ; illegal $BB 
CD30: 01 4C     ORA ($4C,X)        
CD32: 50 9B     BVC $CCCF          
CD34: AD 04 07  LDA $0704          
CD37: 7C        ???  ; illegal $7C 
CD38: 14        ???  ; illegal $14 
CD39: 4B        ???  ; illegal $4B 
CD3A: E3        ???  ; illegal $E3 
CD3B: 04        ???  ; illegal $04 
CD3C: 00        BRK                
CD3D: 40        RTI                
CD3E: 4C 8B 8F  JMP $8F8B          
CD41: 01 4C     ORA ($4C,X)        
CD43: 53        ???  ; illegal $53 
CD44: B9 9D 0F  LDA $0F9D,Y        
CD47: A4 BF     LDY $BF            
CD49: 80        ???  ; illegal $80 
CD4A: 42        ???  ; illegal $42 
CD4B: F2        ???  ; illegal $F2 
CD4C: 47        ???  ; illegal $47 
CD4D: 20 45 0C  JSR $0C45          
CD50: 03        ???  ; illegal $03 
CD51: B8        CLV                
CD52: B7        ???  ; illegal $B7 
CD53: F2        ???  ; illegal $F2 
CD54: AA        TAX                
CD55: B1 15     LDA ($15),Y        
CD57: 0A        ASL A              
CD58: 00        BRK                
CD59: 30 88     BMI $CCE3          
CD5B: B6 3F     LDX $3F,Y          
CD5D: 95 00     STA $00,X          
CD5F: 01 34     ORA ($34,X)        
CD61: 00        BRK                
CD62: 8E 01 34  STX $3401          
CD65: 0C        ???  ; illegal $0C 
CD66: 8E 01 34  STX $3401          
CD69: 8C 8E 01  STY $018E          
CD6C: 34        ???  ; illegal $34 
CD6D: 92        ???  ; illegal $92 
CD6E: 8E 00 01  STX $0100          
CD71: 34        ???  ; illegal $34 
CD72: 12        ???  ; illegal $12 
CD73: 8E 01 34  STX $3401          
CD76: 1D 8E 01  ORA $018E,X        
CD79: 34        ???  ; illegal $34 
CD7A: 17        ???  ; illegal $17 
CD7B: 8E 01 34  STX $3401          
CD7E: 13        ???  ; illegal $13 
CD7F: 8E 00 01  STX $0100          
CD82: 34        ???  ; illegal $34 
CD83: 19 8E 01  ORA $018E,Y        
CD86: 34        ???  ; illegal $34 
CD87: 80        ???  ; illegal $80 
CD88: 8E 01 34  STX $3401          
CD8B: 1E 8E 01  ASL $018E,X        
CD8E: 34        ???  ; illegal $34 
CD8F: 0B        ???  ; illegal $0B 
CD90: 8E 00 01  STX $0100          
CD93: 34        ???  ; illegal $34 
CD94: 1B        ???  ; illegal $1B 
CD95: 8E 01 34  STX $3401          
CD98: 01 8E     ORA ($8E,X)        
CD9A: 01 34     ORA ($34,X)        
CD9C: 06 8E     ASL $8E            
CD9E: 36 4C     ROL $4C,X          
CDA0: C8        INY                
CDA1: 02        ???  ; illegal $02 
CDA2: 28        PLP                
CDA3: 00        BRK                
CDA4: 00        BRK                
CDA5: 00        BRK                
CDA6: 00        BRK                
CDA7: C1 49     CMP ($49,X)        
CDA9: 50 B2     BVC $CD5D          
CDAB: A2 42     LDX #$42           
CDAD: FF        ???  ; illegal $FF 
CDAE: 8C A1 34  STY $34A1          
CDB1: 0F        ???  ; illegal $0F 
CDB2: 98        TYA                
CDB3: 00        BRK                
CDB4: 00        BRK                
CDB5: 00        BRK                
CDB6: 00        BRK                
CDB7: 00        BRK                
CDB8: 80        ???  ; illegal $80 
CDB9: 42        ???  ; illegal $42 
CDBA: FF        ???  ; illegal $FF 
CDBB: 98        TYA                
CDBC: B6 74     LDX $74,Y          
CDBE: 30 98     BMI $CD58          
CDC0: 88        DEY                
CDC1: B6 3F     LDX $3F,Y          
CDC3: 95 00     STA $00,X          
CDC5: 05 0B     ORA $0B            
CDC7: 0F        ???  ; illegal $0F 
CDC8: BC 01 42  LDY $4201,X        
CDCB: FF        ???  ; illegal $FF 
CDCC: 98        TYA                
CDCD: 92        ???  ; illegal $92 
CDCE: 74        ???  ; illegal $74 
CDCF: 30 9A     BMI $CD6B          
CDD1: 45 0C     EOR $0C            
CDD3: 05 BD     ORA $BD            
CDD5: 00        BRK                
CDD6: 00        BRK                
CDD7: 03        ???  ; illegal $03 
CDD8: FF        ???  ; illegal $FF 
CDD9: BA        TSX                
CDDA: 94 EA     STY $EA,X          
CDDC: FF        ???  ; illegal $FF 
CDDD: B5 94     LDA $94,X          
CDDF: 74        ???  ; illegal $74 
CDE0: 30 9E     BMI $CD80          
CDE2: 11 06     ORA ($06),Y        
CDE4: 40        RTI                
CDE5: 88        DEY                
CDE6: 80        ???  ; illegal $80 
CDE7: C0 74     CPY #$74           
CDE9: 7F        ???  ; illegal $7F 
CDEA: 93        ???  ; illegal $93 
CDEB: 00        BRK                
CDEC: 84 5F     STY $5F            
CDEE: 90 00     BCC $CDF0          
CDF0: C4 5D     CPY $5D            
CDF2: 94 00     STY $00,X          
CDF4: 44        ???  ; illegal $44 
CDF5: 3F        ???  ; illegal $3F 
CDF6: 83        ???  ; illegal $83 
CDF7: A8        TAY                
CDF8: 60        RTS                
CDF9: 44        ???  ; illegal $44 
CDFA: 39 BF 40  AND $40BF,Y        
CDFD: 44        ???  ; illegal $44 
CDFE: 5E 1D B4  LSR $B41D,X        
CE01: 49 FF     EOR #$FF           
CE03: 89        ???  ; illegal $89 
CE04: 00        BRK                
CE05: 44        ???  ; illegal $44 
CE06: 63        ???  ; illegal $63 
CE07: 86 A8     STX $A8            
CE09: A2 8C     LDX #$8C           
CE0B: 39 BC 74  AND $74BC,Y        
CE0E: 2B        ???  ; illegal $2B 
CE0F: FB        ???  ; illegal $FB 
CE10: 90 94     BCC $CDA6          
CE12: EB        ???  ; illegal $EB 
CE13: F9 BB 00  SBC $00BB,Y        
CE16: 44        ???  ; illegal $44 
CE17: 3F        ???  ; illegal $3F 
CE18: 8B        ???  ; illegal $8B 
CE19: AA        TAX                
CE1A: 85 0F     STA $0F            
CE1C: 8A        TXA                
CE1D: BD 8F 8C  LDA $8C8F,X        
CE20: 80        ???  ; illegal $80 
CE21: B9 40 04  LDA $0440,Y        
CE24: 67        ???  ; illegal $67 
CE25: 25 85     AND $85            
CE27: 0F        ???  ; illegal $0F 
CE28: 4C 95 22  JMP $2295          
CE2B: 00        BRK                
CE2C: 44        ???  ; illegal $44 
CE2D: 62        ???  ; illegal $62 
CE2E: 47        ???  ; illegal $47 
CE2F: C0 44     CPY #$44           
CE31: 67        ???  ; illegal $67 
CE32: 92        ???  ; illegal $92 
CE33: 00        BRK                
CE34: 44        ???  ; illegal $44 
CE35: 63        ???  ; illegal $63 
CE36: 47        ???  ; illegal $47 
CE37: B2        ???  ; illegal $B2 
CE38: 74        ???  ; illegal $74 
CE39: 5E 86 AA  LSR $AA86,X        
CE3C: A2 CC     LDX #$CC           
CE3E: 16 B0     ASL $B0,X          
CE40: 80        ???  ; illegal $80 
CE41: 44        ???  ; illegal $44 
CE42: 39 39 A6  AND $A639,Y        
CE45: 8C 46 A8  STY $A846          
CE48: A6 8C     LDX $8C            
CE4A: 46 A0     LSR $A0            
CE4C: 22        ???  ; illegal $22 
CE4D: 00        BRK                
CE4E: 84 3F     STY $3F            
CE50: 7F        ???  ; illegal $7F 
CE51: 65 0F     ADC $0F            
CE53: 2E 88 40  ROL $4088          
CE56: 84 3F     STY $3F            
CE58: 7F        ???  ; illegal $7F 
CE59: 65 0F     ADC $0F            
CE5B: 2E 88 22  ROL $2288          
CE5E: 80        ???  ; illegal $80 
CE5F: 84 3F     STY $3F            
CE61: 7F        ???  ; illegal $7F 
CE62: 65 0F     ADC $0F            
CE64: 2E 88 C0  ROL $C088          
CE67: 84 3F     STY $3F            
CE69: 7F        ???  ; illegal $7F 
CE6A: 65 0F     ADC $0F            
CE6C: 2E 88 AA  ROL $AA88          
CE6F: 00        BRK                
CE70: F4        ???  ; illegal $F4 
CE71: 81 B8     STA ($B8,X)        
CE73: 20 F4 81  JSR $81F4          
CE76: B8        CLV                
CE77: 40        RTI                
CE78: F4        ???  ; illegal $F4 
CE79: 81 B8     STA ($B8,X)        
CE7B: 60        RTS                
CE7C: F4        ???  ; illegal $F4 
CE7D: 81 B8     STA ($B8,X)        
CE7F: AA        TAX                
CE80: 80        ???  ; illegal $80 
CE81: F4        ???  ; illegal $F4 
CE82: 81 B8     STA ($B8,X)        
CE84: A0 F4     LDY #$F4           
CE86: 81 B8     STA ($B8,X)        
CE88: C0 F4     CPY #$F4           
CE8A: 81 B8     STA ($B8,X)        
CE8C: E0 F4     CPX #$F4           
CE8E: 81 B8     STA ($B8,X)        
CE90: 00        BRK                
CE91: 00        BRK                
CE92: F4        ???  ; illegal $F4 
CE93: 81 B8     STA ($B8,X)        
CE95: 20 F4 81  JSR $81F4          
CE98: B8        CLV                
CE99: 40        RTI                
CE9A: F4        ???  ; illegal $F4 
CE9B: 81 B8     STA ($B8,X)        
CE9D: 60        RTS                
CE9E: F4        ???  ; illegal $F4 
CE9F: 81 B8     STA ($B8,X)        
CEA1: 00        BRK                
CEA2: 80        ???  ; illegal $80 
CEA3: F4        ???  ; illegal $F4 
CEA4: 81 B8     STA ($B8,X)        
CEA6: A0 F4     LDY #$F4           
CEA8: 81 B8     STA ($B8,X)        
CEAA: C0 F4     CPY #$F4           
CEAC: 81 B8     STA ($B8,X)        
CEAE: E0 F4     CPX #$F4           
CEB0: 81 B8     STA ($B8,X)        
CEB2: 88        DEY                
CEB3: 80        ???  ; illegal $80 
CEB4: 44        ???  ; illegal $44 
CEB5: 5C        ???  ; illegal $5C 
CEB6: B8        CLV                
CEB7: 22        ???  ; illegal $22 
CEB8: CE 16 7F  DEC $7F16          
CEBB: 40        RTI                
CEBC: 40        RTI                
CEBD: 11 1D     ORA ($1D),Y        
CEBF: 62        ???  ; illegal $62 
CEC0: CE 16 7F  DEC $7F16          
CEC3: 88        DEY                
CEC4: 40        RTI                
CEC5: 40        RTI                
CEC6: 11 1D     ORA ($1D),Y        
CEC8: A2 CE     LDX #$CE           
CECA: 16 7F     ASL $7F,X          
CECC: 40        RTI                
CECD: 40        RTI                
CECE: 11 1D     ORA ($1D),Y        
CED0: E2        ???  ; illegal $E2 
CED1: CE 16 7F  DEC $7F16          
CED4: 0A        ASL A              
CED5: 00        BRK                
CED6: 00        BRK                
CED7: 11 85     ORA ($85),Y        
CED9: 00        BRK                
CEDA: 84 58     STY $58            
CEDC: BA        TSX                
CEDD: 89        ???  ; illegal $89 
CEDE: 2F        ???  ; illegal $2F 
CEDF: 08        PHP                
CEE0: 81 B4     STA ($B4,X)        
CEE2: 33        ???  ; illegal $33 
CEE3: FB        ???  ; illegal $FB 
CEE4: 8C 8A 80  STY $808A          
CEE7: 44        ???  ; illegal $44 
CEE8: 67        ???  ; illegal $67 
CEE9: B1 09     LDA ($09),Y        
CEEB: 06 3E     ASL $3E            
CEED: BE 65 17  LDX $1765,Y        
CEF0: B5 10     LDA $10,X          
CEF2: 80        ???  ; illegal $80 
CEF3: 44        ???  ; illegal $44 
CEF4: 67        ???  ; illegal $67 
CEF5: 98        TYA                

BOOT1BPAT:
CEF6: 0F        ???  ; illegal $0F 
CEF7: 8B        ???  ; illegal $8B 
CEF8: 0F        ???  ; illegal $0F 
CEF9: 83        ???  ; illegal $83 
CEFA: 0F        ???  ; illegal $0F 
CEFB: 46 0E     LSR $0E            
CEFD: 48        PHA                
CEFE: 0E 46 0D  ASL $0D46          
CF01: 44        ???  ; illegal $44 
CF02: 0C        ???  ; illegal $0C 
CF03: E7        ???  ; illegal $E7 
CF04: 00        BRK                
CF05: 00        BRK                
CF06: 00        BRK                
CF07: 00        BRK                
CF08: 00        BRK                
CF09: 00        BRK                
CF0A: 00        BRK                
CF0B: 00        BRK                
CF0C: 00        BRK                
CF0D: 00        BRK                
CF0E: 00        BRK                
CF0F: 00        BRK                
CF10: 00        BRK                
CF11: 00        BRK                
CF12: 00        BRK                
CF13: 00        BRK                
CF14: 00        BRK                
CF15: 00        BRK                
CF16: 00        BRK                
CF17: 00        BRK                
CF18: 00        BRK                
CF19: 00        BRK                
CF1A: 00        BRK                
CF1B: 00        BRK                
CF1C: 00        BRK                
CF1D: 00        BRK                
CF1E: 00        BRK                
CF1F: 00        BRK                
CF20: 00        BRK                
CF21: 00        BRK                
CF22: 00        BRK                
CF23: 00        BRK                
CF24: 00        BRK                
CF25: 00        BRK                
CF26: 00        BRK                
CF27: 00        BRK                
CF28: 00        BRK                
CF29: 00        BRK                
CF2A: 00        BRK                
CF2B: 00        BRK                
CF2C: 00        BRK                
CF2D: 00        BRK                
CF2E: 00        BRK                
CF2F: 00        BRK                
CF30: 00        BRK                
CF31: 00        BRK                
CF32: 00        BRK                
CF33: 00        BRK                
CF34: 00        BRK                
CF35: 00        BRK                
CF36: 00        BRK                
CF37: 00        BRK                
CF38: 00        BRK                
CF39: 00        BRK                
CF3A: 00        BRK                
CF3B: 00        BRK                
CF3C: 00        BRK                
CF3D: 00        BRK                
CF3E: 00        BRK                
CF3F: 00        BRK                
CF40: 00        BRK                
CF41: 00        BRK                
CF42: 00        BRK                
CF43: 00        BRK                
CF44: 00        BRK                
CF45: 00        BRK                
CF46: 00        BRK                
CF47: 00        BRK                
CF48: 00        BRK                
CF49: 00        BRK                
CF4A: 00        BRK                
CF4B: 00        BRK                
CF4C: 00        BRK                
CF4D: 00        BRK                
CF4E: 00        BRK                
CF4F: 00        BRK                
CF50: 00        BRK                
CF51: 00        BRK                
CF52: 00        BRK                
CF53: 00        BRK                
CF54: 00        BRK                
CF55: 00        BRK                
CF56: 00        BRK                
CF57: 00        BRK                
CF58: 00        BRK                
CF59: 00        BRK                
CF5A: 00        BRK                
CF5B: 00        BRK                
CF5C: 00        BRK                
CF5D: 00        BRK                
CF5E: 00        BRK                
CF5F: 00        BRK                
CF60: 00        BRK                
CF61: 00        BRK                
CF62: 00        BRK                
CF63: 00        BRK                
CF64: 00        BRK                
CF65: 00        BRK                
CF66: 00        BRK                
CF67: 00        BRK                
CF68: 00        BRK                
CF69: 00        BRK                
CF6A: 00        BRK                
CF6B: 00        BRK                
CF6C: 00        BRK                
CF6D: 00        BRK                
CF6E: 00        BRK                
CF6F: 00        BRK                
CF70: 00        BRK                
CF71: 00        BRK                
CF72: 00        BRK                
CF73: 00        BRK                
CF74: 00        BRK                
CF75: 00        BRK                
CF76: 00        BRK                
CF77: 00        BRK                
CF78: 00        BRK                
CF79: 00        BRK                
CF7A: 00        BRK                
CF7B: 00        BRK                
CF7C: 00        BRK                
CF7D: 00        BRK                
CF7E: 00        BRK                
CF7F: 00        BRK                
CF80: 00        BRK                
CF81: 00        BRK                
CF82: 00        BRK                
CF83: 00        BRK                
CF84: 00        BRK                
CF85: 00        BRK                
CF86: 00        BRK                
CF87: 00        BRK                
CF88: 00        BRK                
CF89: 00        BRK                
CF8A: 00        BRK                
CF8B: 00        BRK                
CF8C: 00        BRK                
CF8D: 00        BRK                
CF8E: 00        BRK                
CF8F: 00        BRK                
CF90: 00        BRK                
CF91: 00        BRK                
CF92: 00        BRK                
CF93: 00        BRK                
CF94: 00        BRK                
CF95: 00        BRK                
CF96: 00        BRK                
CF97: 00        BRK                
CF98: 00        BRK                
CF99: 00        BRK                
CF9A: 00        BRK                
CF9B: 00        BRK                
CF9C: 00        BRK                
CF9D: 00        BRK                
CF9E: 00        BRK                
CF9F: 00        BRK                
CFA0: 00        BRK                
CFA1: 00        BRK                
CFA2: 00        BRK                
CFA3: 00        BRK                
CFA4: 00        BRK                
CFA5: 00        BRK                
CFA6: 00        BRK                
CFA7: 00        BRK                
CFA8: 00        BRK                
CFA9: 00        BRK                
CFAA: 00        BRK                
CFAB: 00        BRK                
CFAC: 00        BRK                
CFAD: 00        BRK                
CFAE: 00        BRK                
CFAF: 00        BRK                
CFB0: 00        BRK                
CFB1: 00        BRK                
CFB2: 00        BRK                
CFB3: 00        BRK                
CFB4: 00        BRK                
CFB5: 00        BRK                
CFB6: 00        BRK                
CFB7: 00        BRK                
CFB8: 00        BRK                
CFB9: 00        BRK                
CFBA: 00        BRK                
CFBB: 00        BRK                
CFBC: 00        BRK                
CFBD: 00        BRK                
CFBE: 00        BRK                
CFBF: 00        BRK                
CFC0: 00        BRK                
CFC1: 00        BRK                
CFC2: 00        BRK                
CFC3: 00        BRK                
CFC4: 00        BRK                
CFC5: 00        BRK                
CFC6: 00        BRK                
CFC7: 00        BRK                
CFC8: 00        BRK                
CFC9: 00        BRK                
CFCA: 00        BRK                
CFCB: 00        BRK                
CFCC: 00        BRK                
CFCD: 00        BRK                
CFCE: 00        BRK                
CFCF: 00        BRK                
CFD0: 00        BRK                
CFD1: 00        BRK                
CFD2: 00        BRK                
CFD3: 00        BRK                
CFD4: 00        BRK                
CFD5: 00        BRK                
CFD6: 00        BRK                
CFD7: 00        BRK                
CFD8: 00        BRK                
CFD9: 00        BRK                
CFDA: 00        BRK                
CFDB: 00        BRK                
CFDC: 00        BRK                
CFDD: 00        BRK                
CFDE: 00        BRK                
CFDF: 00        BRK                
CFE0: 00        BRK                
CFE1: 00        BRK                
CFE2: 00        BRK                
CFE3: 00        BRK                
CFE4: 00        BRK                
CFE5: 00        BRK                
CFE6: 00        BRK                
CFE7: 00        BRK                
CFE8: 00        BRK                
CFE9: 00        BRK                
CFEA: 00        BRK                
CFEB: 00        BRK                
CFEC: 00        BRK                
CFED: 00        BRK                
CFEE: 00        BRK                
CFEF: 00        BRK                
CFF0: 00        BRK                
CFF1: 00        BRK                
CFF2: 00        BRK                
CFF3: 00        BRK                
CFF4: 00        BRK                
CFF5: 00        BRK                
CFF6: 00        BRK                
CFF7: 00        BRK                
CFF8: 00        BRK                
CFF9: 00        BRK                
CFFA: 00        BRK                
CFFB: 00        BRK                
CFFC: 00        BRK                
CFFD: 00        BRK                
CFFE: 00        BRK                
CFFF: 00        BRK                
D000: 00        BRK                
D001: 00        BRK                
D002: 00        BRK                
D003: 00        BRK                
D004: 00        BRK                
D005: 00        BRK                
D006: 00        BRK                
D007: 00        BRK                
D008: 00        BRK                
D009: 00        BRK                
D00A: 00        BRK                
D00B: 00        BRK                
D00C: 00        BRK                
D00D: 00        BRK                
D00E: 00        BRK                
D00F: 00        BRK                
D010: 00        BRK                
D011: 00        BRK                
D012: 00        BRK                
D013: 00        BRK                
D014: 00        BRK                
D015: 00        BRK                
D016: 00        BRK                
D017: 00        BRK                
D018: 00        BRK                
D019: 00        BRK                
D01A: 00        BRK                
D01B: 00        BRK                
D01C: 00        BRK                
D01D: 00        BRK                
D01E: 00        BRK                
D01F: 00        BRK                
D020: 00        BRK                
D021: 00        BRK                
D022: 00        BRK                
D023: 00        BRK                
D024: 00        BRK                
D025: 00        BRK                
D026: 00        BRK                
D027: 00        BRK                
D028: 00        BRK                
D029: 00        BRK                
D02A: 00        BRK                
D02B: 00        BRK                
D02C: 00        BRK                
D02D: 00        BRK                
D02E: 00        BRK                
D02F: 00        BRK                
D030: 00        BRK                
D031: 00        BRK                
D032: 00        BRK                
D033: 00        BRK                
D034: 00        BRK                
D035: 00        BRK                
D036: 00        BRK                
D037: 00        BRK                
D038: 00        BRK                
D039: 00        BRK                
D03A: 00        BRK                
D03B: 00        BRK                
D03C: 00        BRK                
D03D: 00        BRK                
D03E: 00        BRK                
D03F: 00        BRK                
D040: 00        BRK                
D041: 00        BRK                
D042: 00        BRK                
D043: 00        BRK                
D044: 00        BRK                
D045: 00        BRK                
D046: 00        BRK                
D047: 00        BRK                
D048: 00        BRK                
D049: 00        BRK                
D04A: 00        BRK                
D04B: 00        BRK                
D04C: 00        BRK                
D04D: 00        BRK                
D04E: 00        BRK                
D04F: 00        BRK                
D050: 00        BRK                
D051: 00        BRK                
D052: 00        BRK                
D053: 00        BRK                
D054: 00        BRK                
D055: 00        BRK                
D056: 00        BRK                
D057: 00        BRK                
D058: 00        BRK                
D059: 00        BRK                
D05A: 00        BRK                
D05B: 00        BRK                
D05C: 00        BRK                
D05D: 00        BRK                
D05E: 00        BRK                
D05F: 00        BRK                
D060: 00        BRK                
D061: 00        BRK                
D062: 00        BRK                
D063: 00        BRK                
D064: 00        BRK                
D065: 00        BRK                
D066: 00        BRK                
D067: 00        BRK                
D068: 00        BRK                
D069: 00        BRK                
D06A: 00        BRK                
D06B: 00        BRK                
D06C: 00        BRK                
D06D: 00        BRK                
D06E: 00        BRK                
D06F: 00        BRK                
D070: 00        BRK                
D071: 00        BRK                
D072: 00        BRK                
D073: 00        BRK                
D074: 00        BRK                
D075: 00        BRK                
D076: 00        BRK                
D077: 00        BRK                
D078: 00        BRK                
D079: 00        BRK                
D07A: 00        BRK                
D07B: 00        BRK                
D07C: 00        BRK                
D07D: 00        BRK                
D07E: 00        BRK                
D07F: 00        BRK                
D080: 00        BRK                
D081: 00        BRK                
D082: 00        BRK                
D083: 00        BRK                
D084: 00        BRK                
D085: 00        BRK                
D086: 00        BRK                
D087: 00        BRK                
D088: 00        BRK                
D089: 00        BRK                
D08A: 00        BRK                
D08B: 00        BRK                
D08C: 00        BRK                
D08D: 00        BRK                
D08E: 00        BRK                
D08F: 00        BRK                
D090: 00        BRK                
D091: 00        BRK                
D092: 00        BRK                
D093: 00        BRK                
D094: 00        BRK                
D095: 00        BRK                
D096: 00        BRK                
D097: 00        BRK                
D098: 00        BRK                
D099: 00        BRK                
D09A: 00        BRK                
D09B: 00        BRK                
D09C: 00        BRK                
D09D: 00        BRK                
D09E: 00        BRK                
D09F: 00        BRK                
D0A0: 00        BRK                
D0A1: 00        BRK                
D0A2: 00        BRK                
D0A3: 00        BRK                
D0A4: 00        BRK                
D0A5: 00        BRK                
D0A6: 00        BRK                
D0A7: 00        BRK                
D0A8: 00        BRK                
D0A9: 00        BRK                
D0AA: 00        BRK                
D0AB: 00        BRK                
D0AC: 00        BRK                
D0AD: 00        BRK                
D0AE: 00        BRK                
D0AF: 00        BRK                
D0B0: 00        BRK                
D0B1: 00        BRK                
D0B2: 00        BRK                
D0B3: 00        BRK                
D0B4: 00        BRK                
D0B5: 00        BRK                
D0B6: 00        BRK                
D0B7: 00        BRK                
D0B8: 00        BRK                
D0B9: 00        BRK                
D0BA: 00        BRK                
D0BB: 00        BRK                
D0BC: 00        BRK                
D0BD: 00        BRK                
D0BE: 00        BRK                
D0BF: 00        BRK                
D0C0: 00        BRK                
D0C1: 00        BRK                
D0C2: 00        BRK                
D0C3: 00        BRK                
D0C4: 00        BRK                
D0C5: 00        BRK                
D0C6: 00        BRK                
D0C7: 00        BRK                
D0C8: 00        BRK                
D0C9: 00        BRK                
D0CA: 00        BRK                
D0CB: 00        BRK                
D0CC: 00        BRK                
D0CD: 00        BRK                
D0CE: 00        BRK                
D0CF: 00        BRK                
D0D0: 00        BRK                
D0D1: 00        BRK                
D0D2: 00        BRK                
D0D3: 00        BRK                
D0D4: 00        BRK                
D0D5: 00        BRK                
D0D6: 00        BRK                
D0D7: 00        BRK                
D0D8: 00        BRK                
D0D9: 00        BRK                
D0DA: 00        BRK                
D0DB: 00        BRK                
D0DC: 00        BRK                
D0DD: 00        BRK                
D0DE: 00        BRK                
D0DF: 00        BRK                
D0E0: 00        BRK                
D0E1: 00        BRK                
D0E2: 00        BRK                
D0E3: 00        BRK                
D0E4: 00        BRK                
D0E5: 00        BRK                
D0E6: 00        BRK                
D0E7: 00        BRK                
D0E8: 00        BRK                
D0E9: 00        BRK                
D0EA: 00        BRK                
D0EB: 00        BRK                
D0EC: 00        BRK                
D0ED: 00        BRK                
D0EE: 00        BRK                
D0EF: 00        BRK                
D0F0: 00        BRK                
D0F1: 00        BRK                
D0F2: 00        BRK                
D0F3: 00        BRK                
D0F4: 00        BRK                
D0F5: 00        BRK                
D0F6: 00        BRK                
D0F7: 00        BRK                
D0F8: 00        BRK                
D0F9: 00        BRK                
D0FA: 00        BRK                
D0FB: 00        BRK                
D0FC: 00        BRK                
D0FD: 00        BRK                
D0FE: 00        BRK                
D0FF: 00        BRK                
D100: 00        BRK                
D101: 00        BRK                
D102: 00        BRK                
D103: 00        BRK                
D104: 00        BRK                
D105: 00        BRK                
D106: 00        BRK                
D107: 00        BRK                
D108: 00        BRK                
D109: 00        BRK                
D10A: 00        BRK                
D10B: 00        BRK                
D10C: 00        BRK                
D10D: 00        BRK                
D10E: 00        BRK                
D10F: 00        BRK                
D110: 00        BRK                
D111: 00        BRK                
D112: 00        BRK                
D113: 00        BRK                
D114: 00        BRK                
D115: 00        BRK                
D116: 00        BRK                
D117: 00        BRK                
D118: 00        BRK                
D119: 00        BRK                
D11A: 00        BRK                
D11B: 00        BRK                
D11C: 00        BRK                
D11D: 00        BRK                
D11E: 00        BRK                
D11F: 00        BRK                
D120: 00        BRK                
D121: 00        BRK                
D122: 00        BRK                
D123: 00        BRK                
D124: 00        BRK                
D125: 00        BRK                
D126: 00        BRK                
D127: 00        BRK                
D128: 00        BRK                
D129: 00        BRK                
D12A: 00        BRK                
D12B: 00        BRK                
D12C: 00        BRK                
D12D: 00        BRK                
D12E: 00        BRK                
D12F: 00        BRK                
D130: 00        BRK                
D131: 00        BRK                
D132: 00        BRK                
D133: 00        BRK                
D134: 00        BRK                
D135: 00        BRK                
D136: 00        BRK                
D137: 00        BRK                
D138: 00        BRK                
D139: 00        BRK                
D13A: 00        BRK                
D13B: 00        BRK                
D13C: 00        BRK                
D13D: 00        BRK                
D13E: 00        BRK                
D13F: 00        BRK                
D140: 00        BRK                
D141: 00        BRK                
D142: 00        BRK                
D143: 00        BRK                
D144: 00        BRK                
D145: 00        BRK                
D146: 00        BRK                
D147: 00        BRK                
D148: 00        BRK                
D149: 00        BRK                
D14A: 00        BRK                
D14B: 00        BRK                
D14C: 00        BRK                
D14D: 00        BRK                
D14E: 00        BRK                
D14F: 00        BRK                
D150: 00        BRK                
D151: 00        BRK                
D152: 00        BRK                
D153: 00        BRK                
D154: 00        BRK                
D155: 00        BRK                
D156: 00        BRK                
D157: 00        BRK                
D158: 00        BRK                
D159: 00        BRK                
D15A: 00        BRK                
D15B: 00        BRK                
D15C: 00        BRK                
D15D: 00        BRK                
D15E: 00        BRK                
D15F: 00        BRK                
D160: 00        BRK                
D161: 00        BRK                
D162: 00        BRK                
D163: 00        BRK                
D164: 00        BRK                
D165: 00        BRK                
D166: 00        BRK                
D167: 00        BRK                
D168: 00        BRK                
D169: 00        BRK                
D16A: 00        BRK                
D16B: 00        BRK                
D16C: 00        BRK                
D16D: 00        BRK                
D16E: 00        BRK                
D16F: 00        BRK                
D170: 00        BRK                
D171: 00        BRK                
D172: 00        BRK                
D173: 00        BRK                
D174: 00        BRK                
D175: 00        BRK                
D176: 00        BRK                
D177: 00        BRK                
D178: 00        BRK                
D179: 00        BRK                
D17A: 00        BRK                
D17B: 00        BRK                
D17C: 00        BRK                
D17D: 00        BRK                
D17E: 00        BRK                
D17F: 00        BRK                
D180: 00        BRK                
D181: 00        BRK                
D182: 00        BRK                
D183: 00        BRK                
D184: 00        BRK                
D185: 00        BRK                
D186: 00        BRK                
D187: 00        BRK                
D188: 00        BRK                
D189: 00        BRK                
D18A: 00        BRK                
D18B: 00        BRK                
D18C: 00        BRK                
D18D: 00        BRK                
D18E: 00        BRK                
D18F: 00        BRK                
D190: 00        BRK                
D191: 00        BRK                
D192: 00        BRK                
D193: 00        BRK                
D194: 00        BRK                
D195: 00        BRK                
D196: 00        BRK                
D197: 00        BRK                
D198: 00        BRK                
D199: 00        BRK                
D19A: 00        BRK                
D19B: 00        BRK                
D19C: 00        BRK                
D19D: 00        BRK                
D19E: 00        BRK                
D19F: 00        BRK                
D1A0: 00        BRK                
D1A1: 00        BRK                
D1A2: 00        BRK                
D1A3: 00        BRK                
D1A4: 00        BRK                
D1A5: 00        BRK                
D1A6: 00        BRK                
D1A7: 00        BRK                
D1A8: 00        BRK                
D1A9: 00        BRK                
D1AA: 00        BRK                
D1AB: 00        BRK                
D1AC: 00        BRK                
D1AD: 00        BRK                
D1AE: 00        BRK                
D1AF: 00        BRK                
D1B0: 00        BRK                
D1B1: 00        BRK                
D1B2: 00        BRK                
D1B3: 00        BRK                
D1B4: 00        BRK                
D1B5: 00        BRK                
D1B6: 00        BRK                
D1B7: 00        BRK                
D1B8: 00        BRK                
D1B9: 00        BRK                
D1BA: 00        BRK                
D1BB: 00        BRK                
D1BC: 00        BRK                
D1BD: 00        BRK                
D1BE: 00        BRK                
D1BF: 00        BRK                
D1C0: 00        BRK                
D1C1: 00        BRK                
D1C2: 00        BRK                
D1C3: 00        BRK                
D1C4: 00        BRK                
D1C5: 00        BRK                
D1C6: 00        BRK                
D1C7: 00        BRK                
D1C8: 00        BRK                
D1C9: 00        BRK                
D1CA: 00        BRK                
D1CB: 00        BRK                
D1CC: 00        BRK                
D1CD: 00        BRK                
D1CE: 00        BRK                
D1CF: 00        BRK                
D1D0: 00        BRK                
D1D1: 00        BRK                
D1D2: 00        BRK                
D1D3: 00        BRK                
D1D4: 00        BRK                
D1D5: 00        BRK                
D1D6: 00        BRK                
D1D7: 00        BRK                
D1D8: 00        BRK                
D1D9: 00        BRK                
D1DA: 00        BRK                
D1DB: 00        BRK                
D1DC: 00        BRK                
D1DD: 00        BRK                
D1DE: 00        BRK                
D1DF: 00        BRK                
D1E0: 00        BRK                
D1E1: 00        BRK                
D1E2: 00        BRK                
D1E3: 00        BRK                
D1E4: 00        BRK                
D1E5: 00        BRK                
D1E6: 00        BRK                
D1E7: 00        BRK                
D1E8: 00        BRK                
D1E9: 00        BRK                
D1EA: 00        BRK                
D1EB: 00        BRK                
D1EC: 00        BRK                
D1ED: 00        BRK                
D1EE: 00        BRK                
D1EF: 00        BRK                
D1F0: 00        BRK                
D1F1: 00        BRK                
D1F2: 00        BRK                
D1F3: 00        BRK                
D1F4: 00        BRK                
D1F5: 00        BRK                
D1F6: 00        BRK                
D1F7: 00        BRK                
D1F8: 00        BRK                
D1F9: 00        BRK                
D1FA: 00        BRK                
D1FB: 00        BRK                
D1FC: 00        BRK                
D1FD: 00        BRK                
D1FE: 00        BRK                
D1FF: 00        BRK                
D200: 00        BRK                
D201: 00        BRK                
D202: 00        BRK                
D203: 00        BRK                
D204: 00        BRK                
D205: 00        BRK                
D206: 00        BRK                
D207: 00        BRK                
D208: 00        BRK                
D209: 00        BRK                
D20A: 00        BRK                
D20B: 00        BRK                
D20C: 00        BRK                
D20D: 00        BRK                
D20E: 00        BRK                
D20F: 00        BRK                
D210: 00        BRK                
D211: 00        BRK                
D212: 00        BRK                
D213: 00        BRK                
D214: 00        BRK                
D215: 00        BRK                
D216: 00        BRK                
D217: 00        BRK                
D218: 00        BRK                
D219: 00        BRK                
D21A: 00        BRK                
D21B: 00        BRK                
D21C: 00        BRK                
D21D: 00        BRK                
D21E: 00        BRK                
D21F: 00        BRK                
D220: 00        BRK                
D221: 00        BRK                
D222: 00        BRK                
D223: 00        BRK                
D224: 00        BRK                
D225: 00        BRK                
D226: 00        BRK                
D227: 00        BRK                
D228: 00        BRK                
D229: 00        BRK                
D22A: 00        BRK                
D22B: 00        BRK                
D22C: 00        BRK                
D22D: 00        BRK                
D22E: 00        BRK                
D22F: 00        BRK                
D230: 00        BRK                
D231: 00        BRK                
D232: 00        BRK                
D233: 00        BRK                
D234: 00        BRK                
D235: 00        BRK                
D236: 00        BRK                
D237: 00        BRK                
D238: 00        BRK                
D239: 00        BRK                
D23A: 00        BRK                
D23B: 00        BRK                
D23C: 00        BRK                
D23D: 00        BRK                
D23E: 00        BRK                
D23F: 00        BRK                
D240: 00        BRK                
D241: 00        BRK                
D242: 00        BRK                
D243: 00        BRK                
D244: 00        BRK                
D245: 00        BRK                
D246: 00        BRK                
D247: 00        BRK                
D248: 00        BRK                
D249: 00        BRK                
D24A: 00        BRK                
D24B: 00        BRK                
D24C: 00        BRK                
D24D: 00        BRK                
D24E: 00        BRK                
D24F: 00        BRK                
D250: 00        BRK                
D251: 00        BRK                
D252: 00        BRK                
D253: 00        BRK                
D254: 00        BRK                
D255: 00        BRK                
D256: 00        BRK                
D257: 00        BRK                
D258: 00        BRK                
D259: 00        BRK                
D25A: 00        BRK                
D25B: 00        BRK                
D25C: 00        BRK                
D25D: 00        BRK                
D25E: 00        BRK                
D25F: 00        BRK                
D260: 00        BRK                
D261: 00        BRK                
D262: 00        BRK                
D263: 00        BRK                
D264: 00        BRK                
D265: 00        BRK                
D266: 00        BRK                
D267: 00        BRK                
D268: 00        BRK                
D269: 00        BRK                
D26A: 00        BRK                
D26B: 00        BRK                
D26C: 00        BRK                
D26D: 00        BRK                
D26E: 00        BRK                
D26F: 00        BRK                
D270: 00        BRK                
D271: 00        BRK                
D272: 00        BRK                
D273: 00        BRK                
D274: 00        BRK                
D275: 00        BRK                
D276: 00        BRK                
D277: 00        BRK                
D278: 00        BRK                
D279: 00        BRK                
D27A: 00        BRK                
D27B: 00        BRK                
D27C: 00        BRK                
D27D: 00        BRK                
D27E: 00        BRK                
D27F: 00        BRK                
D280: 00        BRK                
D281: 00        BRK                
D282: 00        BRK                
D283: 00        BRK                
D284: 00        BRK                
D285: 00        BRK                
D286: 00        BRK                
D287: 00        BRK                
D288: 00        BRK                
D289: 00        BRK                
D28A: 00        BRK                
D28B: 00        BRK                
D28C: 00        BRK                
D28D: 00        BRK                
D28E: 00        BRK                
D28F: 00        BRK                
D290: 00        BRK                
D291: 00        BRK                
D292: 00        BRK                
D293: 00        BRK                
D294: 00        BRK                
D295: 00        BRK                
D296: 00        BRK                
D297: 00        BRK                
D298: 00        BRK                
D299: 00        BRK                
D29A: 00        BRK                
D29B: 00        BRK                
D29C: 00        BRK                
D29D: 00        BRK                
D29E: 00        BRK                
D29F: 00        BRK                
D2A0: 00        BRK                
D2A1: 00        BRK                
D2A2: 00        BRK                
D2A3: 00        BRK                
D2A4: 00        BRK                
D2A5: 00        BRK                
D2A6: 00        BRK                
D2A7: 00        BRK                
D2A8: 00        BRK                
D2A9: 00        BRK                
D2AA: 00        BRK                
D2AB: 00        BRK                
D2AC: 00        BRK                
D2AD: 00        BRK                
D2AE: 00        BRK                
D2AF: 00        BRK                
D2B0: 00        BRK                
D2B1: 00        BRK                
D2B2: 00        BRK                
D2B3: 00        BRK                
D2B4: 00        BRK                
D2B5: 00        BRK                
D2B6: 00        BRK                
D2B7: 00        BRK                
D2B8: 00        BRK                
D2B9: 00        BRK                
D2BA: 00        BRK                
D2BB: 00        BRK                
D2BC: 00        BRK                
D2BD: 00        BRK                
D2BE: 00        BRK                
D2BF: 00        BRK                
D2C0: 00        BRK                
D2C1: 00        BRK                
D2C2: 00        BRK                
D2C3: 00        BRK                
D2C4: 00        BRK                
D2C5: 00        BRK                
D2C6: 00        BRK                
D2C7: 00        BRK                
D2C8: 00        BRK                
D2C9: 00        BRK                
D2CA: 00        BRK                
D2CB: 00        BRK                
D2CC: 00        BRK                
D2CD: 00        BRK                
D2CE: 00        BRK                
D2CF: 00        BRK                
D2D0: 00        BRK                
D2D1: 00        BRK                
D2D2: 00        BRK                
D2D3: 00        BRK                
D2D4: 00        BRK                
D2D5: 00        BRK                
D2D6: 00        BRK                
D2D7: 00        BRK                
D2D8: 00        BRK                
D2D9: 00        BRK                
D2DA: 00        BRK                
D2DB: 00        BRK                
D2DC: 00        BRK                
D2DD: 00        BRK                
D2DE: 00        BRK                
D2DF: 00        BRK                
D2E0: 00        BRK                
D2E1: 00        BRK                
D2E2: 00        BRK                
D2E3: 00        BRK                
D2E4: 00        BRK                
D2E5: 00        BRK                
D2E6: 00        BRK                
D2E7: 00        BRK                
D2E8: 00        BRK                
D2E9: 00        BRK                
D2EA: 00        BRK                
D2EB: 00        BRK                
D2EC: 00        BRK                
D2ED: 00        BRK                
D2EE: 00        BRK                
D2EF: 00        BRK                
D2F0: 00        BRK                
D2F1: 00        BRK                
D2F2: 00        BRK                
D2F3: 00        BRK                
D2F4: 00        BRK                
D2F5: 00        BRK                
D2F6: 00        BRK                
D2F7: 00        BRK                
D2F8: 00        BRK                
D2F9: 00        BRK                
D2FA: 00        BRK                
D2FB: 00        BRK                
D2FC: 00        BRK                
D2FD: 00        BRK                
D2FE: 00        BRK                
D2FF: 00        BRK                
D300: 00        BRK                
D301: 00        BRK                
D302: 00        BRK                
D303: 00        BRK                
D304: 00        BRK                
D305: 00        BRK                
D306: 00        BRK                
D307: 00        BRK                
D308: 00        BRK                
D309: 00        BRK                
D30A: 00        BRK                
D30B: 00        BRK                
D30C: 00        BRK                
D30D: 00        BRK                
D30E: 00        BRK                
D30F: 00        BRK                
D310: 00        BRK                
D311: 00        BRK                
D312: 00        BRK                
D313: 00        BRK                
D314: 00        BRK                
D315: 00        BRK                
D316: 00        BRK                
D317: 00        BRK                
D318: 00        BRK                
D319: 00        BRK                
D31A: 00        BRK                
D31B: 00        BRK                
D31C: 00        BRK                
D31D: 00        BRK                
D31E: 00        BRK                
D31F: 00        BRK                
D320: 00        BRK                
D321: 00        BRK                
D322: 00        BRK                
D323: 00        BRK                
D324: 00        BRK                
D325: 00        BRK                
D326: 00        BRK                
D327: 00        BRK                
D328: 00        BRK                
D329: 00        BRK                
D32A: 00        BRK                
D32B: 00        BRK                
D32C: 00        BRK                
D32D: 00        BRK                
D32E: 00        BRK                
D32F: 00        BRK                
D330: 00        BRK                
D331: 00        BRK                
D332: 00        BRK                
D333: 00        BRK                
D334: 00        BRK                
D335: 00        BRK                
D336: 00        BRK                
D337: 00        BRK                
D338: 00        BRK                
D339: 00        BRK                
D33A: 00        BRK                
D33B: 00        BRK                
D33C: 00        BRK                
D33D: 00        BRK                
D33E: 00        BRK                
D33F: 00        BRK                
D340: 00        BRK                
D341: 00        BRK                
D342: 00        BRK                
D343: 00        BRK                
D344: 00        BRK                
D345: 00        BRK                
D346: 00        BRK                
D347: 00        BRK                
D348: 00        BRK                
D349: 00        BRK                
D34A: 00        BRK                
D34B: 00        BRK                
D34C: 00        BRK                
D34D: 00        BRK                
D34E: 00        BRK                
D34F: 00        BRK                
D350: 00        BRK                
D351: 00        BRK                
D352: 00        BRK                
D353: 00        BRK                
D354: 00        BRK                
D355: 00        BRK                
D356: 00        BRK                
D357: 00        BRK                
D358: 00        BRK                
D359: 00        BRK                
D35A: 00        BRK                
D35B: 00        BRK                
D35C: 00        BRK                
D35D: 00        BRK                
D35E: 00        BRK                
D35F: 00        BRK                
D360: 00        BRK                
D361: 00        BRK                
D362: 00        BRK                
D363: 00        BRK                
D364: 00        BRK                
D365: 00        BRK                
D366: 00        BRK                
D367: 00        BRK                
D368: 00        BRK                
D369: 00        BRK                
D36A: 00        BRK                
D36B: 00        BRK                
D36C: 00        BRK                
D36D: 00        BRK                
D36E: 00        BRK                
D36F: 00        BRK                
D370: 00        BRK                
D371: 00        BRK                
D372: 00        BRK                
D373: 00        BRK                
D374: 00        BRK                
D375: 00        BRK                
D376: 00        BRK                
D377: 00        BRK                
D378: 00        BRK                
D379: 00        BRK                
D37A: 00        BRK                
D37B: 00        BRK                
D37C: 00        BRK                
D37D: 00        BRK                
D37E: 00        BRK                
D37F: 00        BRK                
D380: 00        BRK                
D381: 00        BRK                
D382: 00        BRK                
D383: 00        BRK                
D384: 00        BRK                
D385: 00        BRK                
D386: 00        BRK                
D387: 00        BRK                
D388: 00        BRK                
D389: 00        BRK                
D38A: 00        BRK                
D38B: 00        BRK                
D38C: 00        BRK                
D38D: 00        BRK                
D38E: 00        BRK                
D38F: 00        BRK                
D390: 00        BRK                
D391: 00        BRK                
D392: 00        BRK                
D393: 00        BRK                
D394: 00        BRK                
D395: 00        BRK                
D396: 00        BRK                
D397: 00        BRK                
D398: 00        BRK                
D399: 00        BRK                
D39A: 00        BRK                
D39B: 00        BRK                
D39C: 00        BRK                
D39D: 00        BRK                
D39E: 00        BRK                
D39F: 00        BRK                
D3A0: 00        BRK                
D3A1: 00        BRK                
D3A2: 00        BRK                
D3A3: 00        BRK                
D3A4: 00        BRK                
D3A5: 00        BRK                
D3A6: 00        BRK                
D3A7: 00        BRK                
D3A8: 00        BRK                
D3A9: 00        BRK                
D3AA: 00        BRK                
D3AB: 00        BRK                
D3AC: 00        BRK                
D3AD: 00        BRK                
D3AE: 00        BRK                
D3AF: 00        BRK                
D3B0: 00        BRK                
D3B1: 00        BRK                
D3B2: 00        BRK                
D3B3: 00        BRK                
D3B4: 00        BRK                
D3B5: 00        BRK                
D3B6: 00        BRK                
D3B7: 00        BRK                
D3B8: 00        BRK                
D3B9: 00        BRK                
D3BA: 00        BRK                
D3BB: 00        BRK                
D3BC: 00        BRK                
D3BD: 00        BRK                
D3BE: 00        BRK                
D3BF: 00        BRK                
D3C0: 00        BRK                
D3C1: 00        BRK                
D3C2: 00        BRK                
D3C3: 00        BRK                
D3C4: 00        BRK                
D3C5: 00        BRK                
D3C6: 00        BRK                
D3C7: 00        BRK                
D3C8: 00        BRK                
D3C9: 00        BRK                
D3CA: 00        BRK                
D3CB: 00        BRK                
D3CC: 00        BRK                
D3CD: 00        BRK                
D3CE: 00        BRK                
D3CF: 00        BRK                
D3D0: 00        BRK                
D3D1: 00        BRK                
D3D2: 00        BRK                
D3D3: 00        BRK                
D3D4: 00        BRK                
D3D5: 00        BRK                
D3D6: 00        BRK                
D3D7: 00        BRK                
D3D8: 00        BRK                
D3D9: 00        BRK                
D3DA: 00        BRK                
D3DB: 00        BRK                
D3DC: 00        BRK                
D3DD: 00        BRK                
D3DE: 00        BRK                
D3DF: 00        BRK                
D3E0: 00        BRK                
D3E1: 00        BRK                
D3E2: 00        BRK                
D3E3: 00        BRK                
D3E4: 00        BRK                
D3E5: 00        BRK                
D3E6: 00        BRK                
D3E7: 00        BRK                
D3E8: 00        BRK                
D3E9: 00        BRK                
D3EA: 00        BRK                
D3EB: 00        BRK                
D3EC: 00        BRK                
D3ED: 00        BRK                
D3EE: 00        BRK                
D3EF: 00        BRK                
D3F0: 00        BRK                
D3F1: 00        BRK                
D3F2: 00        BRK                
D3F3: 00        BRK                
D3F4: 00        BRK                
D3F5: 00        BRK                
D3F6: 00        BRK                
D3F7: 00        BRK                
D3F8: 00        BRK                
D3F9: 00        BRK                
D3FA: 00        BRK                
D3FB: 00        BRK                
D3FC: 00        BRK                
D3FD: 00        BRK                
D3FE: 00        BRK                
D3FF: 00        BRK                
D400: 00        BRK                
D401: 00        BRK                
D402: 00        BRK                
D403: 00        BRK                
D404: 00        BRK                
D405: 00        BRK                
D406: 00        BRK                
D407: 00        BRK                
D408: 00        BRK                
D409: 00        BRK                
D40A: 00        BRK                
D40B: 00        BRK                
D40C: 00        BRK                
D40D: 00        BRK                
D40E: 00        BRK                
D40F: 00        BRK                
D410: 00        BRK                
D411: 00        BRK                
D412: 00        BRK                
D413: 00        BRK                
D414: 00        BRK                
D415: 00        BRK                
D416: 00        BRK                
D417: 00        BRK                
D418: 00        BRK                
D419: 00        BRK                
D41A: 00        BRK                
D41B: 00        BRK                
D41C: 00        BRK                
D41D: 00        BRK                
D41E: 00        BRK                
D41F: 00        BRK                
D420: 00        BRK                
D421: 00        BRK                
D422: 00        BRK                
D423: 00        BRK                
D424: 00        BRK                
D425: 00        BRK                
D426: 00        BRK                
D427: 00        BRK                
D428: 00        BRK                
D429: 00        BRK                
D42A: 00        BRK                
D42B: 00        BRK                
D42C: 00        BRK                
D42D: 00        BRK                
D42E: 00        BRK                
D42F: 00        BRK                
D430: 00        BRK                
D431: 00        BRK                
D432: 00        BRK                
D433: 00        BRK                
D434: 00        BRK                
D435: 00        BRK                
D436: 00        BRK                
D437: 00        BRK                
D438: 00        BRK                
D439: 00        BRK                
D43A: 00        BRK                
D43B: 00        BRK                
D43C: 00        BRK                
D43D: 00        BRK                
D43E: 00        BRK                
D43F: 00        BRK                
D440: 00        BRK                
D441: 00        BRK                
D442: 00        BRK                
D443: 00        BRK                
D444: 00        BRK                
D445: 00        BRK                
D446: 00        BRK                
D447: 00        BRK                
D448: 00        BRK                
D449: 00        BRK                
D44A: 00        BRK                
D44B: 00        BRK                
D44C: 00        BRK                
D44D: 00        BRK                
D44E: 00        BRK                
D44F: 00        BRK                
D450: 00        BRK                
D451: 00        BRK                
D452: 00        BRK                
D453: 00        BRK                
D454: 00        BRK                
D455: 00        BRK                
D456: 00        BRK                
D457: 00        BRK                
D458: 00        BRK                
D459: 00        BRK                
D45A: 00        BRK                
D45B: 00        BRK                
D45C: 00        BRK                
D45D: 00        BRK                
D45E: 00        BRK                
D45F: 00        BRK                
D460: 00        BRK                
D461: 00        BRK                
D462: 00        BRK                
D463: 00        BRK                
D464: 00        BRK                
D465: 00        BRK                
D466: 00        BRK                
D467: 00        BRK                
D468: 00        BRK                
D469: 00        BRK                
D46A: 00        BRK                
D46B: 00        BRK                
D46C: 00        BRK                
D46D: 00        BRK                
D46E: 00        BRK                
D46F: 00        BRK                
D470: 00        BRK                
D471: 00        BRK                
D472: 00        BRK                
D473: 00        BRK                
D474: 00        BRK                
D475: 00        BRK                
D476: 00        BRK                
D477: 00        BRK                
D478: 00        BRK                
D479: 00        BRK                
D47A: 00        BRK                
D47B: 00        BRK                
D47C: 00        BRK                
D47D: 00        BRK                
D47E: 00        BRK                
D47F: 00        BRK                
D480: 00        BRK                
D481: 00        BRK                
D482: 00        BRK                
D483: 00        BRK                
D484: 00        BRK                
D485: 00        BRK                
D486: 00        BRK                
D487: 00        BRK                
D488: 00        BRK                
D489: 00        BRK                
D48A: 00        BRK                
D48B: 00        BRK                
D48C: 00        BRK                
D48D: 00        BRK                
D48E: 00        BRK                
D48F: 00        BRK                
D490: 00        BRK                
D491: 00        BRK                
D492: 00        BRK                
D493: 00        BRK                
D494: 00        BRK                
D495: 00        BRK                
D496: 00        BRK                
D497: 00        BRK                
D498: 00        BRK                
D499: 00        BRK                
D49A: 00        BRK                
D49B: 00        BRK                
D49C: 00        BRK                
D49D: 00        BRK                
D49E: 00        BRK                
D49F: 00        BRK                
D4A0: 00        BRK                
D4A1: 00        BRK                
D4A2: 00        BRK                
D4A3: 00        BRK                
D4A4: 00        BRK                
D4A5: 00        BRK                
D4A6: 00        BRK                
D4A7: 00        BRK                
D4A8: 00        BRK                
D4A9: 00        BRK                
D4AA: 00        BRK                
D4AB: 00        BRK                
D4AC: 00        BRK                
D4AD: 00        BRK                
D4AE: 00        BRK                
D4AF: 00        BRK                
D4B0: 00        BRK                
D4B1: 00        BRK                
D4B2: 00        BRK                
D4B3: 00        BRK                
D4B4: 00        BRK                
D4B5: 00        BRK                
D4B6: 00        BRK                
D4B7: 00        BRK                
D4B8: 00        BRK                
D4B9: 00        BRK                
D4BA: 00        BRK                
D4BB: 00        BRK                
D4BC: 00        BRK                
D4BD: 00        BRK                
D4BE: 00        BRK                
D4BF: 00        BRK                
D4C0: 00        BRK                
D4C1: 00        BRK                
D4C2: 00        BRK                
D4C3: 00        BRK                
D4C4: 00        BRK                
D4C5: 00        BRK                
D4C6: 00        BRK                
D4C7: 00        BRK                
D4C8: 00        BRK                
D4C9: 00        BRK                
D4CA: 00        BRK                
D4CB: 00        BRK                
D4CC: 00        BRK                
D4CD: 00        BRK                
D4CE: 00        BRK                
D4CF: 00        BRK                
D4D0: 00        BRK                
D4D1: 00        BRK                
D4D2: 00        BRK                
D4D3: 00        BRK                
D4D4: 00        BRK                
D4D5: 00        BRK                
D4D6: 00        BRK                
D4D7: 00        BRK                
D4D8: 00        BRK                
D4D9: 00        BRK                
D4DA: 00        BRK                
D4DB: 00        BRK                
D4DC: 00        BRK                
D4DD: 00        BRK                
D4DE: 00        BRK                
D4DF: 00        BRK                
D4E0: 00        BRK                
D4E1: 00        BRK                
D4E2: 00        BRK                
D4E3: 00        BRK                
D4E4: 00        BRK                
D4E5: 00        BRK                
D4E6: 00        BRK                
D4E7: 00        BRK                
D4E8: 00        BRK                
D4E9: 00        BRK                
D4EA: 00        BRK                
D4EB: 00        BRK                
D4EC: 00        BRK                
D4ED: 00        BRK                
D4EE: 00        BRK                
D4EF: 00        BRK                
D4F0: 00        BRK                
D4F1: 00        BRK                
D4F2: 00        BRK                
D4F3: 00        BRK                
D4F4: 00        BRK                
D4F5: 00        BRK                
D4F6: 00        BRK                
D4F7: 00        BRK                
D4F8: 00        BRK                
D4F9: 00        BRK                
D4FA: 00        BRK                
D4FB: 00        BRK                
D4FC: 00        BRK                
D4FD: 00        BRK                
D4FE: 00        BRK                
D4FF: 00        BRK                
D500: 00        BRK                
D501: 00        BRK                
D502: 00        BRK                
D503: 00        BRK                
D504: 00        BRK                
D505: 00        BRK                
D506: 00        BRK                
D507: 00        BRK                
D508: 00        BRK                
D509: 00        BRK                
D50A: 00        BRK                
D50B: 00        BRK                
D50C: 00        BRK                
D50D: 00        BRK                
D50E: 00        BRK                
D50F: 00        BRK                
D510: 00        BRK                
D511: 00        BRK                
D512: 00        BRK                
D513: 00        BRK                
D514: 00        BRK                
D515: 00        BRK                
D516: 00        BRK                
D517: 00        BRK                
D518: 00        BRK                
D519: 00        BRK                
D51A: 00        BRK                
D51B: 00        BRK                
D51C: 00        BRK                
D51D: 00        BRK                
D51E: 00        BRK                
D51F: 00        BRK                
D520: 00        BRK                
D521: 00        BRK                
D522: 00        BRK                
D523: 00        BRK                
D524: 00        BRK                
D525: 00        BRK                
D526: 00        BRK                
D527: 00        BRK                
D528: 00        BRK                
D529: 00        BRK                
D52A: 00        BRK                
D52B: 00        BRK                
D52C: 00        BRK                
D52D: 00        BRK                
D52E: 00        BRK                
D52F: 00        BRK                
D530: 00        BRK                
D531: 00        BRK                
D532: 00        BRK                
D533: 00        BRK                
D534: 00        BRK                
D535: 00        BRK                
D536: 00        BRK                
D537: 00        BRK                
D538: 00        BRK                
D539: 00        BRK                
D53A: 00        BRK                
D53B: 00        BRK                
D53C: 00        BRK                
D53D: 00        BRK                
D53E: 00        BRK                
D53F: 00        BRK                
D540: 00        BRK                
D541: 00        BRK                
D542: 00        BRK                
D543: 00        BRK                
D544: 00        BRK                
D545: 00        BRK                
D546: 00        BRK                
D547: 00        BRK                
D548: 00        BRK                
D549: 00        BRK                
D54A: 00        BRK                
D54B: 00        BRK                
D54C: 00        BRK                
D54D: 00        BRK                
D54E: 00        BRK                
D54F: 00        BRK                
D550: 00        BRK                
D551: 00        BRK                
D552: 00        BRK                
D553: 00        BRK                
D554: 00        BRK                
D555: 00        BRK                
D556: 00        BRK                
D557: 00        BRK                
D558: 00        BRK                
D559: 00        BRK                
D55A: 00        BRK                
D55B: 00        BRK                
D55C: 00        BRK                
D55D: 00        BRK                
D55E: 00        BRK                
D55F: 00        BRK                
D560: 00        BRK                
D561: 00        BRK                
D562: 00        BRK                
D563: 00        BRK                
D564: 00        BRK                
D565: 00        BRK                
D566: 00        BRK                
D567: 00        BRK                
D568: 00        BRK                
D569: 00        BRK                
D56A: 00        BRK                
D56B: 00        BRK                
D56C: 00        BRK                
D56D: 00        BRK                
D56E: 00        BRK                
D56F: 00        BRK                
D570: 00        BRK                
D571: 00        BRK                
D572: 00        BRK                
D573: 00        BRK                
D574: 00        BRK                
D575: 00        BRK                
D576: 00        BRK                
D577: 00        BRK                
D578: 00        BRK                
D579: 00        BRK                
D57A: 00        BRK                
D57B: 00        BRK                
D57C: 00        BRK                
D57D: 00        BRK                
D57E: 00        BRK                
D57F: 00        BRK                
D580: 00        BRK                
D581: 00        BRK                
D582: 00        BRK                
D583: 00        BRK                
D584: 00        BRK                
D585: 00        BRK                
D586: 00        BRK                
D587: 00        BRK                
D588: 00        BRK                
D589: 00        BRK                
D58A: 00        BRK                
D58B: 00        BRK                
D58C: 00        BRK                
D58D: 00        BRK                
D58E: 00        BRK                
D58F: 00        BRK                
D590: 00        BRK                
D591: 00        BRK                
D592: 00        BRK                
D593: 00        BRK                
D594: 00        BRK                
D595: 00        BRK                
D596: 00        BRK                
D597: 00        BRK                
D598: 00        BRK                
D599: 00        BRK                
D59A: 00        BRK                
D59B: 00        BRK                
D59C: 00        BRK                
D59D: 00        BRK                
D59E: 00        BRK                
D59F: 00        BRK                
D5A0: 00        BRK                
D5A1: 00        BRK                
D5A2: 00        BRK                
D5A3: 00        BRK                
D5A4: 00        BRK                
D5A5: 00        BRK                
D5A6: 00        BRK                
D5A7: 00        BRK                
D5A8: 00        BRK                
D5A9: 00        BRK                
D5AA: 00        BRK                
D5AB: 00        BRK                
D5AC: 00        BRK                
D5AD: 00        BRK                
D5AE: 00        BRK                
D5AF: 00        BRK                
D5B0: 00        BRK                
D5B1: 00        BRK                
D5B2: 00        BRK                
D5B3: 00        BRK                
D5B4: 00        BRK                
D5B5: 00        BRK                
D5B6: 00        BRK                
D5B7: 00        BRK                
D5B8: 00        BRK                
D5B9: 00        BRK                
D5BA: 00        BRK                
D5BB: 00        BRK                
D5BC: 00        BRK                
D5BD: 00        BRK                
D5BE: 00        BRK                
D5BF: 00        BRK                
D5C0: 00        BRK                
D5C1: 00        BRK                
D5C2: 00        BRK                
D5C3: 00        BRK                
D5C4: 00        BRK                
D5C5: 00        BRK                
D5C6: 00        BRK                
D5C7: 00        BRK                
D5C8: 00        BRK                
D5C9: 00        BRK                
D5CA: 00        BRK                
D5CB: 00        BRK                
D5CC: 00        BRK                
D5CD: 00        BRK                
D5CE: 00        BRK                
D5CF: 00        BRK                
D5D0: 00        BRK                
D5D1: 00        BRK                
D5D2: 00        BRK                
D5D3: 00        BRK                
D5D4: 00        BRK                
D5D5: 00        BRK                
D5D6: 00        BRK                
D5D7: 00        BRK                
D5D8: 00        BRK                
D5D9: 00        BRK                
D5DA: 00        BRK                
D5DB: 00        BRK                
D5DC: 00        BRK                
D5DD: 00        BRK                
D5DE: 00        BRK                
D5DF: 00        BRK                
D5E0: 00        BRK                
D5E1: 00        BRK                
D5E2: 00        BRK                
D5E3: 00        BRK                
D5E4: 00        BRK                
D5E5: 00        BRK                
D5E6: 00        BRK                
D5E7: 00        BRK                
D5E8: 00        BRK                
D5E9: 00        BRK                
D5EA: 00        BRK                
D5EB: 00        BRK                
D5EC: 00        BRK                
D5ED: 00        BRK                
D5EE: 00        BRK                
D5EF: 00        BRK                
D5F0: 00        BRK                
D5F1: 00        BRK                
D5F2: 00        BRK                
D5F3: 00        BRK                
D5F4: 00        BRK                
D5F5: 00        BRK                
D5F6: 00        BRK                
D5F7: 00        BRK                
D5F8: 00        BRK                
D5F9: 00        BRK                
D5FA: 00        BRK                
D5FB: 00        BRK                
D5FC: 00        BRK                
D5FD: 00        BRK                
D5FE: 00        BRK                
D5FF: 00        BRK                
D600: 00        BRK                
D601: 00        BRK                
D602: 00        BRK                
D603: 00        BRK                
D604: 00        BRK                
D605: 00        BRK                
D606: 00        BRK                
D607: 00        BRK                
D608: 00        BRK                
D609: 00        BRK                
D60A: 00        BRK                
D60B: 00        BRK                
D60C: 00        BRK                
D60D: 00        BRK                
D60E: 00        BRK                
D60F: 00        BRK                
D610: 00        BRK                
D611: 00        BRK                
D612: 00        BRK                
D613: 00        BRK                
D614: 00        BRK                
D615: 00        BRK                
D616: 00        BRK                
D617: 00        BRK                
D618: 00        BRK                
D619: 00        BRK                
D61A: 00        BRK                
D61B: 00        BRK                
D61C: 00        BRK                
D61D: 00        BRK                
D61E: 00        BRK                
D61F: 00        BRK                
D620: 00        BRK                
D621: 00        BRK                
D622: 00        BRK                
D623: 00        BRK                
D624: 00        BRK                
D625: 00        BRK                
D626: 00        BRK                
D627: 00        BRK                
D628: 00        BRK                
D629: 00        BRK                
D62A: 00        BRK                
D62B: 00        BRK                
D62C: 00        BRK                
D62D: 00        BRK                
D62E: 00        BRK                
D62F: 00        BRK                
D630: 00        BRK                
D631: 00        BRK                
D632: 00        BRK                
D633: 00        BRK                
D634: 00        BRK                
D635: 00        BRK                
D636: 00        BRK                
D637: 00        BRK                
D638: 00        BRK                
D639: 00        BRK                
D63A: 00        BRK                
D63B: 00        BRK                
D63C: 00        BRK                
D63D: 00        BRK                
D63E: 00        BRK                
D63F: 00        BRK                
D640: 00        BRK                
D641: 00        BRK                
D642: 00        BRK                
D643: 00        BRK                
D644: 00        BRK                
D645: 00        BRK                
D646: 00        BRK                
D647: 00        BRK                
D648: 00        BRK                
D649: 00        BRK                
D64A: 00        BRK                
D64B: 00        BRK                
D64C: 00        BRK                
D64D: 00        BRK                
D64E: 00        BRK                
D64F: 00        BRK                
D650: 00        BRK                
D651: 00        BRK                
D652: 00        BRK                
D653: 00        BRK                
D654: 00        BRK                
D655: 00        BRK                
D656: 00        BRK                
D657: 00        BRK                
D658: 00        BRK                
D659: 00        BRK                
D65A: 00        BRK                
D65B: 00        BRK                
D65C: 00        BRK                
D65D: 00        BRK                
D65E: 00        BRK                
D65F: 00        BRK                
D660: 00        BRK                
D661: 00        BRK                
D662: 00        BRK                
D663: 00        BRK                
D664: 00        BRK                
D665: 00        BRK                
D666: 00        BRK                
D667: 00        BRK                
D668: 00        BRK                
D669: 00        BRK                
D66A: 00        BRK                
D66B: 00        BRK                
D66C: 00        BRK                
D66D: 00        BRK                
D66E: 00        BRK                
D66F: 00        BRK                
D670: 00        BRK                
D671: 00        BRK                
D672: 00        BRK                
D673: 00        BRK                
D674: 00        BRK                
D675: 00        BRK                
D676: 00        BRK                
D677: 00        BRK                
D678: 00        BRK                
D679: 00        BRK                
D67A: 00        BRK                
D67B: 00        BRK                
D67C: 00        BRK                
D67D: 00        BRK                
D67E: 00        BRK                
D67F: 00        BRK                
D680: 00        BRK                
D681: 00        BRK                
D682: 00        BRK                
D683: 00        BRK                
D684: 00        BRK                
D685: 00        BRK                
D686: 00        BRK                
D687: 00        BRK                
D688: 00        BRK                
D689: 00        BRK                
D68A: 00        BRK                
D68B: 00        BRK                
D68C: 00        BRK                
D68D: 00        BRK                
D68E: 00        BRK                
D68F: 00        BRK                
D690: 00        BRK                
D691: 00        BRK                
D692: 00        BRK                
D693: 00        BRK                
D694: 00        BRK                
D695: 00        BRK                
D696: 00        BRK                
D697: 00        BRK                
D698: 00        BRK                
D699: 00        BRK                
D69A: 00        BRK                
D69B: 00        BRK                
D69C: 00        BRK                
D69D: 00        BRK                
D69E: 00        BRK                
D69F: 00        BRK                
D6A0: 00        BRK                
D6A1: 00        BRK                
D6A2: 00        BRK                
D6A3: 00        BRK                
D6A4: 00        BRK                
D6A5: 00        BRK                
D6A6: 00        BRK                
D6A7: 00        BRK                
D6A8: 00        BRK                
D6A9: 00        BRK                
D6AA: 00        BRK                
D6AB: 00        BRK                
D6AC: 00        BRK                
D6AD: 00        BRK                
D6AE: 00        BRK                
D6AF: 00        BRK                
D6B0: 00        BRK                
D6B1: 00        BRK                
D6B2: 00        BRK                
D6B3: 00        BRK                
D6B4: 00        BRK                
D6B5: 00        BRK                
D6B6: 00        BRK                
D6B7: 00        BRK                
D6B8: 00        BRK                
D6B9: 00        BRK                
D6BA: 00        BRK                
D6BB: 00        BRK                
D6BC: 00        BRK                
D6BD: 00        BRK                
D6BE: 00        BRK                
D6BF: 00        BRK                
D6C0: 00        BRK                
D6C1: 00        BRK                
D6C2: 00        BRK                
D6C3: 00        BRK                
D6C4: 00        BRK                
D6C5: 00        BRK                
D6C6: 00        BRK                
D6C7: 00        BRK                
D6C8: 00        BRK                
D6C9: 00        BRK                
D6CA: 00        BRK                
D6CB: 00        BRK                
D6CC: 00        BRK                
D6CD: 00        BRK                
D6CE: 00        BRK                
D6CF: 00        BRK                
D6D0: 00        BRK                
D6D1: 00        BRK                
D6D2: 00        BRK                
D6D3: 00        BRK                
D6D4: 00        BRK                
D6D5: 00        BRK                
D6D6: 00        BRK                
D6D7: 00        BRK                
D6D8: 00        BRK                
D6D9: 00        BRK                
D6DA: 00        BRK                
D6DB: 00        BRK                
D6DC: 00        BRK                
D6DD: 00        BRK                
D6DE: 00        BRK                
D6DF: 00        BRK                
D6E0: 00        BRK                
D6E1: 00        BRK                
D6E2: 00        BRK                
D6E3: 00        BRK                
D6E4: 00        BRK                
D6E5: 00        BRK                
D6E6: 00        BRK                
D6E7: 00        BRK                
D6E8: 00        BRK                
D6E9: 00        BRK                
D6EA: 00        BRK                
D6EB: 00        BRK                
D6EC: 00        BRK                
D6ED: 00        BRK                
D6EE: 00        BRK                
D6EF: 00        BRK                
D6F0: 00        BRK                
D6F1: 00        BRK                
D6F2: 00        BRK                
D6F3: 00        BRK                
D6F4: 00        BRK                
D6F5: 00        BRK                
D6F6: 00        BRK                
D6F7: 00        BRK                
D6F8: 00        BRK                
D6F9: 00        BRK                
D6FA: 00        BRK                
D6FB: 00        BRK                
D6FC: 00        BRK                
D6FD: 00        BRK                
D6FE: 00        BRK                
D6FF: 00        BRK                
D700: 00        BRK                
D701: 00        BRK                
D702: 00        BRK                
D703: 00        BRK                
D704: 00        BRK                
D705: 00        BRK                
D706: 00        BRK                
D707: 00        BRK                
D708: 00        BRK                
D709: 00        BRK                
D70A: 00        BRK                
D70B: 00        BRK                
D70C: 00        BRK                
D70D: 00        BRK                
D70E: 00        BRK                
D70F: 00        BRK                
D710: 00        BRK                
D711: 00        BRK                
D712: 00        BRK                
D713: 00        BRK                
D714: 00        BRK                
D715: 00        BRK                
D716: 00        BRK                
D717: 00        BRK                
D718: 00        BRK                
D719: 00        BRK                
D71A: 00        BRK                
D71B: 00        BRK                
D71C: 00        BRK                
D71D: 00        BRK                
D71E: 00        BRK                
D71F: 00        BRK                
D720: 00        BRK                
D721: 00        BRK                
D722: 00        BRK                
D723: 00        BRK                
D724: 00        BRK                
D725: 00        BRK                
D726: 00        BRK                
D727: 00        BRK                
D728: 00        BRK                
D729: 00        BRK                
D72A: 00        BRK                
D72B: 00        BRK                
D72C: 00        BRK                
D72D: 00        BRK                
D72E: 00        BRK                
D72F: 00        BRK                
D730: 00        BRK                
D731: 00        BRK                
D732: 00        BRK                
D733: 00        BRK                
D734: 00        BRK                
D735: 00        BRK                
D736: 00        BRK                
D737: 00        BRK                
D738: 00        BRK                
D739: 00        BRK                
D73A: 00        BRK                
D73B: 00        BRK                
D73C: 00        BRK                
D73D: 00        BRK                
D73E: 00        BRK                
D73F: 00        BRK                
D740: 00        BRK                
D741: 00        BRK                
D742: 00        BRK                
D743: 00        BRK                
D744: 00        BRK                
D745: 00        BRK                
D746: 00        BRK                
D747: 00        BRK                
D748: 00        BRK                
D749: 00        BRK                
D74A: 00        BRK                
D74B: 00        BRK                
D74C: 00        BRK                
D74D: 00        BRK                
D74E: 00        BRK                
D74F: 00        BRK                
D750: 00        BRK                
D751: 00        BRK                
D752: 00        BRK                
D753: 00        BRK                
D754: 00        BRK                
D755: 00        BRK                
D756: 00        BRK                
D757: 00        BRK                
D758: 00        BRK                
D759: 00        BRK                
D75A: 00        BRK                
D75B: 00        BRK                
D75C: 00        BRK                
D75D: 00        BRK                
D75E: 00        BRK                
D75F: 00        BRK                
D760: 00        BRK                
D761: 00        BRK                
D762: 00        BRK                
D763: 00        BRK                
D764: 00        BRK                
D765: 00        BRK                
D766: 00        BRK                
D767: 00        BRK                
D768: 00        BRK                
D769: 00        BRK                
D76A: 00        BRK                
D76B: 00        BRK                
D76C: 00        BRK                
D76D: 00        BRK                
D76E: 00        BRK                
D76F: 00        BRK                
D770: 00        BRK                
D771: 00        BRK                
D772: 00        BRK                
D773: 00        BRK                
D774: 00        BRK                
D775: 00        BRK                
D776: 00        BRK                
D777: 00        BRK                
D778: 00        BRK                
D779: 00        BRK                
D77A: 00        BRK                
D77B: 00        BRK                
D77C: 00        BRK                
D77D: 00        BRK                
D77E: 00        BRK                
D77F: 00        BRK                
D780: 00        BRK                
D781: 00        BRK                
D782: 00        BRK                
D783: 00        BRK                
D784: 00        BRK                
D785: 00        BRK                
D786: 00        BRK                
D787: 00        BRK                
D788: 00        BRK                
D789: 00        BRK                
D78A: 00        BRK                
D78B: 00        BRK                
D78C: 00        BRK                
D78D: 00        BRK                
D78E: 00        BRK                
D78F: 00        BRK                
D790: 00        BRK                
D791: 00        BRK                
D792: 00        BRK                
D793: 00        BRK                
D794: 00        BRK                
D795: 00        BRK                
D796: 00        BRK                
D797: 00        BRK                
D798: 00        BRK                
D799: 00        BRK                
D79A: 00        BRK                
D79B: 00        BRK                
D79C: 00        BRK                
D79D: 00        BRK                
D79E: 00        BRK                
D79F: 00        BRK                
D7A0: 00        BRK                
D7A1: 00        BRK                
D7A2: 00        BRK                
D7A3: 00        BRK                
D7A4: 00        BRK                
D7A5: 00        BRK                
D7A6: 00        BRK                
D7A7: 00        BRK                
D7A8: 00        BRK                
D7A9: 00        BRK                
D7AA: 00        BRK                
D7AB: 00        BRK                
D7AC: 00        BRK                
D7AD: 00        BRK                
D7AE: 00        BRK                
D7AF: 00        BRK                
D7B0: 00        BRK                
D7B1: 00        BRK                
D7B2: 00        BRK                
D7B3: 00        BRK                
D7B4: 00        BRK                
D7B5: 00        BRK                
D7B6: 00        BRK                
D7B7: 00        BRK                
D7B8: 00        BRK                
D7B9: 00        BRK                
D7BA: 00        BRK                
D7BB: 00        BRK                
D7BC: 00        BRK                
D7BD: 00        BRK                
D7BE: 00        BRK                
D7BF: 00        BRK                
D7C0: 00        BRK                
D7C1: 00        BRK                
D7C2: 00        BRK                
D7C3: 00        BRK                
D7C4: 00        BRK                
D7C5: 00        BRK                
D7C6: 00        BRK                
D7C7: 00        BRK                
D7C8: 00        BRK                
D7C9: 00        BRK                
D7CA: 00        BRK                
D7CB: 00        BRK                
D7CC: 00        BRK                
D7CD: 00        BRK                
D7CE: 00        BRK                
D7CF: 00        BRK                
D7D0: 00        BRK                
D7D1: 00        BRK                
D7D2: 00        BRK                
D7D3: 00        BRK                
D7D4: 00        BRK                
D7D5: 00        BRK                
D7D6: 00        BRK                
D7D7: 00        BRK                
D7D8: 00        BRK                
D7D9: 00        BRK                
D7DA: 00        BRK                
D7DB: 00        BRK                
D7DC: 00        BRK                
D7DD: 00        BRK                
D7DE: 00        BRK                
D7DF: 00        BRK                
D7E0: 00        BRK                
D7E1: 00        BRK                
D7E2: 00        BRK                
D7E3: 00        BRK                
D7E4: 00        BRK                
D7E5: 00        BRK                
D7E6: 00        BRK                
D7E7: 00        BRK                
D7E8: 00        BRK                
D7E9: 00        BRK                
D7EA: 00        BRK                
D7EB: 00        BRK                
D7EC: 00        BRK                
D7ED: 00        BRK                
D7EE: 00        BRK                
D7EF: 00        BRK                
D7F0: 00        BRK                
D7F1: 00        BRK                
D7F2: 00        BRK                
D7F3: 00        BRK                
D7F4: 00        BRK                
D7F5: 00        BRK                
D7F6: 00        BRK                
D7F7: 00        BRK                
D7F8: 00        BRK                
D7F9: 00        BRK                
D7FA: 00        BRK                
D7FB: 00        BRK                
D7FC: 00        BRK                
D7FD: 00        BRK                
D7FE: 00        BRK                
D7FF: 00        BRK                
