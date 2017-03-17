#include "bmsship.h"
#include "main.h"
enum
{
    E_BSS_ENV_INIT,
	E_BSS_ENV_USART,
	E_BSS_TIMER_EVENT,
	E_BSS_KEY_DETECT,
	E_BSS_CHARGE_DETECT,
	E_BSS_AFE_STATE_MNG,
	E_BSS_FUEL_GUAGE_MNG,
	E_BSS_DSG_MNG,
	E_FAUL_DEAL,
};
unsigned char s_bmsShipState;
void BmsShip(void);
void BSS_ENV_INIT(void);
void BSS_UART(void);
void BSS_TimerEvent(void);
void BSS_KeyDetect(void);
void BSS_ChargeDetect(void);
void BSS_AfeStateMng(void);
void BSS_FuelGaugeMng(void);
void BSS_DsgChgMng(void);
void BSS_FaultDeal(void);
extern unsigned char g_bmsState;
FunctionTable const BmsShipTable[] = 
{
    BSS_ENV_INIT,
	BSS_UART,
	BSS_TimerEvent,
	BSS_KeyDetect,
	BSS_ChargeDetect,
	BSS_AfeStateMng,
	BSS_FuelGaugeMng,
	BSS_DsgChgMng,
	BSS_FaultDeal,
};
void BSS_BmsShip(void)
{
    s_bmsShipState = E_BSS_ENV_INIT;
    (*BmsShipTable[s_bmsShipState])();            
}
void BSS_ENV_INIT(void)
{
    
}
void BSS_UART(void)
{
    
}
void BSS_TimerEvent(void)
{
    
}
void BSS_KeyDetect(void)
{
    
}
void BSS_ChargeDetect(void)
{
    
};
void BSS_AfeStateMng(void)
{
    
}
void BSS_FuelGaugeMng(void)
{
    
}
void BSS_DsgChgMng(void)
{
    
}
void BSS_FaultDeal(void)
{
    
}

