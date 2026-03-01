#include <stm8s105x6.h>
#include "define.h"
#include "spi.h"
#include "vs1011.h"

#define VS1011_RD	0x03
#define VS1011_WR	0x02

//unsigned int  vs1011_mode_reg;
static _bank0_ unsigned char vs1011_vol_reg;

/////////////////////////////////////////////////////////////////////////
void VS1011_init (void)
{
	// set the mode in pure VS1002 mode
//	vs1011_mode_reg = 0x0802;
	VS1011_wrReg (VS1011_REG_MODE, 0x0802);								// VS1002 mode
//	VS1011_wrReg (VS1011_REG_CLOCKF, (VS1011_CLK_FREQ/2000) | 0x8000);	// 12.00MHz x 2 = 24.00MHz
}

/////////////////////////////////////////////////////////////////////////
void VS1011_reset (void)
{
	while ( VS1011_BUSY );

	VS1011_wrReg (VS1011_REG_MODE, 0x0802 | 4);

	while ( VS1011_BUSY );
}

/////////////////////////////////////////////////////////////////////////
void VS1011_setVol (unsigned char val)
{
	vs1011_vol_reg = val;
	VS1011_wrReg (VS1011_REG_VOL, val | (val << 8));
}

/////////////////////////////////////////////////////////////////////////
void VS1011_incVol (void)
{
	if ( vs1011_vol_reg >= 5 )
		vs1011_vol_reg -= 5;
	else
		vs1011_vol_reg = 0;

	VS1011_setVol (vs1011_vol_reg);
}

/////////////////////////////////////////////////////////////////////////
void VS1011_decVol (void)
{
	if ( vs1011_vol_reg <= 95 )
		vs1011_vol_reg += 5;
	else
		vs1011_vol_reg = 100;

	VS1011_setVol (vs1011_vol_reg);
}

/////////////////////////////////////////////////////////////////////////
void VS1011_wrReg (unsigned char reg, unsigned int data)
{
 	while ( VS1011_BUSY );
	SD_SCK_HOLD;
	ENABLE_SPI2();

    VS1011_ENABLE_INST;

	SPI2_write (VS1011_WR);
	SPI2_write (reg);

	SPI2_write (data >> 8);
	SPI2_write (data);

	VS1011_DISABLE_INST;
	DISABLE_SPI2();
	SD_SCK_UNHOLD;
}
