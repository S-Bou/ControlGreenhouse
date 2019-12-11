// ss_signal.cpp

#include "ss_signal.h"

#include <string.h>
#include <stdio.h>

// class constructor ---------------------------------------------------------
Tss_signal::Tss_signal(char * logic_name, Tss_signal_type sig_type, Tss_signal_dir sig_dir) {

        if (strlen(logic_name) > MAX_LOGIC_NAME) {
                strncpy(signal_logic_name,logic_name,MAX_LOGIC_NAME);
                signal_logic_name[MAX_LOGIC_NAME] = '\0';
        } else {
                strcpy(signal_logic_name,logic_name);
        }

        strcpy(signal_units_name,"");

        signal_type = sig_type;
        signal_dir = sig_dir;

        // signal value unknow
        value_undefined = true;
}

//----------------------------------------------------------------------------
void Tss_signal::units(char *cad) {

        if (strlen(cad) > MAX_UNITS_NAME) {
                strncpy(signal_units_name,cad,MAX_UNITS_NAME);
                signal_units_name[MAX_UNITS_NAME] = '\0';
        } else {
                strcpy(signal_units_name,cad);
        }
}

// ---------------------------------------------------------------------------
// generates an string with the logic name
void Tss_signal::logic_name_get(char *cad) {

   *cad ='\0';

   strcpy(cad,signal_logic_name);
}


//----------------------------------------------------------------------------
void Tss_signal::str_state(char *cad) {

        *cad = '\0';

        strcat(cad,signal_logic_name);

        switch(signal_dir) {
        case SS_SD_INPUT:
                strcat(cad," <-- ");
                break;
        case SS_SD_OUTPUT:
                strcat(cad," --> ");
                break;
        default:
                strcat(cad," -?- ");
                break;
        }

        if (value_undefined) {
                strcat(cad,"?");
        } else {
                switch(signal_type) {
                case SS_ST_ANALOG:
                        sprintf(cad+strlen(cad),"%0.3lf",value_real);
                        break;
                case SS_ST_DIGITAL:
                        if (value_digital == SS_VALUE_DIGITAL_HIGH) {
                                strcat(cad,"H");
                        } else {
                                strcat(cad,"L");
                        }
                        break;
                default:
                        strcat(cad,"??");
                        break;
                }
        }

        strcat(cad," ");
        strcat(cad,signal_units_name);
}

//----------------------------------------------------------------------------
// left side
void Tss_signal::str_state_lj(char *cad) {

        *cad = '\0';

        if (value_undefined) {
                strcat(cad,"?");
        } else {
                switch(signal_type) {
                case SS_ST_ANALOG:
                        sprintf(cad+strlen(cad),"%0.3lf",value_real);
                        break;
                case SS_ST_DIGITAL:
                        if (value_digital == SS_VALUE_DIGITAL_HIGH) {
                                strcat(cad,"H");
                        } else {
                                strcat(cad,"L");
                        }
                        break;
                default:
                        strcat(cad,"??");
                        break;
                }
        }

        strcat(cad," ");
        strcat(cad,signal_units_name);

        switch(signal_dir) {
        case SS_SD_INPUT:
                strcat(cad," --> ");
                break;
        case SS_SD_OUTPUT:
                strcat(cad," <-- ");
                break;
        default:
                strcat(cad," -?- ");
                break;
        }

        strcat(cad,signal_logic_name);
}

//----------------------------------------------------------------------------
void Tss_signal::value_analog_set(double val) {

        // test if type of signal correct
        if (signal_type == SS_ST_ANALOG){
                value_real=val;
                value_undefined = false;
        } else {
                throw "Fallo en value_analog_set()";
        }
}

//----------------------------------------------------------------------------
double Tss_signal::value_analog_get(void) {

        // test if type of signal correct
        if ((signal_type == SS_ST_ANALOG) && (!value_undefined)){
                return(value_real);
        } else {
                throw "Fallo en value_analog_get()";
        }
}

//----------------------------------------------------------------------------
void Tss_signal::value_digital_set(Tss_signal_value_digital val) {

        // test if type of signal correct
        if (signal_type == SS_ST_DIGITAL){
                value_digital=val;
                value_undefined = false;
        } else {
                throw "Fallo en value_digital_set()";
        }
}

//----------------------------------------------------------------------------
Tss_signal_value_digital Tss_signal::value_digital_get(void) {
        // test if type of signal correct
        if ((signal_type == SS_ST_DIGITAL) && (!value_undefined)){
                return(value_digital);
        } else {
                throw "Fallo en value_digital_get()";
        }
}

//----------------------------------------------------------------------------
bool Tss_signal::logic_name_equal(char *cad) {

        if (strcmp(cad,signal_logic_name) == 0) {
                return(true);
        } else {
                return(false);
        }
}

//----------------------------------------------------------------------------
Tss_signal_type Tss_signal::signal_type_get(void) {

   return(signal_type);
}

//----------------------------------------------------------------------------
Tss_signal_dir Tss_signal::signal_dir_get(void) {

   return(signal_dir);
}

//----------------------------------------------------------------------------
void Tss_signal::signal_dir_set(Tss_signal_dir sd) {
   signal_dir = sd;
}
