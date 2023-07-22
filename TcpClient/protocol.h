#ifndef PROTOCOL_H
#define PROTOCOL_H

#include<stdlib.h>
#include<unistd.h>
#include<string.h>

typedef unsigned int uint;

struct PDU
{
    uint uiPDULen;         //总的协议数据单元大小
    uint uiMsgType;        //消息类型
    char caData[64];
    uint uiMsgLen;         //实际消息长度
    int caMsg[];           //实际消息
};

PDU *mkPDU(uint uiMsgLen)
{
    uint uiPDULen=sizeof(PDU)+uiMsgLen;
    PDU *pdu=(PDU*)malloc(uiPDULen);    //bug
    if(NULL==pdu){
        exit(EXIT_FAILURE);
    }
    memset(pdu,0,uiPDULen);
    pdu->uiPDULen=uiPDULen;
    pdu->uiMsgLen=uiMsgLen;
    return pdu;
}

#endif // PROTOCOL_H
