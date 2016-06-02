 //==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <syslog.h>
//==================================================================
//================= @DEFINES =======================================
//==================================================================

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================

int main (void)
{
	int idx = 0;
	
	/* Init */
        openlog("MY_SYSLOG", LOG_NDELAY, LOG_LOCAL0);
  
	/* Debug messages */
	syslog(LOG_DEBUG, "Hello world. I am syslog !!!");

	for (idx = 0; idx < 10; idx++)
	{
		syslog(LOG_WARNING, "Loop Count: %d", idx);
	}

	syslog(LOG_DEBUG, "Good bye world"); 
	
	/* exit */
	closelog();

	return 0;
}








