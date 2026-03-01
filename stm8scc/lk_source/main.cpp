#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <ctype.h>
extern "C" {
#include "common.h"
}
#include "ctrl.h"
#include "path.h"
#include "memory.h"
#include "segment.h"
#include "symbol.h"
#include "st8link.h"

#define VERSION     "v1.0"
#define TEMP_FOLDER	"_tmp_"

static char *outputFile  = (char*)"_OUTPUT_";
static Symbol *fileList = NULL;	// list of .obj files
static Symbol *libList  = NULL;	// list of .lib files

void link0(char *filename, bool lib_f);
char *getLibFile(char *libfile);

Memory *dataMem = NULL;
Memory *codeMem = NULL;
unsigned int sp_init_value;
extern char sp_init_defined;

char *libPath = NULL;
bool caseMatch = false;
ST8link *linker = NULL;

////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[], char *env[])
{
    bool started = false;
   	printf("STM8S linker, %s\n", VERSION);

    for (int i = 1; i < argc; i++)
    {
        if ( argv[i][0] == '-' )
        {
			if ( strcmp(argv[i], "-E") == 0 )
			{
				extendedMode = true;
			}
			else if ( strcmp(argv[i], "-o") == 0 )
			{
				if ( ++i < argc )
					outputFile = argv[i];	// assign the output file name
			}
			else if ( memcmp(argv[i], "-SP=", 4) == 0 && isdigit(argv[i][4]) )
			{
				spInitAddr = convertNum(&argv[i][4]);
			}
			else if ( memcmp(argv[i], "-A=", 3) == 0 && isdigit(argv[i][3]) )
			{
				codeStartAddr = convertNum(&argv[i][3]);
			}
        }
        else
        {
			if ( !started )
			{
				dataMem = new Memory(DATA_MEMORY);
				codeMem = new Memory(CODE_MEMORY);
				libList = new Symbol(extendedMode? (char*)"crt0x": (char*)"crt0");
				started = true;
			}

			link0(argv[i], false);   // parse source file
        }
    }

    if ( started )
    {
		Path path(env);
		libPath = path.get();

		std::string tmp_folder = TEMP_FOLDER;
		system(("mkdir " + tmp_folder).c_str());	// create a temp folder
		for (Symbol *sp = libList; sp; sp = sp->next)
		{
			char *ssp = getLibFile(sp->name);
			if ( ssp ) link0(ssp, true);
		}
		system(("rm -r " + tmp_folder).c_str());	// remove temp folder & files

		ST8link linker(dataMem, codeMem);
		linker.scanInclusion();						// code including process
		linker.assignSegmentsAddress();				// assign addresses for segments

		if ( linker.errorCount == 0 )
		{
			char *output = new char[strlen(outputFile)+10];
			sprintf(output, "%s.hex", outputFile);
			linker.outputHex(output);
			sprintf(output, "%s.map", outputFile);
			linker.outputMap(output, dataMem);
			delete [] output;
		}

		deleteSegments(libSegGroup);
		deleteSegments(codeSegGroup);
		deleteSegments(dataSegGroup);
		deleteSegments(miscSegGroup);

		delete dataMem;
		delete codeMem;
	}
    return errorCnt;
}

////////////////////////////////////////////////////////////////////////
/*
	parse the input file, and group lines into segments
*/
////////////////////////////////////////////////////////////////////////
void link0(char *filename, bool lib_f)
{
	if ( searchSymbol(fileList, filename) )
		return;

	addSymbol(&fileList, new Symbol(filename));
    curFile = filename;

    printf("linking '%s' ...\n", filename);

	// using Lex and Bison to parse the source ...
	errorCnt = 0;
    line_t *lp = _main(filename);

	if ( errorCnt > 0 )
		exit(1);

	Segment *seg = NULL;
	while ( lp )
	{
		bool save_line = false;
		bool add_seg   = false;
		item_t *ip0, *ip1;

		switch ( lp->type )	// input line type?
		{
			case 'P':	// processor
				ip0 = itemPtr(lp, 1);
				ip1 = itemPtr(lp, 2);
				if ( ip0 ) dataMem->resize(ip0->data.val);
				if ( ip1 ) codeMem->resize(ip1->data.val);
				break;

			case 'I':	// include lib file
				ip0 = lp->items;
				if ( ip0 && ip0->type == TYPE_STRING )
				{
					std::string str = ip0->data.str;
					int pos = str.find_last_of("/");
					if ( pos == std::string::npos ) pos = str.find_last_of("\\");
					if ( pos != std::string::npos )	str = str.substr(pos+1);

					pos = str.find(".");
					if ( pos != std::string::npos )	str = str.substr(0, pos);

					if ( !searchSymbol(libList, (char*)str.c_str()) )
						addSymbol(&libList, new Symbol((char*)str.c_str()));
				}
				break;

			case 'N':	// code blank
			case 'M':	// data blank
				ip0 = itemPtr(lp, 0);
				ip1 = itemPtr(lp, 1);
				if ( ip0 && ip0->type == TYPE_VALUE &&
					 ip1 && ip1->type == TYPE_VALUE )
				{
					if ( lp->type == 'N' ) codeMem->blank(ip0->data.val, ip1->data.val);
					if ( lp->type == 'M' ) dataMem->blank(ip0->data.val, ip1->data.val);
				}
				break;

			case 'U':	// variable bit test instruction('0') - ".jbz"  (btjf)
			case 'V':	// variable bit test instruction('1') - ".jbnz" (btjt)
				lp->insert = 5;	// init. size = 5 bytes
				save_line = true;
				break;

			case 'J':	// variable branch instruction - ".jp" (jra or jrxx)
				lp->insert = 2;	// init. size = 2 bytes
				save_line = true;
				break;

			case 0:
				break;

			case 'S':
				seg = NULL;
			default:
				if ( seg == NULL )
				{
					seg = new Segment(filename);
					seg->isLIB = lib_f? 1: 0;
					add_seg = true; // printf("new SEG - %s\n", filename);
				}
				save_line = true;
		}

		line_t *next = lp->next;
		lp->next = NULL;

		if ( save_line )
		{
			seg->addLine(lp);
			if ( add_seg )
				linker->addSeg(seg, seg->isLIB);
		}
		else
			freeLine(lp);

		lp = next;
	}
}

//////////////////////////////////////////////
char *getLibFile(char *libfile)
{
	static char buf[4096];
	std::string file_name;
	std::string cc_cmd = "st8cc ";
	std::string as_cmd = "st8as ";
	std::string cp_cmd = "cp ";
	std::string tmp_path = TEMP_FOLDER;
	char *p0 = libfile, *p1;
	FILE *fd;

	if ( extendedMode )	cc_cmd += "-E ";

	while ( (p1 = strchr(p0, '/'))  ) p0 = p1 + 1;
	while ( (p1 = strchr(p0, '\\')) ) p0 = p1 + 1;

	if ( libPath != NULL )
		sprintf(buf, "%slib/%s", libPath, p0);
	else
		sprintf(buf, "%s", p0);

	file_name = buf; file_name += ".c";
	fd = fopen(file_name.c_str(), "r");	// check if the source exists
	if ( fd == NULL ) return NULL;
	fclose(fd);

	// copy the file to current folder...
	system((cp_cmd + file_name + " " + tmp_path).c_str());
	file_name = tmp_path + "/" + libfile + ".c";

	// compile the source
	system((cc_cmd + file_name).c_str());

	// and assemble it
	file_name = tmp_path + "/" + libfile + ".asm";
	system((as_cmd + file_name).c_str());

	// return obj file name...
	file_name = tmp_path + "/" + libfile + ".obj";
	fd = fopen(file_name.c_str(), "r");
	if ( fd == NULL ) return NULL;

	fclose (fd);
	sprintf(buf, "%s", file_name.c_str());
	return buf;
}
