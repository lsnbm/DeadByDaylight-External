#include "../src/overlay.h"
#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include "driver_data.h"  // 嵌入式驱动头
#include <thread>
#include"Mem.h"
#include "ProcessMonitor.h"
#include <Video streaming.h>




//全局构造，调用exit或退出程序，自动调用析构函数
AutoDriver drv(L"driver", Driver_sys, Driver_sys_len);




// 根据进程名查 PID，找不到返回 0
uint32_t GetPidByName(const wchar_t* procName) {
	// 获取所有进程快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) return 0;

	PROCESSENTRY32W pe;
	pe.dwSize = sizeof(pe);

	if (Process32FirstW(hSnap, &pe)) {
		do {
			if (_wcsicmp(pe.szExeFile, procName) == 0) {
				CloseHandle(hSnap);
				return pe.th32ProcessID;
			}
		} while (Process32NextW(hSnap, &pe));
	}

	CloseHandle(hSnap);
	return 0;
}

//程序停止或意外 终止处理函数
void stop() {
	//AutoDriver::StopProcess(AutoDriver::pid_);
	//AutoDriver::StopProcess(GetPidByName(L"Steam.exe"));
}


int main()
{

	 //设置控制台代码页为 UTF-8
  //  system("chcp 65001 > nul");  // > nul 用于隐藏命令输出

	 //设置输出流为宽字符模式（可选）
  //  SetConsoleOutputCP(CP_UTF8);



	//注册终止处理函数
	atexit(stop);


	if (AutoDriver::pid_ = GetPidByName(L"DeadByDaylight-Win64-Shipping.exe"); !AutoDriver::pid_) {
		printf("[+] 游戏未启动\n");
		return 0;

	}


	if (drv.getHandle() == INVALID_HANDLE_VALUE) {
		std::wcerr << "打开设备失败\n";
		return 0;
	}
	ShowWindow(GetConsoleWindow(), SW_HIDE);       // 隐藏窗口（SW_SHOW 可重新显示）
	//ShowWindow(GetConsoleWindow(), SW_SHOW);   
	overlay::render();





}



//// 使用高精度时钟（通常是CPU的TSC时钟）
//using clock = std::chrono::high_resolution_clock;
//
//// 开始计时（时间点）
//auto start = clock::now();
//
//
//
//
//// 结束计时
//auto end = clock::now();
//
//// 计算时间间隔（毫秒）
//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//std::cout << "代码执行时间: " << duration.count() << " 毫秒" << std::endl;
//
//// 计算更精确的单位（微秒）
////auto micro_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
////std::cout << "精确执行时间: " << micro_duration.count() << " 微秒" << std::endl;

