/******************************************************************************
*
*
* metapath - The universal Explorer-like Plugin
*
* Helpers.c
*   General helper functions
*
* See Readme.txt for more information about this source code.
* Please send me your comments to this work.
*
* See License.txt for details about distribution and modification.
*
*                                              (c) Florian Balmer 1996-2011
*                                                  florian.balmer@gmail.com
*                                               http://www.flos-freeware.ch
*
*
******************************************************************************/
#define _WIN32_WINNT 0x501
#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <uxtheme.h>
#include <stdio.h>
#include <string.h>
#include "dlapi.h"
#include "helpers.h"
#include "resource.h"



//=============================================================================
//
//  Manipulation of (cached) ini file sections
//
int IniSectionGetString(
      LPCWSTR lpCachedIniSection,
      LPCWSTR lpName,
      LPCWSTR lpDefault,
      LPWSTR lpReturnedString,
      int cchReturnedString)
{
  WCHAR *p = (WCHAR *)lpCachedIniSection;
  WCHAR tch[256];
  int  ich;

  if (p) {
    lstrcpy(tch,lpName);
    lstrcat(tch,L"=");
    ich = lstrlen(tch);

    while (*p) {
      if (StrCmpNI(p,tch,ich) == 0) {
        lstrcpyn(lpReturnedString,p + ich,cchReturnedString);
        return(lstrlen(lpReturnedString));
      }
      else
        p = StrEnd(p) + 1;
    }
  }
  lstrcpyn(lpReturnedString,lpDefault,cchReturnedString);
  return(lstrlen(lpReturnedString));
}


int IniSectionGetInt(
      LPCWSTR lpCachedIniSection,
      LPCWSTR lpName,
      int iDefault)
{
  WCHAR *p = (WCHAR *)lpCachedIniSection;
  WCHAR tch[256];
  int  ich;
  int  i;

  if (p) {
    lstrcpy(tch,lpName);
    lstrcat(tch,L"=");
    ich = lstrlen(tch);

    while (*p) {
      if (StrCmpNI(p,tch,ich) == 0) {
        if (swscanf(p + ich,L"%i",&i) == 1)
          return(i);
        else
          return(iDefault);
      }
      else
        p = StrEnd(p) + 1;
    }
  }
  return(iDefault);
}


BOOL IniSectionSetString(LPWSTR lpCachedIniSection,LPCWSTR lpName,LPCWSTR lpString)
{
  WCHAR tch[256];
  WCHAR *p = lpCachedIniSection;

  if (p) {
    while (*p) {
      p = StrEnd(p) + 1;
    }
    wsprintf(tch,L"%s=%s",lpName,lpString);
    lstrcpy(p,tch);
    p = StrEnd(p) + 1;
    *p = 0;
    return(TRUE);
  }
  return(FALSE);
}


//=============================================================================
//
//  BeginWaitCursor()
//
void BeginWaitCursor()
{

  DestroyCursor(
    SetCursor(
    LoadCursor(NULL,IDC_WAIT)));

}


//=============================================================================
//
//  EndWaitCursor()
//
void EndWaitCursor()
{

  DestroyCursor(
    SetCursor(
    LoadCursor(NULL,IDC_ARROW)));

}


//=============================================================================
//
//  ExeNameFromWnd()
//
DWORD WINAPI GetModuleFileNameExW(HANDLE,HMODULE,LPTSTR,DWORD);
BOOL WINAPI EnumProcessModules(HANDLE,HMODULE*,DWORD,LPDWORD);

BOOL ExeNameFromWnd(HWND hwnd,LPWSTR szExeName,int cchExeName)
{
  //HMODULE hPSAPI;
  DWORD dwProcessId;
  HANDLE hProcess;
  HMODULE hModule;
  DWORD cbNeeded = 0;
  //FARPROC fpCreateToolhelp32Snapshot;
  //FARPROC fpProcess32First;
  //FARPROC fpProcess32Next;
  //HANDLE hProcessList;
  //PROCESSENTRY32 pe;
  //BOOL bMoreEntries;
  //BOOL bFoundMatching = FALSE;

  /*if (IsWindowsNT())
  {
    if (hPSAPI = LoadLibrary(L"PSAPI"))
    {*/
      GetWindowThreadProcessId(hwnd,&dwProcessId);
      hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE,dwProcessId);
      /*GetProcAddress(hPSAPI,"EnumProcessModules")*/ EnumProcessModules(hProcess,&hModule,sizeof(HMODULE),&cbNeeded);
      /*GetProcAddress(hPSAPI,"GetModuleFileNameExW")*/ GetModuleFileNameExW(hProcess,hModule,szExeName,cchExeName);
      CloseHandle(hProcess);
      //FreeLibrary(hPSAPI);
      return TRUE;
    /*}
    else
      return FALSE;
  }

  else
  {
    fpCreateToolhelp32Snapshot = GetProcAddress(GetModuleHandle(L"Kernel32"),"CreateToolhelp32Snapshot");
    fpProcess32First = GetProcAddress(GetModuleHandle(L"Kernel32"),"Process32First");
    fpProcess32Next = GetProcAddress(GetModuleHandle(L"Kernel32"),"Process32Next");

    if (fpCreateToolhelp32Snapshot)
    {
      GetWindowThreadProcessId(hwnd,&dwProcessId);
      hProcessList = (HANDLE)fpCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
      pe.dwSize = sizeof(PROCESSENTRY32);
      bMoreEntries = fpProcess32First(hProcessList,&pe);

      while (bMoreEntries)
      {
        if (pe.th32ProcessID == dwProcessId)
        {
          bMoreEntries = FALSE;
          bFoundMatching = TRUE;
          lstrcpyn(szExeName,pe.szExeFile,cchExeName);
        }
        else
          bMoreEntries = fpProcess32Next(hProcessList,&pe);
      }
      CloseHandle(hProcessList);
      return (bFoundMatching);
    }
    else
      return FALSE;
  }*/
}


////=============================================================================
////
////  Is32bitExe()
////
//BOOL Is32bitExe(LPCWSTR lpszExeName)
//{
//  SHFILEINFO shfi;
//  DWORD dwExeType;
//  WCHAR tch[MAX_PATH];
//
//  if (PathIsLnkFile(lpszExeName) &&
//      PathGetLnkPath(lpszExeName,tch,COUNTOF(tch)))
//    dwExeType = SHGetFileInfo(tch,0,&shfi,sizeof(SHFILEINFO),SHGFI_EXETYPE);
//  else
//    dwExeType = SHGetFileInfo(lpszExeName,0,&shfi,sizeof(SHFILEINFO),SHGFI_EXETYPE);
//
//  return (HIWORD(dwExeType) && LOWORD(dwExeType)) ? TRUE : FALSE;
//}


//=============================================================================
//
//  PrivateIsAppThemed()
//
BOOL PrivateIsAppThemed()
{
  BOOL bIsAppThemed = FALSE;
  HMODULE hDll = LoadLibrary(L"uxtheme.dll");
  if (hDll)
  {
    FARPROC fp = GetProcAddress(hDll,"IsAppThemed");
    if (fp)
      bIsAppThemed = (BOOL)fp();
    FreeLibrary(hDll);
  }
  return bIsAppThemed;
}


//=============================================================================
//
//  SetExplorerTheme()
//
//BOOL SetExplorerTheme(HWND hwnd)
//{
//  HMODULE hModUxTheme;
//  FARPROC pfnSetWindowTheme;
//
//  if (IsVista()) {
//    if (hModUxTheme = GetModuleHandle(L"uxtheme")) {
//      pfnSetWindowTheme = GetProcAddress(hModUxTheme,"SetWindowTheme");
//
//      if (pfnSetWindowTheme)
//        return (S_OK == pfnSetWindowTheme(hwnd,L"Explorer",NULL));
//    }
//  }
//  return FALSE;
//}


//=============================================================================
//
//  SetTheme()
//
BOOL SetTheme(HWND hwnd,LPCWSTR lpszTheme)
{
  HMODULE hModUxTheme;
  FARPROC pfnSetWindowTheme;

  if (hModUxTheme = GetModuleHandle(L"uxtheme")) {
    pfnSetWindowTheme = GetProcAddress(hModUxTheme,"SetWindowTheme");

    if (pfnSetWindowTheme)
      return (S_OK == pfnSetWindowTheme(hwnd,lpszTheme,NULL));
  }
  return FALSE;
}


//=============================================================================
//
//  BitmapMergeAlpha()
//  Merge alpha channel into color channel
//
BOOL BitmapMergeAlpha(HBITMAP hbmp,COLORREF crDest)
{
  BITMAP bmp;
  if (GetObject(hbmp,sizeof(BITMAP),&bmp)) {

    if (bmp.bmBitsPixel == 32) {

      int x,y;
      RGBQUAD *prgba = bmp.bmBits;

      for (y = 0; y < bmp.bmHeight; y++) {
        for (x = 0; x < bmp.bmWidth; x++) {
          BYTE alpha = prgba[x].rgbReserved;
          prgba[x].rgbRed = ((prgba[x].rgbRed * alpha) + (GetRValue(crDest) * (255-alpha))) >> 8;
          prgba[x].rgbGreen = ((prgba[x].rgbGreen * alpha) + (GetGValue(crDest) * (255-alpha))) >> 8;
          prgba[x].rgbBlue = ((prgba[x].rgbBlue * alpha) + (GetBValue(crDest) * (255-alpha))) >> 8;
          prgba[x].rgbReserved = 0xFF;
        }
        prgba = (RGBQUAD*)((LPBYTE)prgba + bmp.bmWidthBytes);
      }
      return TRUE;
    }
  }
  return FALSE;
}


//=============================================================================
//
//  BitmapAlphaBlend()
//  Perform alpha blending to color channel only
//
BOOL BitmapAlphaBlend(HBITMAP hbmp,COLORREF crDest,BYTE alpha)
{
  BITMAP bmp;
  if (GetObject(hbmp,sizeof(BITMAP),&bmp)) {

    if (bmp.bmBitsPixel == 32) {

      int x,y;
      RGBQUAD *prgba = bmp.bmBits;

      for (y = 0; y < bmp.bmHeight; y++) {
        for (x = 0; x < bmp.bmWidth; x++) {
          prgba[x].rgbRed = ((prgba[x].rgbRed * alpha) + (GetRValue(crDest) * (255-alpha))) >> 8;
          prgba[x].rgbGreen = ((prgba[x].rgbGreen * alpha) + (GetGValue(crDest) * (255-alpha))) >> 8;
          prgba[x].rgbBlue = ((prgba[x].rgbBlue * alpha) + (GetBValue(crDest) * (255-alpha))) >> 8;
        }
        prgba = (RGBQUAD*)((LPBYTE)prgba + bmp.bmWidthBytes);
      }
      return TRUE;
    }
  }
  return FALSE;
}


//=============================================================================
//
//  BitmapGrayScale()
//  Gray scale color channel only
//
BOOL BitmapGrayScale(HBITMAP hbmp)
{
  BITMAP bmp;
  if (GetObject(hbmp,sizeof(BITMAP),&bmp)) {

    if (bmp.bmBitsPixel == 32) {

      int x,y;
      RGBQUAD *prgba = bmp.bmBits;

      for (y = 0; y < bmp.bmHeight; y++) {
        for (x = 0; x < bmp.bmWidth; x++) {
          prgba[x].rgbRed = prgba[x].rgbGreen = prgba[x].rgbBlue =
          (((BYTE)((prgba[x].rgbRed * 38 + prgba[x].rgbGreen * 75 + prgba[x].rgbBlue * 15) >> 7) * 0x80) + (0xD0 * (255-0x80))) >> 8;
        }
        prgba = (RGBQUAD*)((LPBYTE)prgba + bmp.bmWidthBytes);
      }
      return TRUE;
    }
  }
  return FALSE;
}


//=============================================================================
//
//  SetWindowPathTitle()
//
BOOL SetWindowPathTitle(HWND hwnd,LPCWSTR lpszFile)
{

  WCHAR szTitle[256];

  if (lstrlen(lpszFile))
  {

    if (!PathIsRoot(lpszFile))
    {
      SHFILEINFO shfi;
      SHGetFileInfo(lpszFile,0,&shfi,sizeof(SHFILEINFO),SHGFI_DISPLAYNAME);
      lstrcpy(szTitle,shfi.szDisplayName);
    }
    else
      lstrcpy(szTitle,lpszFile);
  }

  return SetWindowText(hwnd,szTitle);

}


//=============================================================================
//
//  CenterDlgInParent()
//
void CenterDlgInParent(HWND hDlg)
{

  RECT rcDlg;
  HWND hParent;
  RECT rcParent;
  MONITORINFO mi;
  HMONITOR hMonitor;

  int xMin, yMin, xMax, yMax, x, y;

  GetWindowRect(hDlg,&rcDlg);

  hParent = GetParent(hDlg);
  GetWindowRect(hParent,&rcParent);

  hMonitor = MonitorFromRect(&rcParent,MONITOR_DEFAULTTONEAREST);
  mi.cbSize = sizeof(mi);
  GetMonitorInfo(hMonitor,&mi);

  xMin = mi.rcWork.left;
  yMin = mi.rcWork.top;

  xMax = (mi.rcWork.right) - (rcDlg.right - rcDlg.left);
  yMax = (mi.rcWork.bottom) - (rcDlg.bottom - rcDlg.top);

  if ((rcParent.right - rcParent.left) - (rcDlg.right - rcDlg.left) > 20)
    x = rcParent.left + (((rcParent.right - rcParent.left) - (rcDlg.right - rcDlg.left)) / 2);
  else
    x = rcParent.left + 70;

  if ((rcParent.bottom - rcParent.top) - (rcDlg.bottom - rcDlg.top) > 20)
    y = rcParent.top  + (((rcParent.bottom - rcParent.top) - (rcDlg.bottom - rcDlg.top)) / 2);
  else
    y = rcParent.top + 60;

  SetWindowPos(hDlg,NULL,max(xMin,min(xMax,x)),max(yMin,min(yMax,y)),0,0,SWP_NOZORDER|SWP_NOSIZE);

}


//=============================================================================
//
//  MakeBitmapButton()
//
void MakeBitmapButton(HWND hwnd,int nCtlId,HINSTANCE hInstance,UINT uBmpId)
{
  HWND hwndCtl = GetDlgItem(hwnd,nCtlId);
  BITMAP bmp;
  BUTTON_IMAGELIST bi;
  HBITMAP hBmp = LoadImage(hInstance,MAKEINTRESOURCE(uBmpId),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
  GetObject(hBmp,sizeof(BITMAP),&bmp);
  bi.himl = ImageList_Create(bmp.bmWidth,bmp.bmHeight,ILC_COLOR32|ILC_MASK,1,0);
  ImageList_AddMasked(bi.himl,hBmp,CLR_DEFAULT);
  DeleteObject(hBmp);
  SetRect(&bi.margin,0,0,0,0);
  bi.uAlign = BUTTON_IMAGELIST_ALIGN_CENTER;
  SendMessage(hwndCtl,BCM_SETIMAGELIST,0,(LPARAM)&bi);
}


//=============================================================================
//
//  DeleteBitmapButton()
//
void DeleteBitmapButton(HWND hwnd,int nCtlId)
{
  HWND hwndCtl = GetDlgItem(hwnd,nCtlId);
  BUTTON_IMAGELIST bi;
  if (SendMessage(hwndCtl,BCM_GETIMAGELIST,0,(LPARAM)&bi))
    ImageList_Destroy(bi.himl);
}


//=============================================================================
//
//  SetWindowTransparentMode()
//
void SetWindowTransparentMode(HWND hwnd,BOOL bTransparentMode)
{
  FARPROC fp;
  int  iAlphaPercent;
  BYTE bAlpha;

  if (bTransparentMode) {
    if (fp = GetProcAddress(GetModuleHandle(L"User32"),"SetLayeredWindowAttributes")) {
      SetWindowLongPtr(hwnd,GWL_EXSTYLE,
        GetWindowLongPtr(hwnd,GWL_EXSTYLE) | WS_EX_LAYERED);

      // get opacity level from registry
      iAlphaPercent = IniGetInt(L"Settings2",L"OpacityLevel",75);
      if (iAlphaPercent < 0 || iAlphaPercent > 100)
        iAlphaPercent = 75;
      bAlpha = iAlphaPercent * 255 / 100;

      fp(hwnd,0,bAlpha,LWA_ALPHA);
    }
  }

  else
    SetWindowLongPtr(hwnd,GWL_EXSTYLE,
      GetWindowLongPtr(hwnd,GWL_EXSTYLE) & ~WS_EX_LAYERED);
}


//=============================================================================
//
//  StatusSetText()
//
BOOL StatusSetText(HWND hwnd,UINT nPart,LPCWSTR lpszText)
{

  UINT uFlags = (nPart == 255) ? nPart|SBT_NOBORDERS : nPart;
  return (BOOL)SendMessage(hwnd,SB_SETTEXT,uFlags,(LPARAM)lpszText);

}


//=============================================================================
//
//  Toolbar_Get/SetButtons()
//
int Toolbar_GetButtons(HWND hwnd,int cmdBase,LPWSTR lpszButtons,int cchButtons)
{
  WCHAR tchButtons[512];
  WCHAR tchItem[32];
  int i,c;
  TBBUTTON tbb;

  lstrcpy(tchButtons,L"");
  c = min(50,(int)SendMessage(hwnd,TB_BUTTONCOUNT,0,0));

  for (i = 0; i < c; i++) {
    SendMessage(hwnd,TB_GETBUTTON,(WPARAM)i,(LPARAM)&tbb);
    wsprintf(tchItem,L"%i ",
      (tbb.idCommand==0)?0:tbb.idCommand-cmdBase+1);
    lstrcat(tchButtons,tchItem);
  }
  TrimString(tchButtons);
  lstrcpyn(lpszButtons,tchButtons,cchButtons);
  return(c);
}

int Toolbar_SetButtons(HWND hwnd,int cmdBase,LPCWSTR lpszButtons,LPCTBBUTTON ptbb,int ctbb)
{
  WCHAR tchButtons[512];
  WCHAR *p;
  int i,c;
  int iCmd;

  ZeroMemory(tchButtons,COUNTOF(tchButtons)*sizeof(tchButtons[0]));
  lstrcpyn(tchButtons,lpszButtons,COUNTOF(tchButtons)-2);
  TrimString(tchButtons);
  while (p = StrStr(tchButtons,L"  "))
    MoveMemory((WCHAR*)p,(WCHAR*)p+1,(lstrlen(p) + 1) * sizeof(WCHAR));

  c = (int)SendMessage(hwnd,TB_BUTTONCOUNT,0,0);
  for (i = 0; i < c; i++)
    SendMessage(hwnd,TB_DELETEBUTTON,0,0);

  for (i = 0; i < COUNTOF(tchButtons); i++)
    if (tchButtons[i] == L' ') tchButtons[i] = 0;

  p = tchButtons;
  while (*p) {
    if (swscanf(p,L"%i",&iCmd) == 1) {
      iCmd = (iCmd==0)?0:iCmd+cmdBase-1;
      for (i = 0; i < ctbb; i++) {
        if (ptbb[i].idCommand == iCmd) {
          SendMessage(hwnd,TB_ADDBUTTONS,(WPARAM)1,(LPARAM)&ptbb[i]);
          break;
        }
      }
    }
    p = StrEnd(p)+1;
  }
  return((int)SendMessage(hwnd,TB_BUTTONCOUNT,0,0));
}


//=============================================================================
//
//  Toolbar_SetButtonImage()
//
void Toolbar_SetButtonImage(HWND hwnd,int idCommand,int iImage)
{
  TBBUTTONINFO tbbi;

  tbbi.cbSize = sizeof(TBBUTTONINFO);
  tbbi.dwMask = TBIF_IMAGE;
  tbbi.iImage = iImage;

  SendMessage(hwnd,TB_SETBUTTONINFO,(WPARAM)idCommand,(LPARAM)&tbbi);
}


//=============================================================================
//
//  SendWMSize()
//
LRESULT SendWMSize(HWND hwnd)
{
  RECT rc; GetClientRect(hwnd,&rc);
  return(SendMessage(hwnd,WM_SIZE,SIZE_RESTORED,
         MAKELPARAM(rc.right,rc.bottom)));
}


//=============================================================================
//
//  FormatString()
//
int FormatString(LPWSTR lpOutput,int nOutput,UINT uIdFormat,...)
{

  WCHAR *p = LocalAlloc(LPTR,sizeof(WCHAR)*nOutput);

  if (GetString(uIdFormat,p,nOutput))
    wvsprintf(lpOutput,p,(LPVOID)((PUINT_PTR)&uIdFormat+1));

  LocalFree(p);

  return lstrlen(lpOutput);

}


//=============================================================================
//
//  PathRelativeToApp()
//
void PathRelativeToApp(
  LPWSTR lpszSrc,LPWSTR lpszDest,int cchDest,BOOL bSrcIsFile,
  BOOL bUnexpandEnv,BOOL bUnexpandMyDocs) {

  WCHAR wchAppPath[MAX_PATH];
  WCHAR wchWinDir[MAX_PATH];
  WCHAR wchUserFiles[MAX_PATH];
  WCHAR wchPath[MAX_PATH];
  WCHAR wchResult[MAX_PATH];
  DWORD dwAttrTo = (bSrcIsFile) ? 0 : FILE_ATTRIBUTE_DIRECTORY;

  GetModuleFileName(NULL,wchAppPath,COUNTOF(wchAppPath));
  PathRemoveFileSpec(wchAppPath);
  GetWindowsDirectory(wchWinDir,COUNTOF(wchWinDir));
  SHGetFolderPath(NULL,CSIDL_PERSONAL,NULL,SHGFP_TYPE_CURRENT,wchUserFiles);

  if (bUnexpandMyDocs &&
      !PathIsRelative(lpszSrc) &&
      !PathIsPrefix(wchUserFiles,wchAppPath) &&
       PathIsPrefix(wchUserFiles,lpszSrc) &&
       PathRelativePathTo(wchPath,wchUserFiles,FILE_ATTRIBUTE_DIRECTORY,lpszSrc,dwAttrTo)) {
    lstrcpy(wchUserFiles,L"%CSIDL:MYDOCUMENTS%");
    PathAppend(wchUserFiles,wchPath);
    lstrcpy(wchPath,wchUserFiles);
  }
  else if (PathIsRelative(lpszSrc) || PathCommonPrefix(wchAppPath,wchWinDir,NULL))
    lstrcpyn(wchPath,lpszSrc,COUNTOF(wchPath));
  else {
    if (!PathRelativePathTo(wchPath,wchAppPath,FILE_ATTRIBUTE_DIRECTORY,lpszSrc,dwAttrTo))
      lstrcpyn(wchPath,lpszSrc,COUNTOF(wchPath));
  }

  if (bUnexpandEnv) {
    if (!PathUnExpandEnvStrings(wchPath,wchResult,COUNTOF(wchResult)))
      lstrcpyn(wchResult,wchPath,COUNTOF(wchResult));
  }
  else
    lstrcpyn(wchResult,wchPath,COUNTOF(wchResult));

  if (lpszDest == NULL || lpszSrc == lpszDest)
    lstrcpyn(lpszSrc,wchResult,(cchDest == 0) ? MAX_PATH : cchDest);
  else
    lstrcpyn(lpszDest,wchResult,(cchDest == 0) ? MAX_PATH : cchDest);
}


//=============================================================================
//
//  PathAbsoluteFromApp()
//
void PathAbsoluteFromApp(LPWSTR lpszSrc,LPWSTR lpszDest,int cchDest,BOOL bExpandEnv) {

  WCHAR wchPath[MAX_PATH];
  WCHAR wchResult[MAX_PATH];

  if (StrCmpNI(lpszSrc,L"%CSIDL:MYDOCUMENTS%",COUNTOF("%CSIDL:MYDOCUMENTS%")-1) == 0) {
    SHGetFolderPath(NULL,CSIDL_PERSONAL,NULL,SHGFP_TYPE_CURRENT,wchPath);
    PathAppend(wchPath,lpszSrc+COUNTOF("%CSIDL:MYDOCUMENTS%")-1);
  }
  else
    lstrcpyn(wchPath,lpszSrc,COUNTOF(wchPath));

  if (bExpandEnv)
    ExpandEnvironmentStringsEx(wchPath,COUNTOF(wchPath));

  if (PathIsRelative(wchPath)) {
    GetModuleFileName(NULL,wchResult,COUNTOF(wchResult));
    PathRemoveFileSpec(wchResult);
    PathAppend(wchResult,wchPath);
  }
  else
    lstrcpyn(wchResult,wchPath,COUNTOF(wchResult));

  PathCanonicalizeEx(wchResult);
  if (PathGetDriveNumber(wchResult) != -1)
    CharUpperBuff(wchResult,1);

  if (lpszDest == NULL || lpszSrc == lpszDest)
    lstrcpyn(lpszSrc,wchResult,(cchDest == 0) ? MAX_PATH : cchDest);
  else
    lstrcpyn(lpszDest,wchResult,(cchDest == 0) ? MAX_PATH : cchDest);
}


///////////////////////////////////////////////////////////////////////////////
//
//
//  Name: PathIsLnkFile()
//
//  Purpose: Determine wheter pszPath is a Windows Shell Link File by
//           comparing the filename extension with L".lnk"
//
//  Manipulates:
//
BOOL PathIsLnkFile(LPCWSTR pszPath)
{

  //WCHAR *pszExt;

  WCHAR tchResPath[256];

  if (!pszPath || !*pszPath)
    return FALSE;

/*pszExt = strrchr(pszPath,L'.');

  if (!pszExt)
    return FALSE;

  if (!lstrcmpi(pszExt,L".lnk"))
    return TRUE;

  else
    return FALSE;*/

  //if (!lstrcmpi(PathFindExtension(pszPath),L".lnk"))
  //  return TRUE;

  //else
  //  return FALSE;

  if (lstrcmpi(PathFindExtension(pszPath),L".lnk"))
    return FALSE;

  else
    return PathGetLnkPath(pszPath,tchResPath,COUNTOF(tchResPath));

}


///////////////////////////////////////////////////////////////////////////////
//
//
//  Name: PathGetLnkPath()
//
//  Purpose: Try to get the path to which a lnk-file is linked
//
//
//  Manipulates: pszResPath
//
BOOL PathGetLnkPath(LPCWSTR pszLnkFile,LPWSTR pszResPath,int cchResPath)
{

  IShellLink       *psl;
  WIN32_FIND_DATA  fd;
  BOOL             bSucceeded = FALSE;

  if (SUCCEEDED(CoCreateInstance(&CLSID_ShellLink,NULL,
                                 CLSCTX_INPROC_SERVER,
                                 &IID_IShellLink,&psl)))
  {
    IPersistFile *ppf;

    if (SUCCEEDED(psl->lpVtbl->QueryInterface(psl,&IID_IPersistFile,&ppf)))
    {
      WORD wsz[MAX_PATH];

      /*MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,
                          pszLnkFile,-1,wsz,MAX_PATH);*/
      lstrcpy(wsz,pszLnkFile);

      if (SUCCEEDED(ppf->lpVtbl->Load(ppf,wsz,STGM_READ)))
      {
        if (NOERROR == psl->lpVtbl->GetPath(psl,pszResPath,cchResPath,&fd,0))
          bSucceeded = TRUE;
      }
      ppf->lpVtbl->Release(ppf);
    }
    psl->lpVtbl->Release(psl);
  }

  // This additional check seems reasonable
  if (!lstrlen(pszResPath))
    bSucceeded = FALSE;

  if (bSucceeded)
    ExpandEnvironmentStringsEx(pszResPath,cchResPath);

  return(bSucceeded);

}


///////////////////////////////////////////////////////////////////////////////
//
//
//  Name: PathIsLnkToDirectory()
//
//  Purpose: Determine wheter pszPath is a Windows Shell Link File which
//           refers to a directory
//
//  Manipulates: pszResPath
//
BOOL PathIsLnkToDirectory(LPCWSTR pszPath,LPWSTR pszResPath,int cchResPath)
{

  WCHAR tchResPath[MAX_PATH];

  if (PathIsLnkFile(pszPath)) {

    if (PathGetLnkPath(pszPath,tchResPath,sizeof(WCHAR)*COUNTOF(tchResPath))) {

      if (PathIsDirectory(tchResPath)) {

        lstrcpyn(pszResPath,tchResPath,cchResPath);
        return (TRUE);
      }

      else
        return FALSE;
      }

    else
      return FALSE;
    }

  else
    return FALSE;

}


///////////////////////////////////////////////////////////////////////////////
//
//
//  Name: PathCreateLnk()
//
//  Purpose: Try to create a lnk-file in the specified directory referring
//           to the specified file or directory
//
//  Manipulates:
//
BOOL PathCreateLnk(LPCWSTR pszLnkDir,LPCWSTR pszPath)
{

  WCHAR tchLnkFileName[MAX_PATH];

  IShellLink *psl;
  BOOL bSucceeded = FALSE;
  BOOL fMustCopy;

  // Try to construct a valid filename...
  if (!SHGetNewLinkInfo(pszPath,pszLnkDir,tchLnkFileName,&fMustCopy,SHGNLI_PREFIXNAME))
    return(FALSE);

  if (SUCCEEDED(CoCreateInstance(&CLSID_ShellLink,NULL,
                                 CLSCTX_INPROC_SERVER,
                                 &IID_IShellLink,&psl)))
  {
    IPersistFile *ppf;

    if (SUCCEEDED(psl->lpVtbl->QueryInterface(psl,&IID_IPersistFile,&ppf)))
    {
      WORD wsz[MAX_PATH];

      /*MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,
                          tchLnkFileName,-1,wsz,MAX_PATH);*/
      lstrcpy(wsz,tchLnkFileName);

      if (NOERROR == psl->lpVtbl->SetPath(psl,pszPath) &&
          SUCCEEDED(ppf->lpVtbl->Save(ppf,wsz,TRUE)))
        bSucceeded = TRUE;

      ppf->lpVtbl->Release(ppf);
    }
    psl->lpVtbl->Release(psl);
  }

  return(bSucceeded);

}


//=============================================================================
//
//  TrimString()
//
BOOL TrimString(LPWSTR lpString)
{

  LPWSTR psz;

  if (!lpString || !*lpString)
    return FALSE;

  // Trim left
  psz = lpString;

  while (*psz == L' ')
    psz = CharNext(psz);

  MoveMemory(lpString,psz,sizeof(WCHAR)*(lstrlen(psz) + 1));

  // Trim right
  psz = StrEnd(lpString);

  while (*(psz = CharPrev(lpString,psz)) == L' ')
    *psz = L'\0';

  return TRUE;

}


//=============================================================================
//
//  ExtractFirstArgument()
//
BOOL ExtractFirstArgument(LPCWSTR lpArgs,LPWSTR lpArg1,LPWSTR lpArg2)
{

  LPWSTR psz;
  BOOL bQuoted = FALSE;

  lstrcpy(lpArg1,lpArgs);
  if (lpArg2)
    *lpArg2 = L'\0';

  if (!TrimString(lpArg1))
    return FALSE;

  if (*lpArg1 == L'\"')
  {
    *lpArg1 = L' ';
    TrimString(lpArg1);
    bQuoted = TRUE;
  }

  if (bQuoted)
    psz = StrChr(lpArg1,L'\"');
  else
    psz = StrChr(lpArg1,L' ');;

  if (psz)
  {
    *psz = L'\0';
    if (lpArg2)
      lstrcpy(lpArg2,psz + 1);
  }

  TrimString(lpArg1);

  if (lpArg2)
    TrimString(lpArg2);

  return TRUE;

}


//=============================================================================
//
//  QuotateFilenameStr()
//
LPWSTR QuotateFilenameStr(LPWSTR lpFile)
{
  if (StrChr(lpFile,L' '))
  {
    MoveMemory(lpFile + 1,lpFile,sizeof(WCHAR)*(lstrlen(lpFile) + 1));
    *lpFile = '\"';
    lstrcat(lpFile,L"\"");
  }
  return lpFile;
}


//=============================================================================
//
//  GetFilenameStr()
//
LPWSTR GetFilenameStr(LPWSTR lpFile)
{
  LPWSTR psz = StrEnd(lpFile);

  while (psz != lpFile && *CharPrev(lpFile,psz) != L'\\')
    psz = CharPrev(lpFile,psz);

  return psz;
}


//=============================================================================
//
//  PrepareFilterStr()
//
void PrepareFilterStr(LPWSTR lpFilter)
{
  LPWSTR psz = StrEnd(lpFilter);
  while (psz != lpFilter)
  {
    if (*(psz = CharPrev(lpFilter,psz)) == L'\n')
      *psz = L'\0';
  }
}


//=============================================================================
//
//  StrTab2Space() - in place conversion
//
void StrTab2Space(LPWSTR lpsz)
{
  WCHAR *c = lpsz;
  while (c = StrChr(lpsz,L'\t'))
    *c = L' ';
}


//=============================================================================
//
//  ExpandEnvironmentStringsEx()
//
//  Adjusted for Windows 95
//
void ExpandEnvironmentStringsEx(LPWSTR lpSrc,DWORD dwSrc)
{
  WCHAR szBuf[312];

  if (ExpandEnvironmentStrings(lpSrc,szBuf,COUNTOF(szBuf)))
    lstrcpyn(lpSrc,szBuf,dwSrc);
}


//=============================================================================
//
//  PathCanonicalizeEx()
//
//
void PathCanonicalizeEx(LPWSTR lpSrc)
{
  WCHAR szDst[MAX_PATH];

  if (PathCanonicalize(szDst,lpSrc))
    lstrcpy(lpSrc,szDst);
}


//=============================================================================
//
//  SearchPathEx()
//
//  This Expansion also searches the L"Favorites" folder
//
extern WCHAR tchFavoritesDir[MAX_PATH];
extern WCHAR szCurDir[MAX_PATH];

DWORD SearchPathEx(LPCWSTR lpPath,LPCWSTR lpFileName,LPCWSTR lpExtension,
                   DWORD nBufferLength,LPWSTR lpBuffer,LPWSTR *lpFilePart)
{
  DWORD dwRetVal = 0;

  if (lstrcmp(lpFileName,L"..") == 0 || lstrcmp(lpFileName,L".") == 0)
  {
    if (lstrcmp(lpFileName,L"..") == 0 && PathIsRoot(szCurDir)) {
      lstrcpyn(lpBuffer,L"*.*",nBufferLength);
      dwRetVal = 1;
    }
    else
      dwRetVal = SearchPath(szCurDir,lpFileName,lpExtension,
                            nBufferLength,lpBuffer,lpFilePart);
  }

  else
  {
    dwRetVal = SearchPath(lpPath,lpFileName,lpExtension,
                          nBufferLength,lpBuffer,lpFilePart);

    // Search L"Favorites" if no result
    if (!dwRetVal)
      dwRetVal = SearchPath(tchFavoritesDir,lpFileName,lpExtension,
                            nBufferLength,lpBuffer,lpFilePart);
  }

  return(dwRetVal);
}


//=============================================================================
//
//  FormatNumberStr()
//
int FormatNumberStr(LPWSTR lpNumberStr)
{
  WCHAR *c;
  WCHAR szSep[8];
  int  i = 0;

  if (!lstrlen(lpNumberStr))
    return(0);

  if (!GetLocaleInfo(LOCALE_USER_DEFAULT,
                     LOCALE_STHOUSAND,
                     szSep,
                     COUNTOF(szSep)))
    szSep[0] = L'\'';

  c = StrEnd(lpNumberStr);

  while ((c = CharPrev(lpNumberStr,c)) != lpNumberStr)
  {
    if (++i == 3)
    {
      i = 0;
      MoveMemory(c+1,c,sizeof(WCHAR)*(lstrlen(c)+1));
      *c = szSep[0];
    }
  }

  return(lstrlen(lpNumberStr));
}


//=============================================================================
//
//  GetDefaultFavoritesDir()
//
void GetDefaultFavoritesDir(LPWSTR lpFavDir,int cchFavDir)
{

  LPITEMIDLIST pidl;

  if (NOERROR == SHGetSpecialFolderLocation(
                   NULL,CSIDL_PERSONAL,&pidl))
  {
    SHGetPathFromIDList(pidl,lpFavDir);
    CoTaskMemFree(pidl);
  }

  else
    GetWindowsDirectory(lpFavDir,cchFavDir);

}


//=============================================================================
//
//  GetDefaultOpenWithDir()
//
void GetDefaultOpenWithDir(LPWSTR lpOpenWithDir,int cchOpenWithDir)
{

  LPITEMIDLIST pidl;

  if (NOERROR == SHGetSpecialFolderLocation(
                   NULL,CSIDL_DESKTOPDIRECTORY,&pidl))
  {
    SHGetPathFromIDList(pidl,lpOpenWithDir);
    CoTaskMemFree(pidl);
  }

  else
    GetWindowsDirectory(lpOpenWithDir,cchOpenWithDir);

}


//=============================================================================
//
//  CreateDropHandle()
//
//  Creates a HDROP to generate a WM_DROPFILES message
//
//
HDROP CreateDropHandle(LPCWSTR lpFileName)
{
  LPDROPFILES  lpdf;
  HGLOBAL      hDrop;

  hDrop = GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE,
            sizeof(DROPFILES) + sizeof(WCHAR)*(lstrlen(lpFileName) + 2));

  lpdf = GlobalLock(hDrop);

  lpdf->pFiles = sizeof(DROPFILES);
  lpdf->pt.x   = 0;
  lpdf->pt.y   = 0;
  lpdf->fNC    = TRUE;
  lpdf->fWide  = TRUE;

  lstrcpy((WCHAR*)&lpdf[1],lpFileName);

  GlobalUnlock(hDrop);

  return(hDrop);
}


//=============================================================================
//
//  DirList_IsFileSelected()
//
//  Check if a file is selected in the DirList
//
//
BOOL DirList_IsFileSelected(HWND hwnd)
{
  int i;
  DLITEM dli;

  i = (ListView_GetSelectedCount(hwnd));
  dli.mask = DLI_TYPE;
  dli.ntype = DLE_NONE;
  DirList_GetItem(hwnd,-1,&dli);

  return (i && dli.ntype == DLE_FILE);
}


//=============================================================================
//
//  ExecDDECommand()
//
//  Execute a DDE command (Msg,App,Topic)
//
//

HDDEDATA CALLBACK DdeCallback(UINT uType, UINT uFmt, HCONV hconv, HSZ hsz1, HSZ hsz2,
                HDDEDATA hdata, ULONG_PTR  dwData1, ULONG_PTR  dwData2)
{
  switch (uType)
  {
    case XTYP_ADVDATA:
        return (HDDEDATA)DDE_FACK;

    default:
        return (HDDEDATA)NULL;
  }
}

BOOL ExecDDECommand(LPCWSTR lpszCmdLine,
  LPCWSTR lpszDDEMsg,LPCWSTR lpszDDEApp,LPCWSTR lpszDDETopic)
{

  WCHAR  lpszURLExec[512];
  WCHAR  lpszDDEMsgBuf[256];
  WCHAR  *pSubst;
  DWORD idInst=0;
  HSZ hszService, hszTopic;
  HCONV hConv;
  BOOL bSuccess = TRUE;

  if (lstrlen(lpszCmdLine) == 0 || lstrlen(lpszDDEMsg) == 0 ||
      lstrlen(lpszDDEApp) == 0 || lstrlen(lpszDDETopic) == 0)
    return FALSE;

  lstrcpyn(lpszDDEMsgBuf,lpszDDEMsg,COUNTOF(lpszDDEMsgBuf));
  if (pSubst = StrStr(lpszDDEMsgBuf,L"%1"))
    *(pSubst+1) = L's';

  wsprintf(lpszURLExec,lpszDDEMsgBuf,lpszCmdLine);

  if (DdeInitialize(&idInst, DdeCallback,
        APPCLASS_STANDARD | APPCMD_CLIENTONLY, 0L) == DMLERR_NO_ERROR)
  {
    hszService = DdeCreateStringHandle(idInst, lpszDDEApp, CP_WINUNICODE);
    hszTopic = DdeCreateStringHandle(idInst, lpszDDETopic, CP_WINUNICODE);
    if (hszService && hszTopic)
    {
      hConv = DdeConnect(idInst, hszService, hszTopic, NULL);
      if (hConv)
      {
        DdeClientTransaction((LPBYTE)lpszURLExec,sizeof(WCHAR)*(lstrlen(lpszURLExec) + 1),
          hConv, 0, 0, XTYP_EXECUTE, TIMEOUT_ASYNC, NULL);
        DdeDisconnect(hConv);
      }
      else
        bSuccess = FALSE;
    }

    if (hszTopic)
      DdeFreeStringHandle(idInst, hszTopic);
    if (hszService)
      DdeFreeStringHandle(idInst, hszService);
    DdeUninitialize(idInst);
  }

  return bSuccess;
}


//=============================================================================
//
//  History Functions
//
//
BOOL History_Init(PHISTORY ph)
{
  if (!ph)
    return FALSE;

  ZeroMemory(ph,sizeof(HISTORY));
  ph->iCurItem = (-1);

  return TRUE;
}

BOOL History_Uninit(PHISTORY ph)
{
  int i;

  if (!ph)
    return FALSE;

  for (i = 0; i < HISTORY_ITEMS; i++)
  {
    if (ph->psz[i])
      LocalFree(ph->psz[i]);
    ph->psz[i] = NULL;
  }

  return TRUE;
}

BOOL History_Add(PHISTORY ph,LPCWSTR pszNew)
{
  int i;

  if (!ph)
    return FALSE;

  // Item to be added is equal to current item
  if (ph->iCurItem >= 0 && ph->iCurItem <= (HISTORY_ITEMS-1))
    if (!lstrcmpi(pszNew,ph->psz[ph->iCurItem]))
      return FALSE;

  if (ph->iCurItem == (HISTORY_ITEMS-1))
  {
    // Shift
    if (ph->psz[0])
      LocalFree(ph->psz[0]);

    MoveMemory(ph->psz,ph->psz+1,(HISTORY_ITEMS-1)*sizeof(WCHAR*));
  }

  else
  {
    ph->iCurItem++;
    for (i = ph->iCurItem; i < HISTORY_ITEMS; i++)
    {
      if (ph->psz[i])
        LocalFree(ph->psz[i]);
      ph->psz[i] = NULL;
    }
  }

  ph->psz[ph->iCurItem] = StrDup(pszNew);

  return TRUE;
}

BOOL History_Forward(PHISTORY ph,LPWSTR pszItem,int cItem)
{
  if (!ph)
    return FALSE;

  if (ph->iCurItem < (HISTORY_ITEMS-1))
    if (ph->psz[ph->iCurItem+1])
    {
      ph->iCurItem++;
      lstrcpyn(pszItem,ph->psz[ph->iCurItem],cItem);
      return TRUE;
    }

  return FALSE;
}

BOOL History_Back(PHISTORY ph,LPWSTR pszItem,int cItem)
{
  if (!ph)
    return FALSE;

  if (ph->iCurItem > 0)
    if (ph->psz[ph->iCurItem-1])
    {
      ph->iCurItem--;
      lstrcpyn(pszItem,ph->psz[ph->iCurItem],cItem);
      return TRUE;
    }

  return FALSE;
}

BOOL History_CanForward(PHISTORY ph)
{
  if (!ph)
    return FALSE;

  if (ph->iCurItem < (HISTORY_ITEMS-1))
    if (ph->psz[ph->iCurItem+1])
      return TRUE;

  return FALSE;
}

BOOL History_CanBack(PHISTORY ph)
{
  if (!ph)
    return FALSE;

  if (ph->iCurItem > 0)
    if (ph->psz[ph->iCurItem-1])
      return TRUE;

  return FALSE;
}

void History_UpdateToolbar(PHISTORY ph,HWND hwnd,int cmdBack,int cmdForward)
{
  if (History_CanBack(ph))
    SendMessage(hwnd,TB_ENABLEBUTTON,cmdBack,MAKELONG(1,0));
  else
    SendMessage(hwnd,TB_ENABLEBUTTON,cmdBack,MAKELONG(0,0));

  if (History_CanForward(ph))
    SendMessage(hwnd,TB_ENABLEBUTTON,cmdForward,MAKELONG(1,0));
  else
    SendMessage(hwnd,TB_ENABLEBUTTON,cmdForward,MAKELONG(0,0));
}


//=============================================================================
//
//  MRU functions
//
LPMRULIST MRU_Create(LPCWSTR pszRegKey,int iFlags,int iSize) {

  LPMRULIST pmru = LocalAlloc(LPTR,sizeof(MRULIST));
  ZeroMemory(pmru,sizeof(MRULIST));
  lstrcpyn(pmru->szRegKey,pszRegKey,COUNTOF(pmru->szRegKey));
  pmru->iFlags = iFlags;
  pmru->iSize = min(iSize,MRU_MAXITEMS);
  return(pmru);
}

BOOL MRU_Destroy(LPMRULIST pmru) {

  int i;
  for (i = 0; i < pmru->iSize; i++) {
    if (pmru->pszItems[i])
      LocalFree(pmru->pszItems[i]);
    }
  ZeroMemory(pmru,sizeof(MRULIST));
  LocalFree(pmru);
  return(1);
}

int MRU_Compare(LPMRULIST pmru,LPCWSTR psz1,LPCWSTR psz2) {

  if (pmru->iFlags & MRU_NOCASE)
    return(lstrcmpi(psz1,psz2));
  else
    return(lstrcmp(psz1,psz2));
}

BOOL MRU_Add(LPMRULIST pmru,LPCWSTR pszNew) {

  int i;
  for (i = 0; i < pmru->iSize; i++) {
    if (MRU_Compare(pmru,pmru->pszItems[i],pszNew) == 0) {
      LocalFree(pmru->pszItems[i]);
      break;
    }
  }
  i = min(i,pmru->iSize-1);
  for (; i > 0; i--)
    pmru->pszItems[i] = pmru->pszItems[i-1];
  pmru->pszItems[0] = StrDup(pszNew);
  return(1);
}

BOOL MRU_Delete(LPMRULIST pmru,int iIndex) {

  int i;
  if (iIndex < 0 || iIndex > pmru->iSize-1)
    return(0);
  if (pmru->pszItems[iIndex])
    LocalFree(pmru->pszItems[iIndex]);
  for (i = iIndex; i < pmru->iSize-1; i++) {
    pmru->pszItems[i] = pmru->pszItems[i+1];
    pmru->pszItems[i+1] = NULL;
  }
  return(1);
}

BOOL MRU_Empty(LPMRULIST pmru) {

  int i;
  for (i = 0; i < pmru->iSize; i++) {
    if (pmru->pszItems[i]) {
      LocalFree(pmru->pszItems[i]);
      pmru->pszItems[i] = NULL;
    }
  }
  return(1);
}

int MRU_Enum(LPMRULIST pmru,int iIndex,LPWSTR pszItem,int cchItem) {

  if (pszItem == NULL || cchItem == 0) {
    int i = 0;
    while (pmru->pszItems[i] && i < pmru->iSize)
      i++;
    return(i);
  }
  else {
    if (iIndex < 0 || iIndex > pmru->iSize-1 || !pmru->pszItems[iIndex])
      return(-1);
    else {
      lstrcpyn(pszItem,pmru->pszItems[iIndex],cchItem);
      return(lstrlen(pszItem));
    }
  }
}

BOOL MRU_Load(LPMRULIST pmru) {

  int i,n = 0;
  WCHAR tchName[32];
  WCHAR tchItem[1024];
  WCHAR *pIniSection = LocalAlloc(LPTR,sizeof(WCHAR)*32*1024);

  MRU_Empty(pmru);
  LoadIniSection(pmru->szRegKey,pIniSection,(int)LocalSize(pIniSection)/sizeof(WCHAR));

  for (i = 0; i < pmru->iSize; i++) {
    wsprintf(tchName,L"%.2i",i+1);
    if (IniSectionGetString(pIniSection,tchName,L"",tchItem,COUNTOF(tchItem))) {
      /*if (pmru->iFlags & MRU_UTF8) {
        WCHAR wchItem[1024];
        int cbw = MultiByteToWideChar(CP_UTF7,0,tchItem,-1,wchItem,COUNTOF(wchItem));
        WideCharToMultiByte(CP_UTF8,0,wchItem,cbw,tchItem,COUNTOF(tchItem),NULL,NULL);
        pmru->pszItems[n++] = StrDup(tchItem);
      }
      else*/
        pmru->pszItems[n++] = StrDup(tchItem);
    }
  }
  LocalFree(pIniSection);
  return(1);
}

BOOL MRU_Save(LPMRULIST pmru) {

  int i;
  WCHAR tchName[32];
  WCHAR *pIniSection = LocalAlloc(LPTR,sizeof(WCHAR)*32*1024);

  //IniDeleteSection(pmru->szRegKey);

  for (i = 0; i < pmru->iSize; i++) {
    if (pmru->pszItems[i]) {
      wsprintf(tchName,L"%.2i",i+1);
      /*if (pmru->iFlags & MRU_UTF8) {
        WCHAR  tchItem[1024];
        WCHAR wchItem[1024];
        int cbw = MultiByteToWideChar(CP_UTF8,0,pmru->pszItems[i],-1,wchItem,COUNTOF(wchItem));
        WideCharToMultiByte(CP_UTF7,0,wchItem,cbw,tchItem,COUNTOF(tchItem),NULL,NULL);
        IniSectionSetString(pIniSection,tchName,tchItem);
      }
      else*/
        IniSectionSetString(pIniSection,tchName,pmru->pszItems[i]);
    }
  }
  SaveIniSection(pmru->szRegKey,pIniSection);
  LocalFree(pIniSection);
  return(1);
}

void MRU_LoadToCombobox(HWND hwnd,LPCWSTR pszKey)
{
  int i;
  WCHAR tch[MAX_PATH];
  LPMRULIST pmru = MRU_Create(pszKey,MRU_NOCASE,8);
  MRU_Load(pmru);
  for (i = 0; i < MRU_Enum(pmru,0,NULL,0); i++) {
    MRU_Enum(pmru,i,tch,COUNTOF(tch));
    SendMessage(hwnd,CB_ADDSTRING,0,(LPARAM)tch);
  }
  MRU_Destroy(pmru);
}


void MRU_AddOneItem(LPCWSTR pszKey,LPCWSTR pszNewItem)
{
  if (lstrlen(pszNewItem) > 0) {
    LPMRULIST pmru = MRU_Create(pszKey,MRU_NOCASE,8);
    MRU_Load(pmru);
    MRU_Add(pmru,pszNewItem);
    MRU_Save(pmru);
    MRU_Destroy(pmru);
  }
}


/*

  Themed Dialogs
  Modify dialog templates to use current theme font
  Based on code of MFC helper class CDialogTemplate

*/

BOOL GetThemedDialogFont(LPWSTR lpFaceName,WORD* wSize)
{
  HDC hDC;
  int iLogPixelsY;
  HMODULE hModUxTheme;
  HTHEME hTheme;
  LOGFONT lf;
  BOOL bSucceed = FALSE;

  hDC = GetDC(NULL);
  iLogPixelsY = GetDeviceCaps(hDC,LOGPIXELSY);
  ReleaseDC(NULL,hDC);

  if (hModUxTheme = GetModuleHandle(L"uxtheme.dll")) {
    if ((BOOL)(GetProcAddress(hModUxTheme,"IsAppThemed"))()) {
      hTheme = (HTHEME)(INT_PTR)(GetProcAddress(hModUxTheme,"OpenThemeData"))(NULL,L"WINDOWSTYLE;WINDOW");
      if (hTheme) {
        if (S_OK == (HRESULT)(GetProcAddress(hModUxTheme,"GetThemeSysFont"))(hTheme,/*TMT_MSGBOXFONT*/805,&lf)) {
          if (lf.lfHeight < 0)
            lf.lfHeight = -lf.lfHeight;
          *wSize = (WORD)MulDiv(lf.lfHeight,72,iLogPixelsY);
          if (*wSize == 0)
            *wSize = 8;
          StrCpyN(lpFaceName,lf.lfFaceName,LF_FACESIZE);
          bSucceed = TRUE;
        }
        (GetProcAddress(hModUxTheme,"CloseThemeData"))(hTheme);
      }
    }
  }

  /*
  if (!bSucceed) {
    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(NONCLIENTMETRICS),&ncm,0);
    if (ncm.lfMessageFont.lfHeight < 0)
      ncm.lfMessageFont.lfHeight = -ncm.lfMessageFont.lfHeight;
    *wSize = (WORD)MulDiv(ncm.lfMessageFont.lfHeight,72,iLogPixelsY);
    if (*wSize == 0)
      *wSize = 8;
    StrCpyN(lpFaceName,ncm.lfMessageFont.lfFaceName,LF_FACESIZE);
  }*/

  return(bSucceed);
}

static BOOL DialogTemplate_IsDialogEx(const DLGTEMPLATE* pTemplate) {

  return ((DLGTEMPLATEEX*)pTemplate)->signature == 0xFFFF;
}

static BOOL DialogTemplate_HasFont(const DLGTEMPLATE* pTemplate) {

  return (DS_SETFONT &
    (DialogTemplate_IsDialogEx(pTemplate) ? ((DLGTEMPLATEEX*)pTemplate)->style : pTemplate->style));
}

static int DialogTemplate_FontAttrSize(BOOL bDialogEx) {

  return (int)sizeof(WORD) * (bDialogEx ? 3 : 1);
}

static BYTE* DialogTemplate_GetFontSizeField(const DLGTEMPLATE* pTemplate) {

  BOOL bDialogEx = DialogTemplate_IsDialogEx(pTemplate);
  WORD* pw;

  if (bDialogEx)
    pw = (WORD*)((DLGTEMPLATEEX*)pTemplate + 1);
  else
    pw = (WORD*)(pTemplate + 1);

  if (*pw == (WORD)-1)
    pw += 2;
  else
    while(*pw++);

  if (*pw == (WORD)-1)
    pw += 2;
  else
    while(*pw++);

  while (*pw++);

  return (BYTE*)pw;
}

DLGTEMPLATE* LoadThemedDialogTemplate(LPCTSTR lpDialogTemplateID,HINSTANCE hInstance) {

  HRSRC hRsrc;
  HGLOBAL hRsrcMem;
  DLGTEMPLATE *pRsrcMem;
  DLGTEMPLATE *pTemplate;
  UINT dwTemplateSize = 0;
  WCHAR wchFaceName[LF_FACESIZE];
  WORD wFontSize;
  BOOL bDialogEx;
  BOOL bHasFont;
  int cbFontAttr;
  int cbNew;
  int cbOld;
  BYTE* pbNew;
  BYTE* pb;
  BYTE* pOldControls;
  BYTE* pNewControls;
  WORD nCtrl;

  hRsrc = FindResource(hInstance,lpDialogTemplateID,RT_DIALOG);
  if (hRsrc == NULL)
    return(NULL);

  hRsrcMem = LoadResource(hInstance,hRsrc);
  pRsrcMem = (DLGTEMPLATE*)LockResource(hRsrcMem);
  dwTemplateSize = (UINT)SizeofResource(hInstance,hRsrc);

  if ((dwTemplateSize == 0) ||
      (pTemplate = LocalAlloc(LPTR,dwTemplateSize+LF_FACESIZE*2)) == NULL) {
    UnlockResource(hRsrcMem);
    FreeResource(hRsrcMem);
    return(NULL);
  }

  CopyMemory((BYTE*)pTemplate,pRsrcMem,(size_t)dwTemplateSize);
  UnlockResource(hRsrcMem);
  FreeResource(hRsrcMem);

  if (!GetThemedDialogFont(wchFaceName,&wFontSize))
    return(pTemplate);

  bDialogEx  = DialogTemplate_IsDialogEx(pTemplate);
  bHasFont   = DialogTemplate_HasFont(pTemplate);
  cbFontAttr = DialogTemplate_FontAttrSize(bDialogEx);

  if (bDialogEx)
    ((DLGTEMPLATEEX*)pTemplate)->style |= DS_SHELLFONT;
  else
    pTemplate->style |= DS_SHELLFONT;

  cbNew = cbFontAttr + ((lstrlen(wchFaceName) + 1) * sizeof(WCHAR));
  pbNew = (BYTE*)wchFaceName;

  pb = DialogTemplate_GetFontSizeField(pTemplate);
  cbOld = (int)(bHasFont ? cbFontAttr + 2 * (lstrlen((WCHAR*)(pb + cbFontAttr)) + 1) : 0);

  pOldControls = (BYTE*)(((DWORD_PTR)pb + cbOld + 3) & ~(DWORD_PTR)3);
  pNewControls = (BYTE*)(((DWORD_PTR)pb + cbNew + 3) & ~(DWORD_PTR)3);

  nCtrl = bDialogEx ?
    (WORD)((DLGTEMPLATEEX*)pTemplate)->cDlgItems :
    (WORD)pTemplate->cdit;

  if (cbNew != cbOld && nCtrl > 0)
    MoveMemory(pNewControls,pOldControls,(size_t)(dwTemplateSize - (pOldControls - (BYTE*)pTemplate)));

  *(WORD*)pb = wFontSize;
  MoveMemory(pb + cbFontAttr,pbNew,(size_t)(cbNew - cbFontAttr));

  return(pTemplate);
}

INT_PTR ThemedDialogBoxParam(
  HINSTANCE hInstance,
  LPCTSTR lpTemplate,
  HWND hWndParent,
  DLGPROC lpDialogFunc,
  LPARAM dwInitParam) {

  INT_PTR ret;
  DLGTEMPLATE *pDlgTemplate;

  pDlgTemplate = LoadThemedDialogTemplate(lpTemplate,hInstance);
  ret = DialogBoxIndirectParam(hInstance,pDlgTemplate,hWndParent,lpDialogFunc,dwInitParam);
  if (pDlgTemplate)
    LocalFree(pDlgTemplate);

  return(ret);
}


/*

  MinimizeToTray - Copyright 2000 Matthew Ellis <m.t.ellis@bigfoot.com>

  Changes made by flo:
   - Commented out: #include "stdafx.h"
   - Moved variable declaration: APPBARDATA appBarData;
   - Declared GetDoAnimateMinimize() as non-static

*/

// MinimizeToTray
//
// A couple of routines to show how to make it produce a custom caption
// animation to make it look like we are minimizing to and maximizing
// from the system tray
//
// These routines are public domain, but it would be nice if you dropped
// me a line if you use them!
//
// 1.0 29.06.2000 Initial version
// 1.1 01.07.2000 The window retains it's place in the Z-order of windows
//     when minimized/hidden. This means that when restored/shown, it doen't
//     always appear as the foreground window unless we call SetForegroundWindow
//
// Copyright 2000 Matthew Ellis <m.t.ellis@bigfoot.com>
/*#include "stdafx.h"*/

// Odd. VC++6 winuser.h has IDANI_CAPTION defined (as well as IDANI_OPEN and
// IDANI_CLOSE), but the Platform SDK only has IDANI_OPEN...

// I don't know what IDANI_OPEN or IDANI_CLOSE do. Trying them in this code
// produces nothing. Perhaps they were intended for window opening and closing
// like the MAC provides...
#ifndef IDANI_OPEN
#define IDANI_OPEN 1
#endif
#ifndef IDANI_CLOSE
#define IDANI_CLOSE 2
#endif
#ifndef IDANI_CAPTION
#define IDANI_CAPTION 3
#endif

#define DEFAULT_RECT_WIDTH 150
#define DEFAULT_RECT_HEIGHT 30

// Returns the rect of where we think the system tray is. This will work for
// all current versions of the shell. If explorer isn't running, we try our
// best to work with a 3rd party shell. If we still can't find anything, we
// return a rect in the lower right hand corner of the screen
static VOID GetTrayWndRect(LPRECT lpTrayRect)
{
  APPBARDATA appBarData;
  // First, we'll use a quick hack method. We know that the taskbar is a window
  // of class Shell_TrayWnd, and the status tray is a child of this of class
  // TrayNotifyWnd. This provides us a window rect to minimize to. Note, however,
  // that this is not guaranteed to work on future versions of the shell. If we
  // use this method, make sure we have a backup!
  HWND hShellTrayWnd=FindWindowEx(NULL,NULL,TEXT("Shell_TrayWnd"),NULL);
  if(hShellTrayWnd)
  {
    HWND hTrayNotifyWnd=FindWindowEx(hShellTrayWnd,NULL,TEXT("TrayNotifyWnd"),NULL);
    if(hTrayNotifyWnd)
    {
      GetWindowRect(hTrayNotifyWnd,lpTrayRect);
      return;
    }
  }

  // OK, we failed to get the rect from the quick hack. Either explorer isn't
  // running or it's a new version of the shell with the window class names
  // changed (how dare Microsoft change these undocumented class names!) So, we
  // try to find out what side of the screen the taskbar is connected to. We
  // know that the system tray is either on the right or the bottom of the
  // taskbar, so we can make a good guess at where to minimize to
  /*APPBARDATA appBarData;*/
  appBarData.cbSize=sizeof(appBarData);
  if(SHAppBarMessage(ABM_GETTASKBARPOS,&appBarData))
  {
    // We know the edge the taskbar is connected to, so guess the rect of the
    // system tray. Use various fudge factor to make it look good
    switch(appBarData.uEdge)
    {
      case ABE_LEFT:
      case ABE_RIGHT:
  // We want to minimize to the bottom of the taskbar
  lpTrayRect->top=appBarData.rc.bottom-100;
  lpTrayRect->bottom=appBarData.rc.bottom-16;
  lpTrayRect->left=appBarData.rc.left;
  lpTrayRect->right=appBarData.rc.right;
  break;

      case ABE_TOP:
      case ABE_BOTTOM:
  // We want to minimize to the right of the taskbar
  lpTrayRect->top=appBarData.rc.top;
  lpTrayRect->bottom=appBarData.rc.bottom;
  lpTrayRect->left=appBarData.rc.right-100;
  lpTrayRect->right=appBarData.rc.right-16;
  break;
    }

    return;
  }

  // Blimey, we really aren't in luck. It's possible that a third party shell
  // is running instead of explorer. This shell might provide support for the
  // system tray, by providing a Shell_TrayWnd window (which receives the
  // messages for the icons) So, look for a Shell_TrayWnd window and work out
  // the rect from that. Remember that explorer's taskbar is the Shell_TrayWnd,
  // and stretches either the width or the height of the screen. We can't rely
  // on the 3rd party shell's Shell_TrayWnd doing the same, in fact, we can't
  // rely on it being any size. The best we can do is just blindly use the
  // window rect, perhaps limiting the width and height to, say 150 square.
  // Note that if the 3rd party shell supports the same configuraion as
  // explorer (the icons hosted in NotifyTrayWnd, which is a child window of
  // Shell_TrayWnd), we would already have caught it above
  hShellTrayWnd=FindWindowEx(NULL,NULL,TEXT("Shell_TrayWnd"),NULL);
  if(hShellTrayWnd)
  {
    GetWindowRect(hShellTrayWnd,lpTrayRect);
    if(lpTrayRect->right-lpTrayRect->left>DEFAULT_RECT_WIDTH)
      lpTrayRect->left=lpTrayRect->right-DEFAULT_RECT_WIDTH;
    if(lpTrayRect->bottom-lpTrayRect->top>DEFAULT_RECT_HEIGHT)
      lpTrayRect->top=lpTrayRect->bottom-DEFAULT_RECT_HEIGHT;

    return;
  }

  // OK. Haven't found a thing. Provide a default rect based on the current work
  // area
  SystemParametersInfo(SPI_GETWORKAREA,0,lpTrayRect,0);
  lpTrayRect->left=lpTrayRect->right-DEFAULT_RECT_WIDTH;
  lpTrayRect->top=lpTrayRect->bottom-DEFAULT_RECT_HEIGHT;
}

// Check to see if the animation has been disabled
/*static */BOOL GetDoAnimateMinimize(VOID)
{
  ANIMATIONINFO ai;

  ai.cbSize=sizeof(ai);
  SystemParametersInfo(SPI_GETANIMATION,sizeof(ai),&ai,0);

  return ai.iMinAnimate?TRUE:FALSE;
}

VOID MinimizeWndToTray(HWND hWnd)
{
  if(GetDoAnimateMinimize())
  {
    RECT rcFrom,rcTo;

    // Get the rect of the window. It is safe to use the rect of the whole
    // window - DrawAnimatedRects will only draw the caption
    GetWindowRect(hWnd,&rcFrom);
    GetTrayWndRect(&rcTo);

    // Get the system to draw our animation for us
    DrawAnimatedRects(hWnd,IDANI_CAPTION,&rcFrom,&rcTo);
  }

  // Add the tray icon. If we add it before the call to DrawAnimatedRects,
  // the taskbar gets erased, but doesn't get redrawn until DAR finishes.
  // This looks untidy, so call the functions in this order

  // Hide the window
  ShowWindow(hWnd,SW_HIDE);
}

VOID RestoreWndFromTray(HWND hWnd)
{
  if(GetDoAnimateMinimize())
  {
    // Get the rect of the tray and the window. Note that the window rect
    // is still valid even though the window is hidden
    RECT rcFrom,rcTo;
    GetTrayWndRect(&rcFrom);
    GetWindowRect(hWnd,&rcTo);

    // Get the system to draw our animation for us
    DrawAnimatedRects(hWnd,IDANI_CAPTION,&rcFrom,&rcTo);
  }

  // Show the window, and make sure we're the foreground window
  ShowWindow(hWnd,SW_SHOW);
  SetActiveWindow(hWnd);
  SetForegroundWindow(hWnd);

  // Remove the tray icon. As described above, remove the icon after the
  // call to DrawAnimatedRects, or the taskbar will not refresh itself
  // properly until DAR finished
}



///   End of Helpers.c   \\\
