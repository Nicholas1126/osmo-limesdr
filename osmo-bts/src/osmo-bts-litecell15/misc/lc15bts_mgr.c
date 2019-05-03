/* Main program for NuRAN Wireless Litecell 1.5 BTS management daemon */

/* Copyright (C) 2015 by Yves Godin <support@nuranwireless.com>
 * 
 * Based on sysmoBTS:
 *     sysmobts_mgr.c
 *     (C) 2012 by Harald Welte <laforge@gnumonks.org>
 *     (C) 2014 by Holger Hans Peter Freyther
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <sys/signal.h>
#include <sys/stat.h>

#include <osmocom/core/talloc.h>
#include <osmocom/core/application.h>
#include <osmocom/core/timer.h>
#include <osmocom/core/msgb.h>
#include <osmocom/vty/telnet_interface.h>
#include <osmocom/vty/logging.h>
#include <osmocom/vty/ports.h>

#include "misc/lc15bts_misc.h"
#include "misc/lc15bts_mgr.h"
#include "misc/lc15bts_par.h"
#include "misc/lc15bts_bid.h"
#include "misc/lc15bts_power.h"
#include "misc/lc15bts_swd.h"

#include "lc15bts_led.h"

static int no_rom_write = 0;
static int daemonize = 0;
void *tall_mgr_ctx;

/* every 6 hours means 365*4 = 1460 rom writes per year (max) */
#define SENSOR_TIMER_SECS		(6 * 3600)

/* every 1 hours means 365*24 = 8760 rom writes per year (max) */
#define HOURS_TIMER_SECS	(1 * 3600)


/* the initial state */
static struct lc15bts_mgr_instance manager = {
	.config_file	= "lc15bts-mgr.cfg",
	.temp = {
		.supply_temp_limit	= {
			.thresh_warn_max	= 80,
			.thresh_crit_max	= 85,
			.thresh_warn_min	= -40,
		},
		.soc_temp_limit	= {
			.thresh_warn_max	= 95,
			.thresh_crit_max	= 100,
			.thresh_warn_min	= -40,
		},
		.fpga_temp_limit	= {
			.thresh_warn_max	= 95,
			.thresh_crit_max	= 100,
			.thresh_warn_min	= -40,
		},
		.rmsdet_temp_limit	= {
			.thresh_warn_max	= 80,
			.thresh_crit_max	= 85,
			.thresh_warn_min	= -40,
		},
		.ocxo_temp_limit	= {
			.thresh_warn_max	= 80,
			.thresh_crit_max	= 85,
			.thresh_warn_min	= -40,
		},
		.tx0_temp_limit	= {
			.thresh_warn_max	= 80,
			.thresh_crit_max	= 85,
			.thresh_warn_min	= -20,
		},
		.tx1_temp_limit	= {
			.thresh_warn_max	= 80,
			.thresh_crit_max	= 85,
			.thresh_warn_min	= -20,
		},
		.pa0_temp_limit	= {
			.thresh_warn_max	= 80,
			.thresh_crit_max	= 85,
			.thresh_warn_min	= -40,
		},
		.pa1_temp_limit	= {
			.thresh_warn_max	= 80,
			.thresh_crit_max	= 85,
			.thresh_warn_min	= -40,
		}
	},
	.volt = {
		.supply_volt_limit = {
			.thresh_warn_max	= 30000,
			.thresh_crit_max	= 30500,
			.thresh_warn_min	= 19000,
			.thresh_crit_min	= 17500,
		}
	},
	.pwr = {
		.supply_pwr_limit = {
			.thresh_warn_max	= 110,
			.thresh_crit_max	= 120,
		},
		.pa0_pwr_limit = {
			.thresh_warn_max	= 50,
			.thresh_crit_max	= 60,
		},
		.pa1_pwr_limit = {
			.thresh_warn_max	= 50,
			.thresh_crit_max	= 60,
		}
	},
	.vswr = {
		.tx0_vswr_limit = {
			.thresh_warn_max	= 3000,
			.thresh_crit_max	= 5000,
		},
		.tx1_vswr_limit = {
			.thresh_warn_max	= 3000,
			.thresh_crit_max	= 5000,
		}
	},
	.gps = {
		.gps_fix_limit = {
			.thresh_warn_max	= 7,
		}
	},
	.state = {
		.action_norm		= SENSOR_ACT_NORM_PA0_ON | SENSOR_ACT_NORM_PA1_ON,
		.action_warn		= 0,
		.action_crit		= 0,
		.action_comb		= 0,
		.state			= STATE_NORMAL,
	}
};

static struct osmo_timer_list sensor_timer;
static void check_sensor_timer_cb(void *unused)
{
	lc15bts_check_temp(no_rom_write);
	lc15bts_check_power(no_rom_write);
	lc15bts_check_vswr(no_rom_write);
	osmo_timer_schedule(&sensor_timer, SENSOR_TIMER_SECS, 0);
	/* TODO checks if lc15bts_check_temp/lc15bts_check_power/lc15bts_check_vswr went ok */
	lc15bts_swd_event(&manager, SWD_CHECK_SENSOR);
}

static struct osmo_timer_list hours_timer;
static void hours_timer_cb(void *unused)
{
	lc15bts_update_hours(no_rom_write);

	osmo_timer_schedule(&hours_timer, HOURS_TIMER_SECS, 0);
	/* TODO: validates if lc15bts_update_hours went correctly */
	lc15bts_swd_event(&manager, SWD_UPDATE_HOURS);
}

static void print_help(void)
{
	printf("lc15bts-mgr [-nsD] [-d cat]\n");
	printf(" -n Do not write to ROM\n");
	printf(" -s Disable color\n");
	printf(" -d CAT enable debugging\n");
	printf(" -D daemonize\n");
	printf(" -c Specify the filename of the config file\n");
}

static int parse_options(int argc, char **argv)
{
	int opt;

	while ((opt = getopt(argc, argv, "nhsd:c:")) != -1) {
		switch (opt) {
		case 'n':
			no_rom_write = 1;
			break;
		case 'h':
			print_help();
			return -1;
		case 's':
			log_set_use_color(osmo_stderr_target, 0);
			break;
		case 'd':
			log_parse_category_mask(osmo_stderr_target, optarg);
			break;
		case 'D':
			daemonize = 1;
			break;
		case 'c':
			manager.config_file = optarg;
			break;
		default:
			return -1;
		}
	}

	return 0;
}

static void signal_handler(int signal)
{
	fprintf(stderr, "signal %u received\n", signal);

	switch (signal) {
	case SIGINT:
	case SIGTERM:
		lc15bts_check_temp(no_rom_write);
		lc15bts_check_power(no_rom_write);
		lc15bts_check_vswr(no_rom_write);
		lc15bts_update_hours(no_rom_write);
		exit(0);
		break;
	case SIGABRT:
	case SIGUSR1:
	case SIGUSR2:
		talloc_report_full(tall_mgr_ctx, stderr);
		break;
	default:
		break;
	}
}

static struct log_info_cat mgr_log_info_cat[] = {
	[DTEMP] = {
		.name = "DTEMP",
		.description = "Temperature monitoring",
		.color = "\033[1;35m",
		.enabled = 1, .loglevel = LOGL_INFO,
	},
	[DFW] =	{
		.name = "DFW",
		.description = "Firmware management",
		.color = "\033[1;36m",
		.enabled = 1, .loglevel = LOGL_INFO,
	},
	[DFIND] = {
		.name = "DFIND",
		.description = "ipaccess-find handling",
		.color = "\033[1;37m",
		.enabled = 1, .loglevel = LOGL_INFO,
	},
	[DCALIB] = {
		.name = "DCALIB",
		.description = "Calibration handling",
		.color = "\033[1;37m",
		.enabled = 1, .loglevel = LOGL_INFO,
	},
	[DSWD] = {
		.name = "DSWD",
		.description = "Software Watchdog",
		.color = "\033[1;37m",
		.enabled = 1, .loglevel = LOGL_INFO,
	},
};

static const struct log_info mgr_log_info = {
	.cat = mgr_log_info_cat,
	.num_cat = ARRAY_SIZE(mgr_log_info_cat),
};

int main(int argc, char **argv)
{
	int rc;

	tall_mgr_ctx = talloc_named_const(NULL, 1, "bts manager");
	msgb_talloc_ctx_init(tall_mgr_ctx, 0);

	osmo_init_logging2(tall_mgr_ctx, &mgr_log_info);

	osmo_init_ignore_signals();
	signal(SIGINT, &signal_handler);
	signal(SIGTERM, &signal_handler);
	signal(SIGUSR1, &signal_handler);
	signal(SIGUSR2, &signal_handler);

	rc = parse_options(argc, argv);
	if (rc < 0)
		exit(2);

	lc15bts_mgr_vty_init();
	logging_vty_add_cmds(&mgr_log_info);
	rc = lc15bts_mgr_parse_config(&manager);
	if (rc < 0) {
		LOGP(DFIND, LOGL_FATAL, "Cannot parse config file\n");
		exit(1);
	}

	rc = telnet_init(tall_mgr_ctx, NULL, OSMO_VTY_PORT_BTSMGR);
	if (rc < 0) {
		fprintf(stderr, "Error initializing telnet\n");
		exit(1);
	}

	INIT_LLIST_HEAD(&manager.lc15bts_leds.list);
	INIT_LLIST_HEAD(&manager.alarms.list);

	/* Initialize the service watchdog notification for SWD_LAST event(s) */
	if (lc15bts_swd_init(&manager, (int)(SWD_LAST)) != 0)
		exit(3);

	/* start temperature check timer */
	sensor_timer.cb = check_sensor_timer_cb;
	check_sensor_timer_cb(NULL);

	/* start operational hours timer */
	hours_timer.cb = hours_timer_cb;
	hours_timer_cb(NULL);

 	/* Enable the PAs */
	rc = lc15bts_power_set(LC15BTS_POWER_PA0, 1);
	if (rc < 0) {
		exit(3);
	}

	rc = lc15bts_power_set(LC15BTS_POWER_PA1, 1);
	if (rc < 0) {
		exit(3);
	}

	/* handle broadcast messages for ipaccess-find */
	if (lc15bts_mgr_nl_init() != 0)
		exit(3);

	/* Initialize the sensor control */
	lc15bts_mgr_sensor_init(&manager);

	if (lc15bts_mgr_calib_init(&manager) != 0)
		exit(3);

	if (daemonize) {
		rc = osmo_daemonize();
		if (rc < 0) {
			perror("Error during daemonize");
			exit(1);
		}
	}

	while (1) {
		log_reset_context();
		osmo_select_main(0);
		lc15bts_swd_event(&manager, SWD_MAINLOOP);
	}
}
