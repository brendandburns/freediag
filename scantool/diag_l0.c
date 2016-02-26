/* freediag
 * (c) fenugrec 2015
 * GPLv3
 */

/* common L0 code */

#include <stdlib.h>
#include <string.h>

#include "diag.h"
#include "diag_err.h"
#include "diag_l0.h"

int diag_l0_debug;	//debug flags for l0

int diag_l0_open(struct diag_l0_device *dl0d, int l1proto) {
	return dl0d->dl0->_open(dl0d, l1proto);
}

void diag_l0_close(struct diag_l0_device *dl0d) {
	dl0d->dl0->_close(dl0d);
}

struct diag_l0_device *diag_l0_new(const char *shortname) {
	struct diag_l0_device *dl0d;
	int rv;
	const struct diag_l0 *l0dev;
	int i;

	for (i=0; l0dev_list[i]; i++) {
		l0dev = l0dev_list[i];
		if (strcmp(shortname, l0dev->shortname) == 0) {
			/* Found it */
			break;
		}
	}

	if (!l0dev_list[i]) {
		//not found !
		return NULL;
	}

	rv=diag_calloc(&dl0d, 1);
	if (rv)
		return diag_pseterr(DIAG_ERR_NOMEM);

	dl0d->dl0 = l0dev;
	if (l0dev->_new(dl0d)) {
		free(dl0d);
		return diag_pseterr(DIAG_ERR_GENERAL);
	}

	return dl0d;
}

/* Delete a diag_l0_device; XXX forces close ?
 *
 * Opposite of diag_l0_new()
 */
void diag_l0_del(struct diag_l0_device *dl0d) {
	if (!dl0d) return;

	dl0d->dl0->_del(dl0d);

	free(dl0d);
	return;
}

struct cfgi* diag_l0_getcfg(struct diag_l0_device *dl0d) {
	if (!dl0d) return NULL;

	if (dl0d->dl0->_getcfg) return dl0d->dl0->_getcfg(dl0d);
	return NULL;
}

