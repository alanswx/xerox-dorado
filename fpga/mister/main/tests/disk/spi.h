#pragma once
#include <cstdint>
void EnableIO();
void DisableIO();
uint8_t spi8(uint8_t);
uint16_t spi_w(uint16_t);
uint32_t spi32_w(uint32_t);
void spi32_b(uint32_t);
uint8_t spi_uio_cmd8(uint8_t,uint8_t);
void spi_block_read(uint8_t *,int,int);
void spi_block_write(const uint8_t *,int,int);
