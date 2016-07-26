
/*****************************************************************************
*File name    :		RecordMessage.h
*Description   :	��¼��Ϣ������
*Author       :		�ε�
*Version       :	����Χ��3.0.2.0
*Date         :		2013.1.30
*Others       :		
*History 	  :		
*Date         Version       Author               Modify
*******************************************************************************/ 
#include "StdAfx.h"
#include "MessageRecordMgr.h"
#include "AFCFunction.h"
/*************************************************
Function          :  CMessageRecordMgr
Description    : CMessageRecordMgr���캯��
Input                :  ��
Output             :  ��
Return              :  ��
Author              :  �ε�
Date                :  2013-01-30
*************************************************/
CMessageRecordMgr::CMessageRecordMgr()
	: CMessage()
{
    m_bCatalogFileExist = false;
    m_bContentFileExist = false;
    m_mapMsgFileOpen.clear();
}

/*************************************************
Function      :  ~CMessageRecordMgr
Description   :  CMessageRecordMgr��������
Input         :  ��
Output        :  ��
Return        :  ��
Author        :  �ε�
Date          :  2013-01-30
*************************************************/
CMessageRecordMgr::~CMessageRecordMgr()
{
    m_fCatalogFile.Close();
    m_fContentFile.Close();
}

/*************************************************
Function      :  Init
Description   :  ��ʼ�� �ļ����� ��Ϣͷ��
Input         :  ��
Output        :  ��
Return        :  ��
Author        :  �ε�
Date          :  2013-01-30
*************************************************/
void CMessageRecordMgr::Init()
{
	//1.�����Һ������ַ���
    Clear();

    //2.��ʼ���ļ����Ʋ���
    InitPartMsgFileName();

    //3.�����ļ���
    GenerateMsgFileName();

	//4.�����ļ�·��
    GenerateMsgFilePath();
}
/*************************************************
Function      :  WriteOneLineMsg
Description   :  ����Ϣ��д��һ����Ϣ
Input         :  const CString& strMsg
Output        :  ��
Return        :  bool
Author        :  �ε�
Date          :  2013-01-30
*************************************************/
bool CMessageRecordMgr::WriteOneLineMsg()
{
    //1.�������������ļ�
    if (!OpenMsgFile(true))
        return false;

    //2.����Ŀ¼�ļ�����
    SetCatalogData();

	//3.дĿ¼�ļ� �����ļ�
    if (!Write())
        return false;

	return true;
}
/*************************************************
Function      :  ReadMsg
Description   :  һ���Զ�ȡ������Ϣ
Input         :  ��
Output        :  ��
Return        :  CString
Author        :  �ε�
Date          :  2013-01-30
*************************************************/
bool CMessageRecordMgr::ReadMsg(vector<CString>& vecContentData)
{
    vector<CString> vecCatalogData;

    //1.�������ļ�
	if (!OpenMsgFile(false))
        return false;

    //2.��ȡĿ¼�ļ�
    if (!ReadCatalogFile(vecCatalogData))
        return false;

    //3.��ȡ�����ļ�
    if (!ReadContentFile(vecCatalogData,vecContentData))
        return false;

    return true;
}

/*************************************************
Function      :  Clear
Description   :  ���·�����ַ���
Input         :  ��
Output        :  ��
Return        :  void
Author        :  �ε�
Date          :  2013-01-30
*************************************************/
void CMessageRecordMgr::Clear()
{
    m_ulMsgLen = 0;
    m_strCatalogData = "";
    m_strCatalogFilePath = "";
    m_strContentFilePath = "";
    m_strCatalogFileName = "";
    m_strContentFileName = "";
    m_vecCatalogData.clear();
    m_vecContentData.clear();
}
/*************************************************
Function      :  IsOpenCatalogFile
Description   : �Ƿ��Ŀ¼�ļ�
Input         :  ��
Output        :  ��
Return        :  bool
Author        :  �ε�
Date          :  2013-01-31
*************************************************/
bool CMessageRecordMgr::IsOpenFile(const CString& strFilePath)
{
    //�Ƿ��Ŀ¼�ļ�
    MAPMSGFILEOPENITER mapFileIter = m_mapMsgFileOpen.find(strFilePath);
    if (m_mapMsgFileOpen.end() != mapFileIter)
    {
        if (mapFileIter->second)
            return true;
        else
        {
            return false;
        }
    }
    return false;
}
/*************************************************
Function      :  CloseFile
Description   :  �ر��ļ������ļ��رյ�״̬��־
Input         :  ��
Output        :  ��
Return        :  bool
Author        :  �ε�
Date          :  2013-01-31
*************************************************/
void CMessageRecordMgr::CloseFile()
{
    m_mapMsgFileOpen[m_strCatalogFilePath] = false;
    m_mapMsgFileOpen[m_strContentFilePath] = false;

    CFileStatus status;
    if(m_fCatalogFile.GetStatus(m_strCatalogFilePath,status)) //�ж��Ƿ��
        m_fCatalogFile.Close();

    if(m_fContentFile.GetStatus(m_strContentFilePath,status)) //�ж��Ƿ��
        m_fContentFile.Close();
}
/*************************************************
Function      :  IsFileExist
Description :  �ļ��Ƿ����
Input           :  ��
Output        :  ��
Return        :  bool
Author        :  �ε�
Date          :  2013-01-31
*************************************************/
bool CMessageRecordMgr::IsFileExist(CString& strFile,const CString& strMsgFileName)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hListFile = NULL;
    CString strFindFile("");

    TCHAR curDir[MAX_PATH] = {0};
    ::GetCurrentDirectory(MAX_PATH,curDir);

    strFindFile.Format("%s\\MsgRecord\\*",curDir);
    hListFile = FindFirstFile(strFindFile.GetBuffer(),&FindFileData);
    if (INVALID_HANDLE_VALUE == hListFile)
        return false;

    do
    {
        if (0 == lstrcmp(FindFileData.cFileName,TEXT("."))
            || 0 == lstrcmp(FindFileData.cFileName,TEXT("..")))
            continue;

        bool bFind = true;
        int nFindLen = strMsgFileName.GetLength();
        if (strlen(FindFileData.cFileName) > nFindLen)
        {
            for (int i=0; i<nFindLen; i++)
            {
                if (FindFileData.cFileName[i] != strMsgFileName[i])
                {
                    bFind = false;
                    break;
                }
            }
            if (bFind)
            {
                strFile.Format("%s",FindFileData.cFileName);
                return true;
            }
        }
    } while (FindNextFile(hListFile,&FindFileData));

    return false;
}
/*************************************************
Function      :  GetMsgFilePath
Description :  ��ȡ�ļ�·��
Input           :  ��
Output        :  ��
Return        :  void
Author        :  �ε�
Date          :  2013-01-31
*************************************************/
void CMessageRecordMgr::GenerateMsgFilePath()
{
    //1.��ȡ��ǰĿ¼
    TCHAR curDir[MAX_PATH] = {0};
    ::GetCurrentDirectory(MAX_PATH,curDir);

    //2.�жϴ����Ϣ��¼�ļ����Ƿ���ڣ��粻�����������ļ���
    CString strPath("");
    strPath.Append(curDir);
    strPath.Append(_T("\\"));
    if (!IsDirExist(strPath))
    {
        strPath.Append(_T("MsgRecord\\"));
        CreateDirectory(strPath,NULL);
    }
    else
        strPath.Append(_T("MsgRecord\\"));

    //3.Ŀ¼�ļ����·��
    m_strCatalogFilePath.Append(strPath);
    m_strCatalogFilePath.Append(m_strCatalogFileName);

    //4.�����ļ����·��
    m_strContentFilePath.Append(strPath);
    m_strContentFilePath.Append(m_strContentFileName);
}
/*************************************************
Function      :  IsDirExist
Description :  �ļ����Ƿ����
Input           :  const CString& strDir
Output        :  ��
Return        :  bool
Author        :  �ε�
Date          :  2013-01-31
*************************************************/
bool CMessageRecordMgr::IsDirExist(const CString& strDir)
{
    CFileFind cfFinder;
    BOOL bWorking = cfFinder.FindFile(strDir);
    while (bWorking)
    {
        bWorking = cfFinder.FindNextFile();
        if (0 == (cfFinder.GetFileName()).Compare(_T("MsgRecord")))
            return true;
    }
    cfFinder.Close();
    return false;
}
/*************************************************
Function      :  GetMsgFileName
Description :  ��ȡ�ļ���
Input           :  ��
Output        :  ��
Return        :  void
Author        :  �ε�
Date          :  2013-01-31
*************************************************/
void CMessageRecordMgr::InitPartMsgFileName()
{
    //1.��ʼ��Һ������ַ���
    CString strUserID("");
    CString strChatID("");
    strUserID.Format("[%d]",m_nOwnerID);
    strChatID.Format("[%d]",m_nChaterID);

    //2.��װĿ¼�ļ��������ֶ�
    m_strCatalogFileName = _T("���");
    m_strCatalogFileName = m_strCatalogFileName + strUserID + _T("_") + _T("����")
        + strChatID + _T("_catalog_");

    //3.��װ�����ļ��������ֶ�
    m_strContentFileName = _T("���");
    m_strContentFileName = m_strContentFileName + strUserID + _T("_") + _T("����")
        + strChatID + _T("_content_");
}
/*************************************************
Function      :  JudgeExistFileName
Description :  ��ȡ���ڵ��ļ�����,�������ھ���װ���ļ���
Input           :  ��
Output        :  ��
Return        :  void
Author        :  �ε�
Date          :  2013-01-31
*************************************************/
void CMessageRecordMgr::GenerateMsgFileName()
{
    //1.��ȡϵͳʱ��
    GetLocalTime(&m_curTime);
    CString strTime("");
    strTime.Format(TEXT("%02d��_%02d��_%02d��"),m_curTime.wYear,m_curTime.wMonth,m_curTime.wDay);

    //2.��ϢĿ¼�ļ����� ��ǰ���ĳĳ_����ĳĳ_catalog_���쿪ʼʱ��.txt
    CString strFilePath("");
    m_bCatalogFileExist = IsFileExist(strFilePath,m_strCatalogFileName);
    if (!m_bCatalogFileExist)
        m_strCatalogFileName = m_strCatalogFileName + strTime + ".txt";
    else
        m_strCatalogFileName = strFilePath;

    //3.��Ϣ�����ļ����� ��ǰ���ĳĳ_����ĳĳ_content_���쿪ʼʱ��_.txt
    m_bContentFileExist = IsFileExist(strFilePath,m_strContentFileName);
    if (!m_bContentFileExist)
        m_strContentFileName = m_strContentFileName + strTime + ".txt";
    else
        m_strContentFileName = strFilePath;
}
/*************************************************
Function      :  OpenFile
Description :  ��Ŀ¼�������ļ�
Input           :  ��
Output        :  ��
Return        :  void
Author        :  �ε�
Date          :  2013-01-31
*************************************************/
bool CMessageRecordMgr::OpenMsgFile(bool bCreateFile)
{
    //1.�ļ��Ƿ��
    bool bOpenCatalogFile = false;
    bool bOpenContentFile = false;

    if (!m_bCatalogFileExist)
        bOpenCatalogFile = true;
    else if (!IsOpenFile(m_strCatalogFilePath))
        bOpenCatalogFile = true;

    if (!m_bContentFileExist)
        bOpenContentFile = true;
    else if (!IsOpenFile(m_strContentFilePath))
        bOpenContentFile = true;
    
    //1.��Ŀ¼�ļ�
    if (!OpenCatalogfile(bCreateFile,bOpenCatalogFile))
        return false;

    //2.�������ļ�
    if (!OpenContentfile(bCreateFile,bOpenContentFile))
        return false;

    return true;
}
/*************************************************
Function      :  OpenCatalogfile
Description :  ��Ŀ¼�ļ�
Input           :  const bool bCreateFile,const bool bOpenCatalogFile
Output        :  ��
Return        :  void
Author        :  �ε�
Date          :  2013-01-31
*************************************************/
bool CMessageRecordMgr::OpenCatalogfile(const bool bCreateFile,const bool bOpenCatalogFile)
{
    CFileException e;
    if (bOpenCatalogFile)
    {
        if (bCreateFile)
        {
            if (FALSE == m_fCatalogFile.Open(m_strCatalogFilePath.GetBuffer(),CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary | CFile::modeNoTruncate,&e))
            {
                CString strCatalog("");
                strCatalog.Format("����%s�ļ�ʧ��!",m_strCatalogFilePath);
                DUIOkeyMsgBox(NULL,strCatalog);
                return false;
            }
        }
        else
        {
            if (FALSE == m_fCatalogFile.Open(m_strCatalogFilePath.GetBuffer(),CFile::modeReadWrite | CFile::typeBinary,&e))
            {
                return false;
            }
        }
        m_mapMsgFileOpen[m_strCatalogFilePath] = true;
    }
    return true;
}
/*************************************************
Function      :  OpenContentfile
Description :  �������ļ�
Input           :  bool bCreateFile,bool bOpenContentFile
Output        :  ��
Return        :  void
Author        :  �ε�
Date          :  2013-01-31
*************************************************/
bool CMessageRecordMgr::OpenContentfile(const bool bCreateFile,const bool bOpenContentFile)
{
    CFileException e;
    if (bOpenContentFile)
    {
        if (bCreateFile)
        {
            if (FALSE == m_fContentFile.Open(m_strContentFilePath.GetBuffer(),CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary | CFile::modeNoTruncate,&e))
            {
                CString strContent("");
                strContent.Format("����%s�ļ�ʧ��!",m_strContentFilePath);
                DUIOkeyMsgBox(NULL,strContent);
                return false;
            }
        }
        else
        {
            if (FALSE == m_fContentFile.Open(m_strContentFilePath.GetBuffer(),CFile::modeReadWrite | CFile::typeBinary,&e))
            {
                return false;
            }
        }
        m_mapMsgFileOpen[m_strContentFilePath] = true;
    }
    return true;
}

/*************************************************
Function      :  SetCatalogData
Description :  ����Ŀ¼�ļ�����
Input           :  ��
Output        :  ��
Return        :  void
Author        :  �ε�
Date          :  2013-02-01
*************************************************/
void CMessageRecordMgr::SetCatalogData()
{
    //2.дĿ¼�ļ�
    CString strUserID("");
    CString strChatID("");
    CString strField("");
    strUserID.Format("[%d]",m_nOwnerID);
    strChatID.Format("[%d]",m_nChaterID);

    //3.��װĿ¼��Ϣ [���ID][����ID][��Ϣ��ʼλ��][��Ϣ����]
    if (m_bSendMsg)
    {
        m_strCatalogData.Append(strUserID);	//���ID
        strField.Format("{%s}",m_strOwnerName);
        m_strCatalogData.Append(strField);
        m_strCatalogData.Append(strChatID);	//����ID
        strField.Format("{%s}",m_strChaterName);
        m_strCatalogData.Append(strField);
    }
    else
    {
        m_strCatalogData.Append(strChatID);
        strField.Format("{%s}",m_strChaterName);
        m_strCatalogData.Append(strField);
        m_strCatalogData.Append(strUserID);
        strField.Format("{%s}",m_strOwnerName);
        m_strCatalogData.Append(strField);
    }
    strField.Format("<%d>",m_nMsgLen);	//������Ϣ����
    m_strCatalogData.Append(strField);
    strField.Format("(%s)",m_strChatDay);
    m_strCatalogData.Append(strField);
    strField.Format("(%s)",m_strChatTime);
    m_strCatalogData.Append(strField);
    m_strCatalogData.Append(_T("\r\n"));
}
/*************************************************
Function      :  Write
Description   :  д��Ŀ¼�������ļ�
Input         :  ��
Output        :  ��
Return        :  bool
Author        :  �ε�
Date          :  2013-02-01
*************************************************/
bool CMessageRecordMgr::Write()
{
    try
    {
        //1.��ʼ������λ������
        int nRetPos = 0;
        int nCurPos = 0;
        CString strCatalogData("");
        std::string strChatMsg(m_strMsg);

        //2.����λ�ò�ͳ��
        while (-1 != nRetPos)
        {
            CString strLen("");
            CString strValidMsg("");

            nRetPos = strChatMsg.find(_T("\r\n"),nCurPos);
            if (-1 != nRetPos)
            {
                //���س�Ϊֹ��һ�仰
                strValidMsg = m_strMsg.Mid(nCurPos,nRetPos - nCurPos);

                //ʵ�ʳ���
                strLen.Format("%d",strValidMsg.GetLength());

                //ȥ���س����λ��
                nCurPos = nRetPos + 2;
            }
            else
            {
                //ʣ������
                strValidMsg = m_strMsg.Mid(nCurPos);

                //ʵ�ʳ���
                if (nCurPos != 0 && strValidMsg != "")
                    strLen.Format("%d",strValidMsg.GetLength());
            }

            //д��س���Ϊֹ��ʵ�ʳ���
            if (strLen != "")
            {
                int nBeginPos = m_strCatalogData.Find("<");
                int nEndPos = m_strCatalogData.Find(">");
                strCatalogData = m_strCatalogData.Mid(0,nBeginPos+1) + strLen + m_strCatalogData.Mid(nEndPos,m_strCatalogData.GetLength());
            }

            if (strValidMsg != "")
            {
                m_fCatalogFile.SeekToEnd();
                strCatalogData = ("" == strCatalogData) ? m_strCatalogData : strCatalogData;
                m_fCatalogFile.Write(strCatalogData.GetBuffer(),strCatalogData.GetLength());

                m_fContentFile.SeekToEnd();
                m_fContentFile.Write(strValidMsg.GetBuffer(),strValidMsg.GetLength());
            }
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}
/*************************************************
Function      :  ReadCatalogFile
Description   :  ��ȡĿ¼�ļ���һ��
Input         :  CString& strCatalogData
Output        :  ��
Return        :  bool
Author        :  �ε�
Date          :  2013-02-01
*************************************************/
bool CMessageRecordMgr::ReadCatalogFile(vector<CString>& vecCatalogData)
{
     CString strCatalogData("");

     //1.��ȡ�ļ�����
     m_fCatalogFile.SeekToBegin();
     CArchive ar(&m_fCatalogFile,CArchive::load);
     while(NULL != ar.ReadString(strCatalogData))    //ѭ����ȡ�ļ���ֱ���ļ�����
     {
         if ("" == strCatalogData)
             continue;
         vecCatalogData.push_back(strCatalogData);
         int nBeginPos = strCatalogData.Find("<");
         int nEndPos = strCatalogData.Find(">");
         CString strLen(strCatalogData.Mid(nBeginPos+1,nEndPos-(nBeginPos+1)));
         m_ulMsgLen += atoi(strLen);
     }

     //2.�ж������Ƿ�Ϊ��
     if (vecCatalogData.empty())
     {
             return false;
     }

    return true;
}
/*************************************************
Function      :  ReadContentFile
Description   :  ��ȡ�����ļ�
Input         :  ��
Output        :  ��
Return        :  bool
Author        :  �ε�
Date          :  2013-02-03
*************************************************/
bool CMessageRecordMgr::ReadContentFile(const vector<CString>& vecCatalogData,vector<CString>& vecContentData)
{
    int curPos = 0; 
    CString strCatalog("");
    CString strContent("");
    CString strContentTmp("");
    CString strSameName("");

    m_fContentFile.Seek(curPos,CFile::begin);
    CArchive ar(&m_fContentFile,CArchive::load);
    while(NULL != ar.ReadString(strContentTmp))    //ѭ����ȡ�ļ���ֱ���ļ�����
     {
         if ("" == strContentTmp)
             continue;
         strContent += strContentTmp;
     }

    VECCATALOGDATAITER vecCatalogIter = vecCatalogData.begin();
    while (vecCatalogIter != vecCatalogData.end())
    {
        strCatalog.Format("%s",*vecCatalogIter);

        //1.��ȡ����ǳ�
        int nBeginPos = strCatalog.Find("{");
        int nEndPos = strCatalog.Find("}");
        CString strNameTmp(strCatalog.Mid(nBeginPos + 1,nEndPos - (nBeginPos + 1)));
        strNameTmp.Append(_T("˵:"));

        //2.��¼��ʱ��
        SYSTEMTIME sysRecordTime;
        nBeginPos = strCatalog.Find("(");
        CString strTime(strCatalog.Mid(nBeginPos,nBeginPos+11));
        sysRecordTime.wYear = atoi(strTime.Mid(1,4));
        sysRecordTime.wMonth = atoi(strTime.Mid(6,7));
        sysRecordTime.wDay = atoi(strTime.Mid(9,10));
        sysRecordTime.wHour = atoi(strTime.Mid(13,14));
        sysRecordTime.wMinute = atoi(strTime.Mid(16,17));
        sysRecordTime.wSecond = atoi(strTime.Mid(19,20));

        //3.��ȡ�������ʱ��
        strNameTmp.Append(_T("      "));
        strNameTmp.Append(strTime.Mid(0,11));
        nBeginPos = strCatalog.Find(")");
        strTime = strCatalog.Mid(nBeginPos+2,10);
        strNameTmp.Append(_T("   "));
        strNameTmp.Append(strTime);

        //4.��ȡ�������ݳ���
        nBeginPos = strCatalog.Find("<");
        nEndPos = strCatalog.Find(">");
        CString strContentLen(strCatalog.Mid(nBeginPos+1,nEndPos - (nBeginPos+1)));
        int ContentLen = atoi(strContentLen.GetBuffer());

        //5.�ж��Ƿ�涨ʱ���ڵļ�¼
        if (!IsValidChatTime(sysRecordTime))
        {
            curPos += ContentLen;
            ++vecCatalogIter;
            continue;
        }

        //6.�ظ���Ŀ¼��ʱ�������ͬһ��˵��
        if (0 != strSameName.Compare(strNameTmp))
            vecContentData.push_back(strNameTmp);
        strSameName = strNameTmp;

        //7.�����������ݳ��ȶ�ȡ�����ļ���ȡ��������
        CString strContentData(strContent.Mid(curPos,ContentLen));
        vecContentData.push_back(strContentData);
        curPos += ContentLen;
        ++vecCatalogIter;
    }
    return true;
}
/*************************************************
Function      :  IsValidChatTime
Description   : ������Ϣ�Ƿ���Ч(��ʾ7���ڵ�������Ϣ)
Input         :  ��
Output        :  ��
Return        :  bool
Author        :  �ε�
Date          :  2013-02-03
*************************************************/
bool CMessageRecordMgr::IsValidChatTime(SYSTEMTIME sysRecordTime)
{
    CTime curTime = CTime::GetCurrentTime();
    __time64_t nCurTime = curTime.GetTime();
    CTime sysRecTime(sysRecordTime,-1);
    __time64_t nDif = nCurTime - sysRecTime.GetTime();

    if (nDif / ONE_DAY_HAVE_SECOND > SAVE_CHAR_RECORD_TIME)
        return false;
    else
        return true;
}