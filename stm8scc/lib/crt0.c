static void _negPtrX24(void);
static void _negPtrX32(void);

void _extendRegA(void)
{
	asm("and	%A, #0x80");
	asm("jreq	.+2");
	asm("ld		%A, #0xff");
}

void _extendSign(void)
{
	asm("jrpl	.+3");
	asm("ld		%A, #0xff");
	asm("ret");
	asm("clr	%A");
}

void _mul16Ptr(unsigned int y)
{
#if 0
	char count = 16;
	asm("clrw	%Y");
	do {
		asm("ld		%A, (1, %X)");
		asm("rrc	%A");
		asm("jrnc	.+3");
		asm("addw	%Y, (4, %SP)");
		asm("rrcw	%Y");
		asm("rrc	(%X)");
		asm("rrc	(1, %X)");
	} while ( --count );
#else
	asm("ldw	%Y, (3, %SP)");	// y2 	(Y <= y[1,0])
	asm("ld		%A, (1, %X)");	// x0
	asm("mul	%Y, %A");		// Y = x0 & y0
	asm("pushw	%Y");			// lower 16-bit product

	asm("ldw	%Y, (4, %SP)");	// y1
	asm("mul	%Y, %A");		// Y = x0 & y1
	asm("ld		%A, %YL");
	asm("add	%A, (1, %SP)");
	asm("ld		(1, %SP), %A");

	asm("ldw	%Y, (5, %SP)");	// y0
	asm("ld		%A, (%X)");		// x1
	asm("mul	%Y, %A");		// Y = x1 & y0
	asm("ld		%A, %YL");
	asm("popw	%Y");
	asm("ldw	(%X), %Y");
	asm("add	%A, (%X)");
	asm("ld		(%X), %A");
#endif
}

void _mul24Ptr(unsigned short y)
{
#if 0
	short prod = 0;
	char count = 24;
	do {
		asm("ld		%A, (2, %X)");
		asm("rrc	%A");
		asm("jrnc	.+13");

		asm("ldw	%Y, (8, %SP)");
		asm("addw	%Y, (2, %SP)");
		asm("ldw	(2, %SP), %Y");
		asm("ld		%A, (7, %SP)");
		asm("adc	%A, (1, %SP)");
		asm("ld		(1, %SP), %A");

		asm("rrc	(1, %SP)");
		asm("rrc	(2, %SP)");
		asm("rrc	(3, %SP)");
		asm("rrc	(%X)");
		asm("rrc	(1, %X)");
		asm("rrc	(2, %X)");
	} while ( --count );
#else
	short prod;
// y0 * x0
	asm("ldw	%Y, (7, %SP)");	// YL <- y0
	asm("ld		%A, (2, %X)");	// A  <- x0
	asm("mul	%Y, %A");
	asm("ldw	(2, %SP), %Y");	// p1, p0
// y2 * x0
	asm("ldw	%Y, (5, %SP)");	// YL <- y2
	asm("mul	%Y, %A");
	asm("ld		%A, %YL");
	asm("ld		(1, %SP), %A");	// p2
// y1 * x0
	asm("ldw	%Y, (6, %SP)");	// YL <- y1
	asm("mul	%Y, %A");
	asm("addw	%Y, (1, %SP)");
	asm("ldw	(1, %SP), %Y");	// p2,p1 = p2,p1+Y
// y0 * x1
	asm("ldw	%Y, (7, %SP)");	// YL <- y0
	asm("ld		%A, (1, %X)");	// A  <- x1
	asm("mul	%Y, %A");
	asm("addw	%Y, (1, %SP)");
	asm("ldw	(1, %SP), %Y");	// p2,p1 += Y
// y1 * x1
	asm("ldw	%Y, (6, %SP)");	// YL <- y1
	asm("mul	%Y, %A");
	asm("ld		%A, %YL");
	asm("add	%A, (1, %SP)");
	asm("ld		(1, %SP), %A");	// p2 += YL
// y0 * x2
	asm("ldw	%Y, (7, %SP)");	// YL <- y0
	asm("ld		%A, (%X)");		// A  <- x2
	asm("mul	%Y, %A");
	asm("ld		%A, %YL");
	asm("add	%A, (1, %SP)");
	asm("ld		(%X), %A");	// p2 += YL

	asm("ldw	%Y, (2, %SP)");
	asm("ldw	(1, %X), %Y");
#endif
}

void _mul32Ptr(unsigned long y)
{
#if 0
	long prod = 0;
	char count = 24;
	do {
		asm("ld		%A, (3, %X)");
		asm("rrc	%A");
		asm("jrnc	.+18");

		asm("ldw	%Y, (10, %SP)");
		asm("addw	%Y, (3, %SP)");
		asm("ldw	(3, %SP), %Y");
		asm("ldw	%Y, (8, %SP)");
		asm("jrnc	.+2");
		asm("incw	%Y");
		asm("addw	%Y, (1, %SP)");
		asm("ldw	(1, %SP), %Y");

		asm("rrc	(1, %SP)");
		asm("rrc	(2, %SP)");
		asm("rrc	(3, %SP)");
		asm("rrc	(4, %SP)");
		asm("rrc	(%X)");
		asm("rrc	(1, %X)");
		asm("rrc	(2, %X)");
		asm("rrc	(3, %X)");
	} while ( --count );
#else
	long prod;
// y0 * x0
	asm("ld		%A, (3, %X)");	// A  <- x0
	asm("ldw	%Y, (9, %SP)");	// YL <- y0
	asm("mul	%Y, %A");
	asm("ldw	(3, %SP), %Y");
// y2 * x0
	asm("ldw	%Y, (7, %SP)");	// YL <- y2
	asm("mul	%Y, %A");
	asm("ldw	(1, %SP), %Y");
// y1 * x0
	asm("ldw	%Y, (8, %SP)");	// YL <- y1
	asm("mul	%Y, %A");
	asm("addw	%Y, (2, %SP)");
	asm("ldw	(2, %SP), %Y");
	asm("jrnc	.+2");
	asm("inc	(1, %SP)");
// y3 * x0
	asm("ldw	%Y, (6, %SP)");	// YL <- y3
	asm("mul	%Y, %A");
	asm("ld		%A, %YL");
	asm("add	%A, (1, %SP)");
	asm("ld		(1, %SP), %A");
// y0 * x1
	asm("ld		%A, (2, %X)");	// A  <- x1
	asm("ldw	%Y, (9, %SP)");	// YL <- y0
	asm("mul	%Y, %A");
	asm("addw	%Y, (2, %SP)");
	asm("ldw	(2, %SP), %Y");
	asm("jrnc	.+2");
	asm("inc	(1, %SP)");
// y1 * x1
	asm("ldw	%Y, (8, %SP)");	// YL <- y1
	asm("mul	%Y, %A");
	asm("addw	%Y, (1, %SP)");
	asm("ldw	(1, %SP), %Y");
// y2 * x1
	asm("ldw	%Y, (7, %SP)");	// YL <- y2
	asm("mul	%Y, %A");
	asm("ld		%A, %YL");
	asm("add	%A, (1, %SP)");
	asm("ld		(1, %SP), %A");
// y0 * x2
	asm("ld		%A, (1, %X)");	// A  <- x2
	asm("ldw	%Y, (9, %SP)");	// YL <- y0
	asm("mul	%Y, %A");
	asm("addw	%Y, (1, %SP)");
	asm("ldw	(1, %SP), %Y");
// y1 * x2
	asm("ldw	%Y, (8, %SP)");	// YL <- y1
	asm("mul	%Y, %A");
	asm("ld		%A, %YL");
	asm("add	%A, (1, %SP)");
	asm("ld		(1, %SP), %A");
// y0 * x3
	asm("ld		%A, (%X)");		// A  <- x3
	asm("ldw	%Y, (9, %SP)");	// YL <- y0
	asm("mul	%Y, %A");
	asm("ld		%A, %YL");
	asm("add	%A, (1, %SP)");
	asm("ld		(1, %SP), %A");

	asm("ldw	%Y, (3, %SP)");
	asm("ldw	(2, %X), %Y");
	asm("ldw	%Y, (1, %SP)");
	asm("ldw	(%X), %Y");
#endif
}

/*
	flag0	- y signed
	flag1	- x signed
	flag2	- mod oper
*/
void _divmod24Ptr(char flag, short y)
{
	char count = 24;	// offset = 1

	if ( flag & 1 )	// y signed?
	{
		flag--;
		asm("ldw	%Y, (5, %SP)");
		asm("jrpl	.+12");
		asm("cplw	%Y");
		asm("neg	(7, %SP)");
		asm("jrne	.+2");
		asm("incw	%Y");
		asm("ldw	(5, %SP), %Y");
		flag++;
	}

	if ( flag & 2 )	// x signed?
	{
		asm("tnz	(%X)");
		asm("jrpl	.+5");
		_negPtrX24();
		flag--;
	}

	asm("clr	%A");	// r[2]	  = 0;
	asm("clrw	%Y");	// r[1,0] = 0;
	do {
		asm("sll	(2, %X)");
		asm("rlc	(1, %X)");
		asm("rlc	(%X)");
		asm("rlcw	%Y");
		asm("rlc	%A");
		asm("subw	%Y, (6, %SP)");	// Y -= y[1,0]
		asm("sbc	%A, (5, %SP)");	// A -= y[2],C
		asm("jrc	.+4");			// borrow happens
		asm("inc	(2, %X)");
		continue;
		asm("addw	%Y, (6, %SP)");	// Y += y[1,0]
		asm("adc	%A, (5, %SP)");	// A += y[2],C
	} while ( --count );

	asm("ld		(1, %SP), %A");
	if ( flag & 4 )	// mod operation?
	{
		asm("ldw	(1, %X), %Y");
		asm("ld		%A, (1, %SP)");
		asm("ld		(%X), %A");
	}
	if ( flag & 1 )	// negative result?
		_negPtrX24();
}

void _divmod32Ptr(char flag, long y)
{
	int  temp = 0;		// offset = 1 (lower 16-bit of remaining)
	char count = 32;	// offset = 3

	if ( flag & 1 )	// y signed?
	{
		flag--;
		asm("ldw	%Y, (7, %SP)");
		asm("jrpl	.+18");

		asm("cplw	%Y");
		asm("cpl	(9, %SP)");
		asm("neg	(10, %SP)");
		asm("jrne	.+2");	asm("inc	(9, %SP)");
		asm("jrne	.+2");	asm("incw	%Y");
		asm("ldw	(7, %SP), %Y");
		flag++;
	}
	if ( flag & 2 )	// x signed?
	{
		asm("tnz	(%X)");	// x positive?
		asm("jrpl	.+5");
		_negPtrX32();
		flag--;
	}

	asm("clrw	%Y");
	do {
		asm("sll	(3, %X)");
		asm("rlc	(2, %X)");
		asm("rlc	(1, %X)");
		asm("rlc	(%X)");
		asm("rlcw	%Y");
		asm("rlc	(2, %SP)");
		asm("rlc	(1, %SP)");
		asm("subw	%Y, (9, %SP)");// Y -= y[1,0]
		asm("ld		%A, (2, %SP)");
		asm("sbc	%A, (8, %SP)");
		asm("ld		(2, %SP), %A");	// r[2] -= y[2],C
		asm("ld		%A, (1, %SP)");
		asm("sbc	%A, (7, %SP)"); // r[3] -= y[3],C
		asm("jrnc	.+11");			// no borrow happens
		asm("addw	%Y, (9, %SP)");
		asm("ld		%A, (2, %SP)");
		asm("adc	%A, (8, %SP)");
		asm("ld		(2, %SP), %A");
		continue;
		asm("ld		(1, %SP), %A");
		asm("inc	(3, %X)");
	} while ( --count );

	if ( flag & 4 )	// mod operation?
	{
		asm("ldw	(2, %X), %Y");
		asm("ldw	%Y, (1, %SP)");
		asm("ldw	(%X), %Y");
	}
	if ( flag & 1 )	// negative result?
		_negPtrX32();
}

// -- x /= y or x %= y --
void _divmod24(char flag, short y, short x)
{
	char count = 24;	// offset = 1
	if ( flag & 1 )
	{
		flag--;
		asm("ldw	%X, %SP");
		asm("addw	%X, #8");
		asm("tnz	(%X)");
		asm("jrpl	.+5");
		_negPtrX24();
		flag++;
	}
	if ( flag & 2 )
	{
		asm("ldw	%X, %SP");
		asm("addw	%X, #5");
		asm("tnz	(%X)");
		asm("jrpl	.+5");
		_negPtrX24();
		flag--;
	}

	asm("clr	%A");	// r[2]		= 0
	asm("clrw	%X");	// r[1,0]	= 0
	do {
		asm("sll	(10, %SP)");
		asm("rlc	(9, %SP)");
		asm("rlc	(8, %SP)");
		asm("rlcw	%X");
		asm("rlc	%A");
		asm("subw	%X, (6, %SP)");
		asm("sbc	%A, (5, %SP)");
		asm("jrc	.+4");
		asm("inc	(10, %SP)");
		continue;
		asm("addw	%X, (6, %SP)");
		asm("adc	%A, (5, %SP)");
	} while ( --count );
	asm("ld		%YL, %A");
	if ( flag & 4 ) //	mod operation
	{
		asm("ldw	(9, %SP), %X");
		asm("ld		%A, %YL");
		asm("ld		(8, %SP), %A");
	}

	if ( flag & 1 )	// negative value
	{
		asm("ldw	%X, %SP");
		asm("addw	%X, #8");
		_negPtrX24();
	}
}

void _divmod32(char flag, long y, long x)
{
	char count = 32;	// offset = 1
	if ( flag & 1 )		// x signed?
	{
		flag--;
		asm("ldw	%X, %SP");
		asm("addw	%X, #9");
		asm("tnz	(%X)");
		asm("jrpl	.+5");
		_negPtrX32();
		flag++;
	}
	if ( flag & 2 )		// y signed?
	{
		asm("ldw	%X, %SP");
		asm("addw	%X, #5");
		asm("tnz	(%X)");
		asm("jrpl	.+5");
		_negPtrX32();
		flag--;
	}

	asm("clrw	%Y");	// r[3,2]	= 0
	asm("clrw	%X");	// r[1,0]	= 0
	do {
		asm("sll	(12, %SP)");
		asm("rlc	(11, %SP)");
		asm("rlc	(10, %SP)");
		asm("rlc	(9, %SP)");
		asm("rlcw	%X");
		asm("rlcw	%Y");
		asm("subw	%X, (7, %SP)");
		asm("ld		%A, %YL");
		asm("sbc	%A, (6, %SP)");
		asm("ld		%YL, %A");
		asm("ld		%A, %YH");
		asm("sbc	%A, (5, %SP)");
		asm("ld		%YH, %A");
		asm("jrc	.+4");
		asm("inc	(12, %SP)");
		continue;
		asm("addw	%X, (7, %SP)");
		asm("jrnc	.+2");
		asm("incw	%Y");
		asm("addw	%Y, (5, %SP)");
	} while ( --count );

	if ( flag & 4 ) //	mod operation
	{
		asm("ldw	(11, %SP), %X");
		asm("ldw	(9 , %SP), %Y");
	}

	if ( flag & 1 )	// negative value
	{
		asm("ldw	%X, %SP");
		asm("addw	%X, #9");
		_negPtrX32();
	}
}

void _leftShift16(void)
{
	asm("tnz	%A");
	asm("jreq	.+6");
	asm("sll	(1, %X)");
	asm("rlc	(%X)");
	asm("dec	%A");
	asm("jra	_leftShift16+1");
}

void _leftShift24(void)
{
	asm("tnz	%A");
	asm("jreq	.+8");
	asm("sll	(2, %X)");
	asm("rlc	(1, %X)");
	asm("rlc	(%X)");
	asm("dec	%A");
	asm("jra	_leftShift24+1");
}

void _leftShift32(void)
{
	asm("inc	%A");
	asm("jra	.+7");
	asm("sll	(3, %X)");
	asm("rlc	(2, %X)");
	asm("rlc	(1, %X)");
	asm("rlc	(%X)");
	asm("dec	%A");
	asm("jrne	.-10");
}

static void _negPtrX24(void)
{
	asm("ldw	%Y, %X");
	asm("ldw	%Y, (%Y)");
	asm("cplw	%Y");
	asm("neg	(2, %X)");
	asm("jrne	.+2");
	asm("incw	%Y");
	asm("ldw	(%X), %Y");
}

static void _negPtrX32(void)
{
	asm("cpl	(%X)");
	asm("cpl	(1, %X)");
	asm("cpl	(2, %X)");
	asm("neg	(3, %X)");
	asm("jrne	.+9");
	asm("inc	(2, %X)");
	asm("jrne	.+5");
	asm("inc	(1, %X)");
	asm("jrne	.+1");
	asm("inc	(%X)");
}

void _decodeZflag8(void)
{
	asm("push	%CC");			// save Flags
	asm("or		%A, (4, %SP)");
	asm("jreq	.+6");
	asm("ld		%A, (1, %SP)");
	asm("and	%A, #0xfd");	// clear Z flag
	asm("ld		(1, %SP), %A");
	asm("pop	%CC");			// restore Flags
}

void _decodeZflag16(void)
{
#if 0
	asm("push	%CC");			// save Flags
	asm("or		%A, (4, %SP)");
	asm("or		%A, (5, %SP)");
	asm("jreq	.+6");
	asm("ld		%A, (1, %SP)");
	asm("and	%A, #0xfd");	// clear Z flag
	asm("ld		(1, %SP), %A");
	asm("pop	%CC");			// restore Flags
#else
	asm("push	%CC");			// save Flags
	asm("or		%A, (5, %SP)");
	asm(".jp	_decodeZflag8+1");
#endif
}

void _decodeZflag24(void)
{
#if 0
	asm("push	%CC");			// save Flags
	asm("or		%A, (4, %SP)");
	asm("or		%A, (5, %SP)");
	asm("or		%A, (6, %SP)");
	asm("jreq	.+6");
	asm("ld		%A, (1, %SP)");
	asm("and	%A, #0xfd");	// clear Z flag
	asm("ld		(1, %SP), %A");
	asm("pop	%CC");			// restore Flags
#else
	asm("push	%CC");			// save Flags
	asm("or		%A, (6, %SP)");
	asm(".jp	_decodeZflag16+1");
#endif
}
