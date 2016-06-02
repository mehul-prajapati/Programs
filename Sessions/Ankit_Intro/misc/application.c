#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <linux/ioctl.h>
/*
#define GET_VAR 1
#define SET_VAR 2
#define CLR_VAR 3
*/

#define GET_VAR _IOR('q',1,int)
#define SET_VAR _IOW('q',2,int)
#define CLR_VAR _IO('q',3)

int main(int argc, char *argv[])
{
 
   assert(argc > 1);
   char buff[100];
   char i = 0;
   int user_var = 5;
   
   memset(buff,0,100);

   printf("\nInput: %s\n", argv[1]);

   int fp = open("/dev/chardev", O_RDWR);

   write(fp, argv[1], strlen(argv[1]));

   while(read(fp, &buff[i++], 1));
{
   printf("\nReversed by the driver: \n\n\t %s \n\n", buff);
}
   // ================ GET ====================//

   if (ioctl(fp, GET_VAR, &user_var)== -1)
   {
        printf("error for get\n");
   }

   printf("get user_var: %d\n", user_var);

   // ==================SET =================//

   user_var = 5; 

   if (ioctl(fp, SET_VAR, &user_var)== -1)
   {
        printf("error for set\n");
   }

   if (ioctl(fp, GET_VAR, &user_var)== -1)
   {
        printf("error for get\n");
   }

   printf("get user_var: %d\n", user_var);

   // =================== CLR ============== // 

   if (ioctl(fp, CLR_VAR)== -1)
   {
        printf("error for clr\n");
   }

   if (ioctl(fp, GET_VAR, &user_var)== -1)
   {
        printf("error for get\n");
   }

   printf("get user_var: %d\n", user_var);
      
   close(fp);

   return 0;
}
