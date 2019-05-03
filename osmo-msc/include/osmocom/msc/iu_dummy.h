/* Trivial switch-off of external Iu dependencies,
 * allowing to run full unit tests even when built without Iu support. */

/*
 * (C) 2016,2017 by sysmocom s.f.m.c. GmbH <info@sysmocom.de>
 *
 * Author: Neels Hofmeyr <nhofmeyr@sysmocom.de>
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
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdint.h>
#include <stdbool.h>

#include <osmocom/core/linuxlist.h>

struct msgb;
struct RANAP_Cause;
struct osmo_auth_vector;

struct ranap_ue_conn_ctx {
	struct llist_head list;
	uint32_t conn_id;
};

int ranap_iu_tx(struct msgb *msg, uint8_t sapi);
int ranap_iu_tx_sec_mode_cmd(struct ranap_ue_conn_ctx *uectx, struct osmo_auth_vector *vec,
			     int send_ck);
int ranap_iu_page_cs(const char *imsi, const uint32_t *tmsi, uint16_t lac);
int ranap_iu_page_ps(const char *imsi, const uint32_t *ptmsi, uint16_t lac, uint8_t rac);
struct msgb *ranap_new_msg_rab_assign_voice(uint8_t rab_id, uint32_t rtp_ip,
					    uint16_t rtp_port,
					    bool use_x213_nsap);
int ranap_iu_rab_act(struct ranap_ue_conn_ctx *ue_ctx, struct msgb *msg);
int ranap_iu_tx_common_id(struct ranap_ue_conn_ctx *uectx, const char *imsi);
int ranap_iu_tx_release(struct ranap_ue_conn_ctx *ctx, const struct RANAP_Cause *cause);
