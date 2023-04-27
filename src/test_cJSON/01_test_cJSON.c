/**
 * Author:   KeeneChen
 * DateTime: 2022.07.08-16:22:47
 * Description: 构造json数据
 * Command: gcc construct_data.c -lcjson -lcjson_utils -o construct_data
 */

#include "cJSON.h"
#include "cJSON_Utils.h"
#include <stdio.h>
#include <stdlib.h>

cJSON* create_json(void);
void show(cJSON* item);
void export_file(cJSON* item);

int main(void)
{
    show(create_json());
    export_file(create_json());

    return 0;
}

/**
 * @brief  构造json数据
 * @param  void
 * @return cJSON* 对象
 */
cJSON* create_json(void)
{
    cJSON* item    = cJSON_CreateObject();
    cJSON* student = cJSON_CreateArray();
    cJSON* teacher = cJSON_CreateArray();

    // 构造student对象
    // 创建一个JSON数据对象
    cJSON* stu_item1 = cJSON_CreateObject();
    // 添加一条字符串类型的JSON数据
    cJSON_AddStringToObject(stu_item1, "id", "3320190993209");
    cJSON_AddStringToObject(stu_item1, "name", "zhangsan");
    cJSON_AddStringToObject(stu_item1, "class", "IOT-19-2");
    cJSON_AddStringToObject(stu_item1, "depart", "计算机与软件工程学院");
    // 添加一条浮点类型的JSON数据
    cJSON_AddNumberToObject(stu_item1, "age", 18);
    // 添加一个嵌套的JSON数据
    cJSON* address = cJSON_CreateObject();
    cJSON_AddStringToObject(address, "country", "China");
    cJSON_AddStringToObject(address, "province", "Szechwan");
    // 向对象中添加子项
    cJSON_AddItemToObject(stu_item1, "address", address);
    // 添加数组对象
    cJSON* skill = cJSON_CreateArray();
    cJSON_AddItemToArray(skill, cJSON_CreateString("C"));
    cJSON_AddItemToArray(skill, cJSON_CreateString("Java"));
    cJSON_AddItemToArray(skill, cJSON_CreateString("JavaScript"));
    cJSON_AddItemToObject(stu_item1, "skill", skill);

    // 添加一条浮点类型的JSON数据
    cJSON_AddNumberToObject(stu_item1, "score", 100.2);
    // 添加一条bool类型的JSON数据
    cJSON_AddFalseToObject(stu_item1, "graduate");

    // 构造teacher对象
    cJSON* tea_item1 = cJSON_CreateObject();
    cJSON_AddStringToObject(tea_item1, "id", "2220190993209");
    cJSON_AddStringToObject(tea_item1, "name", "wangwu");
    cJSON_AddStringToObject(tea_item1, "depart", "计算机与软件工程学院");
    cJSON_AddNumberToObject(tea_item1, "age", 34);
    cJSON_AddStringToObject(tea_item1, "title", "教授");
    cJSON_AddNumberToObject(tea_item1, "grade", 99.8);
    cJSON_AddFalseToObject(tea_item1, "gender");
    // 排序数组
    // cJSONUtils_SortObject(stu_item1);
    // 向stu/tea数组添加元素
    cJSON_AddItemToArray(student, stu_item1);
    cJSON_AddItemToArray(teacher, tea_item1);
    // 向顶层对象item添加元素
    cJSON_AddItemToObject(item, "student", student);
    cJSON_AddItemToObject(item, "teacher", teacher);

    return item;
}

void show(cJSON* item)
{
    printf("%s\n", cJSON_Print(item));
    // 删除对象
    cJSON_Delete(item);
}

void export_file(cJSON* item)
{
    FILE* fp = fopen("./res/export.json", "w+");
    if (fp == NULL) {
        printf("open file error\n");
        return exit(EXIT_FAILURE);
    }
    fputs(cJSON_Print(item), fp);
    fclose(fp);
    cJSON_Delete(item);
}