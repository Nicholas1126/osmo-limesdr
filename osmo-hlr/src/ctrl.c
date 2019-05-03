/* OsmoHLR Control Interface implementation */

/* (C) 2017 sysmocom s.f.m.c. GmbH <info@sysmocom.de>
 * All Rights Reserved
 *
 * Author: Max Suraev <msuraev@sysmocom.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdbool.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>

#include <osmocom/gsm/gsm23003.h>
#include <osmocom/ctrl/ports.h>

#include "hlr.h"
#include "ctrl.h"
#include "db.h"

#define SEL_BY "by-"
#define SEL_BY_IMSI SEL_BY "imsi-"
#define SEL_BY_MSISDN SEL_BY "msisdn-"
#define SEL_BY_ID SEL_BY "id-"

#define hexdump_buf(buf) osmo_hexdump_nospc((void*)buf, sizeof(buf))

static bool startswith(const char *str, const char *start)
{
	return strncmp(str, start, strlen(start)) == 0;
}

static int _get_subscriber(struct db_context *dbc,
			   const char *by_selector,
			   struct hlr_subscriber *subscr)
{
	const char *val;
	if (startswith(by_selector, SEL_BY_IMSI)) {
		val = by_selector + strlen(SEL_BY_IMSI);
		if (!osmo_imsi_str_valid(val))
			return -EINVAL;
		return db_subscr_get_by_imsi(dbc, val, subscr);
	}
	if (startswith(by_selector, SEL_BY_MSISDN)) {
		val = by_selector + strlen(SEL_BY_MSISDN);
		if (!osmo_msisdn_str_valid(val))
			return -EINVAL;
		return db_subscr_get_by_msisdn(dbc, val, subscr);
	}
	if (startswith(by_selector, SEL_BY_ID)) {
		int64_t id;
		char *endptr;
		val = by_selector + strlen(SEL_BY_ID);
		if (*val == '+')
			return -EINVAL;
		errno = 0;
		id = strtoll(val, &endptr, 10);
		if (errno || *endptr)
			return -EINVAL;
		return db_subscr_get_by_id(dbc, id, subscr);
	}
	return -ENOTSUP;
}

static bool get_subscriber(struct db_context *dbc,
			   const char *by_selector,
			   struct hlr_subscriber *subscr,
			   struct ctrl_cmd *cmd)
{
	int rc = _get_subscriber(dbc, by_selector, subscr);
	switch (rc) {
	case 0:
		return true;
	case -ENOTSUP:
		cmd->reply = "Not a known subscriber 'by-xxx-' selector.";
		return false;
	case -EINVAL:
		cmd->reply = "Invalid value part of 'by-xxx-value' selector.";
		return false;
	case -ENOENT:
		cmd->reply = "No such subscriber.";
		return false;
	default:
		cmd->reply = "An unknown error has occured during get_subscriber().";
		return false;
	}
}

/* Optimization: if a subscriber operation is requested by-imsi, just return
 * the IMSI right back. */
static const char *get_subscriber_imsi(struct db_context *dbc,
				       const char *by_selector,
				       struct ctrl_cmd *cmd)
{
	static struct hlr_subscriber subscr;

	if (startswith(by_selector, SEL_BY_IMSI))
		return by_selector + strlen(SEL_BY_IMSI);
	if (!get_subscriber(dbc, by_selector, &subscr, cmd))
		return NULL;
	return subscr.imsi;
}

/* printf fmt and arg to completely omit a string if it is empty. */
#define FMT_S "%s%s%s%s"
#define ARG_S(name, val) \
	(val) && *(val) ? "\n" : "", \
	(val) && *(val) ? name : "", \
	(val) && *(val) ? "\t" : "", \
	(val) && *(val) ? (val) : "" \

/* printf fmt and arg to completely omit bool of given value. */
#define FMT_BOOL "%s"
#define ARG_BOOL(name, val) \
	val ? "\n" name "\t1" : "\n" name "\t0"

static void print_subscr_info(struct ctrl_cmd *cmd,
			      struct hlr_subscriber *subscr)
{
	ctrl_cmd_reply_printf(cmd,
		"\nid\t%" PRIu64
		FMT_S
		FMT_S
		FMT_BOOL
		FMT_BOOL
		FMT_S
		FMT_S
		FMT_S
		FMT_BOOL
		FMT_BOOL
		"\nperiodic_lu_timer\t%u"
		"\nperiodic_rau_tau_timer\t%u"
		"\nlmsi\t%08x"
		,
		subscr->id,
		ARG_S("imsi", subscr->imsi),
		ARG_S("msisdn", subscr->msisdn),
		ARG_BOOL("nam_cs", subscr->nam_cs),
		ARG_BOOL("nam_ps", subscr->nam_ps),
		ARG_S("vlr_number", subscr->vlr_number),
		ARG_S("sgsn_number", subscr->sgsn_number),
		ARG_S("sgsn_address", subscr->sgsn_address),
		ARG_BOOL("ms_purged_cs", subscr->ms_purged_cs),
		ARG_BOOL("ms_purged_ps", subscr->ms_purged_ps),
		subscr->periodic_lu_timer,
		subscr->periodic_rau_tau_timer,
		subscr->lmsi
		);
}

static void print_subscr_info_aud2g(struct ctrl_cmd *cmd, struct osmo_sub_auth_data *aud)
{
	if (aud->algo == OSMO_AUTH_ALG_NONE)
		return;
	ctrl_cmd_reply_printf(cmd,
		"\naud2g.algo\t%s"
		"\naud2g.ki\t%s"
		,
		osmo_auth_alg_name(aud->algo),
		hexdump_buf(aud->u.gsm.ki));
}

static void print_subscr_info_aud3g(struct ctrl_cmd *cmd, struct osmo_sub_auth_data *aud)
{
	if (aud->algo == OSMO_AUTH_ALG_NONE)
		return;
	ctrl_cmd_reply_printf(cmd,
		"\naud3g.algo\t%s"
		"\naud3g.k\t%s"
		,
		osmo_auth_alg_name(aud->algo),
		hexdump_buf(aud->u.umts.k));
	/* hexdump uses a static string buffer, hence only one hexdump per
	 * printf(). */
	ctrl_cmd_reply_printf(cmd,
		"\naud3g.%s\t%s"
		"\naud3g.ind_bitlen\t%u"
		"\naud3g.sqn\t%" PRIu64
		,
		aud->u.umts.opc_is_op? "op" : "opc",
		hexdump_buf(aud->u.umts.opc),
		aud->u.umts.ind_bitlen,
		aud->u.umts.sqn);
}

CTRL_CMD_DEFINE_RO(subscr_info, "info");
static int get_subscr_info(struct ctrl_cmd *cmd, void *data)
{
	struct hlr_subscriber subscr;
	struct hlr *hlr = data;
	const char *by_selector = cmd->node;

	if (!get_subscriber(hlr->dbc, by_selector, &subscr, cmd))
		return CTRL_CMD_ERROR;

	print_subscr_info(cmd, &subscr);

	return CTRL_CMD_REPLY;
}

CTRL_CMD_DEFINE_RO(subscr_info_aud, "info-aud");
static int get_subscr_info_aud(struct ctrl_cmd *cmd, void *data)
{
	const char *imsi;
	struct osmo_sub_auth_data aud2g;
	struct osmo_sub_auth_data aud3g;
	struct hlr *hlr = data;
	const char *by_selector = cmd->node;
	int rc;

	imsi = get_subscriber_imsi(hlr->dbc, by_selector, cmd);
	if (!imsi)
		return CTRL_CMD_ERROR;

	rc = db_get_auth_data(hlr->dbc, imsi, &aud2g, &aud3g, NULL);

	switch (rc) {
	case 0:
		break;
	case -ENOENT:
	case -ENOKEY:
		/* No auth data found, tell the print*() functions about it. */
		aud2g.algo = OSMO_AUTH_ALG_NONE;
		aud3g.algo = OSMO_AUTH_ALG_NONE;
		break;
	default:
		cmd->reply = "Error retrieving authentication data.";
		return CTRL_CMD_ERROR;
	}

	print_subscr_info_aud2g(cmd, &aud2g);
	print_subscr_info_aud3g(cmd, &aud3g);

	return CTRL_CMD_REPLY;
}

CTRL_CMD_DEFINE_RO(subscr_info_all, "info-all");
static int get_subscr_info_all(struct ctrl_cmd *cmd, void *data)
{
	struct hlr_subscriber subscr;
	struct osmo_sub_auth_data aud2g;
	struct osmo_sub_auth_data aud3g;
	struct hlr *hlr = data;
	const char *by_selector = cmd->node;
	int rc;

	if (!get_subscriber(hlr->dbc, by_selector, &subscr, cmd))
		return CTRL_CMD_ERROR;

	rc = db_get_auth_data(hlr->dbc, subscr.imsi, &aud2g, &aud3g, NULL);

	switch (rc) {
	case 0:
		break;
	case -ENOENT:
	case -ENOKEY:
		/* No auth data found, tell the print*() functions about it. */
		aud2g.algo = OSMO_AUTH_ALG_NONE;
		aud3g.algo = OSMO_AUTH_ALG_NONE;
		break;
	default:
		cmd->reply = "Error retrieving authentication data.";
		return CTRL_CMD_ERROR;
	}

	print_subscr_info(cmd, &subscr);
	print_subscr_info_aud2g(cmd, &aud2g);
	print_subscr_info_aud3g(cmd, &aud3g);

	return CTRL_CMD_REPLY;
}

static int verify_subscr_cs_ps_enabled(struct ctrl_cmd *cmd, const char *value, void *data)
{
	if (!value || !*value
	    || (strcmp(value, "0") && strcmp(value, "1")))
		return 1;
	return 0;
}

static int get_subscr_cs_ps_enabled(struct ctrl_cmd *cmd, void *data,
				    bool is_ps)
{
	struct hlr_subscriber subscr;
	struct hlr *hlr = data;
	const char *by_selector = cmd->node;

	if (!get_subscriber(hlr->dbc, by_selector, &subscr, cmd))
		return CTRL_CMD_ERROR;

	cmd->reply = (is_ps ? subscr.nam_ps : subscr.nam_cs)
		     ? "1" : "0";
	return CTRL_CMD_REPLY;
}

static int set_subscr_cs_ps_enabled(struct ctrl_cmd *cmd, void *data,
				    bool is_ps)
{
	const char *imsi;
	struct hlr *hlr = data;
	const char *by_selector = cmd->node;

	imsi = get_subscriber_imsi(hlr->dbc, by_selector, cmd);
	if (!imsi)
		return CTRL_CMD_ERROR;
	if (db_subscr_nam(hlr->dbc, imsi, strcmp(cmd->value, "1") == 0, is_ps))
		return CTRL_CMD_ERROR;
	cmd->reply = "OK";
	return CTRL_CMD_REPLY;
}

CTRL_CMD_DEFINE(subscr_ps_enabled, "ps-enabled");
static int verify_subscr_ps_enabled(struct ctrl_cmd *cmd, const char *value, void *data)
{
	return verify_subscr_cs_ps_enabled(cmd, value, data);
}
static int get_subscr_ps_enabled(struct ctrl_cmd *cmd, void *data)
{
	return get_subscr_cs_ps_enabled(cmd, data, true);
}
static int set_subscr_ps_enabled(struct ctrl_cmd *cmd, void *data)
{
	return set_subscr_cs_ps_enabled(cmd, data, true);
}

CTRL_CMD_DEFINE(subscr_cs_enabled, "cs-enabled");
static int verify_subscr_cs_enabled(struct ctrl_cmd *cmd, const char *value, void *data)
{
	return verify_subscr_cs_ps_enabled(cmd, value, data);
}
static int get_subscr_cs_enabled(struct ctrl_cmd *cmd, void *data)
{
	return get_subscr_cs_ps_enabled(cmd, data, false);
}
static int set_subscr_cs_enabled(struct ctrl_cmd *cmd, void *data)
{
	return set_subscr_cs_ps_enabled(cmd, data, false);
}

int hlr_ctrl_cmds_install()
{
	int rc = 0;

	rc |= ctrl_cmd_install(CTRL_NODE_SUBSCR_BY, &cmd_subscr_info);
	rc |= ctrl_cmd_install(CTRL_NODE_SUBSCR_BY, &cmd_subscr_info_aud);
	rc |= ctrl_cmd_install(CTRL_NODE_SUBSCR_BY, &cmd_subscr_info_all);
	rc |= ctrl_cmd_install(CTRL_NODE_SUBSCR_BY, &cmd_subscr_ps_enabled);
	rc |= ctrl_cmd_install(CTRL_NODE_SUBSCR_BY, &cmd_subscr_cs_enabled);

	return rc;
}

static int hlr_ctrl_node_lookup(void *data, vector vline, int *node_type,
				void **node_data, int *i)
{
	const char *token = vector_slot(vline, *i);

	switch (*node_type) {
	case CTRL_NODE_ROOT:
		if (strcmp(token, "subscriber") != 0)
			return 0;
		*node_data = NULL;
		*node_type = CTRL_NODE_SUBSCR;
		break;
	case CTRL_NODE_SUBSCR:
		if (!startswith(token, "by-"))
			return 0;
		*node_data = (void*)token;
		*node_type = CTRL_NODE_SUBSCR_BY;
		break;
	default:
		return 0;
	}

	return 1;
}

struct ctrl_handle *hlr_controlif_setup(struct hlr *hlr)
{
	int rc;
	struct ctrl_handle *hdl = ctrl_interface_setup_dynip2(hlr,
							      hlr->ctrl_bind_addr,
							      OSMO_CTRL_PORT_HLR,
							      hlr_ctrl_node_lookup,
							      _LAST_CTRL_NODE_HLR);
	if (!hdl)
		return NULL;

	rc = hlr_ctrl_cmds_install();
	if (rc) /* FIXME: close control interface? */
		return NULL;

	return hdl;
}
