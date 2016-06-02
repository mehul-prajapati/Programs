 #include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <conio.h>
#include <iomanip>

using namespace std;
int main() {
    FILE *fp, *ft;
    char another, choice;

struct student {
        char first_name[50], last_name[50];
        char course[100];
        int section;
    };

    struct student e;
    char xfirst_name[50], xlast_name[50];
    long int recsize;

    fp=fopen("users.txt","rb+");

    if (fp == NULL) {
        fp = fopen("users.txt","wb+");

        if (fp==NULL)
        {
             puts("Cannot open file");
             return 0;
        }
    }


 recsize = sizeof(e);

 while(1) {
     system("cls");

     printf "\t\t====== STUDENT INFORMATION SYSTEM ======";
     printf"\n\n                                          ";
     printf "\n\n";
     cout<<" \n\t\t\t======================";
     printf "\n \t\t\t  1. Add    Records";
     printf "\n \t\t\t  2. List   Records";
     printf "\n \t\t\t  3. Modify Records";
     printf "\n \t\t\t  4. Delete Records";
     printf "\n \t\t\t  5. Exit   Program";
     cout<<" \n\t\t\t======================";
     printf "\n\n";
     printf "\t\t\t Select Your Choice ::";
     fflush(stdin);
     choice = getche();
     switch(choice)
     {
      case '1' :
            fseek(fp,0,SEEK_END);
            another ='Y';
            while(another == 'Y' || another == 'y')
            {
                  system("cls");
                printf "Enter the First Name : ";
                scanf e.first_name;
                printf "Enter the Last Name : ";
                scanf e.last_name;
                printf "Enter the Course    : ";
                scanf e.course;
                printf "Enter the Section   : ";
                scanf e.section;
                fwrite(&e,recsize,1,fp);
                printf "\n Add Another Record (Y/N) ";
                fflush(stdin);
                another = getchar();
            }
            break;
   case '2':
            system("cls");
           rewind(fp);
           printf "=== View the Records in the Database ===";
           printf "\n";
           while (fread(&e,recsize,1,fp) == 1){
           printf "\n";
           printf"\nName     :: " <<e.first_name <<" "<<e.last_name;
           //printf "\n";
           printf"\nCourse   :: " <<e.course ;
           printf"\nSection  :: "<<e.section;
           }
           printf "\n\n";
           system("pause");
           break;

   case '3' :
            system("cls");
          another = 'Y';
          while (another == 'Y'|| another == 'y')
          {
              printf "\n Enter the last name of the student : ";
              scanf xlast_name;

            rewind(fp);
            while (fread(&e,recsize,1,fp) == 1)
            {
                if (strcmp(e.last_name,xlast_name) == 0)
                {
                printf "Enter new the Firt Name : ";
                scanf e.first_name;
                printf "Enter new the Last Name : ";
                scanf e.last_name;
                printf "Enter new the Course    : ";
                scanf e.course;
                printf "Enter new the Section   : ";
                scanf e.section;
                fseek(fp, - recsize, SEEK_CUR);
                fwrite(&e,recsize,1,fp);
                break;
                }
                else
                cout<<"record not found";
            }
            printf "\n Modify Another Record (Y/N) ";
                fflush(stdin);
                another = getchar();
            }
            break;


     case '4':
       system("cls");
           another = 'Y';
          while (another == 'Y'|| another == 'y')
          {
              printf "\n Enter the last name of the student to delete : ";
              scanf xlast_name;

              ft = fopen("temp.dat", "wb");

              rewind(fp);
              while (fread (&e, recsize,1,fp) == 1)

                 if (strcmp(e.last_name,xlast_name) != 0)
                {
                    fwrite(&e,recsize,1,ft);
                }
                fclose(fp);
                fclose(ft);
                remove("users.txt");
                rename("temp.dat","users.txt");

                fp=fopen("users.txt","rb+");

                printf "\n Delete Another Record (Y/N) ";
                fflush(stdin);
                another = getchar();
              }

              break;

              case '5':
              fclose(fp);
              printf "\n\n";
              printf "\t\t     THANK YOU FOR USING THIS SOFTWARE";
              printf "\n\n";
              exit(0);
          }
     }


system("pause");
return 0;
}
