#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <winbase.h>
#include <fstream>
#include <thread>

// 定义IOCTL控制码
#define IOCTL_READ_MEMORY           CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9000, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_WRITE_MEMORY          CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9001, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_GETMODULE_BASE        CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9002, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_TERMINATE_PROCESS     CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9003, METHOD_NEITHER, FILE_ANY_ACCESS)



class AutoDriver {
public:
	static inline  uint32_t        pid_;

	AutoDriver(const std::wstring& deviceName,
		const unsigned char* driverBin,
		size_t driverLen) :
		deviceName_(deviceName),
		driverBin_(driverBin),
		driverLen_(driverLen)
	{

		// 初始化临时路径和驱动文件路径
		wchar_t tmp[MAX_PATH] = { 0 };
		GetTempPathW(MAX_PATH, tmp);
		tempDir_ = tmp;
		driverPath_ = tempDir_ + L"Driver.sys";

		if (!writeDriver()) {
			std::wcerr << "[!] 写入失败" << std::endl;
			return;
		}

		if (!installAndStart()) {
			std::wcerr << "[!] 加载失败" << std::endl;
			return;
		}

		openDevice();
	}

	~AutoDriver() {
		// 关闭设备句柄
		if (hDevice_ != INVALID_HANDLE_VALUE) {
			CloseHandle(hDevice_);
			hDevice_ = INVALID_HANDLE_VALUE;
		}

		// 卸载驱动
		if (uninstallDriver()) {
			std::wcout << "[+] 卸载成功" << std::endl;
		}
		else {
			std::wcerr << "[!] 卸载失败" << std::endl;
		}

		// 删除临时文件
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		if (DeleteFileW(driverPath_.c_str())) {
			std::wcout << "[+] 删除文件成功" << std::endl;
		}
		else {
			std::wcerr << "[*] 删除失败，安排重启删除" << std::endl;
			MoveFileExW(driverPath_.c_str(), nullptr, MOVEFILE_DELAY_UNTIL_REBOOT);
		}
	}

	HANDLE getHandle() const { return hDevice_; }

	// 模板读写函数
	template<typename T>
	inline __forceinline static  T Read(uintptr_t address) {
		T value = {};
		R0ReadMemory(pid_, reinterpret_cast<PVOID>(address), &value, sizeof(T));
		return value;
	}
	inline __forceinline static   bool Read(uintptr_t address,PVOID value,size_t size) {
		return  R0ReadMemory(pid_, reinterpret_cast<PVOID>(address),value, size);
	}


	template<typename T>
	inline __forceinline static  bool Write(uintptr_t address, const T& value) {
		return R0WriteMemory(pid_, reinterpret_cast<PVOID>(address), const_cast<T*>(&value), sizeof(T));
	}
	inline __forceinline static  bool Write(uintptr_t address,  PVOID value,size_t size) {
		return R0WriteMemory(pid_, reinterpret_cast<PVOID>(address), value, size);
	}



	template<typename T>
	inline __forceinline static   T GetModule(const char* moduleName) {
		uintptr_t base = {};
		if (R0GetModuleBase(pid_, moduleName, base)) {
			return  base;
		}
		return 0;
	}

	inline __forceinline static   std::string ReadString(uintptr_t addr, size_t len) {
		std::vector<char> buf(len);
		R0ReadMemory(pid_, reinterpret_cast<PVOID>(addr), buf.data(), len);
		return std::string(buf.data(), len);
	}

	inline __forceinline static    std::wstring ReadFStringW(uintptr_t fstrAddr) {
		uintptr_t dataPtr = Read<uintptr_t>(fstrAddr);
		int32_t count = Read<int32_t>(fstrAddr + 0x08);
		if (!dataPtr || count <= 0 || count > 1024) return L"";
		std::vector<wchar_t> buf(count + 1);
		R0ReadMemory(pid_, reinterpret_cast<PVOID>(dataPtr), buf.data(), count * sizeof(wchar_t));
		buf[count] = L'\0';
		return std::wstring(buf.data());
	}
	inline __forceinline static bool  StopProcess(int pid) {
		IObuffer_.src_pid = pid;
		BOOL result = DeviceIoControl(hDevice_, IOCTL_TERMINATE_PROCESS,
			&IObuffer_, sizeof(IObuffer_), &IObuffer_, sizeof(IObuffer_), &returned_, nullptr);
		if (result) std::cout << "Process terminated: " << pid << std::endl;
		else std::cerr << "Terminate failed. Err: " << GetLastError() << std::endl;
		return result;
	}

	inline __forceinline static  bool StartProcess(const std::wstring& app, const std::wstring& cmd = L"") {
		PROCESS_INFORMATION pi = {};
		STARTUPINFOW si = { sizeof(si) };
		std::wstring line = cmd.empty() ? app : cmd;
		LPWSTR lpCmd = &line[0];
		BOOL ok = CreateProcessW(app.c_str(), lpCmd, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
		if (!ok) {
			DWORD e = GetLastError();
			LPWSTR msg = nullptr;
			FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				nullptr, e, 0, (LPWSTR)&msg, 0, nullptr);
			std::wcerr << L"创建进程失败: " << e << L" msg=" << msg << std::endl;
			if (msg) LocalFree(msg);
			return false;
		}
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return true;
	}
private:
	// 成员变量
	static inline   HANDLE          hDevice_;
	std::wstring    deviceName_;
	std::wstring    tempDir_;
	std::wstring    driverPath_;
	const unsigned char* driverBin_;
	size_t          driverLen_;


	struct IOBuffer {
		UINT64 src_pid;
		PVOID  src_addr;
		PVOID  dst_addr;
		SIZE_T size;
		const char* ModuleNme;
		UINT64 ModuleBase;
	};
	static inline IOBuffer IObuffer_;
	static inline DWORD returned_;

	// 写驱动到磁盘
	bool writeDriver() {
		std::ofstream fout(driverPath_, std::ios::binary);
		if (!fout) return false;
		fout.write(reinterpret_cast<const char*>(driverBin_), driverLen_);
		return fout.good();
	}

	// 安装并启动驱动服务
	bool installAndStart() {
		SC_HANDLE scm = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
		if (!scm) return false;
		// 删除旧服务
		SC_HANDLE old = OpenServiceW(scm, L"AutoDrv", DELETE | SERVICE_STOP);
		if (old) {
			SERVICE_STATUS st;
			ControlService(old, SERVICE_CONTROL_STOP, &st);
			DeleteService(old);
			CloseServiceHandle(old);
		}
		SC_HANDLE svc = CreateServiceW(
			scm, L"AutoDrv", L"AutoDrv",
			SERVICE_START | SERVICE_STOP | DELETE,
			SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START,
			SERVICE_ERROR_IGNORE, driverPath_.c_str(),
			nullptr, nullptr, nullptr, nullptr, nullptr);
		bool ok = svc && StartServiceW(svc, 0, nullptr);
		if (svc) CloseServiceHandle(svc);
		CloseServiceHandle(scm);
		return ok;
	}

	// 打开设备句柄
	void openDevice() {
		hDevice_ = CreateFileW(
			L"\\\\.\\liaoStarDriver",
			GENERIC_READ | GENERIC_WRITE,
			0,
			nullptr,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
			nullptr
		);
		if (hDevice_ == INVALID_HANDLE_VALUE) {
			std::wcerr << "[!] 打开设备失败，错误: " << GetLastError() << std::endl;
		}
		else {
			std::wcout << "[+] 设备已打开" << std::endl;
		}
	}

	// 卸载驱动服务
	bool uninstallDriver() {
		SC_HANDLE scm = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
		if (!scm) return false;
		SC_HANDLE svc = OpenServiceW(scm, L"AutoDrv", SERVICE_STOP | DELETE);
		if (svc) {
			SERVICE_STATUS st;
			ControlService(svc, SERVICE_CONTROL_STOP, &st);
			DeleteService(svc);
			CloseServiceHandle(svc);
		}
		CloseServiceHandle(scm);
		return true;
	}

	// 内核读写和模块基址获取
	inline __forceinline static   bool R0ReadMemory(UINT64 pid, PVOID addr, PVOID buf, SIZE_T sz) {
		IObuffer_.src_pid = pid;
		IObuffer_.src_addr = addr;
		IObuffer_.dst_addr = buf;
		IObuffer_.size = sz;
		return DeviceIoControl(hDevice_, IOCTL_READ_MEMORY,&IObuffer_, sizeof(IObuffer_), &IObuffer_, sizeof(IObuffer_), &returned_, nullptr);
	}

	inline __forceinline static   bool R0WriteMemory(UINT64 pid, PVOID addr, PVOID buf, SIZE_T sz) {
		IObuffer_.src_pid = pid;
		IObuffer_.src_addr = addr;
		IObuffer_.dst_addr = buf;
		IObuffer_.size = sz;
		return DeviceIoControl(hDevice_, IOCTL_WRITE_MEMORY,&IObuffer_, sizeof(IObuffer_), &IObuffer_, sizeof(IObuffer_), &returned_, nullptr);
	}

	inline __forceinline static   bool R0GetModuleBase(UINT64 pid, const char* name, uintptr_t& base) {
		IObuffer_.src_pid = pid;
		IObuffer_.ModuleNme = name;
		bool result = DeviceIoControl(hDevice_, IOCTL_GETMODULE_BASE,&IObuffer_, sizeof(IObuffer_), &IObuffer_, sizeof(IObuffer_), &returned_, nullptr);
		if (result) base = IObuffer_.ModuleBase;
		return result;
	}


};


//初始化示例
//ProcessMonitor monitor;
//monitor.SetUpdateInterval(2500); // 2秒更新一次
//
//ProcessMonitor::ProcessInfo info;
//if (!AutoDriver::pid_) {
//	if (monitor.GetProcessInfo(L"DeadByDaylight-Win64-Shipping.exe", info)) {
//		AutoDriver::pid_ = info.pid;
//		printf("[+] pid=%d\n", AutoDriver::pid_);
//
//	}
//	else {
//		printf("[+] 游戏未启动\n");
//		return 0;
//	}
//
//}
//
////	printf("输入游戏PID:\n");
//	//std::cin >> GamePid;
//AutoDriver drv(L"driver", Driver_sys, Driver_sys_len);
//if (drv.getHandle() == INVALID_HANDLE_VALUE) {
//	std::wcerr << L"打开设备失败\n";
//	return -1;
//}
