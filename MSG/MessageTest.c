#include "MSG.h"
#include "Message.h"

#include <stdio.h>

int MessageTest_main()
{
    printf("TEST BEGIN\r\n");

    MQ_Init();
    struct Message m1 = {1, 1, 1};
    struct Message m2 = {2, 2, 2};
    MQ_SendMessage(&m1);
    MQ_SendMessage(&m2);

    MSG tmp;
    while (MQ_GetMessage(&tmp) != 0) {
        printf("%d\r\n", tmp.msgType);
    }

    printf("TEST END\r\n");

    return 0;
}