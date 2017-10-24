#include "stdafx.h"
#include <windows.h>
#include <tlhelp32.h> 
#include <stdio.h> 
#include <time.h>
#include <iostream>
#include <string>
#include <tchar.h>
using std::string;
using namespace std;
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
BOOL KillProcess(DWORD ProcessId); 
bool hasProcesses(TCHAR*, DWORD &);
int main(int argc, char* argv[]) { 
	DWORD pid = 0;    
	TCHAR toFind[] = _T("chrome.exe");
	while (true) 
	{       
		if (hasProcesses(toFind, pid) == true)
		{
			cout << pid << endl;
			//KillProcess(pid);  
		}
			
		Sleep(200); 
	}    
	return 0; 
}


BOOL KillProcess(DWORD ProcessId) {    
	//通过进程ID获取句柄    
	HANDLE hProcess=OpenProcess(PROCESS_TERMINATE,FALSE,ProcessId);    
	if(hProcess==NULL)        
		return FALSE;    //终止进程，参数为进程句柄和退出码    
	if (!TerminateProcess(hProcess, 0))
	{
		cout << TerminateProcess(hProcess, 0) << endl;
		
		return FALSE;
	}
	CloseHandle(hProcess);
	return TRUE;
}



bool hasProcesses(TCHAR* pname, DWORD &pid) {
	PROCESSENTRY32 pe32;        
	pe32.dwSize = sizeof(pe32);        //给系统内所有的进程拍个快照          
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);        
	if (hProcessSnap == INVALID_HANDLE_VALUE)            
		return false;        //遍历进程快照，轮流显示每个进程的信息          
	BOOL hasNext = Process32First(hProcessSnap, &pe32);        //关闭进程的间隔时间    
	
	while (hasNext) {
		if (_tcscmp(pe32.szExeFile, pname) == 0)
		{
			pid = pe32.th32ProcessID;
			//TerminateProcess(pe32.th32ProcessID, 0);
			return true;
		}
		//_tprintf(_T("Process32Next -> th32ProcessID=%ld, cntThreads=%ld, th32ParentProcessID=%ld, pcPriClassBase=%ld, szExeFile=%s\n"), pe32.th32ProcessID, pe32.cntThreads, pe32.th32ParentProcessID, pe32.pcPriClassBase, pe32.szExeFile);
		hasNext = Process32Next(hProcessSnap, &pe32);
	}
	//不要忘记清除掉snapshot对象          
	CloseHandle(hProcessSnap);        
	return false;
}