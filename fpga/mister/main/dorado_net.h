#pragma once
// Call from user_io_poll(), after its core-type guard, while Main owns SPI.
void dorado_net_poll();
// Call from user_io_init(), including reloads with the same core name.
bool dorado_net_reset();
// Must succeed BEFORE FPGA programming/reset, Main reexec, or init.
bool dorado_net_barrier(const char *reason);
