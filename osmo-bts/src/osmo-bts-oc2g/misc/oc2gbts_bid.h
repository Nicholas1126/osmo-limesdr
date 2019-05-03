#ifndef _OC2GBTS_BOARD_H
#define _OC2GBTS_BOARD_H

#include <stdint.h>

enum oc2gbts_option_type {
	OC2GBTS_OPTION_OCXO,
	OC2GBTS_OPTION_FPGA,
	OC2GBTS_OPTION_PA,
	OC2GBTS_OPTION_BAND,
	OC2GBTS_OPTION_TX_ATT,
	OC2GBTS_OPTION_TX_FIL,
	OC2GBTS_OPTION_RX_ATT,
	OC2GBTS_OPTION_RMS_FWD,
	OC2GBTS_OPTION_RMS_REFL,
	OC2GBTS_OPTION_DDR_32B,
	OC2GBTS_OPTION_DDR_ECC,
	OC2GBTS_OPTION_PA_TEMP,
	_NUM_OPTION_TYPES
};

enum oc2gbts_ocxo_type {
	OC2GBTS_OCXO_BILAY_NVG45AV2072,
	OC2GBTS_OCXO_TAITIEN_NJ26M003,
	_NUM_OCXO_TYPES
};

enum oc2gbts_fpga_type {
	OC2GBTS_FPGA_35T,
	OC2GBTS_FPGA_50T,
	OC2GBTS_FPGA_75T,
	OC2GBTS_FPGA_100T,
	_NUM_FPGA_TYPES
};

enum oc2gbts_gsm_band {
	OC2GBTS_BAND_850,
	OC2GBTS_BAND_900,
	OC2GBTS_BAND_1800,
	OC2GBTS_BAND_1900,
};

void oc2gbts_rev_get(char *rev_maj, char *rev_min);
int oc2gbts_model_get(void);
int oc2gbts_option_get(enum oc2gbts_option_type type);
const char* get_hwversion_desc();

#endif
