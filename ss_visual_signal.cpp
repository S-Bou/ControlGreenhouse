//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ss_visual_signal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TF_signaled *F_signaled;

#include "ss_signal.h"
#include "ss_visual_niusb6008.h"

static Tss_signal *psignal;

//---------------------------------------------------------------------------
__fastcall TF_signaled::TF_signaled(TComponent* Owner)
   : TForm(Owner)
{
   psignal = NULL;   // not signal refenrenced
}
//---------------------------------------------------------------------------
void TF_signaled::SetSignal(Tss_signal *p)
{
   psignal = p;
}

void __fastcall TF_signaled::sbAnalogValueChange(TObject *Sender)
{
   char buff[100];

   //Label1->Caption = sbAnalogValue->Position;
   
   psignal->value_analog_set(sbAnalogValue->Position/100.0);

   psignal->str_state(buff);
   lbSignalStr->Caption = buff;

   F_niusb6008->SignalUpdate(psignal);

}
//---------------------------------------------------------------------------
void __fastcall TF_signaled::btHighClick(TObject *Sender)
{
   char buff[100];
   psignal->value_digital_set(SS_VALUE_DIGITAL_HIGH);

   psignal->str_state(buff);
   lbSignalStr->Caption = buff;

   F_niusb6008->SignalUpdate(psignal);
}
//---------------------------------------------------------------------------

void __fastcall TF_signaled::btLowClick(TObject *Sender)
{
   char buff[100];
   psignal->value_digital_set(SS_VALUE_DIGITAL_LOW);

   psignal->str_state(buff);
   lbSignalStr->Caption = buff;

   F_niusb6008->SignalUpdate(psignal);
}
//---------------------------------------------------------------------------

void __fastcall TF_signaled::FormShow(TObject *Sender)
{
   this->UpdateSignal();
}
//---------------------------------------------------------------------------


void TF_signaled::UpdateSignal(void)
{
   //TODO: Add your source code here
      char buff[100];
   Tss_signal_type st;

   if (psignal == NULL) {
      lbSignalStr->Caption = "no signal asociated";
      sbAnalogValue->Visible = false;
      btHigh->Visible = false;
      btLow->Visible = false;
      return;
   }

   psignal->str_state(buff);
   lbSignalStr->Caption = buff;

   st = psignal->signal_type_get();

   // analog signal
   if (st == SS_ST_ANALOG) {
      sbAnalogValue->Visible = true;
      sbAnalogValue->Min = -2000;
      sbAnalogValue->Max = 2000;
      sbAnalogValue->Position = psignal->value_analog_get()*100.0;
   } else {
      sbAnalogValue->Visible = false;
   }

   // digital signal
   if (st == SS_ST_DIGITAL) {
      btHigh->Visible = true;
      btLow->Visible = true;
   } else {
      btHigh->Visible = false;
      btLow->Visible = false;
   }

}
