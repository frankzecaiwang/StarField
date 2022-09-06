#ifndef WINVER
#define WINVER 0x0501  //#define _WIN32_WINNT_WINXP                  0x0501 // Windows XP
#endif

#include <windows.h>
#include <wingdi.h>
#include "star_motion.h"
#include "time.h"
#include "math.h"
//#include "MP3Player.h"


void OnPaint(HWND);
void OnResize(HWND);
void OnTimer(HWND);
void OnMouseLBUPEvent(HWND hwnd, LPARAM lParam);

void initialization(HWND);

const char g_szClassName[] = "myWindowClass";

STAR_STRUCT g_aStarList[MAX_STAR];

//MP3Player g_player;

//	g_player.OpenFromFile(L"demo.mp3");

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
        	break;
        
		case WM_DESTROY:
            PostQuitMessage(0);
        	break;
        	
        case WM_PAINT:
        	OnPaint(hwnd);
            break;

        case WM_SIZE:
        	OnResize(hwnd);
            break;
            
         case WM_TIMER:
         	OnTimer(hwnd);
         	break;
        	
   	    case WM_LBUTTONDOWN:

			OnMouseLBUPEvent(hwnd, lParam);
			
        	break;
         	
   	    case WM_KEYDOWN:
        	switch (wParam)
        	{
        		case VK_ESCAPE:
            		PostQuitMessage(0);
            		break;
        	}
        	break;
        
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(0,0,0));//(HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Shooting Stars",
        WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
        CW_USEDEFAULT, CW_USEDEFAULT, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    initialization(hwnd);
    
    SetTimer(hwnd,             // handle to main window 
    	0,                         // timer identifier 
    	TIMER_LEN,                      // 25ms interval 
    	(TIMERPROC) NULL);         // no timer callback 

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}


void initialization(HWND hwnd)
{		
	RECT wndsize;
	GetWindowRect(hwnd, &wndsize);
	
	g_init_width      = wndsize.right-wndsize.left;
	g_init_height       = wndsize.bottom-wndsize.top;
	g_current_width   = g_init_width;
	g_current_height    = g_init_height;
	
	srand(time(NULL));
	
	for(int i=0; i < MAX_STAR; i++)
	{
		g_aStarList[i].set_dir(ERUPTION);
		g_aStarList[i].initialization(FIRST_INIT);			
	}
	
}

void OnTimer(HWND hwnd)
{
	
	for(int i=0; i < MAX_STAR; i++)
	{
		g_aStarList[i].step_on();
	}
		
	RedrawWindow(hwnd,NULL,NULL,RDW_INVALIDATE | RDW_INTERNALPAINT);	
}

void OnPaint(HWND hwnd)
{

	PAINTSTRUCT ps;
 	HRGN bgRgn;
    HBRUSH hBrush_star;
    RECT clientRect;
    HDC hdc;
    HBRUSH hBrush;
    
    RECT rect_star;
    
    float length_ratio_scale, width_ratio_scale;
    
    double dRadius;
    
 	hdc = BeginPaint(hwnd, &ps);
     
  	GetClientRect(hwnd, &clientRect);
   	//bgRgn = CreateRectRgnIndirect(&clientRect);
    hBrush = CreateSolidBrush(RGB(0,0,0));
    //FillRgn(hdc, bgRgn, hBrush);
    
    FillRect(hdc,&clientRect,hBrush);
    
    
    length_ratio_scale = g_current_width*1.0/g_init_width;
    width_ratio_scale  = g_current_height*1.0/g_init_height;
    
    //SelectObject(hdc, GetStockObject(DC_BRUSH)); 
    
    for(int i=0; i < MAX_STAR; i++)
    {    	
    	int width, height;
    	
    	g_aStarList[i].get_current_xy((int*)&rect_star.left, (int *)&rect_star.top, &width, &height);
    	
    	rect_star.left = long(rect_star.left * length_ratio_scale + g_current_width / 2);	
    	rect_star.top = long(rect_star.top * width_ratio_scale + g_current_height / 2);
    	
		rect_star.right = long(rect_star.left + width * length_ratio_scale);
    	rect_star.bottom = long(rect_star.top + height * width_ratio_scale);

    	
    	hBrush_star = CreateSolidBrush(RGB(g_aStarList[i].color_r,g_aStarList[i].color_g,g_aStarList[i].color_b));
    	
		//SetDCBrushColor(hdc, RGB(g_aStarList[i].color_r,g_aStarList[i].color_g,g_aStarList[i].color_b));
    	
    	FillRect(hdc, &rect_star, hBrush_star);
    	
    	DeleteObject(hBrush_star); 
    	
    	//Rectangle(hdc,rect_star.left, rect_star.top, rect_star.right, rect_star.bottom);
    }
    
    //SelectObject(hdc, hBrush_old); 
    
    SetDCBrushColor(hdc, RGB(255,255,255));
    
   	//DeleteObject(bgRgn);
    DeleteObject(hBrush);    
      
    EndPaint(hwnd, &ps);
    	
}


void OnResize(HWND hwnd)
{
	RECT wndsize;
	GetWindowRect(hwnd, &wndsize);

	g_current_width      = wndsize.right-wndsize.left;
	g_current_height       = wndsize.bottom-wndsize.top;
}


void OnMouseLBUPEvent(HWND hwnd, LPARAM lParam)
{
	//pt.x = GET_X_LPARAM(lParam);
	//pt.y = GET_Y_LPARAM(lParam);	

	for(int i=0; i < MAX_STAR; i++)
	{
		g_aStarList[i].set_dir(g_aStarList[i].m_dir==ERUPTION? FALLING:ERUPTION);			
	}	
}

