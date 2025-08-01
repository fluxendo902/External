#pragma once

#include "../Includes.h"

extern uintptr_t HyperionBase;

namespace Offsets
{
	static uintptr_t Name = 0x78;
	static uintptr_t Children = 0x80;
	static uintptr_t ClassDescriptor = 0x18;
	static uintptr_t Size = 0x8;

	static uintptr_t IsCoreScript = 0x1a0;
	static uintptr_t ModuleDetection = 0x1a0 - 0x4;

	static uintptr_t FakeDataModelPointer = 0x6ED6E38;
	static uintptr_t FakeDataModelToDataModel = 0x1C0;

	static uintptr_t WalkSpeed = 0x1DC;
	static uintptr_t WalkSpeedCheck = 0x3B8;

	static uintptr_t JumpPower = 0x1B8;

	static uintptr_t HipHeight = 0x1A8;

	static uintptr_t ModelInstance = 0x328;

	static uintptr_t LocalPlayer = 0x128;

	static uintptr_t ModuleScriptByteCode = 0x158;

	static uintptr_t ValueBase = 0xD8;
}