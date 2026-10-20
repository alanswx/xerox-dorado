CXX ?= c++
ROOT := ../../..
OUT ?= $(ROOT)/build/dorado-main-transport-test
.PHONY: test ddr-test control-test
test:
	$(MAKE) -C ../server
	mkdir -p $(dir $(OUT))
	$(CXX) -O2 -Wall -Wextra -std=c++14 -Itests -I../server dorado_net.cpp dorado_ddr.cpp dorado_control_client.cpp tests/test_main_transport.cpp -o $(OUT)
	python3 tests/run.py $(ROOT)/build/dorado-server $(OUT) $(ROOT)
ddr-test:
	python3 tests/test_ddr_transport.py
control-test:
	mkdir -p $(dir $(OUT))control-objects
	$(CC) -O2 -Wall -Wextra -Werror -std=c11 -I../server -c ../server/dorado_ddr_ring.c -o $(dir $(OUT))control-objects/ring.o
	$(CC) -O2 -Wall -Wextra -Werror -std=c11 -I../server -c ../server/dorado_ddr_control.c -o $(dir $(OUT))control-objects/control.o
	$(CXX) -O2 -Wall -Wextra -Werror -std=c++14 -pthread -I../server dorado_control_client.cpp tests/test_control_client.cpp $(dir $(OUT))control-objects/ring.o $(dir $(OUT))control-objects/control.o -o $(OUT)-control-test
	$(OUT)-control-test
