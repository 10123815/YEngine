// DX11Tutorial.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "DX11Tutorial.h"
#include "Engine.h"
#include "Scene.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HWND hWnd;
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass (HINSTANCE hInstance);
BOOL                InitInstance (HINSTANCE, int);
LRESULT CALLBACK    WndProc (HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain (_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPWSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER (hPrevInstance);
	UNREFERENCED_PARAMETER (lpCmdLine);

	// TODO: 在此放置代码。

	// 初始化全局字符串
	LoadStringW (hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW (hInstance, IDC_DX11TUTORIAL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass (hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators (hInstance, MAKEINTRESOURCE (IDC_DX11TUTORIAL));

	MSG msg;

	auto& engine = ysd_simple_engine::Engine::Instance ( );
	engine->Initialize (hWnd);

	ysd_simple_engine::Scene* p_scene = new ysd_simple_engine::Scene;
	p_scene->Init ( );

	// 主消息循环: 
	while (true)
	{
		if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}
		else
		{
			engine->Update ( );
		}

	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass (HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof (WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon (hInstance, MAKEINTRESOURCE (IDI_DX11TUTORIAL));
	wcex.hCursor = LoadCursor (nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon (wcex.hInstance, MAKEINTRESOURCE (IDI_SMALL));

	return RegisterClassExW (&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance (HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindowW (szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
							   0, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow (hWnd, nCmdShow);
	UpdateWindow (hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		{
			int wmId = LOWORD (wParam);
			// 分析菜单选择: 
			switch (wmId)
			{
				case IDM_EXIT:
					DestroyWindow (hWnd);
					break;
				default:
					return DefWindowProc (hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_DESTROY:
			PostQuitMessage (0);
			break;
		default:
			return DefWindowProc (hWnd, message, wParam, lParam);
	}
	return 0;
}
