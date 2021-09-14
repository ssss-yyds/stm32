#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

#include "spi.h"
#include "lcd.h"
#include "key.h"
 
// "2.8 TFT SPI 240*320"   120 160 
void GPIO_Configuration(void);

void xianshi()//显示信息
{ 
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;   
	//显示32*32汉字
	showhanzi32(0,0,0);	 //淘
	showhanzi32(40,0,1);	 //晶
	showhanzi32(80,0,2);    //驰
	//显示16*16汉字
	showhanzi16(0,35,0);	  //专
	showhanzi16(20,35,1);	  //注
	showhanzi16(40,35,2);	  //显
	showhanzi16(60,35,3);	  //示
	showhanzi16(80,35,4);	  //方
	showhanzi16(100,35,5);	  //案	   
	LCD_ShowString(0,55,200,16,16,"2.8 TFT SPI 240*320");
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	GPIO_Configuration();

	key_init();

	PAout(2)=1;
	delay_init();
	SPI1_Init();
	LCD_Init();
//	POINT_COLOR=RED;//设置字体为红色 
//	xianshi();	   //显示信息
//	showqq();	   //显示QQ
	
	POINT_COLOR=RED;
	BACK_COLOR=WHITE;

	PAout(2)=0;
    while (1)
	{
		PCout(13)=1;
		delay_ms(1000);
		PCout(13)=0;
		delay_ms(1000);
    }
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); 						 
//=============================================================================
//LED -> PC13
//=============================================================================			 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);//PA2	LCD_BL
}


void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line14) != RESET)
	{
		delay_ms(10);
		if(KEY1==0)
			show_image(40,80,160,160);
		 EXTI_ClearITPendingBit(EXTI_Line14);  //清除LINE15线路挂起位
	}
  
	else if (EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		delay_ms(10);
		if(KEY2==0)
			LCD_Clear(WHITE);
		 EXTI_ClearITPendingBit(EXTI_Line15);  //清除LINE15线路挂起位
	}
}

