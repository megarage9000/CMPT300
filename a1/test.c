#include <stdio.h>
#include "list.h"

int main() {
    List * list = List_create();
    int count = List_count(list);

    printf("List information:\n - Address: %p\n - Count: %d\n", (List *)list, count);
}