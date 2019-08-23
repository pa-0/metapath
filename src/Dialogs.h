/******************************************************************************
*
*
* metapath - The universal Explorer-like Plugin
*
* Dialogs.h
*   Definitions for metapath dialog boxes
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


int  ErrorMessage(int,UINT,...);
BOOL GetDirectory(HWND,int,LPWSTR,LPCWSTR,BOOL);
BOOL GetDirectory2(HWND,int,LPWSTR,int);

INT_PTR CALLBACK RunDlgProc(HWND, UINT, WPARAM, LPARAM);
void RunDlg(HWND);

INT_PTR CALLBACK GotoDlgProc(HWND, UINT, WPARAM, LPARAM);
void GotoDlg(HWND);

INT_PTR CALLBACK AboutDlgProc(HWND,UINT,WPARAM,LPARAM);
INT_PTR OptionsPropSheet(HWND, HINSTANCE);

INT_PTR CALLBACK GetFilterDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL GetFilterDlg(HWND);

INT_PTR CALLBACK RenameFileDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL RenameFileDlg(HWND);

INT_PTR CALLBACK CopyMoveDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CopyMoveDlg(HWND, UINT*);

INT_PTR CALLBACK OpenWithDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL OpenWithDlg(HWND, LPDLITEM);

INT_PTR CALLBACK NewDirDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL NewDirDlg(HWND,LPWSTR);

INT_PTR CALLBACK FindWinDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK FindTargetDlgProc(HWND, UINT, WPARAM, LPARAM);


// End of Dialogs.h
