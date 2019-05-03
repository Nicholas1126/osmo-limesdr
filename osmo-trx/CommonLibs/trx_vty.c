/*
 * Copyright (C) 2012-2017 sysmocom - s.f.m.c. GmbH
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <osmocom/core/talloc.h>
#include <osmocom/core/utils.h>
#include <osmocom/core/rate_ctr.h>

#include <osmocom/vty/command.h>
#include <osmocom/vty/vty.h>
#include <osmocom/vty/misc.h>

#include "trx_vty.h"
#include "../config.h"

static struct trx_ctx* g_trx_ctx;

static const struct value_string clock_ref_names[] = {
	{ REF_INTERNAL,	"internal" },
	{ REF_EXTERNAL,	"external" },
	{ REF_GPS,	"gpsdo" },
	{ 0,		NULL }
};

static const struct value_string filler_names[] = {
	{ FILLER_DUMMY,		"Dummy bursts" },
	{ FILLER_ZERO,		"Disabled" },
	{ FILLER_NORM_RAND,	"Normal bursts with random payload" },
	{ FILLER_EDGE_RAND,	"EDGE bursts with random payload" },
	{ FILLER_ACCESS_RAND,	"Access bursts with random payload" },
	{ 0,			NULL }
};

struct trx_ctx *trx_from_vty(struct vty *v)
{
        /* It can't hurt to force callers to continue to pass the vty instance
         * to this function, in case we'd like to retrieve the global
         * trx instance from the vty at some point in the future. But
         * until then, just return the global pointer, which should have been
         * initialized by trx_vty_init().
         */
        OSMO_ASSERT(g_trx_ctx);
        return g_trx_ctx;
}

enum trx_vty_node {
	TRX_NODE = _LAST_OSMOVTY_NODE + 1,
	CHAN_NODE,
};

static struct cmd_node trx_node = {
	TRX_NODE,
	"%s(config-trx)# ",
	1,
};

static struct cmd_node chan_node = {
	CHAN_NODE,
	"%s(config-trx-chan)# ",
	1,
};

DEFUN(cfg_trx, cfg_trx_cmd,
	"trx",
	"Configure the TRX\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	if (!trx)
		return CMD_WARNING;

	vty->node = TRX_NODE;

	return CMD_SUCCESS;
}

DEFUN(cfg_bind_ip, cfg_bind_ip_cmd,
	"bind-ip A.B.C.D",
	"Set the IP address for the local bind\n"
	"IPv4 Address\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	osmo_talloc_replace_string(trx, &trx->cfg.bind_addr, argv[0]);

	return CMD_SUCCESS;
}

DEFUN(cfg_remote_ip, cfg_remote_ip_cmd,
	"remote-ip A.B.C.D",
	"Set the IP address for the remote BTS\n"
	"IPv4 Address\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	osmo_talloc_replace_string(trx, &trx->cfg.remote_addr, argv[0]);

	return CMD_SUCCESS;
}

DEFUN(cfg_base_port, cfg_base_port_cmd,
	"base-port <1-65535>",
	"Set the TRX Base Port\n"
	"TRX Base Port\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	trx->cfg.base_port = atoi(argv[0]);

	return CMD_SUCCESS;
}

DEFUN(cfg_dev_args, cfg_dev_args_cmd,
	"dev-args DESC",
	"Set the device-specific arguments to pass to the device\n"
	"Device-specific arguments\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	osmo_talloc_replace_string(trx, &trx->cfg.dev_args, argv[0]);

	return CMD_SUCCESS;
}

DEFUN(cfg_tx_sps, cfg_tx_sps_cmd,
	"tx-sps (1|4)",
	"Set the Tx Samples-per-Symbol\n"
	"Tx Samples-per-Symbol\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	trx->cfg.tx_sps = atoi(argv[0]);

	return CMD_SUCCESS;
}

DEFUN(cfg_rx_sps, cfg_rx_sps_cmd,
	"rx-sps (1|4)",
	"Set the Rx Samples-per-Symbol\n"
	"Rx Samples-per-Symbol\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	trx->cfg.rx_sps = atoi(argv[0]);

	return CMD_SUCCESS;
}

DEFUN(cfg_test_rtsc, cfg_test_rtsc_cmd,
	"test rtsc <0-7>",
	"Set the Random Normal Burst test mode with TSC\n"
	"TSC\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	if (trx->cfg.rach_delay_set) {
		vty_out(vty, "rach-delay and rtsc options are mutual-exclusive%s",
			VTY_NEWLINE);
		return CMD_WARNING;
	}

	trx->cfg.rtsc_set = true;
	trx->cfg.rtsc = atoi(argv[0]);
	if (!trx->cfg.egprs) /* Don't override egprs which sets different filler */
		trx->cfg.filler = FILLER_NORM_RAND;

	return CMD_SUCCESS;
}

DEFUN(cfg_test_rach_delay, cfg_test_rach_delay_cmd,
	"test rach-delay <0-68>",
	"Set the Random Access Burst test mode with delay\n"
	"RACH delay\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	if (trx->cfg.rtsc_set) {
		vty_out(vty, "rach-delay and rtsc options are mutual-exclusive%s",
			VTY_NEWLINE);
		return CMD_WARNING;
	}

	if (trx->cfg.egprs) {
		vty_out(vty, "rach-delay and egprs options are mutual-exclusive%s",
			VTY_NEWLINE);
		return CMD_WARNING;
	}

	trx->cfg.rach_delay_set = true;
	trx->cfg.rach_delay = atoi(argv[0]);
	trx->cfg.filler = FILLER_ACCESS_RAND;

	return CMD_SUCCESS;
}

DEFUN(cfg_clock_ref, cfg_clock_ref_cmd,
	"clock-ref (internal|external|gpsdo)",
	"Set the Reference Clock\n"
	"Enable internal referece (default)\n"
	"Enable external 10 MHz reference\n"
	"Enable GPSDO reference\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	trx->cfg.clock_ref = get_string_value(clock_ref_names, argv[0]);

	return CMD_SUCCESS;
}

DEFUN(cfg_multi_arfcn, cfg_multi_arfcn_cmd,
	"multi-arfcn (disable|enable)",
	"Enable multi-ARFCN transceiver (default=disable)\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	if (strcmp("disable", argv[0]) == 0) {
		trx->cfg.multi_arfcn = false;
		return CMD_SUCCESS;
	}

	if (trx->cfg.num_chans > TRX_MCHAN_MAX) {
		vty_out(vty, "Up to %i channels are supported for multi-TRX mode%s",
			TRX_MCHAN_MAX, VTY_NEWLINE);
		return CMD_WARNING;
	}

	trx->cfg.multi_arfcn = true;
	return CMD_SUCCESS;
}

DEFUN(cfg_offset, cfg_offset_cmd,
	"offset FLOAT",
	"Set the baseband frequency offset (default=0, auto)\n"
	"Baseband Frequency Offset\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	trx->cfg.offset = atof(argv[0]);

	return CMD_SUCCESS;
}

DEFUN(cfg_rssi_offset, cfg_rssi_offset_cmd,
	"rssi-offset FLOAT",
	"Set the RSSI to dBm offset in dB (default=0)\n"
	"RSSI to dBm offset in dB\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	trx->cfg.rssi_offset = atof(argv[0]);

	return CMD_SUCCESS;
}

DEFUN(cfg_swap_channels, cfg_swap_channels_cmd,
	"swap-channels (disable|enable)",
	"Swap channels (default=disable)\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	if (strcmp("disable", argv[0]) == 0) {
		trx->cfg.swap_channels = false;
	} else if (strcmp("enable", argv[0]) == 0) {
		trx->cfg.swap_channels = true;
	} else {
		return CMD_WARNING;
	}

	return CMD_SUCCESS;
}

DEFUN(cfg_egprs, cfg_egprs_cmd,
	"egprs (disable|enable)",
	"Enable EDGE receiver (default=disable)\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	if (strcmp("disable", argv[0]) == 0) {
		trx->cfg.egprs = false;
	} else if (strcmp("enable", argv[0]) == 0) {
		trx->cfg.egprs = true;
		trx->cfg.filler = FILLER_EDGE_RAND;
	} else {
		return CMD_WARNING;
	}

	return CMD_SUCCESS;
}

DEFUN(cfg_ext_rach, cfg_ext_rach_cmd,
	"ext-rach (disable|enable)",
	"Enable extended (11-bit) RACH (default=disable)\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	if (strcmp("disable", argv[0]) == 0)
		trx->cfg.ext_rach = false;

	if (strcmp("enable", argv[0]) == 0)
		trx->cfg.ext_rach = true;

	return CMD_SUCCESS;
}

DEFUN(cfg_rt_prio, cfg_rt_prio_cmd,
	"rt-prio <1-32>",
	"Set the SCHED_RR real-time priority\n"
	"Real time priority\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	trx->cfg.sched_rr = atoi(argv[0]);

	return CMD_SUCCESS;
}

DEFUN(cfg_filler, cfg_filler_cmd,
	"filler dummy",
	"Enable C0 filler table\n"
	"Dummy method\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	trx->cfg.filler = FILLER_DUMMY;

	return CMD_SUCCESS;
}

DEFUN(cfg_chan, cfg_chan_cmd,
	"chan <0-100>",
	"Select a channel to configure\n"
	"Channel index\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);
	int idx = atoi(argv[0]);

	if (idx >= TRX_CHAN_MAX) {
		vty_out(vty, "Chan list full.%s", VTY_NEWLINE);
		return CMD_WARNING;
	} else if (trx->cfg.multi_arfcn && trx->cfg.num_chans >= TRX_MCHAN_MAX) {
		vty_out(vty, "Up to %i channels are supported for multi-TRX mode%s",
			TRX_MCHAN_MAX, VTY_NEWLINE);
		return CMD_WARNING;
	}

	if (trx->cfg.num_chans < idx) { /* Unexisting or creating non-consecutive */
		vty_out(vty, "Non-existent or non-consecutive chan %d.%s",
				idx, VTY_NEWLINE);
		return CMD_WARNING;
	} else if (trx->cfg.num_chans == idx)  { /* creating it */
		trx->cfg.num_chans++;
		trx->cfg.chans[idx].trx = trx;
		trx->cfg.chans[idx].idx = idx;
	}

	vty->node = CHAN_NODE;
	vty->index = &trx->cfg.chans[idx];

	return CMD_SUCCESS;
}

DEFUN(cfg_chan_rx_path, cfg_chan_rx_path_cmd,
	"rx-path NAME",
	"Set the Rx Path\n"
	"Rx Path name\n")
{
	struct trx_chan *chan = vty->index;

	osmo_talloc_replace_string(chan->trx, &chan->rx_path, argv[0]);

	return CMD_SUCCESS;
}

DEFUN(cfg_chan_tx_path, cfg_chan_tx_path_cmd,
	"tx-path NAME",
	"Set the Tx Path\n"
	"Tx Path name\n")
{
	struct trx_chan *chan = vty->index;

	osmo_talloc_replace_string(chan->trx, &chan->tx_path, argv[0]);

	return CMD_SUCCESS;
}

static int dummy_config_write(struct vty *v)
{
	return CMD_SUCCESS;
}

static int config_write_trx(struct vty *vty)
{
	struct trx_chan *chan;
	int i;
	struct trx_ctx *trx = trx_from_vty(vty);

	vty_out(vty, "trx%s", VTY_NEWLINE);
	if (trx->cfg.bind_addr)
		vty_out(vty, " bind-ip %s%s", trx->cfg.bind_addr, VTY_NEWLINE);
	if (trx->cfg.remote_addr)
		vty_out(vty, " remote-ip %s%s", trx->cfg.remote_addr, VTY_NEWLINE);
	if (trx->cfg.base_port != DEFAULT_TRX_PORT)
		vty_out(vty, " base-port %u%s", trx->cfg.base_port, VTY_NEWLINE);
	if (trx->cfg.dev_args)
		vty_out(vty, " dev-args %s%s", trx->cfg.dev_args, VTY_NEWLINE);
	if (trx->cfg.tx_sps != DEFAULT_TX_SPS)
		vty_out(vty, " tx-sps %u%s", trx->cfg.tx_sps, VTY_NEWLINE);
	if (trx->cfg.rx_sps != DEFAULT_RX_SPS)
		vty_out(vty, " rx-sps %u%s", trx->cfg.rx_sps, VTY_NEWLINE);
	if (trx->cfg.rtsc_set)
		vty_out(vty, " test rtsc %u%s", trx->cfg.rtsc, VTY_NEWLINE);
	if (trx->cfg.rach_delay_set)
		vty_out(vty, " test rach-delay %u%s", trx->cfg.rach_delay, VTY_NEWLINE);
	if (trx->cfg.clock_ref != REF_INTERNAL)
		vty_out(vty, " clock-ref %s%s", get_value_string(clock_ref_names, trx->cfg.clock_ref), VTY_NEWLINE);
	vty_out(vty, " multi-arfcn %s%s", trx->cfg.multi_arfcn ? "enable" : "disable", VTY_NEWLINE);
	if (trx->cfg.offset != 0)
		vty_out(vty, " offset %f%s", trx->cfg.offset, VTY_NEWLINE);
	if (trx->cfg.rssi_offset != 0)
		vty_out(vty, " rssi-offset %f%s", trx->cfg.rssi_offset, VTY_NEWLINE);
	vty_out(vty, " swap-channels %s%s", trx->cfg.swap_channels ? "enable" : "disable", VTY_NEWLINE);
	vty_out(vty, " egprs %s%s", trx->cfg.egprs ? "enable" : "disable", VTY_NEWLINE);
	vty_out(vty, " ext-rach %s%s", trx->cfg.ext_rach ? "enable" : "disable", VTY_NEWLINE);
	if (trx->cfg.sched_rr != 0)
		vty_out(vty, " rt-prio %u%s", trx->cfg.sched_rr, VTY_NEWLINE);

	for (i = 0; i < trx->cfg.num_chans; i++) {
		chan = &trx->cfg.chans[i];
		vty_out(vty, " chan %u%s", chan->idx, VTY_NEWLINE);
		if (chan->rx_path)
			vty_out(vty, "  rx-path %s%s", chan->rx_path, VTY_NEWLINE);
		if (chan->tx_path)
			vty_out(vty, "  tx-path %s%s", chan->tx_path, VTY_NEWLINE);
	}

	return CMD_SUCCESS;
}

static void trx_dump_vty(struct vty *vty, struct trx_ctx *trx)
{
	struct trx_chan *chan;
	int i;
	vty_out(vty, "TRX Config:%s", VTY_NEWLINE);
	vty_out(vty, " Local IP: %s%s", trx->cfg.bind_addr, VTY_NEWLINE);
	vty_out(vty, " Remote IP: %s%s", trx->cfg.remote_addr, VTY_NEWLINE);
	vty_out(vty, " TRX Base Port: %u%s", trx->cfg.base_port, VTY_NEWLINE);
	vty_out(vty, " Device args: %s%s", trx->cfg.dev_args, VTY_NEWLINE);
	vty_out(vty, " Tx Samples-per-Symbol: %u%s", trx->cfg.tx_sps, VTY_NEWLINE);
	vty_out(vty, " Rx Samples-per-Symbol: %u%s", trx->cfg.rx_sps, VTY_NEWLINE);
	vty_out(vty, " Test Mode: TSC: %u (%s)%s", trx->cfg.rtsc,
		trx->cfg.rtsc_set ? "Enabled" : "Disabled", VTY_NEWLINE);
	vty_out(vty, " Test Mode: RACH Delay: %u (%s)%s", trx->cfg.rach_delay,
		trx->cfg.rach_delay_set ? "Enabled" : "Disabled", VTY_NEWLINE);
	vty_out(vty, " C0 Filler Table: %s%s", get_value_string(filler_names, trx->cfg.filler), VTY_NEWLINE);
	vty_out(vty, " Clock Reference: %s%s", get_value_string(clock_ref_names, trx->cfg.clock_ref), VTY_NEWLINE);
	vty_out(vty, " Multi-Carrier: %s%s", trx->cfg.multi_arfcn ? "Enabled" : "Disabled", VTY_NEWLINE);
	vty_out(vty, " Tuning offset: %f%s", trx->cfg.offset, VTY_NEWLINE);
	vty_out(vty, " RSSI to dBm offset: %f%s", trx->cfg.rssi_offset, VTY_NEWLINE);
	vty_out(vty, " Swap channels: %s%s", trx->cfg.swap_channels ? "Enabled" : "Disabled", VTY_NEWLINE);
	vty_out(vty, " EDGE support: %s%s", trx->cfg.egprs ? "Enabled" : "Disabled", VTY_NEWLINE);
	vty_out(vty, " Extended RACH support: %s%s", trx->cfg.ext_rach ? "Enabled" : "Disabled", VTY_NEWLINE);
	vty_out(vty, " Real Time Priority: %u (%s)%s", trx->cfg.sched_rr,
		trx->cfg.sched_rr ? "Enabled" : "Disabled", VTY_NEWLINE);
	vty_out(vty, " Channels: %u%s", trx->cfg.num_chans, VTY_NEWLINE);
	for (i = 0; i < trx->cfg.num_chans; i++) {
		chan = &trx->cfg.chans[i];
		vty_out(vty, "  Channel %u:%s", chan->idx, VTY_NEWLINE);
		if (chan->rx_path)
			vty_out(vty, "   Rx Path: %s%s", chan->rx_path, VTY_NEWLINE);
		if (chan->tx_path)
			vty_out(vty, "   Tx Path: %s%s", chan->tx_path, VTY_NEWLINE);
	}
}

DEFUN(show_trx, show_trx_cmd,
	"show trx",
	SHOW_STR "Display information on the TRX\n")
{
	struct trx_ctx *trx = trx_from_vty(vty);

	trx_dump_vty(vty, trx);

	return CMD_SUCCESS;
}

static int trx_vty_is_config_node(struct vty *vty, int node)
{
	switch (node) {
	case TRX_NODE:
	case CHAN_NODE:
		return 1;
	default:
		return 0;
	}
}

static int trx_vty_go_parent(struct vty *vty)
{
	switch (vty->node) {
	case TRX_NODE:
		vty->node = CONFIG_NODE;
		vty->index = NULL;
		vty->index_sub = NULL;
		break;
	case CHAN_NODE:
		vty->node = TRX_NODE;
		vty->index = NULL;
		vty->index_sub = NULL;
		break;
	default:
		vty->node = CONFIG_NODE;
		vty->index = NULL;
		vty->index_sub = NULL;
	}

	return vty->node;
}

static const char trx_copyright[] =
	"Copyright (C) 2007-2014 Free Software Foundation, Inc.\r\n"
	"Copyright (C) 2013 Thomas Tsou <tom@tsou.cc>\r\n"
	"Copyright (C) 2015 Ettus Research LLC\r\n"
	"Copyright (C) 2017-2018 by sysmocom s.f.m.c. GmbH <info@sysmocom.de>\r\n"
	"License AGPLv3+: GNU AGPL version 3 or later <http://gnu.org/licenses/agpl-3.0.html>\r\n"
	"This is free software: you are free to change and redistribute it.\r\n"
	"There is NO WARRANTY, to the extent permitted by law.\r\n";

struct vty_app_info g_vty_info = {
	.name		= "OsmoTRX",
	.version	= PACKAGE_VERSION,
	.copyright	= trx_copyright,
	.go_parent_cb	= trx_vty_go_parent,
	.is_config_node	= trx_vty_is_config_node,
};

struct trx_ctx *vty_trx_ctx_alloc(void *talloc_ctx)
{
	struct trx_ctx * trx = talloc_zero(talloc_ctx, struct trx_ctx);

	trx->cfg.bind_addr =  talloc_strdup(trx, DEFAULT_TRX_IP);
	trx->cfg.remote_addr = talloc_strdup(trx, DEFAULT_TRX_IP);
	trx->cfg.base_port = DEFAULT_TRX_PORT;
	trx->cfg.tx_sps = DEFAULT_TX_SPS;
	trx->cfg.rx_sps = DEFAULT_RX_SPS;
	trx->cfg.filler = FILLER_ZERO;

	return trx;
}

int trx_vty_init(struct trx_ctx* trx)
{
	g_trx_ctx = trx;
	install_element_ve(&show_trx_cmd);

	install_element(CONFIG_NODE, &cfg_trx_cmd);

	install_node(&trx_node, config_write_trx);
	install_element(TRX_NODE, &cfg_bind_ip_cmd);
	install_element(TRX_NODE, &cfg_remote_ip_cmd);
	install_element(TRX_NODE, &cfg_base_port_cmd);
	install_element(TRX_NODE, &cfg_dev_args_cmd);
	install_element(TRX_NODE, &cfg_tx_sps_cmd);
	install_element(TRX_NODE, &cfg_rx_sps_cmd);
	install_element(TRX_NODE, &cfg_test_rtsc_cmd);
	install_element(TRX_NODE, &cfg_test_rach_delay_cmd);
	install_element(TRX_NODE, &cfg_clock_ref_cmd);
	install_element(TRX_NODE, &cfg_multi_arfcn_cmd);
	install_element(TRX_NODE, &cfg_offset_cmd);
	install_element(TRX_NODE, &cfg_rssi_offset_cmd);
	install_element(TRX_NODE, &cfg_swap_channels_cmd);
	install_element(TRX_NODE, &cfg_egprs_cmd);
	install_element(TRX_NODE, &cfg_ext_rach_cmd);
	install_element(TRX_NODE, &cfg_rt_prio_cmd);
	install_element(TRX_NODE, &cfg_filler_cmd);

	install_element(TRX_NODE, &cfg_chan_cmd);
	install_node(&chan_node, dummy_config_write);
	install_element(CHAN_NODE, &cfg_chan_rx_path_cmd);
	install_element(CHAN_NODE, &cfg_chan_tx_path_cmd);

	return 0;
}
