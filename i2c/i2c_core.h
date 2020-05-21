#ifndef 	__12C_CORE__
#define 	__12C_CORE__

extern struct i2c_controller * mach_i2c_controller(const char * name);
extern int register_i2c_controller(struct i2c_controller *p_i2c_controller);
extern void (*i2c_transfer)(struct i2c_msg *msgs,unsigned int cnt);

#endif