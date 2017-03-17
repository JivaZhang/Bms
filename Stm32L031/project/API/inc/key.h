#ifndef __KEY_H__ 
#define __KEY_H__ 

enum
{
    E_PWR_NO_PRESS,     //0
    E_PWR_SHORT_PRESS,  //1
	E_PWR_LONG_PRESS,   //2
};
void API_InitKeyVar(void);
int API_KeyScan(void);
#endif