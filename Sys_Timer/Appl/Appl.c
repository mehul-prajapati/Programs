//==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include "SysTmr.h"
#include <unistd.h>
#include <stdio.h>
//==================================================================
//================= @DEFINES =======================================
//==================================================================

//==================================================================
//================= @ENUMS =========================================
//==================================================================
enum
{
	SYS_TIMER_ID_1			= 0x01,
	SYS_TIMER_ID_2			= 0x02,
	SYS_TIMER_ID_3			= 0x03,
	SYS_TIMER_ID_4			= 0x04,

	SYS_TIMER_MAX
};

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
SYS_TIMER sysTimer[SYS_TIMER_MAX];

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================
void i_Timer_Expiry(UINT32 appId, UINT32 appData);

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================
int main (void)
{
	
	SYS_TIMER_Init(&sysTimer[SYS_TIMER_ID_1], SYS_TIMER_ID_1);	
	SYS_TIMER_Set_Callback(&sysTimer[SYS_TIMER_ID_1], i_Timer_Expiry);
	SYS_TIMER_Start(&sysTimer[SYS_TIMER_ID_1], 2, 12345);
	

        SYS_TIMER_Init(&sysTimer[SYS_TIMER_ID_2], SYS_TIMER_ID_2);
        SYS_TIMER_Set_Callback(&sysTimer[SYS_TIMER_ID_2], i_Timer_Expiry);
        SYS_TIMER_Start(&sysTimer[SYS_TIMER_ID_2], 3, 1234);


        SYS_TIMER_Init(&sysTimer[SYS_TIMER_ID_3], SYS_TIMER_ID_3);
        SYS_TIMER_Set_Callback(&sysTimer[SYS_TIMER_ID_3], i_Timer_Expiry);
        SYS_TIMER_Start(&sysTimer[SYS_TIMER_ID_3], 4, 234);

	while (1)
	{
		sleep(1);
		SYS_TIMER_Tick();
	}

	return (0);
}


void i_Timer_Expiry(UINT32 appId, UINT32 appData)
{
	printf("You gotta be kidding me!!!\n");
	
	printf("App Id = %d AppData = %d\n", appId, appData);
}
