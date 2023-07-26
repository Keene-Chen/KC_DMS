#ifndef __KC_DMS_VERSION_H__
#define __KC_DMS_VERSION_H__

/* 版本信息 */
#define KCDMS_MAJOR  1
#define KCDMS_MINJOR 0
#define KCDMS_FIX    1
#define prompt(maj, min, fix)                                                                    \
    printf("\n\033[1;35m ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓ \033[1;0m\n");           \
    printf("\033[1;35m ┃ KC_DMS                                     ┃ \033[1;0m\n");             \
    printf("\033[1;35m ┃ Author : KeeneChen                         ┃ \033[1;0m\n");             \
    printf("\033[1;35m ┃ Version: v%d.%d.%d                            ┃ \033[1;0m\n", maj, min, \
           fix);                                                                                 \
    printf("\033[1;35m ┃ GitHub : https://github.com/Keene-Chen     ┃ \033[1;0m\n");             \
    printf("\033[1;35m ┃ Data monitoring system for embedded Linux  ┃ \033[1;0m\n");             \
    printf("\033[1;35m ┃ and EMQX cloud platform                    ┃ \033[1;0m\n");             \
    printf("\033[1;35m ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛ \033[1;0m\n");

#endif // __KC_DMS_VERSION_H__