
// MainProjectView.cpp: CMainProjectView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MainProject.h"
#endif


#include "MainProjectDoc.h"
#include "MainProjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainProjectView

IMPLEMENT_DYNCREATE(CMainProjectView, CFormView)

BEGIN_MESSAGE_MAP(CMainProjectView, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CFormView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CMainProjectView::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_RUN, &CMainProjectView::OnBnClickedBtnRun)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CMainProjectView::OnBnClickedBtnSave)
	ON_LBN_SELCHANGE(IDC_LIST_RESULT, &CMainProjectView::OnLbnSelchangeListResult)
	ON_BN_CLICKED(IDC_BTN_COMPARE, &CMainProjectView::OnBnClickedBtnCompare)
END_MESSAGE_MAP()

// CMainProjectView 생성/소멸

CMainProjectView::CMainProjectView() noexcept
	: CFormView(IDD_MAINPROJECT_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMainProjectView::~CMainProjectView()
{
}

void CMainProjectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC_TARGET, m_View_Target);
	DDX_Control(pDX, IDC_VIEW_RESULT, m_View_Result);
	DDX_Control(pDX, IDC_LIST_RESULT, m_Listbox_Result);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress_Matching);
}

BOOL CMainProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CMainProjectView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_pFrame = (CMainFrame*)AfxGetMainWnd();
	m_Progress_Matching.SetRange(0, 100);


}


// CMainProjectView 인쇄

BOOL CMainProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMainProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMainProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMainProjectView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}


// CMainProjectView 진단

#ifdef _DEBUG
void CMainProjectView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMainProjectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMainProjectDoc* CMainProjectView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMainProjectDoc)));
	return (CMainProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CMainProjectView 메시지 처리기


void CMainProjectView::OnBnClickedBtnLoad()
{

	CString strLog(_T(""));

	if (!m_Target_Image.Read(m_pFrame->GetTargetImagePath(), strLog))
	{
		m_pFrame->WriteLog(strLog);
		return;
	}

	strLog.Format(_T("Success to Image Load! [Width = %d , Height = %d]"), m_Target_Image.nWidth, m_Target_Image.nHeight);
	m_pFrame->WriteLog(strLog);

	if (m_Target_Image.Draw(&m_View_Target, strLog) == false)
	{
		m_pFrame->WriteLog(strLog);
		return;
	}

}


void CMainProjectView::OnBnClickedBtnRun()
{
	CString strLog(_T(""));

	CImageObject Result_Image_OpenCV;
	if (m_pFrame->Blurring(m_Target_Image, Result_Image_OpenCV, TYPE_OPENCV, strLog) == false)
		m_pFrame->WriteLog(strLog);

	strLog = _T("Success to Image Blur(OpenCV)");
	m_pFrame->WriteLog(strLog);
	

	m_Vector_Result_Image.push_back(Result_Image_OpenCV);
	m_Listbox_Result.AddString(_T("Result_OpenCV.bmp"));



	CImageObject Result_Image_Implement;
	if (m_pFrame->Blurring(m_Target_Image, Result_Image_Implement, TYPE_IMPLEMENT, strLog) == false)
		m_pFrame->WriteLog(strLog);

	strLog = _T("Success to Image Blur(Implement)");
	m_pFrame->WriteLog(strLog);

	m_Vector_Result_Image.push_back(Result_Image_Implement);
	m_Listbox_Result.AddString(_T("Result_Implementation.bmp"));



	GetDlgItem(IDC_BTN_RUN)->EnableWindow(FALSE);

	if (m_Vector_Result_Image.back().Draw(&m_View_Result,strLog) == false)
	{
		m_pFrame->WriteLog(strLog);
		return;
	}
	int nList_Cnt = m_Vector_Result_Image.size();
	m_Listbox_Result.SetCurSel(nList_Cnt - 1);



}


void CMainProjectView::OnBnClickedBtnSave()
{
	CString strLog(_T(""));

	int nList_Index = m_Listbox_Result.GetCurSel();
	CImageObject Result_Image = m_Vector_Result_Image[nList_Index];
	CString str_FileName(_T(""));
	m_Listbox_Result.GetText(nList_Index, str_FileName);
	if (Result_Image.Write(m_pFrame->GetResultImagePath() + str_FileName, strLog) == false)
	{
		m_pFrame->WriteLog(strLog);
		return;
	}

	strLog = _T("Success to Image Save");
	m_pFrame->WriteLog(strLog);
}





void CMainProjectView::OnLbnSelchangeListResult()
{
	CString strLog(_T(""));

	int nList_Index = m_Listbox_Result.GetCurSel();


	if (nList_Index >= 0 && nList_Index < m_Listbox_Result.GetCount())
	{
		if (m_Vector_Result_Image[nList_Index].Draw(&m_View_Result, strLog) == false)
		{
			m_pFrame->WriteLog(strLog);
			return;
		}
	}
}


void CMainProjectView::OnBnClickedBtnCompare()
{
	CString strLog(_T(""));

	if (m_Vector_Result_Image.size() < 2)
		return;

	for (int i = 0; i < m_Vector_Result_Image[0].nSize; i++)
	{
		int nPos = ((double)i / (double)m_Vector_Result_Image[0].nSize) * 100;
		m_Progress_Matching.SetPos(nPos);
		if(m_Vector_Result_Image[0].pBuffer[i] != m_Vector_Result_Image[1].pBuffer[i])
		{
			
			strLog.Format(_T("[ %d ] Matching error!"), i);
			AfxMessageBox(strLog);
			m_pFrame->WriteLog(strLog);
			m_Progress_Matching.SetPos(0);
			return;
		}

	}
	GetDlgItem(IDC_STC_COMPARE)->ShowWindow(TRUE);
	m_Progress_Matching.SetPos(0);
	strLog = _T("Success to Image Matching");
	m_pFrame->WriteLog(strLog);
}
