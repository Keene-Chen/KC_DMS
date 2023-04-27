#include <stdio.h>
#include <yyjson.h>

int main(int argc, char** argv)
{
    const char* json = "{\"name\":\"John\",\"age\":30,\"city\":\"New York\"}";

    yyjson_doc* doc = yyjson_read(json, strlen(json), 0);

    if (doc) {
        yyjson_val* name = yyjson_obj_get(doc->root, "name");
        yyjson_val* age  = yyjson_obj_get(doc->root, "age");
        yyjson_val* city = yyjson_obj_get(doc->root, "city");

        if (name && age && city) {
            printf("Name: %s\n", yyjson_get_str(name));
            printf("Age: %d\n", yyjson_get_int(age));
            printf("City: %s\n", yyjson_get_str(city));
        }

        yyjson_doc_free(doc);
    }

    return 0;
}
