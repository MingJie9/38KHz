/*
 * @Author: ming 1209932698@qq.com
 * @Date: 2024-07-30 14:50:47
 * @LastEditors: ming 1209932698@qq.com
 * @LastEditTime: 2024-07-30 16:47:36
 * @FilePath: \4-1 OLED显示屏\Hardware\DHT11.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include"LED.h"
#define DHT11_PIN GPIO_Pin_9
#define DHT11_PORT GPIOA

// 定义变量
uint8_t humidity;
uint8_t temperature;

// 系统时钟配置：配置 STM32 的系统时钟为 72MHz
void SystemInit2(void) {
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    while (!RCC_WaitForHSEStartUp());
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_2);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLK2Config(RCC_HCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08);
}

// GPIO 初始化：配置 PA8 为推挽输出模式
void GPIO_Config1(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = DHT11_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

// 设置 GPIO 引脚为输出模式
void DHT11_SetPinOutput(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = DHT11_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

// 设置 GPIO 引脚为输入模式
void DHT11_SetPinInput(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = DHT11_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}


// 读取 DHT11 的数据
uint8_t DHT11_ReadData(uint8_t *temperature, uint8_t *humidity) {
    uint8_t bits[5] = {0};
    uint8_t i, j;

    // 启动信号
    DHT11_SetPinOutput();
    GPIO_ResetBits(DHT11_PORT, DHT11_PIN);
    Delay_ms(18);
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);
    Delay_us(30);
    DHT11_SetPinInput();

    // 响应信号
    Delay_us(40);
    if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == Bit_RESET) {
        Delay_us(80);
        if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == Bit_SET) {
            Delay_us(80);
            LED2_OFF();

            // 读取 5 字节数据
            for (j = 0; j < 5; j++) {
                for (i = 0; i < 8; i++) {
                    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == Bit_RESET);
                    Delay_us(30);  // 微调此延时
                    if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == Bit_SET) {
                        bits[j] |= (1 << (7 - i));
                    }
                    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN) == Bit_SET);
                    //LED1_OFF();
                }
            }

            // 校验和
            if ((bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) {
                *humidity = bits[0];
                *temperature = bits[2];
                return 1; // 成功
            }
        }
    }
    return 0; // 失败
}


/*
int main(void) {
    uint8_t temperature, humidity;

    SystemInit1();
    GPIO_Config();

    while (1) {
        if (DHT11_ReadData(&temperature, &humidity)) {
            // 成功读取数据
        } else {
            // 读取数据失败
        }
        Delay_ms(2000); // 每隔 2 秒读取一次数据
    }
}
*/

