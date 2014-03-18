#ifndef DISIODEFINES_H_
#define DISIODEFINES_H_

#include "spi1.h"
#include "bio.h"



#define SD_SELECT()        	board_io_sd_cs()
#define	SD_DESELECT()      	board_io_sd_ds()
#define SD_PRESENT()        (!board_io_sd_card_cd_sig())   /* Low means card is present */
#define	FCLK_SLOW()			spi1_set_max_clock(1)          /* Set slow SPI clock (100k-400k) */
#define	FCLK_FAST()			spi1_set_max_clock(24)         /* Set fast SPI clock (depends on the CSD) */

#define xmit_spi(dat)		spi1_exchange_byte(dat)
#define rcvr_spi()			spi1_exchange_byte(0xff)


#endif /* DISIODEFINES_H_ */
