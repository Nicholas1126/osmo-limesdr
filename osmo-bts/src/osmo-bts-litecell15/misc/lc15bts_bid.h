#ifndef _LC15BTS_BOARD_H
#define _LC15BTS_BOARD_H

#include <stdint.h>

enum lc15bts_option_type {
        LC15BTS_OPTION_OCXO,
	LC15BTS_OPTION_FPGA,
	LC15BTS_OPTION_PA,
	LC15BTS_OPTION_BAND,
	LC15BTS_OPTION_TX_ISO_BYP,
	LC15BTS_OPTION_RX_DUP_BYP,
	LC15BTS_OPTION_RX_PB_BYP,
	LC15BTS_OPTION_RX_DIV,
	LC15BTS_OPTION_RX1A,
	LC15BTS_OPTION_RX1B,
	LC15BTS_OPTION_RX2A,
	LC15BTS_OPTION_RX2B,
	LC15BTS_OPTION_DDR_32B,
	LC15BTS_OPTION_DDR_ECC,
	LC15BTS_OPTION_LOG_DET,
	LC15BTS_OPTION_DUAL_LOG_DET,
        _NUM_OPTION_TYPES
};

enum lc15bts_ocxo_type {
	LC15BTS_OCXO_BILAY_NVG45AV2072,
	LC15BTS_OCXO_TAITIEN_NJ26M003,
        _NUM_OCXO_TYPES
};

enum lc15bts_fpga_type {
        LC15BTS_FPGA_35T,
        LC15BTS_FPGA_50T,
        LC15BTS_FPGA_75T,
        LC15BTS_FPGA_100T,
        _NUM_FPGA_TYPES
};

enum lc15bts_gsm_band {
        LC15BTS_BAND_850,
        LC15BTS_BAND_900,
        LC15BTS_BAND_1800,
        LC15BTS_BAND_1900,
};

int lc15bts_rev_get(void);
int lc15bts_model_get(void);
int lc15bts_option_get(enum lc15bts_option_type type);
const char* get_hwversion_desc();

#endif
