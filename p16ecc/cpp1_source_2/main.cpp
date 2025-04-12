#include <stdio.h>
#include <string.h>
#include <string>
#include "cpp1.h"

#define CC_FOLDER_NAME		"p16ecc"
#define CC_FOLDER_LEN		strlen(CC_FOLDER_NAME)
#define IS_SLASH(c)			((c) == '/' || (inWindows && (c) == '\\'))
#define IS_SEPERATOR(c)		((c) == '=' || (inWindows && (c) == ';') || \
										   (!inWindows && (c) == ':'))

int main(int argc, char *argv[], char *env[])
{
	bool inWindows = false;
	std::string	incPath;

	if ( argc < 2 )
	{
		printf("missing input file!\n");
		return -1;
	}

	for (; env && *env; env++) 	// search for "PATH=" setting
	{
		if ( memcmp(*env, "OS=Windows", 10) == 0 )
			inWindows = true;
		else if ( strncasecmp(*env, "path=", 5) == 0 )
		{
			char *p = strstr(*env, CC_FOLDER_NAME);
			int len = CC_FOLDER_LEN;

			if ( p && IS_SLASH(p[-1]) && IS_SLASH(p[len]) &&
				 memcmp(&p[len + 1], "bin", 3) == 0 	   )
			{
				while ( !IS_SEPERATOR(p[-1]) ) p--, len++;

				incPath.assign(p, len);
				incPath += "/include/";
			}
		}
	}

	std::string output = argv[1];
	output += "_";

	return cpp1(incPath.c_str(), argv[1], (char*)output.c_str(), 0);
}
