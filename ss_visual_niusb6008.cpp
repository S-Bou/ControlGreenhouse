//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ss_visual_niusb6008.h"
#include "ss_visual_signal.h"
#include "ss_version.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TF_niusb6008 *F_niusb6008;

#define NUM_TERMINALS 32
#define LBCOLOR_HIGHLIGHT clYellow
#define LBCOLOR_NORMAL    clAqua

char *terminal_names[NUM_TERMINALS] = {
   "GND","AI0","AI4",
   "GND","AI1","AI5",
   "GND","AI2","AI6",
   "GND","AI3","AI7",
   "GND","AO0","AO1",
   "GND",
   "P0.0","P0.1","P0.2","P0.3","P0.4","P0.5","P0.6","P0.7",
   "P1.0","P1.1","P1.2","P1.3",
   "PFI0",
   "+2.5V", "+5V", "GND"
};

Tss_signal *signals[NUM_TERMINALS]; // signals associated to terminals

Tss_daq *pdaq = NULL;   // pointer to DAQ with the signals

TLabel *lb[NUM_TERMINALS];   // labels for terminals

//---------------------------------------------------------------------------
__fastcall TF_niusb6008::TF_niusb6008(TComponent* Owner)
   : TForm(Owner)
{
   int i;

   lbVersion->Caption = "SimSeny II v. ";
   lbVersion->Caption = lbVersion->Caption + SS_VERSION;


   for (i=0;i<16;i++) {
      lb[i] = new TLabel(this);
      lb[i]->Parent = this;
      lb[i]->Name = "sg" + IntToStr(i);
      lb[i]->Caption = "";
      lb[i]->Alignment = taRightJustify;
      lb[i]->Left = imgDaq->Left - 5;
      lb[i]->Top = 5 +i*15;
      lb[i]->Color = LBCOLOR_NORMAL;
      lb[i]->OnMouseEnter = this->lbEnter;
      lb[i]->OnMouseLeave = this->lbLeave;
      lb[i]->Caption = terminal_names[i];
   }
   for (i=16;i<NUM_TERMINALS;i++) {
      lb[i] = new TLabel(this);
      lb[i]->Parent = this;
      lb[i]->Name = "sg" + IntToStr(i);
      lb[i]->Caption = "";
      lb[i]->Alignment = taLeftJustify;
      lb[i]->Left = imgDaq->Left + imgDaq->Width + 5;
      lb[i]->Top = 5 + (i-16)*15;
      lb[i]->Color = LBCOLOR_NORMAL;
      lb[i]->OnMouseEnter = this->lbEnter;
      lb[i]->OnMouseLeave = this->lbLeave;
      lb[i]->Caption = terminal_names[i];      
   }
}
//---------------------------------------------------------------------------
void __fastcall TF_niusb6008::lbEnter(TObject *Sender)
{
   // entering a label
   TLabel *plabel;
   int i;
   int pos_x, pos_y;

   plabel = dynamic_cast <TLabel *> (Sender);
   plabel->Color = LBCOLOR_HIGHLIGHT;

   // buscarlo
   for (i=0;i<NUM_TERMINALS;i++) {
      if (plabel == lb[i])
         break;
   }

   if (i<NUM_TERMINALS) {// encontrado
      //Label1->Caption = i;
      if (signals[i] == NULL) {
         F_signaled->Hide();
      } else {
         if (signals[i]->signal_dir_get() == SS_SD_INPUT) {
             F_signaled->SetSignal(signals[i]);
             if (i<16) {
                pos_x = this->Left + lb[i]->Left - F_signaled->Width - 10;
                pos_y = this->Top + lb[i]->Top;
             } else {
                pos_x = this->Left + lb[i]->Left + lb[i]->Width + 10;
                pos_y = this->Top + lb[i]->Top;
             }
             F_signaled->Left = pos_x;
             F_signaled->Top = pos_y;
             F_signaled->UpdateSignal();
             F_signaled->Show();
         } else {
           F_signaled->Hide();
         }
      }
   } else {
      //Label1->Caption = "Desconocido";
   }
}
//---------------------------------------------------------------------------

void __fastcall TF_niusb6008::lbLeave(TObject *Sender)
{
   //leaving a label
   TLabel *plabel;

   plabel = dynamic_cast <TLabel *> (Sender);
   plabel->Color = LBCOLOR_NORMAL;
}
//---------------------------------------------------------------------------


void TF_niusb6008::SetDaq(Tss_daq * p)
{

   int i;
   char buff[100];

   pdaq  = p;

   // update references to signals
   for (i=0;i<NUM_TERMINALS;i++) {
      signals[i] = pdaq->signals_find(terminal_names[i]);
   }

   // update labels
   for (i=0;i<NUM_TERMINALS;i++) {
      if (signals[i] != NULL) {
         if (i<16) { //lesf side
            signals[i]->str_state_lj(buff);
         } else {
            signals[i]->str_state(buff);
         }
         lb[i]->Caption = buff;
      }
   }

}

//---------------------------------------------------------------------------
void TF_niusb6008::SignalUpdate(Tss_signal * p)
{
   //TODO: Add your source code here
   int i;
   char buff[100];

   for (i=0;i<NUM_TERMINALS;i++) {
      if (signals[i] == p) {
         if (i<16) { //left side
            signals[i]->str_state_lj(buff);
         } else {
            signals[i]->str_state(buff);
         }
         lb[i]->Caption = buff;
      }
   }

}


void __fastcall TF_niusb6008::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
   CanClose = false; // que no se pueda cerrar

}
//---------------------------------------------------------------------------
bool mouse_down = false;
int mouse_x, mouse_y;

void __fastcall TF_niusb6008::imgDaqMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   //mouse_down = true;
   mouse_down = false;
   mouse_x = X;
   mouse_y = Y;
}
//---------------------------------------------------------------------------

void __fastcall TF_niusb6008::imgDaqMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   if (mouse_down) {
      this->Left = this->Left + (X - mouse_x);
      this->Top =  this->Top + (Y - mouse_y);
      mouse_x = X;
      mouse_y = Y;
   }
}
//---------------------------------------------------------------------------

void __fastcall TF_niusb6008::imgDaqMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   mouse_down = false;
}
//---------------------------------------------------------------------------

