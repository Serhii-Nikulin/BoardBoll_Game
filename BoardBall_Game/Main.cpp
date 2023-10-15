// BoardBall_Game.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Main.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                   
WCHAR szWindowClass[MAX_LOADSTRING]; 

const int Global_Scale = 3;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Ceil_Width = 16;
const int Ceil_Height = 8;
//------------------------------------------------------------------------------------------------------------
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//------------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
   
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BOARDBALLGAME, szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BOARDBALLGAME));
    
    MSG msg;
   
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}
//------------------------------------------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{//подготавлиявет структуру окна szWindowClass
 
   WNDCLASSEXW wcex;
  
   wcex.cbSize = sizeof(WNDCLASSEX);

   wcex.style          = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc    = WndProc;
   wcex.cbClsExtra     = 0;
   wcex.cbWndExtra     = 0;
   wcex.hInstance      = hInstance;
   wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BOARDBALLGAME));
   wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
   wcex.hbrBackground  = CreateSolidBrush(RGB(0, 0, 0) );
   wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BOARDBALLGAME);
   wcex.lpszClassName  = szWindowClass;
   wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

   return RegisterClassExW(&wcex);
}
//------------------------------------------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)//Saves instance handle and creates main window
{

   hInst = hInstance; // Store instance handle in our global variable

   RECT window_rect;
   window_rect.left = 0;
   window_rect.top = 0;
   window_rect.right = 320 * 3;
   window_rect.bottom = 200 * 3;

   AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, TRUE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, nullptr, nullptr, hInstance, nullptr);//создаем окно с учетом коррекции координат его вершин
   
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
  
   return TRUE;
}
//------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, bool is_blue)
{
   HPEN pen;
   HBRUSH brush;

   if (is_blue)
   {
      brush = CreateSolidBrush(RGB(62, 71, 202) );
      pen = CreatePen(PS_SOLID, 0, RGB(62, 71, 202) );
   }
   else
   {
      brush = CreateSolidBrush(RGB(235, 28, 35) );
      pen = CreatePen(PS_SOLID, 0, RGB(235, 28, 35) );
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);
   Rectangle(hdc, x * Global_Scale, y * Global_Scale, (x + Brick_Width) * Global_Scale, (y + Brick_Height) * Global_Scale);

}
//------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc)
{//отрисовка экрана игры

   int i, j;
   
   for (i = 0; i < 14; i++)
      for (j = 0; j < 12; j++)
         Draw_Brick(hdc, Level_X_Offset + Ceil_Width * j, Level_Y_Offset + Ceil_Height * i, true);
}
//------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)//Processes messages for the main window.
{
   //оконная процедура для обработки сообщений полученных от ОС
  
    switch (message)
    {
    case WM_COMMAND://описывает пункт выбора меню
        {
            int wmId = LOWORD(wParam);
        
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT://Запрос программе на отрисовку
        {
          PAINTSTRUCT ps;
          HDC hdc = BeginPaint(hWnd, &ps);
          
          Draw_Frame(hdc);
          EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY://срабатывает при выходе из программы
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
//------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)// Message handler for about box.
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
//------------------------------------------------------------------------------------------------------------