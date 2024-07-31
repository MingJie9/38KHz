/*
 * @Author: ming 1209932698@qq.com
 * @Date: 2023-10-19 11:35:34
 * @LastEditors: ming 1209932698@qq.com
 * @LastEditTime: 2024-07-28 22:38:35
 * @FilePath: \3-4 按键控制LED\User\main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Ky500.h"
uint8_t KeyNum;		//定义用于接收按键键码的变量
#define IR_GPIO_PORT GPIOA
#define IR_GPIO_PIN GPIO_Pin_5

void startCode()
	{
		GPIO_ResetBits(IR_GPIO_PORT, IR_GPIO_PIN);
    	Delay_us(9000);

    	// 4500us高电平
    	GPIO_SetBits(IR_GPIO_PORT, IR_GPIO_PIN);
    	Delay_us(4500);
	}
void listCode()
	{
		GPIO_ResetBits(IR_GPIO_PORT, IR_GPIO_PIN);
    	Delay_us(646);

    	// 4500us高电平
    	GPIO_SetBits(IR_GPIO_PORT, IR_GPIO_PIN);
    	Delay_us(20000);
	}	
void endCode()
	{
		GPIO_ResetBits(IR_GPIO_PORT, IR_GPIO_PIN);
    	Delay_us(646);

    	// 4500us高电平
    	GPIO_SetBits(IR_GPIO_PORT, IR_GPIO_PIN);
    	Delay_us(10000);
	}
void OneCode()
	{
		GPIO_ResetBits(IR_GPIO_PORT, IR_GPIO_PIN);
    	Delay_us(9000);

    	// 4500us高电平
    	GPIO_SetBits(IR_GPIO_PORT, IR_GPIO_PIN);
    	Delay_us(4500);
			
	}	

int main(void)
{
	/*模块初始化*/
	LED_Init();		//LED初始化
	Key_Init();		//按键初始化
	LED1_ON();
	LED2_ON();
	SystemInit();
  TIM4_PWM_Init();
	// 假设已经初始化TIM2用于Delay_us函数

    uint8_t data = 0x55;  // 要发送的数据


		
	//GPIO_SetBits(IR_GPIO_PORT, IR_GPIO_PIN);
	while (1)
	{
		//Delay_ms(1000);
		//IR_SendPulse(640, 2000);
		//Delay_ms(1000);
		//LED1_OFF();
		//Delay_ms(1000);
		//LED1_ON();
		LED1_OFF();
		LED2_OFF();
		//send_frame_buffer(data);
		Send_logic_symbol_0();
        Delay_us(40000);  // 40ms延迟，防止发送过于频繁

    


		
		

	}
}
