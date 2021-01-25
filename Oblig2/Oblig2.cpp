// Oblig2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Oblig2.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

static int pw = 0, pn = 0;

class TrafficLight {
public:
    int state; //state 0 = raudt, state 1 = raudt + gult, state 2 = grønt, state 3 = gult
    int x, y; //posisjon i øvre venstre hjørne

    TrafficLight(int _x, int _y): x(_x), y(_y), state(0) {}
    TrafficLight(int _x, int _y, int _state) : x(_x), y(_y), state(_state) {}

    int changeToNextState() {
        state = (state + 1) % 4;
        return state;
    }

    void paintTrafficLight(HDC hdc) {

        HBRUSH hBrushBlack = CreateSolidBrush(RGB(0, 0, 0));
        HGDIOBJ hOrg = SelectObject(hdc, hBrushBlack);

        //kode for svart firkant
        Rectangle(hdc, x, y, x + 50, y + 140);

        //Raud brush
        HBRUSH hBrushRed = CreateSolidBrush(RGB(255, 0, 0));
        //Gul brush
        HBRUSH hBrushYellow = CreateSolidBrush(RGB(255, 255, 0));
        //Grøn brush
        HBRUSH hBrushGreen = CreateSolidBrush(RGB(0, 255, 0));
        //Grå brush
        HBRUSH hBrushGrey = CreateSolidBrush(RGB(128, 128, 128));

        int xPosCircle = x + 5, yTop = y + 5, yMiddle = yTop + 45, yBottom = yMiddle + 45;
        int dx = 40, dy = 40;

        switch (state)
        {
        case 0:
            //raud øvst, 2 grå under
            SelectObject(hdc, hBrushRed);
            Ellipse(hdc, xPosCircle, yTop, xPosCircle + dx, yTop + dy);
            
            SelectObject(hdc, hBrushGrey);
            Ellipse(hdc, xPosCircle, yMiddle, xPosCircle + dx, yMiddle + dy);
            Ellipse(hdc, xPosCircle, yBottom, xPosCircle + dx, yBottom + dy);
            
            break;
        case 1:
            //raud øvst, gul i midten, grå nedst
            SelectObject(hdc, hBrushRed);
            Ellipse(hdc, xPosCircle, yTop, xPosCircle + dx, yTop + dy);

            SelectObject(hdc, hBrushYellow);
            Ellipse(hdc, xPosCircle, yMiddle, xPosCircle + dx, yMiddle + dy);

            SelectObject(hdc, hBrushGrey);
            Ellipse(hdc, xPosCircle, yBottom, xPosCircle + dx, yBottom + dy);

            break;
        case 2:
            //2 grå på toppen, grøn nedst
            SelectObject(hdc, hBrushGrey);
            Ellipse(hdc, xPosCircle, yTop, xPosCircle + dx, yTop + dy);
            Ellipse(hdc, xPosCircle, yMiddle, xPosCircle + dx, yMiddle + dy);

            SelectObject(hdc, hBrushGreen);
            Ellipse(hdc, xPosCircle, yBottom, xPosCircle + dx, yBottom + dy);

            break;
        case 3:
            //gul i midten, grå på toppen/botn
            SelectObject(hdc, hBrushGrey);
            Ellipse(hdc, xPosCircle, yTop, xPosCircle + dx, yTop + dy);
            Ellipse(hdc, xPosCircle, yBottom, xPosCircle + dx, yBottom + dy);

            SelectObject(hdc, hBrushYellow);
            Ellipse(hdc, xPosCircle, yMiddle, xPosCircle + dx, yMiddle + dy);

            break;
        default:
            break;
        }
        //Set svart til den normale igjen
        SelectObject(hdc, hOrg);
        //Sletter brushes
        DeleteObject(hBrushBlack);
        DeleteObject(hBrushRed);
        DeleteObject(hBrushYellow);
        DeleteObject(hBrushGreen);
        DeleteObject(hBrushGrey);
    }
};

class Car {
public:
    int x, y;
    COLORREF color;
    int direction; //0 = l->r, 1 = r->l, 2 = t->b, 3 = b->t. Bruker berre 0 og 2 nå?
    bool driveDirection; //false = horisontal, true = vertikal
    TrafficLight* ptl; //trafikklyset til bilen..
    bool finished;
    Car(int _x, int _y, COLORREF _color, bool _driveDirection, TrafficLight* _ptl): x(_x), y(_y), color(_color), driveDirection(_driveDirection), ptl(_ptl) {
        direction = 0;
        finished = false;
    }
    Car(int _x, int _y, COLORREF _color, int _direction, TrafficLight* _ptl) : x(_x), y(_y), color(_color), direction(_direction), ptl(_ptl) {
        driveDirection = false;
        finished = false;
    }

    void driveCar(int distance, int cross, int end, Car* pCarInFront) {
        //if (finished) return;

        int state = ptl->state;
        //TODO detta her e rot...
        if (pCarInFront == 0) {
            if (driveDirection) { //vertikal
                bool stopp = (y < cross && (y + 26 >= cross && (state == 0 || state == 1)));
                if (!stopp) {
                    y = y + distance;
                    if (y > end) finished = true;
                }
            }
            else { //horisontal
                bool stopp = (x < cross && (x + 26 >= cross && (state == 0 || state == 1)));
                if (!stopp) {
                    x = x + distance;
                    if (x > end) finished = true;
                }
            }
        }
        else {
            if (driveDirection) { //vertikal
                int yCar = pCarInFront->y;
                bool stopp = (y < cross && y + 26 >= yCar) || (y < cross && (y + 26 >= cross && (state == 0 || state == 1)));
                if (!stopp) {
                    y = y + distance;
                    if (y > end) finished = true;
                }
            }
            else { //horisontal
                int xCar = pCarInFront->x;
                bool stopp = (x < cross && x + 26 >= xCar) || (x < cross && (x + 26 >= cross && (state == 0 || state == 1)));
                if (!stopp) {
                    x = x + distance;
                    if (x > end) finished = true;
                }
            }
        }
    }

    void paintCar(HDC hdc) {
        //if (finished) return;

        HBRUSH hBrush = CreateSolidBrush(color);
        HGDIOBJ hOrg = SelectObject(hdc, hBrush);

        Rectangle(hdc, x, y, x + 16, y + 16);

        SelectObject(hdc, hOrg);
        DeleteObject(hBrush);
    }
};

class CarList {
public:
    Car* list[1000];
    int nr;
    CarList() {
        nr = 0;
    }

    Car* get(int _nr) {
        return list[nr];
    }

    void put(Car* pc) {
        list[nr] = pc;
        nr++;
    }
    
    void driveCars(int dx, int cross, int end) {
        for (int i = 0; i < nr; i++)
        {
            if (list[i]->finished) continue;
            if (i == 0) list[i]->driveCar(dx, cross, end, 0);
            else list[i]->driveCar(dx, cross, end, list[i-1]);
        }
    }

    void paintCars(HDC hdc) {
        for (int i = 0; i < nr; i++) {
            if (list[i]->finished) continue;
            list[i]->paintCar(hdc);
        }
    }

    void Clear() {
        for (int i = 0; i < nr; i++) {
            delete list[i];
        }
        nr = 0;
    }
};

class RoadCrossing {
public:
    int x, y; //koordinatar for øvre venstre hjørne i vegen som er horisontal
    RoadCrossing(int _x, int _y): x(_x), y(_y) {}

    void paintRoadCrossing(HDC hdc) {

        //TODO eitt road-objekt = 2 vegar som krysse?!?

        //lagar brushes - TODO flytte ut/lagre ein annan plass?
        //Gul brush
        HBRUSH hBrushYellow = CreateSolidBrush(RGB(255, 255, 0));
        //Grå brush
        HBRUSH hBrushGrey = CreateSolidBrush(RGB(128, 128, 128));
        //mørkegrå brush
        HBRUSH hBrushDarkGrey = CreateSolidBrush(RGB(105, 105, 105));

        //originalobjektet + vel grå
        HGDIOBJ hOrg = SelectObject(hdc, hBrushGrey);

        //krysser er 55x55, 250 lang veg ut frå kvar side. midtstripe er 5 tjukk, eitt felt = 25 breitt
        //dei gråe vegane
        Rectangle(hdc, x, y, x + 555, y + 55);
        Rectangle(hdc, x + 250, y - 250, x + 305, y + 305);

        //gul midtstriper
        SelectObject(hdc, hBrushYellow);
        Rectangle(hdc, x, y + 25, x + 555, y + 30);
        Rectangle(hdc, x + 275, y - 250, x + 280, y + 305);

        //grå krysning
        SelectObject(hdc, hBrushDarkGrey);
        Rectangle(hdc, x + 250, y, x + 305, y + 55);

        SelectObject(hdc, hOrg);
        //sletter brushes
        DeleteObject(hBrushGrey);
        DeleteObject(hBrushYellow);
        DeleteObject(hBrushDarkGrey);
    }
};

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ProbDialog(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OBLIG2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OBLIG2));

    MSG msg;

    // Main message loop:
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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OBLIG2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OBLIG2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      50, 50, 1280, 770, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int x = 300, y = 300; //"globale" koordinatar
    static int cStartWEX = x, cStartWEY = y + 34, cStartNSX = x + 255, cStartNSY = y-250; //startpos for bilane
    //TODO: ein annan lokasjon på desse?
    static TrafficLight tl1(x + 150, y + 105, 2); //er grønt ved start, trafikklys for v->a
    static TrafficLight tl2(x + 355, y - 190, 0); //er raudt ved start, trafikklys for n->s
    //static Road road1(100, 300, false);
    //static Road road2(300, 75, true);
    static RoadCrossing road(x, y); //vegane og krysset
    static CarList carListHor;
    static CarList carListVer;

    switch (message)
    {
    case WM_CREATE:
        DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ProbDialog);
        //TODO burde nok starte timers når dialogen er heilt ferdig..
        SetTimer(hWnd, 0, 2000, 0); //timer for lysa
        SetTimer(hWnd, 1, 100, 0); // timer for bilane
        SetTimer(hWnd, 2, 1000, 0); //timer for når bilane blir oppretta
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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

    case WM_RBUTTONDOWN:

        //bil skal komme frå nord->sør
        //carListVer.put(new Car(cStartNSX, cStartNSY, RGB(255, 0, 0), true, &tl2));
        InvalidateRect(hWnd, 0, true);
        break;

    case WM_LBUTTONDOWN:
        //bil skal komme frå vest->aust
        //carListHor.put(new Car(cStartWEX, cStartWEY, RGB(0, 0, 255), false, &tl1));
        //endre state på trafikklyset
        //tl1.changeToNextState();
        //tl2.changeToNextState();
        InvalidateRect(hWnd, 0, true);
        break;

    case WM_TIMER:

        switch (wParam)
        {
        case 0: //lystimer
            //endre state på trafikklyset
            tl1.changeToNextState();
            tl2.changeToNextState();
            break;

        case 1: //biltimer
            carListVer.driveCars(10, y, y+305);
            carListHor.driveCars(10, x+250, x+555);
            break;

        case 2: //oppretta bilar
        {
            //TODO gjer detta betre? litt usikker kva ein meine med sannsyn per sekund?
            //henta frå: https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/rand?view=msvc-160
            int range_max = 100, range_min = 0;
            int randomW = (double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min; //range_min <= random < range_max
            int randomN = (double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min;
            //random er mellom 0 og 99
            if (randomW < pw) { //pw frå 0-100, pw=100 alltid bil, pw=0 ingen bil
                carListHor.put(new Car(cStartWEX, cStartWEY, RGB(0, 0, 255), false, &tl1));
            }
            if (randomN < pn) { //pn frå 0-100, pn=100 alltid bil, pw=0 ingen bil
                carListVer.put(new Car(cStartNSX, cStartNSY, RGB(255, 0, 0), true, &tl2));
            }
        }   
            break;
        default:
            break;
        }
        //TODO vil avogtil kalle denne unødvendig, fiks?
        InvalidateRect(hWnd, 0, true);
        break;

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_UP:
            pn = (pn + 10) >= 100 ? 100 : pn + 10;
            break;

        case VK_DOWN:
            pn = (pn - 10) <= 0 ? 0 : pn - 10;
            break;

        case VK_LEFT:
            pw = (pw - 10) <= 0 ? 0 : pw - 10;
            break;

        case VK_RIGHT:
            pw = (pw + 10) >= 100 ? 100 : pw + 10;
            break;

        default:
            break;
        }
    }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            //HDC memhdc = CreateCompatibleDC(hdc);
            //HBITMAP memBM = CreateCompatibleBitmap(hdc, 1280, 720);
            //SelectObject(memhdc, memBM);
            //teikne trafikklyset
            tl1.paintTrafficLight(hdc);
            tl2.paintTrafficLight(hdc);

            //teikne vegkrysset
            road.paintRoadCrossing(hdc);

            //teikne billistene
            carListVer.paintCars(hdc);
            carListHor.paintCars(hdc);

            //skrive ut verdiane
            WCHAR tekst[100];
            wsprintf(tekst, L"pw: %d, pn: %d", pw, pn);
            TextOut(hdc, 10, 10, tekst, lstrlen(tekst));

            //BitBlt(hdc, 0, 0, 1280, 720, memhdc, 0, 0, SRCCOPY);


            /*HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
            HPEN hPen = (HPEN)GetStockObject(WHITE_PEN);
            SelectObject(hdc, hBrush);
            SelectObject(hdc, hPen);

            Rectangle(hdc, 0, 720, 2560, 2560);
            Rectangle(hdc, 1280, 0, 2560, 2560);*/

            EndPaint(hWnd, &ps);
            //DeleteDC(memhdc);
            //DeleteObject(memBM);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 0);
        KillTimer(hWnd, 1);
        carListVer.Clear();
        carListHor.Clear();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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


INT_PTR CALLBACK ProbDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        HWND hStaticText1 = GetDlgItem(hDlg, IDC_STATICTEXT);
        HWND hStaticText2 = GetDlgItem(hDlg, IDC_STATICTEXT2);
        HWND hStaticText3 = GetDlgItem(hDlg, IDC_STATICTEXT3);
        SetWindowText(hStaticText1, L"Sannsyn for bilar frå vest->aust: (0-100)");
        SetWindowText(hStaticText2, L"Sannsyn for bilar frå nord->sør: (0-100)");
        SetWindowText(hStaticText3, L"");
        return (INT_PTR)TRUE;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            //må hente ut tal og så lagre innhaldet
            BOOL varInt1 = true;
            int value1 = GetDlgItemInt(hDlg, IDC_EDIT1, &varInt1, false);
            BOOL varInt2 = true;
            int value2 = GetDlgItemInt(hDlg, IDC_EDIT2, &varInt2, false);
            HWND hStaticText3 = GetDlgItem(hDlg, IDC_STATICTEXT3);
            if (varInt1 == FALSE || varInt2 == FALSE) {
                SetWindowText(hStaticText3, L"Du skreiv ikkje inn eit tal!");
            }
            else if (value1 > 100 || value1 < 0 || value2 > 100 || value2 < 0) {
                SetWindowText(hStaticText3, L"Talet må vere mellom 0 og 100 (inklusivt)");
            }
            else {
                pw = value1;
                pn = value2;
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
