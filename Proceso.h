//---------------------------------------------------------------------------
#ifndef ProcesoH
enum {PIN_OFF, PIN_ON};
void process_error();
void process_init(char*);
void process_read_port1(void);
void process_read_ai0(void);
void process_read_ai1(void);
void process_write_port0(void);
void process_write_ao0(void);
void process_write_ao1(void);
#define ProcesoH
//---------------------------------------------------------------------------
#endif
