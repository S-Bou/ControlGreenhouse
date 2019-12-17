//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Proceso.h"
#include "DatosComunes.h"
#include "Usuario.h"
#include "VentanaPuertaAbierta.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
int contador=1;
int conthume=1;
char DeviceName[20];
double humedad;
double temperatura;
bool ALARMA=false, TEMPE=false;
TVPrincipal *VPrincipal;
TVPrincipal *EsDeNoche;
TVPrincipal *EsDeDia;
TVPrincipal *reinicio;
TVPrincipal *CLIMA_OFF;
TVPrincipal *CLIMA_ON;
TVPrincipal *DOOR_CLOSE;
TVPrincipal *DOOR_OPEN;
TVPrincipal *VALVE_ON;
TVPrincipal *VALVE_OFF;
TVPrincipal *FAN_OFF;
TVPrincipal *FAN_ON;
//---------------------------------------------------------------------------
__fastcall TVPrincipal::TVPrincipal(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::BotonIniciarTarjeta(TObject *Sender)
{
    strcpy(DeviceName, VPrincipal->Edit1->Text.c_str());   //Button iniciar
    process_init(DeviceName);
    PTimer->Enabled=true;
    process_write_port0();
    process_write_ao0();
    VPrincipal->GroupBoxEntradas->Enabled=true;
    VPrincipal->GroupBoxSalidas->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::BotonIniciarAutomata(TObject *Sender)
{
    if(VPrincipal->TimerEstadoPuertos->Enabled == false){ //Button timer ON/OFF
        VPrincipal->TimerEstadoPuertos->Enabled = true;
        VPrincipal->PTimer->Caption = "Autómata OFF";
    }else{
        VPrincipal->TimerEstadoPuertos->Enabled = false;
        VPrincipal->PTimer->Color = clBtnFace;
        VPrincipal->PTimer->Caption = "Autómata ON";
    }
}
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::TimerPuertos(TObject *Sender)
{
    process_write_port0();                          //Set port0
    process_read_port1();                           //Send data to Datos
    int port1 = estado_Port1();

    process_read_ai0();
    humedad = estado_AI0();
    VPrincipal->Edit2->Text=redondeo(humedad/2);
    VPrincipal->Edit2->Text=Edit2->Text+"%";

    if (humedad<40 && !ALARMA){                               //State of electrovalve P0_2 and AO0
        //OUTPUT
        VPrincipal->CheckBoxValve->Checked=true;
        CheckBoxValveClick(VALVE_ON);
        //INPUT
        VPrincipal->Shape8->Width=humedad;
        VPrincipal->Shape8->Brush->Color=clYellow;
        VPrincipal->Shape8->Visible=true;
        VPrincipal->Shape9->Visible=false;
        VPrincipal->Shape10->Visible=false;
    }
    if(humedad>=40 && humedad<=160){
        //OUTPUT
        VPrincipal->TimerLedHumedad->Enabled=false;
        VPrincipal->CheckBoxFan->Checked=false;
        CheckBoxFanClick(FAN_OFF);
        VPrincipal->CheckBoxValve->Checked=false;
        CheckBoxValveClick(VALVE_OFF);
        //Reset warning
        VPrincipal->Shape4->Brush->Color=clWhite;
        VPrincipal->ContadorHU->Color=clWhite;
        VPrincipal->ContadorHU->Visible=false;
        conthume=1;
        //INPUT
        VPrincipal->Shape9->Brush->Color=clBlue;
        VPrincipal->Shape9->Width=humedad;
        VPrincipal->Shape8->Visible=false;
        VPrincipal->Shape9->Visible=true;
        VPrincipal->Shape10->Visible=false;
    }
    if(humedad>160 && !ALARMA){                                          //Humity > 80%
        VPrincipal->TimerLedHumedad->Enabled=true;

        VPrincipal->Shape10->Brush->Color=clRed;
        VPrincipal->Shape10->Width=humedad;
        VPrincipal->Shape8->Visible=false;
        VPrincipal->Shape9->Visible=false;
        VPrincipal->Shape10->Visible=true;
    }
    if ((port1 & 0x01) == 0){
        CheckBoxNocheClick(EsDeNoche);               //State of lamp P1_0
        VPrincipal->CheckBoxLampara->Checked=true;
        VPrincipal->Shape1->Brush->Color=clRed;
    }else{
        CheckBoxDiaClick(EsDeDia);
        VPrincipal->CheckBoxLampara->Checked=false;
        VPrincipal->Shape1->Brush->Color=clWhite;
    }
    if ((port1 & 0x02) != 0){                        //State of door P1_1
        CheckBoxPuertaCerradaClick(DOOR_CLOSE);
        VAlarmaPuerta->Close();
        ALARMA = false;
    }else{
        CheckBoxPuertaAbiertaClick(DOOR_OPEN);
    }

//---------TEMPERATURA------------------------------------------------------------------------------

    process_read_ai1();
    temperatura = estado_AI1();
    double tempereal = temperatura/4;
    VPrincipal->Edit3->Text=redondeo(tempereal);
    VPrincipal->Edit3->Text=Edit3->Text+"ºC";

    if (tempereal<15 && !ALARMA){                  //State of temperature P0_3 and AO1
        VPrincipal->CheckBoxClima->Checked=true;
        CheckBoxClimaClick(CLIMA_ON);
        
        VPrincipal->Shape11->Brush->Color=clBlue;
        VPrincipal->Shape11->Width=temperatura;
        VPrincipal->Shape11->Visible=true;
        VPrincipal->Shape12->Visible=false;
        VPrincipal->Shape13->Visible=false;
        TEMPE=true;
    }
    if(tempereal>=15 && tempereal<=35){
        VPrincipal->CheckBoxClima->Checked=false;
        CheckBoxClimaClick(CLIMA_OFF);

        VPrincipal->Shape12->Brush->Color=clGreen;
        VPrincipal->Shape12->Width=temperatura;
        VPrincipal->Shape11->Visible=false;
        VPrincipal->Shape12->Visible=true;
        VPrincipal->Shape13->Visible=false;
        TEMPE=false;
    }
    if(tempereal>35 && !ALARMA){                               //Temperature > 35ºC
        VPrincipal->CheckBoxClima->Checked=true;
        CheckBoxClimaClick(CLIMA_ON);

        VPrincipal->Shape13->Brush->Color=clRed;
        VPrincipal->Shape13->Width=temperatura;
        VPrincipal->Shape11->Visible=false;
        VPrincipal->Shape12->Visible=false;
        VPrincipal->Shape13->Visible=true;
        TEMPE=true;
    }

    if(VPrincipal->PTimer->Color == clYellow){  //change color of buttton timer
        VPrincipal->PTimer->Color = clLime;
    }else{
        VPrincipal->PTimer->Color = clYellow;
    }
}
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::Timer_Led_Humedad(TObject *Sender)
{
    if(conthume==6){                                     //Timer alarma humedad
        if(VPrincipal->Shape4->Brush->Color==clWhite){
            VPrincipal->Shape4->Brush->Color=clRed;
            VPrincipal->ContadorHU->Color=clWhite;
        }else{
            VPrincipal->Shape4->Brush->Color=clWhite;
            VPrincipal->ContadorHU->Color=clRed;
        }
    VPrincipal->ContadorHU->Text=conthume;
    VPrincipal->CheckBoxFan->Checked=true;
    CheckBoxFanClick(FAN_ON);
    }
    if(conthume<6){
    VPrincipal->ContadorHU->Visible=true;
    VPrincipal->CheckBoxFan->Checked=false;
    VPrincipal->ContadorHU->Text=conthume;
    conthume++;
    }
}
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::TimerPuertaAbierta(TObject *Sender)
{
    if(contador==6){                                     //Timer alarma puerta
        if(VPrincipal->Shape5->Brush->Color==clWhite){
            VPrincipal->Shape5->Brush->Color=clRed;
            VPrincipal->ContadorPA->Color=clWhite;
            VAlarmaPuerta->Label1->Font->Color=clYellow;
        }else{
            VPrincipal->Shape5->Brush->Color=clWhite;
            VPrincipal->ContadorPA->Color=clRed;
            VAlarmaPuerta->Label1->Font->Color=clBlack;
        }

    VPrincipal->CheckBoxClima->Checked=false;       //Stop clima
    CheckBoxClimaClick(CLIMA_OFF);

    VPrincipal->ContadorPA->Text=contador;
    VAlarmaPuerta->Show();
    ALARMA = true;
    }
    if(Sender==reinicio){
        VPrincipal->Shape5->Brush->Color=clWhite;
        VPrincipal->ContadorPA->Color=clWhite;
        contador=0;
    }
    if(contador<6){
    VPrincipal->ContadorPA->Visible=true;
    VPrincipal->ContadorPA->Text=contador;
    contador++;
    }
}
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::CheckBoxDiaClick(TObject *Sender)
{                                                             
    if(VPrincipal->CheckBoxDia->Checked==true){
        Store_Port0(0x01, PIN_OFF);                           //CheckBox Día
        VPrincipal->ImageDia->Visible=true;
        VPrincipal->ImageNoche->Visible=false;
        VPrincipal->CheckBoxNoche->Checked=false;
        process_write_port0();
    }
    if(Sender==EsDeDia){
        VPrincipal->CheckBoxDia->Checked=true;
        VPrincipal->CheckBoxNoche->Checked=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::CheckBoxNocheClick(TObject *Sender)
{
    if(VPrincipal->CheckBoxNoche->Checked==true){
        Store_Port0(0x01, PIN_ON);                           //CheckBox Noche
        VPrincipal->ImageNoche->Visible=true;
        VPrincipal->ImageDia->Visible=false;
        VPrincipal->CheckBoxDia->Checked=false;
        process_write_port0();
    }
    if(Sender==EsDeDia){
        VPrincipal->CheckBoxDia->Checked=false;
        VPrincipal->CheckBoxNoche->Checked=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::CheckBoxPuertaCerradaClick(TObject *Sender)
{
    if(VPrincipal->CheckBoxPuertaCerrada->Checked==true){   //CheckBox puerta cerrada
        VPrincipal->ImagePAbierta->Visible=false;
        VPrincipal->ImagePCerrada->Visible=true;
        VPrincipal->CheckBoxPuertaAbierta->Checked=false;
    }
    if(Sender==DOOR_CLOSE){
        VPrincipal->CheckBoxPuertaCerrada->Checked=true;
        VPrincipal->CheckBoxPuertaAbierta->Checked=false;
    }
    VPrincipal->TimerPAbierta->Enabled=false;
    VPrincipal->ContadorPA->Visible=false;
    VPrincipal->Shape5->Brush->Color=clWhite;
    contador=1;
}
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::CheckBoxPuertaAbiertaClick(TObject *Sender)
{
    if(VPrincipal->CheckBoxPuertaAbierta->Checked==true){
        VPrincipal->ImagePAbierta->Visible=true;      //CheckBox puerta abierta
        VPrincipal->ImagePCerrada->Visible=false;
        VPrincipal->CheckBoxPuertaCerrada->Checked=false;
    }
    if(Sender==DOOR_OPEN){
        VPrincipal->CheckBoxPuertaCerrada->Checked=false;
        VPrincipal->CheckBoxPuertaAbierta->Checked=true;
    }
    if(VPrincipal->TimerEstadoPuertos->Enabled==true && TEMPE){
    VPrincipal->TimerPAbierta->Enabled=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::CheckBoxLamparaClick(TObject *Sender)
{                                                         //CheckBox lámpara
    if(VPrincipal->CheckBoxLampara->Checked==true){
        VPrincipal->Shape1->Brush->Color=clRed;
        Store_Port0(0x01, PIN_ON);
        process_write_port0();
    }
    if(VPrincipal->CheckBoxLampara->Checked==false){
        VPrincipal->Shape1->Brush->Color=clWhite;
        Store_Port0(0x01, PIN_OFF);
        process_write_port0();
    }
}
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::CheckBoxClimaClick(TObject *Sender)
{
    if(VPrincipal->CheckBoxClima->Checked==true){
        VPrincipal->Shape2->Brush->Color=clRed;
        Store_Port0(0x02, PIN_ON);
        process_write_port0();
    }
    if(VPrincipal->CheckBoxClima->Checked==false){
        VPrincipal->Shape2->Brush->Color=clWhite;
        Store_Port0(0x02, PIN_OFF);
        process_write_port0();
    }
}
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::CheckBoxValveClick(TObject *Sender)
{
    if(VPrincipal->CheckBoxValve->Checked==true){
        VPrincipal->Shape3->Brush->Color=clRed;
        Store_Port0(0x04, PIN_ON);
        process_write_port0();
    }
    if(Sender == VALVE_ON){
        float HU_Inversa = ((humedad-40)/2)*-5;
        VPrincipal->ScrollBarElectroValvula->Position= HU_Inversa;
        VPrincipal->Edit4->Text=redondeo(HU_Inversa);
        VPrincipal->Edit4->Text=Edit4->Text+"%";
        Store_Humedad(VPrincipal->ScrollBarElectroValvula->Position);
        process_write_ao0();
    }
    if(VPrincipal->CheckBoxValve->Checked==false){
        VPrincipal->Shape3->Brush->Color=clWhite;
        Store_Port0(0x04, PIN_OFF);
        process_write_port0();


        VPrincipal->ScrollBarElectroValvula->Position=0;
        VPrincipal->Edit4->Text=0;
        VPrincipal->Edit4->Text=Edit4->Text+"%";
        Store_Humedad(0);
        process_write_ao0();
    }
}
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::CheckBoxFanClick(TObject *Sender)
{
    if(VPrincipal->CheckBoxFan->Checked==true){
        VPrincipal->Shape6->Brush->Color=clRed;
        Store_Port0(0x08, PIN_ON);
        process_write_port0();

    }
    if(Sender == FAN_ON){
        float ventilacion=((humedad/2)-80)*5;
        VPrincipal->ScrollBarVentilacion->Position=ventilacion;
        VPrincipal->Edit5->Text=redondeo(ventilacion);
        VPrincipal->Edit5->Text=Edit5->Text+"%";
        Store_Fan(VPrincipal->ScrollBarVentilacion->Position);
        process_write_ao1();

    }
    if(VPrincipal->CheckBoxFan->Checked==false){
        VPrincipal->Shape6->Brush->Color=clWhite;
        VPrincipal->ScrollBarVentilacion->Position=0;
        VPrincipal->Edit5->Text=0;
        VPrincipal->Edit5->Text=Edit5->Text+"%";
        Store_Port0(0x08, PIN_OFF);
        Store_Fan(0);
        process_write_port0();
        process_write_ao1();
    }
}
//---------------------------------------------------------------------------
double redondeo(double num){                       //Redondeo de decimales
    double rounded = (int)(num * 10.0)/10.0;       //El numero de ceros
    return rounded;                                //determina los decimales
}                                                  //que muestra: 10->1 decimal
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::ScrollBarElectroValvulaChange(TObject *Sender)
{
    VPrincipal->CheckBoxValve->Checked=true;
    VPrincipal->Shape3->Brush->Color=clRed;
    Store_Humedad(VPrincipal->ScrollBarElectroValvula->Position);
    process_write_port0();
    process_write_ao0();

    VPrincipal->Edit4->Text=VPrincipal->ScrollBarElectroValvula->Position;
    VPrincipal->Edit4->Text=Edit4->Text+"%";
    if(VPrincipal->ScrollBarElectroValvula->Position==0)
    {
        VPrincipal->CheckBoxValve->Checked=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TVPrincipal::ScrollBarVentilacionChange(TObject *Sender)
{
    VPrincipal->CheckBoxFan->Checked=true;
    VPrincipal->Shape6->Brush->Color=clRed;
    Store_Fan(VPrincipal->ScrollBarVentilacion->Position);
    process_write_port0();
    process_write_ao1();

    VPrincipal->Edit5->Text=VPrincipal->ScrollBarVentilacion->Position;
    VPrincipal->Edit5->Text=Edit5->Text+"%";
    if(VPrincipal->ScrollBarVentilacion->Position==0)
    {
        VPrincipal->CheckBoxFan->Checked=false;
        CheckBoxFanClick(FAN_OFF);
    }
}
//---------------------------------------------------------------------------

