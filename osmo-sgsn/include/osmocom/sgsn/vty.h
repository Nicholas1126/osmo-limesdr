#pragma once

#include <osmocom/vty/command.h>

enum bsc_vty_node {
	GBPROXY_NODE = _LAST_OSMOVTY_NODE + 1,
	SGSN_NODE,
	GTPHUB_NODE,
};
