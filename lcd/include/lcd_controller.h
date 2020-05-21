#ifndef  __LCD_CONTRLLER
#define  __LCD_CONTRLLER
#include "lcd.h" 
extern void register_lcd_controller(p_lcd_controller_t plcdcon);
extern void lcd_controller_init(p_lcd_params_t p_params);
extern void lcd_controller_enable(void);
extern void lcd_controller_disable(void);
extern int select_lcd_controller(const char *name);
extern void lcd_init(void);
#endif