#include "MSG.h"

void MSG_init(MSG *msg)
{
    msg->msgType = CM_Init;
    msg->msgParam = VK_UNPRESS;
    msg->msgCount = 0;
}