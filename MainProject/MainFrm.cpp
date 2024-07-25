
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MainProject.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{

	m_pConfig_Data			= nullptr;
	m_pLog_Data				= nullptr;
	m_strTargetImage_Path	= _T("");
	m_strResultImage_Path	= _T("");
	m_nKernel_Size			= 0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pLog_Data = new CLogData(LogFilePath);
	if (!m_pLog_Data->OpenLogFile())
	{
		AfxMessageBox(_T("Failed to Log File Load!"));
		PostQuitMessage(0);
	}

	CString strLog(_T(""));
	strLog = _T("Success to Log File Load!");
	WriteLog(strLog);

	CFileDialog dlg(TRUE, _T("ini"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Config Files (*.ini)|*.ini||"));

	if (dlg.DoModal() == IDCANCEL) {
		return 0;
	}

	CString strConfigPath(_T(""));
	strConfigPath = dlg.GetPathName();



	m_pConfig_Data = new CConfigData(strConfigPath);
	if (!m_pConfig_Data->ReadDataFile())
	{
		AfxMessageBox(_T("Failed to Config File read!"));
		PostQuitMessage(0);
	}
	
	strLog = _T("Success to Config File Load!");
	WriteLog(strLog);

	m_strTargetImage_Path	= m_pConfig_Data -> GetLoadImagePath();
	m_strResultImage_Path	= m_pConfig_Data -> GetSaveImagePath();
	m_nKernel_Size		= m_pConfig_Data -> GetKernelSize();

	return 0;
}

bool CMainFrame::LoadDLL(int nType, CString& str_Error_Log)
{
	switch (nType)
	{
	case TYPE_OPENCV:
	{
		m_hDll = LoadLibrary(_T("OpenCVdll.dll"));
		if (!m_hDll)
		{
			str_Error_Log.Format(_T("Load OpenCV DLL Error!"));
			return false;
		}

		Blur_OpenCV = (pOpenCVBlur)GetProcAddress(m_hDll, "Filtering_OpenCV");
		if (!Blur_OpenCV)
		{
			str_Error_Log.Format(_T("Load OpenCV Blur function Error!"));
			return false;
		}
		break;
	}

	case TYPE_IMPLEMENT:
	{
		m_hDll = LoadLibrary(_T("Implementdll.dll"));
		if (!m_hDll)
		{
			str_Error_Log.Format(_T("Load Implement DLL Error!"));
			return false;
		}

		Blur_Implement = (pImplementBlur)GetProcAddress(m_hDll, "Filtering_Implement");
		if (!Blur_Implement)
		{
			str_Error_Log.Format(_T("Load Implement Blur function Error!"));
			return false;
		}
	}
	break;
	}

	return true;
}

bool CMainFrame::Blurring(CImageObject Image_Input, CImageObject& Image_Output, int nType, CString& str_Error_Log)
{

	if (!Image_Input.pBuffer)
	{
		str_Error_Log.Format(_T("Target Image Buffer is NULL!"));
		return false;
	}

	if (!LoadDLL(nType, str_Error_Log))
		return false;
	
	CString strLog(_T(""));
	if(nType == TYPE_OPENCV)
		strLog = _T("Success to Blur DLL(OpenCV) File Load!");
	if(nType == TYPE_IMPLEMENT)
		strLog = _T("Success to Blur DLL(Implement) File Load!");

	WriteLog(strLog);

	bool bResult = false;
	if (nType == TYPE_OPENCV)
	{
		Mat Mat_Input;
		if (ImageObjectToMat(Image_Input, Mat_Input, str_Error_Log) == false)
			return false;

		Mat Mat_Output;
		TRY
		{
			bResult = Blur_OpenCV(Mat_Input, Mat_Output, m_nKernel_Size);
		if (MatToImageObject(Mat_Output, Image_Output, str_Error_Log) == false)
			return false;

		}
			CATCH(CMemoryException, e)
		{
			e->ReportError();
			return false;
		}
		END_CATCH
	}

	if (nType == TYPE_IMPLEMENT)
	{
		TRY
		{
			bResult = Blur_Implement(Image_Input, Image_Output, m_nKernel_Size);
		}
			CATCH(CMemoryException, e)
		{
			e->ReportError();
			return false;
		}
		END_CATCH
	}

	FreeLibrary(m_hDll);
	return bResult;
}

bool CMainFrame::MatToImageObject(Mat Mat_Input, CImageObject& Image_Output, CString& str_Error_Log)
{
	if (Mat_Input.empty())
	{
		str_Error_Log.Format(_T("[MatToIMGObject] Input Mat is Empty!"));
		return false;
	}

	Image_Output.Init();

	Image_Output.nWidth = Mat_Input.cols;
	Image_Output.nHeight = Mat_Input.rows;
	Image_Output.nSize = Mat_Input.cols * Mat_Input.rows;

	int nBuffer_Size = sizeof(BYTE) * Image_Output.nSize;
	Image_Output.pBuffer = new BYTE[nBuffer_Size];



	memcpy(Image_Output.pBuffer, Mat_Input.data, nBuffer_Size);
	return true;
}

bool CMainFrame::ImageObjectToMat(CImageObject Image_Input, Mat& Mat_Output, CString& str_Error_Log)
{
	if (!Image_Input.pBuffer)
	{
		str_Error_Log.Format(_T("[IMGObjectToMat] Input Buffer is NULL!"));
		return false;
	}

	Mat_Output = cv::Mat(Image_Input.nHeight, Image_Input.nWidth, CV_8U, Image_Input.pBuffer).clone();

	return true;
}

void CMainFrame::WriteLog(CString strLog)
{
	m_pLog_Data->Log(strLog);
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_SYSMENU;
	cs.style &= ~WS_MINIMIZEBOX;
	cs.style &= ~WS_THICKFRAME;
	cs.style &= ~FWS_ADDTOTITLE;
	cs.hMenu = NULL;

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

