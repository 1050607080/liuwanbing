
/*****************************************************************************
*File name    :		RecordMessage.h
*Description   :	��¼��Ϣ������
*Author       :		�ε�
*Version       :	����Χ��3.0.2.0
*Date         :		2012.1.30
*Others       :		
*History 	  :		
*Date         Version       Author               Modify
*******************************************************************************/ 
#ifndef _RECORDMESSAGE_H_
#define	_RECORDMESSAGE_H_

#define SAVE_CHAR_RECORD_TIME  7    //���������¼ʱ��
const __time64_t ONE_DAY_HAVE_SECOND = 60*60*24;
#include "PublicDefine.h"
#include <vector>

//������Ϣ�ṹ��
class CMessage
{
public:
	//��ǰ���ID
	int m_nOwnerID;

	//��ǰ�������
	CString m_strOwnerName;

	//����ID
	int m_nChaterID;

	//��������
	CString m_strChaterName;

	//������¼����
	int m_nMsgLen;

	//������������
	CString m_strMsg;

    //����ʱ�� ����: �� �� ��
    CString m_strChatDay;

	//����ʱ�� ʱ:��:��
	CString m_strChatTime;

	//���췽�� true ������ҷ�����Ϣ������ false ���෴֮��
	bool m_bSendMsg;

	//��Ϣ����(���ڶ�ȡ��Ϣ)
	int m_nMsgIndex;

    //��ǰʱ��
    SYSTEMTIME m_curTime;

	//��ʼ����Ϣ��
	CMessage()
	{
		m_nOwnerID = 0;
		m_strOwnerName = "";
		m_nChaterID = 0;
		m_strChaterName = "";
		m_nMsgLen = 0;
		m_strMsg = "";
        m_strChatDay = "";
		m_strChatTime = "";
		m_bSendMsg = true;
		m_nMsgIndex = 0;
	}
};

//��Ϣ��¼������
class EXT_CLASS CMessageRecordMgr  : public CMessage
{
public:

	CMessageRecordMgr();

	~CMessageRecordMgr();

public:
    //��ʼ���ļ����� ��Ϣͷ��
    void Init();

	//����Ϣ��д��һ����Ϣ [����][���ID][����ID][��Ϣ��ʼλ��][��Ϣ����]
	bool WriteOneLineMsg();

	//һ���Զ�ȡ������Ϣ
	bool ReadMsg(vector<CString>& vecContentData);

    //�ر��ļ�
    void CloseFile();

private:
    //����Զ���·�����ַ���
    void Clear();

    //�Ƿ���ļ�
    bool IsOpenFile(const CString& strFilePath);

    //�ļ����Ƿ����
    bool CMessageRecordMgr::IsDirExist(const CString& strDir);

    //�ļ��Ƿ����
    bool IsFileExist(CString& strFile,const CString& strMsgFileName);

    //��ȡ�ļ�·��
    void GenerateMsgFilePath();

    //��ȡ�ļ�����
    void InitPartMsgFileName();

    //��ȡ���ڵ��ļ�����,�������ھ���װ���ļ���
    void GenerateMsgFileName();

    //����Ŀ¼�ļ�����
    void SetCatalogData();

    //���ļ�
    bool OpenMsgFile(bool bCreateFile);

    //��Ŀ¼�ļ�
    bool OpenCatalogfile(const bool bCreateFile,const bool bOpenCatalogFile);

    //�������ļ�
    bool OpenContentfile(const bool bCreateFile,const bool bOpenContentFile);

    //д���ļ�
    bool Write();

    //��ȡĿ¼�ļ�
    bool ReadCatalogFile(vector<CString>& vecCatalogData);

    //��ȡ�����ļ�
    bool ReadContentFile(const vector<CString>& vecCatalog,vector<CString>& vecContentData);

    //�Ƿ���Ч������ʱ��
    bool IsValidChatTime(SYSTEMTIME sysRecordTime);
private:
    //���������ļ�,boolΪ�Ƿ�����ļ�
    std::map<CString,bool> m_mapMsgFileOpen;

	//��ϢĿ¼�ļ����� ��ǰ���ĳĳ_����ĳĳ_catalog_���쿪ʼʱ��.txt
	CString m_strCatalogFileName;

	//��Ϣ�����ļ����� ��ǰ���ĳĳ_����ĳĳ_content_���쿪ʼʱ��_.txt
	CString m_strContentFileName;

	//��ϢĿ¼�ļ�����Ϣͷ�ṹ
	CString m_strCatalogData;

	//Ŀ¼�ļ����·��
	CString m_strCatalogFilePath;

	//�����ļ����·��
	CString m_strContentFilePath;

    //Ŀ¼�ļ�����
    CFile m_fCatalogFile;

    //�����ļ�����
    CFile m_fContentFile;

    //Ŀ¼�ļ��Ƿ����
    bool m_bCatalogFileExist;

    //�����ļ��Ƿ����
    bool m_bContentFileExist;

    //���Ŀ¼�ļ�������
    vector<CString>m_vecCatalogData;

    //��������
    vector<CString> m_vecContentData;

    //��Ϣ�ܳ���
    ULONG m_ulMsgLen;
private: 
    //����ļ��Ƿ���ڵ�����������
    typedef std::map<CString,bool>::const_iterator MAPMSGFILEOPENITER;

    //���Ŀ¼�ļ������ݵ�����������
    typedef std::vector<CString>::const_iterator VECCATALOGDATAITER;
};
#endif