#include "Includes.h"

#include "Driver/Driver.hpp"
#include "Roblox/Processes.hpp"
#include "Roblox/Offsets.hpp"
#include "Roblox/Instance/Instance.hpp"

HWND ReturnedHwnd = NULL;
BOOL CALLBACK WindowsProcess(HWND hwnd, LPARAM lParam) {
    DWORD windowProcessId;
    GetWindowThreadProcessId(hwnd, &windowProcessId);
    if (windowProcessId == (DWORD)lParam) {
        ReturnedHwnd = hwnd;
        return FALSE;
    }
    return TRUE;
}

HWND GetHandleFromProcessId(DWORD processId) {
    EnumWindows(WindowsProcess, processId);
    return ReturnedHwnd;
}

HMODULE GetResourceModule() {
    HMODULE Module;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GetResourceModule, &Module);
    return Module;
}

uintptr_t HyperionBase;

int main()
{
    system("cls");

    DWORD RobloxProcessId = Processes()->GetRobloxPID();
    while (!RobloxProcessId)
    {
        system("cls");
        Logger()->Error("Roblox isn't open!");
        RobloxProcessId = Processes()->GetRobloxPID();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    HWND RobloxHWND;

    while (!(RobloxHWND = GetHandleFromProcessId(RobloxProcessId))) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    Logger()->Debug("Roblox PID: " + std::to_string(RobloxProcessId));

    uintptr_t HyperionAddress = Processes()->GetHyperionBase(RobloxProcessId);
    if (!HyperionAddress)
    {
        Logger()->Error("Error getting Roblox hyperion base!");
        Logger()->WaitAndClose();
        return 0;
    }

    HyperionBase = HyperionAddress;

    Logger()->Debug("Roblox Hyperion Address: " + std::to_string(HyperionAddress));

    Driver->Initialize(RobloxProcessId);

    while (!Driver->GetRobloxHandle()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    {

        auto DataModel = Instance()->GetDataModel();
        if (DataModel.Valid(false))
        {
            auto Players = DataModel.FindFirstChild("Players");
            if (Players.Valid(false))
            {
                auto LocalPlayerAddress = Driver->ReadPointer<uintptr_t>(Players.Self + Offsets::LocalPlayer);
                auto LocalPlayer = RobloxInstance(LocalPlayerAddress);
                if (LocalPlayer.Valid(false))
                {
                    auto CharacterAddress = Driver->ReadPointer<uintptr_t>(LocalPlayer.Self + Offsets::ModelInstance);
                    auto Character = RobloxInstance(CharacterAddress);
                    if (Character.Valid(false))
                    {
                        auto Humanoid = Character.FindFirstChild("Humanoid");
                        if (Humanoid.Valid(false))
                        {
                            Driver->WritePointer(Humanoid.Self + Offsets::WalkSpeed, 50.0f);
                            Driver->WritePointer(Humanoid.Self + Offsets::WalkSpeedCheck, 50.0f);
                            Driver->WritePointer(Humanoid.Self + Offsets::JumpPower, 200.0f);
                            //Driver->WritePointer(Humanoid.Self + Offsets::HipHeight, 50.0f);
                        }
                    }
                }
            }
        }
    }

    while (Processes()->GetRobloxPID()) {
        Sleep(5000);
    }

    Logger()->WaitAndClose();

    return 0;
}
