#pragma once
extern const char *test_core_name;
extern const char *test_original_core_name;
inline const char *user_io_get_core_name(int original=0) {
    return original ? test_original_core_name : test_core_name;
}
#define CoreName test_core_name
