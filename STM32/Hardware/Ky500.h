#ifndef __KY500_H
#define __KY500_H

void send_frame_buffer(uint8_t data);
void Send_logic_symbol_end(void);
void Send_logic_symbol_boot(void);
void Send_logic_symbol_1(void);
void Send_logic_symbol_0(void);
void Delay_us1(uint16_t us);
void TIM4_PWM_Init(void);	

#endif
