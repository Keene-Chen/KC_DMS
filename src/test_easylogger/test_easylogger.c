#define LOG_TAG "main1"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "elog.h"

void test_elog_1(void)
{
    uint8_t buf[128] = { 0 };

    for (int i = 0; i < sizeof(buf); i++) {
        buf[i] = i;
    }

    /* test log output for all level */
    log_a("Hello EasyLogger!");
    log_e("Hello EasyLogger!");
    log_w("Hello EasyLogger!");
    log_i("Hello EasyLogger!");
    log_d("Hello EasyLogger!");
    log_v("Hello EasyLogger!");
    // elog_raw("Hello EasyLogger!");
    elog_hexdump("test", 16, buf, sizeof(buf));
    // sleep(3);
}
void test_elog_2(void)
{
    elog_assert(LOG_TAG, "Hello KeeneChen");
    elog_a(LOG_TAG, "Hello KeeneChen");
    log_a("Hello KeeneChen");

    elog_error(LOG_TAG, "Hello KeeneChen");
    elog_e(LOG_TAG, "Hello KeeneChen");
    log_e("Hello KeeneChen");

    elog_warn(LOG_TAG, "Hello KeeneChen");
    elog_w(LOG_TAG, "Hello KeeneChen");
    log_w("Hello KeeneChen");

    elog_info(LOG_TAG, "Hello KeeneChen");
    elog_i(LOG_TAG, "Hello KeeneChen");
    log_i("Hello KeeneChen");

    elog_debug(LOG_TAG, "Hello KeeneChen");
    elog_d(LOG_TAG, "Hello KeeneChen");
    log_d("Hello KeeneChen");

    elog_verbose(LOG_TAG, "Hello KeeneChen");
    elog_v(LOG_TAG, "Hello KeeneChen");
    log_v("Hello KeeneChen");
}

int main(void)
{
    /* close printf buffer */
    setbuf(stdout, NULL);

    /* initialize EasyLogger */
    // elog_file_init();
    elog_init();

    /* set EasyLogger log format */
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
#ifdef ELOG_COLOR_ENABLE
    elog_set_text_color_enabled(true);
#endif
    /* start EasyLogger */
    elog_start();

    /* dynamic set enable or disable for output logs (true or false) */
    // elog_set_output_enabled(false);
    /* dynamic set output logs's level (from ELOG_LVL_ASSERT to ELOG_LVL_VERBOSE) */
    // elog_set_filter_lvl(ELOG_FILTER_LVL_ALL);
    /* dynamic set output logs's filter for tag */
    elog_set_filter_tag("main1");
    /* dynamic set output logs's filter for keyword */
    //    elog_set_filter_kw("Hello");
    /* dynamic set output logs's tag filter */
    //    elog_set_filter_tag_lvl("main", ELOG_LVL_WARN);

    /* test logger output */

    test_elog_1();
    test_elog_2();
    // test_elog_file();

    return EXIT_SUCCESS;
}

#if 0
void test_elog_file(void)
{
    ElogFileCfg log;
    log.name = "sad.txt";
    log.max_size = 500;
    log.max_rotate = 2;
    elog_file_config(&log);

    elog_file_write(log.name, log.max_size);
}
#endif
