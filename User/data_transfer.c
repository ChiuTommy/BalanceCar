#include "data_transfer.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "stdio.h"
#include "control.h"
#include "attitude_computation.h"
#include "scheduler.h"
#include "include.h"

int usartsendswitch = 1;

void Data_trans_Init(void)
{
	USART1_Config();
	USART2_Config();
}

/// �ض���c�⺯��printf��USART1/2
int fputc(int ch, FILE *f)
{
	if(usartsendswitch == 1)
	{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	}
	else
	{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART2, (uint8_t) ch);
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);			
	}
	
	return (ch);
}

/// �ض���c�⺯��scanf��USART1/2
int fgetc(FILE *f)
{
	if(usartsendswitch == 1)
	{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	}
	else
	{
		/* �ȴ�����2�������� */
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
	}

	return (int)USART_ReceiveData(USART1);
}

//**********************************************************************************************

void bluetooth_handle(u8 ch)
{
	static int counter = 0;
	static float receivetable[5] = {0,0,0,0,0};
	static int mode = 0;
	float number;

	if(ch == 'a')	//ԭʼ�Ƕ�΢��
	{
		angle0 += 0.4;
	}
	else if(ch == 'b')
	{
		angle0 -= 0.4;
	}
	else if(ch == 'c')	//���ƽǶ�����
	{
		mode = 1;
	}
	else if(ch == 'd')
	{
		mode = 2;
	}
	else if(ch == 'e')
	{
		
	}
	else if( (ch <= 0x39)  &&  (ch >= 0x30) )	//�����������
	{
		switch(counter)
		{
			case 0:
				receivetable[0] = (int)ch;
				counter = 1;
				break;
			case 1:
				receivetable[1] = ch;
				counter = 2;
				break;
			case 2:
				receivetable[2] = ch;
				counter = 3;	
				break;
			case 3:
				receivetable[3] = ch;
				counter = 4;	
				break;				
			case 4:
				receivetable[4] = ch;
				number = (receivetable[0]-0x30)*100 + (receivetable[1]-0x30)*10 + (receivetable[2]-0x30) + (receivetable[3]-0x30)*0.1 + (receivetable[4]-0x30)*0.01;
				
				if(mode == 1)
				{
					if(number >= 100)
					{
						number = number - 100;
						number = -number;
					}
//					Desire_Angle = -number;
					printf("Desire_Angle:%f\n",Desire_Angle);
					mode = 0;
				}
				if(mode == 2)
				{
					if(number >= 100)
					{
						number = number - 100;
						number = -number;
					}
					mydirection = mydirection + number;
					mode = 0;
				}
				counter = 0;
				break;
			default:
				break;
		}
	}
}

void dataupload(void)	//��10ms���߳���
{
	static int i = 0;
	i++;
	if(i>10)
	{
		i = 0;
		usartsendswitch = 2;	//1��ͨ������1���ͣ�2��ͨ������2����
								//����1������
								//����2������
//		printf("L:%f  	R:%f\n",Speed_Left_CM_S,Speed_Right_CM_S);
//		printf("%lf\n",Angle.y);
	}
}


//ģʽѡ�񣺷��� p��i��d, a  �Ƕ�, b ��ӡ��̬, c ��ӡPID
//���������ֺ󣬾Ϳ��Է�����ֵ����ֵҪ��XXX.XX������6.32Ӧ�÷��ͳ�00632
void Receive_Handle(u8 ch)		//�˺������ж��е���
{
	static int counter = 0;
	static float receivetable[5] = {0,0,0,0,0};
	static int transmode = 0;	//p---0		i---1		d---2
	float number;
	
	if(ch == 'p')
	{
		transmode = 0;
		counter = 0;
	}
	else if(ch == 'i')
	{
		transmode = 1;
		counter = 0;
	}
	else if(ch == 'd')
	{
		transmode = 2;
		counter = 0;
	}
	else if(ch == 'a')
	{
		transmode = 3;
		counter = 0;
	}
	else if( (ch <= 0x39)  &&  (ch >= 0x30) )	//�����������
	{
		switch(counter)
		{
			case 0:
				receivetable[0] = (int)ch;
				counter = 1;
				break;
			case 1:
				receivetable[1] = ch;
				counter = 2;
				break;
			case 2:
				receivetable[2] = ch;
				counter = 3;	
				break;
			case 3:
				receivetable[3] = ch;
				counter = 4;	
				break;				
			case 4:
				receivetable[4] = ch;
				number = (receivetable[0]-0x30)*100 + (receivetable[1]-0x30)*10 + (receivetable[2]-0x30) + (receivetable[3]-0x30)*0.1 + (receivetable[4]-0x30)*0.01;
				
				if(transmode == 0)
				{
					kpb = number;
				}
				else if(transmode == 1)
				{
					kib = number;
				}
				else if(transmode == 2)
				{
					kdb = number;
				}
				else if(transmode == 3)
				{
					angle0 = number;
				}
				
				counter = 0;	
				break;
			default:
				break;
		}
	}
}
