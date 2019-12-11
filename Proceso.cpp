//---------------------------------------------------------------------------
#pragma hdrstop
#include "Proceso.h"
#include "ss_nidaqmx.h"
#include "DatosComunes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
int portstate;
TaskHandle Read_Port1;
TaskHandle Write_Port0;
TaskHandle Read_AI0;
TaskHandle Read_AI1;
TaskHandle Write_AO0;
TaskHandle Write_AO1;
// process error reporting --------------------------------------------------
void process_error(int32 code, char *suffix) {
    AnsiString msg;
    char buff[200]; // buffer for the error message
    msg = "Error: ";
    msg = msg + suffix;
    msg = msg + "\n";
    msg = msg + "In: ";
    DAQmxGetErrorString (code, buff, 200);  // get the error message
    msg = msg + buff;
    ShowMessage(msg);   exit(1);     // force the end of the application
}
// process initialization ---------------------------------------------------
void process_init(char *deviceName) {

    char nameP1[20], nameP0[20], nameAI0[20], nameAI1[20], nameAO0[20], nameAO1[20];;
    strcpy(nameP1, deviceName);
    strcat(nameP1, "/port1");
    strcpy(nameP0, deviceName);
    strcat(nameP0, "/port0");
    strcpy(nameAI0, deviceName);
    strcat(nameAI0, "/ai0");
    strcpy(nameAI1, deviceName);
    strcat(nameAI1, "/ai1");
    strcpy(nameAO0, deviceName);
    strcat(nameAO0, "/ao0");
    strcpy(nameAO1, deviceName);
    strcat(nameAO1, "/ao1");

    int32 daq_error;
    portstate = 0x00;

// First, create an empty task
daq_error = DAQmxCreateTask("Read task",&Read_Port1);
    if(daq_error != 0)process_error(daq_error,"process_init()->1.0");

daq_error = DAQmxCreateTask("Write task",&Write_Port0);
    if(daq_error != 0)process_error(daq_error,"process_init()->1.1");

daq_error = DAQmxCreateTask("Analog read task", &Read_AI0);
    if(daq_error != 0)process_error(daq_error,"process_init()->1.2");

daq_error = DAQmxCreateTask("Analog read task", &Read_AI1);
    if(daq_error != 0)process_error(daq_error,"process_init()->1.2");

daq_error = DAQmxCreateTask("Analog write task", &Write_AO0);
    if(daq_error != 0)process_error(daq_error, "process_init()->1.3");

daq_error = DAQmxCreateTask("Analog write task", &Write_AO1);
    if(daq_error != 0)process_error(daq_error, "process_init()->1.3");

// Now, add channels to the task
daq_error = DAQmxCreateDIChan(Read_Port1, nameP1,
            "",DAQmx_Val_ChanForAllLines);
    if(daq_error != 0)process_error(daq_error, "process_init()->2.0");

daq_error = DAQmxCreateDOChan(Write_Port0, nameP0,
            "",DAQmx_Val_ChanForAllLines);
    if(daq_error != 0)process_error(daq_error, "process_init()->2.1");

daq_error = DAQmxCreateAIVoltageChan(Read_AI0, nameAI0,
            "", DAQmx_Val_RSE, 0.0, 10.0, DAQmx_Val_Volts, NULL);
    if(daq_error != 0)process_error(daq_error, "process_init()->2.2");

daq_error = DAQmxCreateAIVoltageChan(Read_AI1, nameAI1,
            "", DAQmx_Val_RSE, 0.0, 10.0, DAQmx_Val_Volts, NULL);
    if(daq_error != 0)process_error(daq_error, "process_init()->2.2");

daq_error = DAQmxCreateAOVoltageChan(Write_AO0, nameAO0,
            "", 0.0, 5.0, DAQmx_Val_Volts, NULL);
    if(daq_error != 0)process_error(daq_error, "process_init()->2.3");

daq_error = DAQmxCreateAOVoltageChan(Write_AO1, nameAO1,
            "", 0.0, 5.0, DAQmx_Val_Volts, NULL);
    if(daq_error != 0)process_error(daq_error, "process_init()->2.3");

Store_Port0(0xFF, PIN_OFF);
}
// read port 1 pins and return state -------------------------------------------
void process_read_port1(void) {
    int32 daq_error;
    uInt32 data;

daq_error = DAQmxReadDigitalScalarU32 (Read_Port1, 0.0, &data, NULL);

    if (daq_error != 0) process_error(daq_error,"process_read_port1()");

    Store_Port1(data);
}
// write port 0 pins -----------------------------------------------------------
void process_write_port0(void){
    int bitMask = estado_Wport1();
    int statepin = estado_Pin();
    int32 daq_error;

    if (statepin == PIN_OFF){
        portstate &=~ bitMask; //Pin set 0
    }else{
        portstate |=  bitMask; //Pin set 1
    }

daq_error = DAQmxWriteDigitalScalarU32(Write_Port0, true, 0.0, portstate, NULL);

    if (daq_error != 0) process_error(daq_error,"process_write_port0()");
}
// read ai0 --------------------------------------------------------------------
void process_read_ai0(void){
    int32 daq_error;
    float64 Humity;

daq_error = DAQmxReadAnalogScalarF64 (Read_AI0, 1.0,&Humity, NULL);
    if (daq_error != 0)process_error(daq_error,"process_read_ai0()");

    Store_AI0(Humity);
}
// read ai1 --------------------------------------------------------------------
void process_read_ai1(void){
    int32 daq_error;
    float64 tempe;

daq_error = DAQmxReadAnalogScalarF64 (Read_AI1, 1.0,&tempe, NULL);
    if (daq_error != 0)process_error(daq_error,"process_read_ai1()");

    Store_AI1(tempe);
}
// write ao0 -------------------------------------------------------------------
void process_write_ao0(void){
    int32 daq_error;
    float64 volts = EstadoHumedad();

daq_error = DAQmxWriteAnalogScalarF64(Write_AO0, true, 1.0, volts, NULL);
    if(daq_error != 0)process_error(daq_error, "process_write_ao0()");
}
// write ao1 -------------------------------------------------------------------
void process_write_ao1(void){
    int32 daq_error;
    float64 volts = EstadoVentilador();

daq_error = DAQmxWriteAnalogScalarF64(Write_AO1, true, 1.0, volts, NULL);
    if(daq_error != 0)process_error(daq_error, "process_write_ao1()");
}

