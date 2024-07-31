#ifndef __DHT11_H
#define __DHT11_H

extern uint8_t temperature;
extern uint8_t humidity;
void SystemInit2(void);
void GPIO_Config1(void);
void DHT11_SetPinOutput(void);
void DHT11_SetPinInput(void);
uint8_t DHT11_ReadData(uint8_t *temperature, uint8_t *humidity);

#endif
