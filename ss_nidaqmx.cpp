//---------------------------------------------------------------------------
// ss_nidaqmx.cpp
// Angel Perles  2008
//---------------------------------------------------------------------------

#define SIMSENY_MODULE  //to avoid macro translation
#include "ss_nidaqmx.h"

#include "ss_daq_niusb6008.h"

#include <stdio.h>


#include <string.h>
#include <ctype.h>

// general code configuration
#define DEFAULT_IP_ADDR "127.0.0.1";
#define DEFAULT_IP_PORT "23456";

// nidaqmx configuration
#define MAX_TASKS       100     // max number of captured tasks
#define MAX_SIGNALS     16      // max number of signals per task


// ##########################################################################
// simseny II general code ##################################################
// ##########################################################################

// --------------------------------------------------------------------------
enum {CAPTURE_MODE_UNDEFINED, CAPTURE_MODE_BYPASS, CAPTURE_MODE_SIMULATE};
static int ss_capture_mode = CAPTURE_MODE_UNDEFINED;

//-- ss_initial_check(void) -------------------------------------------------
// This function is allways called at the start of any DAQ function in order
// to prepare te initial configuration and to decide the simulation mode
void ss_initial_check(void)
{
   char *p;
   char cad[100];
   //int ssport;

   // this will occur the first time this function is called
   if (ss_capture_mode == CAPTURE_MODE_UNDEFINED) {
       AnsiString NewString;
       NewString = DEFAULT_IP_ADDR;
       NewString = NewString + ":";
       NewString = NewString + DEFAULT_IP_PORT;

       if (InputQuery("SimSeny II: Connection port ...",
                      //"Press \"Cancel\" to use the data adquisition card.\nPress \"OK\" for connecting to SimSeny.\n\nIP address an port:",
                      //"Press \"Cancel\" if you want to use the DAQ.\nPress \"OK\" for connecting to SimSeny.\n\nIP address an port:",
                      "\"OK\" --> if you want to simulate signals.\n\"Cancel\" --> if you want to use the DAQ.\n\nSimSeny IP address and port:",
                      NewString)) {
           strcpy(cad,NewString.c_str());
           if ((p=strchr(cad,':'))==NULL)
                ss_error("Incorrect IP:PORT address",0);
           *(p) = '\0';
           p++;
           //ssport = atoi(p);
           //simseny9112_conectar(cad,ssport);
           ss_capture_mode = CAPTURE_MODE_SIMULATE;
       } else { // "Cancel" elected
          ss_capture_mode = CAPTURE_MODE_BYPASS;
       }
   }
}

//-- ss_error ---------------------------------------------------------------
// Shows an error and terminates the app
void ss_error(char *msg, int cod) {
   AnsiString cad;

   static int cont = 0;

   if (cont != 0) // evitar entradas concurrentes
      return;

   cont++;
   cad = msg;
   cad = cad + " [";
   cad = cad + cod;
   cad = cad + "]";

   ShowMessage(cad);
   cont--;
   exit(1);
}
// ##########################################################################
// nidaqmx support code       ###############################################
// ##########################################################################

// task information struct
typedef struct
{
        TaskHandle tsk_handle;
        int channel_configurated;   // 0= false (no channel configuration specified)
        int num_of_signals;         // number of signals
        Tss_signal* signal[MAX_SIGNALS];         // pointer to the signals
} Ttask_info;

Ttask_info task_info_list[MAX_TASKS];

int task_cnt;   // next taskhandle identifier

//---------------------------------------------------------------------------
void ss_nidaqmx_initial_check(void) {

   static int first_time = 1;
   int i;

   if (first_time) {
      first_time = 0;
      ss_initial_check();
      // check if in simulated mode
      if (ss_capture_mode == CAPTURE_MODE_SIMULATE) {
         for (i=0;i<MAX_TASKS;i++) {
            task_info_list[i].tsk_handle = 0;
         }
         task_cnt = 1;  // first taskhandle identifier
         // create the simulated daq
         ss_daq_niusb6008_init();
      }
   }
}

// -------------------------------------------------------------------------
// find a task identifier in the task info list
// and returns its index
// returns -1 if not found
int ss_nidaqmx_task_find(TaskHandle taskHandle) {

  int i;

  for (i=0;i<MAX_TASKS;i++) {
     if (task_info_list[i].tsk_handle == taskHandle) {
        return(i);
     }
  }

  // not found
  return(-1);
}

// -------------------------------------------------------------------------
// updates the asociated GUI for the given signal
void ss_nidaqmx_signals_update_gui(Tss_signal *psignal) {
   ss_daq_niusb6008_signals_update_gui(psignal);
}

// -------------------------------------------------------------------------
// find signals by name and update array of pointer to signals
int ss_nidaqmx_signals_find(Tss_signal *ps[], char *cad) {

   Tss_signal *psignal;
   char buff[100];
   int i;

   if (strcmp(cad,"PORT0")==0) {   // is port 0
      for (i=0;i<8;i++) {
         sprintf(buff,"P0.%d",i);
         *ps = ss_daq_niusb6008_signals_find(buff);
         ps++;
      }
      return(8);
   }

   if (strcmp(cad,"PORT1")==0) {   // is port 1
      for (i=0;i<4;i++) {
         sprintf(buff,"P1.%d",i);
         *ps = ss_daq_niusb6008_signals_find(buff);
         ps++;
      }
      return(4);
   }

   // rest of signals
   *ps = ss_daq_niusb6008_signals_find(cad);
   if (*ps == NULL) {
      return(0);
   } else {
      return(1);
   }
}


// ##########################################################################
// nidaqmx captured functions ###############################################
// ##########################################################################

//---------------------------------------------------------------------------
int32 ss_DAQmxCreateTask (const char taskName[], TaskHandle *taskHandle) {

   int i;

   ss_nidaqmx_initial_check();

   switch(ss_capture_mode) {
   case CAPTURE_MODE_BYPASS:
        return(DAQmxCreateTask (taskName,taskHandle));
        //break;
   case CAPTURE_MODE_SIMULATE:
        // find a free task list entry
        i = ss_nidaqmx_task_find(0);
        if (i<0) {
           ss_error("ss_DAQmxCreateTask(): task_info_list[] exhausted",0);
        }

        // save task information
        task_info_list[i].tsk_handle = task_cnt;
        task_info_list[i].channel_configurated = 0;   // false

        task_cnt++;  // increment tass id

        *taskHandle = task_info_list[i].tsk_handle;

        break;
   default:
        ss_error("ss_DAQmxCreateTask(): Unexpected value",0);
        break;
   }
   return(0);
}

//----------------------------------------------------------------------------
int32 ss_DAQmxCreateAIVoltageChan (TaskHandle taskHandle,
        const char physicalChannel[],
        const char nameToAssignToChannel[],
        int32 terminalConfig,
        float64 minVal,
        float64 maxVal,
        int32 units,
        const char customScaleName[])
{
   char *p;
   char buff[100];
   unsigned int i;
   int task_index;

   ss_nidaqmx_initial_check();

   switch(ss_capture_mode) {
   case CAPTURE_MODE_BYPASS:
        return(DAQmxCreateAIVoltageChan (taskHandle, physicalChannel, nameToAssignToChannel,
                terminalConfig, minVal, maxVal, units, customScaleName));
        //break;
   case CAPTURE_MODE_SIMULATE:
        // check allowed configurations
        if (units != DAQmx_Val_Volts) {
           ss_error("ss_DAQmxCreateAIVoltageChan(): only units==DAQmx_Val_Volts supported",0);
        }
        // find the task
        task_index = ss_nidaqmx_task_find(taskHandle);
        if (task_index < 0) {
           ss_error("ss_DAQmxCreateAIVoltageChan(): task does not exists",0);
        }

        // test that the task hasn't benn configurated yet
        if (task_info_list[task_index].channel_configurated) {
          ss_error("ss_DAQmxCreateAIVoltageChan(): the task is already configurated",0);
        }

        // extract physical channel
        p=(char *)strchr(physicalChannel,'/');
        if (p==NULL) {
            ss_error("Incorrect \"device/channel\" syntax",0);
        } else {
            p++;
            strcpy(buff,p);
            for (i=0;i<strlen(buff);i++) {
                buff[i] = toupper(buff[i]);
            }
        }

        // find if the asociated daq signal exits
        Tss_signal *psignal;
        psignal = ss_daq_niusb6008_signals_find(buff);
        if (psignal == NULL) { // not valid
            ss_error("ss_DAQmxCreateAIVoltageChan(): channel name incorrect",0);
        }

        // test if the signal is compatible
        if (psignal->signal_type_get() != SS_ST_ANALOG) {
            ss_error("ss_DAQmxCreateAIVoltageChan(): channel does not support this type of signal",0);
        }
        if (psignal->signal_dir_get() != SS_SD_INPUT) {
            ss_error("ss_DAQmxCreateAIVoltageChan(): channel is not an input",0);
        }

        // save the signal in the task info list
        task_info_list[task_index].signal[0] = psignal;

        // mark the task as configurated
        task_info_list[task_index].channel_configurated = 1;

        break;
   default:
        ss_error("ss_DAQmxCreateAIVoltageChan(): Unexpected value",0);
        break;
   }
   return(0);
}

//----------------------------------------------------------------------------
int32 ss_DAQmxCreateAOVoltageChan (TaskHandle taskHandle,
         const char physicalChannel[],
         const char nameToAssignToChannel[],
         float64 minVal,
         float64 maxVal,
         int32 units,
         const char customScaleName[])
{
   char *p;
   char buff[100];
   unsigned int i;
   int task_index;

   ss_nidaqmx_initial_check();

   switch(ss_capture_mode) {
   case CAPTURE_MODE_BYPASS:
        return(DAQmxCreateAOVoltageChan (taskHandle, physicalChannel, nameToAssignToChannel,
               minVal, maxVal, units, customScaleName));
        //break;
   case CAPTURE_MODE_SIMULATE:
        // check allowed configurations
        if (units != DAQmx_Val_Volts) {
           ss_error("ss_DAQmxCreateAOVoltageChan(): only units==DAQmx_Val_Volts supported",0);
        }
        // find the task
        task_index = ss_nidaqmx_task_find(taskHandle);
        if (task_index < 0) {
           ss_error("ss_DAQmxCreateAOVoltageChan(): task does not exists",0);
        }

        // test that the task hasn't benn configurated yet
        if (task_info_list[task_index].channel_configurated) {
          ss_error("ss_DAQmxCreateAOVoltageChan(): the task is already configurated",0);
        }

        // extract physical channel
        p=(char *)strchr(physicalChannel,'/');
        if (p==NULL) {
            ss_error("Incorrect \"device/channel\" syntax",0);
        } else {
            p++;
            strcpy(buff,p);
            for (i=0;i<strlen(buff);i++) {
                buff[i] = toupper(buff[i]);
            }
        }

        // find if the asociated daq signal exits
        Tss_signal *psignal;
        psignal = ss_daq_niusb6008_signals_find(buff);
        if (psignal == NULL) { // not valid
            ss_error("ss_DAQmxCreateAOVoltageChan(): channel name incorrect",0);
        }

        // test if the signal is compatible
        if (psignal->signal_type_get() != SS_ST_ANALOG) {
            ss_error("ss_DAQmxCreateAOVoltageChan(): channel does not support this type of signal",0);
        }
        if (psignal->signal_dir_get() != SS_SD_OUTPUT) {
            ss_error("ss_DAQmxCreateAOVoltageChan(): channel is not an outputput",0);
        }

        // save the signal in the task info list
        task_info_list[task_index].signal[0] = psignal;

        // mark the task as configurated
        task_info_list[task_index].channel_configurated = 1;

        break;
   default:
        ss_error("ss_DAQmxCreateAOVoltageChan(): Unexpected value",0);
        break;
   }
   return(0);
}

//----------------------------------------------------------------------------
int32 ss_DAQmxCreateDIChan (TaskHandle taskHandle,
         const char lines[],
         const char nameToAssignToLines[],
         int32 lineGrouping)
{
   char *p;
   char buff[100];
   unsigned int i;
   int task_index;
   Tss_signal *psignal;

   ss_nidaqmx_initial_check();

   switch(ss_capture_mode) {
   case CAPTURE_MODE_BYPASS:
        return(DAQmxCreateDIChan(taskHandle,lines,nameToAssignToLines,lineGrouping));
        //break;
   case CAPTURE_MODE_SIMULATE:
        // check allowed configurations
        if (lineGrouping != DAQmx_Val_ChanForAllLines) {
           ss_error("ss_DAQmxCreateDIChan(): only lineGrouping=DAQmx_Val_ChanForAllLines supported",0);
        }
        // find the task
        task_index = ss_nidaqmx_task_find(taskHandle);
        if (task_index < 0) {
           ss_error("ss_DAQmxCreateDIChan(): task does not exists",0);
        }

        // test that the task hasn't benn configurated yet
        if (task_info_list[task_index].channel_configurated) {
          ss_error("ss_DAQmxCreateDIChan(): the task is already configurated",0);
        }

        // extract physical channel
        p=(char *)strchr(lines,'/');
        if (p==NULL) {
            ss_error("Incorrect \"device/channel\" syntax",0);
        } else {
            p++;
            strcpy(buff,p);
            for (i=0;i<strlen(buff);i++) {
                buff[i] = toupper(buff[i]);
            }
        }

        // find and save the signals
        unsigned int num_signals;
        num_signals = ss_nidaqmx_signals_find(task_info_list[task_index].signal,buff);
        if (num_signals == 0) { // not valid signals
            ss_error("ss_DAQmxCreateDIChan(): channel name incorrect",0);
        }

        // test if the signals are compatible
        for (i=0;i<num_signals;i++) {
            psignal = task_info_list[task_index].signal[i];
            if (psignal->signal_type_get() != SS_ST_DIGITAL) {
                ss_error("ss_DAQmxCreateDIChan(): channel does not support this type of signal",0);
            }
        }

        // update num of signals
        task_info_list[task_index].num_of_signals = num_signals;

        // mark the task as configurated
        task_info_list[task_index].channel_configurated = 1;

        // configure direction in the daq
        for (i=0;i<num_signals;i++) {
            psignal = task_info_list[task_index].signal[i];
            psignal->signal_dir_set(SS_SD_INPUT);
        }

        // update gui
        for (i=0;i<num_signals;i++) {
            psignal = task_info_list[task_index].signal[i];
            ss_nidaqmx_signals_update_gui(psignal);
        }

        break;
   default:
        ss_error("ss_DAQmxCreateDIChan(): Unexpected value",0);
        break;
   }
   return(0);
}

//----------------------------------------------------------------------------
int32 ss_DAQmxGetErrorString (int32 errorCode, char errorString[], uInt32 bufferSize) {

   char *default_msg = "SimSeny actually does not support error string.";

   ss_nidaqmx_initial_check();

   switch(ss_capture_mode) {
   case CAPTURE_MODE_BYPASS:
        return(DAQmxGetErrorString (errorCode, errorString, bufferSize));
        //break;
   case CAPTURE_MODE_SIMULATE:
        // provisional mesage
        strncpy(errorString,default_msg,bufferSize-1);
        if (strlen(default_msg) >= bufferSize) {
            errorString[bufferSize-1] = '\0';
        }
        break;
   default:
        ss_error("ss_DAQmxGetErrorString(): Unexpected value",0);
        break;
   }
   return(0);
}

//----------------------------------------------------------------------------
int32 ss_DAQmxStartTask (TaskHandle taskHandle) {

   int i;

   ss_nidaqmx_initial_check();

   switch(ss_capture_mode) {
   case CAPTURE_MODE_BYPASS:
        return(DAQmxStartTask (taskHandle));
        //break;
   case CAPTURE_MODE_SIMULATE:
        i = ss_nidaqmx_task_find(taskHandle);
        if (i<0) {
           ss_error("ss_DAQmxStartTask(): task does not exists",0);
        }
        break;
   default:
        ss_error("ss_DAQmxStartTask(): Unexpected value",0);
        break;
   }
   return(0);
}

//----------------------------------------------------------------------------
int32 ss_DAQmxStopTask (TaskHandle taskHandle)  {

   int i;

   ss_nidaqmx_initial_check();

   switch(ss_capture_mode) {
   case CAPTURE_MODE_BYPASS:
        return(DAQmxStopTask (taskHandle));
        //break;
   case CAPTURE_MODE_SIMULATE:
        i = ss_nidaqmx_task_find(taskHandle);
        if (i<0) {
           ss_error("ss_DAQmxStopTask(): task does not exists",0);
        }
        break;
   default:
        ss_error("ss_DAQmxStopTask(): Unexpected value",0);
        break;
   }
   return(0);
}

//----------------------------------------------------------------------------
int32 ss_DAQmxReadAnalogScalarF64 (TaskHandle taskHandle, float64 timeout,
         float64 *value, bool32 *reserved) {

   int task_index;
   Tss_signal *psignal;

   ss_nidaqmx_initial_check();

   switch(ss_capture_mode) {
   case CAPTURE_MODE_BYPASS:
        return(DAQmxReadAnalogScalarF64(taskHandle, timeout, value, reserved));
        //break;
   case CAPTURE_MODE_SIMULATE:
        // find task
        task_index = ss_nidaqmx_task_find(taskHandle);
        if (task_index < 0) {
           ss_error("ss_DAQmxReadAnalogScalarF64(): task does not exists",0);
        }
        // tests before proceeding
        // test that the task has been configurated
        if (!task_info_list[task_index].channel_configurated) {
          ss_error("ss_DAQmxReadAnalogScalarF64(): task not configurated",0);
        }
        psignal = task_info_list[task_index].signal[0];
        // test if the signal is compatible
        if ((psignal->signal_type_get() != SS_ST_ANALOG) || (psignal->signal_dir_get() != SS_SD_INPUT)) {
            ss_error("ss_DAQmxReadAnalogScalarF64(): the signal is not compatible",0);
        }

        // return the value
        *value = psignal->value_analog_get();

        break;
   default:
        ss_error("ss_DAQmxReadAnalogScalarF64(): Unexpected value",0);
        break;
   }
   return(0);
}

//----------------------------------------------------------------------------
int32 ss_DAQmxWriteAnalogScalarF64 (TaskHandle taskHandle, bool32 autoStart,
         float64 timeout, float64 value, bool32 *reserved)
{

   int task_index;
   Tss_signal *psignal;

   ss_nidaqmx_initial_check();

   switch(ss_capture_mode) {
   case CAPTURE_MODE_BYPASS:
        return(DAQmxWriteAnalogScalarF64(taskHandle, autoStart, timeout, value, reserved));
        //break;
   case CAPTURE_MODE_SIMULATE:
        // find task
        task_index = ss_nidaqmx_task_find(taskHandle);
        if (task_index < 0) {
           ss_error("ss_DAQmxWriteAnalogScalarF64(): task does not exists",0);
        }
        // tests before proceeding
        // test that the task has been configurated
        if (!task_info_list[task_index].channel_configurated) {
          ss_error("ss_DAQmxwriteAnalogScalarF64(): task not configurated",0);
        }
        psignal = task_info_list[task_index].signal[0];
        // test if the signal is compatible
        if ((psignal->signal_type_get() != SS_ST_ANALOG) || (psignal->signal_dir_get() != SS_SD_OUTPUT)) {
            ss_error("ss_DAQmxWriteAnalogScalarF64(): the signal is not compatible",0);
        }

        // set the value
        psignal->value_analog_set(value);
        ss_nidaqmx_signals_update_gui(psignal);

        break;
   default:
        ss_error("ss_DAQmxWriteAnalogScalarF64(): Unexpected value",0);
        break;
   }
   return(0);

}

//----------------------------------------------------------------------------
int32 ss_DAQmxReadDigitalScalarU32 (TaskHandle taskHandle,
         float64 timeout,
         uInt32 *value,
         bool32 *reserved)
{
   int task_index;
   Tss_signal *psignal;
   int i;

   ss_nidaqmx_initial_check();

   switch(ss_capture_mode) {
   case CAPTURE_MODE_BYPASS:
        return(DAQmxReadDigitalScalarU32(taskHandle,timeout,value,reserved));
        //break;
   case CAPTURE_MODE_SIMULATE:
        // find task
        task_index = ss_nidaqmx_task_find(taskHandle);
        if (task_index < 0) {
           ss_error("ss_DAQmxReadDigitalScalarU32(): task does not exists",0);
        }
        // tests before proceeding
        // test that the task has been configurated
        if (!task_info_list[task_index].channel_configurated) {
          ss_error("ss_DAQmxReadDigitalScalarU32(): task not configurated",0);
        }

        // test if the signal is compatible
        psignal = task_info_list[task_index].signal[0];
        if ((psignal->signal_type_get() != SS_ST_DIGITAL) || (psignal->signal_dir_get() != SS_SD_INPUT)) {
            ss_error("ss_DAQmxReadDigitalScalarU32(): the signal is not compatible",0);
        }

        // get the value to return
        uInt32 dat;
        dat = 0;
        for (i=0;i<task_info_list[task_index].num_of_signals;i++) {
            psignal = task_info_list[task_index].signal[i];
            if (psignal->value_digital_get() == SS_VALUE_DIGITAL_HIGH) {
               dat = dat | (1 << i);
            }
        }

        // return the value
        *value = dat;

        break;
   default:
        ss_error("ss_DAQmxReadDigitalScalarU32(): Unexpected value",0);
        break;
   }
   return(0);
}

//----------------------------------------------------------------------------
int32 ss_DAQmxCreateDOChan (TaskHandle taskHandle,
         const char lines[],
         const char nameToAssignToLines[],
         int32 lineGrouping)
{
   char *p;
   char buff[100];
   unsigned int i;
   int task_index;
   Tss_signal *psignal;

   ss_nidaqmx_initial_check();

   switch(ss_capture_mode) {
   case CAPTURE_MODE_BYPASS:
        return(DAQmxCreateDOChan(taskHandle,lines,nameToAssignToLines,lineGrouping));
        //break;
   case CAPTURE_MODE_SIMULATE:
        // check allowed configurations
        if (lineGrouping != DAQmx_Val_ChanForAllLines) {
           ss_error("ss_DAQmxCreateDOChan(): only lineGrouping=DAQmx_Val_ChanForAllLines supported",0);
        }
        // find the task
        task_index = ss_nidaqmx_task_find(taskHandle);
        if (task_index < 0) {
           ss_error("ss_DAQmxCreateDOChan(): task does not exists",0);
        }

        // test that the task hasn't benn configurated yet
        if (task_info_list[task_index].channel_configurated) {
          ss_error("ss_DAQmxCreateDOChan(): the task is already configurated",0);
        }

        // extract physical channel
        p=(char *)strchr(lines,'/');
        if (p==NULL) {
            ss_error("Incorrect \"device/channel\" syntax",0);
        } else {
            p++;
            strcpy(buff,p);
            for (i=0;i<strlen(buff);i++) {
                buff[i] = toupper(buff[i]);
            }
        }

        // find and save the signals
        unsigned int num_signals;
        num_signals = ss_nidaqmx_signals_find(task_info_list[task_index].signal,buff);
        if (num_signals == 0) { // not valid signals
            ss_error("ss_DAQmxCreateDOChan(): channel name incorrect",0);
        }

        // test if the signals are compatible
        for (i=0;i<num_signals;i++) {
            psignal = task_info_list[task_index].signal[i];
            if (psignal->signal_type_get() != SS_ST_DIGITAL) {
                ss_error("ss_DAQmxCreateDOChan(): channel does not support this type of signal",0);
            }
        }

        // update num of signals
        task_info_list[task_index].num_of_signals = num_signals;

        // mark the task as configurated
        task_info_list[task_index].channel_configurated = 1;

        // configure direction in the daq
        for (i=0;i<num_signals;i++) {
            psignal = task_info_list[task_index].signal[i];
            psignal->signal_dir_set(SS_SD_OUTPUT);
        }

        // update gui
        for (i=0;i<num_signals;i++) {
            psignal = task_info_list[task_index].signal[i];
            ss_nidaqmx_signals_update_gui(psignal);
        }

        break;
   default:
        ss_error("ss_DAQmxCreateDOChan(): Unexpected value",0);
        break;
   }
   return(0);

}

//----------------------------------------------------------------------------
int32 ss_DAQmxWriteDigitalScalarU32 (TaskHandle taskHandle,
         bool32 autoStart,
         float64 timeout,
         uInt32 value,
         bool32 *reserved)
{
   int task_index;
   Tss_signal *psignal;
   int i;

   ss_nidaqmx_initial_check();

   switch(ss_capture_mode) {
   case CAPTURE_MODE_BYPASS:
        return(DAQmxWriteDigitalScalarU32 (taskHandle,autoStart,timeout,value,reserved));
        //break;
   case CAPTURE_MODE_SIMULATE:
        // find task
        task_index = ss_nidaqmx_task_find(taskHandle);
        if (task_index < 0) {
           ss_error("ss_DAQmxWriteDigitalScalarU32(): task does not exists",0);
        }
        // tests before proceeding
        // test that the task has been configurated
        if (!task_info_list[task_index].channel_configurated) {
          ss_error("ss_DAQmxWriteDigitalScalarU32(): task not configurated",0);
        }

        // test if the signal is compatible
        psignal = task_info_list[task_index].signal[0];
        if ((psignal->signal_type_get() != SS_ST_DIGITAL) || (psignal->signal_dir_get() != SS_SD_OUTPUT)) {
            ss_error("ss_DAQmxWriteDigitalScalarU32(): the signal is not compatible",0);
        }

        // set the value
        for (i=0;i<task_info_list[task_index].num_of_signals;i++) {
            psignal = task_info_list[task_index].signal[i];
            if (value & (1 << i)) {
               psignal->value_digital_set(SS_VALUE_DIGITAL_HIGH);
            } else {
               psignal->value_digital_set(SS_VALUE_DIGITAL_LOW);
            }
        }

        // update gui
        for (i=0;i<task_info_list[task_index].num_of_signals;i++) {
            psignal = task_info_list[task_index].signal[i];
            ss_nidaqmx_signals_update_gui(psignal);
        }

        break;
   default:
        ss_error("ss_DAQmxReadDigitalScalarU32(): Unexpected value",0);
        break;
   }
   return(0);

}

