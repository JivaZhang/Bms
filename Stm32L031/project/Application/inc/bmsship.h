#ifndef __BMS_SHIP_H__
extern void BSS_BmsShip(void);
extern void BSS_ENV_INIT(void);
extern void BSS_UART(void);
extern void BSS_TimerEvent(void);
extern void BSS_KeyDetect(void);
extern void BSS_ChargeDetect(void);
extern void BSS_AfeStateMng(void);
extern void BSS_FuelGaugeMng(void);
extern void BSS_DsgChgMng(void);
extern void BSS_FaultDeal(void);
extern void BmsShip(void);
#endif //__BMS_SHIP_H__