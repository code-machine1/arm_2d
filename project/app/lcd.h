#ifndef __LCD_H
#define __LCD_H		
#include "at32f421_wk_config.h"	 
#include "stdint.h"
#include "stdlib.h"
 
//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令 
}_lcd_dev; 	  

// LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数

// LCD的画笔颜色和背景色	   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色

// PB0~15,作为数据线
#define DATAOUT(x) GPIOB->odt=x; //数据输出
#define DATAIN     GPIOB->idt;   //数据输入	

//-----------------LCD端口定义---------------- 
 
#define	LCD_CS_SET  gpio_bits_set(GPIOC,GPIO_PINS_13)    //片选端口    PA4
#define	LCD_RS_SET	gpio_bits_set(GPIOA,GPIO_PINS_1)    //数据/命令   PA1	   
#define	LCD_WR_SET	gpio_bits_set(GPIOA,GPIO_PINS_3)    //写数据	  PA3
#define	LCD_RD_SET	gpio_bits_set(GPIOA,GPIO_PINS_2)    //读数据	  PA2
#define	LCD_RES_SET	gpio_bits_set(GPIOA,GPIO_PINS_0)    //复位	      PA0

#define	LCD_CS_CLR  gpio_bits_reset(GPIOC,GPIO_PINS_13)     //片选端口  		PC9
#define	LCD_RS_CLR	gpio_bits_reset(GPIOA,GPIO_PINS_1)     //数据/命令		PC8	   
#define	LCD_WR_CLR	gpio_bits_reset(GPIOA,GPIO_PINS_3)     //写数据			PC7
#define	LCD_RD_CLR	gpio_bits_reset(GPIOA,GPIO_PINS_2)     //读数据			PC6   
#define	LCD_RES_CLR	gpio_bits_set(GPIOA,GPIO_PINS_0)    //复位	      PA0
//写数据函数
//#define LCD_WR_DATA(data){\
//LCD_RS_SET;\
//LCD_CS_CLR;\
//DATAOUT(data);\
//LCD_WR_CLR;\
//LCD_WR_SET;\
//LCD_CS_SET;\
//} 

//
	 
//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define YELLOW_1       	 0x1F
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#define BLOCK_SIZE       4096  //图片缓存区的大小，根据RAM容量和需求调整（最大4k）

void LCD_WR_REG(u16 reg);//写寄存器
void LCD_WR_data(u16 data);//写数据
void LCD_WR_DATA(u16 data);//写数据
u16 LCD_RD_DATA(void);//读点
u16 LCD_ReadReg(u16 LCD_Reg);//读寄存器
void LCD_SetCursor(u16 x,u16 y);//设置光标位置
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_DisplayOn(void);//开显示
void LCD_DisplayOff(void);//关显示
u16 LCD_ReadPoint(u16 x,u16 y);//读点函数
void LCD_Clear(u16 color);//清屏函数
	
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);//指定区域填充颜色
void LCD_DrawPoint(u16 x,u16 y,u16 color);//在指定位置画一个点
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);//在指定位置画一条线
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//在指定位置画一个矩形
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);//在指定位置画一个圆

void DrawProgressBar(u16 x1, u16 y1, u16 x2, u16 y2,u8 scale,u16 color);


void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示汉字串
void LCD_ShowChinese12x12(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个12x12汉字
void LCD_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个16x16汉字
void LCD_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个24x24汉字
void LCD_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode);//显示单个32x32汉字

void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode);//显示一个字符
void LCD_ShowChar1(u16 x, u16 y, u8 num, u16 fc, u16 bc,u8 size, u8 mode);
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode);//显示字符串
u32 mypow(u8 m,u8 n);//求幂
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey);//显示整数变量
void LCD_ShowIntNum64(u16 x, u16 y, u16 num, u8 len, u16 fc, u16 bc);
void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey);//显示两位小数变量

void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[]);//显示图片
void TranferPicturetoTFT_LCD(uint8_t Pic_Num);
void Set_Dir(u8 dir);
void LCD_Init(void);		


#endif  
	 
	 




