#pragma once
// Call from user_io_poll(), after its core-type guard, while Main owns SPI.
void dorado_net_poll();
// Call from user_io_init(), including reloads with the same core name.
void dorado_net_reset();
