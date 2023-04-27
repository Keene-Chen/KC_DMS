/* Example: parse a simple configuration file */

#include "rxi_ini.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

int main(void)
{
    ini_t* config    = ini_load("./res/test.ini");
    const char* name = ini_get(config, "protocol", "version");
    if (name) {
        printf("name: %s\n", name);
    }
    ini_free(config);

    return 0;
}