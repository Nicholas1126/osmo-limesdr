#ifndef _OSMO_PCAP_TEST_H_
#define _OSMO_PCAP_TEST_H_

#include <pcap.h>
#include <osmocom/core/timer.h>
#include <time.h>

struct msgb;

void osmo_pcap_init(void);

struct osmo_pcap {
	pcap_t			*h;
	struct osmo_timer_list	timer;
	struct timespec		start_sys;
	struct timeval		start_pcap;
	struct msgb 		*deliver_msg;
};

pcap_t *osmo_pcap_test_open(const char *pcapfile);
void osmo_pcap_test_close(pcap_t *handle);

int osmo_pcap_test_run(struct osmo_pcap *p, uint8_t pnum, int (*cb)(struct msgb *msgb));

void osmo_pcap_stats_printf(void);

#endif
