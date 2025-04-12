#include <stdio.h>
#include <string.h>
#include <string>
#include "cpp1.h"

typedef struct File_t_ {
	std::string		name;
	FILE			*fhandle;
	int  			line_no;
	bool            sys_file;
	struct File_t_  *next;
} File_t;


static File_t *fileStack = NULL;
static FILE   *output = NULL;

static int  lexer(File_t *file, std::string *inc_path);
static int  readLine(FILE *fin, std::string *line_buf);
static char *skipSP(char *line);

//////////////////////////////////////////////////////////////////////////
int cpp1(std::string *inc_path, char *file_in, char *file_out, bool sys_file)
{
	if ( file_out )
	{
		output = fopen(file_out, "w");
		if ( output == NULL )
		{
			printf("can't open output file '%s'!\n", file_out);
			return -1;
		}
	}

	// elliminate duplicated file scaning...
	for (File_t *fp = fileStack; fp; fp = fp->next)
		if ( fp->name == file_in && fp->sys_file == sys_file ) return 0;

	FILE *fd = fopen(file_in, "r");
	if ( fd == NULL )
	{
		printf("can't open file '%s'!\n", file_in);
		return -1;
	}

	File_t *file 	= new File_t;
	file->fhandle	= fd;
	file->name   	= file_in;
	file->line_no	= 0;
	file->sys_file 	= sys_file;
	file->next 		= fileStack;
	fileStack 		= file;

	int rtcode = lexer(file, inc_path);
	fclose(file->fhandle);
	delete file;

	file = fileStack = fileStack->next;

	if ( fileStack == NULL )	// end of cpp1, close the output.
		fclose(output);

	return rtcode;
}

static int lexer(File_t *file, std::string *inc_path)
{
	for (bool line_mark = true;;)
	{
		std::string line_buf = "";
		int length = readLine(file->fhandle, &line_buf);
		if ( length <= 0 ) { fclose(file->fhandle); return 0; }

		file->line_no++;

		char *p = skipSP((char*)line_buf.c_str()), *p1;
		if ( memcmp(p, "#include", 8) == 0 )
		{
			int rtcode;
			std::string file_path = inc_path->c_str();

			p = skipSP(p + 8);
			switch ( *p++ )
			{
				case '"':	// user source file
					if ( (p1 = strchr(p, '"')) != NULL )
					{
						*p1 = '\0';
						rtcode = cpp1(inc_path, p, NULL);
						if ( rtcode < 0 ) return rtcode;
						break;
					}
					return -1;
				case '<':	// system source file
					if ( (p1 = strchr(p, '>')) != NULL )
					{
						*p1 = '\0';
						file_path += p;
						p = (char*)file_path.c_str();
						rtcode = cpp1(inc_path, p, NULL, true);
						if ( rtcode < 0 ) return rtcode;
						break;
					}
				default:
					return -1;
			}
			line_mark = true;
		}
		else
		{
			if ( line_mark )
			{
				if ( file->sys_file )
					fprintf(output, "#LINE %d %s\n", file->line_no, file->name.c_str());
				else
					fprintf(output, "#line %d %s\n", file->line_no, file->name.c_str());
			}

			fprintf(output, "%s", line_buf.c_str());
			line_mark = false;
		}
	}
}

static int readLine(FILE *fin, std::string *line_buf)
{
	int length = 0;
	char c = 0;

	while ( fin )
	{
		if ( fscanf(fin, "%c", &c) != 1 || feof(fin) )
			break;

		if ( c != '\r' )
		{
			line_buf->append(1, c);
			length++;
			if ( c == '\n' ) break;
		}
	}

	if ( length > 0 && c != '\n' )
	{
		line_buf->append(1, '\n');
		length++;
	}

	return length;
}

static char *skipSP(char *p)
{
	while ( *p == ' ' || *p == '\t' ) p++;
	return p;
}
