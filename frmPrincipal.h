//---------------------------------------------------------------------------

#ifndef frmPrincipalH
#define frmPrincipalH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class Tprincipal : public TForm
{
__published:	// IDE-managed Components
	TEdit *edt_Userfield1;
	TEdit *edt_Userfield2;
	TEdit *edt_Userfield3;
	TEdit *edt_Userfield4;
	TEdit *edt_Agency;
	TEdit *edt_Description;
	TButton *btn_LerDados;
	TButton *btn_Atualizar;
	TButton *btn_Cancelar;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	void __fastcall btn_AtualizarClick(TObject *Sender);
	void __fastcall btn_LerDadosClick(TObject *Sender);
	void __fastcall btn_CancelarClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall Tprincipal(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tprincipal *principal;
//---------------------------------------------------------------------------
#endif