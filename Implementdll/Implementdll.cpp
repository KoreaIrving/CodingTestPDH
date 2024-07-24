// Implementdll.cpp : DLL의 초기화 루틴을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Implementdll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 이 DLL이 MFC DLL에 대해 동적으로 링크되어 있는 경우
//		MFC로 호출되는 이 DLL에서 내보내지는 모든 함수의
//		시작 부분에 AFX_MANAGE_STATE 매크로가
//		들어 있어야 합니다.
//
//		예:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 일반적인 함수 본문은 여기에 옵니다.
//		}
//
//		이 매크로는 MFC로 호출하기 전에
//		각 함수에 반드시 들어 있어야 합니다.
//		즉, 매크로는 함수의 첫 번째 문이어야 하며
//		개체 변수의 생성자가 MFC DLL로
//		호출할 수 있으므로 개체 변수가 선언되기 전에
//		나와야 합니다.
//
//		자세한 내용은
//		MFC Technical Note 33 및 58을 참조하십시오.
//

// CImplementdllApp

BEGIN_MESSAGE_MAP(CImplementdllApp, CWinApp)
END_MESSAGE_MAP()


// CImplementdllApp 생성

CImplementdllApp::CImplementdllApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CImplementdllApp 개체입니다.

CImplementdllApp theApp;


// CImplementdllApp 초기화

BOOL CImplementdllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

bool Filtering_Implement(CImageObject Image_Input,CImageObject& Image_Output, int nKernel_Size)
{
	if (!Image_Input.pBuffer)
	{
		AfxMessageBox(_T("Input Buffer is NULL!"));
		return false;
	}

	Image_Output.Init();

	
	Image_Output.pBuffer = new BYTE[Image_Input.nSize];
	Image_Output.nWidth = Image_Input.nWidth;
	Image_Output.nHeight = Image_Input.nHeight;
	Image_Output.nSize = Image_Input.nSize;



	// 9 / 2 = 4
	int nKernel_Range = nKernel_Size/2;


	for (int y = 0; y < Image_Input.nHeight; y++)
	{
		for (int x = 0; x < Image_Input.nWidth; x++)
		{
			int nSum = 0;
			int nCount = 0;
			



			// 필터 적용
			for (int i = -nKernel_Range; i <= nKernel_Range; i++)
			{
				for (int j = -nKernel_Range; j <= nKernel_Range; j++)
				{
					int kx = x + j;
					int ky = y + i;


					// Reflect 101 border handling
					if (ky < 0) ky = -ky;
					if (kx < 0) kx = -kx;
					if (kx >= Image_Input.nWidth)
						kx = 2 * Image_Input.nWidth - kx - 2;
					if (ky >= Image_Input.nHeight)
						ky = 2 * Image_Input.nHeight - ky - 2;

					
					nSum += Image_Input.pBuffer[ky * Image_Input.nWidth + kx]; // 주변 픽셀들의 값을 다 더함
					nCount++; // 픽셀의 수 카운트

				}
			}
			// 평균값 계산
			BYTE average = round((double)nSum / (double)nCount); // 주변 픽셀들의 총합을 픽셀 수로 나눠서 평균

			// 결과를 임시 이미지에 저장
			Image_Output.pBuffer[y * Image_Input.nWidth + x] = average;
		}
	}


        return true;
}