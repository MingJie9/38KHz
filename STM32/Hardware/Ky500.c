#include "stm32f10x.h"
#include "delay.h"
// 初始化TIM4 PWM
void TIM4_PWM_Init(void)
{
    uint16_t psc = 19 - 1;
    uint16_t arr = 100 - 1;  // PWM频率=72000/(psc+1)/(arr+1) = 38kHz

    // 启用TIM4和GPIOB时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置GPIOB6为复用推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 配置TIM4
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = arr / 2;  // 50%占空比
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM4, ENABLE);

    TIM_Cmd(TIM4, ENABLE);
    TIM_CtrlPWMOutputs(TIM4, ENABLE);
}

// 延迟函数（微秒级）
void Delay_us1(uint16_t us)
{
    TIM_SetCounter(TIM2, 0);
    TIM_Cmd(TIM2, ENABLE);
    while (TIM_GetCounter(TIM2) < us);
    TIM_Cmd(TIM2, DISABLE);
}

// 红外信号发送函数
void Send_logic_symbol_0(void)
{
    TIM_Cmd(TIM4, ENABLE);
    Delay_us(550);
    TIM_Cmd(TIM4, DISABLE);
    Delay_us(550);
}

void Send_logic_symbol_1(void)
{
    TIM_Cmd(TIM4, ENABLE);
    Delay_us(550);
    TIM_Cmd(TIM4, DISABLE);
    Delay_us(1680);
}

void Send_logic_symbol_boot(void)
{
    TIM_Cmd(TIM4, ENABLE);
    Delay_us(9000);
    TIM_Cmd(TIM4, DISABLE);
    Delay_us(4500);
}

void Send_logic_symbol_end(void)
{
    TIM_Cmd(TIM4, ENABLE);
    Delay_us(560);
    TIM_Cmd(TIM4, DISABLE);
    Delay_us(20000);
}

// 发送数据帧
void send_frame_buffer(uint8_t data)
{
    Send_logic_symbol_boot();

    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & 0x80)
            Send_logic_symbol_1();
        else
            Send_logic_symbol_0();
        data <<= 1;
    }

    Send_logic_symbol_end();
}




