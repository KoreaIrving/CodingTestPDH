class CLogData {
public:
    CLogData(CString strFile_Path);

    virtual ~CLogData();

private:
    CStdioFile stdioFile_Log;
    CString strLog_File_Path;

public:

    bool OpenLogFile();
    void Log(const CString& message);
};


