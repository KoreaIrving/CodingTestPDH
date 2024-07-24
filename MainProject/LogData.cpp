#include "pch.h"
#include "LogData.h"

CLogData::CLogData(CString strFile_Path)
{
    strLog_File_Path = strFile_Path;
}

CLogData::~CLogData()
{
    if (stdioFile_Log.m_pStream != nullptr) {
        stdioFile_Log.Close();
    }
}



bool CLogData::OpenLogFile()
{
    if (!stdioFile_Log.Open(strLog_File_Path, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::modeRead | CFile::typeText | CFile::shareDenyNone))
    {
        return false;
    }


    stdioFile_Log.SeekToEnd();
    

    return true;
}

void CLogData::Log(const CString& message)
{
    SYSTEMTIME time;
    ::GetLocalTime(&time);

    CString strTime(_T(""));
    strTime.Format(_T("[ %04d/%02d/%02d %02d:%02d:%02d:%03d ]"),
        time.wYear, time.wMonth, time.wDay,
        time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

    CString logMessage(_T(""));
    logMessage.Format(_T("%s\t%s\n"), strTime, message);

    stdioFile_Log.WriteString(logMessage);
    stdioFile_Log.Flush();
}