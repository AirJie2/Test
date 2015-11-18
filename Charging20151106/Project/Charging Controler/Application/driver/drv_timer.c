#include "stm32f10x.h"
#include "user_lib.h"
#include "drv_led_relay.h"
#include "drv_timer.h"

void TIM2_Configuration(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  //GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIO����
  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//TIM1-CH1
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  TIM_DeInit(TIM2);
  
  TIM_TimeBaseStructure.TIM_Period = 2000;//1ms
  TIM_TimeBaseStructure.TIM_Prescaler = 35;//36��Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//72M
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  //TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  
  /* PWM1 ģʽ����: Channel3 Channel4*/
  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
  TIM_OCInitStructure.TIM_Pulse = 400;//400*g_InsProperty.uch_LCDLightGrade;//�ߵ�ƽ���,��ʱռ�ձ�
  //TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�������:TIM����Ƚϼ��Ը�
  TIM_OC4Init(TIM2, &TIM_OCInitStructure);//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);//ʹ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���
  TIM_ARRPreloadConfig(TIM2, ENABLE);//ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
  
  START_PWM();
  TIM_Cmd(TIM2, ENABLE);//ʹ��TIMx����
  
  
}
