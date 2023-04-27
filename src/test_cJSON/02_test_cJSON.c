#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    // 解析JSON格式的数据
    const char* json_str = "{\"name\":\"Alice\", \"age\":20, \"score\":[80,90,95]}";
    cJSON* root          = cJSON_Parse(json_str);
    if (root == NULL) {
        printf("JSON解析失败：%s\n", cJSON_GetErrorPtr());
        exit(EXIT_FAILURE);
    }

    // 访问JSON数据
    cJSON* name  = cJSON_GetObjectItem(root, "name");
    cJSON* age   = cJSON_GetObjectItem(root, "age");
    cJSON* score = cJSON_GetObjectItem(root, "score");
    if (name != NULL) {
        printf("姓名：%s\n", name->valuestring);
    }
    if (age != NULL) {
        printf("年龄：%d\n", age->valueint);
    }
    if (score != NULL && cJSON_IsArray(score)) {
        printf("分数：");
        cJSON* item = NULL;
        cJSON_ArrayForEach(item, score)
        {
            printf("%d ", item->valueint);
        }
        printf("\n");
    }

    // 生成JSON格式的数据
    cJSON* new_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(new_obj, "name", "Bob");
    cJSON_AddNumberToObject(new_obj, "age", 22);
    cJSON* new_score = cJSON_CreateIntArray((int[]){ 85, 92, 97 }, 3);
    cJSON_AddItemToObject(new_obj, "score", new_score);

    char* new_json_str = cJSON_Print(new_obj);
    printf("生成的JSON格式数据：%s\n", new_json_str);

    // 释放资源
    cJSON_Delete(root);
    cJSON_Delete(new_obj);
    free(new_json_str);

    return 0;
}
