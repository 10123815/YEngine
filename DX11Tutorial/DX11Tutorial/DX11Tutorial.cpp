// DX11Tutorial.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "DX11Tutorial.h"
#include "Engine.h"
#include "Scene.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HWND hWnd;
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
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

	// TODO: �ڴ˷��ô��롣

	// ��ʼ��ȫ���ַ���
	LoadStringW (hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW (hInstance, IDC_DX11TUTORIAL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass (hInstance);

	// ִ��Ӧ�ó����ʼ��: 
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

	// ����Ϣѭ��: 
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance (HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		{
			int wmId = LOWORD (wParam);
			// �����˵�ѡ��: 
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
