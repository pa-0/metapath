/******************************************************************************
*
*
* metapath - The universal Explorer-like Plugin
*
* metapath.h
*   Global definitions and declarations
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



//==== Main Window ============================================================
#define WC_METAPATH L"metapath"

#define WS_METAPATH (WS_OVERLAPPEDWINDOW ^ \
                    (WS_MINIMIZEBOX | WS_MAXIMIZEBOX)) | \
                    (WS_CLIPCHILDREN | WS_POPUP)


//==== Data Type for WM_COPYDATA ==============================================
#define DATA_METAPATH_PATHARG 0xFB30


//==== ComboBox Control =======================================================
//#define WC_COMBOBOX L"ComboBox"

#define WS_DRIVEBOX (WS_CHILD | \
                     /*WS_VISIBLE |*/ \
                     WS_CLIPSIBLINGS | \
                     WS_VSCROLL | \
                     CBS_DROPDOWNLIST)


//==== Listview Control =======================================================
#define WS_DIRLIST (WS_CHILD | \
                    WS_VISIBLE | \
                    WS_CLIPSIBLINGS | \
                    WS_CLIPCHILDREN | \
                    LVS_REPORT | \
                    LVS_NOCOLUMNHEADER | \
                    LVS_SHAREIMAGELISTS | \
                    LVS_AUTOARRANGE | \
                    LVS_SINGLESEL | \
                    LVS_SHOWSELALWAYS)


//==== Toolbar Style ==========================================================
#define WS_TOOLBAR (WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | \
                    TBSTYLE_TOOLTIPS | TBSTYLE_ALTDRAG | TBSTYLE_LIST | \
                    CCS_NODIVIDER | CCS_NOPARENTALIGN)


//==== ReBar Style ============================================================
#define WS_REBAR (WS_CHILD | /*WS_VISIBLE |*/ WS_CLIPCHILDREN | WS_BORDER | \
                  RBS_VARHEIGHT | RBS_BANDBORDERS /*| RBS_FIXEDORDER */ | \
                  CCS_NODIVIDER |/*CCS_NORESIZE | */CCS_NOPARENTALIGN)


//==== Ids ====================================================================
#define IDC_STATUSBAR 0x00A0
#define IDC_TOOLBAR   0x00A1
#define IDC_REBAR     0x00A2
#define IDC_DRIVEBOX  0xA000
#define IDC_DIRLIST   0xA001


//==== Statusbar ==============================================================
#define ID_FILEINFO   0
#define ID_MENUHELP 255


//==== Timer for Change Notifications =========================================
#define ID_TIMER 0xA000


//==== Callback Message from System Tray ======================================
#define WM_TRAYMESSAGE WM_USER


//==== Function Declarations ==================================================
BOOL InitApplication(HINSTANCE);
HWND InitInstance(HINSTANCE,LPSTR,int);
BOOL ActivatePrevInst();
void ShowNotifyIcon(HWND,BOOL);

BOOL ChangeDirectory(HWND,LPCWSTR,BOOL);
void LoadSettings();
void SaveSettings(BOOL);

void ParseCommandLine();
void LoadFlags();
int  CheckIniFile(LPWSTR,LPCWSTR);
int  CheckIniFileRedirect(LPWSTR,LPCWSTR);
int  FindIniFile();
int  TestIniFile();
int  CreateIniFile();
int  CreateIniFileEx(LPCWSTR);

BOOL DisplayPath(LPCWSTR,UINT);
BOOL DisplayLnkFile(LPCWSTR);

void LaunchTarget(LPCWSTR,BOOL);
void SnapToTarget(HWND);
void SnapToDefaultPos(HWND);

LRESULT CALLBACK HiddenWndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK MainWndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT MsgCreate(HWND,WPARAM,LPARAM);
void    CreateBars(HWND,HINSTANCE);
void    MsgThemeChanged(HWND,WPARAM,LPARAM);
void    MsgSize(HWND,WPARAM,LPARAM);
void    MsgInitMenu(HWND,WPARAM,LPARAM);
LRESULT MsgCommand(HWND,WPARAM,LPARAM);
LRESULT MsgNotify(HWND,WPARAM,LPARAM);



///   End of metapath.h   \\\
