initial-rom2-test:
	@$(MAKE) -s -C ../../dorado build/baseboard_rom_dump build/im_image build/boot0_hunks
	@mkdir -p obj_initial_rom2 obj_exec
	@cd ../.. && python3 tools/sil_im_map.py --verilog >/dev/null
	@cd ../.. && ./dorado/build/baseboard_rom_dump 'chm/dorado/doradobaserom.mb!13' > verilog/verilator/obj_initial_rom2/rom.txt
	@cd ../.. && ./dorado/build/im_image 'chm/dorado/expanded/bootstrap.dm!20_/Initial.mb' > verilog/verilator/obj_initial_rom2/alu.txt 2>/dev/null
	@cd ../.. && python3 tools/baseboard_rom_im.py verilog/verilator/obj_initial_rom2/rom.txt --alu-fixture verilog/verilator/obj_initial_rom2/alu.txt > verilog/verilator/obj_initial_rom2/im.txt
	@cd ../.. && ./dorado/build/boot0_hunks 'chm/dorado/AEmu.mb!2' -1 4 > verilog/verilator/obj_exec/exec.vec
	@cd ../.. && $(V) --binary --top-module tb_exec $(V_WARN) \
		-Iverilog/cells -Iverilog/generated --Mdir verilog/verilator/obj_initial_rom2 -o tb_initial \
		+define+WORLD +define+SCREEN +define+FULL \
		verilog/verilator/tb_exec.sv verilog/generated/dorado_full.v \
		verilog/generated/ContA-Rev-Cd.v verilog/generated/ContB-Rev-Cd.v \
		verilog/generated/ProcH-Rev-Ce.v verilog/generated/ProcL-Rev-Ci.v \
		verilog/generated/MemC-Rev-Be.v verilog/generated/MemD-Rev-Ca.v \
		verilog/generated/MemX-Rev-Ch.v verilog/generated/msa.v \
		verilog/generated/IFU-Rev-Ch.v verilog/generated/DispY-Rev-Cl.v verilog/generated/DskEth-Rev-Cf.v \
		verilog/verilator/dorado_pack.sv verilog/cells/dorado_trident.v >/dev/null
	@cd ../.. && ./verilog/verilator/obj_initial_rom2/tb_initial \
		+preload=verilog/verilator/obj_initial_rom2/im.txt +start=3907 \
		+cycles=$$(($(INITIAL_ROM_CYCLES) * $(SYSPER) / 4)) +mb0 +nostuck +terminal \
		+pendulum +hasram +ioreset +fastwait +imparityon +keepmirdebug \
		+cpu-manifold +paritysources +initial-rom-check $(INITIAL_ROM_ARGS)
