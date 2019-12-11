//---------------------------------------------------------------------------

#ifndef ss_daqH
#define ss_daqH


#include "ss_signal.h"

//----------------------------------------------------------------------------
class Tss_daq {

public:
        // constructor, ------------------------------------------------------
        // num_signals -> maximum number of signals that can be created later
        Tss_daq(int num_signals);

        // get the number of available signals--------------------------------
        int signals_get_numof(void);

        // find a signal based of his logic name -----------------------------
        Tss_signal *signals_find(char *logic_name);

        // pointer to the set of signals -------------------------------------
        Tss_signal **p_signal;

private:
        int number_of_signals;


};




//---------------------------------------------------------------------------
#endif
