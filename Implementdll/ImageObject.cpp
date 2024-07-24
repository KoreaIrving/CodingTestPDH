#include "pch.h"
#include "ImageObject.h"



CImageObject::CImageObject()
{
    pBuffer = NULL;

    Init();
}


CImageObject::~CImageObject()
{

}

void CImageObject::Init()
{
    nWidth = 0;
    nHeight = 0;
    nSize = 0;


    if (pBuffer)
        delete[] pBuffer;
    pBuffer = NULL;
}

bool CImageObject::Read(CString str_Path_Name, CString& str_Error_Log)
{
    Init();

    BITMAPFILEHEADER BmFh;    // 비트맵 파일헤드 구조체
    BITMAPINFOHEADER BmIh;    // 비트맵 영상헤드 구조체
    RGBQUAD RgbInfo[256];

    CFile hFile;


    if (!hFile.Open(str_Path_Name, CFile::modeRead | CFile::typeBinary))
    {
        str_Error_Log.Format(_T("[Read] Image File Open Error!"));
        return false;
    }

    hFile.Read(&BmFh, sizeof(BITMAPFILEHEADER));
    if (BmFh.bfType != 0x4D42)
    {
        str_Error_Log.Format(_T("[Read] Image Type Error!"));
        return false;
    }

    hFile.Read(&BmIh, sizeof(BITMAPINFOHEADER));
    if (BmIh.biBitCount != 8)
    {
        str_Error_Log.Format(_T("[Read] Image Bit Count Error!"));
        return false;
    }
    hFile.Read(RgbInfo, sizeof(RGBQUAD) * 256);

    nWidth = BmIh.biWidth;
    nHeight = BmIh.biHeight;
    nSize = BmIh.biSizeImage;



    if (pBuffer) delete[] pBuffer;
    pBuffer = NULL;


    BYTE* pOrigin_Buffer = new BYTE[nSize];
    hFile.Read(pOrigin_Buffer, nSize);


    hFile.Close();


    pBuffer = new BYTE[nSize];
    if (!Flip(pOrigin_Buffer, pBuffer))
    {
        str_Error_Log.Format(_T("[Read] Image Flip Error!"));
        return false;
    }



    return true;


}

bool CImageObject::Write(CString str_Path_Name, CString& str_Error_Log)
{
    if (!pBuffer)
    {
        str_Error_Log.Format(_T("[Write] Image Buffer is NULL!"));
        return false;
    }

    BYTE* pFlip_Buffer = new BYTE[nSize];
    Flip(pBuffer, pFlip_Buffer);

    BITMAPFILEHEADER BmFh;    // 비트맵 파일헤드 구조체
    BITMAPINFOHEADER BmIh;    // 비트맵 영상헤드 구조체

    BmFh.bfSize = sizeof(BITMAPFILEHEADER);
    BmFh.bfType = 0x4D42;
    BmFh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (256 * sizeof(RGBQUAD));
    BmIh.biSize = 40;
    BmIh.biWidth = nWidth;
    BmIh.biHeight = nHeight;
    BmIh.biPlanes = 1;
    BmIh.biBitCount = 8;
    BmIh.biCompression = BI_RGB;
    BmIh.biSizeImage = nWidth * nHeight;
    BmIh.biXPelsPerMeter = 0;
    BmIh.biYPelsPerMeter = 0;
    BmIh.biClrUsed = 0;
    BmIh.biClrImportant = 0;


    RGBQUAD rgbInfo[256];

    for (int i = 0; i < 256; i++)
    {
        rgbInfo[i].rgbBlue = i;
        rgbInfo[i].rgbGreen = i;
        rgbInfo[i].rgbRed = i;
        rgbInfo[i].rgbReserved = 0;
    }



    CFile file;
    if (!file.Open(str_Path_Name, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeReadWrite))
    {
        str_Error_Log.Format(_T("[Write] Image File Open Error!"));
        return false;
    }


    file.Write(&BmFh, sizeof(BITMAPFILEHEADER));
    file.Write(&BmIh, sizeof(BITMAPINFOHEADER));
    file.Write(rgbInfo, sizeof(RGBQUAD) * 256);


    file.Write(pFlip_Buffer, nSize);
    file.Close();

    return true;
}

bool CImageObject::Flip(BYTE* pBuffer_Input, BYTE* pBuffer_Output)
{

    TRY
    {
        for (int i = 0; i < nHeight; i++) // row
        {
            for (int j = 0; j < nWidth; j++) // col
            {
                pBuffer_Output[(nHeight - i - 1) * nWidth + j] = pBuffer_Input[j + i * nWidth];
            }
        }
    }
        CATCH(CMemoryException, e)
    {
        e->ReportError();
        return false;
    }
    END_CATCH

        return true;
}

bool CImageObject::Draw(CWnd* pWnd, CString& str_Error_Log)
{
    if (!pBuffer)
    {
        str_Error_Log.Format(_T("[Draw] Image Buffer is NULL!"));
        return false;
    }


    CDC* pDC = pWnd->GetDC();
    CRect rect;  pWnd->GetClientRect(&rect);

    int nViewWidth = rect.Width();
    int nViewHeight = rect.Height();

    CBitmap bitMap, * pOldBitmap = nullptr;
    CDC memDC;


    memDC.CreateCompatibleDC(pDC);
    bitMap.CreateCompatibleBitmap(pDC, nViewWidth, nViewHeight);
    pOldBitmap = (CBitmap*)memDC.SelectObject(&bitMap);

    BITMAPINFO* BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));


    BmInfo->bmiHeader.biSize = 40;
    BmInfo->bmiHeader.biWidth = nWidth;
    BmInfo->bmiHeader.biHeight = nHeight;
    BmInfo->bmiHeader.biPlanes = 1;
    BmInfo->bmiHeader.biBitCount = 8;
    BmInfo->bmiHeader.biCompression = BI_RGB;
    BmInfo->bmiHeader.biSizeImage = nSize;
    BmInfo->bmiHeader.biXPelsPerMeter = 0;
    BmInfo->bmiHeader.biYPelsPerMeter = 0;
    BmInfo->bmiHeader.biClrUsed = 0;
    BmInfo->bmiHeader.biClrImportant = 0;

    //8bit = 256색
    for (int i = 0; i < 256; i++)
    {
        BmInfo->bmiColors[i].rgbRed = BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbBlue = i;
        BmInfo->bmiColors[i].rgbReserved = 0;
    }

    BYTE* pDraw_Buffer = new BYTE[nSize];
    if (!Flip(pBuffer, pDraw_Buffer))
    {
        str_Error_Log.Format(_T("[Draw] Image Flip Error!"));
        return false;
    }



    // DIB 함수
    SetDIBitsToDevice(
        memDC.GetSafeHdc(),	// Handle to the device context.  
        0, 0,					// Specifies the x and y-coordinate, in logical units, of the upper-left corner of the destination rectangle.  
        nWidth, nHeight,		// Specifies the width and height, in logical units, of the DIB.  
        0, 0,					// Specifies the x and y-coordinate, in logical units, of the lower-left corner of the DIB.  
        0,						// Specifies the starting scan line in the DIB.  
        nHeight,				// Specifies the number of DIB scan lines contained in the array pointed to by the lpvBits parameter.  
        pDraw_Buffer,				// image buffer, 24bit

        BmInfo,					// DIB 는 수평 라인을 32비트(4바이트)단위로 저장한다.

        DIB_RGB_COLORS);

    //StretchBltMode 설정.  
    pDC->SetStretchBltMode(COLORONCOLOR);
    pDC->StretchBlt(0, 0, nViewWidth, nViewHeight, &memDC, 0, 0, nWidth, nHeight, SRCCOPY);

    return true;
}