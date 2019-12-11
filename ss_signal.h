// ss_signal.h
// Angel Perles. 2008


// physical signals representation

#ifndef ss_signalH
#define ss_signalH

#define MAX_LOGIC_NAME 20
#define MAX_UNITS_NAME 20

// type of the signal: analog, digital, ...
enum Tss_signal_type {SS_ST_ANALOG,SS_ST_DIGITAL};

// direction of the signal: input, output, ...
enum Tss_signal_dir {SS_SD_INPUT, SS_SD_OUTPUT, SS_SD_UNDEFINED};

// posible digital values
enum Tss_signal_value_digital {SS_VALUE_DIGITAL_HIGH, SS_VALUE_DIGITAL_LOW};

//----------------------------------------------------------------------------
class Tss_signal {

public:

	// Class constructor
	// logic_name -> string that identifies the signal
	// sig_type -> type of the signal: analog, digital, ...
	// sid_dir -> direction of the signal: input, output, ...
	Tss_signal(char * logic_name, Tss_signal_type sig_type, Tss_signal_dir sig_dir);

   // set a string identifying the units of the magnitude. For reports
   void units(char *cad);

   // generates an string with the logic name
   void logic_name_get(char *cad);

   // generates a report string on the given string
   void str_state(char *cad);
   void Tss_signal::str_state_lj(char *cad); // left side version

   // set/get functions for analog values
   void value_analog_set(double val);
   double value_analog_get(void);

   // set/get functions for digital values
   void value_digital_set(Tss_signal_value_digital val);
   Tss_signal_value_digital value_digital_get(void);

   // test if the given logic name is equal to the one assigned to this signal
   bool logic_name_equal(char *cad);

   // returns the signal type
   Tss_signal_type signal_type_get(void);

   // returns/sets the signal dir
   Tss_signal_dir signal_dir_get(void);
   void signal_dir_set(Tss_signal_dir sd);

private:

	char signal_logic_name[MAX_LOGIC_NAME+1];
        char signal_units_name[MAX_UNITS_NAME+1];
	Tss_signal_type signal_type;
	Tss_signal_dir signal_dir;

        // signal value storage
        bool value_undefined;        // the valuas has not benn deined yet
        double value_real;      // volts, amps. ...
        Tss_signal_value_digital value_digital;      // {0,1}

};

//----------------------------------------------------------------------------
#endif

