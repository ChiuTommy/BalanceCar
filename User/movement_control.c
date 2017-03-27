#include "movement_control.h"
#include "bsp_encoder.h"
#include "bsp_motor.h"
#include "stdlib.h"
#include "include.h"

//����������IO��PWM���
void Speed_OutPut(s32 Speed_Left,s32 Speed_Right)
{
	Left_Motor(Speed_Left);
	Right_Motor(Speed_Right);
}

/*
 * ֹͣ:IN1 0 IN2 0
 * ��ת:IN1 1 IN2 0
 * ��ת:IN1 0 IN2 1
 *
 * ���������������
 * ��ֱ����65mm
 * ������������390
 * ����תһȦ�����390������
*/
void Get_Speed(float * Speed_Left_CM_S,float * Speed_Right_CM_S,u32 T)
{
	s32 Lnumber,Rnumber;
	
	Lnumber = Get_Left_Speed();		//��ȡ������
	Rnumber = -Get_Right_Speed();	//��ȡ������
	
	*Speed_Left_CM_S  = (float)Lnumber / (float)T * 52.3598775f;		// (6.5*3.14159265 / 390) / ( T / 1000 ) = 52.3598775
	*Speed_Right_CM_S = (float)Rnumber / (float)T * 52.3598775f;
}

