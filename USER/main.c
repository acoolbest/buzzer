#if 0
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
//ALIENTEK Mini STM32开发板范例代码8
//PWM输出实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司

//7个音符组成了美妙的音乐
//低	 Do re mi fa so la si do
int melody[] = {NOTE_C4, NOTE_D4,NOTE_E4, NOTE_F4, NOTE_G4,NOTE_A4,NOTE_B4,NOTE_C5};
//中	 Do re mi fa so la si do
int melody2[] = {NOTE_C5, NOTE_D5,NOTE_E5, NOTE_F5, NOTE_G5,NOTE_A5,NOTE_B5,NOTE_C6};
//高
int melody3[] = {NOTE_C6, NOTE_D6,NOTE_E6, NOTE_F6, NOTE_G6,NOTE_A6,NOTE_B6,NOTE_C7};
//PWM输出口
//PwmOut m(PB_13);

int main(void)
{	
	u16 led0pwmval=0;    
	u8 dir=1;	
	delay_init();	    	 //延时函数初始化	  
	//LED_Init();		  	//初始化与LED连接的硬件接口
	//TIM1_PWM_Init(899,0);//不分频。PWM频率=72000/(899+1)=80Khz 
	TIM4_PWM_Init(1799,0);
	while(1)
	{
		delay_ms(10);	 
		if(dir)led0pwmval++;
		else led0pwmval--;	 
		if(led0pwmval>300)dir=0;
		if(led0pwmval==0)dir=1;	   					 
		//TIM_SetCompare1(TIM1,led0pwmval);
		TIM_SetCompare3(TIM4,led0pwmval);
	} 
}
#endif
#if 0
#include "delay.h"
#include "sys.h"
#include "pwm.h"

#define Sound1 2000            //音符1
#define Sound2 2500          //音符2
#define Sound3 3000          //音符3

#define time1 100            //蜂鸣器通电时间
#define time2 300            //蜂鸣器断电时间

ErrorStatus HSEStartUpStatus;

void sound1 ( void )
{
    unsigned int i = 500;
    while ( i-- ){
         
        GPIO_SetBits ( GPIOB, 5 );
        delay_ms ( i );
        GPIO_ResetBits ( GPIOB, 5 );
        delay_ms ( i-- );
    }
     
}
 
void sound2 ( void ){
     
    unsigned int i = 300;
    while ( i-- ){
         
        GPIO_SetBits ( GPIOB, 5 );
        delay_ms ( i );
        GPIO_ResetBits ( GPIOB, 5 );
        delay_ms ( i-- );
    }
     
}

int main(void)
{
	delay_init();                //延时函数初始化
	GPIO_Configuration();    //GPIO初始化
	
	//SoundTimerInit(Sound1,ENABLE);                 //定时器输出pwm.  蜂鸣器发出音符1
	//GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_SET);      //PD_15输出1.蜂鸣器开启
	while(1)
	{  
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);      //PD_15输出1.蜂鸣器开启
		sound1();
		delay_ms(1000);
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);    //PD_15输出0，蜂鸣器关闭
		delay_ms(1000);
		sound2();
		#if 0
		
		GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_SET);      //PD_15输出1.蜂鸣器开启
		delay_ms(time1);                                 //延时100ms
		GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_RESET);    //PD_15输出0，蜂鸣器关闭
		delay_ms(time2);                                //延时200ms

		
		SoundTimerInit(Sound2,ENABLE);
		GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_SET);      //PD_15输出1.蜂鸣器开启
		delay_ms(time1);                                 //延时100ms
		GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_RESET);    //PD_15输出0，蜂鸣器关闭
		delay_ms(time2);                                //延时200ms

		SoundTimerInit(Sound3 ,ENABLE);
		GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_SET);      //PD_15输出1.蜂鸣器开启
		delay_ms(time1);                                 //延时100ms
		GPIO_WriteBit(GPIOB,GPIO_Pin_5,Bit_RESET);    //PD_15输出0，蜂鸣器关闭
		delay_ms(time2);                                //延时200ms
		#endif
	}     
}
#endif

#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"

//Timer3部分重映射  TIM3_CH2->PB5：PB5作为PWM输出。

// 定义低音音名（数值单位：Hz）
#define L1 262
#define L2 294
#define L3 330
#define L4 349
#define L5 392
#define L6 440
#define L7 494

// 定义中音音名
#define M1 523
#define M2 587
#define M3 659
#define M4 698
#define M5 784
#define M6 880
#define M7 988

// 定义高音音名
#define H1 1047
#define H2 1175
#define H3 1319
#define H4 1397
#define H5 1568
#define H6 1760
#define H7 1976

// 定义时值单位，决定演奏速度（数值单位：ms）
#define TT 2000

typedef struct
{
  short mName; // 音名：取值L1～L7、M1～M7、H1～H7分别表示低音、中音、高音的1234567，取值0表示休止符
  short mTime; // 时值：取值T、T/2、T/4、T/8、T/16、T/32分别表示全音符、二分音符、四分音符、八分音符.，取值0表示演奏结束
}tNote;

#if 1
// 定义乐曲：刘德华《恭喜发财》
const tNote MyScore1[]=
{
  {L3,TT/8},{M6,TT/4},{M5,TT/4},{M6,TT/4},{M5,TT/8},{M3,TT/8},{M3,TT/4},{L3,TT/8},{M6,TT/4},{M5,TT/4},{M6,TT/4},{M5,TT/8},{M6,TT/8},{M6,TT/2},{M3,TT/8},{M2,TT/8+TT/16},{M3,TT/16},{M2,TT/8},
  {M1,TT/8},{L6,TT/4},{M3,TT/8},{M2,TT/8+TT/16},{M3,TT/16},{M2,TT/8},{M1,TT/8},{M2,TT/4},{M2,TT/8+TT/16},{M1,TT/8},{M1,TT/4},{M2,TT/4},{M3,TT/4},{M5,TT/4},{M6,TT},{M6,TT/8+TT/16},{M5,TT/16},
  {M3,TT/8},{M5,TT/8},{M6,TT/4}, //恭喜你发财。。。礼多人不怪
  
  {L3,TT/8},{L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/8},{L3,TT/8},{L3,TT/8},{L5,TT/8},{L6,TT/8},{M1,TT/8},{L6,TT/8},{L5,TT/8},{L6,TT/4},{L3,TT/8},{L5,TT/8},{M1,TT/4},{M1,TT/8},{M1,TT/8},{M2,TT/8},
  {M2,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L3,TT/8},{M2,TT/4},{M2,TT/8},{M1,TT/8},{M2,TT/8},{L6,TT/8},{L6,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{M2,TT/8},{M1,TT/8},{L6,TT/8},
  {M1,TT/8},{M3,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{M3,TT/4},{L5,TT/4},{L6,TT/2},//我祝满天下的女孩。。。智商充满你脑袋

  {L3,TT/8},{L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/8},{L3,TT/8},{L3,TT/8},{L5,TT/8},{L6,TT/8},{M1,TT/8},{L6,TT/8},{L5,TT/8},{L6,TT/4},{L3,TT/8},{L5,TT/8},{M1,TT/4},{M1,TT/8},{M1,TT/8},{M2,TT/8},
  {M2,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L3,TT/8},{M2,TT/4},{M2,TT/8},{M1,TT/8},{M2,TT/8},{L6,TT/8},{L6,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{M2,TT/8},{M1,TT/8},{L6,TT/8},
  {M1,TT/8},{M3,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{M3,TT/4},{L5,TT/4},{L6,TT/2},//我祝满天下的女孩。。。智商充满你脑袋

  {L5,TT/4},{M1,TT/2+TT/4},{M1,TT/8},{M2,TT/8},{M3,TT/2+TT/4},{M3,TT/8},{M5,TT/8},{M5,TT/4+TT/8},{M3,TT/8},{M2,TT/4},{M1,TT/4},{M2,TT/2},{M2,TT/4+TT/8},{L6,TT/8},{M2,TT/4},{M3,TT/4},
  {M4,TT/8+TT/16},{M5,TT/16},{M4,TT/8},{M3,TT/8},{M2,TT/2},{M5,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/4},{L5,TT/8},{L6,TT/2},//大摇大摆。。。要喊得够豪迈

  {L3,TT/8},{M6,TT/4},{M5,TT/4},{M6,TT/4},{M5,TT/8},{M3,TT/8},{M3,TT/4},{L3,TT/8},{M6,TT/4},{M5,TT/4},{M6,TT/4},{M5,TT/8},{M6,TT/8},{M6,TT/2},{M3,TT/8},{M2,TT/8+TT/16},{M3,TT/16},{M2,TT/8},
  {M1,TT/8},{L6,TT/4},{M3,TT/8},{M2,TT/8+TT/16},{M3,TT/16},{M2,TT/8},{M1,TT/8},{M2,TT/4},{M2,TT/8+TT/16},{M1,TT/8},{M1,TT/4},{M2,TT/4},{M3,TT/4},{M5,TT/4},{M6,TT},{M6,TT/8+TT/16},{M5,TT/16},
  {M3,TT/8},{M5,TT/8},{M6,TT/4}, //恭喜你发财。。。礼多人不怪

  {L3,TT/8},{L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/8},{L3,TT/8},{L3,TT/8},{L5,TT/8},{L6,TT/8},{M1,TT/8},{L6,TT/8},{L5,TT/8},{L6,TT/4},{L3,TT/8},{L5,TT/8},{M1,TT/4},{M1,TT/8},{M1,TT/8},{M2,TT/8},
  {M2,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L3,TT/8},{M2,TT/4},{M2,TT/8},{M1,TT/8},{M2,TT/8},{L6,TT/8},{L6,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{M2,TT/8},{M1,TT/8},{L6,TT/8},
  {M1,TT/8},{M3,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{M3,TT/4},{L5,TT/4},{L6,TT/2},//我祝满天下的女孩。。。智商充满你脑袋

  {L5,TT/4},{M1,TT/2+TT/4},{M1,TT/8},{M2,TT/8},{M3,TT/2+TT/4},{M3,TT/8},{M5,TT/8},{M5,TT/4+TT/8},{M3,TT/8},{M2,TT/4},{M1,TT/4},{M2,TT/2},{M2,TT/4+TT/8},{L6,TT/8},{M2,TT/4},{M3,TT/4},
  {M4,TT/8+TT/16},{M5,TT/16},{M4,TT/8},{M3,TT/8},{M2,TT/2},{M5,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/4},{L5,TT/8},{L6,TT/2},//大摇大摆。。。要喊得够豪迈

  {L3,TT/8},{M6,TT/4},{M5,TT/4},{M6,TT/4},{M5,TT/8},{M3,TT/8},{M3,TT/4},{L3,TT/8},{M6,TT/4},{M5,TT/4},{M6,TT/4},{M5,TT/8},{M6,TT/8},{M6,TT/2},{M3,TT/8},{M2,TT/8+TT/16},{M3,TT/16},{M2,TT/8},
  {M1,TT/8},{L6,TT/4},{M3,TT/8},{M2,TT/8+TT/16},{M3,TT/16},{M2,TT/8},{M1,TT/8},{M2,TT/4},{M2,TT/8+TT/16},{M1,TT/8},{M1,TT/4},{M2,TT/4},{M3,TT/4},{M5,TT/4},{M6,TT},{M6,TT/8+TT/16},{M5,TT/16},
  {M3,TT/8},{M5,TT/8},{M6,TT/4}, //恭喜你发财。。。礼多人不怪

  {L3,TT/8},{M6,TT/4},{M5,TT/4},{M6,TT/4},{M5,TT/8},{M3,TT/8},{M3,TT/4},{L3,TT/8},{M6,TT/4},{M5,TT/4},{M6,TT/4},{M5,TT/8},{M6,TT/8},{M6,TT/2},{M3,TT/8},{M2,TT/8+TT/16},{M3,TT/16},{M2,TT/8},
  {M1,TT/8},{L6,TT/4},{M3,TT/8},{M2,TT/8+TT/16},{M3,TT/16},{M2,TT/8},{M1,TT/8},{M2,TT/4},{M2,TT/8+TT/16},{M1,TT/8},{M1,TT/4},{M2,TT/4},{M3,TT/4},{M5,TT/4},{M6,TT},{M6,TT/8+TT/16},{M5,TT/16},
  {M3,TT/8},{M5,TT/8},{M6,TT/4}, //恭喜你发财。。。礼多人不怪
  {0,0},
};

#endif
#if 1

// 定义乐曲：《荷塘月色》
const tNote MyScore2[]=
{
  {M1,TT/8},{M1,TT/4},{L6,TT/8},{L5,TT/4},{L6,TT/4},{M1,TT/4},{M1,TT/8},{M2,TT/8},{M3,TT/2},{M2,TT/8},{M2,TT/4},{M1,TT/8},{M2,TT/4},{M2,TT/8},{M5,TT/8},{M5,TT/8},{M3,TT/8},
  {M3,TT/8},{M2,TT/8},{M3,TT/2},{M1,TT/8},{M1,TT/4},{L6,TT/8},{L5,TT/4},{M5,TT/4},{M3,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/2},{M2,TT/8},{M2,TT/4},//苍茫的天涯是我的爱。。。最呀最摇
  {M1,TT/8},{M2,TT/8},{M2,TT/4},{M3,TT/8},{M2,TT/8},{M1,TT/8},{L6,TT/8},{M2,TT/8},{M1,TT/2},//剪一段时光。。。美丽的琴音就落在我身旁
  {M1,TT/8},{M1,TT/4},{L6,TT/8},{L5,TT/4},{L6,TT/4},{M1,TT/8},{M1,TT/4},{M2,TT/8},{M3,TT/2},{M2,TT/8},{M2,TT/4},{M1,TT/8},{M2,TT/4},{M2,TT/8},{M5,TT/8},{M5,TT/8},{M3,TT/8},
  {M3,TT/8},{M2,TT/8},{M3,TT/2},{M1,TT/8},{M1,TT/8},{M1,TT/8},{L6,TT/8},{L5,TT/4},{M5,TT/4},{M3,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/2},{M2,TT/8},{M2,TT/4},//苍茫的天涯是我的爱。。。最呀最摇
  {M1,TT/8},{M2,TT/8},{M2,TT/4},{M3,TT/8},{M2,TT/8},{M1,TT/8},{L6,TT/8},{M2,TT/8},{M1,TT/2},//萤火虫。。。谁采下那一朵昨夜的忧伤
  {M3,TT/8},{M5,TT/4},{M5,TT/8},{M5,TT/4},{M5,TT/4},{M6,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/2},{M6,TT/8},{H1,TT/8},{M6,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},
  {L6,TT/8},{M2,TT/4},{M2,TT/8},{M3,TT/8},{M3,TT/8},{M2,TT/4+TT/8},{M3,TT/8},{M5,TT/4},{M5,TT/8},{M5,TT/4},{M5,TT/4},{M6,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},//苍茫的天涯是我的爱。。。最呀最摇
  {M1,TT/2},{L6,TT/8},{M1,TT/8},{L6,TT/8},{L5,TT/8},{M2,TT/4},{M3,TT/4},{M1,TT/2+TT/4},	//我像只鱼儿。。。等你宛在水中央
  {M1,TT/4+TT/8},{M5,TT/8},{M1,TT/8},{M5,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT},{M1,TT/4+TT/8},{M5,TT/8},{M1,TT/8},{M5,TT/8},{M1,TT/8},{M2,TT/8},{M2,TT},{M1,TT/4+TT/8},{M5,TT/8},{M1,TT/8},{M5,TT/8},
  {M2,TT/8},{M1,TT/8},{L6,TT/2},{L6,TT/8},{L5,TT/8},{M1,TT/8},{M2,TT/8},{M1,TT/4+TT/8},{M5,TT/8},{M1,TT/8},{M5,TT/8},{M1,TT/8},{L6,TT/8},{M1,TT},
  {M1,TT/8},{M1,TT/4},{L6,TT/8},{L5,TT/4},{L6,TT/4},{M1,TT/8},{M1,TT/4},{M2,TT/8},{M3,TT/2},{M2,TT/8},{M2,TT/4},{M1,TT/8},{M2,TT/4},{M2,TT/8},{M5,TT/8},{M5,TT/8},{M3,TT/8},
  {M3,TT/8},{M2,TT/8},{M3,TT/2},{M1,TT/8},{M1,TT/8},{M1,TT/8},{L6,TT/8},{L5,TT/4},{M5,TT/4},{M3,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/2},{M2,TT/8},{M2,TT/4},//苍茫的天涯是我的爱。。。最呀最摇
  {M1,TT/8},{M2,TT/8},{M2,TT/4},{M3,TT/8},{M2,TT/8},{M1,TT/8},{L6,TT/8},{M2,TT/8},{M1,TT/2},//萤火虫。。。谁采下那一朵昨夜的忧伤
  {M3,TT/8},{M5,TT/4},{M5,TT/8},{M5,TT/4},{M5,TT/4},{M6,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/2},{M6,TT/8},{H1,TT/8},{M6,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},
  {L6,TT/8},{M2,TT/4},{M2,TT/8},{M3,TT/8},{M3,TT/8},{M2,TT/4+TT/8},{M3,TT/8},{M5,TT/4},{M5,TT/8},{M5,TT/4},{M5,TT/4},{M6,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},//苍茫的天涯是我的爱。。。最呀最摇
  {M1,TT/2},{L6,TT/8},{M1,TT/8},{L6,TT/8},{L5,TT/8},{M2,TT/4},{M3,TT/4},{M1,TT/2+TT/4},	//我像只鱼儿。。。等你宛在水中央
  {M1,TT/4+TT/8},{M5,TT/8},{M1,TT/8},{M5,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT},{M1,TT/4+TT/8},{M5,TT/8},{M1,TT/8},{M5,TT/8},{M1,TT/8},{M2,TT/8},{M2,TT},{M1,TT/4+TT/8},{M5,TT/8},{M1,TT/8},{M5,TT/8},
  {M2,TT/8},{M1,TT/8},{L6,TT/2},{L6,TT/8},{L5,TT/8},{M1,TT/8},{M2,TT/8},{M1,TT/4+TT/8},{M5,TT/8},{M1,TT/8},{M5,TT/8},{M1,TT/8},{L6,TT/8},{M1,TT},

  {0,0},
};
#endif

#if 1

// 定义乐曲：《最炫民族风》
const tNote MyScore3[]=
{
  {L6,TT/4},{L3,TT/8},{L5,TT/8},{L6,TT/4},{L6,TT/8},{M1,TT/8},{M1,TT/4},{M2,TT/8},{M1,TT/8},{L6,TT/2},{M1,TT/4},{M1,TT/8},{L5,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M5,TT/8},
  {M5,TT/8},{M3,TT/8},{M2,TT/4},{M3,TT/2},{M6,TT/8},{M6,TT/8},{M6,TT/8},{M5,TT/8},{M3,TT/8},{M3,TT/4},{M1,TT/8},{L6,TT/8},{L6,TT/8},{L6,TT/8},{M3,TT/8},//苍茫的天涯是我的爱。。。最呀最摇
  {M2,TT/2},{M3,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{L6,TT/4},{L5,TT/4},{L6,TT/2}, //摆，什么样的歌声才是最开怀
  {L6,TT/4},{L3,TT/8},{L5,TT/8},{L6,TT/4},{L6,TT/8},{M1,TT/8},{M1,TT/4},{M2,TT/8},{M1,TT/8},{L6,TT/2},{M1,TT/4},{M1,TT/8},{L5,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M5,TT/8},
  {M5,TT/8},{M3,TT/8},{M2,TT/4},{M3,TT/2},{M6,TT/8},{M6,TT/8},{M6,TT/8},{M5,TT/8},{M3,TT/8},{M3,TT/4},{M1,TT/8},{L6,TT/8},{L6,TT/8},{L6,TT/8},{M3,TT/8},//苍茫的天涯是我的爱。。。最呀最摇
  {M2,TT/2},{M3,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{L6,TT/4},{L5,TT/4},{L6,TT/2}, //摆，什么样的歌声才是最开怀
  {M3,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M3,TT/8},{M5,TT/8},{M5,TT/8},{M6,TT/8},{H1,TT/8},{M6,TT/8},{M5,TT/4},{M6,TT/2},{L6,TT/4},{L6,TT/8},
  {L5,TT/8},{L6,TT/4},{M1,TT/4},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L6,TT/8},{M6,TT/8},{M6,TT/8},{M5,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},
  {M1,TT/8},{M2,TT/8},{M3,TT/2},//留下来
  {M1,TT/8},{L6,TT/8},{L6,TT/8},{M1,TT/8},{M2,TT/4},{L5,TT/8},{L5,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/2},{L6,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},
  {L5,TT/8},{L3,TT/8},{L6,TT/2},{L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/4},{M1,TT/4},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L6,TT/8},{M6,TT/8},
  {M6,TT/8},{M5,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},
  {M1,TT/8},{M2,TT/8},{M3,TT/2},//留下来
  {M1,TT/8},{L6,TT/8},{L6,TT/8},{M1,TT/8},{M2,TT/4},{L5,TT/8},{L5,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/4+TT/8},{M1,TT/8},{L6,TT/8},
  {M1,TT/8},{M2,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M3,TT/8},{M5,TT/8},{M6,TT/2},{M6,TT/2},{L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/4},
  {L6,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},
  {M6,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},{M1,TT/2},//登上天外云霄的舞台
  {L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/4},{M1,TT/4},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L6,TT/8},{M6,TT/8},{M6,TT/8},{M5,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},
  {M1,TT/8},{M2,TT/8},{M3,TT/2},//留下来
  {M1,TT/8},{L6,TT/8},{L6,TT/8},{M1,TT/8},{M2,TT/4},{L5,TT/8},{L5,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/2},{L6,TT/8},{M1,TT/8},{M2,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/8},
  {L5,TT/8},{L3,TT/8},{L6,TT/2},{L6,TT/4},{L6,TT/8},{L5,TT/8},{L6,TT/4},{M1,TT/4},{M2,TT/8},{M3,TT/16},{M2,TT/16},{M1,TT/8},{M2,TT/8},{M3,TT/2},{L6,TT/8},{M6,TT/8},
  {M6,TT/8},{M5,TT/8},{M2,TT/8},{M3,TT/16},{M2,TT/16},
  {M1,TT/8},{M2,TT/8},{M3,TT/2},//留下来
  {M1,TT/8},{L6,TT/8},{L6,TT/8},{M1,TT/8},{M2,TT/4},{L5,TT/8},{L5,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M2,TT/8},{M1,TT/4+TT/8},{M1,TT/8},{L6,TT/8},
  {M1,TT/8},{M2,TT/8},{M3,TT/8},{M5,TT/8},{M3,TT/8},{M3,TT/8},{M5,TT/8},{M6,TT/2},{M6,TT/2},
  {0,0},
};
#endif
// 蜂鸣器停止发声
void buzzerQuiet(void)	
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_Cmd(TIM3, DISABLE);  //停止TIM3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);	 //根据设定参数初始化GPIOB.5
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);	//PB.5 输出低
}

/////////////////////////////////////////////////////////	 
//蜂鸣器发出指定频率的声音
//usFreq是发声频率，取值 (系统时钟/65536)+1 ～ 20000，单位：Hz
void buzzerSound(unsigned short usFreq)	 
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	unsigned long  ulVal;
	if((usFreq<=8000000/65536UL)||(usFreq>20000))
	{
		buzzerQuiet();// 蜂鸣器静音
	}
	else
	{
		GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
		//设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO	  
		ulVal=8000000/usFreq;
		TIM3->ARR =ulVal ;	//设置自动重装载寄存器周期的值（音调） 
		TIM_SetCompare2(TIM3,ulVal/2);//音量
		TIM_Cmd(TIM3, ENABLE);  //启动TIM3
	}  
}

void musicPlay(const tNote * MyScore) 
{ 
	u8 i=0; 
	while(1) 
	{
		if (MyScore[i].mTime == 0) break; 
		buzzerSound(MyScore[i].mName); 
		delay_ms(MyScore[i].mTime);
		i++; 
		buzzerQuiet(); // 蜂鸣器静音
		delay_ms(10);// 10 ms 
	} 
}

int main(void) 
{ 
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	TIM3_PWM_Init(14399,10);	 //分频。PWM频率=72000/14400/11（Khz）
	for (;;) 
	{
		musicPlay(MyScore3);
		delay_ms(1500);
		delay_ms(1500);
	} 
}


