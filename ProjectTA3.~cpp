//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Usuario.cpp", VPrincipal);
USEFORM("ss_visual_niusb6008.cpp", F_niusb6008);
USEFORM("ss_visual_signal.cpp", F_signaled);
USEFORM("VentanaPuertaAbierta.cpp", VAlarmaPuerta);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TVPrincipal), &VPrincipal);
         Application->CreateForm(__classid(TF_niusb6008), &F_niusb6008);
         Application->CreateForm(__classid(TF_signaled), &F_signaled);
         Application->CreateForm(__classid(TVAlarmaPuerta), &VAlarmaPuerta);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
