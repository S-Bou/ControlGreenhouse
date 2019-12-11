//ss_daq.cpp

#include "ss_daq.h"

#include <malloc.h>


//----------------------------------------------------------------------------
Tss_daq::Tss_daq(int num_signals) {

        if (num_signals > 0) {
                number_of_signals = num_signals;
                p_signal = new Tss_signal*[number_of_signals];
        } else {
                throw "Fallo en Tss_daq()";
        }

        for (int i=0;i<number_of_signals;i++) {
                p_signal[i] = NULL;
        }
}

// ---------------------------------------------------------------------------
// get the number of available signals
int Tss_daq::signals_get_numof(void) {
   return(number_of_signals);
}

// ---------------------------------------------------------------------------
// find a signal based of his logic name
// return  NULL if not found
Tss_signal *Tss_daq::signals_find(char *logic_name) {

   int i;
   Tss_signal *p;

   for (i=0;i<number_of_signals;i++) {
      if (p_signal[i]->logic_name_equal(logic_name)) {
         return(p_signal[i]);
      }
   }
   return(NULL);
}


