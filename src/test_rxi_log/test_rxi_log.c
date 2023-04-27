/**
 * @file    : test_rxi_log.c
 * @author  : KeeneChen
 * @date    : 2023.03.01-14:26:27
 * @details : test_rxi_log
 */

#include "rxi_log.h"
#include <stdio.h>

int main(void)
{
    const char* filepath = "./res/test.log";

    FILE* fp = fopen(filepath, "a+");
    if (fp == NULL) {
        printf("failed to open file\n");
        return -1;
    }

    log_add_fp(fp, LOG_INFO);

    log_set_quiet(false);

    log_trace("Hello %s", "world");
    log_debug("Hello %s", "world");
    log_error("Hello %s", "world");
    log_info("Hello %s", "world");
    log_fatal("Hello %s", "world");
    log_warn("Hello %s", "world");

    printf("log level string: %s\n", log_level_string(LOG_INFO));
    printf("log version: %s\n", LOG_VERSION);

    fclose(fp);

    return 0;
}