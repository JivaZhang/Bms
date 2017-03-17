#ifndef __LED_H__ 
#define __LED_H__ 
void API_PowerLedOn(void);
void API_PowerLedOff(void);
void API_FuelGaugeLedExchange(unsigned char percent,char* pLedState);
void API_FuelGaugeLedDisplay(char ledState);
#endif