#include "oled.h"
#include "font.h"

#include <stdarg.h>
#include <stdio.h>

#define OLED_PAGES  8                   // OLED页数
#define OLED_ROW    8 * OLED_PAGES      // OLED行数 8页每页8行
#define OLED_COLUMN 128                 // OLED列数

uint8_t OLED_GRAM[OLED_PAGES][OLED_COLUMN];

struct OLED_Config g_oledCfg;

// 字体设置
static const ASCIIFont *currFont = &afont16x8;

static void OLED_ConfigDisplay(I2C_HandleTypeDef *handle, uint8_t i2cAddr)
{
    g_oledCfg.i2cHandle = handle;
    g_oledCfg.i2cAddr = i2cAddr;
}

static int OLED_WriteCmd(uint8_t cmd)
{
    uint8_t buf[2];
    buf[0] = 0x00;
    buf[1] = cmd;
    return HAL_I2C_Master_Transmit(g_oledCfg.i2cHandle, g_oledCfg.i2cAddr, buf, sizeof(buf), HAL_MAX_DELAY);
}

static int OLED_WriteData(uint8_t *data, size_t len)
{
    uint8_t buf[256] = {0};
    buf[0] = 0x40;
    if (len > 128) {
        len = 128;
    }
    memcpy(buf+1, data, len);

    // len+1包含了0x40作为数据开头
    return HAL_I2C_Master_Transmit(g_oledCfg.i2cHandle, g_oledCfg.i2cAddr, buf, len+1, HAL_MAX_DELAY);
}

static int OLED_InitDisplay()
{
    // 等待OLED初始化
    HAL_Delay(20);

    if (g_oledCfg.i2cHandle->State != HAL_I2C_STATE_READY) {
        return -1;
    }

    /*写入一系列的命令，对OLED进行初始化配置*/
    OLED_WriteCmd(0xAE);	//设置显示开启/关闭，0xAE关闭，0xAF开启
        
    OLED_WriteCmd(0xD5);	//设置显示时钟分频比/振荡器频率
    OLED_WriteCmd(0x80);	//0x00~0xFF
        
    OLED_WriteCmd(0xA8);	//设置多路复用率
    OLED_WriteCmd(0x3F);	//0x0E~0x3F
        
    OLED_WriteCmd(0xD3);	//设置显示偏移
    OLED_WriteCmd(0x00);	//0x00~0x7F
        
    OLED_WriteCmd(0x40);	//设置显示开始行，0x40~0x7F
        
    OLED_WriteCmd(0xA1);	//设置左右方向，0xA1正常，0xA0左右反置
        
    OLED_WriteCmd(0xC8);	//设置上下方向，0xC8正常，0xC0上下反置
    OLED_WriteCmd(0xDA);	//设置COM引脚硬件配置
    OLED_WriteCmd(0x12);
        
    OLED_WriteCmd(0x81);	//设置对比度
    OLED_WriteCmd(0xCF);	//0x00~0xFF
    OLED_WriteCmd(0xD9);	//设置预充电周期
    OLED_WriteCmd(0xF1);
    OLED_WriteCmd(0xDB);	//设置VCOMH取消选择级别
    OLED_WriteCmd(0x30);
    OLED_WriteCmd(0xA4);	//设置整个显示打开/关闭
    OLED_WriteCmd(0xA6);	//设置正常/反色显示，0xA6正常，0xA7反色
    OLED_WriteCmd(0x8D);	//设置充电泵
    OLED_WriteCmd(0x14);
    OLED_WriteCmd(0xAF);	//开启显示

    return 0;
}

int OLED_Init()
{
    OLED_ConfigDisplay(&hi2c2, 0x78);
    OLED_InitDisplay();

    return 0;
}

void OLED_SetPos(uint8_t X, uint8_t Y)
{
    if (X >= OLED_COLUMN || Y >= OLED_ROW) {
        return;
    }
    uint8_t pg = Y / OLED_PAGES;

    // b0~b7: 设置页地址
    OLED_WriteCmd(0xb0 + pg);

   // 设置列地址
   // 低4位
   OLED_WriteCmd((X & 0x0f));
   // 高4位
   OLED_WriteCmd(((X & 0xf0) >> 4) | 0x10);
}

void OLED_SetCursor(uint8_t page, uint8_t X)
{
#ifdef OLED_1INCH3
    X += 2;
#endif

    // b0~b7: 设置页地址
    OLED_WriteCmd(0xb0 + page);

   // 设置列地址
   // 低4位
   OLED_WriteCmd((X & 0x0f));
   // 高4位
   OLED_WriteCmd(((X & 0xf0) >> 4) | 0x10);
}

void OLED_Fill(uint8_t data)
{   
    memset(OLED_GRAM, data, sizeof(OLED_GRAM));
}

void OLED_NewFrame()
{
    OLED_Fill(0x00);
}

// 逻辑参考SetPixel
/*
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t width, uint8_t height)
{
   // 这个是像素级别操作, 和DrawImage不同
   for (int16_t i = Y; i < height; i++) {
        for (int16_t j = X; j < width; j++) {
            if (i >= 0 && i < OLED_ROW && j >= 0 && j < OLED_COLUMN) {
                // 这里只会清零XY对应的那个像素
                OLED_GRAM[i / 8][j] &= ~(0x01 << (i % 8));
            }
        }
   }
}
*/

void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
	int16_t i, j;
	
	/*参数检查，保证指定区域不会超出屏幕范围*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	if (X + Width > 128) {Width = 128 - X;}
	if (Y + Height > 64) {Height = 64 - Y;}
	
	for (j = Y; j < Y + Height; j ++)		//遍历指定页
	{
		if(j / 8 < 0){continue;}
		for (i = X; i < X + Width; i ++)	//遍历指定列
		{
			if(i < 0){continue;}
			OLED_GRAM[j / 8][i] &= ~(0x01 << (j % 8));	//将显存数组指定数据清零
		}
	}
}

void OLED_SetPixel(int X, int Y)
{
    // 越界
    if (X >= OLED_COLUMN || Y >= OLED_ROW) {
        return;
    }

    // 第一维是所在页数, 第二维为列数
    OLED_GRAM[Y/8][X] |= 0x01 << (Y % 8);
}

// 刷新显存数据到屏幕
void OLED_ShowFrame()
{
    for (int i = 0; i < OLED_PAGES; i++) {
        OLED_SetCursor(i, 0);
        OLED_WriteData(OLED_GRAM[i], OLED_COLUMN);
    }
}

// image需要按列行式进行存储
/*
void OLED_ShowImage(int16_t X, int16_t Y, uint8_t width, uint8_t height, const uint8_t *image)
{
    // 先清空那片区域
    OLED_ClearArea(X, Y, width, height);

    // 画的内容占多少页面（向上取整）
    uint8_t pages = ((height - 1) / 8) + 1;
    // 开始在哪页画
    uint8_t startPage = Y / 8;

    uint8_t shift = Y % 8;

    // 处理每一页
    for (uint8_t i = 0; i < pages; i++) {
        // 处理每一列
        for (uint8_t j = 0; j < width; j++) {
            // 只画范围内
            if (X + j >= 0 && X + j < OLED_COLUMN) {
                if (startPage + i >= 0 && startPage + i < OLED_ROW) {
                    // 低位往高位移动
                    OLED_GRAM[startPage + i][X + j] |= (image[i * width + j] << shift);
                }
                // 当前页有跨到下一页的处理
                if (startPage + i + 1 >= 0 && startPage + i + 1 < OLED_ROW) {
                    // 高位往低位移动
                    OLED_GRAM[startPage + i + 1][X + j] |= (image[i * width + j] >> (8 - shift));
                }
            }
        }
    }
}
*/
void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
	uint8_t i, j;
	
	/*参数检查，保证指定图像不会超出屏幕范围*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	
	/*将图像所在区域清空*/
	OLED_ClearArea(X, Y, Width, Height);
	
	/*遍历指定图像涉及的相关页*/
	/*(Height - 1) / 8 + 1的目的是Height / 8并向上取整*/
	for (j = 0; j < (Height - 1) / 8 + 1; j ++)
	{
		/*遍历指定图像涉及的相关列*/
		for (i = 0; i < Width; i ++)
		{
			/*超出边界，则跳过显示*/
			if (X + i > 127) {break;}
			if (Y / 8 + j > 7) {return;}
			if (X + i < 0)	{continue;}
			if (Y / 8 + j < 0) {continue;}
			
			/*显示图像在当前页的内容*/
			OLED_GRAM[Y / 8 + j][X + i] |= Image[j * Width + i] << (Y % 8);
			
			/*超出边界，则跳过显示*/
			/*使用continue的目的是，下一页超出边界时，上一页的后续内容还需要继续显示*/
			if (Y / 8 + j + 1 > 7) {continue;}
			
			/*显示图像在下一页的内容*/
			OLED_GRAM[Y / 8 + j + 1][X + i] |= Image[j * Width + i] >> (8 - Y % 8);
		}
	}
}

void OLED_ShowChar(uint8_t X, uint8_t Y, char c)
{
    uint8_t chrIndex = c - ' ';
    uint32_t arrayIndex = (((currFont->h) + 7) / 8) * currFont->w;

    // printf("chrIndex: %u, arrayIndex: %lu\r\n", chrIndex, arrayIndex);
    OLED_ShowImage(X, Y, currFont->w, currFont->h, currFont->chars + chrIndex * arrayIndex);
}

void OLED_ShowString(uint8_t X, uint8_t Y, const char *str)
{
    while (*str) {
        OLED_ShowChar(X, Y, *str);
        X += currFont->w;
        str++;
    }
}

// void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t width, uint8_t height)
// {
//     for (uint8_t i = 0; i < height; i++) {
//         for (uint8_t j = 0; j < width; j++) {
//             OLED_SetPixel(X+j, Y+i);
//         }
//     }
// }

void OLED_DrawPoint(uint8_t X, uint8_t Y)
{
	/*参数检查，保证指定位置不会超出屏幕范围*/
	if (X > 127) {return;}
	if (Y > 63) {return;}
	
	/*将显存数组指定位置的一个Bit数据置1*/
	OLED_GRAM[Y / 8][X] |= 0x01 << (Y % 8);
}

void OLED_DrawRectangle(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled)
{
	uint8_t i, j;
	if (!IsFilled)		//指定矩形不填充
	{
		/*遍历上下X坐标，画矩形上下两条线*/
		for (i = X; i < X + Width; i ++)
		{
			OLED_DrawPoint(i, Y);
			OLED_DrawPoint(i, Y + Height - 1);
		}
		/*遍历左右Y坐标，画矩形左右两条线*/
		for (i = Y; i < Y + Height; i ++)
		{
			OLED_DrawPoint(X, i);
			OLED_DrawPoint(X + Width - 1, i);
		}
	}
	else				//指定矩形填充
	{
		/*遍历X坐标*/
		for (i = X; i < X + Width; i ++)
		{
			/*遍历Y坐标*/
			for (j = Y; j < Y + Height; j ++)
			{
				/*在指定区域画点，填充满矩形*/
				OLED_DrawPoint(i, j);
			}
		}
	}
}

void OLED_DrawLine(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1)
{
	int16_t x, y, dx, dy, d, incrE, incrNE, temp;
	int16_t x0 = X0, y0 = Y0, x1 = X1, y1 = Y1;
	uint8_t yflag = 0, xyflag = 0;
	
	if (y0 == y1)		//横线单独处理
	{
		/*0号点X坐标大于1号点X坐标，则交换两点X坐标*/
		if (x0 > x1) {temp = x0; x0 = x1; x1 = temp;}
		
		/*遍历X坐标*/
		for (x = x0; x <= x1; x ++)
		{
			OLED_DrawPoint(x, y0);	//依次画点
		}
	}
	else if (x0 == x1)	//竖线单独处理
	{
		/*0号点Y坐标大于1号点Y坐标，则交换两点Y坐标*/
		if (y0 > y1) {temp = y0; y0 = y1; y1 = temp;}
		
		/*遍历Y坐标*/
		for (y = y0; y <= y1; y ++)
		{
			OLED_DrawPoint(x0, y);	//依次画点
		}
	}
	else				//斜线
	{
		/*使用Bresenham算法画直线，可以避免耗时的浮点运算，效率更高*/
		/*参考文档：https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf*/
		/*参考教程：https://www.bilibili.com/video/BV1364y1d7Lo*/
		
		if (x0 > x1)	//0号点X坐标大于1号点X坐标
		{
			/*交换两点坐标*/
			/*交换后不影响画线，但是画线方向由第一、二、三、四象限变为第一、四象限*/
			temp = x0; x0 = x1; x1 = temp;
			temp = y0; y0 = y1; y1 = temp;
		}
		
		if (y0 > y1)	//0号点Y坐标大于1号点Y坐标
		{
			/*将Y坐标取负*/
			/*取负后影响画线，但是画线方向由第一、四象限变为第一象限*/
			y0 = -y0;
			y1 = -y1;
			
			/*置标志位yflag，记住当前变换，在后续实际画线时，再将坐标换回来*/
			yflag = 1;
		}
		
		if (y1 - y0 > x1 - x0)	//画线斜率大于1
		{
			/*将X坐标与Y坐标互换*/
			/*互换后影响画线，但是画线方向由第一象限0~90度范围变为第一象限0~45度范围*/
			temp = x0; x0 = y0; y0 = temp;
			temp = x1; x1 = y1; y1 = temp;
			
			/*置标志位xyflag，记住当前变换，在后续实际画线时，再将坐标换回来*/
			xyflag = 1;
		}
		
		/*以下为Bresenham算法画直线*/
		/*算法要求，画线方向必须为第一象限0~45度范围*/
		dx = x1 - x0;
		dy = y1 - y0;
		incrE = 2 * dy;
		incrNE = 2 * (dy - dx);
		d = 2 * dy - dx;
		x = x0;
		y = y0;
		
		/*画起始点，同时判断标志位，将坐标换回来*/
		if (yflag && xyflag){OLED_DrawPoint(y, -x);}
		else if (yflag)		{OLED_DrawPoint(x, -y);}
		else if (xyflag)	{OLED_DrawPoint(y, x);}
		else				{OLED_DrawPoint(x, y);}
		
		while (x < x1)		//遍历X轴的每个点
		{
			x ++;
			if (d < 0)		//下一个点在当前点东方
			{
				d += incrE;
			}
			else			//下一个点在当前点东北方
			{
				y ++;
				d += incrNE;
			}
			
			/*画每一个点，同时判断标志位，将坐标换回来*/
			if (yflag && xyflag){OLED_DrawPoint(y, -x);}
			else if (yflag)		{OLED_DrawPoint(x, -y);}
			else if (xyflag)	{OLED_DrawPoint(y, x);}
			else				{OLED_DrawPoint(x, y);}
		}	
	}
}

void OLED_Test()
{
    OLED_NewFrame();
    OLED_ShowFrame();
    
    /*
    int i = 0;
    for (;;) {
        if (i > 64-currFont->h) {
            i = 0;
        }
        OLED_NewFrame();
        OLED_ShowString(0, i, "Hello World");
        // OLED_ShowChar(0, 0, 'A');
        OLED_ShowFrame();
        HAL_Delay(100);
        i+=3;
    }
    */
    // OLED_ShowChar(0, 0, 'A');
    // OLED_ShowChar(0, 0, 'B');
    int i = 0;
    for (;;) {
        if (i >= 127) {
            i = 0;
        }
        i+=3;
        OLED_NewFrame();
        OLED_DrawRectangle(0, 0, i, 16, 1);
        OLED_ShowFrame();
        HAL_Delay(100);
    }
    

}

/**
  * 函    数：OLED显示汉字串
  * 参    数：X 指定汉字串左上角的横坐标，范围：0~127
  * 参    数：Y 指定汉字串左上角的纵坐标，范围：0~63
  * 参    数：Chinese 指定要显示的汉字串，范围：必须全部为汉字或者全角字符，不要加入任何半角字符
  *           显示的汉字需要在OLED_Data.c里的OLED_CF16x16数组定义
  *           未找到指定汉字时，会显示默认图形（一个方框，内部一个问号）
  * 返 回 值：无
  * 说    明：调用此函数后，要想真正地呈现在屏幕上，还需调用更新函数
  */
void OLED_ShowChinese(uint8_t X, uint8_t Y, const char *Chinese)
{
	uint8_t pChinese = 0;
	uint8_t pIndex;
	uint8_t i;
	char SingleChinese[OLED_CHN_CHAR_WIDTH + 1] = {0};
	
	for (i = 0; Chinese[i] != '\0'; i ++)		//遍历汉字串
	{
		SingleChinese[pChinese] = Chinese[i];	//提取汉字串数据到单个汉字数组
		pChinese ++;							//计次自增
		
		/*当提取次数到达OLED_CHN_CHAR_WIDTH时，即代表提取到了一个完整的汉字*/
		if (pChinese >= OLED_CHN_CHAR_WIDTH)
		{
			pChinese = 0;		//计次归零
			
			/*遍历整个汉字字模库，寻找匹配的汉字*/
			/*如果找到最后一个汉字（定义为空字符串），则表示汉字未在字模库定义，停止寻找*/
			for (pIndex = 0; strcmp(OLED_CF16x16[pIndex].Index, "") != 0; pIndex ++)
			{
				/*找到匹配的汉字*/
				if (strcmp(OLED_CF16x16[pIndex].Index, SingleChinese) == 0)
				{
					break;		//跳出循环，此时pIndex的值为指定汉字的索引
				}
			}
			
			/*将汉字字模库OLED_CF16x16的指定数据以16*16的图像格式显示*/
			OLED_ShowImage(X + ((i + 1) / OLED_CHN_CHAR_WIDTH - 1) * 16, Y, 16, 16, OLED_CF16x16[pIndex].Data);
		}
	}
}

void OLED_SetASCIIFont(enum OLED_ASCII_FONT_SIZE font)
{
	if (font == O_FONT_8X6) {
		currFont = &afont8x6;
	} else if (font == O_FONT_12X6) {
		currFont = &afont12x6;
	} else if (font == O_FONT_16X8) {
		currFont = &afont16x8;
	} else if (font == O_FONT_24X12) {
		currFont = &afont24x12;
	} else {
		currFont = &afont16x8;
	}
}

void OLED_Printf(uint8_t X, uint8_t Y, char *format, ...)
{
	char String[30];						//定义字符数组
	va_list arg;							//定义可变参数列表数据类型的变量arg
	va_start(arg, format);					//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);			//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);							//结束变量arg
	OLED_ShowString(X, Y, String);			//OLED显示字符数组（字符串）
}