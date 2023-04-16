#include <iostream>
#include <filesystem>
#include <string>
#include <cstdio>
#include <tchar.h>
#include <process.h>
#include <windows.h>
#include <tlhelp32.h>


unsigned long PIDByName(std::wstring AProcessName)
{
    HANDLE pHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 ProcessEntry;
    unsigned long pid;
    ProcessEntry.dwSize = sizeof(ProcessEntry);
    bool Loop = Process32First(pHandle, &ProcessEntry);

    while (Loop)
    {
        if (ProcessEntry.szExeFile == AProcessName)
        {
            pid = ProcessEntry.th32ProcessID;
            CloseHandle(pHandle);
            return pid;
        }
        Loop = Process32Next(pHandle, &ProcessEntry);
    }
    return 0;
}

int main()
{
    namespace fs = std::filesystem;

    std::wstring lname = L"cabal.exe";
    unsigned long process_id = PIDByName(lname);


    //std::string stcabal_s = "start " + fs::current_path().string() + "\\cabal.exe";
    std::string stcabal_s = "start cabal.exe";
    const char* stcabal_c = stcabal_s.c_str();

    std::string file_name = "launcher.dat";
    //std::remove(file_name.c_str()) ;

    //int retval = ::_tsystem(_T("taskkill /F /T /IM cabal.exe"));
    remove(file_name.c_str());
    system(stcabal_c);

    const auto cabal = OpenProcess(PROCESS_TERMINATE, false, process_id);
    TerminateProcess(cabal, 1);
    CloseHandle(cabal);
}
