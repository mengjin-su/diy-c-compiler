%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "cpp1.h"

typedef struct _File_t {
	char			*fname;
	int				sys_file;
	FILE 			*yyin;		// for saving nested status...
	int  			yylineno;
	YY_BUFFER_STATE yy_state;
	
	struct _File_t  *next;
} File_t;

static FILE *fout = NULL;
static const char *incPath = NULL;
static File_t *fileStack = NULL;

static int  lineMark;
static char lastChar;
static int  define_state= 0;
static int  error_count = 0;
        
static void cpp0(char *in_file);
static void outputSrc(char *str);
static char *skipSP(char *s);
static File_t *popStack(void);

%}

escape_code             [ntbrfv\\'"a?]
escape_character        (\\{escape_code})

c_char                  ([^'\\\n]|{escape_character})
c_char_sequence         ({c_char}+)
character_constant      (\L?\'{c_char_sequence}\')

s_char                  ([^"\\\n]|{escape_character})
s_char_sequence         ({s_char}+)
string_constant         \L?\"{s_char_sequence}?\"
lib_inc_file            <{s_char_sequence}>

SP                      [ \t\v\f\015]
newline                 [\n]


%x COMMENT
%x INCLUDE
%x DEFINE
%x DEFINE_COMMENT

%%

"//".*                  	{ /* C++ style comments, remove it. */ }
"/*"                    	{ BEGIN(COMMENT); }
<COMMENT>"*/"           	{ BEGIN(INITIAL); lineMark = 1; }
<COMMENT>{newline}      	{ yylineno++; }
<COMMENT>.              	{ /* C style comments, remove it. */ }

^{SP}*"#include"{SP}+		{ BEGIN(INCLUDE); }
<INCLUDE>{string_constant}.*{newline} 	|
<INCLUDE>{lib_inc_file}.*{newline} 		{
							  yylineno++;
							  BEGIN(INITIAL);
							  cpp0(yytext);
							}
							
^{SP}*"#define"{SP}+ | 
^{SP}*"#pragma"{SP}+	    { BEGIN(DEFINE);  outputSrc(yytext); define_state = -1;}

<DEFINE>"//".*{newline}		{ BEGIN(INITIAL); outputSrc("\n"); yylineno++; }
<DEFINE>{string_constant}	{ outputSrc(yytext); }
<DEFINE>[ \t]+				{ if ( define_state ) outputSrc(yytext); }
<DEFINE>"/*"				{ BEGIN(DEFINE_COMMENT); }
<DEFINE>.					{ if ( define_state == 0 )
							  {
								outputSrc(" "); 
								define_state = 1;	
							  }
							  outputSrc(yytext); 
							}
<DEFINE>{SP}*"\\".*{newline} { yylineno++; define_state = 0; }
<DEFINE>{newline}			{ BEGIN(INITIAL); outputSrc(yytext); yylineno++; 
							  if ( define_state >= 0 ) lineMark = 1;
							}

<DEFINE_COMMENT>"*/"		{ BEGIN(DEFINE); }
<DEFINE_COMMENT>.			;
<DEFINE_COMMENT>{newline}	{ yylineno++; }

{newline}               	{ outputSrc(yytext);
							  yylineno++;
                        	}
{string_constant}	|
.                      		{ outputSrc(yytext); }

<<EOF>>						{ fclose(yyin);			// close input file

							  if ( lastChar != '\n' ) 
								outputSrc("\n"); 	// terminate with CR
							  
							  File_t *fp = popStack();
							  
							  // restore status...
							  yy_delete_buffer(YY_CURRENT_BUFFER);
							  yy_switch_to_buffer(fp->yy_state);
							  yyin = fp->yyin;
							  yylineno = fp->yylineno;
								
			  				  free(fp);
								
							  if ( fileStack == NULL ) // end of running.
							  {
								yyterminate();
							  }
							  else	
							  { 
								lineMark = 1;
								lastChar = 0;
							  }
							}
%%

/************************************************************/
int cpp1(const char *inc_path, char *file_in, char *file_out, int sys_file)
{
	File_t *fp;
	
	if ( file_out )
	{
		incPath = inc_path;
		
		fout = fopen(file_out, "w");	
		if ( fout == NULL )
		{
			printf("can't open output file '%s'!\n", file_out);
			return -1;
		}		
	}

	// elliminate duplicated file scaning...
	for (fp = fileStack; fp; fp = fp->next)
		if ( strcmp(fp->fname, file_in) == 0 && fp->sys_file == sys_file ) return 0;

	FILE *fd = fopen(file_in, "r");
	if ( fd == NULL )
	{
		printf("can't open file '%s'!\n", file_in);
		return -1;
	}

	fp = malloc(sizeof(File_t));
	fp->fname = malloc(strlen(file_in)+1);
	strcpy(fp->fname, file_in);
	fp->sys_file = sys_file;
	fp->next  = fileStack;
	fileStack = fp;
	
	lastChar = 0;
	lineMark = 1;
	
	if ( file_out )		// root entry
	{
		yyin     = fd;	
		yylineno = 0;
		yylex();

		fclose(fout);
		return error_count;		
	}
	
	// save current status for yylex state machine
	fp->yyin     = yyin;
	fp->yylineno = yylineno;
	fp->yy_state = YY_CURRENT_BUFFER;		
	yy_switch_to_buffer(yy_create_buffer(fd, YY_BUF_SIZE));

	yyin     = fd;	// init. file scaning...	
	yylineno = 0;
	return 0;
}

/************************************************************/
static void cpp0(char *in_file)
{
	char file_name[4096];
	char c = (*in_file == '"')? '"' :
			 (*in_file == '<')? '>' : 0;

	switch ( c )
	{
		case '"':	// include a user file...
			strcpy(file_name, skipSP(in_file+1));
			*strchr(file_name, c) = '\0';
			cpp1(NULL, file_name, NULL, 0);
			break;

		case '>':	// include a lib file...
			sprintf(file_name, "%s%s", incPath, skipSP(in_file+1));
			*strchr(file_name, c) = '\0';
			cpp1(NULL, file_name, NULL, 1);
			break;
		
		default:
			error_count++;	
	}
}

/************************************************************/
int yywrap(void) 
{
	return -1;
}

/************************************************************/
static void outputSrc(char *str)
{
	int i, len = strlen(str);
	
	for (i = 0; i < len; i++)
	{
		char c = *str++;
		if ( fout != NULL && c != '\r' )
		{
			if ( lineMark )
			{
				if ( lastChar && lastChar != '\n')
					fprintf(fout, "\n");
			
				if ( fileStack->sys_file )
					fprintf(fout, "#LINE %d %s\n", yylineno+1, fileStack->fname);
				else
					fprintf(fout, "#line %d %s\n", yylineno+1, fileStack->fname);
			}

			lineMark = 0;	
			fprintf(fout, "%c", lastChar = c);
		}
	}
}

/************************************************************/
int yyerror(char *s)
{
    fflush(stdout);
	error_count++;
    return 0;
}

/************************************************************/
static char *skipSP(char *s)
{
	while ( *s == ' ' || *s == '\t' ) s++;
	return s;
}

/************************************************************/
static File_t *popStack(void)
{
	if ( fileStack )
	{
		File_t *fp = fileStack;
		free(fp->fname);
		
		fileStack = fileStack->next;
		return fp;
	}
	return NULL;
}