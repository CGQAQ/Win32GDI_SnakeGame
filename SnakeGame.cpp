// SnakeGame.cpp
//Author：CG
//贪吃蛇小游戏

#include "stdafx.h"
#include "SnakeGame.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

#define MAX_LOADSTRING 100

#define BORDER  (10)
#define SNAKE_SIZE (20)
#define GAME_WIDTH (30)
#define GAME_HEIGHT (20)
#define INFO_WIDTH (10)
#define SNAKE_MAX_LENGTH 20

#define SNAKE_MOVE_TIMERID 1
#define SNAKE_MOVE_TIMER_INTERVAL 500


std::vector<POINT> vSnake;
POINT ptDirection = { 1,0 };
INT iSnakeLen = 4;
INT iSnakeTail = 0;
INT iSnakeHead = 3;

POINT ptFood;									//食物坐标


HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            
HWND hWnd;										
BOOL isGameStart = FALSE;

RECT rect;
HBRUSH game_background_brush;
INT wndWidth, wndHeight;
INT clientWidth, clientHeight;


INT iScore = 0;



ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	

	
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SNAKEGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKEGAME));

	MSG msg;

	
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKEGAME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SNAKEGAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.cbSize = sizeof(WNDCLASSEX);

	return RegisterClassExW(&wcex);
}




BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; 

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW^WS_MAXIMIZEBOX ^WS_SIZEBOX,
		0, 0, 1030, 520, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

VOID RefreshFood(HDC hdc)
{
	if (ptFood.x == -1)
	{
		srand(time(NULL));
		INT iSnakeX = rand() % 30;
		INT iSnakeY = rand() % 20;
		ptFood = { iSnakeX, iSnakeY };
	}


	HRGN hRgn = CreateRectRgn(BORDER + ptFood.x*SNAKE_SIZE, BORDER + ptFood.y*SNAKE_SIZE, BORDER + (ptFood.x + 1)*SNAKE_SIZE, BORDER + (ptFood.y + 1)*SNAKE_SIZE);
	FillRgn(hdc, hRgn, (HBRUSH)::GetStockObject(BLACK_BRUSH));
}

VOID InitGame()
{
	iSnakeHead = 3;
	iSnakeTail = 0;
	iSnakeLen = 4;
	ptDirection = { 1,0 };
	ptFood = { -1,-1 };
	iScore = 0;
	vSnake.clear();
	vSnake.resize(SNAKE_MAX_LENGTH);
	for (INT i = 0; i < 4; i++)
	{
		vSnake[i].x = i*SNAKE_SIZE;
		vSnake[i].y = 0;
	}
	InvalidateRect(hWnd, NULL, TRUE);
}

void DrawSnakeEyes(INT i, HDC hdc)
{
	if (i == iSnakeHead)
	{
		INT iSnakeHeadX = vSnake[iSnakeHead].x;
		INT iSnakeHeadY = vSnake[iSnakeHead].y;
		if (ptDirection.x == 1)
		{
			HRGN hRgn1 = CreateEllipticRgn(BORDER + iSnakeHeadX + 10, BORDER + iSnakeHeadY + 3, BORDER + iSnakeHeadX + 15, BORDER + iSnakeHeadY + 8);
			FillRgn(hdc, hRgn1, (HBRUSH)::GetStockObject(BLACK_BRUSH));

			HRGN hRgn2 = CreateEllipticRgn(BORDER + iSnakeHeadX + 10, BORDER + iSnakeHeadY + 12, BORDER + iSnakeHeadX + 15, BORDER + iSnakeHeadY + 17);
			FillRgn(hdc, hRgn2, (HBRUSH)::GetStockObject(BLACK_BRUSH));
		}
		if (ptDirection.x == -1)
		{
			HRGN hRgn1 = CreateEllipticRgn(BORDER + iSnakeHeadX + 5, BORDER + iSnakeHeadY + 3, BORDER + iSnakeHeadX + 10, BORDER + iSnakeHeadY + 8);
			FillRgn(hdc, hRgn1, (HBRUSH)::GetStockObject(BLACK_BRUSH));

			HRGN hRgn2 = CreateEllipticRgn(BORDER + iSnakeHeadX + 5, BORDER + iSnakeHeadY + 12, BORDER + iSnakeHeadX + 10, BORDER + iSnakeHeadY + 17);
			FillRgn(hdc, hRgn2, (HBRUSH)::GetStockObject(BLACK_BRUSH));
		}
		if (ptDirection.y == 1)
		{
			HRGN hRgn1 = CreateEllipticRgn(BORDER + iSnakeHeadX + 5, BORDER + iSnakeHeadY + 12, BORDER + iSnakeHeadX + 10, BORDER + iSnakeHeadY + 17);
			FillRgn(hdc, hRgn1, (HBRUSH)::GetStockObject(BLACK_BRUSH));

			HRGN hRgn2 = CreateEllipticRgn(BORDER + iSnakeHeadX + 12, BORDER + iSnakeHeadY + 12, BORDER + iSnakeHeadX + 17, BORDER + iSnakeHeadY + 17);
			FillRgn(hdc, hRgn2, (HBRUSH)::GetStockObject(BLACK_BRUSH));
		}
		if (ptDirection.y == -1)
		{
			HRGN hRgn1 = CreateEllipticRgn(BORDER + iSnakeHeadX + 3, BORDER + iSnakeHeadY + 3, BORDER + iSnakeHeadX + 8, BORDER + iSnakeHeadY + 8);
			FillRgn(hdc, hRgn1, (HBRUSH)::GetStockObject(BLACK_BRUSH));

			HRGN hRgn2 = CreateEllipticRgn(BORDER + iSnakeHeadX + 12, BORDER + iSnakeHeadY + 3, BORDER + iSnakeHeadX + 17, BORDER + iSnakeHeadY + 8);
			FillRgn(hdc, hRgn2, (HBRUSH)::GetStockObject(BLACK_BRUSH));
		}
	}
	return;
}

VOID AddSnakeLen()
{
	if (iSnakeLen < SNAKE_MAX_LENGTH)
	{
		// 从头部添加
		//if (iSnakeHead > iSnakeTail)
		//{
		//	if (iSnakeHead + 1 >= SNAKE_MAX_LENGTH)
		//	{
		//		if (ptDirection.x == 1)
		//		{
		//			
		//			vSnake[0] = { vSnake[iSnakeHead].x + SNAKE_SIZE,vSnake[iSnakeHead].y };
		//			iSnakeHead = 0;
		//		}
		//		else if (ptDirection.x == -1)
		//		{
		//			
		//			vSnake[0] = { vSnake[iSnakeHead].x - SNAKE_SIZE,vSnake[iSnakeHead].y };
		//			iSnakeHead = 0;
		//		}
		//		else if (ptDirection.y == 1)
		//		{
		//			
		//			vSnake[0] = { vSnake[iSnakeHead].x ,vSnake[iSnakeHead].y + SNAKE_SIZE };
		//			iSnakeHead = 0;
		//		}
		//		else if (ptDirection.y == -1)
		//		{
		//			
		//			vSnake[0] = { vSnake[iSnakeHead].x,vSnake[iSnakeHead].y - SNAKE_SIZE };
		//			iSnakeHead = 0;
		//		}
		//	}
		//	else
		//	{
		//		if (ptDirection.x == 1)
		//		{
		//			
		//			vSnake[iSnakeHead + 1] = { vSnake[iSnakeHead].x + SNAKE_SIZE,vSnake[iSnakeHead].y };
		//			iSnakeHead++;
		//		}
		//		else if (ptDirection.x == -1)
		//		{
		//			
		//			vSnake[iSnakeHead + 1] = { vSnake[iSnakeHead].x - SNAKE_SIZE,vSnake[iSnakeHead].y };
		//			iSnakeHead++;
		//		}
		//		else if (ptDirection.y == 1)
		//		{
		//			
		//			vSnake[iSnakeHead + 1] = { vSnake[iSnakeHead].x ,vSnake[iSnakeHead].y + SNAKE_SIZE };
		//			iSnakeHead++;
		//		}
		//		else if (ptDirection.y == -1)
		//		{
		//			
		//			vSnake[iSnakeHead + 1] = { vSnake[iSnakeHead].x,vSnake[iSnakeHead].y - SNAKE_SIZE };
		//			iSnakeHead++;
		//		}
		//	}
		//}
		//else
		//{
		//	if (ptDirection.x == 1)
		//	{
		//		
		//		vSnake[iSnakeHead + 1] = { vSnake[iSnakeHead].x + SNAKE_SIZE,vSnake[iSnakeHead].y };
		//		iSnakeHead++;
		//	}
		//	else if (ptDirection.x == -1)
		//	{
		//		//У?УВсЙ?УГУ?У?ТЖХИ
		//		vSnake[iSnakeHead + 1] = { vSnake[iSnakeHead].x - SNAKE_SIZE,vSnake[iSnakeHead].y };
		//		iSnakeHead++;
		//	}
		//	else if (ptDirection.y == 1)
		//	{
		//		//У?УВУ?У?У?У?ТЖХИ
		//		vSnake[iSnakeHead + 1] = { vSnake[iSnakeHead].x ,vSnake[iSnakeHead].y + SNAKE_SIZE };
		//		iSnakeHead++;
		//	}
		//	else if (ptDirection.y == -1)
		//	{
		//		//У?УВУ?У?У?У?ТЖХИ
		//		vSnake[iSnakeHead + 1] = { vSnake[iSnakeHead].x,vSnake[iSnakeHead].y - SNAKE_SIZE };
		//		iSnakeHead++;
		//	}
		//}

		//...У?УВсК?УГФ?У?У?У?
		if (iSnakeTail > 0)
		{
			if (ptDirection.x == 1)
			{
				//У?УВУ?У?У?У?ТЖХИ
				vSnake[iSnakeTail - 1] = { vSnake[iSnakeTail].x - SNAKE_SIZE,vSnake[iSnakeTail].y };
				iSnakeTail--;
			}
			else if (ptDirection.x == -1)
			{
				//У?УВсЙ?УГУ?У?ТЖХИ
				vSnake[iSnakeTail - 1] = { vSnake[iSnakeTail].x + SNAKE_SIZE,vSnake[iSnakeTail].y };
				iSnakeTail--;
			}
			else if (ptDirection.y == 1)
			{
				//У?УВУ?У?У?У?ТЖХИ
				vSnake[iSnakeTail - 1] = { vSnake[iSnakeTail].x ,vSnake[iSnakeTail].y - SNAKE_SIZE };
				iSnakeTail--;
			}
			else if (ptDirection.y == -1)
			{
				//У?УВУ?У?У?У?ТЖХИ
				vSnake[iSnakeTail - 1] = { vSnake[iSnakeTail].x ,vSnake[iSnakeTail].y + SNAKE_SIZE };
				iSnakeTail--;
			}
		}
		else
		{
			if (ptDirection.x == 1)
			{
				//У?УВУ?У?У?У?ТЖХИ
				vSnake[SNAKE_MAX_LENGTH - 1] = { vSnake[iSnakeTail].x - SNAKE_SIZE,vSnake[iSnakeTail].y };
				iSnakeTail = SNAKE_MAX_LENGTH - 1;
			}
			else if (ptDirection.x == -1)
			{
				//У?УВсЙ?УГУ?У?ТЖХИ
				vSnake[SNAKE_MAX_LENGTH - 1] = { vSnake[iSnakeTail].x + SNAKE_SIZE,vSnake[iSnakeTail].y };
				iSnakeTail = SNAKE_MAX_LENGTH - 1;
			}
			else if (ptDirection.y == 1)
			{
				//У?УВУ?У?У?У?ТЖХИ
				vSnake[SNAKE_MAX_LENGTH - 1] = { vSnake[iSnakeTail].x ,vSnake[iSnakeTail].y - SNAKE_SIZE };
				iSnakeTail = SNAKE_MAX_LENGTH - 1;
			}
			else if (ptDirection.y == -1)
			{
				//У?УВУ?У?У?У?ТЖХИ
				vSnake[SNAKE_MAX_LENGTH - 1] = { vSnake[iSnakeTail].x ,vSnake[iSnakeTail].y + SNAKE_SIZE };
				iSnakeTail = SNAKE_MAX_LENGTH - 1;
			}
		}

		iSnakeLen++;
		iScore += 10;
	}
	return;
}

VOID DrawGame(HDC hdc)
{

	HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	SelectObject(hdc, hBrush);
	if (iSnakeHead > iSnakeTail)
	{
		for (INT i = iSnakeTail; i <= iSnakeHead; i++)
		{
			Rectangle(hdc, BORDER + vSnake[i].x, BORDER + vSnake[i].y, BORDER + vSnake[i].x + SNAKE_SIZE, BORDER + vSnake[i].y + SNAKE_SIZE);
			DrawSnakeEyes(i, hdc);

		}
	}
	else
	{
		for (INT i = iSnakeTail; i < SNAKE_MAX_LENGTH; i++)
		{
			Rectangle(hdc, BORDER + vSnake[i].x, BORDER + vSnake[i].y, BORDER + vSnake[i].x + SNAKE_SIZE, BORDER + vSnake[i].y + SNAKE_SIZE);
		}
		for (INT i = 0; i <= iSnakeHead; i++)
		{
			Rectangle(hdc, BORDER + vSnake[i].x, BORDER + vSnake[i].y, BORDER + vSnake[i].x + SNAKE_SIZE, BORDER + vSnake[i].y + SNAKE_SIZE);
			DrawSnakeEyes(i, hdc);
		}
	}

	RefreshFood(hdc);
	return;
}

VOID MoveSnake()
{
	if (ptDirection.x == 1)
	{
		if ((vSnake[iSnakeHead].x + SNAKE_SIZE) >= (GAME_WIDTH*SNAKE_SIZE))//撞墙
		{
			KillTimer(hWnd, SNAKE_MOVE_TIMERID);
			MessageBox(hWnd, TEXT("你撞墙了！"), TEXT("警告："), MB_OK);
			isGameStart = FALSE;
			InitGame();
			return;
		}
		
		if (iSnakeLen <= SNAKE_MAX_LENGTH)
		{
			if (iSnakeHead + 1 < SNAKE_MAX_LENGTH)
			{
				POINT newHead = { vSnake[iSnakeHead].x + SNAKE_SIZE, vSnake[iSnakeHead].y };
				vSnake[iSnakeHead + 1] = newHead;
				iSnakeHead++;
				if (iSnakeTail + 1 < SNAKE_MAX_LENGTH)
				{
					iSnakeTail++;
				}
				else
				{
					iSnakeTail = 0;
				}
				InvalidateRect(hWnd, NULL, TRUE);
				//DRAWGAME();
			}
			else
			{
				POINT newHead = { vSnake[iSnakeHead].x + SNAKE_SIZE, vSnake[iSnakeHead].y };
				vSnake[0] = newHead;

				iSnakeTail++;
				iSnakeHead = 0;
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
	}
	else if (ptDirection.x == -1)
	{
		if ((vSnake[iSnakeHead].x) <= 0)
		{
			KillTimer(hWnd, SNAKE_MOVE_TIMERID);
			MessageBox(hWnd, TEXT("你撞墙了！"), TEXT("警告："), MB_OK);
			isGameStart = FALSE;
			InitGame();
			return;
		}
		//У?УВсЙ?УГУ?У?ТЖХИ
		if (iSnakeLen <= SNAKE_MAX_LENGTH)
		{
			if (iSnakeHead + 1 < SNAKE_MAX_LENGTH)
			{
				POINT newHead = { vSnake[iSnakeHead].x - SNAKE_SIZE, vSnake[iSnakeHead].y };
				vSnake[iSnakeHead + 1] = newHead;
				iSnakeHead++;
				if (iSnakeTail + 1 < SNAKE_MAX_LENGTH)
				{
					iSnakeTail++;
				}
				else
				{
					iSnakeTail = 0;
				}
				InvalidateRect(hWnd, NULL, TRUE);
				//DRAWGAME();
			}
			else
			{
				POINT newHead = { vSnake[iSnakeHead].x - SNAKE_SIZE, vSnake[iSnakeHead].y };
				vSnake[0] = newHead;

				iSnakeTail++;
				iSnakeHead = 0;
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
	}
	else if (ptDirection.y == 1)
	{
		if ((vSnake[iSnakeHead].y + SNAKE_SIZE) >= (GAME_HEIGHT*SNAKE_SIZE))//У?УЖсЙ?Ф?сЛГУ?Ф?У?
		{
			KillTimer(hWnd, SNAKE_MOVE_TIMERID);
			MessageBox(hWnd, TEXT("你撞墙了！"), TEXT("警告："), MB_OK);
			isGameStart = FALSE;
			InitGame();
			return;
		}
		//У?УВУ?У?У?У?ТЖХИ
		if (iSnakeLen <= SNAKE_MAX_LENGTH)
		{
			if (iSnakeHead + 1 < SNAKE_MAX_LENGTH)
			{
				POINT newHead = { vSnake[iSnakeHead].x, vSnake[iSnakeHead].y + SNAKE_SIZE };
				vSnake[iSnakeHead + 1] = newHead;
				iSnakeHead++;
				if (iSnakeTail + 1 < SNAKE_MAX_LENGTH)
				{
					iSnakeTail++;
				}
				else
				{
					iSnakeTail = 0;
				}
				InvalidateRect(hWnd, NULL, TRUE);
				//DRAWGAME();
			}
			else
			{
				POINT newHead = { vSnake[iSnakeHead].x, vSnake[iSnakeHead].y + SNAKE_SIZE };
				vSnake[0] = newHead;

				iSnakeTail++;
				iSnakeHead = 0;
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
	}
	else if (ptDirection.y == -1)
	{
		if ((vSnake[iSnakeHead].y) <= 0)//У?УЖсЙ?Ф?сЛГУ?Ф?У?
		{
			KillTimer(hWnd, SNAKE_MOVE_TIMERID);
			MessageBox(hWnd, TEXT("你撞墙了！"), TEXT("警告："), MB_OK);
			isGameStart = FALSE;
			InitGame();
			return;
		}
		//У?УВУ?У?У?У?ТЖХИ
		if (iSnakeLen <= SNAKE_MAX_LENGTH)
		{
			if (iSnakeHead + 1 < SNAKE_MAX_LENGTH)
			{
				POINT newHead = { vSnake[iSnakeHead].x, vSnake[iSnakeHead].y - SNAKE_SIZE };
				vSnake[iSnakeHead + 1] = newHead;
				iSnakeHead++;
				if (iSnakeTail + 1 < SNAKE_MAX_LENGTH)
				{
					iSnakeTail++;
				}
				else
				{
					iSnakeTail = 0;
				}
				InvalidateRect(hWnd, NULL, TRUE);
				//DRAWGAME();
			}
			else
			{
				POINT newHead = { vSnake[iSnakeHead].x, vSnake[iSnakeHead].y - SNAKE_SIZE };
				vSnake[0] = newHead;

				iSnakeTail++;
				iSnakeHead = 0;
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
	}
	else
	{
		return;
	}
}
BOOL IsSnakeEatFood()
{
	if (vSnake[iSnakeHead].x == ptFood.x*SNAKE_SIZE&&vSnake[iSnakeHead].y == ptFood.y*SNAKE_SIZE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL IsHitItself()
{
	if (iSnakeHead > iSnakeTail)
	{
		for (INT i = iSnakeTail; i < iSnakeHead; i++)
		{
			for (INT j = i + 1; j <= iSnakeHead; j++)
			{
				if (vSnake[i].x == vSnake[j].x&&vSnake[i].y == vSnake[j].y)
				{
					return TRUE;
				}
			}
		}
	}
	else
	{
		for (INT i = 0; i < SNAKE_MAX_LENGTH - 1; i++)
		{
			for (INT j = i + 1; j < SNAKE_MAX_LENGTH; j++)
			{
				if (vSnake[i].x == vSnake[j].x&&vSnake[i].y == vSnake[j].y)
				{
					return TRUE;
				}
				if (j == iSnakeHead)
				{
					j = iSnakeTail;
				}
			}
			if (i == iSnakeHead)
			{
				i = iSnakeTail;
			}
		}
	}
	return FALSE;
}
VOID CALLBACK SnakeMoveTimerCallBack(HWND hWnd, UINT uInt, UINT_PTR uPTR, DWORD dWord)
{
	MoveSnake();
	if (IsSnakeEatFood())
	{
		AddSnakeLen();
		ptFood.x = -1;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		InvalidateRect(hWnd, NULL, TRUE);
		RefreshFood(hdc);
		EndPaint(hWnd, &ps);
	}
	if (IsHitItself())
	{
		KillTimer(hWnd, SNAKE_MOVE_TIMERID);
		MessageBoxW(hWnd, L"你撞到你自己了！", L"警告：", MB_OK);
		InitGame();
	}
}


//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
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
		case WM_CREATE:
			::hWnd = hWnd;
			GetWindowRect(hWnd, &rect);
			wndWidth = rect.right - rect.left;
			wndHeight = rect.bottom - rect.top;

			GetClientRect(hWnd, &rect);
			clientHeight = rect.bottom - rect.top;
			clientWidth = rect.right - rect.left;

			MoveWindow(hWnd, 0, 0, (GAME_WIDTH + INFO_WIDTH)*SNAKE_SIZE + BORDER * 3 + (wndWidth - clientWidth), (wndHeight - clientHeight) + BORDER * 2 + GAME_HEIGHT*SNAKE_SIZE, TRUE);
			InitGame();


			break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
		
			game_background_brush = (HBRUSH)GetStockObject(GRAY_BRUSH);
			SelectObject(hdc, game_background_brush);

			
			Rectangle(hdc, BORDER, BORDER, BORDER + GAME_WIDTH*SNAKE_SIZE, GAME_HEIGHT*SNAKE_SIZE + BORDER);

			
			Rectangle(hdc, BORDER * 2 + GAME_WIDTH*SNAKE_SIZE, BORDER, BORDER * 2 + GAME_WIDTH*SNAKE_SIZE + INFO_WIDTH*SNAKE_SIZE, GAME_HEIGHT*SNAKE_SIZE + BORDER);
			HBRUSH game_score_background = (HBRUSH)GetStockObject(WHITE_BRUSH);
			SelectObject(hdc, game_score_background);
			Rectangle(hdc, BORDER * 3 + GAME_WIDTH*SNAKE_SIZE, BORDER * 8, BORDER * 3 + GAME_WIDTH*SNAKE_SIZE + INFO_WIDTH*SNAKE_SIZE - BORDER * 2, BORDER * 8 + BORDER * 6);
			RECT rectScore = { BORDER * 3 + GAME_WIDTH*SNAKE_SIZE ,BORDER * 10, BORDER * 3 + GAME_WIDTH*SNAKE_SIZE + INFO_WIDTH*SNAKE_SIZE - BORDER * 2, BORDER * 10 + BORDER * 6 };

			HANDLE  hHeap = ::GetProcessHeap();
			WCHAR* wsScore = (WCHAR*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 128);
			wsprintfW(wsScore, L"分数： %d", iScore);
			DrawTextExW(hdc, wsScore, wcslen(wsScore), &rectScore, DT_CENTER, NULL);
			HeapFree(hHeap, NULL, wsScore);
			DrawGame(hdc);
			RefreshFood(hdc);


			EndPaint(hWnd, &ps);
		}
		break;
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_ADD:
					AddSnakeLen();
					break;
				case VK_UP:
					if (ptDirection.y != 1)
					{
						ptDirection = { 0, -1 };
					}
					break;

				case VK_DOWN:
					if (ptDirection.y != -1)
					{
						ptDirection = { 0, 1 };
					}
					break;
				case VK_LEFT:
					if (ptDirection.x != 1)
					{
						ptDirection = { -1, 0 };
					}
					break;
				case VK_RIGHT:
					if (ptDirection.x != -1)
					{
						ptDirection = { 1, 0 };
					}
					break;
				case VK_HOME:
					if (!isGameStart)
					{
						SetTimer(hWnd, SNAKE_MOVE_TIMERID, SNAKE_MOVE_TIMER_INTERVAL, &SnakeMoveTimerCallBack);
						isGameStart = TRUE;
					}
					else
					{
						KillTimer(hWnd, SNAKE_MOVE_TIMERID);
						isGameStart = FALSE;
					}
					break;
				case VK_END:
					KillTimer(hWnd, SNAKE_MOVE_TIMERID);
					InitGame();
					isGameStart = FALSE;
					break;
				default:
					break;
			}
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


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
