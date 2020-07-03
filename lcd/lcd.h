
#ifndef  __LCD
#define  __LCD

#include "klist.h" 
/*NORMAL:正常极性
 *INVERT:发转极性
 */
enum{
	NORMAL =0,
	INVERT,
};

typedef struct pins_polarity {
	int de;		/* normal: 高电平时可以传输数据 */
	int pwren;	/* normal: 高电平有效 */
	int vclk;	/*normal:在下降沿获取数据*/
	int rgb;	/*normal:高电平表示1*/
	int hsync;	/*normal:高脉冲*/
	int vsync;	/*normal:高脉冲*/
}pins_polarity_t, *p_pins_polarity_t;

typedef struct time_sequence {
	/*垂直方向*/
	int tvp; 	/*vsync脉冲宽度*/
	int tvb;	/*垂直上肩宽度 Verticl Back porch*/
	int tvf;	/*垂直下肩宽度 Verticl Front porch*/
	
	/*水平方向*/
	int thp;	/*hsync脉冲宽度*/
	int thb;	/*水平左肩宽度，Horizontal Back porch*/
	int thf;	/*水平右肩宽度，Horizontal Back porch*/
	
	int vclk;
}time_sequece_t, *p_time_sequence_t;

typedef struct lcd_params {
	/*lcd 名字*/
	const char *name;
	/*引脚极性*/
	pins_polarity_t pins_pol;
	
	/*时序*/
	time_sequece_t time_seq;
	
	/*分辨率， bpp*/
	int xres;
	int yres;
	int bpp;
	
	/*frambuffer的地址*/
	unsigned int fb_base;
}lcd_params_t, *p_lcd_params_t;


typedef struct lcd_controller {
	struct list_head list;
	const char *name;
	void (*init)(p_lcd_params_t);
	void (*enable)(void);
	void (*disable)(void);	
}lcd_controller_t, *p_lcd_controller_t;


#endif