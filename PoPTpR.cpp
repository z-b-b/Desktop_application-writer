/* POPPAD TYPER */

#include <windows.h>

#define IDM_NEW     1
#define IDM_OPEN    2
#define IDM_SAVE    3
#define IDM_SAVEAS  4
#define IDM_PRINT   5
#define IDM_EXIT    6

#define IDM_UNDO    10
#define IDM_CUT     11
#define IDM_COPY    12
#define IDM_PASTE   13
#define IDM_DEL     14
#define IDM_SELALL  15

#define IDM_HELP    20
#define IDM_ABOUT   22

#define BUFFER(x,y) *(pBuffer + y * cxBuffer + x)

char szAppName[] = "PopPad" ; // ùí îåôò äçìåï - àá
char szCAppName[] = "Typer" ; // ùí îåôò äçìåï - áï

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ; // PROC - àá
LRESULT CALLBACK CWndProc (HWND, UINT, WPARAM, LPARAM); // PROC - áï

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     HACCEL     hAccel ;
     HWND       hwnd ;
     MSG        msg ;
     WNDCLASSEX wndclass ;

     wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (hInstance, szAppName) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = szAppName ;
     wndclass.lpszClassName = szAppName ;
     wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

     RegisterClassEx (&wndclass);

	 wndclass.lpszClassName = szCAppName;
	 wndclass.lpfnWndProc   = CWndProc  ;

	 RegisterClassEx (&wndclass);

     hwnd = CreateWindow (szAppName, "PopPad with Typer , æàá & çðï",
                          WS_OVERLAPPEDWINDOW,
                          GetSystemMetrics (SM_CXSCREEN) / 4,
                          GetSystemMetrics (SM_CYSCREEN) / 4,
                          GetSystemMetrics (SM_CXSCREEN) / 2,
                          GetSystemMetrics (SM_CYSCREEN) / 2,
                          NULL, NULL, hInstance, NULL) ;

     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ; 

     hAccel = LoadAccelerators (hInstance, szAppName) ;

     while (GetMessage (&msg, NULL, 0, 0))
          {
          if (!TranslateAccelerator (hwnd, hAccel, &msg))
               {
               TranslateMessage (&msg) ;
               DispatchMessage (&msg) ;
               }
          }
     return msg.wParam ;
     }

AskConfirmation (HWND hwnd)
     {
     return MessageBox (hwnd, "Really want to close Pop - Typ?",
                        "PopPad and Typer", MB_YESNO | MB_ICONQUESTION) ;
     }

// PROC - àá
LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     static HWND hwndEdit ;
     int         iSelect, iEnable ;

     switch (iMsg)
          {
          case WM_CREATE :
               hwndEdit = CreateWindow (szCAppName, "~ TYPER ~",
						  WS_CHILDWINDOW | WS_VISIBLE | WS_CAPTION ,
                          0, 0, 0, 0,
                          hwnd, (HMENU) 1,
                          ((LPCREATESTRUCT) lParam)->hInstance, NULL) ;
               return 0  ;

          case WM_SETFOCUS :
               SetFocus (hwndEdit) ;
               return 0 ;

          case WM_SIZE : 
               MoveWindow (hwndEdit, 0, 0, LOWORD (lParam),
                                           HIWORD (lParam), TRUE) ;
               return 0 ;

          case WM_INITMENUPOPUP :
               if (lParam == 1)
                    {
                    EnableMenuItem ((HMENU) wParam, IDM_UNDO,
                         SendMessage (hwndEdit, EM_CANUNDO, 0, 0) ?
                              MF_ENABLED : MF_GRAYED) ;

                    EnableMenuItem ((HMENU) wParam, IDM_PASTE,
                         IsClipboardFormatAvailable (CF_TEXT) ?
                              MF_ENABLED : MF_GRAYED) ;

                    iSelect = SendMessage (hwndEdit, EM_GETSEL, 0, 0) ;

                    if (HIWORD (iSelect) == LOWORD (iSelect))
                         iEnable = MF_GRAYED ;
                    else
                         iEnable = MF_ENABLED ;

                    EnableMenuItem ((HMENU) wParam, IDM_CUT,   iEnable) ;
                    EnableMenuItem ((HMENU) wParam, IDM_COPY,  iEnable) ;
                    EnableMenuItem ((HMENU) wParam, IDM_DEL,   iEnable) ;

                    return 0 ;
                    }
               break ;

          case WM_COMMAND :

               if (lParam)
                    {
                    if (LOWORD (lParam) == 1 &&
                         (HIWORD (wParam) == EN_ERRSPACE ||
                          HIWORD (wParam) == EN_MAXTEXT))
                              MessageBox (hwnd, "Edit control out of space.",
                                          szAppName, MB_OK | MB_ICONSTOP) ;
                    return 0 ;
                    }

               else switch (LOWORD (wParam))
                         {
                         case IDM_NEW :
                         case IDM_OPEN :
                         case IDM_SAVE :
                         case IDM_SAVEAS :
                         case IDM_PRINT :
                              MessageBeep (0) ;
                              return 0 ;

                         case IDM_EXIT :
                              SendMessage (hwnd, WM_CLOSE, 0, 0) ;
                              return 0 ;

                         case IDM_UNDO :
                              SendMessage (hwndEdit, WM_UNDO, 0, 0) ;
                              return 0 ;

                         case IDM_CUT :
                              SendMessage (hwndEdit, WM_CUT, 0, 0) ;
                              return 0 ;

                         case IDM_COPY :
                              SendMessage (hwndEdit, WM_COPY, 0, 0) ;
                              return 0 ;

                         case IDM_PASTE :
                              SendMessage (hwndEdit, WM_PASTE, 0, 0) ;
                              return 0 ;

                         case IDM_DEL :
                              SendMessage (hwndEdit, WM_CLEAR, 0, 0) ;
                              return 0 ;

                         case IDM_SELALL :
                              SendMessage (hwndEdit, EM_SETSEL, 0, -1) ;
                              return 0 ;

                         case IDM_HELP :
                              MessageBox (hwnd, "Help not yet implemented!",
                                   szAppName, MB_OK | MB_ICONEXCLAMATION) ;
                              return 0 ;

                         case IDM_ABOUT :
                              MessageBox (hwnd,
                                   "POPPAD2 (c) Charles Petzold, 1996",
                                   szAppName, MB_OK | MB_ICONINFORMATION) ;
                              return 0 ;
                         }
               break ;

          case WM_CLOSE :
               if (IDYES == AskConfirmation (hwnd))
                    DestroyWindow (hwnd) ;
               return 0 ;

          case WM_QUERYENDSESSION :
               if (IDYES == AskConfirmation (hwnd))
                    return 1 ;
               else
                    return 0 ;

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}

// PROC - áï
LRESULT CALLBACK CWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     static char *pBuffer = NULL ;
     static int  cxChar, cyChar, cxClient, cyClient, cxBuffer, cyBuffer,
                 xCaret, yCaret ;
     HDC         hdc ;
     int         x, y, i ;
     PAINTSTRUCT ps ;
     TEXTMETRIC  tm ;

     switch (iMsg)
          {
          case WM_CREATE :
               hdc = GetDC (hwnd) ;

               SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT)) ;
               GetTextMetrics (hdc, &tm) ;
               cxChar = tm.tmAveCharWidth ;
               cyChar = tm.tmHeight ;

               ReleaseDC (hwnd, hdc) ;
               return 0 ;

          case WM_SIZE :
                                   // obtain window size in pixels

               cxClient = LOWORD (lParam) ;
               cyClient = HIWORD (lParam) ;

                                   // calculate window size in characters

               cxBuffer = max (1, cxClient / cxChar) ;
               cyBuffer = max (1, cyClient / cyChar) ;

                                   // allocate memory for buffer and clear it

               if (pBuffer != NULL)
                    free (pBuffer) ;

               if ((pBuffer = (char *) malloc (cxBuffer * cyBuffer)) == NULL)
                    MessageBox (hwnd, "Window too large.  Cannot "
                                      "allocate enough memory.", "Typer",
                                      MB_ICONEXCLAMATION | MB_OK) ;
               else
                    for (y = 0 ; y < cyBuffer ; y++)
                         for (x = 0 ; x < cxBuffer ; x++)
                              BUFFER(x,y) = ' ' ;

                                   // set caret to upper left corner
               xCaret = 0 ;
               yCaret = 0 ;

               if (hwnd == GetFocus ())
                    SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;

               return 0 ;

          case WM_SETFOCUS :
                                   // create and show the caret

               CreateCaret (hwnd, NULL, cxChar, cyChar) ;
               SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
               ShowCaret (hwnd) ;
               return 0 ;

          case WM_KILLFOCUS :
                                   // hide and destroy the caret
               HideCaret (hwnd) ;
               DestroyCaret () ;
               return 0 ;

          case WM_KEYDOWN :
               switch (wParam)
                    {
                    case VK_HOME :
                         xCaret = 0 ;
                         break ;

                    case VK_END :
                         xCaret = cxBuffer - 1 ;
                         break ;

                    case VK_PRIOR :
                         yCaret = 0 ;
                         break ;

                    case VK_NEXT :
                         yCaret = cyBuffer - 1 ;
                         break ;

                    case VK_LEFT :
                         xCaret = max (xCaret - 1, 0) ;
                         break ;

                    case VK_RIGHT :
                         xCaret = min (xCaret + 1, cxBuffer - 1) ;
                         break ;

                    case VK_UP :
                         yCaret = max (yCaret - 1, 0) ;
                         break ;

                    case VK_DOWN :
                         yCaret = min (yCaret + 1, cyBuffer - 1) ;
                         break ;

                    case VK_DELETE :
                         for (x = xCaret ; x < cxBuffer - 1 ; x++)
                              BUFFER (x, yCaret) = BUFFER (x + 1, yCaret) ;

                         BUFFER (cxBuffer - 1, yCaret) = ' ' ;

                         HideCaret (hwnd) ;
                         hdc = GetDC (hwnd) ;

                         SelectObject (hdc,
                              GetStockObject (SYSTEM_FIXED_FONT)) ;

                         TextOut (hdc, xCaret * cxChar, yCaret * cyChar,
                                  & BUFFER (xCaret, yCaret),
                                  cxBuffer - xCaret) ;

                         ShowCaret (hwnd) ;
                         ReleaseDC (hwnd, hdc) ;
                         break ;
                    }

               SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
               return 0 ;

          case WM_CHAR :
               for (i = 0 ; i < (int) LOWORD (lParam) ; i++)
                    {
                    switch (wParam)
                         {
                         case '\b' :                    // backspace
                              if (xCaret > 0)
                                   {
                                   xCaret-- ;
                                   SendMessage (hwnd, WM_KEYDOWN,
                                                VK_DELETE, 1L) ;
                                   }
                              break ;

                         case '\t' :                    // tab
                              do
                                   {
                                   SendMessage (hwnd, WM_CHAR, ' ', 1L) ;
                                   }
                              while (xCaret % 8 != 0) ;
                              break ;

                         case '\n' :                    // line feed
                              if (++yCaret == cyBuffer)
                                   yCaret = 0 ;
                              break ;

                         case '\r' :                    // carriage return
                              xCaret = 0 ;

                              if (++yCaret == cyBuffer)
                                   yCaret = 0 ;
                              break ;

                         case '\x1B' :                  // escape
                              for (y = 0 ; y < cyBuffer ; y++)
                                   for (x = 0 ; x < cxBuffer ; x++)
                                        BUFFER (x, y) = ' ' ;

                              xCaret = 0 ;
                              yCaret = 0 ;

                              InvalidateRect (hwnd, NULL, FALSE) ;
                              break ;

                         default :                       // character codes
                              BUFFER (xCaret, yCaret) = (char) wParam ;

                              HideCaret (hwnd) ;
                              hdc = GetDC (hwnd) ;

                              SelectObject (hdc,
                                   GetStockObject (SYSTEM_FIXED_FONT)) ;

                              TextOut (hdc, xCaret * cxChar, yCaret * cyChar,
                                       & BUFFER (xCaret, yCaret), 1) ;

                              ShowCaret (hwnd) ;
                              ReleaseDC (hwnd, hdc) ;

                              if (++xCaret == cxBuffer)
                                   {
                                   xCaret = 0 ;

                                   if (++yCaret == cyBuffer)
                                        yCaret = 0 ;
                                   }
                              break ;
                         }
                    }

               SetCaretPos (xCaret * cxChar, yCaret * cyChar) ;
               return 0 ;

          case WM_PAINT :
               hdc = BeginPaint (hwnd, &ps) ;
               SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT)) ;

               for (y = 0 ; y < cyBuffer ; y++)
                    TextOut (hdc, 0, y * cyChar, & BUFFER(0,y), cxBuffer) ;

               EndPaint (hwnd, &ps) ;
               return 0 ;

          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}
