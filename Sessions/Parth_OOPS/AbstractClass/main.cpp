/*
 * main.cpp
 *
 *  Created on: 02-Jun-2014
 *      Author: parth.vshah
 */

#include "AbstractClass.cpp"
#include <stdlib.h>

int main(int argc, char **argv)
{
	Rectangle rec("Rec");
	rec.SetHeight(10);
	rec.SetWidth(20);
	printf ("Area of %s is : %d\n", rec._name.c_str(), rec.getArea());

	Triangle tri("Tri");
	//tri.SetHeight(10);
	//rec.SetWidth(20);
	printf ("Area of %s is : %d\n", tri._name.c_str(), tri.getArea());


	return 0;
}


