#include "bsp_lcd1602.h"

/*
需要移植的代码：
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//LCD1602使能端口控制
#define BSP_LCD1602_EN_H		GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define BSP_LCD1602_EN_L		GPIO_ResetBits(GPIOA,GPIO_Pin_6)

//LCD1602读/写端口控制
#define BSP_LCD1602_RW_H		GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define BSP_LCD1602_RW_L		GPIO_ResetBits(GPIOA,GPIO_Pin_7)

//LCD1602指令/数据端口控制
#define BSP_LCD1602_RS_H		GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define BSP_LCD1602_RS_L		GPIO_ResetBits(GPIOB,GPIO_Pin_0)

//LCD1602数据输出端口控制
#define BSP_LCD1602_D0_H		GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define BSP_LCD1602_D0_L		GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define BSP_LCD1602_D1_H		GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define BSP_LCD1602_D1_L		GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define BSP_LCD1602_D2_H		GPIO_SetBits(GPIOB,GPIO_Pin_5)	
#define BSP_LCD1602_D2_L		GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define BSP_LCD1602_D3_H		GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define BSP_LCD1602_D3_L		GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define BSP_LCD1602_D4_H		GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define BSP_LCD1602_D4_L		GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define BSP_LCD1602_D5_H		GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define BSP_LCD1602_D5_L		GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define BSP_LCD1602_D6_H		GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define BSP_LCD1602_D6_L		GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define BSP_LCD1602_D7_H		GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define BSP_LCD1602_D7_L		GPIO_ResetBits(GPIOB,GPIO_Pin_15)

//接口函数-LCD1602毫秒级延时
#define BSP_LCD1602_DelayMs_Port(x)		CoTickDelay(x)

//接口函数-LCD1602微秒级延时
#define BSP_LCD1602_DelayUs_Port(x)   delay_us(x)

//接口函数-LCD1602初始化IO口
void BSP_LCD1602_IO_Init_Port(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//EN RW推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//RS推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//Data口推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/************ low level data pushing commands **********/

/*
 ************************************************************
 *  名称：	BSP_LCD1602_PulseEnable
 *  功能：	EN引脚动作一次
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_PulseEnable(void)
{
	BSP_LCD1602_EN_L;
	BSP_LCD1602_DelayUs_Port(1);
	BSP_LCD1602_EN_H;		
	BSP_LCD1602_DelayUs_Port(1);		// enable pulse must be >450ns
	BSP_LCD1602_EN_L;		
	BSP_LCD1602_DelayUs_Port(100);		// commands need > 37us to settle
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_Write8Bits
 *  功能：	写入8bit数据
 *	输入：	value-8bit数据
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_Write8Bits(u8 value)
{
	if(value&0X01)	BSP_LCD1602_D0_H;//DB0
	else	BSP_LCD1602_D0_L;
	if(value&0X02)	BSP_LCD1602_D1_H;//DB1
	else 	BSP_LCD1602_D1_L;
	if(value&0X04)	BSP_LCD1602_D2_H;//DB2
	else	BSP_LCD1602_D2_L;
	if(value&0X08)	BSP_LCD1602_D3_H;//DB3
	else	BSP_LCD1602_D3_L;
	if(value&0X10)	BSP_LCD1602_D4_H;//DB4
	else	BSP_LCD1602_D4_L;
	if(value&0X20)	BSP_LCD1602_D5_H;//DB5
	else	BSP_LCD1602_D5_L;
	if(value&0X40)	BSP_LCD1602_D6_H;//DB6
	else	BSP_LCD1602_D6_L;
	if(value&0X80)	BSP_LCD1602_D7_H;//DB7
	else	BSP_LCD1602_D7_L;
	
	BSP_LCD1602_PulseEnable();
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_Send
 *  功能：	写入命令或数据
 *	输入：	value-数据/命令  mdoe-命令数据选择0,1
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_Send(u8 value,u8 mode)
{
	if(mode == 0)
	{
		BSP_LCD1602_RS_L;
	}
	else if(mode == 1)
	{
		BSP_LCD1602_RS_H;
	}
	else
	{
		return;//错误
	}
	
	BSP_LCD1602_RW_L;
	BSP_LCD1602_Write8Bits(value);
}


/*********** mid level commands, for sending data/cmds */

/*
 ************************************************************
 *  名称：	BSP_LCD1602_SendCommand
 *  功能：	发送命令
 *	输入：	value-命令
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_SendCommand(u8 value)
{
	BSP_LCD1602_Send(value,0);
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_SendData
 *  功能：	发送数据
 *	输入：	value-数据
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_SendData(u8 value)
{
	BSP_LCD1602_Send(value,1);
}


/********** high level commands, for the user! */

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

u8 _displayfunction;
u8 _displaycontrol;
u8 _displaymode;

u8 _numlines,_currline;

/*
 ************************************************************
 *  名称：	BSP_LCD1602_Clear
 *  功能：	清屏
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_Clear(void)
{
	BSP_LCD1602_SendCommand(LCD_CLEARDISPLAY);	// clear display, set cursor position to zero
	BSP_LCD1602_DelayMs_Port(2);	// this command takes a long time!
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_Clear
 *  功能：	光标返回到原点
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_Home(void)
{
	BSP_LCD1602_SendCommand(LCD_RETURNHOME);	// set cursor position to zero
	BSP_LCD1602_DelayMs_Port(2);	// this command takes a long time!
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_SetCursor
 *  功能：	设定光标位置
 *	输入：	col-列  row-行(每行最多可写入40个值)
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_SetCursor(u8 col,u8 row)
{
	u16 row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	
  if ( row >= _numlines )
	{
    row = _numlines-1;    // we count rows starting w/0
  }
  
  BSP_LCD1602_SendCommand(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_NoDisplay
 *  功能：	关闭显示(快速)
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_NoDisplay(void)
{
	_displaycontrol &= ~LCD_DISPLAYON;
  BSP_LCD1602_SendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_NoDisplay
 *  功能：	打开显示(快速)
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_Display(void)
{
	_displaycontrol |= LCD_DISPLAYON;
  BSP_LCD1602_SendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_NoCursor
 *  功能：	隐藏光标
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_NoCursor(void)
{
	_displaycontrol &= ~LCD_CURSORON;
  BSP_LCD1602_SendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}	

/*
 ************************************************************
 *  名称：	BSP_LCD1602_Cursor
 *  功能：	显示光标
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_Cursor(void)
{
	_displaycontrol |= LCD_CURSORON;
  BSP_LCD1602_SendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_NoBlink
 *  功能：	光标不闪烁
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_NoBlink(void)
{
	_displaycontrol &= ~LCD_BLINKON;
  BSP_LCD1602_SendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_NoBlink
 *  功能：	光标闪烁
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_Blink(void)
{
	_displaycontrol |= LCD_BLINKON;
  BSP_LCD1602_SendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_ScrollDisplayLeft
 *  功能：	显示向左滚动一个位置(不改变RAM)
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_ScrollDisplayLeft(void)
{
	BSP_LCD1602_SendCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_ScrollDisplayRight
 *  功能：	显示向右滚动一个位置(不改变RAM)
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_ScrollDisplayRight(void)
{
	BSP_LCD1602_SendCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_LeftToRight
 *  功能：	设置写入LCD文本自增方向从左到右
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_LeftToRight(void)
{
	_displaymode |= LCD_ENTRYLEFT;
  BSP_LCD1602_SendCommand(LCD_ENTRYMODESET | _displaymode);
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_LeftToRight
 *  功能：	设置写入LCD文本自增方向从右到左
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_RightToLeft(void)
{
	_displaymode &= ~LCD_ENTRYLEFT;
  BSP_LCD1602_SendCommand(LCD_ENTRYMODESET | _displaymode);
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_AutoScroll
 *  功能：	自动滚动8个格
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_AutoScroll(void)
{
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
  BSP_LCD1602_SendCommand(LCD_ENTRYMODESET | _displaymode);
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_NoAutoScroll
 *  功能：	关闭自动滚动8个格
 *	输入：	无
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_NoAutoScroll(void)
{
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  BSP_LCD1602_SendCommand(LCD_ENTRYMODESET | _displaymode);
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_CreateChar
 *  功能：	创建自定义字符
 *	输入：	loaction-位置   charmap[]-字符点阵数组
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_CreateChar(u8 location,u8* charmap)
{
	location &= 0x7; // we only have 8 locations 0-7
  BSP_LCD1602_SendCommand(LCD_SETCGRAMADDR | (location << 3));
  
	for (u8 i=0; i<8; i++)
	{
    BSP_LCD1602_SendData(charmap[i]);
  }
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_SendStr
 *  功能：	发送字符串
 *	输入：	str-字符串
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_SendStr(char *str)
{
	while(0 != *str)
	{
		BSP_LCD1602_SendData(*str);
		str++;
	}
	
}

/*
 ************************************************************
 *  名称：	BSP_LCD1602_Begin
 *  功能：	LCD1602最初设定
 *	输入：	cols-列  lines-行
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_Begin(u8 cols,u8 lines)
{
	_displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
	
	if (lines > 1)
	{
    _displayfunction |= LCD_2LINE;
  }
	_numlines = lines;
	_currline = 0;
	
	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	BSP_LCD1602_DelayMs_Port(50);
	
	// Now we pull both RS and R/W low to begin commands
	BSP_LCD1602_RS_L;
	BSP_LCD1602_EN_L;
	BSP_LCD1602_RW_L;

	// this is according to the hitachi HD44780 datasheet
	// page 45 figure 23
	// Send function set command sequence
	BSP_LCD1602_SendCommand(LCD_FUNCTIONSET | _displayfunction);
	BSP_LCD1602_DelayMs_Port(5);  // wait more than 4.1ms

	// second try
	BSP_LCD1602_SendCommand(LCD_FUNCTIONSET | _displayfunction);
	BSP_LCD1602_DelayMs_Port(1);

	// third go
	BSP_LCD1602_SendCommand(LCD_FUNCTIONSET | _displayfunction);
	
	// finally, set # lines, font size, etc.
  BSP_LCD1602_SendCommand(LCD_FUNCTIONSET | _displayfunction);  
	
	BSP_LCD1602_DelayMs_Port(10);
	
  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  BSP_LCD1602_Display();
	BSP_LCD1602_DelayMs_Port(1);

  // clear it off
  BSP_LCD1602_Clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  BSP_LCD1602_SendCommand(LCD_ENTRYMODESET | _displaymode);
	BSP_LCD1602_DelayMs_Port(1);
}


/*
 ************************************************************
 *  名称：	BSP_LCD1602_Init
 *  功能：	LCD1602初始化
 *	输入：	cols-列  lines-行
 *	输出：  无
 ************************************************************
*/
void BSP_LCD1602_Init(u8 cols,u8 lines)
{
	BSP_LCD1602_IO_Init_Port();
	BSP_LCD1602_Begin(cols,lines);
}













