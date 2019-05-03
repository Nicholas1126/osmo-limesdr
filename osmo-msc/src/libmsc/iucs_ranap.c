/* Implementation of RANAP messages to/from an MSC via an Iu-CS interface.
 * This keeps direct RANAP dependencies out of libmsc. */

/* (C) 2016 by sysmocom s.m.f.c. GmbH <info@sysmocom.de>
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

#include "../../bscconfig.h"

#ifdef BUILD_IU

#include <osmocom/core/logging.h>

#include <osmocom/ranap/ranap_ies_defs.h>
#include <osmocom/ranap/iu_client.h>
#include <osmocom/ranap/RANAP_IuTransportAssociation.h>
#include <osmocom/ranap/iu_helpers.h>

#include <osmocom/msc/debug.h>
#include <osmocom/msc/gsm_data.h>
#include <osmocom/msc/gsm_subscriber.h>
#include <osmocom/msc/iucs.h>
#include <osmocom/msc/vlr.h>
#include <osmocom/msc/iucs_ranap.h>
#include <osmocom/msc/msc_mgcp.h>

#include <asn1c/asn1helpers.h>

/* To continue authorization after a Security Mode Complete */
int gsm0408_authorize(struct ran_conn *conn);

static int iucs_rx_rab_assign(struct ran_conn *conn, RANAP_RAB_SetupOrModifiedItemIEs_t * setup_ies)
{
	uint8_t rab_id;
	RANAP_RAB_SetupOrModifiedItem_t *item = &setup_ies->raB_SetupOrModifiedItem;
	RANAP_TransportLayerAddress_t *transp_layer_addr;
	RANAP_IuTransportAssociation_t *transp_assoc;
	uint16_t port = 0;
	int rc;
	char addr[INET_ADDRSTRLEN];

	rab_id = item->rAB_ID.buf[0];

	LOGP(DIUCS, LOGL_NOTICE,
	     "Received RAB assignment event for %s rab_id=%hhd\n", vlr_subscr_name(conn->vsub), rab_id);

	if (item->iuTransportAssociation && item->transportLayerAddress) {
		transp_layer_addr = item->transportLayerAddress;
		transp_assoc = item->iuTransportAssociation;

		rc = ranap_transp_assoc_decode(&port, transp_assoc);
		if (rc != 0) {
			LOGP(DIUCS, LOGL_ERROR,
			     "Unable to decode RTP port in RAB assignment (%s rab_id=%hhd)\n",
			     vlr_subscr_name(conn->vsub), rab_id);
			return 0;
		}

		rc = ranap_transp_layer_addr_decode(addr, sizeof(addr), transp_layer_addr);
		if (rc != 0) {
			LOGP(DIUCS, LOGL_ERROR,
			     "Unable to decode IP-Address in RAB assignment (%s rab_id=%hhd)\n",
			     vlr_subscr_name(conn->vsub), rab_id);
			return 0;
		}

		return msc_mgcp_ass_complete(conn, port, addr);
	}

	LOGP(DIUCS, LOGL_ERROR,
	     "RAB assignment lacks RTP connection information. (%s rab_id=%hhd)\n",
	     vlr_subscr_name(conn->vsub), rab_id);
	return 0;
}

int iucs_rx_sec_mode_compl(struct ran_conn *conn,
			   RANAP_SecurityModeCompleteIEs_t *ies)
{
	OSMO_ASSERT(conn->via_ran == OSMO_RAT_UTRAN_IU);

	/* TODO evalute ies */

	ran_conn_rx_sec_mode_compl(conn);
	return 0;
}

int iucs_rx_ranap_event(struct gsm_network *network,
			struct ranap_ue_conn_ctx *ue_ctx, int type, void *data)
{
	struct ran_conn *conn;

	conn = ran_conn_lookup_iu(network, ue_ctx);

	if (!conn) {
		LOGP(DRANAP, LOGL_ERROR, "Cannot find subscriber for IU event %u\n", type);
		return -1;
	}

	switch (type) {
	case RANAP_IU_EVENT_IU_RELEASE:
	case RANAP_IU_EVENT_LINK_INVALIDATED:
		LOGP(DIUCS, LOGL_INFO, "IuCS release for %s\n",
		     vlr_subscr_name(conn->vsub));
		ran_conn_rx_iu_release_complete(conn);
		return 0;

	case RANAP_IU_EVENT_SECURITY_MODE_COMPLETE:
		LOGP(DIUCS, LOGL_INFO, "IuCS security mode complete for %s\n",
		     vlr_subscr_name(conn->vsub));
		return iucs_rx_sec_mode_compl(conn,
					      (RANAP_SecurityModeCompleteIEs_t*)data);
	case RANAP_IU_EVENT_RAB_ASSIGN:
		return iucs_rx_rab_assign(conn,
				(RANAP_RAB_SetupOrModifiedItemIEs_t*)data);
	default:
		LOGP(DIUCS, LOGL_NOTICE, "Unknown message received:"
		     " RANAP event: %i\n", type);
		return -1;
	}
}

#endif /* BUILD_IU */
