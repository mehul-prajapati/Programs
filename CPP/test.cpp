#include <iostream>
#include <stdlib.h>

class Test
{
	int Cnt;
	float div;

public:
	void set_data()
	{
		std::cout << "Enter Count\n";
		std::cin >> Cnt;		
		std::cout << "Enter Div\n";
		std::cin >> div;
	}
};


int main()
{


	Test hello;	

	hello.set_data();






	return 0;
}
