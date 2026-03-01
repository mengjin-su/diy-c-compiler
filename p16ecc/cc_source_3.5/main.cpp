#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "common.h"
#include "display.h"

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		std::string str = "cpp1 "; 
		str += argv[i];
		
		if ( system(str.c_str()) == 0 )
		{
			str = argv[i]; str += "_";
			
			_main((char*)str.c_str());
			remove(str.c_str());
			
			display(progUnit, 0);
		}
	}
	return 0;
}
