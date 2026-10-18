CXX ?= c++
ROOT := ../../..
OUT ?= $(ROOT)/build/dorado-main-transport-test
.PHONY: test ddr-test
test:
	$(MAKE) -C ../server
	mkdir -p $(dir $(OUT))
	$(CXX) -O2 -Wall -Wextra -std=c++14 -Itests -I../server dorado_net.cpp dorado_ddr.cpp dorado_control_client.cpp tests/test_main_transport.cpp -o $(OUT)
	python3 tests/run.py $(ROOT)/build/dorado-server $(OUT) $(ROOT)
ddr-test:
	python3 tests/test_ddr_transport.py
