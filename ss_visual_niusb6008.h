//---------------------------------------------------------------------------

#ifndef ss_visual_niusb6008H
#define ss_visual_niusb6008H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>

#include "ss_daq.h"
#include <ExtCtrls.hpp>
#include <Graphics.hpp>

//---------------------------------------------------------------------------
class TF_niusb6008 : public TForm
{
__published:	// IDE-managed Components
   TImage *imgDaq;
   TLabel *lbVersion;
   void __fastcall lbEnter(TObject *Sender);
   void __fastcall lbLeave(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall imgDaqMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall imgDaqMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
   void __fastcall imgDaqMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
   __fastcall TF_niusb6008(TComponent* Owner);
   void SetDaq(Tss_daq * p);
   void SignalUpdate(Tss_signal * p);
};
//---------------------------------------------------------------------------
extern PACKAGE TF_niusb6008 *F_niusb6008;
//---------------------------------------------------------------------------
#endif
