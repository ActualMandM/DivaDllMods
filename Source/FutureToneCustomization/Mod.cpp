bool* visualSetting;
int32_t* style;

int64_t* songSelectAddr;

SIG_SCAN
(
	sigCustomizationState,
	0x14067EA3C,
	"\x44\x8B\x49\x08\x45\x85\xC9\x7E\x26\xE8\x00\x00\x00\x00",
	"xxxxxxxxxx????"
);

SIG_SCAN
(
	sigCustomizationStyle,
	0x140681E70,
	"\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x88\x41\x2C",
	"xxxxx????xxx"
);

SIG_SCAN
(
	sigNPRArchive,
	0x14040B2B0,
	"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x20\x33\xED\x48\xC7\x41\x00\x00\x00\x00\x00\x48\x8B\xF9\x48\x89\x29\x48\x89\x69\x08\x48\x8D\x99\x00\x00\x00\x00\x40\x88\x69\x10\x8D\x75\x04\x48\x89\x69\x14\x66\x89\x69\x1C\x89\x69\x28\xC7\x41\x00\x00\x00\x00\x00",
	"xxxx?xxxx?xxxx?xxxxxxxxxx?????xxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxx?????"
);

SIG_SCAN
(
	sigStyle,
	0x14040CD00,
	"\x48\x89\x6C\x24\x00\x56\x48\x83\xEC\x20\x49\x8B\xF0\x48\x8B\xEA\x48\x3B\xCA\x74\x7D\x48\x89\x5C\x24\x00\x48\x8D\x59\x18\x48\x89\x7C\x24\x00\x49\x8D\x78\x28\x66\x0F\x1F\x84\x00\x00\x00\x00\x00",
	"xxxx?xxxxxxxxxxxxxxxxxxxx?xxxxxxxx?xxxxxxxxx????"
);

SIG_SCAN
(
	sigVisualSetting,
	0x1401D64B0,
	"\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x84\xC0\x74\x28",
	"xxxxx????xxxx"
);

SIG_SCAN
(
	sigSongSelect,
	0x1406F4920,
	"\x48\x39\x2D\x00\x00\x00\x00\x74\x12\x48\x8D\x54\x24\x00\xE8\x00\x00\x00\x00\x48\x8B\xD8\x8D\x7D\x01\xEB\x25",
	"xxx????xxxxxx?x????xxxxxxxx"
);

SIG_SCAN
(
	sigGetSpriteInfo,
	0x1405BC8F0,
	"\x41\x56\x48\x83\xEC\x30\x48\x89\x5C\x24\x40\x48\x8D\x0D\xCC\xCC\xCC\xCC\x48\x89\x7C\x24\x28\x4C\x89\x7C\x24\x20\x4C",
	"xxxxxxxxxxxxxx????xxxxxxxxxxx"
);

SIG_SCAN
(
	sigGetModules,
	0x1401C9D13,
	"\xE8\xCC\xCC\xCC\xCC\x4C\x8B\xD0\x4C\x8B\x08",
	"x????xxxxxx"
);

SIG_SCAN
(
	sigGetCstmItems,
	0x014034DA87,
	"\xE8\xCC\xCC\xCC\xCC\x4C\x63\x57\x10",
	"x????xxxx"
);

SIG_SCAN
(
	sigPvSelCtrl,
	0x1406EDC40,
	"\x40\x55\x57\x41\x57\x48\x81",
	"xxxxxxx"
);

SIG_SCAN
(
	sigPS4PvSelCtrl,
	0x1402033C0,
	"\x48\x8B\xC4\x48\x89\x58\x10\x48\x89\x70\x18\x48\x89\x78\x20\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x48\xFF\xFF\xFF\x48\x81\xEC\x90\x01\x00\x00\x0F\x29\x70\xC8\x0F",
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
);

SIG_SCAN
(
	sigLoadDecorationStamps,
	0x1401FCB60,
	"\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x48\x89\x7C\x24\x18\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x30\xFF",
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
);

SIG_SCAN
(
	sigDecorationStampFile,
	0x140C855B8, 
	"rom/photo_print/stamp/module/stamp_md_sub_%03d_00.tga\0",
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
);

struct string_range
{
	char* start;
	char* end;

	string_range(char* str, size_t length)
	{
		start = str;
		end = str + length;
	}
};

struct SpriteInfo
{
	uint32_t id;
	string_range name;
	uint16_t index;
	uint16_t setIndex;
};

struct ModuleData {
	int id;
	int sort_index;
	int chara;
	int cos;
	INSERT_PADDING(0x5C);
	uint32_t spr_set_id;
	uint32_t unk_70;
	uint32_t sprite_id;
	uint32_t unk_78;
	std::string name;
	int shop_price;
	uint64_t unk_A8;
	uint64_t unk_B0;
	int attributes;
};

struct CstmItemData {
	int id;
	int obj_id;
	int sort_index;
	std::string name;
	int chara;
	int parts;
	uint32_t sprite_id;
	bool unk_3C;
	uint32_t unk_40;
	void* unk_48;
	void* unk_50;
	uint32_t spr_set_id;
	uint32_t bind_module;
	bool unk_60;
};

FUNCTION_PTR(std::vector<ModuleData>*, __fastcall, GetModules, nullptr);
FUNCTION_PTR(std::vector<CstmItemData>*, __fastcall, GetCstmItems, nullptr);
FUNCTION_PTR(SpriteInfo*, __fastcall, GetSpriteInfo, sigGetSpriteInfo(), void* a1, string_range* name);

bool prevVisualSetting = (bool)-1;

HOOK(char, __fastcall, _CustomizationState, (char*)sigCustomizationState() - 0x3C, __int64 a1)
{
	bool curVisualSetting = (*songSelectAddr != 0) ? *(bool*)(*songSelectAddr + 0x27538) : *visualSetting;

	if (curVisualSetting)
	{
		WRITE_MEMORY((char*)sigCustomizationStyle() + 0x49, uint8_t, 0xB2, 0x01);
		WRITE_MEMORY((char*)sigCustomizationStyle() + 0x55, uint8_t, 0x33, 0xD2);
		WRITE_MEMORY((char*)sigCustomizationStyle() + 0x5C, uint8_t, 0xB2, 0x01);

		WRITE_MEMORY((char*)sigNPRArchive() + 0x17A, int32_t, -1);

		WRITE_MEMORY(style, int32_t, -1);
	}
	else
	{
		WRITE_MEMORY((char*)sigCustomizationStyle() + 0x49, uint8_t, 0x33, 0xD2);
		WRITE_MEMORY((char*)sigCustomizationStyle() + 0x55, uint8_t, 0xB2, 0x01);
		WRITE_MEMORY((char*)sigCustomizationStyle() + 0x5C, uint8_t, 0x33, 0xD2);

		WRITE_MEMORY((char*)sigNPRArchive() + 0x17A, int32_t, 0);

		WRITE_MEMORY(style, int32_t, 0);
	}

	auto modules = GetModules();
	char buf[64];
	for (int i = 0; i < modules->size(); i++) {
		size_t length;
		if (curVisualSetting) length = sprintf_s(buf, "SPR_SEL_MD%03dCMN_MD_IMG", modules->at(i).id);
		else length = sprintf_s(buf, "SPR_SEL_MD%03dCMN_MD_IMG_FT", modules->at(i).id);

		auto name = string_range(buf, length);
		auto spr = GetSpriteInfo(nullptr, &name);
		if (spr->id != -1) {
			modules->at(i).sprite_id = spr->id;
		}
	}

	auto cstm_items = GetCstmItems();
	for (int i = 0; i < cstm_items->size(); i++) {
		size_t length;
		if (curVisualSetting) length = sprintf_s(buf, "SPR_CMNITM_THMB%03d_ITM_IMG", cstm_items->at(i).id);
		else length = sprintf_s(buf, "SPR_CMNITM_THMB%03d_ITM_IMG_FT", cstm_items->at(i).id);

		auto name = string_range(buf, length);
		auto spr = GetSpriteInfo(nullptr, &name);
		if (spr->id != -1) {
			cstm_items->at(i).sprite_id = spr->id;
		}
	}

	prevVisualSetting = curVisualSetting;
	
	return original_CustomizationState(a1);
}

HOOK(bool, __fastcall, PvSelCtrl, sigPvSelCtrl(), uint64_t a1) {
	bool ret = originalPvSelCtrl(a1);

	bool curVisualSetting = *(bool*)(a1 + 0x27538);
	if (prevVisualSetting != curVisualSetting) {
		int moduleCount = *(int*)(a1 + 0x3380 + 0x600);

		auto modules = GetModules();
		char buf[64];
		size_t length;
		for (int i = 0; i < modules->size(); i++) {
			if (curVisualSetting) length = sprintf_s(buf, "SPR_SEL_MD%03dCMN_MD_IMG", modules->at(i).id);
			else length = sprintf_s(buf, "SPR_SEL_MD%03dCMN_MD_IMG_FT", modules->at(i).id);

			auto name = string_range(buf, length);
			auto spr = GetSpriteInfo(nullptr, &name);
			if (spr->id != -1) modules->at(i).sprite_id = spr->id;
		}

		auto pv_sel_modules = (ModuleData*)(a1 + 0x3380 + 0x608);
		for (int i = 0; i < moduleCount; i++) {
			if (curVisualSetting) length = sprintf_s(buf, "SPR_SEL_MD%03dCMN_MD_IMG", pv_sel_modules[i].id);
			else length = sprintf_s(buf, "SPR_SEL_MD%03dCMN_MD_IMG_FT", pv_sel_modules[i].id);

			auto name = string_range(buf, length);
			auto spr = GetSpriteInfo(nullptr, &name);
			if (spr->id != -1) pv_sel_modules[i].sprite_id = spr->id;
		}

		prevVisualSetting = curVisualSetting;
	}

	return ret;
}

HOOK(bool, __fastcall, PS4PvSelCtrl, sigPS4PvSelCtrl(), uint64_t a1) {
	bool ret = originalPS4PvSelCtrl(a1);

	bool curVisualSetting = (*songSelectAddr != 0) ? *(bool*)(*songSelectAddr + 0x27538) : *visualSetting;
	if (prevVisualSetting != curVisualSetting) {
		int moduleCount = *(int*)(a1 + 0x55E8 + 0x2240);

		auto modules = GetModules();
		char buf[64];
		size_t length;
		for (int i = 0; i < modules->size(); i++) {
			if (curVisualSetting) length = sprintf_s(buf, "SPR_SEL_MD%03dCMN_MD_IMG", modules->at(i).id);
			else length = sprintf_s(buf, "SPR_SEL_MD%03dCMN_MD_IMG_FT", modules->at(i).id);

			auto name = string_range(buf, length);
			auto spr = GetSpriteInfo(nullptr, &name);
			if (spr->id != -1) modules->at(i).sprite_id = spr->id;
		}

		auto pv_sel_modules = (ModuleData*)(a1 + 0x36A68);
		for (int i = 0; i < moduleCount; i++) {
			if (curVisualSetting) length = sprintf_s(buf, "SPR_SEL_MD%03dCMN_MD_IMG", pv_sel_modules[i].id);
			else length = sprintf_s(buf, "SPR_SEL_MD%03dCMN_MD_IMG_FT", pv_sel_modules[i].id);

			auto name = string_range(buf, length);
			auto spr = GetSpriteInfo(nullptr, &name);
			if (spr->id != -1) pv_sel_modules[i].sprite_id = spr->id;
		}

		prevVisualSetting = curVisualSetting;
	}

	return ret;
}

HOOK(void, __fastcall, LoadDecorationStamps, sigLoadDecorationStamps()) {
	if (prevVisualSetting) WRITE_MEMORY(sigDecorationStampFile(), char, "rom/photo_print/stamp/module/stamp_md_sub_%03d_00.tga");
	else WRITE_MEMORY(sigDecorationStampFile(), char, "rom/photo_print/stamp/module/stamp_md_sub_%03d_01.tga");

	return originalLoadDecorationStamps();
}

extern "C" __declspec(dllexport) void Init()
{
	if (!sigValid)
	{
		versionWarning(TEXT(MOD_NAME));
		return;
	}

	// Save Data
	{
		uint8_t* instrAddr = (uint8_t*)sigVisualSetting() + 0x40;
		visualSetting = (bool*)(instrAddr + readUnalignedU32(instrAddr + 0x3) + 0x7);
		printf("[%s] visualSetting: 0x%llx\n", MOD_NAME, visualSetting);
	}

	// Song Select
	{
		uint8_t* instrAddr = (uint8_t*)sigSongSelect();
		songSelectAddr = (int64_t*)(instrAddr + readUnalignedU32(instrAddr + 0x3) + 0x7);
		printf("[%s] songSelectAddr: 0x%llx\n", MOD_NAME, songSelectAddr);
	}

	{
		uint8_t* instrAddr = (uint8_t*)sigStyle() + 0xA0;
		style = (int32_t*)(instrAddr + readUnalignedU32(instrAddr + 0x3) + 0x7);
		style -= 0x2;
		printf("[%s] style: 0x%llx\n", MOD_NAME, style);
	}

	{
		uint8_t* instrAddr = (uint8_t*)sigGetModules();
		GetModules = (std::vector<ModuleData>*(__fastcall*)())(instrAddr + readUnalignedU32(instrAddr + 0x1) + 0x5);
		printf("[%s] GetModules: 0x%llx\n", MOD_NAME, (void*)GetModules);
	}

	{
		uint8_t* instrAddr = (uint8_t*)sigGetCstmItems();
		GetCstmItems = (std::vector<CstmItemData>*(__fastcall*)())(instrAddr + (int32_t)readUnalignedU32(instrAddr + 0x1) + 0x5);
		printf("[%s] GetCstmItems: 0x%llx\n", MOD_NAME, (void*)GetCstmItems);
	}

	INSTALL_HOOK(_CustomizationState);
	INSTALL_HOOK(PvSelCtrl);
	INSTALL_HOOK(PS4PvSelCtrl);
	INSTALL_HOOK(LoadDecorationStamps);
}
