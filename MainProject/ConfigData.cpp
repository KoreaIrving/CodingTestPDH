#include "pch.h"
#include "ConfigData.h"

#include <io.h>



CConfigData::CConfigData(CString strFilePath)
{
	InitData();
	m_str_ConfigFile_Path = strFilePath;

}



CConfigData::~CConfigData()
{
}

void CConfigData::InitData()
{
	m_str_LoadImage_Path = _T("");
	m_n_Kernel_Size = 0;
	m_str_SaveImage_Path = _T("");
}

bool CConfigData::ReadDataFile()
{
	BOOL bRet = TRUE;

	if (!IsExistFile(m_str_ConfigFile_Path))
	{
		AfxMessageBox(_T("Setting file doesn't exist"));
		return false;
	}
	InitData();

	CString str_Section = _T("");
	str_Section.Format(SECTION_PATH);
	if (!GetData(str_Section, ITEM_LOAD_IMAGE_PATH, &m_str_LoadImage_Path))
		bRet = FALSE;
	if (m_str_LoadImage_Path.IsEmpty())
		m_str_LoadImage_Path = _T("../");

	if (!GetData(str_Section, ITEM_SAVE_IMAGE_PATH, &m_str_SaveImage_Path))
		bRet = FALSE;
	if (m_str_SaveImage_Path.IsEmpty())
		m_str_SaveImage_Path = _T("../");

	str_Section.Format(SECTION_PARAM);
	if (!GetData(str_Section, ITEM_KERNEL_SIZE, &m_n_Kernel_Size))
		bRet = FALSE;
	if (m_n_Kernel_Size <= 0)
		m_n_Kernel_Size = 3;

	return bRet;
}

bool CConfigData::IsExistFile(CString str_Path)
{
	if (_access((CStringA)str_Path, 0) == 0)
		return TRUE;

	else
		return FALSE;


}

bool CConfigData::GetData(CString str_Section, CString str_Item, CString* pstr_Value)
{
	TCHAR chrData[256];
	memset(chrData, NULL, sizeof(chrData));
	GetPrivateProfileStringW(str_Section, str_Item, _T("ERROR"), chrData, 256, m_str_ConfigFile_Path);

	if (strcmp((CW2A)chrData, "ERROR") == 0)
	{
		return FALSE;
	}

	pstr_Value->Format(_T("%s"), chrData);
	return TRUE;

}

bool CConfigData::GetData(CString str_Section, CString str_Item, int* pn_Value)
{
	*pn_Value = GetPrivateProfileInt(str_Section, str_Item, -9999, m_str_ConfigFile_Path);
	if (*pn_Value == -9999)
	{
		return FALSE;
	}
	return TRUE;
}
