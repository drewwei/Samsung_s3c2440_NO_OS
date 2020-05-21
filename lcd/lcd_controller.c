#include "klist.h"
#include "lcd.h"
#include "string.h"

p_lcd_controller_t  g_lcd_controller; /*全局变量,用于记录选中的lcd控制器*/

extern struct list_head lcd_controller_list;

void lcd_controller_init(p_lcd_params_t p_params)
{
	g_lcd_controller->init(p_params);
}

void lcd_controller_enable(void)
{
	g_lcd_controller->enable();
}

void lcd_controller_disable(void)
{
	g_lcd_controller->disable();
}

void register_lcd_controller(p_lcd_controller_t plcdcon)
{
	list_add_tail(&(plcdcon->list), &lcd_controller_list); /*添加到全局链表lcd_controller_list中*/
}

int select_lcd_controller(const char *name)
{
	struct list_head *post;
	list_for_each(post, &lcd_controller_list)
	{
		if(!strcmp(((p_lcd_controller_t)post)->name, name))
		{
			g_lcd_controller = (p_lcd_controller_t)post;  /*由于链表成员变量在结构体的第一个位置处，该处的地址就是结构体的地址*/
			return 0;
		}	
		
	}
	return -1;
}