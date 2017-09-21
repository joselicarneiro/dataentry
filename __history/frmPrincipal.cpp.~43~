//---------------------------------------------------------------------------

#include <vcl.h>
#include <UIAutomation.h>

#pragma hdrstop

#include "frmPrincipal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tprincipal *principal;
HMODULE MyAutomation;

IUIAutomation *g_pNexio;
IUIAutomationElement *root;

//---------------------------------------------------------------------------
__fastcall Tprincipal::Tprincipal(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void InsertText(IUIAutomationElement *Elemento, wchar_t *Valor) {
	int retEn;
//	IUnknown * *padrao;
	IUIAutomationValuePattern *padrao;

	//Verificar se parametros Elemento e Valor não estão vazios
	Elemento->get_CurrentIsEnabled(&retEn);
	//Fazer Condição
	Elemento->GetCurrentPatternAs(UIA_ValuePatternId, __uuidof(IUIAutomationValuePattern), (void * *)&padrao);
	padrao->SetValue(Valor);
}

void BuscaElemento(IUIAutomationElement *pTipoElemento, IUIAutomationElement *pNomeElemento){
	IUIAutomationTreeWalker *pElemento;
	pElemento->GetFirstChildElement(pTipoElemento, &pNomeElemento);
}

void UI_Spy() {

	// Init COM
	CoInitialize(NULL);

	// Init UIAutomation instance
	IUIAutomation *pAuto;
	CoCreateInstance(CLSID_CUIAutomation, NULL,
		CLSCTX_INPROC_SERVER, IID_IUIAutomation, reinterpret_cast<void**>(&pAuto));

	if (pAuto) {

		IUIAutomationElement *pElm;
		POINT p;

		for (int i = 0; i < 10; i++) {
			for (int j = 5; j > 0; j--) {
				Sleep(1000);
				printf("%d ", j);
			}
			GetCursorPos(&p);
			if (pAuto->ElementFromPoint(p, &pElm) == S_OK) {
				wprintf(L"\nPOSITION x = %d, y = %d\n", p.x, p.y);
				InsertText(pElm, L"Roger");
				BSTR str;
				pElm->get_CurrentAutomationId(&str);
				wprintf(L"-Name = %s\n", str);
				SysFreeString(str);

				pElm->get_CurrentLocalizedControlType(&str);
				wprintf(L"-Type = %s\n", str);
				SysFreeString(str);

				CONTROLTYPEID typeId;
				pElm->get_CurrentControlType(&typeId);

				switch (typeId) {

					// Process checkbox
				case UIA_CheckBoxControlTypeId:
					IUIAutomationTogglePattern  *toggle;
					pElm->GetCurrentPattern(UIA_TogglePatternId, (IUnknown**)&toggle);
					ToggleState state;
					toggle->get_CurrentToggleState(&state);
					printf("-Checkbox = %s\n", state == ToggleState::ToggleState_On ? "TRUE"
						: (state == ToggleState::ToggleState_Off ? "FALSE" : "INTER"));
					break;

					// Process VB6 listview
				case UIA_PaneControlTypeId:
					pElm->get_CurrentClassName(&str);
					if (str != NULL && wcscmp(str, L"ListView20WndClass") == 0) {
						HWND hwnd;
						pElm->get_CurrentNativeWindowHandle((UIA_HWND*)&hwnd);
						printf("-VB6 Listview: %p\n", hwnd);

//                        String txt = getListViewItemText(hwnd, 0, 0);
						//txt = "Test";
//                        printf("-[0,0] = %S\n", (const wchar_t*)txt);
					}
					SysFreeString(str);
					break;

					// Process normal listview
				case UIA_ListControlTypeId:
					HWND hwnd;
					pElm->get_CurrentNativeWindowHandle((UIA_HWND*)&hwnd);
					printf("-Normal Listview: %p\n", hwnd);

//                    CString txt = getListViewItemText(hwnd, 0, 0);
					//txt = "Test";
//                    printf("-[0,0] = %S\n", (const wchar_t*)txt);
					break;
				}

				wprintf(L"\n");
				pElm->Release();
			}
			printf("\n");
		}

		// Release UIAutomation instance
		pAuto->Release();
	}

	// Release COM
	CoUninitialize();
}

HRESULT InitializeUIAutomation()
{
	CoInitialize(NULL);
    HRESULT hr =
		CoCreateInstance(__uuidof(CUIAutomation),
            NULL, CLSCTX_INPROC_SERVER,
            __uuidof(IUIAutomation),
			(void**)&g_pNexio);
    return (hr);
}


IUIAutomationElement* GetTopLevelWindowByName(LPWSTR windowName)
{
	if (!windowName)
	{
		return NULL;
	}

	IUIAutomationElement* pRoot;
	IUIAutomationElement* pFound;
	VARIANT varProp;
	varProp.vt = VT_BSTR;
	varProp.bstrVal = SysAllocString(windowName);

	wchar_t *name;

	// Get the desktop element
	HRESULT hr = g_pNexio->GetRootElement(&pRoot);
	// Get a top-level element by name, such as "Program Manager"
	if (pRoot)
	{
		IUIAutomationCondition* pCondition;
		g_pNexio->CreatePropertyCondition(UIA_NamePropertyId,
			varProp, &pCondition);
		pRoot->FindFirst(TreeScope_Children, pCondition, &pFound);
		pRoot->Release();
		pCondition->Release();
	}

	VariantClear(&varProp);

	return pFound;
}

void __fastcall Tprincipal::btn_AtualizarClick(TObject *Sender)
{
	InitializeUIAutomation();
	BuscaElemento("", L"Userfield1");
	CoUninitialize();
}
//---------------------------------------------------------------------------


void __fastcall Tprincipal::btn_LerDadosClick(TObject *Sender)
{
	IUIAutomationElement *test;
	UI_Spy();

	InitializeUIAutomation();
	test=GetTopLevelWindowByName(L"Nexio Ingest Suite Control Center");
	CoUninitialize();
}
//---------------------------------------------------------------------------

void __fastcall Tprincipal::btn_CancelarClick(TObject *Sender)
{
	principal->Close();
}
//---------------------------------------------------------------------------

