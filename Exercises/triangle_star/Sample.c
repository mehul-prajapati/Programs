 //==================================================================
//================= @INCLUDES ======================================
//==================================================================
#include <stdio.h>
#include <unistd.h>
//==================================================================
//================= @DEFINES =======================================
//==================================================================

//==================================================================
//================= @ENUMS =========================================
//==================================================================

//==================================================================
//================= @DATATYPES =====================================
//==================================================================
/* The ENVIRON variable contains the environment. */

//==================================================================
//================= @FUNCTION PROTOTYPE ============================
//==================================================================

//==================================================================
//================= @FUNCTION DEFINITION ===========================
//==================================================================

int main (void)
{
	unsigned char loopCnt = 0;
	unsigned int width = 0;	
	unsigned char starCnt = 0;
	unsigned char spaceCnt = 0;
	char printChar = '#';
 
	printf("Enter the width of triagle\n");
	scanf("%ud", &width);

while(1)
{
	/* print one by one * fomr array */
	for (loopCnt = 0; loopCnt < width; loopCnt++)
	{
		for (spaceCnt = width - 1; spaceCnt > loopCnt; spaceCnt--)
		{
			printf(" ");
		}
	
		for (starCnt = 0; starCnt <= loopCnt; starCnt++) 
		{
			printf("%c", printChar);
		}

		for (starCnt = 0; starCnt < loopCnt; starCnt++)
                {
                        printf("%c", printChar);
                }
		printf("\n");		
	}

	usleep(450000);
	
        for (loopCnt = width - 1; loopCnt > 0; loopCnt--)
        {
                for (spaceCnt = width; spaceCnt > loopCnt; spaceCnt--)
                {
                        printf(" ");
                }

                for (starCnt = loopCnt - 1; starCnt > 0; starCnt--)
                {
                        printf("%c", printChar);
                }

                for (starCnt = loopCnt; starCnt > 0; starCnt--)
                {
                        printf("%c", printChar);
                }
                printf("\n");
        }

	printChar++;
}

	return 0;
}








