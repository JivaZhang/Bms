/*
  Fuel gauge modules created by Jesse on Jan 13,2017 at BriSky
*/
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_uart.h"
#include "main.h"
#include "./gauge.h"
#include "i2c.h"
#include "chargedischarge.h"
typedef char bool;
#define true    1
#define false   0
extern UART_HandleTypeDef huart2;

static void alert(void){}
static volatile char delay;
static void udelay(unsigned int us)
{
  while(us--) delay++;
}

static void mdelay(unsigned int ms)
{
  while(ms--){
    udelay(500);}
}



static unsigned long j_power;
static unsigned short j_capacity;//mAh
//static unsigned long j_energy;//mWh
static unsigned short j_remaining;
static unsigned short j_voltage;//
static  short j_temp;//
//static unsigned long j_volt;
static  short j_current;
static  short j_current10;
static  short j_current16;
static  long j_capacity_acc;
static unsigned short j_charge_num, j_discharge_num;
static unsigned short j_serial_num;
static unsigned char j_counted,j_cycle;//charge & discharge
static unsigned char j_percentage;
static unsigned char j_num_cell=6;
static unsigned char j_checksum;
static unsigned short j_state;
static unsigned short j_flags,j_flags_b;
static unsigned short j_device;
static unsigned short j_fwver,j_hwver;
static unsigned short j_chemid;
static unsigned short j_inner_temp,j_charge_volt,j_time2full;
static unsigned char safety_data[10];
static unsigned char charge_inhabit[6];
static unsigned char charge_cfg[14];
static unsigned char charge_termination[27];
static unsigned char config_data[60];
static unsigned char discharge[22];
static unsigned char mfr_info[32];
static unsigned char mfr_data[12];
static unsigned char lifetime_data[12];
static unsigned char pack_config[8];
static unsigned char lifetime_resolution[5];
static unsigned char power_data[12];
static unsigned char it_cfg[93];
static unsigned char current_th[11];
static unsigned char gauging_state[17];
static unsigned char R_a0[32];
static unsigned char R_a0x[32];
static float  calib_data[4];
static unsigned char security[24];

/*
Nominal capacity 标称容量	22000mAh @ 0.5C Discharge(放电)
Minimum capacity 最小容量	20482mAh @ 0.5C Discharge(放电)
Nominal voltage  标称电压	22.2V (cell 3.7V)
Open-Circuit Voltage 开路电压 	24.9V (cell 4.15V)
PACK Voltage(As of shipment)
电池电压(出货状态) 	22.8~23.4V(cell 3.8~3.9V) 
电压	4.2v	3.98V	3.87V	3.79V	3.7V	3.45V	2.75V
电量	100%	80%	60%	40%	20%	5%	0
*/
extern I2C_HandleTypeDef hi2c1;
static int I2C_Test(void)
{
	HAL_StatusTypeDef status;
	unsigned char tries = 3; //try times
	unsigned char timeout = 1;//timeout 
	status = HAL_I2C_IsDeviceReady(&hi2c1,0xaa,tries,timeout);
	if(status == HAL_OK)
	{
	    return 0; //状态正常
	}
	return 1;//状态错误
}

static int I2C_ReadReg(unsigned char reg,unsigned char *pData,uint16_t len)
{
    HAL_StatusTypeDef status;
    unsigned char r = reg;
	unsigned char timeout = 100;//timeout 
	status = I2C_Test();
	if(status != HAL_OK)
	{
	    return I2C_ERROR;//error
	}  
	status = HAL_I2C_Master_Transmit(&hi2c1,0xaa,&r,1,timeout);
	if (status != HAL_OK)
	{
	    return I2C_ERROR;//error;
	}
	
	do
	{
	    status = HAL_I2C_Master_Receive(&hi2c1,0xaa,pData,len,timeout);
	}while(status != HAL_OK);
        
	return I2C_OK;
}

static int I2C_WriteReg(unsigned char reg,unsigned char *data,uint16_t len)
{
	HAL_StatusTypeDef status;
	unsigned char da[4];
	da[0] = reg;
	da[1] = *data;
        da[2] = *(data+1);
	unsigned char timeout = 1;//timeout 
	status = I2C_Test();
	if(status != HAL_OK)
	{
	    return I2C_ERROR;//error
	}  
	status = HAL_I2C_Master_Transmit(&hi2c1,0xaa,da,len+1,timeout);
    if (status != HAL_OK)
	{
	    return I2C_ERROR;//error;
	}
	return I2C_OK;
}

static int control(unsigned short sub_cmd,unsigned short *data)
{
  int r;
  r = I2C_WriteReg(0, (unsigned char *)&sub_cmd,2);
  if (r==I2C_OK)
    r = I2C_ReadReg(0,(unsigned char *)data,2);
  return r;
}

static unsigned short df_ver(void)
{
  return control(12,&j_state);   //data flash version on the device
}

static unsigned short static_chem_checksum(void)
{
  return control(0x17,&j_state);   //chemistry checksum
}


static unsigned short control_status(void)
{
  //high byte 
  //6   in full access sealed state
  //5   in sealed state
  //4   calibration function is active deault is 0, 
  //3   Coulomb counter calibration is active
  //2   board calibration is active
  //1   a valid flash checksum has been generated
  //low byte
  //5   in fullsleep mode, will be cleared by communication
  //4   in sleep mode
  //3   impedance track algorithm using constant-power mode
  //2   Ra table updates are disabled
  //1   cell voltages are OK for Qmax updates
  //0   Qmax updates are enabled
  return control(0,&j_state);   
}

static unsigned short chem_id(void)
{
  return control(8, &j_chemid);
}


static unsigned short current(void)    //instantaneous current
{
  return control(24, &j_current);
}       

////////////// above in sealed mode

int sealed(void)       //change to sealed mode
{
  unsigned short c=0x20;
  int r;
  r = I2C_WriteReg(0, (unsigned char *)&c,2);
  return r;
}

int unsealed(void)
{
  unsigned short c=0x414;
  int r;
  r = I2C_WriteReg(0, (unsigned char *)&c,2);
  c=0x3672;
  r = I2C_WriteReg(0, (unsigned char *)&c,2);
  return r;
}


/*
You should be able to change registers after the IT Enable command has been 
sent. The cell voltage must be greater than the Flash Update OK Cell Voltage. 
If you cannot write to the flash, then you will need to reload the default 
srec to disable IT. 
*/
void it_enable(void)    //enable impedance track algorithm
{
//  control(33);
}


static void cal_enable(void)   //toggle calibration mode
{
//  control(0x2d);
}

static void enter_cal(void)    //enter calibration mode
{
//  control(129);
}

static void exit_cal(void)
{
//  control(128);
}


void reset(void)        //full reset of the device
{
  unsigned short c=0x41;
  int r;
  r = I2C_WriteReg(0, (unsigned char *)&c,2);
}




static void board_offset(unsigned short offset)//float
{
//  control(9);
  i2c_write(9,offset);  //assume it in this way
  return;
}


static void cc_offset(unsigned short offset)
{
//  control(10);
  i2c_write(10,offset);  //assume it in this way
  return;
}


static void set_fullsleep(void )//set fullsleep bit in control register
{
//  control(16);  //assume it in this way
  return;
}


/////////////////////////////////////////////

static unsigned char state_of_charge(void)      //0-100 percent
{
  unsigned char t;
  int r;
  r = I2C_ReadReg(2,(unsigned char *)&t,1);//
  if(r==I2C_OK)
  {
    j_percentage = t;
    return j_percentage;
  }
  else  return 0;
}

static unsigned short remaining_capacity(void) //mAh
{
  unsigned short t;
  int r;
  r = I2C_ReadReg(4,(unsigned char *)&t,2);//
  if(r==I2C_OK)
  {
    j_remaining = t;
    return j_remaining;
  }
  else  return 0;
}

static unsigned short full_capacity(void)      //mAh
{
  unsigned short t;
  int r;
  r = I2C_ReadReg(6,(unsigned char *)&t,2);//
  if(r==I2C_OK)
  {
    j_capacity = t;
    return j_capacity;
  }
  else  return 0;
}


static unsigned short voltage(void)    // call pack voltage with a range of 0-65535mV
{
  unsigned short t;
  int r;
  r = I2C_ReadReg(8,(unsigned char *)&t,2);//
  if(r==I2C_OK)
  {
    j_voltage = t;
    return j_voltage;
  }
  else  return 0;
}

static short current10(void)    //per second mA
{
  short t;
  int r;
  r = I2C_ReadReg(10/*16*/,(unsigned char *)&t,2);//
  if(r==I2C_OK)
  {
    j_current10 = t;
    return j_current10;
  }
  else  return 0;
}

static short current16(void)    //per second mA
{
  short t;
  int r;
  r = I2C_ReadReg(16,(unsigned char *)&t,2);//
  if(r==I2C_OK)
  {
    j_current16 = t;
    return j_current16;
  }
  else  return 0;
}

static short k2c(unsigned short t)
{
  return (short)t-2735;
}


static short temperature(void)        //0-6553.5K
{
  unsigned short t;
  int r;
//  r = I2C_ReadReg(0xc,(unsigned char *)&t,2);//
  r = I2C_ReadReg(0x2a,(unsigned char *)&t,2);//
  if(r==I2C_OK)
  {
    j_temp = k2c(t);
    return j_temp;
  }
  else  return 0;
}


static  int flags(void)//gas-gauge status register
{
  //high byte
  //7   over temperature in charge
  //6   over temperature in discharge
  //5   high battery voltage
  //4   low battery voltage
  //3   unable to begin charging
  //1   full charged
  //0   charging allowed
  //low byte
  //7   cleared on entry to relax mode and set when OCV iis performed
  //6   internal short
  //2   state of charge threshold 1 reached
  //1   state of charge threshold final reached
  //0   discharging
  unsigned short t;
  int r;
  r = I2C_ReadReg(14,(unsigned char *)&t,2);//
  if(r==I2C_OK)
  {
    j_flags = t;
    return r;
  }
}

static  int flags_b(void)
{
  //high byte
  //7   stateof health calculation is active
  //6   Indicates that LiFePO4 RELAX is enabled.
  //2   DOD at End-of-Charge is updated
  //1   Indicates RemainingCapacity() has been changed due to change in temperature.
  unsigned short t;
  int r;
  r = I2C_ReadReg(0x12,(unsigned char *)&t,2);//
  if(r==I2C_OK)
  {
    j_flags_b = t;
    return r;
  }
  else  return 0;
}

static unsigned short serial_num(void)
{
  I2C_ReadReg(0x7e,(unsigned char *)&j_serial_num,2);
//  I2C_ReadReg(0x28,(unsigned char *)&j_serial_num,2);
  return j_serial_num;
}

static unsigned short time_to_empty(void)       //in minutes
{
  unsigned short v;
  I2C_ReadReg(0x18,(unsigned char*)&v,2);
  return v;
}

static unsigned short time_to_full(void)
{
  unsigned short v;
  I2C_ReadReg(0x1a,(unsigned char*)&v,2);
  return v;
}

static unsigned short cycle_count(void)
{
  unsigned short v;
  I2C_ReadReg(0x2c,(unsigned char*)&j_cycle,2);
  return v;
}

static unsigned short state_of_health(void)
{
  unsigned short v;
  I2C_ReadReg(0x2e,(unsigned char*)&v,2);
  return v;
}
/*
time_to_empty           0x18
available energy        0x24
average power           0x26
internal temperature    0x2a
charge_voltage          0x30
charge_current          0x32
pack_configuration      0x3a
design capacity         0x3c
data flash class        0x3e
data flash block        0x3f
authenticate            0x40...0x53
block data              0x40...0x5f
authenticate checksum   0x54
block data checksum     0x60
block data control      0x61
grid number             0x62
learned status          0x63    1
QStart                  0x66
true rc                 0x68
true fcc                0x6a
state time since last changed   0x6c
*/


static unsigned char checksum(unsigned char *p, int l)
{
  unsigned char cs=0;
  for(int i=0;i<l;i++) cs += *p++;
  return ~cs;
}



static int get_block_data(unsigned char sc,unsigned char *data,uint16_t len)
{
    unsigned char c;
    uint16_t o=0;
//    if(len>32) len= 32;
    while(len)
    {
      c=0;
      I2C_WriteReg(0x61, &c,1);//block_data_control
      c=sc;//0x40;     //pack configuration subclass
      I2C_WriteReg(0x3e, &c,1);//select data class
      c=o/32;        //1 manufacturer data 0 authentication data
      I2C_WriteReg(0x3f, &c,1);//select data class
      I2C_ReadReg(0x40,data+o,len>32?32:len);
      I2C_ReadReg(0x60,&j_checksum,1);
      if(j_checksum!=checksum(data+o,len>32?32:len))
      {
        return -1;
      }
      if(len>32) len-=32,o+=32;
      else break;
    }
    return 0;
}

static int deposit_block_data(unsigned char *data)
{
    unsigned char c;
    c=0;
    I2C_WriteReg(0x61, &c,1);//block_data_control
    c=0x58;     //manufacturer info subclass
    I2C_WriteReg(0x3e, &c,1);//select data class
    c=0;        //1 manufacturer data 0 authentication data
    I2C_WriteReg(0x3f, &c,1);//select data class
    I2C_ReadReg(0x40,data,32);
    *(data+2) = 0x41;
//    I2C_WriteReg(0x40,data,32);
  unsigned char cs;
    cs = checksum(data,32);
//    I2C_WriteReg(0x60,&cs,1);
    I2C_ReadReg(0x60,&cs,1);
    return 0;
}







static unsigned short designed_capacity(void)
{
  unsigned char st;
  I2C_ReadReg(0x3c,(unsigned char *)&st,2);
  return st;
}

static unsigned char learned_state(void)
{
  unsigned char st;
  //3   Qmax updated in the field
  //2   IT is enabled
  //1,0 0,0 Battery is ok
  //    0,1 Qmax is first updated
  I2C_ReadReg(0x63,(unsigned char *)&st,1);
  return 0xf & st;
}


static void reset_param(void)//chip reset
{
  unsigned short r=0x41;
  I2C_WriteReg(0, (unsigned char *)&r,2);
  r=1;
    r = I2C_WriteReg(0,(unsigned char *)&r,2);
}


static void init(void)
{
  control(1,&j_device);
  if(j_device!=0x100)
  {
      while(9);
  }
  control(2,&j_fwver); 
  control(3,&j_hwver); 
  control(8,&j_chemid);
  temperature();
  if(j_power == 0x4A634F6B) return;
  j_power = 0x4A634F6B;
  j_serial_num = serial_num();
  j_flags = flags();
  j_state = control_status();
  j_capacity = 20000;//Qmax=20000
  j_remaining = 0;
  j_voltage = voltage();
  j_current = current();
  j_num_cell = 6;
  j_charge_num = j_discharge_num =0;
  j_counted = 0;
}




void acc_capacity(void) //will be called per second
{
  unsigned short i;
  j_voltage = voltage();
  j_current = current();
  j_remaining = remaining_capacity();
  j_percentage = state_of_charge();
  j_capacity_acc += j_current;
  return;
}


void gauge(void)
{
  volatile unsigned char data;
  init();
  API_EnableDischarge();
  while(1)
  {//HAL_UART_Transmit(&huart2,"A",1,1);
//  HAL_UART_Receive(&huart2,&j_num_cell,1,1);
/*    get_block_data(2,safety_data,10);
    get_block_data(32,charge_inhabit,6);
    get_block_data(34,charge_cfg,14);
    get_block_data(36,charge_termination,27);
    get_block_data(48,config_data,60);
    get_block_data(49,discharge,22);
    get_block_data(56,mfr_data,12);
    get_block_data(58,mfr_info,32);
    get_block_data(59,lifetime_data,12);
    get_block_data(64,pack_config,8);
    get_block_data(66,lifetime_resolution,5);
    get_block_data(68,power_data,12);
    get_block_data(80,it_cfg,93);
    get_block_data(81,current_th,11);
    get_block_data(82,gauging_state,17);
    get_block_data(88,R_a0,32);
    get_block_data(89,R_a0x,32);
    get_block_data(104,(unsigned char *)calib_data,16);
    get_block_data(112,security,24);
    I2C_ReadReg(0x24,(unsigned char*)&j_inner_temp,2);
    I2C_ReadReg(0x26,(unsigned char*)&j_inner_temp,2);
    I2C_ReadReg(0x2a,(unsigned char*)&j_inner_temp,2);
    I2C_ReadReg(0x30,(unsigned char*)&j_charge_volt,2);
    I2C_ReadReg(0x32,(unsigned char*)&j_charge_volt,2);
    I2C_ReadReg(0x3a,(unsigned char*)&j_charge_volt,2);
    I2C_ReadReg(0x3c,(unsigned char*)&j_capacity,2);
*/
    temperature();
    serial_num();

    j_state = control_status();
    j_voltage = voltage();
    j_current = current();
    j_current10 = current10();
    j_current16 = current16();
    j_capacity = full_capacity();
    j_remaining = remaining_capacity();
    j_percentage == state_of_charge();
    j_flags = flags();
    if(j_flags & 0x1001) alert();// low

    j_flags_b = flags();
    if(j_flags & 0x1001) alert();// low
    acc_capacity();
    j_capacity = full_capacity();
    if(j_voltage <3700*6 || j_percentage<20) alert();
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_SET);
    mdelay(5);
  }
  return;
}

void sleep(unsigned long sec)
{
//  for(i=0;i<10000;i++)
}

void learning(void)
{
    it_enable(/*turn on fet*/);        //control(0x21);
//    discharge();
    while(9)
    {
      gauge();
      if(j_voltage<3600*6 || j_flags==0) break;
    }
    sleep(3600*5);
    it_enable(/*set DOD0*/);
    charge();
    while(9)
    {
      gauge();  
      if(j_voltage >4200*6) break;
    }
    sleep(3600*2);
//    discharge();
}

/*
Sorry, have set the update status to 0x04 now to perform battery learning. I'm currently work on CALB 180Ah battery, is it OK if i'm using chemistry ID for CALB 40Ah? I also facing some issue while performing battery learning. following procedures found in forum as below,

1. Send an IT Enable (21) command to turn ON FETs. Send a Reset command (41) to disable Ra-table updates.
2. Discharge to empty
3. Wait 5 hours
4. Send IT Enable command to set a DOD0 reference point.
5. Charge to full
6. Wait 2 hours Qmax will be learned at this step. You should see VOK turn green after Qmax has learned. You should see Update Status change to a 05.
7. Discharge at typical low rate of target application (usually C/5 for notebooks) down to Term Voltage. Using low rate will allow to update resistance points at deep states of discharge. Rate of discharge must be higher than C/10 rate, otherwise resistance will not update.
8. Wait 5 hours for relaxation. You should see Update Status change to a 06 during the rest period.
9. Repeat steps 5-8 while running EV Software log to verify accuracy.


I have tried to perform battery learning with A123 but unsuccessful. The steps following are as below,
The battery learning cycle is as below,
1. Using EV2300, send an IT Enable ‘0021’command to turn ON FETs. The bq34z100 does not provide FET control,  but the 21 command enables Impedance Track. Send a Reset command ‘0041’ to disable Ra-table updates. 
2. Discharge to empty.
3. Wait until the OCVTAKEN flag turns red.
4. Send IT Enable ‘0021’ command to set a DOD0 reference point.
5. Charge to full
6. Wait until the OCVTAKEN flag turns red. You should also see VOK turn green after Qmax has learned. You should see Update Status change to a 05. 
7. Discharge at typical low rate of target application (usually C/5 for notebooks) down to Term Voltage. Using low rate will allow to update resistance points at deep states of discharge. Rate of discharge must be higher than C/10 rate, otherwise resistance will not update.
9. Wait until the OCVTAKEN flag turns red. You should see Update Status change to a 06 during the rest period.
It failed at step 6 where OCVTAKEN has changed to red but update status still remain at 0x04. Have tried it quite a few times but still getting the same result. 
Could you please advice if there is any mistake made in the BQ configuration? 

 Here are a few comments from reviewing your gg file.
    We have seen some issues with running optimization cycles with fast convergence enabled. You can set the FConvEn bit low in the Pack Cfg B register. The next FW release will ignore fast convergence during the optimization cycle.
    What size sense resistor are you using. The calibration parameters indicate that it is 100 mohm. There may be a current calibration problem, if you are not using a 100 mohm resistor.
    You are using a LiFePO4 cell, so you may have better success trying to update Qmax after discharge instead of charge. Try this sequence instead.
1. Using EV2300, send an IT Enable ‘0021’command to enable IT.
2. Charge to full.
3. Wait until the OCVTAKEN flag turns red.
4. Discharge to empty.
5. Wait until the OCVTAKEN flag turns red. You should also see VOK turn green after Qmax has learned. You should see Update Status change to a 05. 
6. Charge to full and wait for the OCVTAKEN flag to turn red.
7. Discharge at typical low rate of target application (usually C/5 for notebooks) down to Term Voltage. Using low rate will allow to update resistance points at deep states of discharge. Rate of discharge must be higher than C/10 rate, otherwise resistance will not update.
9. Wait until the OCVTAKEN flag turns red. You should see Update Status change to a 06 during the rest period.
*/