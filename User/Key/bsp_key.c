#include "bsp_key.h"

int run_control = 1;

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ����P[A|B|C|D|E]13Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void KEY_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PC13) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);
											
	/* config the NVIC(PC13) */
	NVIC_Configuration();

	/* EXTI line gpio config(PC13) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* EXTI line(PC13) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		if(run_control)
			run_control = 0;
		else
			run_control = 1;
		
		EXTI_ClearITPendingBit(EXTI_Line0);     //����жϱ�־λ
	}  
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line13) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{
		// LED1 ȡ��		
		if(run_control)
			run_control = 0;
		else
			run_control = 1;
		EXTI_ClearITPendingBit(EXTI_Line13);     //����жϱ�־λ
	}  
}




//static void NVIC_Configuration(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  /* �����ж�Դ */
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//}

//void EXTI_PA0_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure; 
//	EXTI_InitTypeDef EXTI_InitStructure;

//	/* config the extiline clock and AFIO clock */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
//											
//	/* config the NVIC */
//	NVIC_Configuration();

//	/* EXTI line gpio config*/	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	/* EXTI line mode config */
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 
//	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure); 
//}

//void EXTI0_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //ȷ���Ƿ������EXTI Line�ж�
//	{
//		if(run_control)
//			run_control = 0;
//		else
//			run_control = 1;
//		
//		EXTI_ClearITPendingBit(EXTI_Line0);     //����жϱ�־λ
//	}  
//}

