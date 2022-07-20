// v1.01: 0x1412B7290
// v1.02: 0x1412B63E0
bool* visualSetting;

// v1.01: 0x1416E5748
// v1.02: 0x1416E4898
int32_t* style;

// v1.01: 0x140683430
// v1.02: 0x140681BB0
SIG_SCAN
(
	sigCustomizationStyle,
	0x140681BB0,
	"\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x88\x41\x2C",
	"xxxxx????xxx"
);

// v1.01: 0x14040B370
// v1.02: 0x14040B270
SIG_SCAN
(
	sigNPRArchive,
	0x14040B270,
	"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x33\xED\x48\xC7\x41\x00\x00\x00\x00\x00\x48\x8B\xF9\x48\x89\x29\x48\x89\x69\x08\x48\x8D\x99\x00\x00\x00\x00\x40\x88\x69\x10\x8D\x75\x04\x48\x89\x69\x14\x66\x89\x69\x1C\x89\x69\x28\xC7\x41\x00\x00\x00\x00\x00",
	"xxxx?xxxx?xxxx?xxxxxxxxxx?????xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxx?????"
);

// v1.01: 0x14040CDC0
// v1.02: 0x14040CCC0
SIG_SCAN
(
	sigStyle,
	0x14040CCC0,
	"\x48\x89\x6C\x24\x00\x56\x48\x83\xEC\x20\x49\x8B\xF0\x48\x8B\xEA\x48\x3B\xCA\x74\x7D\x48\x89\x5C\x24\x00\x48\x8D\x59\x18\x48\x89\x7C\x24\x00\x49\x8D\x78\x28\x66\x0F\x1F\x84\x00\x00\x00\x00\x00",
	"xxxx?xxxxxxxxxxxxxxxxxxxx?xxxxxxxx?xxxxxxxxx????"
);

// v1.01: 0x1401D65B0
// v1.02: 0x1401D64A0
SIG_SCAN
(
	sigVisualSetting,
	0x1401D64A0,
	"\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x84\xC0\x74\x28",
	"xxxxx????xxxx"
);

extern "C" __declspec(dllexport) void Init()
{
	// v1.01: 0x1401D65F0
	// v1.02: 0x1401D64E0
	{
		uint8_t* instrAddr = (uint8_t*)sigVisualSetting() + 0x40;
		visualSetting = (bool*)(instrAddr + readUnalignedU32(instrAddr + 0x3) + 0x7);
		printf("[Future Tone Customization] visualSetting: 0x%08x\n", visualSetting);
	}

	// v1.01: 0x14040CE60
	// v1.02: 0x14040CD60
	{
		uint8_t* instrAddr = (uint8_t*)sigStyle() + 0xA0;
		style = (int32_t*)(instrAddr + readUnalignedU32(instrAddr + 0x3) + 0x7);
		style -= 0x2;
		printf("[Future Tone Customization] style: 0x%08x\n", style);
	}
}

bool prevVisualSetting = (bool)-1;

extern "C" __declspec(dllexport) void OnFrame()
{
	if (prevVisualSetting != *visualSetting)
	{
		if (*visualSetting)
		{
			// v1.01: 0x140683479, 0x140683485, 0x14068348C
			// v1.02: 0x140681BF9, 0x140681C05, 0x140681C0C
			WRITE_MEMORY((uint8_t*)sigCustomizationStyle() + 0x49, uint8_t, 0xB2, 0x01);
			WRITE_MEMORY((uint8_t*)sigCustomizationStyle() + 0x55, uint8_t, 0x33, 0xD2);
			WRITE_MEMORY((uint8_t*)sigCustomizationStyle() + 0x5C, uint8_t, 0xB2, 0x01);

			// v1.01: 0x14040B4EA
			// v1.02: 0x14040B3EA
			WRITE_MEMORY((uint8_t*)sigNPRArchive() + 0x17A, int32_t, -1);

			WRITE_MEMORY(style, int32_t, -1);
		}
		else
		{
			WRITE_MEMORY((uint8_t*)sigCustomizationStyle() + 0x49, uint8_t, 0x33, 0xD2);
			WRITE_MEMORY((uint8_t*)sigCustomizationStyle() + 0x55, uint8_t, 0xB2, 0x01);
			WRITE_MEMORY((uint8_t*)sigCustomizationStyle() + 0x5C, uint8_t, 0x33, 0xD2);

			WRITE_MEMORY((uint8_t*)sigNPRArchive() + 0x17A, int32_t, 0);

			WRITE_MEMORY(style, int32_t, 0);
		}

		prevVisualSetting = *visualSetting;
	}
}
