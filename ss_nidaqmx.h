//---------------------------------------------------------------------------
// simseny_nidaqmx.h
//---------------------------------------------------------------------------
#ifndef ss_nidaqmxH
#define ss_nidaqmxH
//---------------------------------------------------------------------------

#include "nidaqmx.h"
#include "vcl.h"

void ss_initial_check(void);
void ss_error(char *msg, int cod);


// defines false functions for the nidaqmx to avoid to installa nidaqmx libraries
#ifdef SS_FALSE_DAQ

int32 __CFUNC DAQmxCreateTask (const char taskName[], TaskHandle *taskHandle)
   {return(0);};

int32 __CFUNC DAQmxCreateAIVoltageChan (TaskHandle taskHandle,
        const char physicalChannel[],
        const char nameToAssignToChannel[],
        int32 terminalConfig,
        float64 minVal,
        float64 maxVal,
        int32 units,
        const char customScaleName[])
   {return(0);};

int32 __CFUNC DAQmxGetErrorString (int32 errorCode, char errorString[], uInt32 bufferSize)
   {return(0);};

int32 __CFUNC DAQmxStartTask (TaskHandle taskHandle)
   {return(0);};

int32 __CFUNC DAQmxStopTask (TaskHandle taskHandle)
   {return(0);};

int32 __CFUNC DAQmxReadAnalogScalarF64 (TaskHandle taskHandle, float64 timeout,
         float64 *value, bool32 *reserved)
   {return(0);};

int32 __CFUNC DAQmxCreateAOVoltageChan (TaskHandle taskHandle,
         const char physicalChannel[],
         const char nameToAssignToChannel[],
         float64 minVal,
         float64 maxVal,
         int32 units,
         const char customScaleName[])
   {return(0);};

int32 __CFUNC DAQmxWriteAnalogScalarF64 (TaskHandle taskHandle, bool32 autoStart,
         float64 timeout, float64 value, bool32 *reserved)
   {return(0);};

int32 __CFUNC DAQmxCreateDIChan (TaskHandle taskHandle,
         const char lines[],
         const char nameToAssignToLines[],
         int32 lineGrouping)
   {return(0);};

int32 __CFUNC DAQmxReadDigitalScalarU32 (TaskHandle taskHandle,
         float64 timeout,
         uInt32 *value,
         bool32 *reserved)
   {return(0);};

int32 __CFUNC DAQmxCreateDOChan (TaskHandle taskHandle,
         const char lines[],
         const char nameToAssignToLines[],
         int32 lineGrouping)
   {return(0);};

int32 __CFUNC DAQmxWriteDigitalScalarU32 (TaskHandle taskHandle,
         bool32 autoStart,
         float64 timeout,
         uInt32 value,
         bool32 *reserved)
   {return(0);};

#endif

// nidaqmx captured functions -----------------------------------------------




int32 ss_DAQmxCreateTask (const char taskName[], TaskHandle *taskHandle);

int32 ss_DAQmxCreateAIVoltageChan (TaskHandle taskHandle,
        const char physicalChannel[],
        const char nameToAssignToChannel[],
        int32 terminalConfig,
        float64 minVal,
        float64 maxVal,
        int32 units,
        const char customScaleName[]);

int32 ss_DAQmxGetErrorString (int32 errorCode, char errorString[], uInt32 bufferSize);

int32 ss_DAQmxStartTask (TaskHandle taskHandle);
int32 ss_DAQmxStopTask (TaskHandle taskHandle);

int32 ss_DAQmxReadAnalogScalarF64 (TaskHandle taskHandle, float64 timeout,
         float64 *value, bool32 *reserved);

int32 ss_DAQmxCreateAOVoltageChan (TaskHandle taskHandle,
         const char physicalChannel[],
         const char nameToAssignToChannel[],
         float64 minVal,
         float64 maxVal,
         int32 units,
         const char customScaleName[]);

int32 ss_DAQmxWriteAnalogScalarF64 (TaskHandle taskHandle, bool32 autoStart,
         float64 timeout, float64 value, bool32 *reserved);

int32 ss_DAQmxCreateDIChan (TaskHandle taskHandle,
         const char lines[],
         const char nameToAssignToLines[],
         int32 lineGrouping);

int32 ss_DAQmxReadDigitalScalarU32 (TaskHandle taskHandle,
         float64 timeout,
         uInt32 *value,
         bool32 *reserved);

int32 ss_DAQmxCreateDOChan (TaskHandle taskHandle,
         const char lines[],
         const char nameToAssignToLines[],
         int32 lineGrouping);

int32 ss_DAQmxWriteDigitalScalarU32 (TaskHandle taskHandle,
         bool32 autoStart,
         float64 timeout,
         uInt32 value,
         bool32 *reserved);

#ifndef SIMSENY_MODULE
   #define DAQmxCreateTask(A,B) ss_DAQmxCreateTask(A,B)
   #define DAQmxCreateAIVoltageChan(A,B,C,D,E,F,G,H) ss_DAQmxCreateAIVoltageChan(A,B,C,D,E,F,G,H)
   #define DAQmxGetErrorString(A,B,C) ss_DAQmxGetErrorString(A,B,C)
   #define DAQmxStartTask(A) ss_DAQmxStartTask(A)
   #define DAQmxStopTask(A) ss_DAQmxStopTask(A)
   #define DAQmxReadAnalogScalarF64(A,B,C,D) ss_DAQmxReadAnalogScalarF64(A,B,C,D)
   #define DAQmxCreateAOVoltageChan(A,B,C,D,E,F,G) ss_DAQmxCreateAOVoltageChan(A,B,C,D,E,F,G)
   #define DAQmxWriteAnalogScalarF64(A,B,C,D,E) ss_DAQmxWriteAnalogScalarF64(A,B,C,D,E)
   #define DAQmxCreateDIChan(A,B,C,D) ss_DAQmxCreateDIChan(A,B,C,D)
   #define DAQmxReadDigitalScalarU32(A,B,C,D) ss_DAQmxReadDigitalScalarU32(A,B,C,D)
   #define DAQmxCreateDOChan(A,B,C,D) ss_DAQmxCreateDOChan(A,B,C,D)
   #define DAQmxWriteDigitalScalarU32(A,B,C,D,E) ss_DAQmxWriteDigitalScalarU32(A,B,C,D,E)
#endif

//---------------------------------------------------------------------------
#endif
