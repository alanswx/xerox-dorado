#pragma once
#define UIO_SECTOR_RD 0x17
#define UIO_SECTOR_WR 0x18
#define UIO_SET_SDSTAT 0x1c
#define UIO_SET_SDINFO 0x1d
extern const char *disk_test_core;
inline const char *user_io_get_core_name(int=0) { return disk_test_core; }
