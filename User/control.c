#include "control.h"
#include "include.h"

/*
 * ˵��
 * control.c�����ƽ��С����������������̬���ƴ����ļ�
 * ���ļ��ĵ��÷�ʽΪ��
 * 1.�������к���
 * 2.��extern�еı��������������
 * 3.������������ں�������ʱ����
 * 
 * ���ļ��еĺ���û���ϸ��ʹ��ָ�봫�����ݣ�Ҳû�ж�������Ŀ��ƶ����Դ���
 * ���еĴ��붼�������Եģ����Ի��ң�����ʽ������л������
 * 
*/

float kpb,kib,kdb;
float kps,kis,kds;

/*
 * ��̬�����ñ�����ʼ��
 * ��ʼ�����ݣ�pid�е� kp kd
 */
void Control_Init(void)
{
	//��̬PID
	kpb = 66;
	kib = 3;
	kdb = 3;
	
	//�ٶ�PID
	kps = 3.5;
	kis = 0;
	kds = 1;
}

/*
 * Ϊ�������㣬����������п���ʹ��float���������Ե���Ŀ��������ӦΪ s16���ͣ�ȡֵ��Χ+1000 ~ -1000
 *
 * ������� Angle_y 					float	y�����			-180�� ~ +180��
 *					Gyro_y						float	y����ٶ�		-2000��/s ~ +2000��/s
 * 					Expect_Angle_y 		float	y���������		-180�� ~  +180��
 * �������� Control_Out_Left	s16		�����ٶȿ�����  +1000 ~ -1000
 *          Control_Out_Right	s16		�����ٶȿ�����	+1000 ~ -1000
 *
 */
float error_sum = 0;
void Balance_Control(float Angle_y,float Gyro_y,s16 *Control_Out_Left,s16 *Control_Out_Right,float Expect_Angle_y)
{
	float error;
	
	s16 pid_out;
	
	error = Expect_Angle_y - Angle_y;	//ǰ�� -- �������� -- ��
	error_sum += error;
	error_sum = LIMIT(error_sum,-32767,32767);	//�޷�
	pid_out = kpb * error + error_sum * kib- kdb * Gyro_y;	//PID���ƣ����ڽ��ٶ��ǽǶȵ�һ�׵�������������pid��ʽ��d��Ҫ�󣬲��ó�dt��ֱ�ӳ�dt�����ˡ�
														//���ǰ����Gyro�Ǹ�����������Ҫ������������������ǰ���Ǹ���
	
//	pid_out = 0;
	
	//ϣ��������������������ǰ�㣩 -- �������		ϣ��ǰ��������������Ⱥ����� -- �������
	*Control_Out_Left = pid_out;
	*Control_Out_Right = pid_out;
}

/*
 * �ٶȿ��ƺ���
 * �������	Speed_Left				s16		�����ٶ�
 * 					Speed_Right				s16		�����ٶ�
 *					Expect_Speed			s16		�����ٶ�
 * ��������	Control_Out_Left	s16 *	�����ٶȿ�����	+1000 ~ -1000
 *					Control_Out_Right	s16 *	�����ٶȿ�����	+1000 ~ -1000
 *
*/
void Speed_Control(s16 Speed_Left,s16 Speed_Right,s16 *Control_Out_Left,s16 *Control_Out_Right,float Expect_Speed)
{
	float Lerror,Rerror;
	s16 LOut,ROut;
	
	Lerror = Speed_Left  - Expect_Speed;	//��ǰת��Speed_LeftΪ������errorΪ����
	Rerror = Speed_Right - Expect_Speed;	//��ǰת��Speed_RightΪ������errorΪ����
	
	LOut = (s16)(kps * Lerror);
	ROut = (s16)(kps * Rerror);
	
	*Control_Out_Left  = LOut;
	*Control_Out_Right = ROut;
}

//******************************************************************************************************************
//The End


///**************************************************************************
//�������ܣ�ֱ��PD����
//��ڲ������Ƕȡ����ٶ�
//����  ֵ��ֱ������PWM
//��    �ߣ�ƽ��С��֮��
//**************************************************************************/
//int balance(float Angle,float Gyro)
//{  
//   float Bias,kp=300,kd=1;
//	 int balance;
//	
//	//����Ǵ�������û�������е��᣿
//	 Bias=Angle-ZHONGZHI;       //===���ƽ��ĽǶ���ֵ �ͻ�е���
//	
//	//����PID���ƻ����Ǽ򵥰��������Ѷȶ�����̬������ٶȿ�������
//	 balance=kp*Bias+Gyro*kd;   //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
//	
//	 return balance;
//}

///**************************************************************************
//�������ܣ��ٶ�PI���� �޸�ǰ�������ٶȣ�����Target_Velocity�����磬�ĳ�60�ͱȽ�����
//��ڲ��������ֱ����������ֱ�����
//����  ֵ���ٶȿ���PWM
//��    �ߣ�ƽ��С��֮��
//**************************************************************************/
//int velocity(int encoder_left,int encoder_right)
//{  
//	static float Velocity,Encoder_Least,Encoder,Movement;
//	static float Encoder_Integral,Target_Velocity;
//	float kp=80,ki=0.4;
//	
//	//=============ң��ǰ�����˲���=======================// 
//	/*
//	1.����ǰ��������
//	*/

//	if(1==Flag_Qian)    	//ǰ��
//		Movement=Target_Velocity/Flag_sudu;	         //===ǰ����־λ��1 
//	else if(1==Flag_Hou)	//����
//		Movement=-Target_Velocity/Flag_sudu;         //===���˱�־λ��1
//	else  Movement=0;		//ͣ

//	//=============�ٶ�PI������=======================//	
//	Encoder_Least =(Encoder_Left+Encoder_Right)-0;                    //==��ȡ�����ٶ�ƫ��==
//																														// = �����ٶȣ����ұ�����֮�ͣ�- Ŀ���ٶȣ��˴�Ϊ�㣩 

//	//���ٶ�ƫ����е�ͨ�˲�������˲����˲��̶Ȼ�ͦ��İ���
//	//�Ҿ���������ٶ�ƫ�����ȷ��Ӧ�ý���ƽ��ǰ���ٶȣ���Ϊ���ұ�����ȡ���Ǻ�
//	Encoder *= 0.8;		                                                //===һ�׵�ͨ�˲���       
//	Encoder += Encoder_Least*0.2;	                                    //===һ�׵�ͨ�˲���    

//	//�ǵ���һ�仰��˵�ٶȿ��������������ܵÿ�ʱ����һ�㣬��Ҫ�ܵ��ٿ�㡣
//	Encoder_Integral +=Encoder;                                       //===���ֳ�λ�� ����ʱ�䣺10ms
//	Encoder_Integral=Encoder_Integral-Movement;                       //===����ң�������ݣ�����ǰ������
//	if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===�����޷�
//	if(Encoder_Integral<-10000)	Encoder_Integral=-10000;              //===�����޷�	
//	Velocity=Encoder*kp+Encoder_Integral*ki;                          //===�ٶȿ���	
//	if(Turn_Off(Angle_Balance,Voltage)==1||Flag_Stop==1)   Encoder_Integral=0;      //===����رպ��������
//	return Velocity;
//}

///**************************************************************************
//�������ܣ�ת�����  �޸�ת���ٶȣ����޸�Turn_Amplitude����
//��ڲ��������ֱ����������ֱ�������Z��������
//����  ֵ��ת�����PWM
//��    �ߣ�ƽ��С��֮��
//**************************************************************************/
//int turn(int encoder_left,int encoder_right,float gyro)//ת�����
//{
//	 static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;
//	  float Turn_Amplitude=88/Flag_sudu,Kp=42,Kd=0;     
//	  //=============ң��������ת����=======================//
//  	if(1==Flag_Left||1==Flag_Right)                      //��һ������Ҫ�Ǹ�����תǰ���ٶȵ����ٶȵ���ʼ�ٶȣ�����С������Ӧ��
//		{
//			if(++Turn_Count==1)
//			Encoder_temp=myabs(encoder_left+encoder_right);
//			Turn_Convert=50/Encoder_temp;
//			if(Turn_Convert<0.6)Turn_Convert=0.6;
//			if(Turn_Convert>3)Turn_Convert=3;
//		}	
//	  else
//		{
//			Turn_Convert=0.9;
//			Turn_Count=0;
//			Encoder_temp=0;
//		}			
//		if(1==Flag_Left)	           Turn_Target-=Turn_Convert;
//		else if(1==Flag_Right)	     Turn_Target+=Turn_Convert; 
//		else Turn_Target=0;
//	
//    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;    //===ת���ٶ��޷�
//	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
//		if(Flag_Qian==1||Flag_Hou==1)  Kd=0.5;        
//		else Kd=0;   //ת���ʱ��ȡ�������ǵľ��� �е�ģ��PID��˼��
//  	//=============ת��PD������=======================//
//		Turn=-Turn_Target*Kp -gyro*Kd;                 //===���Z�������ǽ���PD����
//	  return Turn;
//}
