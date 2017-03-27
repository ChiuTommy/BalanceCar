#include "time.h"

/*
1.��ʱ������
ʵ��delay_us��delay_ms
ʵ�ַ�ʽ��whileѭ��
�ӿڣ�delay_us��delay_ms�����յ������������������������������ת����ʹ�õ���ʱ������ȥ��

2.΢���ʱ��
���ڼ����������ʱ����
*/

//�ڲ���������
void Delay_Us(u32 myUs);
void Delay_Ms(u32 myMs);

//**************************************************************************
//                         ����ʱ����
//**************************************************************************
void delay_us(u32 us)
{
	Delay_Us(us);   
}

void delay_ms(u32 ms)
{
	Delay_Ms(ms);   
}



//**********************************************************************************
//                               whileѭ��ʵ��
//**********************************************************************************

void Delay_Us(u32 myUs)   
{
  u32 i;
  while(myUs--)
  {
    i=6;
    while(i--);
  }
}
 
void Delay_Ms(u32 myMs)
{
  u32 i;
  while(myMs--)
  {
    i=7200;
    while(i--);
  }
}

//*************************************************************************************
//	΢���ʱ�������ڼ��������ü��
//*************************************************************************************
void Time_Counter_Config(void)
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	/* ����TIM3ʱ������*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 1000-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3, DISABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	/*TIM3����ж�*/
	// �����ж���Դ
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 	
	// ������ռ���ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	 
	// ���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*��ʼ��ʱ*/
void COUNTER_START(void)
{
	TIM3 -> CNT = 0;
	TIM_Cmd(TIM3, ENABLE);
}

/*������ֵֹͣ��ʱ������ʱ��ֵ����λus*/
int ms;
u32 COUNTER_STOP(void)
{
	int countertime;
	countertime=(TIM3 -> CNT)+(1000*ms);
	TIM_Cmd(TIM3, DISABLE);
	ms=0;
	return countertime;
}

/*����жϷ�����*/
void TIM3_IRQHandler (void)
{
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);	
	ms++;
}


//***************************************************************************************************************
//The End

















////**********************************************************************************
////                                Timerʵ��
////**********************************************************************************
//void Time_NVIC_Configuration(void)
//{   
//	NVIC_InitTypeDef NVIC_InitStructure; 

//	/*TIM2����ж�*/
//	// �����ж���Դ
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 	
//	// ������ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
//	// ���������ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	  
//	
//	/*TIM4����ж�*/
//	// �����ж���Դ
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	
//	// ������ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
//	// ���������ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	  
//}

//void Time_Mode_Config(void)
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//	/* ���ü�ʱģʽ��ģʽ�Ļ�����ʱ��TIM2 */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//	TIM_TimeBaseStructure.TIM_Period = 1000-1;
//	TIM_TimeBaseStructure.TIM_Prescaler = 720-1;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//	TIM_Cmd(TIM2, ENABLE);
//	
//	/* ������ʱ��ʱ��TIM4 */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//	TIM_TimeBaseStructure.TIM_Period = 50000-1;
//	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//	TIM_ARRPreloadConfig(TIM4,DISABLE);
//	
//	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
//	TIM_Cmd(TIM4, DISABLE);
//}

//void Time_Configuration(void)
//{
//	Time_NVIC_Configuration();
//	Time_Mode_Config();
//}

///*��ʱ����*/
//uint16_t go_onflag;
//void Delay_us(u32 us)
//{   
//	TIM_SetCounter(TIM4,0);
//	TIM_SetAutoreload(TIM4,us);
//	TIM_Cmd(TIM4, ENABLE);
//	while(go_onflag==0);
//	TIM_Cmd(TIM4, DISABLE);
//	go_onflag=0;
//}

//void TIM4_IRQHandler (void)
//{
//	TIM_ClearFlag(TIM4, TIM_FLAG_Update);	
//	go_onflag=1;
//}

///*��ȡ����ʱ��(��ȷ��10us)*/
//uint16_t  Rrepeati;
//uint16_t  Rrepeatj;
//u32 readnowtime(void)    
//{  
//	return Rrepeati*10000000+Rrepeatj*10000+TIM_GetCounter(TIM2)*10;
//}

//void TIM2_IRQHandler (void)
//{   
//	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
//	if(Rrepeatj==1000)
//	{  
//		Rrepeatj=0;
//		Rrepeati++;
//	}
//	else
//		Rrepeatj++;
//} 
