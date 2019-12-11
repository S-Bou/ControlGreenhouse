//---------------------------------------------------------------------------
// ss_daq_ni_usb6008.cpp
// Angel Perles. 2008

#include "ss_daq_niusb6008.h"
#include "ss_visual_niusb6008.h"

//#include "ss_daq.h"

static Tss_daq *daq;

//---------------------------------------------------------------------------
// create a NI USB-6008 daq
void ss_daq_niusb6008_init(void) {

   int i;

   // create the daq
   daq = new Tss_daq(22);

   // create and configure the signals
   i=0;

   //analog inputs
   daq->p_signal[i] = new Tss_signal("AI0",SS_ST_ANALOG,SS_SD_INPUT);
   daq->p_signal[i]->units("V.");
   daq->p_signal[i]->value_analog_set(0.0);

   i++;
   daq->p_signal[i] = new Tss_signal("AI1",SS_ST_ANALOG,SS_SD_INPUT);
   daq->p_signal[i]->units("V.");
   daq->p_signal[i]->value_analog_set(0.0);

   i++;
   daq->p_signal[i] = new Tss_signal("AI2",SS_ST_ANALOG,SS_SD_INPUT);
   daq->p_signal[i]->units("V.");
   daq->p_signal[i]->value_analog_set(0.0);

   i++;
   daq->p_signal[i] = new Tss_signal("AI3",SS_ST_ANALOG,SS_SD_INPUT);
   daq->p_signal[i]->units("V.");
   daq->p_signal[i]->value_analog_set(0.0);

   i++;
   daq->p_signal[i] = new Tss_signal("AI4",SS_ST_ANALOG,SS_SD_INPUT);
   daq->p_signal[i]->units("V.");
   daq->p_signal[i]->value_analog_set(0.0);

   i++;
   daq->p_signal[i] = new Tss_signal("AI5",SS_ST_ANALOG,SS_SD_INPUT);
   daq->p_signal[i]->units("V.");
   daq->p_signal[i]->value_analog_set(0.0);

   i++;
   daq->p_signal[i] = new Tss_signal("AI6",SS_ST_ANALOG,SS_SD_INPUT);
   daq->p_signal[i]->units("V.");
   daq->p_signal[i]->value_analog_set(0.0);

   i++;
   daq->p_signal[i] = new Tss_signal("AI7",SS_ST_ANALOG,SS_SD_INPUT);
   daq->p_signal[i]->units("V.");
   daq->p_signal[i]->value_analog_set(0.0);

   // analog outputs
   i++;
   daq->p_signal[i] = new Tss_signal("AO0",SS_ST_ANALOG,SS_SD_OUTPUT);
   daq->p_signal[i]->units("V.");
   daq->p_signal[i]->value_analog_set(0.0);

   i++;
   daq->p_signal[i] = new Tss_signal("AO1",SS_ST_ANALOG,SS_SD_OUTPUT);
   daq->p_signal[i]->units("V.");
   daq->p_signal[i]->value_analog_set(0.0);

   //digital i/o
   i++;
   daq->p_signal[i] = new Tss_signal("P0.0",SS_ST_DIGITAL,SS_SD_UNDEFINED);
   daq->p_signal[i]->value_digital_set(SS_VALUE_DIGITAL_HIGH);

   i++;
   daq->p_signal[i] = new Tss_signal("P0.1",SS_ST_DIGITAL,SS_SD_UNDEFINED);
   daq->p_signal[i]->value_digital_set(SS_VALUE_DIGITAL_HIGH);

   i++;
   daq->p_signal[i] = new Tss_signal("P0.2",SS_ST_DIGITAL,SS_SD_UNDEFINED);
   daq->p_signal[i]->value_digital_set(SS_VALUE_DIGITAL_HIGH);

   i++;
   daq->p_signal[i] = new Tss_signal("P0.3",SS_ST_DIGITAL,SS_SD_UNDEFINED);
   daq->p_signal[i]->value_digital_set(SS_VALUE_DIGITAL_HIGH);

   i++;
   daq->p_signal[i] = new Tss_signal("P0.4",SS_ST_DIGITAL,SS_SD_UNDEFINED);
   daq->p_signal[i]->value_digital_set(SS_VALUE_DIGITAL_HIGH);

   i++;
   daq->p_signal[i] = new Tss_signal("P0.5",SS_ST_DIGITAL,SS_SD_UNDEFINED);
   daq->p_signal[i]->value_digital_set(SS_VALUE_DIGITAL_HIGH);

   i++;
   daq->p_signal[i] = new Tss_signal("P0.6",SS_ST_DIGITAL,SS_SD_UNDEFINED);
   daq->p_signal[i]->value_digital_set(SS_VALUE_DIGITAL_HIGH);

   i++;
   daq->p_signal[i] = new Tss_signal("P0.7",SS_ST_DIGITAL,SS_SD_UNDEFINED);
   daq->p_signal[i]->value_digital_set(SS_VALUE_DIGITAL_HIGH);

   i++;
   daq->p_signal[i] = new Tss_signal("P1.0",SS_ST_DIGITAL,SS_SD_UNDEFINED);
   daq->p_signal[i]->value_digital_set(SS_VALUE_DIGITAL_HIGH);

   i++;
   daq->p_signal[i] = new Tss_signal("P1.1",SS_ST_DIGITAL,SS_SD_UNDEFINED);
   daq->p_signal[i]->value_digital_set(SS_VALUE_DIGITAL_HIGH);

   i++;
   daq->p_signal[i] = new Tss_signal("P1.2",SS_ST_DIGITAL,SS_SD_UNDEFINED);
   daq->p_signal[i]->value_digital_set(SS_VALUE_DIGITAL_HIGH);

   i++;
   daq->p_signal[i] = new Tss_signal("P1.3",SS_ST_DIGITAL,SS_SD_UNDEFINED);
   daq->p_signal[i]->value_digital_set(SS_VALUE_DIGITAL_HIGH);

   // paint the asociated visual control
   ss_daq_niusb6008_paint();
}

//----------------------------------------------------------------------------
// repaint information about the daq
void ss_daq_niusb6008_paint(void) {

   int nofs, i;
   char buff1[100], buff2[100];

   //
   F_niusb6008->SetDaq(daq);
   F_niusb6008->Show();

   // obtener el número de señales
   /*
   nofs = daq->signals_get_numof();
   for (i=0;i<nofs;i++) {
      daq->p_signal[i]->logic_name_get(buff1);
      daq->p_signal[i]->str_state(buff2);
      F_ni_usb6008->ValList->Values[buff1]=buff2;
   }
   */
}

//----------------------------------------------------------------------------
// find a signal in this daq
// returns a pointer to the signal or NULL if not found
Tss_signal *ss_daq_niusb6008_signals_find(char *logic_name) {

   return(daq->signals_find(logic_name));
}


//----------------------------------------------------------------------------
// request update asociated graphical information for the given signal
void ss_daq_niusb6008_signals_update_gui(Tss_signal *psignal) {

   F_niusb6008->SignalUpdate(psignal);
}
//---------------------------------------------------------------------------


