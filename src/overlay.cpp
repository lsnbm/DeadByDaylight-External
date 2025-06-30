#include "overlay.h"
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <dwmapi.h>
#include <d3d11.h>
#include <cstdint>
#include <iostream>
#include"Vector.h"
#include"GlobalOffsets.h"






static  int screenWidth;
static int screenHeight;
static uint64_t gameBase = 0;




static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



inline __forceinline FNameEntry* GetEntryByIndex(uint32_t Index, uint64_t GName)
{
	const uint32_t ChunkIdx = Index >> 16;
	const uint32_t InChunk = Index & 65535;

	uint32_t CurrentBlock = AutoDriver::Read<uint32_t>(GName + 0x8);
	uint32_t CurrentByteCursor = AutoDriver::Read<uint32_t>(GName + 0xC);

	// 验证索引有效性
	if (!(ChunkIdx <= CurrentBlock && !(ChunkIdx == CurrentBlock && InChunk > CurrentByteCursor))) {
		return nullptr;
	}

	// 读取对应块的地址
	uint64_t BlockAddress = AutoDriver::Read<uint64_t>(GName + 0x10 + ChunkIdx * sizeof(uint64_t));
	if (BlockAddress == 0) {
		return nullptr;
	}

	// 计算最终地址
	uint64_t EntryAddress = BlockAddress + (InChunk * 4);
	return reinterpret_cast<FNameEntry*>(EntryAddress);
}
inline __forceinline std::string GetNameByIndexString(int32_t Index, uint64_t GName)
{
	FNameEntry* entry = GetEntryByIndex(Index, GName);
	if (entry == nullptr) {
		return "";
	}

	// 读取完整的FNameEntry结构体
	FNameEntry nameEntry = AutoDriver::Read<FNameEntry>((uint64_t)entry);

	if (nameEntry.Header.Len <= 0 || nameEntry.Header.Len > 1023) { // 最大长度检查
		return "";
	}

	if (nameEntry.Header.bIsWide) {
		// 处理宽字符
		std::wstring wideStr;
		wideStr.resize(nameEntry.Header.Len);

		// 从union中读取WideName数据
		for (int i = 0; i < nameEntry.Header.Len && i < 0x400; i++) {
			wideStr[i] = nameEntry.Name.WideName[i];
		}

		// 转换为std::string
		std::string result;
		for (wchar_t wc : wideStr) {
			if (wc == 0) break;
			if (wc < 128) {
				result += (char)wc;
			}
			else {
				result += '?'; // 非ASCII字符用?替代，或者使用更复杂的转换
			}
		}
		return result;
	}
	else {
		// 处理ANSI字符
		std::string result;
		result.resize(nameEntry.Header.Len);

		// 从union中读取AnsiName数据
		for (int i = 0; i < nameEntry.Header.Len && i < 0x400; i++) {
			result[i] = nameEntry.Name.AnsiName[i];
		}

		return result;
	}
}


inline __forceinline const char* GetNameByIndex(int32_t Index, uint64_t GName)
{
	static thread_local char buffer[1024];
	// 获取 entry
	FNameEntry* entry = GetEntryByIndex(Index, GName);
	if (!entry) {
		buffer[0] = '\0';
		return buffer;
	}

	// 读取完整结构体
	FNameEntry nameEntry = AutoDriver::Read<FNameEntry>(reinterpret_cast<uint64_t>(entry));
	int len = nameEntry.Header.Len;
	if (len <= 0 || len >= (int)sizeof(buffer)) {
		buffer[0] = '\0';
		return buffer;
	}

	if (nameEntry.Header.bIsWide) {
		// 宽字符处理
		int out = 0;
		for (int i = 0; i < len && i < 0x400; ++i) {
			wchar_t wc = nameEntry.Name.WideName[i];
			if (wc == 0) break;
			buffer[out++] = (wc < 128 ? static_cast<char>(wc) : '?');
		}
		buffer[out] = '\0';
	}
	else {
		// ANSI 字符处理
		int out = 0;
		for (int i = 0; i < len && i < 0x400; ++i) {
			char c = nameEntry.Name.AnsiName[i];
			if (c == '\0') break;
			buffer[out++] = c;
		}
		buffer[out] = '\0';
	}

	return buffer;
}


static inline __forceinline const char* WideToUtf8(const wchar_t* wstr) {
	static thread_local char buffer[2048];
	if (!wstr || wstr[0] == L'\0') {
		buffer[0] = '\0';
		return buffer;
	}

	int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
	if (size <= 0 || size > (int)sizeof(buffer)) {
		buffer[0] = '\0';
		return buffer;
	}
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buffer, size, nullptr, nullptr);
	return buffer;
}


static inline __forceinline const char* WideToUtf8(const std::wstring& wstr) {
	return WideToUtf8(wstr.c_str());
}








void overlay::render()
{

	static int maxAttempts = 2, attempt = 0;
	while (attempt < maxAttempts) {
		gameBase =GetModule<uint64_t>("DeadByDaylight-Win64-Shipping.exe");
		if (gameBase) {
			printf("第 %d 次尝试，获取游戏基地址 OK!!!\n", attempt + 1);
			break;  // 获取到就跳出循环
		}
		else {
			printf("第 %d 次尝试，获取游戏基地址失败!!!\n", attempt + 1);
		}
		attempt++;
	}

	if (!gameBase) {
		printf("经过 %d 次尝试，未获取到游戏基地址，程序退出\n", maxAttempts);
		return;
	}
	// 获取主屏幕分辨率
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	printf("screenWidth=%d\n", screenWidth);
	printf("screenHeight=%d\n", screenHeight);
	printf("GameBase=%llx\n", gameBase);


	WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"liao", nullptr };
	::RegisterClassExW(&wc);
	HWND hwnd = ::CreateWindowExW(WS_EX_TOPMOST, wc.lpszClassName, L"liao", WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);


	//防止截屏
	SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE);

	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(hwnd, &margin);

	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		return;
	}

	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;



	// 加载字体前先清除默认字体
	io.Fonts->Clear();
	ImFont* chineseFont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

	// 构建字体纹理
	bool buildResult = io.Fonts->Build();
	if (!buildResult) {
		// 字体构建失败
		printf("Font build failed!\n");
	}

	// 设为默认字体
	if (chineseFont != nullptr) {
		io.FontDefault = chineseFont;
	}


	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 0.f);

	bool done = false;
	while (done == false)
	{

		if (GetAsyncKeyState(VK_INSERT) & 1) { overlay::enabled = !overlay::enabled; }

		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				done = true;
				break;
			}
		}

		if (done == true)
		{
			break;
		}

		if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
			g_ResizeWidth = g_ResizeHeight = 0;
			CreateRenderTarget();
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();




		auto DrawList = ImGui::GetBackgroundDrawList();
		if (gui::帧率显示) {
			char fpsText[64];
			sprintf_s(fpsText, "FPS: %.1f", ImGui::GetIO().Framerate);
			DrawList->AddText(NULL, 50.f, ImVec2(10, 10), COLOR_YELLOW, fpsText);

		}

		{

			static uint64_t GName = gameBase + OFFSET_GNames;
			static uint64_t GUObject = gameBase + OFFSET_GUObjects;
			static uint64_t UWorld;
			static  Engine engine;
			FCameraCacheEntry CameraCachePrivate;
			static	float cellWidth = 180.0f, rowHeight = 30.0f;//技能表格单元格宽度和高度


			if (gui::初始化) {

				UWorld = AutoDriver::Read<uint64_t>(gameBase + OFFSET_GWorld);
				if (UWorld) {
					engine.pLevel =Read<uint64_t>(UWorld + OFFSET_UWorld_ULevel_PersistentLevel);
					if (engine.pLevel) {
						engine.actors =Read<uint64_t>(engine.pLevel + OFFSET_ULevel_Actors);
						if (engine.actors) {
							engine.Num = AutoDriver::Read<uint32_t>(engine.pLevel + OFFSET_ULevel_Actors + 0x8);
							if (engine.Num > 3000) engine.Num = 0;
						}
					}
				}
				if (UWorld) {
					engine.GameInstance = AutoDriver::Read<uint64_t>(UWorld + OFFSET_GWorld_OwningGameInstance);
					if (engine.GameInstance) {
						uint64_t lpArr = AutoDriver::Read<uint64_t>(engine.GameInstance + OFFSET_OwningGameInstance_LocalPlayers);
						if (lpArr) {
							engine.LocalPlayer = AutoDriver::Read<uint64_t>(lpArr);
							if (engine.LocalPlayer) {
								engine.PlayerController = AutoDriver::Read<uint64_t>(engine.LocalPlayer + OFFSET_LocalPlayer_PlayerController);
								if (engine.PlayerController) {
									engine.AcknowledgedPawn = AutoDriver::Read<uint64_t>(engine.PlayerController + OFFSET_PlayerController_AcknowledgedPawn);
									engine.PlayerCameraManager = AutoDriver::Read<uint64_t>(engine.PlayerController + OFFSET_PlayerController_CameraManager);
								}
							}
						}
					}
				}
				if (engine.GameState = AutoDriver::Read<uint64_t>(UWorld + OFFSET_GWorld_GameState)) {

					//房间所有玩家数组
					engine.baseArray = AutoDriver::Read<uint64_t>(engine.GameState + OFFSET_GameState_PlayerState);
					engine.PlayerCount = AutoDriver::Read<uint32_t>(engine.GameState + OFFSET_GameState_PlayerState + 0x8);
					engine.PlayerCountMax = AutoDriver::Read<uint32_t>(engine.GameState + OFFSET_GameState_PlayerState + 0x8 + 0x4);
					// 安全检查玩家数量
					if (engine.PlayerCount > 100)engine.PlayerCount = 0; // 防止异常数据
				}

				// 遍历所有对象缓存图腾和地窖指针
				engine.TotemsCache.clear();
				engine.GeneratorCache.clear();
				engine.HatchCache = 0;
				for (unsigned int i = 0; i < engine.Num; ++i) {
					uint64_t actor = AutoDriver::Read<uint64_t>(engine.actors + i * sizeof(uint64_t)); if (!actor) continue;

					uint32_t idx = AutoDriver::Read<uint32_t>(actor + 0x0018 + 0x8);
					std::string name = GetNameByIndexString(idx, GName);

					if (name == "BP_Hatch01_C") {
						engine.HatchCache = actor;
					}
					else if (name == "BP_TotemBase_C") {
						engine.TotemsCache.push_back(actor);
					}
					else if (name == "GeneratorStandard_C" || name == "GeneratorShort_C" || name == "GeneratorStandard_8thAnniversary_C" || name == "GeneratorNoPole_8thAnniversary_C"||name=="GeneratorShort_8thAnniversary_C") {
						engine.GeneratorCache.push_back(actor);
					}
					else if (name == "BP_ConjoinedTwin_C") {
						engine.ConjoinedTwin = actor;
					}
					else if (name == "BP_Pallet_C") {
						engine.Pallets.push_back(actor);
					}
					else if (name == "") {
						engine.窗口.push_back(actor);
					}
				}

				// 遍历房间所有玩家缓存指针
				engine.player.resize(engine.PlayerCount);
				for (unsigned int i = 0; i < engine.PlayerCount; ++i) {

					// 先向vector中添加元素
					engine.player[i].PlayerData = AutoDriver::Read<uint64_t>(engine.baseArray + i * sizeof(uint64_t));
					engine.player[i].name = WideToUtf8(AutoDriver::ReadFStringW(engine.player[i].PlayerData + OFFSET_PlayerState_PlayerNamePrivate));
					engine.player[i].PerkStruct = engine.player[i].PlayerData + OFFSET_PlayerState_FPlayerStateData_PlayerData;
					engine.player[i].EquipedPerkIds = AutoDriver::Read<uint64_t>(engine.player[i].PerkStruct + 0x10);
					engine.player[i].EquipedPerkIdsNum = AutoDriver::Read<uint32_t>(engine.player[i].PerkStruct + 0x18); if (engine.player[i].EquipedPerkIdsNum > 10) { engine.player[i].EquipedPerkIdsNum = 0; }//安全处理
					engine.player[i].PawnPrivate = AutoDriver::Read<uint64_t>(engine.player[i].PlayerData + OFFSET_PlayerState_PawnPrivate);
					engine.player[i].rootComponent = AutoDriver::Read<uint64_t>(engine.player[i].PawnPrivate + OFFSET_AActor_USceneComponent_RootComponent);
					//当前玩家主动技能
					engine.player[i].Character = engine.player[i].PlayerData + OFFSET_PlayerState_FCharacterStateData_SlasherData;
					// 读取主动技能id索引
					engine.player[i].powerId = AutoDriver::Read<uint32_t>(engine.player[i].Character + 0x4);
					//获取英文名
					engine.player[i].powerName = GetNameByIndexString(engine.player[i].powerId, GName);
					// //读取主动技能名并匹配映射表
					if (auto it = nameMapping.find(engine.player[i].powerName); it != nameMapping.end()) {
						engine.player[i].powerName = WideToUtf8(it->second);

						engine.player[i].color = COLOR_RED;
					}
					else {
						engine.player[i].color = COLOR_WHITE;
					}

		
					engine.player[i].perks.resize(engine.player[i].EquipedPerkIdsNum);
					for (uint32_t perk = 0; perk < engine.player[i].EquipedPerkIdsNum; ++perk) {
						engine.player[i].perks[perk].PerkNameData = engine.player[i].EquipedPerkIds + perk * sizeof(FName);
						engine.player[i].perks[perk].perkNameIdx = AutoDriver::Read<int32_t>(engine.player[i].perks[perk].PerkNameData + 0x8);
						engine.player[i].perks[perk].perkName = GetNameByIndexString(engine.player[i].perks[perk].perkNameIdx, GName);
				
						if (auto it = PeiZhiPerkNameMap.find(engine.player[i].perks[perk].perkName); it != PeiZhiPerkNameMap.end())
						{
							// 转换宽字符串为UTF-8
							engine.player[i].perks[perk].perkName = WideToUtf8(it->second);
						}
						else {
							std::cout << "未找到技能名映射: " << engine.player[i].perks[perk].perkName << "\n";
						}

					}
				}

				EngingDebugPrintf(engine);


				gui::初始化 = false;
			}

			if (0) {
				static constexpr int32_t OFFSET_APwan_ACharacter_UCharacterMovementComponent = 0x0330;
				uint64_t	CharacterMovement = AutoDriver::Read<uint64_t>(engine.AcknowledgedPawn + OFFSET_APwan_ACharacter_UCharacterMovementComponent);
				static constexpr int32_t OFFSET_APwan_ACharacter_UCharacterMovementComponent_EMovementMode = 0x01DC;
				uint8_t MovementMode = AutoDriver::Read<uint8_t>(CharacterMovement + OFFSET_APwan_ACharacter_UCharacterMovementComponent_EMovementMode);
				switch (MovementMode) {
				case 0: {
					/** 无移动状态 - 默认值或无效状态 */
					DrawList->AddText(nullptr, 50.0f, { screenWidth / 2.f,screenHeight / 2.f }, COLOR_WHITE, WideToUtf8(L"无移动"));
					break;
				}
				case 1: {

					/** 行走模式 - 角色在地面上正常行走 */
					/** - 受重力影响，需要与地面保持接触 */
					/** - 具有标准的行走速度、加速度和转向控制 */
					/** - 常用于玩家角色和AI在平坦地形上的移动 */
					DrawList->AddText(nullptr, 50.0f, { screenWidth / 2.f,screenHeight / 2.f }, COLOR_WHITE, WideToUtf8(L"行走模式"));
					break;
				}
				case 2: {
					/** 导航网格行走模式 - AI角色沿导航路径移动 */
					/** - 与MOVE_Walking类似，但专注于路径跟随 */
					/** - 由AI控制器使用，用于在复杂环境中导航 */
					/** - 通常与导航网格系统(Navigation Mesh)配合使用 */
					DrawList->AddText(nullptr, 50.0f, { screenWidth / 2.f,screenHeight / 2.f }, COLOR_WHITE, WideToUtf8(L"导航网格行走模式"));
					break;
				}
				case 3: {
					/** 下落模式 - 角色在空中下落，如跳跃或跌落 */
					/** - 受重力加速度影响，垂直速度逐渐增加 */
					/** - 可以从跳跃、走下悬崖或被击飞等情况进入 */
					/** - 可以过渡到其他状态，如着陆后变为MOVE_Walking */
					DrawList->AddText(nullptr, 50.0f, { screenWidth / 2.f,screenHeight / 2.f }, COLOR_WHITE, WideToUtf8(L"下落模式"));
					break;
				}
				case 4: {
					/** 游泳模式 - 角色在液体(如水)中移动 */
					/** - 移动速度通常较慢，受浮力影响 */
					/** - 可以在液体表面或水下移动 */
					/** - 常用于水域探索或水上战斗场景 */
					DrawList->AddText(nullptr, 50.0f, { screenWidth / 2.f,screenHeight / 2.f }, COLOR_WHITE, WideToUtf8(L"游泳模式"));
					break;
				}
				case 5: {
					/** 飞行模式 - 角色在三维空间中自由移动，不受重力限制 */
					/** - 提供全方位移动控制，包括上下、前后、左右 */
					/** - 可用于实现飞行生物、飞行器或魔法飞行能力 */
					/** - 在开放世界游戏或空战游戏中常见 */
					DrawList->AddText(nullptr, 50.0f, { screenWidth / 2.f,screenHeight / 2.f }, COLOR_WHITE, WideToUtf8(L"飞行模式"));
					break;
				}
				case 6: {
					/** 自定义移动模式 - 用于实现特殊移动行为 */
					/** - 开发者可基于此扩展自定义移动逻辑，如爬墙、滑行、传送等 */
					/** - 需要实现额外代码处理自定义移动的物理特性和行为规则 */
					/** - 常用于实现游戏特有的机制或能力 */
					DrawList->AddText(nullptr, 50.0f, { screenWidth / 2.f,screenHeight / 2.f }, COLOR_WHITE, WideToUtf8(L"自定义移动模式"));
					break;
				}
				case 7: {

					/** 移动模式最大值 - 非实际移动模式，仅用于标记枚举范围 */
					/** - 用于循环遍历所有移动模式或作为数组大小参考 */
					/** - 添加新移动模式时需相应更新此值 */
					DrawList->AddText(nullptr, 50.0f, { screenWidth / 2.f,screenHeight / 2.f }, COLOR_WHITE, WideToUtf8(L"移动模式最大值"));
					break;
				}
				}

				if (GetAsyncKeyState(VK_SPACE) & 0x0001)
				{
					AutoDriver::Write<uint8_t>(CharacterMovement + OFFSET_APwan_ACharacter_UCharacterMovementComponent_EMovementMode, 3);
				}

			}




			if (gui::绘制) {
				CameraCachePrivate = AutoDriver::Read<FCameraCacheEntry>(engine.PlayerCameraManager + OFFSET_CameraManager_CameraCachePrivate);

				if (gui::技能显示) {
					// 标题
					const char* headers[] = { "PlayerName", "Perk1", "Perk2", "Perk3", "Perk4" };
					for (int col = 0; col < 5; ++col) { DrawList->AddText(nullptr, 20.0f, { 150 + col * cellWidth, 120 }, COLOR_WHITE, headers[col]); }

					// 数据行
					for (size_t row = 0; row < engine.player.size(); ++row) {
						// 指针到当前玩家结构
						const Player& P = engine.player[row];

						// 第 0 列：玩家名
						DrawList->AddText(nullptr, 20.0f, { 150 + 0 * cellWidth, 150 + row * rowHeight }, P.color, P.name.c_str());

						// 第 1~4 列：被动技能
						for (int col = 1; col < 5; ++col) {

							int perkIdx = col - 1;  // 1 列映射到 perks[0]

							// 在适当的位置绘制每列文本
							DrawList->AddText(nullptr, 20.0f, { 150 + col * cellWidth, 150 + row * rowHeight }, COLOR_WHITE, P.perks[perkIdx].perkName.c_str());
						}
					}
				}

				if (gui::连体婴儿子) {
					// 读位置
					uint64_t ConjoinedTwinRootComponent = AutoDriver::Read<uint64_t>(engine.ConjoinedTwin + OFFSET_AActor_USceneComponent_RootComponent);
					FVector ConjoinedTwinPos = AutoDriver::Read<FVector>(ConjoinedTwinRootComponent + OFFSET_USceneComponent_RelativeLocation);
					Vector2 screen = WorldToScreen(CameraCachePrivate.POV, Vector3(ConjoinedTwinPos.X, ConjoinedTwinPos.Y, ConjoinedTwinPos.Z));


					//在屏幕上中
					if (screen.x > 0 && screen.x < screenWidth || screen.y > 0 && screen.y < screenHeight) {

						DrawList->AddText(nullptr, 30.0f, { screen.x, screen.y }, COLOR_PURPLE, WideToUtf8(L"维克托"));
					}

				}

				// 遍历房间玩家
				for (auto Player : engine.player) {

					if (gui::杀手显示) {
						if (Player.color == COLOR_RED) {
							DrawList->AddText(
								NULL, 80.0f, { 10, 20 },
								Player.color, (WideToUtf8(L"当前杀手:") + Player.powerName).c_str()
							);
						}

					}

					if (gui::位置显示) {
						//获取坐标

						FVector pos = AutoDriver::Read<FVector>(Player.rootComponent + OFFSET_USceneComponent_RelativeLocation);
						if (pos.X == 0 && pos.Y == 0 && pos.Z == 0) { continue; }
						Vector3 poss = Vector3(pos.X, pos.Y, pos.Z);
						Vector2 screenPos = WorldToScreen(CameraCachePrivate.POV, poss);

						//在屏幕上中
						if (screenPos.x > 0 && screenPos.x < screenWidth || screenPos.y>0 && screenPos.y < screenHeight) {
							DrawList->AddText(
								NULL, 27.0f, { screenPos.x, screenPos.y },
								Player.color, Player.name.c_str()
							);



							if (gui::距离显示) {
								//计算距离并逃过自己
								Vector3 campos = Vector3(CameraCachePrivate.POV.Location.X, CameraCachePrivate.POV.Location.Y, CameraCachePrivate.POV.Location.Z);
								float distance = (Vector3::Distance(campos, poss) / 39.62f) - 6; if (distance < 0) { continue; }


								DrawList->AddText(
									NULL, 27.0f, { screenPos.x, screenPos.y - 20 },
									Player.color, (std::to_string(static_cast<int>(distance)) + "m").c_str()
								);
							}
						}
					}
				}


				if (gui::图腾显示) {

					// 遍历并绘制图腾
					for (uint64_t totem : engine.TotemsCache) {

						ImU32 color;
						switch (AutoDriver::Read<uint8_t>(totem + OFFSET_ATotem_ETotemState)) {
						case (uint8_t)ETotemState::Cleansed:  continue;
						case (uint8_t)ETotemState::Dull: color = COLOR_WHITE; break; ;
						case (uint8_t)ETotemState::Hex:  color = COLOR_RED; break; ;
						case (uint8_t)ETotemState::Boon: color = COLOR_BLUE; break; ;
						default: color = COLOR_WHITE; break;
						}

						// 读位置
						uint64_t totemRootComponent = AutoDriver::Read<uint64_t>(totem + OFFSET_AActor_USceneComponent_RootComponent); if (!totemRootComponent) continue;
						FVector totemPos = AutoDriver::Read<FVector>(totemRootComponent + OFFSET_USceneComponent_RelativeLocation);
						Vector2 screen = WorldToScreen(CameraCachePrivate.POV, Vector3(totemPos.X, totemPos.Y, totemPos.Z));


						//在屏幕上中
						if (screen.x > 0 && screen.x < screenWidth || screen.y > 0 && screen.y < screenHeight) {

							DrawList->AddText(
								nullptr, 20.0f,
								{ screen.x, screen.y },
								color,
								WideToUtf8(L"图腾")
							);
						}


					}


				}
				if (gui::发电机显示) {

					for (uint64_t generator : engine.GeneratorCache) {

						// 读位置
						uint64_t generatorRootComponent = AutoDriver::Read<uint64_t>(generator + OFFSET_AActor_USceneComponent_RootComponent); if (!generatorRootComponent) continue;
						FVector generatorPos = AutoDriver::Read<FVector>(generatorRootComponent + OFFSET_USceneComponent_RelativeLocation);
						Vector2 screen = WorldToScreen(CameraCachePrivate.POV, Vector3(generatorPos.X, generatorPos.Y, generatorPos.Z));

						//在屏幕上中
						if (screen.x > 0 && screen.x < screenWidth || screen.y > 0 && screen.y < screenHeight) {


							uint64_t _generatorCharge = AutoDriver::Read<uint64_t>(generator + OFFSET_AGenerator_UChargeableComponent);
							//嵌入式结构体不用读取指针
							uint64_t _currentCharge = _generatorCharge + OFFSET_AGenerator_UChargeableComponent_FSpeedBasedNetSyncedValue;
							float _replicatedValue = AutoDriver::Read<float>(_currentCharge + OFFSET_AGenerator_UChargeableComponent_FSpeedBasedNetSyncedValue_replicatedValue);
							//float _replicatedSpeed = AutoDriver::Read<float>(_currentCharge + 0x20);
							DrawList->AddText(
								nullptr, 20.0f,
								{ screen.x, screen.y },
								COLOR_WHITE,
								WideToUtf8(L"发电机进度:" + std::to_wstring(_replicatedValue))
							);



						}

					}
				}
				if (gui::板子显示) {
					for (uint64_t Pallet : engine.Pallets) {

						// 读位置
						uint64_t PalletRootComponent = AutoDriver::Read<uint64_t>(Pallet + OFFSET_AActor_USceneComponent_RootComponent); if (!PalletRootComponent) continue;
						FVector PalletPos = AutoDriver::Read<FVector>(PalletRootComponent + OFFSET_USceneComponent_RelativeLocation);
						Vector2 screen = WorldToScreen(CameraCachePrivate.POV, Vector3(PalletPos.X, PalletPos.Y, PalletPos.Z));

						//在屏幕上中
						if (screen.x > 0 && screen.x < screenWidth || screen.y > 0 && screen.y < screenHeight) {
							static constexpr int32_t	OFFSET_APallet_EPalletState = 0x03E0;
							//static constexpr int32_t	OFFSET_APallet_UBoxComponent = 0x0418;
							//没有被破坏
							if (AutoDriver::Read<uint8_t>(Pallet + OFFSET_APallet_EPalletState) != (uint8_t)EPalletState::Fallen) {

								//碰撞体积
								//UBoxComponent	_pushBox = AutoDriver::Read<UBoxComponent>(Pallet + OFFSET_APallet_UBoxComponent);

								DrawList->AddText(
									nullptr, 20.0f,
									{ screen.x, screen.y },
									COLOR_WHITE,
									WideToUtf8(L"板子")
								);
							}
						}

					}
				}
				if (gui::地窖显示) {
					// 绘制地窖
					if (engine.HatchCache) {

						// 读位置
						uint64_t rootComponent = AutoDriver::Read<uint64_t>(engine.HatchCache + OFFSET_AActor_USceneComponent_RootComponent);
						FVector pos = AutoDriver::Read<FVector>(rootComponent + OFFSET_USceneComponent_RelativeLocation);
						Vector2 screen = WorldToScreen(CameraCachePrivate.POV, Vector3(pos.X, pos.Y, pos.Z));

						//在屏幕上中
						if (screen.x > 0 && screen.x < screenWidth || screen.y > 0 && screen.y < screenHeight) {
							DrawList->AddText(
								nullptr, 30.0f,
								{ screen.x, screen.y },
								COLOR_YELLOW,
								WideToUtf8(L"地窖")
							);
						}
					}
				}
			}

			if (gui::类名调试) {
				CameraCachePrivate = AutoDriver::Read<FCameraCacheEntry>(engine.PlayerCameraManager + OFFSET_CameraManager_CameraCachePrivate);

				//获取每个Actor的地址->获取对象根组件->用根组件获取对象位置
				for (unsigned int i = 0; i < engine.Num; ++i) {

					uint64_t actor = AutoDriver::Read<uint64_t>(engine.actors + i * sizeof(uint64_t)); if (!actor)  continue;

					//获取位置并转为屏幕坐标
					uint64_t rootComponent = AutoDriver::Read<uint64_t>(actor + OFFSET_AActor_USceneComponent_RootComponent); if (!rootComponent) continue;
					FVector pos = AutoDriver::Read<FVector>(rootComponent + OFFSET_USceneComponent_RelativeLocation);
					if (pos.X == 0 && pos.Y == 0 && pos.Z == 0) continue;
					Vector3 poss = Vector3(pos.X, pos.Y, pos.Z);
					Vector2 screenPos = WorldToScreen(CameraCachePrivate.POV, poss);

					//在屏幕上中
					if (screenPos.x > 0 && screenPos.x < screenWidth || screenPos.y > 0 && screenPos.y < screenHeight) {
						//actor继承uobject->Fname->索引
						uint32_t DisplayIndex = AutoDriver::Read<uint32_t>(actor + 0x0018 + 0x8);
						std::string testName = GetNameByIndexString(DisplayIndex, GName);

						DrawList->AddText(
							NULL, 25.0f, { screenPos.x, screenPos.y },
							COLOR_WHITE, testName.c_str()
						);
					}
				}
			}


			// 如果菜单隐藏则不绘制(循环只做一次设置)
			if (overlay::enabled)
			{
				//仅在启用时设置窗口为顶层窗口
				SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);
				//绘制菜单
				DBD_draw_gui();


				// 如果鼠标点击了左键，且没有悬停在任何 ImGui 菜单上
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) &&
					!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) &&
					!ImGui::IsAnyItemHovered())
				{
					//鼠标穿透
					SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED);
					// 弹出 Windows 原生消息框
					MessageBoxA(NULL, "IINSERT隐藏菜单!!!", "提示", MB_OK | MB_ICONWARNING);

				}

			}
			else
			{
				//鼠标穿透
				SetWindowLong(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED);
			}
			/*上面循环调用	SetWindowLong设置窗口属性的问题还没有方法解决！！！！！！！！*/

		}




		ImGui::Render();
		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pSwapChain->Present(1, 0);
	}
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);


	return;
}




bool CreateDeviceD3D(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res == DXGI_ERROR_UNSUPPORTED)
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	if (res != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	switch (msg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		g_ResizeWidth = (UINT)LOWORD(lParam);
		g_ResizeHeight = (UINT)HIWORD(lParam);
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
