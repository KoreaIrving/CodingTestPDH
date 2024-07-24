#pragma once


class CImageObject
{

public:
    CImageObject();
    virtual ~CImageObject();

    BYTE* pBuffer;

    int nWidth;
    int nHeight;
    int nSize;

    void Init();

    bool Read(CString strPath_Name, CString& strError_Log);

    bool Draw(CWnd* pWnd, CString& str_Error_Log);

    bool Write(CString str_Path_Name, CString& str_Error_Log);
   
    bool Flip(BYTE* pBuffer_Input, BYTE* pBuffer_Output);

};

