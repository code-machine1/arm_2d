#include "lcd.h"
#include "stdlib.h"
#include "lcdfont.h"
#include "wk_system.h"
#include "at32_spiflash.h"
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//写寄存器函数
//regval:寄存器值
void LCD_WR_REG(u16 reg)
{
    //#if LCD_USE8BIT_MODEL==1//使用8位并行数据总线模式
    //	LCD_RS_CLR;
    //	LCD_CS_CLR;
    //	send_data(reg);
    //	LCD_WR_CLR;
    //	LCD_WR_SET;
    //	LCD_CS_SET;
    //	LCD_RS_SET;
    //#else   //使用16位并行数据总线模式
    LCD_RS_CLR;
    LCD_CS_CLR;
    DATAOUT(reg);
    LCD_WR_CLR;
    LCD_WR_SET;
    LCD_RS_SET;
    LCD_CS_SET;
    //#endif
}

//写LCD数据
//data:要写入的值
void LCD_WR_data(u16 data)
{
    //#if LCD_USE8BIT_MODEL==1//使用8位并行数据总线模式
    //	LCD_CS_CLR;
    //	send_data(data);
    //	LCD_WR_CLR;
    //	LCD_WR_SET;
    //	LCD_CS_SET;
    //#else  //使用16位并行数据总线模式
    LCD_CS_CLR;
    DATAOUT(data);
    LCD_WR_CLR;
    LCD_WR_SET;
    LCD_CS_SET;
    //#endif
}

//写LCD数据
//data:要写入的值
void LCD_WR_DATA(u16 data)
{
    //#if LCD_USE8BIT_MODEL==1//使用8位并行数据总线模式
    //	LCD_CS_CLR;
    //	send_data(data>>8);
    //	LCD_WR_CLR;
    //	LCD_WR_SET;
    //	send_data(data);
    //	LCD_WR_CLR;
    //	LCD_WR_SET;
    //	LCD_CS_SET;
    //#else   //使用16位并行数据总线模式
    LCD_CS_CLR;
    DATAOUT(data);
    LCD_WR_CLR;
    LCD_WR_SET;
    LCD_CS_SET;
    //#endif
}

void set_mode(gpio_mode_type pin_mode)
{
    gpio_init_type gpio_init_struct;
    gpio_default_para_init(&gpio_init_struct);

    if (pin_mode == GPIO_MODE_OUTPUT)
    {
        gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
        gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
        gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
        gpio_init_struct.gpio_pins = DB0_PIN | DB1_PIN | DB2_PIN | DB10_PIN | DB11_PIN |
                                     DB12_PIN | DB13_PIN | DB14_PIN | DB15_PIN | DB3_PIN |
                                     DB4_PIN | DB5_PIN | DB6_PIN | DB7_PIN | DB8_PIN |
                                     DB9_PIN;
        gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
        gpio_init(GPIOB, &gpio_init_struct);
    }
    else
    {
        gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
        gpio_init_struct.gpio_pins = DB0_PIN | DB1_PIN | DB2_PIN | DB10_PIN | DB11_PIN |
                                     DB12_PIN | DB13_PIN | DB14_PIN | DB15_PIN | DB3_PIN |
                                     DB4_PIN | DB5_PIN | DB6_PIN | DB7_PIN | DB8_PIN |
                                     DB9_PIN;
        gpio_init_struct.gpio_pull = GPIO_PULL_UP;
        gpio_init(GPIOB, &gpio_init_struct);
    }
}

//读LCD数据
//返回值:读到的值
u16 LCD_RD_DATA(void)
{
    //u16 data=0,data0=0;
    u16 data = 0;
    set_mode(GPIO_MODE_INPUT);
#if LCD_USE8BIT_MODEL==1//使用8位并行数据总线模式
    LCD_CS_CLR;
    LCD_RD_CLR;
    data = DATAIN;
    LCD_RD_SET;
    LCD_RD_CLR;
    data0 = DATAIN;
    LCD_RD_SET;
    LCD_CS_SET;
    data &= 0xff;
    data = data0 | data << 8;
#else //使用16位并行数据总线模式
    LCD_CS_CLR;
    LCD_RD_CLR;
    data = DATAIN;
    LCD_RD_SET;
    LCD_CS_SET;
#endif
    set_mode(GPIO_MODE_OUTPUT);
    return data;
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_WR_REG(0x2a);//列地址设置
    LCD_WR_data(x1 >> 8);
    LCD_WR_data(x1 & 0xff);
    LCD_WR_data(x2 >> 8);
    LCD_WR_data(x2 & 0xff);
    LCD_WR_REG(0x2b);//行地址设置
    LCD_WR_data(y1 >> 8);
    LCD_WR_data(y1 & 0xff);
    LCD_WR_data(y2 >> 8);
    LCD_WR_data(y2 & 0xff);
    LCD_WR_REG(0x2c);//储存器写
}

/******************************************************************************
      函数说明：设置光标位置
      入口数据：x,y 光标位置
      返回值：  无
******************************************************************************/
void LCD_SetCursor(u16 x, u16 y)
{
    LCD_WR_REG(0x2a);//列地址设置
    LCD_WR_data(x >> 8);
    LCD_WR_data(x & 0xff);
    LCD_WR_REG(0x2b);//行地址设置
    LCD_WR_data(y >> 8);
    LCD_WR_data(y & 0xff);
}

//LCD开启显示
void LCD_DisplayOn(void)
{
    LCD_WR_REG(0X29);	//开启显示
}

//LCD关闭显示
void LCD_DisplayOff(void)
{
    LCD_WR_REG(0X28);	//关闭显示
}


//读取个某点的颜色值
//x,y:坐标
//返回值:此点的颜色
//u16 LCD_ReadPoint(u16 x,u16 y)
//{
// 	u16 rgb;
//	LCD_SetCursor(x,y);
//	LCD_WR_REG(0X2E);
//#if LCD_USE8BIT_MODEL==1//使用8位并行数据总线模式
//	LCD_DATA_IN();
//	LCD_RS_SET;
//	LCD_CS_CLR;
//	LCD_RD_CLR;
//	rgb=DATAIN;
//	LCD_RD_SET;
//	LCD_RD_CLR;
//#else //使用16位并行数据总线模式
//	rgb=LCD_RD_DATA();//dummy read
//#endif
//	rgb=LCD_RD_DATA();//实际坐标颜色
//	return rgb;
//}


void LCD_Clear(u16 color)
{
    u16 i, j;
    LCD_Address_Set(0, 0, lcddev.width - 1, lcddev.height - 1); //设置显示范围

    for (i = 0; i < lcddev.width; i++)
    {
        for (j = 0; j < lcddev.height; j++)
        {
            LCD_WR_DATA(color);
        }
    }
}

/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color)
{
    u16 i, j;
    LCD_Address_Set(xsta, ysta, xend - 1, yend - 1); //设置显示范围

    for (i = ysta; i < yend; i++)
    {
        for (j = xsta; j < xend; j++)
        {
            LCD_WR_DATA(color);
        }
    }
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(u16 x, u16 y, u16 color)
{

    LCD_Address_Set(x, y, x, y); //设置光标位置
    LCD_WR_DATA(color);
}


/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1; //画线起点坐标
    uCol = y1;

    if (delta_x > 0)
    {
        incx = 1;    //设置单步方向
    }
    else if (delta_x == 0)
    {
        incx = 0;    //垂直线
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0;    //水平线
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
    {
        distance = delta_x;    //选取基本增量坐标轴
    }
    else
    {
        distance = delta_y;
    }

    for (t = 0; t < distance + 1; t++)
    {
        LCD_DrawPoint(uRow, uCol, color); //画点
        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}


/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   矩形的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}


/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
                color   圆的颜色
      返回值：  无
******************************************************************************/
void Draw_Circle(u16 x0, u16 y0, u8 r, u16 color)
{
    int a, b;
    a = 0;
    b = r;

    while (a <= b)
    {
        LCD_DrawPoint(x0 - b, y0 - a, color);       //3
        LCD_DrawPoint(x0 + b, y0 - a, color);       //0
        LCD_DrawPoint(x0 - a, y0 + b, color);       //1
        LCD_DrawPoint(x0 - a, y0 - b, color);       //2
        LCD_DrawPoint(x0 + b, y0 + a, color);       //4
        LCD_DrawPoint(x0 + a, y0 - b, color);       //5
        LCD_DrawPoint(x0 + a, y0 + b, color);       //6
        LCD_DrawPoint(x0 - b, y0 + a, color);       //7
        a++;

        if ((a * a + b * b) > (r * r)) //判断要画的点是否过远
        {
            b--;
        }
    }
}

/******************************************************************************
      函数说明：显示汉字串
      入口数据：x,y显示坐标
                *s 要显示的汉字串
                fc 字的颜色
                bc 字的背景色
                sizey 字号 可选 16 24 32
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    while (*s != 0)
    {
        if (sizey == 12)
        {
            LCD_ShowChinese12x12(x, y, s, fc, bc, sizey, mode);
        }
        else if (sizey == 16)
        {
            LCD_ShowChinese16x16(x, y, s, fc, bc, sizey, mode);
        }
        else if (sizey == 24)
        {
            LCD_ShowChinese24x24(x, y, s, fc, bc, sizey, mode);
        }
        else if (sizey == 32)
        {
            LCD_ShowChinese32x32(x, y, s, fc, bc, sizey, mode);
        }
        else
        {
            return;
        }

        s += 2;
        x += sizey;
    }
}

/******************************************************************************
      函数说明：显示单个12x12汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese12x12(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    u8 i, j, m = 0;
    u16 k;
    u16 HZnum;//汉字数目
    u16 TypefaceNum;//一个字符所占字节大小
    u16 x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont12) / sizeof(typFNT_GB12);	//统计汉字数目

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont12[k].Index[0] == *(s)) && (tfont12[k].Index[1] == *(s + 1)))
        {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);

            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode) //非叠加方式
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j))
                        {
                            LCD_WR_DATA(fc);
                        }
                        else
                        {
                            LCD_WR_DATA(bc);
                        }

                        m++;

                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if (tfont12[k].Msk[i] & (0x01 << j))
                        {
                            LCD_DrawPoint(x, y, fc);    //画一个点
                        }

                        x++;

                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }

        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/******************************************************************************
      函数说明：显示单个16x16汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese16x16(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    u8 i, j, m = 0;
    u16 k;
    u16 HZnum;//汉字数目
    u16 TypefaceNum;//一个字符所占字节大小
    u16 x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont16) / sizeof(typFNT_GB16);	//统计汉字数目

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1)))
        {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);

            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode) //非叠加方式
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j))
                        {
                            LCD_WR_DATA(fc);
                        }
                        else
                        {
                            LCD_WR_DATA(bc);
                        }

                        m++;

                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if (tfont16[k].Msk[i] & (0x01 << j))
                        {
                            LCD_DrawPoint(x, y, fc);    //画一个点
                        }

                        x++;

                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }

        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}


/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese24x24(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    u8 i, j, m = 0;
    u16 k;
    u16 HZnum;//汉字数目
    u16 TypefaceNum;//一个字符所占字节大小
    u16 x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont24) / sizeof(typFNT_GB24);	//统计汉字数目

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1)))
        {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);

            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode) //非叠加方式
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j))
                        {
                            LCD_WR_DATA(fc);
                        }
                        else
                        {
                            LCD_WR_DATA(bc);
                        }

                        m++;

                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if (tfont24[k].Msk[i] & (0x01 << j))
                        {
                            LCD_DrawPoint(x, y, fc);    //画一个点
                        }

                        x++;

                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }

        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese32x32(u16 x, u16 y, u8 *s, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    u8 i, j, m = 0;
    u16 k;
    u16 HZnum;//汉字数目
    u16 TypefaceNum;//一个字符所占字节大小
    u16 x0 = x;
    TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    HZnum = sizeof(tfont32) / sizeof(typFNT_GB32);	//统计汉字数目

    for (k = 0; k < HZnum; k++)
    {
        if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1)))
        {
            LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);

            for (i = 0; i < TypefaceNum; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    if (!mode) //非叠加方式
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j))
                        {
                            LCD_WR_DATA(fc);
                        }
                        else
                        {
                            LCD_WR_DATA(bc);
                        }

                        m++;

                        if (m % sizey == 0)
                        {
                            m = 0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if (tfont32[k].Msk[i] & (0x01 << j))
                        {
                            LCD_DrawPoint(x, y, fc);    //画一个点
                        }

                        x++;

                        if ((x - x0) == sizey)
                        {
                            x = x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }

        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}


/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(u16 x, u16 y, u8 num, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    u16 temp, sizex, t, m = 0;
    u16 i, TypefaceNum; //一个字符所占字节大小
    u16 x0 = x;
    sizex = sizey / 2;
    TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
    num = num - ' '; //得到偏移后的值
    LCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1); //设置光标位置

    for (i = 0; i < TypefaceNum; i++)
    {
        if (sizey == 12)
        {
            temp = ascii_1206[num][i];    //调用6x12字体
        }
        else if (sizey == 16)
        {
            temp = ascii_1608[num][i];    //调用8x16字体
        }
        else if (sizey == 24)
        {
            temp = ascii_2412[num][i];    //调用12x24字体
        }
        else if (sizey == 32)
        {
            temp = ascii_3216[num][i];    //调用16x32字体
        }
        else if (sizey == 40)
        {
            temp = ascii_4020[num][i];    //调用16x32字体
        }
        else if (sizey == 64)
        {
            temp = ascii_6432[num][i];    //调用16x32字体
        }
        else
        {
            return;
        }

        for (t = 0; t < 8; t++)
        {
            if (!mode) //非叠加模式
            {
                if (temp & (0x01 << t))
                {
                    LCD_WR_DATA(fc);
                }
                else
                {
                    LCD_WR_DATA(bc);
                }

                m++;

                if (m % sizex == 0)
                {
                    m = 0;
                    break;
                }
            }
            else//叠加模式
            {
                if (temp & (0x01 << t))
                {
                    LCD_DrawPoint(x, y, fc);    //画一个点
                }

                x++;

                if ((x - x0) == sizex)
                {
                    x = x0;
                    y++;
                    break;
                }
            }
        }
    }
}

void LCD_ShowChar1(u16 x, u16 y, u8 num, u16 fc, u16 bc,u8 size, u8 mode)
{
    u16 temp, t1/*t*/;
    u16 t;                                           
    u16 y0 = y; 

    for (t = 0; t < 256; t++) //总共256个数据
    {
        if (size == 64)
        {
            temp = GBK6464[num][t];    
        }
        else
        {
            return;    //没有的字库
        }

        for (t1 = 0; t1 < 8; t1++) //yuan 8
        {
            if (temp & 0x80)
            {
                LCD_DrawPoint(x, y, fc);
            }
            else if (mode == 0)
            {
                LCD_DrawPoint(x, y, bc);
            }

            temp <<= 1;
            y++;

            if (y >= lcddev.height)
            {
                return;    //超区域了
            }

            if ((y - y0) == size)
            {
                y = y0;
                x++;

                if (x >= lcddev.width)
                {
                    return;    //超区域了
                }

                break;
            }
        }
    }
}

/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowString(u16 x, u16 y, const u8 *p, u16 fc, u16 bc, u8 sizey, u8 mode)
{
    while (*p != '\0')
    {
        LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
        x += sizey / 2;
        p++;
    }
}


/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
u32 mypow(u8 m, u8 n)
{
    u32 result = 1;

    while (n--)
    {
        result *= m;
    }

    return result;
}


/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowIntNum(u16 x, u16 y, u16 num, u8 len, u16 fc, u16 bc, u8 sizey)
{
    u8 t, temp;
    u8 enshow = 0;
    u8 sizex = sizey / 2;

    for (t = 0; t < len; t++)
    {
        temp = (num / mypow(10, len - t - 1)) % 10;

        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
                continue;
            }
            else
            {
                enshow = 1;
            }
        }

        LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}






/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowFloatNum1(u16 x, u16 y, float num, u8 len, u16 fc, u16 bc, u8 sizey)
{
    u8 t, temp, sizex;
    u16 num1;
    sizex = sizey / 2;
    num1 = num * 100;

    for (t = 0; t < len; t++)
    {
        temp = (num1 / mypow(10, len - t - 1)) % 10;

        if (t == (len - 2))
        {
            LCD_ShowChar(x + (len - 2)*sizex, y, '.', fc, bc, sizey, 0);
            t++;
            len += 1;
        }

        LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
    }
}


/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组
      返回值：  无
******************************************************************************/
void LCD_ShowPicture(u16 x, u16 y, u16 length, u16 width, const u8 pic[])
{
    u8 picH, picL;
    u16 i, j;
    u32 k = 0;
    LCD_Address_Set(x, y, x + length - 1, y + width - 1);

    for (i = 0; i < length; i++)
    {
        for (j = 0; j < width; j++)
        {
            picH = pic[k * 2];
            picL = pic[k * 2 + 1];
            LCD_WR_DATA(picH << 8 | picL);
            k++;
        }
    }
}



/******************************************************************************/
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}

void Set_Dir(u8 dir)
{
    if ((dir >> 4) % 4)
    {
        lcddev.width = 480;
        lcddev.height = 320;
    }
    else
    {
        lcddev.width = 320;
        lcddev.height = 480;
    }
}

void LCD_Scan_Dir(u8 dir)
{
    u16 regval = 0;
    u16 dirreg = 0;

    if ((lcddev.dir == 1 && lcddev.id != 0X6804 && lcddev.id != 0X1963) || (lcddev.dir == 0
            && lcddev.id == 0X1963)) //横屏时，对6804和1963不改变扫描方向！竖屏时1963改变方向
    {
        switch (dir) //方向转换
        {
        case 0:
            dir = 6;
            break;

        case 1:
            dir = 7;
            break;

        case 2:
            dir = 4;
            break;

        case 3:
            dir = 5;
            break;

        case 4:
            dir = 1;
            break;

        case 5:
            dir = 0;
            break;

        case 6:
            dir = 3;
            break;

        case 7:
            dir = 2;
            break;
        }
    }

    switch (dir)
    {
    case L2R_U2D://从左到右,从上到下
        regval |= (0 << 7) | (0 << 6) | (0 << 5);
        break;

    case L2R_D2U://从左到右,从下到上
        regval |= (1 << 7) | (0 << 6) | (0 << 5);
        break;

    case R2L_U2D://从右到左,从上到下
        regval |= (0 << 7) | (1 << 6) | (0 << 5);
        break;

    case R2L_D2U://从右到左,从下到上
        regval |= (1 << 7) | (1 << 6) | (0 << 5);
        break;

    case U2D_L2R://从上到下,从左到右
        regval |= (0 << 7) | (0 << 6) | (1 << 5);
        break;

    case U2D_R2L://从上到下,从右到左
        regval |= (0 << 7) | (1 << 6) | (1 << 5);
        break;

    case D2U_L2R://从下到上,从左到右
        regval |= (1 << 7) | (0 << 6) | (1 << 5);
        break;

    case D2U_R2L://从下到上,从右到左
        regval |= (1 << 7) | (1 << 6) | (1 << 5);
        break;
    }

    dirreg = 0X36;
    regval |= 0X08;
    LCD_WriteReg(dirreg, regval);
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(0);
    LCD_WR_DATA(0);
    LCD_WR_DATA((lcddev.width - 1) >> 8);
    LCD_WR_DATA((lcddev.width - 1) & 0XFF);
    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(0);
    LCD_WR_DATA(0);
    LCD_WR_DATA((lcddev.height - 1) >> 8);
    LCD_WR_DATA((lcddev.height - 1) & 0XFF);
}

/******************************************************************************/
void LCD_Display_Dir(u8 dir)
{
    if (dir == 0)
    {
        lcddev.dir = 0;
        lcddev.wramcmd = 0X2C;
        lcddev.setxcmd = 0X2A;
        lcddev.setycmd = 0X2B;
        lcddev.width = 320;
        lcddev.height = 480;
    }
    else
    {
        lcddev.dir = 1;
        lcddev.wramcmd = 0X2C;
        lcddev.setxcmd = 0X2A;
        lcddev.setycmd = 0X2B;
        lcddev.width = 480;
        lcddev.height = 320;
    }

    LCD_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}


//初始化lcd
void LCD_Init(void)
{
    LCD_RES_SET;
    wk_delay_ms(120);
    LCD_RES_CLR;
    wk_delay_ms(120);
    gpio_bits_set(GPIOB, GPIO_PINS_ALL);
    LCD_WR_REG(0XD3);
    lcddev.id = LCD_RD_DATA();
    lcddev.id = LCD_RD_DATA();
    lcddev.id = LCD_RD_DATA();
    lcddev.id <<= 8;
    lcddev.id |= LCD_RD_DATA();
    //        LCD_WR_REG(0xE0);
    //        LCD_WR_DATA(0x00);
    //        LCD_WR_DATA(0x07);
    //        LCD_WR_DATA(0x10);
    //        LCD_WR_DATA(0x09);
    //        LCD_WR_DATA(0x17);
    //        LCD_WR_DATA(0x0B);
    //        LCD_WR_DATA(0x41);
    //        LCD_WR_DATA(0x89);
    //        LCD_WR_DATA(0x4B);
    //        LCD_WR_DATA(0x0A);
    //        LCD_WR_DATA(0x0C);
    //        LCD_WR_DATA(0x0E);
    //        LCD_WR_DATA(0x18);
    //        LCD_WR_DATA(0x1B);
    //        LCD_WR_DATA(0x0F);
    //        LCD_WR_REG(0xE1);
    //        LCD_WR_DATA(0x00);
    //        LCD_WR_DATA(0x17);
    //        LCD_WR_DATA(0x1A);
    //        LCD_WR_DATA(0x04);
    //        LCD_WR_DATA(0x0E);
    //        LCD_WR_DATA(0x06);
    //        LCD_WR_DATA(0x2F);
    //        LCD_WR_DATA(0x45);
    //        LCD_WR_DATA(0x43);
    //        LCD_WR_DATA(0x02);
    //        LCD_WR_DATA(0x0A);
    //        LCD_WR_DATA(0x09);
    //        LCD_WR_DATA(0x32);
    //        LCD_WR_DATA(0x36);
    //        LCD_WR_DATA(0x0F);
    LCD_WR_REG(0xE0);   //P-Gamma
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x3A);
    LCD_WR_DATA(0x56);
    LCD_WR_DATA(0x4D);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x30);
    LCD_WR_DATA(0x3E);
    LCD_WR_DATA(0x0F);
    LCD_WR_REG(0XE1);   //N-Gamma
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x38);
    LCD_WR_DATA(0x34);
    LCD_WR_DATA(0x4D);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x0D);
    LCD_WR_DATA(0x0B);
    LCD_WR_DATA(0x31);
    LCD_WR_DATA(0x37);
    LCD_WR_DATA(0x0F);
    LCD_WR_REG(0XF7);
    LCD_WR_DATA(0xA9);
    LCD_WR_DATA(0x51);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x82);
    LCD_WR_REG(0xC0);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x09);
    LCD_WR_REG(0xC1);
    LCD_WR_DATA(0x41);
    LCD_WR_REG(0xC5);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x80);
    LCD_WR_REG(0xB1);
    LCD_WR_DATA(0xB0);
    LCD_WR_DATA(0x11);
    LCD_WR_REG(0xB4);
    LCD_WR_DATA(0x02);
    LCD_WR_REG(0xB6);
    LCD_WR_DATA(0x02);
    LCD_WR_DATA(0x22);
    LCD_WR_DATA(0x3B);
    LCD_WR_REG(0xB7);
    LCD_WR_DATA(0xC6);
    LCD_WR_REG(0xBE);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x04);
    LCD_WR_REG(0xE9);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x55);
    LCD_WR_REG(0x36);
    LCD_WR_DATA(0x08);
    LCD_WR_REG(0X11);
    LCD_Display_Dir(0);
    wk_delay_ms(120);
    LCD_WR_REG(0x29);
    /* Sleep Out11h */
    LCD_WR_REG(0x11);
    wk_delay_ms(200);
    LCD_WR_REG(0x29);
    LCD_Clear(WHITE);
}




/**
 * @name   TranferPicturetoTFT_LCD
 * @brief  读取外部Flash图片资源到TFT-LCD显示
 * @param  Pic_Num：图片序号
 * @retval None
 */

void TranferPicturetoTFT_LCD(uint8_t Pic_Num)
{
    uint32_t uiDataLength = Pic_Size;
    uint32_t uiPic_Addr = NULL;
    uint16_t usPic_Data;
    static u8 Read_data[BLOCK_SIZE] = {0};  // 图像数据缓冲区

    //判断是第几张图片，设置存放位置
    switch (Pic_Num)
    {
    case 1:
        uiPic_Addr = Flash_Pic1_Addr;
        break;

    case 2:
        uiPic_Addr = Flash_Pic2_Addr;
        break;

    case 3:
        uiPic_Addr = Flash_Pic3_Addr;
        break;

    default:
        //uiPic_Addr = Flash_Pic1_Addr;
        break;
    }

    /*
        //硬件SPI方式
        //选择Flash芯片：CS输出低电平
        FLASH_CS_LOW();
        //硬件sip读取外部flash
        //发送命令，读取数据
        spi_byte_write(SPIF_READDATA);
        //发送地址高字节
        spi_byte_write((uiPic_Addr & 0xFF0000) >> 16);
        //发送地址中字节
        spi_byte_write((uiPic_Addr & 0xFF00) >> 8);
        //发送地址低字节
        spi_byte_write(uiPic_Addr & 0xFF);
        //设置窗口大小
        LCD_Address_Set(0, 0, 480, 320);
        //开始传输数据
        while (uiDataLength)
        {
            //从Flash读取数据，组成十六位数据
            usPic_Data = spi_byte_read();
            usPic_Data <<= 8;
            usPic_Data += spi_byte_read();
            //将数据写入到TFT-LCD屏幕中
            LCD_WR_DATA(usPic_Data);
            uiDataLength -= 2;
        }
        */

    //硬件SPI（8位传输）+DMA方式
    while (uiDataLength)
    {
        spiflash_read(Read_data, uiPic_Addr, BLOCK_SIZE);

        for (u32 i = 0; i < BLOCK_SIZE; i += 2)
        {
            usPic_Data = Read_data[i];
            usPic_Data <<= 8;
            usPic_Data = usPic_Data + Read_data[i + 1];
            LCD_WR_DATA(usPic_Data);
        }

        uiPic_Addr += BLOCK_SIZE;
        uiDataLength -= BLOCK_SIZE;
    }

    //禁用Flash芯片：CS引脚输出高电平
    //FLASH_CS_HIGH();
}

void DrawProgressBar(u16 x1, u16 y1, u16 x2, u16 y2,u8 scale,u16 color)
{
	u16 Bar_Length = y2-y1;
	u16 len = Bar_Length*((float)(100-scale)/100);
	
	LCD_Fill(x1,y1,x2,y1+len,0x18a3);
	LCD_Fill(x1,y1+len,x2,y2,0xfd64);
	LCD_ShowPicture(x1-2,y1+len,14,12,gImage_1);
}

