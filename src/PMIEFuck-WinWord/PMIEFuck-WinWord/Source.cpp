#import <D:\Program Files (x86)\Microsoft Office\Office14\MSWORD.OLB> auto_search auto_rename no_auto_exclude rename("ExitWindows", "WordExitWindows")
#include <Windows.h>

typedef size_t (*MBSTOWCS)(wchar_t *wcstr, const char *mbstr, size_t count);

LPUNKNOWN pUnk = NULL;
BSTR strGDoc, strGS, strGD;



LONG WINAPI MyUnhandledExceptionFilter(__in struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	DeleteFileW(strGDoc);
	DeleteFileW(strGS);

	ExitThread(0);
}
#pragma comment(linker, "/EXPORT:qwopfnch=?qwopfnch@@YGHPAK@Z")
__declspec(dllexport) BOOL WINAPI qwopfnch(LPDWORD lpArgs)
{
	IID clsid;
	IID IIDWinWord;
	HRESULT hRes;


	// initialize the COM servers
	IIDFromString(L"{000209ff-0000-0000-c000-000000000046}", &clsid);
	IIDFromString(L"{00000000-0000-0000-c000-000000000046}", &IIDWinWord);

	CoInitialize(NULL);
	hRes = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IIDWinWord, (LPVOID *)&pUnk);
	if (hRes != S_OK)
		ExitThread(0);

	// prepare the arguments
	BSTR strDocumentPath = SysAllocString((LPWSTR)lpArgs[0]);
	BSTR strSourcePath = SysAllocString((LPWSTR)lpArgs[1]); 
	BSTR strDestPath = SysAllocString((LPWSTR)lpArgs[2]);

	VARIANTARG vDocName;
	VariantInit(&vDocName);
	vDocName.vt = VT_BSTR;
	vDocName.bstrVal = strDocumentPath;

	VARIANTARG vArg1;
	VariantInit(&vArg1);
	vArg1.vt = VT_BSTR;
	vArg1.bstrVal = strSourcePath;

	VARIANTARG vArg2;
	VariantInit(&vArg2);
	vArg2.vt = VT_BSTR;
	vArg2.bstrVal = strDestPath;

	VARIANTARG vFalse;
	VariantInit(&vFalse);
	vFalse.vt = VT_BOOL;
	vFalse.boolVal = FALSE;

	VARIANTARG vTrue;
	VariantInit(&vTrue);
	vTrue.vt = VT_BOOL;
	vTrue.boolVal = TRUE;
	
	// launch the app (should be already up&running but whatever)
	hRes = OleRun(pUnk);
	Word::_ApplicationPtr pWordApp = pUnk;

	// check if > office 2003
	BSTR bstrWordVersion = pWordApp->Application->GetVersion();

	if (bstrWordVersion == NULL || wcslen(bstrWordVersion) < 3)
		ExitThread(0);

	bstrWordVersion[2] = L'\0';
	if (_wtoi(bstrWordVersion) < 12) // 12 == office 2007
		ExitThread(0);


	// in case something bad happens, we have a chance to cleanup things.
	LPTOP_LEVEL_EXCEPTION_FILTER lpOldExceptionFilter = NULL;
	lpOldExceptionFilter = SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	// open the document
	Word::DocumentsPtr pWordDoc = pWordApp->Documents;
	Word::_DocumentPtr pDoc = pWordDoc->Open(&vDocName,
		&vtMissing,
		&vtMissing,
		&vFalse,		// addtorecentfile
		&vtMissing,
		&vtMissing,
		&vtMissing,
		&vtMissing,
		&vtMissing,
		&vtMissing,
		&vtMissing,
		&vtMissing,
		&vtMissing,
		&vtMissing,
		&vtMissing,
		&vtMissing);


	// execute the macro
	BSTR strMacroName = SysAllocString(L"MyMac");
	hRes = pWordApp->Run(strMacroName, &vArg1, &vArg2);

	Sleep(3000);

	// clean up
	pDoc->Close(&vFalse);
	pDoc->Release();
	pWordDoc->Release();
	pWordApp->Quit(&vFalse);

	CoUninitialize();

	if (lpOldExceptionFilter)
		SetUnhandledExceptionFilter(lpOldExceptionFilter);

	ExitThread(0);
	return TRUE;
}


BOOL WINAPI DllMain(
  _In_  HINSTANCE hinstDLL,
  _In_  DWORD fdwReason,
  _In_  LPVOID lpvReserved
  )
{
/*
int main()
{
	DWORD bu[] = { (DWORD)L"d", (DWORD)L"A", (DWORD)L"A" };
	qwopfnch((LPDWORD)&bu);
*/
	return TRUE;
}
