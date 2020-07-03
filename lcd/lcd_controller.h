#ifndef  __LCD_CONTRLLER
#define  __LCD_CONTRLLER
#include "lcd.h" 
extern void register_lcd_controller(p_lcd_controller_t plcdcon);
extern void lcd_controller_init(p_lcd_controller_t p_lcd_controller,p_lcd_params_t p_params);
extern void lcd_controller_enable(p_lcd_controller_t  lcd_controller);
extern void lcd_controller_disable(p_lcd_controller_t  lcd_controller);
extern p_lcd_controller_t select_lcd_controller(const char *name);
extern void lcd_init(const char *name);

void regist_s3c2440_lcd_controller(void);
#endif