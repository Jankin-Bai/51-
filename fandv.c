#include <8052.h>
#include <stdlib.h>
#include <stdio.h>
void _nop_(void);
__sfr __at (0xBC) ADC_CONTR; //A/D ת�����ƼĴ��� ADC_POWER SPEED1 SPEED0 ADC_FLAG ADC_START CHS2 CHS1 CHS0 0000,0000
__sfr __at (0xBD) ADC_RES ;  //A/D ת�������8λ ADCV.9 ADCV.8 ADCV.7 ADCV.6 ADCV.5 ADCV.4 ADCV.3 ADCV.2  0000,0000
__sfr __at (0xBE) ADC_RESL;  //A/D ת�������2λ                                           ADCV.1 ADCV.0  0000,0000


//                                      7     6     5     4     3     2     1     0         Reset Value
__sfr __at (0x80) P0; //8 bitPort0          P0.7  P0.6  P0.5  P0.4  P0.3  P0.2  P0.1  P0.0       1111,1111
__sfr __at (0x94) P0M0;//                                                                         0000,0000
__sfr __at (0x93) P0M1;//                                                                         0000,0000
__sfr __at (0x90) P1; //8 bitPort1          P1.7  P1.6  P1.5  P1.4  P1.3  P1.2  P1.1  P1.0       1111,1111
__sfr __at (0x92) P1M0;//                                                                         0000,0000
__sfr __at (0x91) P1M1;//                                                                         0000,0000
__sfr __at (0x9D) P1ASF;//P1 analog special function
__sfr __at (0xA0) P2;//8 bitPort2          P2.7  P2.6  P2.5  P2.4  P2.3  P2.2  P2.1  P2.0       1111,1111
__sfr __at (0x96) P2M0;//                                                                         0000,0000
__sfr __at (0x95) P2M1;//                                                                         0000,0000
__sfr __at (0xB0) P3;//8 bitPort3          P3.7  P3.6  P3.5  P3.4  P3.3  P3.2  P3.1  P3.0       1111,1111
__sfr __at (0xB2) P3M0;//                                                                         0000,0000
__sfr __at (0xB1) P3M1;//                                                                         0000,0000
__sfr __at (0xC0) P4;//8 bitPort4          P4.7  P4.6  P4.5  P4.4  P4.3  P4.2  P4.1  P4.0       1111,1111
__sfr __at (0xB4) P4M0;//                                                                         0000,0000
__sfr __at (0xB3) P4M1;//                                                                         0000,0000
//                                      7      6         5         4      3     2     1     0     Reset Value
__sfr __at (0xBB) P4SW;//Port-4 switch     -   LVD_P4.6  ALE_P4.5  NA_P4.4   -     -     -     -     x000,xxxx

__sfr __at (0xC8) P5;//8 bitPort5           -     -       -      -    P5.3  P5.2  P5.1  P5.0    xxxx,1111
__sfr __at (0xCA) P5M0;//                                                                         0000,0000
__sfr __at (0xC9) P5M1;//                                                                         0000,0000
/*--------------------------------------------------------------------------------*/


	#define OSC_FREQ (11059200UL)
	#define OSC_PER_INST (1)

	#define PRELOAD01  (65536 - (unsigned int)(OSC_FREQ / (OSC_PER_INST * 1063)))    
	#define PRELOAD01H (PRELOAD01 / 256)
	#define PRELOAD01L (PRELOAD01 % 256)

#define u8  unsigned char
#define u16 unsigned short
#define u32 unsigned int

/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/

	/*Ӳ����ʱN ms*/
	void Hardware_Delay_T0(unsigned int N)   
	{
		unsigned int ms;
		         
		// Configure Timer 0 as a 16-bit timer 
		TMOD &= 0xF0; // Clear all T0 bits (T1 left unchanged)
		TMOD |= 0x01; // Set required T0 bits (T1 left unchanged) 

		ET0 = 0;  // No interupts

		// Delay value is *approximately* 1 ms per loop   
		for (ms = 0; ms < N; ms++)
		{                 
			TH0 = PRELOAD01H;  
			TL0 = PRELOAD01L;   
			TF0 = 0;          // clear overflow flag
			TR0 = 1;          // start timer 0

			while (TF0 == 0); // Loop until Timer 0 overflows (TF0 == 1)

			TR0 = 0;          // Stop Timer 0
		}
	}        
/*--------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------*/
	
	/*�˿ڶ���
		PIN DESCRIPTION   
		P17 P16 P15 P14
 		|   |   |   |
 		\---\---\---\---P10        1  2  3  F1
 		\---\---\---\---P11        4  5  6  F2
 		\---\---\---\---P12        7  8  9  F3
 		\---\---\---\---P13        *  0  #  F4
	*/
/*	#define KeyPort P1

	#define key_state_0 0      //���尴��״̬������״̬����
	#define key_state_1 1
	#define key_state_2 2
	#define key_state_3 3

	#define NO_key        0

	//����ɨ�躯��
	unsigned char KeyBoardScan(void)
	{   
		unsigned char key_temp;
		unsigned char key_num = NO_key;
		static unsigned char key_status = key_state_0;         //����״̬

		//�߷�ת��ɨ�����
		KeyPort = 0x0f;
		key_temp = KeyPort;
		KeyPort = 0xf0;
		key_temp |= KeyPort;
		KeyPort = 0xff;

	 switch(key_status){
		//״̬��Ϊ��ʼ״̬
		case key_state_0:                                    
			if(key_temp != 0xff)
				key_status = key_state_1;
		break;
					
		//״̬һΪ��������״̬
		case key_state_1:                                   
			if(key_temp == 0xff)
				key_status = key_state_0;             //��ֻ�Ƕ���������״̬��
			else{	key_status = key_state_2;            //��ȷʵ���£�����״̬��
				switch(key_temp){
					case 0x7e:        //0111 1110
						key_num = '1';		
						break;	
					case 0xbe:        //1011 1110	
						key_num = '2';	
						break;	
					case 0xde:        //1101 1110
							key_num = '3';
						break;
					case 0x7d:        //0111 1101
							key_num = '4';
						break;
					case 0xbd:        //1011 1101
							key_num = '5';
						break;
					case 0xdd:        //1101 1101
							key_num = '6';	
						break;
					case 0x7b:        //0111 1011
							key_num = '7';
						break;
					case 0xbb:        //1011 1011
							key_num = '8';
						break;
					case 0xdb:        //1101 1011
							key_num = '9';
						break;
					case 0xb7:        //1011 0111
							key_num = '0';
						break;
					case 0x77:        //0111 0111
							key_num = '*';                  //
						break;
					case 0xd7:        //1101 0111
							key_num = '#';                  //#
						break;
					case 0xee:        //1110 1110
							key_num = 'A';                  //F1
						break;
					case 0xed:        //1110 1101
							key_num = 'B';                  //F2
						break;
					case 0xeb:        //1110 1011
							key_num = 'C';                  //F3
						break;
					case 0xe7:        //1110 0111
							key_num = 'D';                  //F4
						break;
						}
				}
		break;

		//״̬����ʾȷʵ�а�������
		case key_state_2:
			if(key_temp == 0xff)
				key_status = key_state_3;	//����⵽���֣�����״̬��
		break;

		case key_state_3:
			if(key_temp == 0xff)
				key_status = key_state_0;	//���ж�ȷʵ���ֺ󣬵��س�ʼ״̬��
			else
				key_status = key_state_2;	//��ֻ�Ƕ���������������֣�����״̬��
		break;
	 }		
	 return key_num;
	}

*/
/*--------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------
LCD�������ȷ��ʽ
1.Ӣ���ַ�ÿ��16��
2.�����ַ�ÿ��8����(GBK����)
3.ע��ո�ҲҪд�ϡ�
------------------------------------------------------------------------------------*/

#define	CS   P2_7//��4����  RS
#define SID  P2_6//��5����  RW
#define SCK  P2_5//��6����  E
unsigned char const AC_TABLE[]={
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,    //��һ�к���λ��  
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,    //�ڶ��к���λ��  
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,    //�����к���λ�� 
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,    //�����к���λ�� 
};
/******************************** ��������    ��SendByte  ���ڷ���һ���ֽ�*****/
void SendByte(unsigned char Dbyte)
{
     unsigned char i;
     for(i=0;i<8;i++)
     {
           SCK = 0;
           Dbyte=Dbyte<<1;     
           SID = CY;           
           SCK = 1;
           SCK = 0;
     }
}
 
/***********����һ���ֽ�***/
unsigned char ReceiveByte(void)
{
     unsigned char i,temp1,temp2;
     temp1 = 0;
     temp2 = 0;
     for(i=0;i<8;i++)
     {
           temp1=temp1<<1;
           SCK = 0;
           SCK = 1;            
           SCK = 0;
           if(SID) temp1++;
     }
     for(i=0;i<8;i++)
     {
           temp2=temp2<<1;
           SCK = 0;
           SCK = 1;
           SCK = 0;
           if(SID) temp2++;
     }
     return ((0xf0&temp1)+(0x0f&temp2));
}
 
void CheckBusy( void )
{
     do   SendByte(0xfc);      
     while(0x80&ReceiveByte());    
}
 
void WriteCommand(unsigned char Cbyte )
{
     CS = 1;
     CheckBusy();
     SendByte(0xf8);            
     SendByte(0xf0&Cbyte);     
     SendByte(0xf0&Cbyte<<4);
     CS = 0;
}
 
void WriteData(unsigned char Dbyte )
{
     CS = 1;
     CheckBusy();
     SendByte(0xfa);            
     SendByte(0xf0&Dbyte);      
     SendByte(0xf0&Dbyte<<4);
     CS = 0;
}
 
unsigned char ReadData( void )
{
     CheckBusy();
     SendByte(0xfe);            
     return ReceiveByte();
}
 
 
void LcmInit( void )
{
     WriteCommand(0x30);      
     WriteCommand(0x03);      
     WriteCommand(0x0C);      
     WriteCommand(0x01);      
     WriteCommand(0x06);      
}
 
void LcmClearTXT( void )
{
     unsigned char i;
     WriteCommand(0x30);     
     WriteCommand(0x80);      
     for(i=0;i<64;i++)
     WriteData(0x20);
}
 
 
void PutStr(unsigned char row,unsigned char col,unsigned char *puts)
{
     WriteCommand(0x30);     
     WriteCommand(AC_TABLE[8*row+col]);   
     while(*puts != '\0')      
     {
           if(col==8)           
           {            
                 col='0';
                 row++;
           }
           if(row==4) row='0';     
           WriteCommand(AC_TABLE[8*row+col]);
           WriteData(*puts);      
           puts++;
           WriteData(*puts);
           puts++;
           col++;
     }
}
 
 
void DisplayDots(unsigned char DotByte)
{
     unsigned char i,j;
     WriteCommand(0x34);     
     WriteCommand(0x36);     
     for(i=0;i<32;i++)           
     {
           WriteCommand(0x80|i);     
           WriteCommand(0x80);     
           for(j=0;j<32;j++)     
           {            
              WriteData(DotByte);                  
           }
           DotByte=~DotByte;
     }
}
 

/*����12C��Ƭ����֧��AD��ؼĴ�����λѰַ,��ʹ�á�|���ԼĴ���ִ��λ,�ʶ���һ������*/

/*��|��˵��,֮�����ᵽXָδ֪��0x80=1000 0000B 0xXX | 0x80 = 1XXX XXXXB*/ /*��&��˵��,0xEF=0111 1111B 0xXX & 0xEF=0XXX XXXXB*/

#define ADC_POWER 0x80 //ADC power control bit

#define ADC_FLAG 0x10 //ADC complete flag

#define ADC_START 0x08 //ADC start control bit

#define ADC_SPEEDLL 0x00 //540 clocks

#define ADC_SPEEDL 0x20 //360 clocks

#define ADC_SPEEDH 0x40 //180 clocks

#define ADC_SPEEDHH 0x60 //90 clocks

//AD�ĳ�ʼ��

void InitADC(){
	P1ASF = 0xff; //Set all P1 as analog input port 0xff=1111 1111B ��P1ȫ������AD,ʹ��ʱ����ʵ�������ֵ
	ADC_RES = 0; //���ת������洢�Ĵ���
	ADC_RESL=0;
	ADC_CONTR = 0x00;
	_nop_;_nop_;_nop_;_nop_;//�ȴ�ADC_CONTRֵд��
}

//ADת��������дʱ,������ƺ�������ֵΪת�����,������������Ҫת�������š�ת������,���ȫ������ADת�����ʲ���,��ֱ�ӽ�speed��ֵ�ڳ�ʼ��ʱд��,

unsigned int GetADC(unsigned char ch,unsigned char speed){
		unsigned int res;
		ADC_CONTR =ADC_CONTR | ADC_POWER | speed | ADC_START | ch;
		_nop_;_nop_;_nop_;_nop_;//ȷ��ADC_CONTR��ֵд��

		while(!(ADC_CONTR & 0x10)); //���ADת��δ����FLAGλΪ0,�����ڴ˵ȴ�,���Ϊ1,����ѭ��
		res=ADC_RES*4+ADC_RESL; //��ADת�����,��ʽ�Լ����
		ADC_RES=0;
		ADC_RESL=0;
		ADC_CONTR=0; //�Ĵ�����λ
	return res;

}
#define N 5
unsigned char value_buff[N];
unsigned char i=0;
unsigned char filter()
{
     unsigned char count;
     unsigned int sum=0;
     value_buff[i++]=get_data();
     if(i==N)
         i=0;
for(count=0;count<N;count++)
       sum=value_buff[count];
return (unsigned char)(sum/N);
}

/*
unsigned short	Get_ADC10bitResult(unsigned char channel)	//channel = 0~7
{
	ADC_RES = 0;
	ADC_RESL = 0;

	ADC_CONTR = (ADC_CONTR & 0xe0) | 0x08 | channel; 	//start the ADC
	_nop_;_nop_;_nop_;_nop_;

	while((ADC_CONTR & 0x10) == 0)	;	//wait for ADC finish
	ADC_CONTR &= ~0x10;		//���ADC������־
	return	(((unsigned short)ADC_RES << 2) | (ADC_RESL & 3));			
}
*/

void main( void )
{     
    unsigned char buf[30];
	//LCD��ʼ��
	LcmInit();
    LcmClearTXT();
	//IO�ڳ�ʼ��
	P0M1 = 0;	P0M0 = 0;	//����Ϊ׼˫���
	P1M1 = 0;	P1M0 = 0;	//����Ϊ׼˫���
	P2M1 = 0;	P2M0 = 0;	//����Ϊ׼˫���
	P3M1 = 0;	P3M0 = 0;	//����Ϊ׼˫���
	P4M1 = 0;	P4M0 = 0;	//����Ϊ׼˫���
	P5M1 = 0;	P5M0 = 0;	//����Ϊ׼˫���
	
	//ADC��ʼ��
	P1M1 |= (1<<3);		// ��ADC������Ϊ��������
	P1M0 &= ~(1<<3);
	//P1ASF = (1<<3);		//P1.3��ADC
	//ADC_CONTR = 0xE0;	//90T, ADC power on
    InitADC();

	 while(1)
     {  
		
		unsigned char Fr;
	 	unsigned char V;
		
		/*
		unsigned char j;
		u16	msecond;

		u16	Bandgap;	//


		Fr=Get_ADC10bitResult(1);
	 	V=Get_ADC10bitResult(2);
		P1ASF = 0;
				Get_ADC10bitResult(0);	//�ı�P1ASF���ȶ�һ�β��������, ���ڲ��Ĳ������ݵĵ�ѹ��������ֵ.
				Bandgap = Get_ADC10bitResult(0);	//���ڲ���׼ADC, P1ASF=0, ��0ͨ��
				P1ASF = 1<<3;
				j = Get_ADC10bitResult(3);	//���ⲿ��ѹADC
				j = (u16)((u32)j * 123 / Bandgap);	//�����ⲿ��ѹ, BandgapΪ1.23V, ���ѹ�ֱ���0.01V
		*/

		Fr=GetADC(0,00);
		V=filter(GetADC(3,00));
		PutStr(0,0,"      f-V       ");
		sprintf(buf, " f = %d  ",Fr);
		PutStr(1,0,buf);
		sprintf(buf, " V = %d  ",V);
		PutStr(2,0,buf);
		PutStr(3,0,"****************");
		PutStr(0,0,"                ");		//����
		PutStr(1,0,"                ");		//����
		PutStr(2,0,"                ");		//����
		PutStr(3,0,"                ");		//����
	
     }
}
