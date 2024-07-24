
// MainProjectView.h: CMainProjectView 클래스의 인터페이스
//

#pragma once
#include "MainFrm.h"


class CMainProjectView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CMainProjectView() noexcept;
	DECLARE_DYNCREATE(CMainProjectView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_MAINPROJECT_FORM };
#endif

// 특성입니다.
public:
	CMainProjectDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMainProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CMainFrame* m_pFrame;
	CImageObject m_Target_Image;
	//CImageObject m_Result_Image;
	//std::vector< CImageObject> m_Vector_Target_Image;
	std::vector< CImageObject> m_Vector_Result_Image;


// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnBnClickedBtnSave();
	CStatic m_View_Target;
	CStatic m_View_Result;
	CListBox m_Listbox_Result;

	afx_msg void OnLbnSelchangeListResult();
	afx_msg void OnBnClickedBtnCompare();
	CProgressCtrl m_Progress_Matching;
};

#ifndef _DEBUG  // MainProjectView.cpp의 디버그 버전
inline CMainProjectDoc* CMainProjectView::GetDocument() const
   { return reinterpret_cast<CMainProjectDoc*>(m_pDocument); }
#endif

