#include <stdio.h>
int main(int argc, char *argv[])
{

struct student_database {
    char name[10];
    int roll;
    int marks;
}stud1 = {"Ankit",10,90};

struct student_database *ptr;
ptr = &stud1;

printf("Roll Number : %d\n",(*ptr).roll);
printf("Marks of Student : %d\n",(*ptr).marks);
printf("Name of the Student : %s\n",(ptr)->name);
printf(" %u\n %u\n", (char)*ptr,(unsigned int)stud1.roll);

return 0;
}
