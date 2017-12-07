//---------------------------------------------------------------------------
#include <vcl.h>
#include <UIAutomation.h>
#include <stdio.h>

#pragma hdrstop

#include "frmPrincipal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

Tprincipal *principal;
HMODULE MyAutomation;

IUIAutomation *g_pNexio;
IUIAutomationElement *root, *elementFound, *pAplicacao;
IUIAutomationElementArray *g_pAutomation;

//---------------------------------------------------------------------------
__fastcall Tprincipal::Tprincipal(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------



//void Tprincipal::WalkerControlElements(IUIAutomationElement *pElementRoot, wchar_t *automationID, wchar_t *valorElemento) {
void Tprincipal::WalkerControlElements(IUIAutomationElement *pElementRoot, wchar_t *automationID) {

	IUIAutomationTreeWalker* TreeWalker;
	IUIAutomationElement *elementNode;
	IUIAutomationElement *guardaElementNode;
	wchar_t *strLoc;
	wchar_t *strName;
	wchar_t *nomeUAI;
	UnicodeString keep1, keep2;

	g_pNexio->get_ControlViewWalker(&TreeWalker);
	TreeWalker->GetFirstChildElement(pElementRoot, &elementNode);

	while(elementNode!=NULL) {
		elementNode->get_CurrentLocalizedControlType(&strLoc);
		elementNode->get_CurrentAutomationId(&strName);
		keep1=UnicodeString(strName).c_str();
		keep2=UnicodeString(automationID).c_str();
		if(keep1==keep2){
			elementFound=elementNode;
		}
		WalkerControlElements(elementNode, automationID);
		guardaElementNode=elementNode;
		TreeWalker->GetNextSiblingElement(guardaElementNode, &elementNode);
	}
}

IUIAutomationElement* GetTopLevelWindowByName(LPWSTR windowName) {
	if (!windowName){
		return NULL;
	}

	IUIAutomationElement* pRoot;
	IUIAutomationElement* pFound;
	VARIANT varProp;
	varProp.vt = VT_BSTR;
	varProp.bstrVal = SysAllocString(windowName);

	HRESULT hr = g_pNexio->GetRootElement(&pRoot);

	if (pRoot) {
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

IUIAutomationElement* BuscaElementos(IUIAutomationElement *pElementRoot, LPWSTR pAutoID, int MaxDepth) {
	if(MaxDepth==0) {
		return NULL;
	}

/*
	VARIANT varBusca;
	varBusca.vt = VT_BSTR;
	varBusca.bstrVal = SysAllocString(pAutomationID);
*/

	IUIAutomationTreeWalker* proximoElementoIrmao;
	IUIAutomationTreeWalker* proximoElementoFilho;

	IUIAutomationElement* pFound;


	int tamanho;
	wchar_t **nomeElement;
	wchar_t **nomeAutomationID;

	g_pNexio->get_RawViewWalker(&proximoElementoIrmao);
	g_pNexio->get_RawViewWalker(&proximoElementoFilho);
	proximoElementoIrmao->GetFirstChildElement(pElementRoot, &pFound);
	proximoElementoFilho->GetNextSiblingElement(pElementRoot, &pFound);
	int propId;

	while(true){

		while(pFound != NULL) {
			pFound->get_CurrentName(nomeElement);
			pFound->get_CurrentAutomationId(nomeAutomationID);
			if((nomeElement==pAutoID) || (nomeAutomationID==pAutoID)) {
				return pFound;
			}
			proximoElementoIrmao->GetNextSiblingElement(pFound, &pFound);
		}
		proximoElementoIrmao->GetFirstChildElement(pElementRoot, &pFound);

		if(pFound == NULL) {
			return pFound;
		}
	}

//	VariantClear(&varBusca);

	return NULL;
}

IUIAutomationElement* BuscaElemento(IUIAutomationElement *pElementRoot, LPWSTR pAutomationID){
//
//	IUIAutomationElementArray* pFound;
//	VARIANT varProp;
//	VARIANT varName;
//
//	varProp.vt = VT_BSTR;
//	varProp.bstrVal = SysAllocString(L"TextBox");
//
//	varName.vt = VT_BSTR;
//	varName.bstrVal = SysAllocString(L"");
//
//	IUIAutomationCondition* pCondition;
//	IUIAutomationCondition* pCondName;
//	IUIAutomationCondition* pCondComb;
//	IUIAutomationElement*   pElement;
//
//	IUIAutomationTreeWalker* buscaObjeto;
//
////	HRESULT hr=g_pNexio->CreatePropertyCondition(UIA_ClassNamePropertyId,
////		varProp, &pCondition);
//
////	HRESULT hg=g_pNexio->CreatePropertyCondition(UIA_ClassNamePropertyId, varName,
////		&pCondName);
//
////	g_pNexio->get_ControlViewWalker(&buscaObjeto);
//
//	HRESULT hr=g_pNexio->CreateTreeWalker(pCondName, &buscaObjeto);
//
//   //	HRESULT hr=g_pNexio->CreatePropertyCondition(UIA_AutomationIdPropertyId,
////		varProp, &pCondition);
//
////	HRESULT hc=g_pNexio->CreateAndCondition(pCondition, pCondName, &pCondComb);
//
//
////	HRESULT hr=g_pNexio->CreatePropertyCondition(UIA_AutomationIdPropertyId,
//  //		varProp, &pCondition);
//
//	if (SUCCEEDED(hr)) {
//		hr = pElementRoot->FindAll(TreeScope_Subtree, pCondition, &pFound);
//		buscaObjeto->
////		pElementRoot->Release();
//		pCondition->Release();
//		pCondName->Release();
//		pCondComb->Release();
//		int tamanho;
//		wchar_t **nomeElement;
//		wchar_t **nomeAutomationID;
//		pFound->get_Length(&tamanho);
//		for (int i = 0; i < tamanho; i ++) {
//			pFound->GetElement(i, &pElement);
//			pElement->get_CurrentName(nomeElement);
//			pElement->get_CurrentAutomationId(nomeAutomationID);
//			wprintf(L"Contem: %s %s\n", nomeElement, nomeAutomationID);
//		}
//	}
//
//	VariantClear(&varProp);
//
//	return pElement;
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
				Sleep(100);
				printf("%d ", j);
			}
			GetCursorPos(&p);
			if (pAuto->ElementFromPoint(p, &pElm) == S_OK) {
				wprintf(L"\nPOSITION x = %d, y = %d\n", p.x, p.y);
				BSTR str;
				pElm->get_CurrentAutomationId(&str);
				wprintf(L"-Name = %s\n", str);
				InsertText(pElm, str);
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

void __fastcall Tprincipal::btn_AtualizarClick(TObject *Sender) {
	InitializeUIAutomation();
	IUIAutomationValuePattern *novoValor;
	wchar_t *strApp;
	int elementoVisivel;

	pAplicacao=GetTopLevelWindowByName(L"Nexio Ingest Suite Control Center");
	pAplicacao->SetFocus();

	pAplicacao->get_CurrentName(&strApp);
	WalkerControlElements(pAplicacao, L"TimelineMetaData_USERFIELD1");

//	elementFound->get_CurrentIsOffscreen(&elementoVisivel);
//	if(elementoVisivel==1) {
//		elementFound->GetCurrentPatternAs(UIA_ValuePatternId, __uuidof(IUIAutomationValuePattern), (void * *)&novoValor);
//		novoValor->SetValue(L"Teste de atualizacao do valor");
//	} else {
//		Application->MessageBox(UnicodeString("Ingest não está visível!").c_str(), L"Erro", MB_OK);
//	}

	elementFound->GetCurrentPatternAs(UIA_ValuePatternId, __uuidof(IUIAutomationValuePattern), (void * *)&novoValor);
	novoValor->SetValue(UnicodeString(edt_Userfield1->Text).c_str());

	WalkerControlElements(pAplicacao, L"TimelineMetaData_USERFIELD2");
	elementFound->GetCurrentPatternAs(UIA_ValuePatternId, __uuidof(IUIAutomationValuePattern), (void * *)&novoValor);
	novoValor->SetValue(UnicodeString(edt_Userfield2->Text).c_str());

	WalkerControlElements(pAplicacao, L"TimelineMetaData_USERFIELD3");
	elementFound->GetCurrentPatternAs(UIA_ValuePatternId, __uuidof(IUIAutomationValuePattern), (void * *)&novoValor);
	novoValor->SetValue(UnicodeString(edt_Userfield3->Text).c_str());

	WalkerControlElements(pAplicacao, L"TimelineMetaData_USERFIELD4");
	elementFound->GetCurrentPatternAs(UIA_ValuePatternId, __uuidof(IUIAutomationValuePattern), (void * *)&novoValor);
	novoValor->SetValue(UnicodeString(edt_Userfield4->Text).c_str());

	WalkerControlElements(pAplicacao, L"TimelineMetaData_AGENCY");
	elementFound->GetCurrentPatternAs(UIA_ValuePatternId, __uuidof(IUIAutomationValuePattern), (void * *)&novoValor);
	novoValor->SetValue(UnicodeString(edt_Agency->Text).c_str());

	WalkerControlElements(pAplicacao, L"TimelineMetaData_AGENCY");
	elementFound->GetCurrentPatternAs(UIA_ValuePatternId, __uuidof(IUIAutomationValuePattern), (void * *)&novoValor);
	novoValor->SetValue(UnicodeString(edt_Agency->Text).c_str());

	WalkerControlElements(pAplicacao, L"TimelineMetaData_DESCRIPTION");
	elementFound->GetCurrentPatternAs(UIA_ValuePatternId, __uuidof(IUIAutomationValuePattern), (void * *)&novoValor);
	novoValor->SetValue(UnicodeString(edt_Description->Text).c_str());

	CoUninitialize();
}

void __fastcall Tprincipal::btn_PesqDadosClick(TObject *Sender)
{
		Application->MessageBox(UnicodeString("Função de busca dos dados não implementada!").c_str(), L"Atenção", MB_OK);
}

void __fastcall Tprincipal::btn_CancelarClick(TObject *Sender)
{
	principal->Close();
}

