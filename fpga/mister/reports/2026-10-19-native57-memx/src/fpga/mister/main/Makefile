CXX ?= c++
ROOT := ../../..
OUT ?= $(ROOT)/build/dorado-main-transport-test
.PHONY: test
test:
	$(MAKE) -C ../server
	mkdir -p $(dir $(OUT))
	$(CXX) -O2 -Wall -Wextra -std=c++14 -Itests dorado_net.cpp tests/test_main_transport.cpp -o $(OUT)
	python3 tests/run.py $(ROOT)/build/dorado-server $(OUT) $(ROOT)
