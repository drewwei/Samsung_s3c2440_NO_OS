#include "lcd.h" 
#include "lcd_controller.h"
#define LCD_FB_BASE		0X33C00000

struct lcd_params lcd_4_3_parmas = {
	.name = "lcd_4.3",
	.pins_pol = {
		.de    = NORMAL,	/* normal: 高电平时可以传输数据 */
		.pwren = NORMAL,	/* normal: 高电平有效 */
		.vclk  = NORMAL,	/* normal: 在下降沿获取数据 */
		.rgb   = NORMAL,	/* normal: 高电平表示1 */
		.hsync = INVERT,	/* normal: 高脉冲 */
		.vsync = INVERT,	/* normal: 高脉冲 */
	},
	.time_seq = {
		/* 垂直方向 */
		.tvp=	10, /* vysnc脉冲宽度 */
		.tvb=	2,  /* 上边黑框, Vertical Back porch */
		.tvf=	2,  /* 下边黑框, Vertical Front porch */

		/* 水平方向 */
		.thp=	41, /* hsync脉冲宽度 */
		.thb=	2,  /* 左边黑框, Horizontal Back porch */
		.thf=	2,  /* 右边黑框, Horizontal Front porch */

		.vclk=	9,  /* MHz */
	},
	.xres = 480,
	.yres = 272,
	.bpp  = 16,
	
	.fb_base = LCD_FB_BASE,
};

extern struct lcd_controller s3c2440_lcd_controller;

void lcd_init(void)
{
	register_lcd_controller(&s3c2440_lcd_controller);
	select_lcd_controller("s3c2440");
	lcd_controller_init(&lcd_4_3_parmas);
	lcd_controller_enable();	
}