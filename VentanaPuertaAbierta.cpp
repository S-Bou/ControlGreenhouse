//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Usuario.h"
#include "VentanaPuertaAbierta.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TVAlarmaPuerta *VAlarmaPuerta;
extern TVPrincipal *reinicio;
//---------------------------------------------------------------------------
__fastcall TVAlarmaPuerta::TVAlarmaPuerta(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TVAlarmaPuerta::BAceptarClick(TObject *Sender)
{
    VPrincipal->TimerPuertaAbierta(reinicio);
    Close();
}
//---------------------------------------------------------------------------
