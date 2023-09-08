#include <iostream>
#include <Windows.h>

// Important windows.h functions:
// Note: Windows.h is the Windows API, so let's try not to mess things up
// EnumDisplayDevices is a function from the Windows API that enumerates display devices
// which are monitors and GPUs (graphics cards)
// Because of this, there's no need to create another function to display graphics cards, got it?
// Every device is a "Display device" and has certain information
// such as name, ID, and key
// DeviceName: Name of the device, used to identify the device.
// DeviceString: A user-friendly description of the device.
// StateFlags: Indicates the current state of the device
// Example of its use!!!!!
// EnumDisplayDevice(nullptr, deviceindex, &dd, 0)
// nullptr is a null pointer, which specifies the device name
// this means that in this case, you want to list all devices (you can use the device name if it's a specific device)
// deviceindex is just for enumeration, nothing fancy
// &dd is a pointer to the display_device, which is to receive information about the queried device
// 0 is the type of "query," the number 0 means it's a default query

void showRAM() {
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
	//this is perfect, and its precise!!!
    if (GlobalMemoryStatusEx(&status)) {
		const double rambutingb = status.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
		std::cout.precision(0);
        std::cout << "Total RAM: " << std::fixed << rambutingb << " GB " << std::endl;
    }
}

void showDisplayDevices() {
	//this is da structure :nerd:
	DISPLAY_DEVICE dd;
	dd.cb = sizeof(dd);
	int deviceIndex = 0;

	std::cout << "Display devices: " << std::endl;
	while (EnumDisplayDevices(nullptr, deviceIndex, &dd, 0)) {

		//this part is optional, i was just removing some goofy \\.\\ when printin da devices
		wchar_t wideDeviceName[32];
		MultiByteToWideChar(CP_ACP, 0, dd.DeviceName, -1, wideDeviceName, 32);
		std::wstring deviceName(wideDeviceName);

		size_t pos = deviceName.find(L"\\\\.\\");
		if (pos != std::wstring::npos) {
			deviceName = deviceName.substr(pos + 4);
		}

		std::wcout << L"Device #" << deviceIndex + 1 << L": " << deviceName << std::endl;
		std::wcout << L"Description: " << dd.DeviceString << std::endl;
		std::wcout << L"State Flags: " << dd.StateFlags << std::endl;
		std::wcout << std::endl;
 		deviceIndex++;
	}
}

void ShowCPUInfo() {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	
	std::cout << "CPU Informatiom:" << std::endl;
	std::cout << "Architecture: ";
	if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
		std::cout << "64 Bits" << std::endl;
	} else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
		std::cout << "32  Bits" << std::endl;
	} else {
		std::cout << "Unknown" << std::endl;
	}
	std::cout << "Number of cores: " << sysInfo.dwNumberOfProcessors << std::endl;
}

void showHardDrives() {
    DWORD drivesMask = GetLogicalDrives();
    char driveLetter = 'A';

    std::cout << "Available Drives:" << std::endl;

    while (drivesMask) {
        if (drivesMask & 1) {
            char rootPath[] = { driveLetter, ':', '\\', '\0' };
            ULARGE_INTEGER totalBytes, freeBytes;

            if (GetDiskFreeSpaceEx(rootPath, NULL, &totalBytes, &freeBytes)) {
                double totalGB = static_cast<double>(totalBytes.QuadPart) / (1024.0 * 1024.0 * 1024.0);
                double freeGB = static_cast<double>(freeBytes.QuadPart) / (1024.0 * 1024.0 * 1024.0);

                std::cout << "Drive " << driveLetter << ": " << std::endl;
                std::cout << "Total Space: " << std::fixed << totalGB << " GB" << std::endl;
                std::cout << "Free Space: " << std::fixed << freeGB << " GB" << std::endl;
            }
        }

        drivesMask >>= 1;
        ++driveLetter;
    }
}

int main() {
    while (true) {
        char opcao;
		std::cout << std::endl;
        std::cout << "Hardware stuff" << std::endl;
        std::cout << "Choose: \n[1]-Ram\n[2]-Display Devices\n[3]-CPU\n[4]-HardDrives\n[9]-Exit" << std::endl;
        std::cin >> opcao;
        switch (opcao) {
        case '1':
			std::cout << std::endl;
            showRAM();
            break;
        case '2':
			std::cout << std::endl;
            showDisplayDevices();
            break;
        case '3':
			std::cout << std::endl;
            ShowCPUInfo();
            break;
		case '4':
			std::cout << std::endl;
			showHardDrives();
			break;
       case '9':
            return 0;
        default:
            std::cout << "Invalid option!" << std::endl;
            break;
        }
    }
}
