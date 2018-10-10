#include <8052.h>
#include <stdlib.h>
#include <stdio.h>


/*--------------------------------------------------------------------------------*/
//sdcc LCD.c && packihx LCD.ihx > LCD.hex

	#define OSC_FREQ (11059200UL)
	#define OSC_PER_INST (12)

	#define PRELOAD01  (65536 - (unsigned int)(OSC_FREQ / (OSC_PER_INST * 1063)))    
	#define PRELOAD01H (PRELOAD01 / 256)
	#define PRELOAD01L (PRELOAD01 % 256)

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

	/*΢�뼶��ʱN*11us*/

 //for 8051 11us or10.5us
 #if OSC_PER_INST == (12)
 void Delay_11us(void){
	__asm
	  nop 
	  nop 
	  nop 
	  nop
	  nop
	  __endasm;
		
	}

#endif
/*--------------------------------------------------------------------------------*/
	/*����*/

	/*�˿ڶ���
		PIN DESCRIPTION   
		P17 P16 P15 P14
 		|   |   |   |
 		\---\---\---\---P10        1  2  3  F1
 		\---\---\---\---P11        4  5  6  F2
 		\---\---\---\---P12        7  8  9  F3
 		\---\---\---\---P13        *  0  #  F4
	*/
/*	#define KeyPort (P1)

	#define key_state_0 (0)      //���尴��״̬������״̬����
	#define key_state_1 (1)
	#define key_state_2 (2)
	#define key_state_3 (3)

	#define NO_key        (0)

	//����ɨ�躯��
	unsigned char KeyBoardScan(void)
	{   
		unsigned char key_temp;
		unsigned char key_num = NO_key;
		   unsigned char key_status = key_state_0;         //����״̬

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
							key_num = '*';                  
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
/*----------------------------------------------------------------------------------*/
//LCD12864�������ȷ��ʽ
//1.Ӣ���ַ�ÿ��16��
//2.�����ַ�ÿ��8����(GBK����)
//3.ע��ո�ҲҪд�ϡ�
/*------------------------------------------------------------------------------------*/

#define	CS   (P2_7)//��4����  RS
#define SID  (P2_6)//��5����  RW
#define SCK  (P2_5)//��6����  E
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
 
	unsigned int Number=0xa7;
    unsigned char buf[30];
	unsigned char Menu_Status = 0;


/*--------------------------------------------------------------------------------*/
#define DHT11 (P2_0)

unsigned char  U8FLAG;
unsigned char  U8count,U8temp;
unsigned char  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
unsigned char  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
unsigned char  U8comdata;
void  DHT11_COM(void)
      {
     
	        unsigned char i;
          
       for(i=0;i<8;i++)	   
	    {
		
	   	    U8FLAG=2;	
	   	while((!DHT11)&&U8FLAG++);
			Delay_11us();
		    Delay_11us();
			Delay_11us();
	  		U8temp=0;
	     if(DHT11)U8temp=1;
		    U8FLAG=2;
		 while((DHT11)&&U8FLAG++);
	   	//��ʱ������forѭ��		  
	   	 if(U8FLAG==1)break;
	   	//�ж�����λ��0����1	 
	   	   
		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 
	   	 
		   U8comdata<<=1;
	   	   U8comdata|=U8temp;        //0
	     }//rof
	   
	}

	//--------------------------------
	//-----ʪ�ȶ�ȡ�ӳ��� ------------
	//--------------------------------
	//----���±�����Ϊȫ�ֱ���--------
	//----�¶ȸ�8λ== U8T_data_H------
	//----�¶ȵ�8λ== U8T_data_L------
	//----ʪ�ȸ�8λ== U8RH_data_H-----
	//----ʪ�ȵ�8λ== U8RH_data_L-----
	//----У�� 8λ == U8checkdata-----
	//----��������ӳ�������----------
	//---- Delay();, Delay_10us();,DHT11_COM(); 
	//--------------------------------

	void DHT11_Read(void)
	{
	  //��������18ms 
       DHT11=0;
	  Hardware_Delay_T0(18);
	   DHT11=1;
	 //������������������ ������ʱ20us
	   Delay_11us();
	   Delay_11us();
	 //������Ϊ���� �жϴӻ���Ӧ�ź� 
	   DHT11=1;
	 //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
	   if(!DHT11)		 //T !	  
	   {
	   U8FLAG=2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
	   while((!DHT11)&&U8FLAG++);
	   U8FLAG=2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
	   while((DHT11)&&U8FLAG++);
	 //���ݽ���״̬		 
	   DHT11_COM();
	   U8RH_data_H_temp=U8comdata;
	   DHT11_COM();
	   U8RH_data_L_temp=U8comdata*10;
	   DHT11_COM();
	   U8T_data_H_temp=U8comdata;
	   DHT11_COM();
	   U8T_data_L_temp=U8comdata*10;
	   DHT11_COM();
	   U8checkdata_temp=U8comdata;
	   DHT11=1;
	 //����У�� 
	 
	   U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
	   if(U8temp==U8checkdata_temp)
	   {
	   	  U8RH_data_H=U8RH_data_H_temp;
	   	  U8RH_data_L=U8RH_data_L_temp;
		  U8T_data_H=U8T_data_H_temp;
	   	  U8T_data_L=U8T_data_L_temp;
	   	  U8checkdata=U8checkdata_temp;
	   }//fi
	   }//fi

	}


/*--------------------------------------------------------------------------------*/
//�ö�ʱ���ж�д�� UART 
//������Ϊ9600��9600����Ԫ/�룩����ʱ��Ƶ��Ϊ11.0592MHzʱ
//����Ҫ��ָ������s=(1000000/9600)/(12/11.0592)=96��
#define RXD (P1_0)
#define TXD (P1_1)

unsigned char UART_Flag;
//���������жϳ�ʼ��

//��ѯ��־λ

void WaitTF0(void){
	while(!UART_Flag);
	UART_Flag=0; //�����־λ
}


void UART_timer_init(void){
	TMOD=0x02;		//������0����ʽ2��8λ�Զ����أ�
	TH0=0xa0;		//ԤֵΪ256-96=140
	TL0=TH0;		//�Զ�����
	TR0=0;			//timer run Ϊ0��ʾ��ʱ�رմ˶�ʱ������Ҫʱ����1
	TF0=0;			//timer flow ��ʱ�������־	

	ET0=1;			//����ʱ��0�ж�
	EA=1;			//�ж������ܿ���
}
//��������
unsigned char UART_RXByte(){
	unsigned char data=0;
	unsigned char i=8;
	TR0=1;
	WaitTF0();//�ȹ���ʼλ
	//����8λ����λ
	while(i--){
		data = data<<1;
		if(RXD)
			data|=0x80;//���յ�λ
		WaitTF0();	//λ����ʱ
	}
/*	
	for(i=0;i<8;i++){
		data |= (unsigned char)RXD;
		WaitTF0();	//λ����ʱ
		x<<=1;
	}
*/
	WaitTF0();
	TR0=0; //ֹͣ��ʱ��0
	return data;
}

//��������

void UART_TXByte(unsigned char data)
{
	unsigned char i=9;
	TR0=1;
	TXD = 0;//������ʼλ
	WaitTF0();
	//����8λ����λ
	while(i--){
		TXD=(__bit)(data & 0x01);//�ȴ���λ
		WaitTF0();	//λ����ʱ
		data=data>>1;
	}
	//����У��λ(��)
	TXD=1;//���ͽ���
	WaitTF0();
	TR0=0; //ֹͣ��ʱ��0
}


void IntTimer0() __interrupt  1
{
	UART_Flag=1; //���ñ�־λ
}





/*--------------------------------------------------------------------------------*/

void main( void )
{     
     
	
	/***********************************************************************************/


	unsigned char i=0;
	 LcmInit();
     LcmClearTXT();
		 while(1)
     {
		 
		 //ReadTempAndHumi(void);
		DHT11_Read();
		Number+=0.01;
       	PutStr(0,0,"--- ������ ---");
        sprintf(buf, "Hu:%d%% Te:%d��",U8RH_data_H,U8T_data_H);
		PutStr(1,0,buf);
		sprintf(buf, "Img:%c %d %hx   ",i,i,i,i);
		PutStr(2,0,buf);

		sprintf(buf, "                ");
		PutStr(3,0,buf);
		Hardware_Delay_T0(10);
		i++;
		if(i==255)
		i=0;
		unsigned char KKl=i;
		UART_timer_init();
		UART_TXByte(KKl);
		
		/*
		PutStr(0,0,"���ܴ�����ϵͳ");
		sprintf(buf,"Te: %d           ",temp_value);
		PutStr(1,0,buf);
		sprintf(buf,"Hu: %d           ",humi_value);
		PutStr(2,0,buf);
		sprintf(buf,"CO2: 0.0003%d  ",rand()%10);
		PutStr(3,0,buf);
		
		rand()%10;
		PutStr(0,0,"���ܴ������ϵͳ");
		PutStr(1,0,"�¶ȣ�24.5 'C ");
		PutStr(2,0,"���գ�2346 Lux");
		PutStr(3,0,"ʪ�ȣ�64.8 %");
     */
		//Hardware_Delay_T0(1000);
	 }
	
	 
    //unsigned int Menu_Status = 0;

}