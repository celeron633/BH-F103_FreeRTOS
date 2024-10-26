#include "MSG.h"

void MSG_Init(MSG *msg)
{
    msg->msgType = CM_Init;
    msg->msgParam = VK_UNPRESS;
    msg->msgCount = 0;
}