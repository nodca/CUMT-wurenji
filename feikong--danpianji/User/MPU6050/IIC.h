#ifndef _IIC_H
#define _IIC_H

#include <inttypes.h>

#define IIC_WR	0		/* Ð´¿ØÖÆbit */
#define IIC_RD	1		/* ¶Á¿ØÖÆbit */

void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint8_t _ucByte);
uint8_t IIC_Read_Byte(uint8_t ack);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
uint8_t IIC_CheckDevice(uint8_t _Address);
void IIC_GPIO_Init(void);

#endif
