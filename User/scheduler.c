#include "scheduler.h"
#include "attitude_computation.h"
#include "control.h"
#include "movement_control.h"
#include "stdio.h"
#include "time.h"
#include "data_transfer.h"

loop_t loop;

void Duty_1ms(void)
{
	
}

void Duty_2ms(void)
{
	u32 T = 2;
	Attitude_sensor_Read(T);	//��ȡ���ݣ���ͨ�˲�
}


//���̶�ȡ�ٶ�����	
//��������ȫ�֣����������̵߳���
float Speed_Left_CM_S = 0;
float Speed_Right_CM_S = 0;
float Desire_Angle = 0;
s16 mydirection = 0;
void Duty_5ms(void)
{
	u32 T = 5;
	s16 Balance_Out_Left,Balance_Out_Right;	//��̬PID���
//	s16 Speed_Out_Left,Speed_Out_Right;		//�ٶ�PID���
	s16 Out_Left,Out_Right;					//���������
	s16 direction = 0;
	
	
	//PID���
	Attitude_sensor_Update(T);							//��̬���ݸ���
	Get_Speed(&Speed_Left_CM_S,&Speed_Right_CM_S,T);	//��ȡ��ǰ�ٶ�
			
	//��ʧ��
	if(Angle.y > 15 || Angle.y < -15)
	{
		Out_Left = Out_Right = 0;
	}
	else
	{
		Balance_Control(Angle.y,Gyro.y,&Balance_Out_Left,&Balance_Out_Right,Desire_Angle);			//��̬ƽ��PID����
//		Speed_Control(Speed_Left_CM_S,Speed_Right_CM_S,&Speed_Out_Left,&Speed_Out_Right,0.0f);		//�ٶ�PID����
	
		direction = (Speed_Left_CM_S - Speed_Right_CM_S)/6 + mydirection;
	
		//��������ϣ����ǰת����������ϣ�����ת
		Out_Left  = Balance_Out_Left + direction;  //+ Speed_Out_Left;
		Out_Right = Balance_Out_Right - direction; //+ Speed_Out_Right;
	}
	
	Speed_OutPut(Out_Left,-Out_Right);	//�����������ֵ��ֵ�������������
//	Speed_OutPut(0,0);	//�����������ֵ��ֵ�������������

}

void Duty_10ms(void)
{
	dataupload();
}

void Duty_20ms(void)
{

}

void Duty_50ms(void)
{
	
}


//*************************************************************************************
//���к�������


//��ʼ������
void Loop_Init(void)
{
	loop.check_flag = 0;
	
	loop.cnt_2ms = 0;
	loop.cnt_5ms = 0;
	loop.cnt_10ms = 0;
	loop.cnt_20ms = 0;
	loop.cnt_50ms = 0;
	
	loop.error_flag = 0;
}

void Duty_Loop(void)
{
	if(loop.check_flag == 1)
	{
		Duty_1ms();							//����1ms������
		
		if(loop.cnt_2ms >= 2)
		{
			loop.cnt_2ms = 0;
			Duty_2ms();						//����2ms������
		}
		if( loop.cnt_5ms >= 5 )
		{
			loop.cnt_5ms = 0;
			Duty_5ms();						//����5ms������
		}
		if( loop.cnt_10ms >= 10 )
		{
			loop.cnt_10ms = 0;
			Duty_10ms();					//����10ms������
		}
		if( loop.cnt_20ms >= 20 )
		{
			loop.cnt_20ms = 0;
			Duty_20ms();					//����20ms������
		}
		if( loop.cnt_50ms >= 50 )
		{
			loop.cnt_50ms = 0;
			Duty_50ms();					//����50ms������
		}
		
		loop.check_flag = 0;	//�����������
	}
}

void Loop_check(void)  //TIME INTTERRUPT
{
	//���д����ʵ������ʡʱ�䣬�����ڶ�������������Ӧ�ñȽϿ�
	//���ʱ�䲻���˿��԰��ⲿ�ָĵ����б�����򵥵ö��д��
	//�õ�һ����������+ȡ�����ķ�������
	loop.cnt_2ms++;
	loop.cnt_5ms++;
	loop.cnt_10ms++;
	loop.cnt_20ms++;
	loop.cnt_50ms++;
	
	if(loop.check_flag == 1)	//���������check_flag�����ﻹ��1��û�б���0��
								//֤����ѭ������1ms������û�������꣬������check_flagû�����е�
	{
		loop.error_flag++;		//ÿ�γ������⣬error_flag+1
	}
	
	loop.check_flag = 1;
}
