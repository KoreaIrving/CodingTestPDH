
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once
#include "ImageObject.h"
#include "ConfigData.h"
#include "LogData.h"


#include "opencv2\opencv.hpp"
using namespace cv;



typedef bool(*pOpenCVBlur)(Mat Mat_Input_Image, Mat& Mat_Output_Image, int nKernel_Size);
typedef bool(*pImplementBlur)(CImageObject Image_Input, CImageObject& Image_Output, int nKernel_Size);

#define TYPE_OPENCV			0
#define TYPE_IMPLEMENT		1

#define LogFilePath			_T("image_blur.log")


class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

private:
	CConfigData*	m_pConfig_Data;
	CLogData*		m_pLog_Data;
	CString			m_strTargetImage_Path;
	CString			m_strResultImage_Path;
	int				m_nKernel_Size;
	pOpenCVBlur		Blur_OpenCV;
	pImplementBlur	Blur_Implement;
	CImageObject m_Image;
	HMODULE m_hDll;
public:
	bool LoadDLL(int nType, CString& str_Error_Log);
	bool Blurring(CImageObject Image_Input, CImageObject& Image_Output, int nType, CString& str_Error_Log);
	bool MatToImageObject(Mat Mat_Input, CImageObject& Image_Output, CString &str_Error_Log);
	bool ImageObjectToMat(CImageObject Image_Input , Mat& Mat_Output, CString& str_Error_Log);
	CString GetTargetImagePath()	{ return m_strTargetImage_Path; }
	CString GetResultImagePath()	{ return m_strResultImage_Path; }
	int		GetKernelSize()		{ return m_nKernel_Size; }

	void WriteLog(CString strLog);

};


