#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

typedef struct PDU{
    int a;
    int b;
    int c;
    int d[];
}PDU;

int main()
{


    printf("%ld\n",sizeof(PDU));

    //空间申请
    PDU *pdu=(PDU*)malloc(sizeof(PDU)+100*sizeof (int));

    pdu->a=90;
    pdu->b=80;
    pdu->c=70;
    memcpy(pdu->d,"please persevere", 17); //   一定要比输入的数据长度大，否则会异常
    printf("a=%d,b=%d,c=%d,%s\n",pdu->a,pdu->b,pdu->c,(char*)pdu->d);

    free(pdu);
    pdu=NULL;
    printf("Hello World!\n");
    return 0;
}
