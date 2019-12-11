//---------------------------------------------------------------------------

#ifndef ss_daq_niusb6008H
#define ss_daq_niusb6008H

#include "ss_daq.h"

void ss_daq_niusb6008_init(void);
void ss_daq_niusb6008_paint(void);
Tss_signal *ss_daq_niusb6008_signals_find(char *logic_name);
void ss_daq_niusb6008_signals_update_gui(Tss_signal *psignal);

//---------------------------------------------------------------------------
#endif
