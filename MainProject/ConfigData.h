#pragma once
#define  SECTION_PATH					_T("Image Path")
#define  SECTION_PARAM					_T("Parameter")
#define  ITEM_LOAD_IMAGE_PATH			_T("Load Path")
#define  ITEM_SAVE_IMAGE_PATH			_T("Save Path")
#define  ITEM_KERNEL_SIZE				_T("Kernel Size")

class CConfigData
{
public:

	CConfigData(CString strFilePath) ;
	virtual ~CConfigData();

private:
	CString m_str_LoadImage_Path;
	int	m_n_Kernel_Size;
	CString m_str_SaveImage_Path;
	CString m_str_ConfigFile_Path;

public:
	void InitData();
	bool ReadDataFile();

	CString GetLoadImagePath() { return m_str_LoadImage_Path; }
	CString GetSaveImagePath() { return m_str_SaveImage_Path; }
	int GetKernelSize() { return m_n_Kernel_Size; }

	bool IsExistFile(CString path);

	bool GetData(CString str_Section, CString str_Item, CString* pstr_Value);
	bool GetData(CString str_Section, CString str_Item, int* pn_Value);
};

