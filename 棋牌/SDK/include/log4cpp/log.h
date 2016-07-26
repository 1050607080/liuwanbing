#pragma once

#ifndef AFC_LOG_H
#define AFC_LOG_H



#include "log4cpp/Category.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/PatternLayout.hh"


//log�ȼ�
#define LOG_EMERG	log4cpp::Priority::EMERG
#define LOG_FATAL	log4cpp::Priority::FATAL
#define LOG_ALERT	log4cpp::Priority::ALERT
#define LOG_CRIT	log4cpp::Priority::CRIT
#define LOG_ERROR	log4cpp::Priority::ERROR
#define LOG_WARN	log4cpp::Priority::WARN
#define LOG_NOTICE	log4cpp::Priority::NOTICE
#define LOG_INFO	log4cpp::Priority::INFO
#define LOG_DEBUG	log4cpp::Priority::DEBUG
#define LOG_NOTSET	log4cpp::Priority::NOTSET

#include <string>
#include <map>
#include <set>
using namespace std;


//��ȡ����ĵ�ǰ·��
static char *AfcGetAppPath()
{
	static char s_Path[MAX_PATH];
	bool s_bIsReady = false;
	if (!s_bIsReady)
	{
		ZeroMemory(s_Path,sizeof(s_Path));
		DWORD dwLength=GetModuleFileNameA(GetModuleHandle(NULL), s_Path, sizeof(s_Path));
		char *p = strrchr(s_Path, '\\');
		if(p != NULL)	*p = '\0';
		s_bIsReady = true;
	}
	return s_Path;
}

//ɾ�������ļ��У����ڶ�֮ǰ������־ɾ��
static void DeleteDir(LPCTSTR lpszDir)
{
	WIN32_FIND_DATA wfd;
	TCHAR szFile[MAX_PATH];
	TCHAR szDelDir[MAX_PATH];

	lstrcpy(szDelDir, lpszDir);

	wsprintf(szFile, TEXT("%s\\*.*"), lpszDir);
	HANDLE hFindFile = FindFirstFile(szFile, &wfd);
	if ( INVALID_HANDLE_VALUE == hFindFile )
		return;

	BOOL bFind = TRUE;
	while ( bFind )
	{
		bFind = FindNextFile(hFindFile, &wfd);

		if ( lstrcmpi(wfd.cFileName, TEXT(".")) == 0 ||
			lstrcmpi(wfd.cFileName, TEXT("..")) == 0 )
			continue;

		wsprintf(szFile, TEXT("%s\\%s"), szDelDir, wfd.cFileName);

		if ( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			DeleteDir(szFile);
		else
			DeleteFile(szFile);

	}
	FindClose(hFindFile);
	RemoveDirectory(szDelDir);
}

static BOOL DirectoryExist(LPCTSTR Path)
{
	WIN32_FIND_DATA fd;
	BOOL ret = FALSE;
	HANDLE hFind = FindFirstFile(Path, &fd);
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{  //Ŀ¼����
		ret = TRUE;    
	}
	FindClose(hFind);
	return ret;
}


//log��
namespace XAFCLog
{
	typedef map<string, int> CategoryTypeMap;	//��¼��־���ͣ�0��ʾ����Ϊ.\log\xxx.txt��1��ʾ����Ϊ.\log\2012-11-25\xxx.txt
	typedef set<string> AppenderDirSet;			//������¼�Ѿ�������Ŀ¼�������ж��Ƿ���Ҫ�½���־Ŀ¼
	static CategoryTypeMap g_CategoryTypeMap;	
	static AppenderDirSet g_AppenderDirSet;
	static bool	g_bUseLog;
	//�ж�Ŀ¼�Ƿ��Ѿ�����
	static bool LogFileExist(string filename)
	{
		return g_AppenderDirSet.find(filename) != g_AppenderDirSet.end();
	}
	//�ж���־�����ͣ�0����1
	static int LogType(string name)
	{
		CategoryTypeMap::iterator it = g_CategoryTypeMap.find(name);
		if (it != g_CategoryTypeMap.end())
		{
			return it->second;
		}
		else
		{
			return 1;
		}
	}

	//���5����ǰ����־
	static void ClearOldLog()
	{
		if (!g_bUseLog)
			return;
		char szFind[MAX_PATH];
		char szFile[MAX_PATH];

		CTime Now = CTime::GetCurrentTime(); 
		CTimeSpan SpanOneDay(1,0,0,0);//һ��ľ���
		CTime Tmp = Now - CTimeSpan(5,0,0,0);
		for (int i = 0; i < 365; i++)
		{
			Tmp =  Tmp - SpanOneDay;
			CString s = Tmp.Format("%Y-%m-%d");
			sprintf(szFile, ".\\log\\%s", s.GetBuffer());
			DeleteDir(szFile);
		}


		//WIN32_FIND_DATA FindFileData;

		//sprintf(szFind,"%s\\log\\*.*", G_GetAppPath());

		//HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
		//if(INVALID_HANDLE_VALUE == hFind)    return;

		//while(TRUE)
		//{
		//	if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		//	{
		//		if(FindFileData.cFileName[0]!='.')
		//		{
		//			sprintf(szFile, "%s\\log\\%s", G_GetAppPath(), FindFileData.cFileName);
		//			DeleteDir(szFile);
		//		}
		//	}
		//	if(!FindNextFile(hFind,&FindFileData))
		//		break;
		//}
		//FindClose(hFind);
	}

	//����һ���������
	static log4cpp::Appender* CreateAppender(string name, string filename)
	{
		// 1ʵ����һ��layout ����
		log4cpp::PatternLayout* layout = new log4cpp::PatternLayout();
		// 2. ��ʼ��һ��appender ����
		log4cpp::Appender* appender = new log4cpp::FileAppender(name, filename);//
		// 3. ��layout��������appender������
		appender->setLayout(layout);
		//����log��ʽ
		layout->setConversionPattern("%d{%Y-%m-%d %H:%M:%S.%l} %p %m %n");

		return appender;
	}

	//����һ��Category��
	static log4cpp::Category* CreateLog(const string& logname, int nametype = 1, log4cpp::Priority::Value priority = LOG_WARN)
	{
		if (!DirectoryExist(".\\log"))
		{
			return NULL;
		}

		if (log4cpp::Category::exists(logname)) 
			return &log4cpp::Category::getInstance(logname);

		g_CategoryTypeMap[logname] = nametype;
	

		CreateDirectory(".\\log\\", NULL);
		char logfile[MAX_PATH];
		if (1 == nametype)
		{
			char szDateStr[32];
			SYSTEMTIME sysTime;
			GetLocalTime(&sysTime);
			GetDateFormatA( LOCALE_USER_DEFAULT, LOCALE_USE_CP_ACP, &sysTime, "yyyy-MM-dd", szDateStr, 32);

			sprintf(logfile, ".\\log\\%s\\", szDateStr);
			CreateDirectory(logfile, NULL);
			sprintf(logfile, ".\\log\\%s\\%s.ini", szDateStr, logname.c_str());
		}
		else
		{
			sprintf(logfile, ".\\log\\%s.ini", logname.c_str());
		}

		//����Appender
		log4cpp::Appender* appender = CreateAppender("FileAppender", logfile);
		g_AppenderDirSet.insert(logfile);

		// 4. ʵ����һ��category����
		log4cpp::Category& warn_log = log4cpp::Category::getInstance(logname);
		// 5. ����additivityΪfalse���滻���е�appender
		warn_log.setAdditivity(false);
		// 5. ��appender���󸽵�category��
		warn_log.setAppender(appender);
		// 6. ����category�����ȼ������ڴ����ȼ�����־������¼
		warn_log.setPriority(priority);

		return &warn_log;
	}

	//���಻ֱ��ʹ�ã�����ʹ��һ��ʵ�ֲ���ȫlambda��������
	//����һ��g(a, b, c, d, e)�ĺ��������a=__FILE__, b=__LINE__ʹ��LogEx(a, b)(c,d,e)��ʵ�֣��൱��һ������ȫ��lambda����
	class LogEx
	{
		int m_line;
		string m_filename;
		string m_funtionname;
		char m_buf[1024 * 4];
	public:
		LogEx(int line, string funname, string filename)
			: m_line(line), m_funtionname(funname)
		{
			int r = filename.rfind('\\');
			if (r >= 0)
				m_filename = filename.substr(r + 1, filename.length());
			else
				m_filename = filename;
		}
		~LogEx()
		{

		}
		void operator()(string name, const char * fmt, ...)
		{
// 			if (!g_bUseLog)
// 				return;
			if (log4cpp::Category::exists(name))
			{
				sprintf(m_buf, "%s %s %d ", m_filename.c_str(), m_funtionname.c_str(), m_line);
				char* message = m_buf + strlen(m_buf);
				log4cpp::Category &_log = log4cpp::Category::getInstance(name);
				
				if (LogType(name) == 1)
				{
					char logfile[MAX_PATH];
					char szDateStr[32];
					SYSTEMTIME sysTime;
					GetLocalTime(&sysTime);
					GetDateFormatA( LOCALE_USER_DEFAULT, LOCALE_USE_CP_ACP, &sysTime, "yyyy-MM-dd", szDateStr, 32);

					sprintf(logfile, ".\\log\\%s\\%s.ini", szDateStr, name.c_str());
					if (!LogFileExist(logfile))
					{
						//log4cpp::Appender* faddender = _log.g
						_log.removeAllAppenders();

						sprintf(logfile, ".\\log\\%s\\", szDateStr);
						// 1ʵ����һ��layout ����
						CreateDirectory(logfile, NULL);

						sprintf(logfile, ".\\log\\%s\\%s.ini", szDateStr, name.c_str());

						//����Appender
						log4cpp::Appender* appender = CreateAppender("FileAppender", logfile);
						g_AppenderDirSet.insert(logfile);
						// 5. ����additivityΪfalse���滻���е�appender
						_log.setAdditivity(false);
						// 5. ��appender���󸽵�category��
						_log.setAppender(appender);
					}
				}	

				va_list va;
				va_start(va, fmt);
				vsprintf(message, fmt, va);
				va_end(va);
				_log.log(_log.getPriority(), m_buf);
			}
		}

	};
	
	//ʵ������ļ��� LOG(a, b, c)�ȼ���LogEx(__LINE__, __FUNCTION__, __FILE__)(a, b, c)
	#define LOG LogEx(__LINE__, __FUNCTION__, __FILE__)


	//���ڻ�ȡ���ʱ�䣬�Ӷ���CTimeRange�࿪ʼÿ��ʹ��GetCount��������������ϴε�GetCount�ĺ�����
	class CTimeRange
	{
		DWORD m_Start;
	public:
		CTimeRange()
		{
			m_Start = GetTickCount();

		}
		~CTimeRange()
		{

		}
		int GetCount()
		{
			DWORD _t = GetTickCount();
			int r = _t - m_Start;
			m_Start = GetTickCount();
			return r;
		}
	};


	//����
	class CSingLog
	{
	public:
		CSingLog()
		{
			if (DirectoryExist(".\\log"))
				g_bUseLog = true;
			else
				g_bUseLog = false;
			ClearOldLog();
		}
		~CSingLog()
		{
			log4cpp::Category::shutdown();
		}

	};


};



static XAFCLog::CSingLog g_SingLog;

#endif