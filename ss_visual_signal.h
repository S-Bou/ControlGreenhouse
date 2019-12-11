//---------------------------------------------------------------------------

#ifndef ss_visual_signalH
#define ss_visual_signalH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "ss_signal.h"
//---------------------------------------------------------------------------
class TF_signaled : public TForm
{
__published:	// IDE-managed Components
   TLabel *lbSignalStr;
   TScrollBar *sbAnalogValue;
   TButton *btHigh;
   TButton *btLow;
   void __fastcall sbAnalogValueChange(TObject *Sender);
   void __fastcall btHighClick(TObject *Sender);
   void __fastcall btLowClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TF_signaled(TComponent* Owner);
   void SetSignal(Tss_signal *p);
   void UpdateSignal(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TF_signaled *F_signaled;
//---------------------------------------------------------------------------
#endif
