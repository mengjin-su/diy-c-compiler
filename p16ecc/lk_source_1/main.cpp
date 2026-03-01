#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
extern "C" {
#include "common.h"
}
#include "path.h"
#include "segment.h"

#define VERSION     "v0.1.1"

static char *outputFile  = (char*)"_OUTPUT_";
static void link0(char *filename, bool lib_f=false);

int     useBSR6  = 0;
char    *libPath;
Segment	*segmentList = NULL;

////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[], char *env[])
{
    int start_parsing = 0;
	Path *path = new Path(env);
	libPath = path->get();

   	printf ("PIC16E linker, %s\n", VERSION);

    for (int i = 1; i < argc; i++)
    {
        if ( argv[i][0] == '-' )
        {	// assign the output file name
			if ( strcmp(argv[i], "-o") == 0 )
			{
				if ( ++i < argc )
					outputFile = argv[i];
			}
			else if ( strcmp(argv[i], "-X") == 0 )
			{
				useBSR6 = 1;
			}
			continue;
        }
        else
        {
			link0(argv[i]);   // parse source file
			start_parsing++;
        }
    }

	if ( start_parsing )
	{
		deleteSegments(codeSegGroup);
		deleteSegments(dataSegGroup);
		deleteSegments(fuseSegGroup);
		deleteSegments(miscSegGroup);
	}
	delete path;
    return errorCnt;
}

////////////////////////////////////////////////////////////////////////
/*
	parse the input source file, and generate the raw file in
	'line_t' format.
*/
////////////////////////////////////////////////////////////////////////
static void link0(char *filename, bool lib_f)
{
    curFile = filename;

    printf("linking '%s' ...\n", filename);

	// using Lex and Bison to parse the source ...
	errorCnt = 0;
    line_t *lp = _main(filename);
	if ( errorCnt > 0 )	exit(1);

	Segment *seg = NULL;
	while ( lp )
	{
		bool save_line = false;
		bool add_seg   = false;

		switch ( lp->type )	// input line type?
		{
			case 'P':	// processor
			case 'I':	// include lib file
			case 'N':	// RAM blank
			case 'M':	// ROM blank
			case 0:
				break;

			case 'S':	// SEGMENT line...
				seg = NULL;
			default:
				if ( seg == NULL )
				{	// create a segment...
					seg = new Segment(filename);
					seg->isLIB = lib_f;
					add_seg = true;
				}
				save_line = true;
				break;
		}

		line_t *next = lp->next;
		lp->next = NULL;

		if ( save_line )
		{
			seg->addLine(lp);		// add the line into segment...
			if ( add_seg )			// new segment, add the segment
			{
				seg->init();
				addSegment(seg);
			}
		}
		else
			freeLine(lp);

		lp = next;
	}
}
