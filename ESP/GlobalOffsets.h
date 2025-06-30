#pragma once

#include <cstdint>
#include"Vector.h"
#include <unordered_map>
#include <ostream>
#include <imgui.h>

// 基础类型别名
typedef unsigned char uint8_t;
typedef uint8_t      uint8;


extern int screenWidth;
extern int screenHeight;



// Game.exe 全局变量: GWorld 的偏移
static constexpr int32_t OFFSET_GWorld = 0xAB956E0;
// Game.exe 全局变量: GName 索引表的偏移
static constexpr int32_t OFFSET_GNames = 0xA93D140;
// Game.exe 全局变量: GUObject 列表的偏移
static constexpr int32_t OFFSET_GUObjects = 0xa9ff990;


//所有对象位置获取

// UWorld::Ulevel_PersistentLevel 成员偏移
static constexpr int32_t OFFSET_UWorld_ULevel_PersistentLevel = 0x0038;
// ULevel::TArray<AActor*>_actor
static constexpr int32_t OFFSET_ULevel_Actors = 0x00A0;
// AActor::RootComponent 成员偏移
static constexpr int32_t OFFSET_AActor_USceneComponent_RootComponent = 0x01A8;

// AActor::USceneComponent::RelativeLocation 成员偏移
static constexpr int32_t OFFSET_USceneComponent_RelativeLocation = 0x0150;



//本地玩家相机获取


// UWorld::OwningGameInstance 成员偏移
static constexpr int32_t OFFSET_GWorld_OwningGameInstance = 0x01C8;
// UGameInstance::LocalPlayers 成员偏移 (TArray<UPlayer*>)
static constexpr int32_t OFFSET_OwningGameInstance_LocalPlayers = 0x0040;
// ULocalPlayer::PlayerController 成员偏移
static constexpr int32_t OFFSET_LocalPlayer_PlayerController = 0x0038;

// APlayerController::AcknowledgedPawn 成员偏移
static constexpr int32_t OFFSET_PlayerController_AcknowledgedPawn = 0x0350;
// APlayerController::CameraManager 成员偏移
static constexpr int32_t OFFSET_PlayerController_CameraManager = 0x0360;
// APlayerCameraManager::CameraCachePrivate 成员偏移
static constexpr int32_t OFFSET_CameraManager_CameraCachePrivate = 0x22D0;


//房间玩家获取

// UWorld::GameState 成员偏移 (AGameStateBase*)
static constexpr int32_t OFFSET_GWorld_GameState = 0x0168;

// AGameStateBase::PlayerArray 成员偏移 (TArray<APlayerState*>)
static constexpr int32_t OFFSET_GameState_PlayerState = 0x02B8;

// APlayerState::PlayerNamePrivate 成员偏移 (FString)
static constexpr int32_t OFFSET_PlayerState_PlayerNamePrivate = 0x03A0;

// APlayerState::PawnPrivate 成员偏移 (AActor*)
static constexpr int32_t OFFSET_PlayerState_PawnPrivate = 0x0320;

// DBDPlayerState::CamperData 成员偏移 (嵌入式 FCharacterStateData)
static constexpr int32_t OFFSET_PlayerState_FCharacterStateData_CamperData = 0x0410;

// DBDPlayerState::SlasherData 成员偏移 (嵌入式 FCharacterStateData)
static constexpr int32_t OFFSET_PlayerState_FCharacterStateData_SlasherData = 0x0430;

// DBDPlayerState::PlayerData 成员偏移 (嵌入式 FPlayerStateData)
static constexpr int32_t OFFSET_PlayerState_FPlayerStateData_PlayerData = 0x0450;

//图腾状态

// ATotem继承Aactor
static constexpr int32_t OFFSET_ATotem_ETotemState = 0x03F8;
// AGenerator继承Aactor
static constexpr int32_t OFFSET_AGenerator_UChargeableComponent = 0x0620;
static constexpr int32_t OFFSET_AGenerator_UChargeableComponent_FSpeedBasedNetSyncedValue = 0x0198;
static constexpr int32_t OFFSET_AGenerator_UChargeableComponent_FSpeedBasedNetSyncedValue_replicatedValue = 0x0018;


struct Perks {


	uint64_t PerkNameData;// 被动技能名称数据指针
	int32_t perkNameIdx;// 被动技能索引
	std::string perkName; // 被动技能名称

};

struct  Player {
public:
	uint64_t PlayerData;//玩家指针
	std::string name;//玩家名称
	uint64_t  PerkStruct;// Perk结构体
	uint64_t EquipedPerkIds; // 装备的被动技能ID数组指针
	int32_t  EquipedPerkIdsNum; // 装备的被动技能ID数量

	uint64_t Character;// 主动技能指针
	uint32_t powerId;// 主动技能名索引
	std::string powerName; // 主动技能名称

	ImU32 color;//区分杀手和幸存者颜色

	std::vector<Perks> perks; // 被动技能ID数组

	uint64_t	PawnPrivate;
	uint64_t	rootComponent;//根组件

};

class Engine {

public:

	//世界所有对象
	uint64_t pLevel;
	uint64_t actors;
	uint32_t Num;

	//本地玩家相机
	uint64_t GameInstance;
	uint64_t LocalPlayer;
	uint64_t PlayerController;
	uint64_t AcknowledgedPawn;
	uint64_t PlayerCameraManager;

	//房间所有玩家数组
	uint64_t GameState;
	uint64_t baseArray;
	uint32_t PlayerCount;
	uint32_t PlayerCountMax;


	std::vector<uint64_t> TotemsCache;	// 图腾
	std::vector<uint64_t> GeneratorCache;	// 发电机
	std::vector<uint64_t>  Pallets;
	std::vector<uint64_t>  窗口;
	uint64_t HatchCache;//地窖
	uint64_t ConjoinedTwin;//连体婴角色的儿子

	std::vector<Player> player; // 玩家数据

};



// 表示三维空间中的位置或方向
struct FVector {
public:
	double X; // 0x0000: X 轴坐标
	double Y; // 0x0008: Y 轴坐标
	double Z; // 0x0010: Z 轴坐标
};

// 表示三维空间中的旋转角度
struct FRotator {
public:
	double Pitch; // 0x0000: 绕 X 轴俯仰角
	double Yaw;   // 0x0008: 绕 Y 轴偏航角
	double Roll;  // 0x0010: 绕 Z 轴翻滚角
};

// 存储相机的最小视图信息
struct FMinimalViewInfo {
public:
	FVector Location;   // 0x0000: 相机世界位置
	FRotator Rotation;  // 0x0018: 相机旋转
	float    FOV;       // 0x0030: 视野范围
	uint8    Padding[4]; // 0x0034: 对齐填充
};

// 用于缓存相机某一时刻的视图状态
struct FCameraCacheEntry {
public:
	float            Timestamp; // 0x0000: 时间戳
	uint8            Pad[12];   // 0x0004: 填充对齐
	FMinimalViewInfo POV;       // 0x0010: 相机视图信息
};



// 预计算常量
static constexpr float DEG_TO_RAD = 0.01745329251994329577f; // π/180
static constexpr float FOV_DEG_TO_RAD = 0.00872664625997164788f; // π/360
inline __forceinline Vector2 WorldToScreen(const FMinimalViewInfo& viewinfo, const Vector3& world)
{
	// 提取旋转角度并转换为弧度
	const float radPitch = viewinfo.Rotation.Pitch * DEG_TO_RAD;
	const float radYaw = viewinfo.Rotation.Yaw * DEG_TO_RAD;
	const float radRoll = viewinfo.Rotation.Roll * DEG_TO_RAD;

	// 一次性计算所有三角函数
	const float SP = sinf(radPitch);
	const float CP = cosf(radPitch);
	const float SY = sinf(radYaw);
	const float CY = cosf(radYaw);
	const float SR = sinf(radRoll);
	const float CR = cosf(radRoll);

	// 预计算常用乘积
	const float SRSP = SR * SP;
	const float CRSP = CR * SP;

	// 直接计算旋转轴分量（避免创建Vector3对象）
	// vAxisX = (CP * CY, CP * SY, SP)
	const float axisX_x = CP * CY;
	const float axisX_y = CP * SY;
	const float axisX_z = SP;

	// vAxisY = (SR * SP * CY - CR * SY, SR * SP * SY + CR * CY, -SR * CP)
	const float axisY_x = SRSP * CY - CR * SY;
	const float axisY_y = SRSP * SY + CR * CY;
	const float axisY_z = -SR * CP;

	// vAxisZ = (-(CR * SP * CY + SR * SY), CY * SR - CR * SP * SY, CR * CP)
	const float axisZ_x = -(CRSP * CY + SR * SY);
	const float axisZ_y = CY * SR - CRSP * SY;
	const float axisZ_z = CR * CP;

	// 计算世界坐标与相机位置的差值
	const float deltaX = world.x - viewinfo.Location.X;
	const float deltaY = world.y - viewinfo.Location.Y;
	const float deltaZ = world.z - viewinfo.Location.Z;

	// 手动计算点积进行坐标变换
	const float transformedX = deltaX * axisY_x + deltaY * axisY_y + deltaZ * axisY_z;
	const float transformedY = deltaX * axisZ_x + deltaY * axisZ_y + deltaZ * axisZ_z;
	const float transformedZ = deltaX * axisX_x + deltaY * axisX_y + deltaZ * axisX_z;

	// 避免除零和负Z值
	const float z = (transformedZ < 1.0f) ? 1.0f : transformedZ;

	// 预计算屏幕中心和FOV相关值
	const float centerX = screenWidth * 0.5f;
	const float centerY = screenHeight * 0.5f;
	const float fovScale = centerX / tanf(viewinfo.FOV * FOV_DEG_TO_RAD);
	const float invZ = fovScale / z;

	// 计算最终屏幕坐标
	const float screenX = centerX + transformedX * invZ;
	const float screenY = centerY - transformedY * invZ;

	return { screenX, screenY };
}


struct FNameEntryHeader final
{
public:
	uint16_t                                        bIsWide : 1;       // 0x0000(0x0002) 字符串编码标志：0=ANSI(单字节)，1=Wide(双字节)
	uint16_t                                        Len : 15;          // 0x0000(0x0002) 字符串长度(0-32767)，最大支持32767个字符
};


union FStringData final
{
public:
	char                                          AnsiName[0x400];    // 0x0000(0x0800) ANSI字符串存储区(1024字节)
	wchar_t                                       WideName[0x400];    // 0x0000(0x0800) Wide字符串存储区(2048字节，宽字符占2字节)
};


struct FNameEntry final
{
public:
	uint8                                         Pad_0[0x4];         // 0x0000(0x0004) 内存对齐填充
	struct FNameEntryHeader                       Header;             // 0x0004(0x0002) 字符串元数据(编码类型和长度)
	union FStringData                             Name;               // 0x0006(0x0800) 字符串实际存储区(根据Header选择ANSI或Wide)
};



template<typename ArrayElementType>
class TArray
{
public:
	ArrayElementType* Data;
	int32_t NumElements;
	int32_t MaxElements;
};

class FName final
{
public:
	int32_t ComparisonIndex;  //0x0 // 打包了真正的 DisplayIndex（名字表下标）+ 若干标志位
	int32_t Number;           //0x4 // 实例编号，用于区分同名不同实例
	int32_t DisplayIndex;     //0x8// 真正的名字表下标（新版引擎）
};


//存储角色战斗状态与核心能力（如主动技能、附加物、段位）
struct FCharacterStateData final
{
	int32_t                                         _pips;                  // 角色的段位/积分（影响匹配等级或奖励进度）
	class FName                                   _powerId;                // 角色主动技能/力量ID（杀手专属，如"Power_Teleport"传送；幸存者通常无实际意义）
	TArray<class FName>                           _addonIds;              // 装备的附加物ID数组（强化主动技能效果，如缩短杀手力量冷却时间）

	// ---------------------------
	// 内存布局说明（引擎自动生成）：
	// 0x0000(0x0004)：_pips 占4字节
	// 0x0004(0x000C)：_powerId 占12字节（FName内部结构）
	// 0x0010(0x0010)：_addonIds 数组指针及元数据占16字节
	// 总大小：0x0020字节（32字节）
	// ---------------------------
};
//存储玩家自定义配置与被动技能（如装备的被动技能、等级、徽章）
struct FPlayerStateData final
{
public:
	int32_t                                         CharacterLevel;            // 角色等级（影响被动技能解锁与等级上限）
	class FName                                   EquipedFavorId;             // 装备的祭品ID（影响游戏内特殊机制，如血点获取效率）
	TArray<class FName>                           EquipedPerkIds;            // **核心字段**：装备的被动技能ID数组（如"Perk_RepairSpeedBoost"修理加速）
	TArray<int32_t>                                 EquipedPerkLevels;      // 对应被动技能的等级（1-3级，决定技能效果强度）
	//	class FString                                 EquippedBannerId;        // 装备的横幅ID（外观展示，与技能无关）
	//	class FString                                 EquippedBadgeId;        // 装备的徽章ID（成就展示，与技能无关）
	//	class FName                                   EquippedCharacterClass;// 角色职业ID（如"Survivor_Leon"或"Killer_Michael"，决定可装备的技能池）
	//	EGameState                                    _playerGameState;     // 玩家游戏状态（存活/倒地/死亡等，与技能触发条件相关）
	//	uint8                                         Pad_5D[0x3];         // 内存对齐填充（引擎自动生成，无需手动处理）
	//	int32_t                                         _prestigeLevel;     // 声望等级（解锁角色特殊外观或技能升级路径）
	//	uint8                                         Pad_64[0x4];       // 内存对齐填充（确保结构体总大小为4字节倍数）

		// ---------------------------
		// 内存布局说明（引擎自动生成）：
		// 0x0000(0x0004)：CharacterLevel 占4字节
		// 0x0004(0x000C)：EquipedFavorId 占12字节
		// 0x0010(0x0010)：EquipedPerkIds 数组占16字节
		// 0x0020(0x0010)：EquipedPerkLevels 数组占16字节
		// 0x0030(0x0010)：EquippedBannerId 占16字节（FString内部结构）
		// 0x0040(0x0010)：EquippedBadgeId 占16字节
		// 0x0050(0x000C)：EquippedCharacterClass 占12字节
		// 0x005C(0x0001)：_playerGameState 占1字节
		// 0x005D(0x0003)：Pad_5D 填充3字节（对齐到0x0060）
		// 0x0060(0x0004)：_prestigeLevel 占4字节
		// 0x0064(0x0004)：Pad_64 填充4字节（总大小对齐到0x0068）
		// 总大小：0x0068字节（104字节）
		// ---------------------------
};

//技能检定
struct FSpeedBasedNetSyncedValue
{
	uint8   Pad_0[0x18];
	float   ReplicatedValue;             // offset 0x18
	uint8   Pad_1C[0x4];
	float   ReplicatedSpeed;             // offset 0x20
	uint8   Pad_24[0x4];
	float   ReplicatedLastUpdateTimestamp; // offset 0x28
	uint8   Pad_2C[0xC];
};

//图腾颜色和状态
struct FLinearColor final
{
public:
	float                                         R;                                                 // 0x0000(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         G;                                                 // 0x0004(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         B;                                                 // 0x0008(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                         A;                                                 // 0x000C(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

enum class ETotemState : uint8
{
	Cleansed = 0,  // 图腾已被净化，处于初始/纯净状态
	Dull = 1,      // 图腾失去能量，处于无活性状态
	Hex = 2,       // 图腾被诅咒，施加负面效果
	Boon = 3,      // 图腾被祝福，提供正面增益
	ETotemState_MAX = 4,
};
// 木板状态枚举类型，用于表示游戏中木板的不同状态
enum class EPalletState : uint8
{
	// 木板直立状态（未被放下）
	Up = 0,

	// 木板正在下落过程中（从直立到放下的动画阶段）
	Falling = 1,

	// 木板已放下状态（玩家可用于阻挡或翻越）
	Fallen = 2,

	// 木板已被破坏状态（无法再交互）
	Destroyed = 3,

	// 枚举值上限标记（用于数组边界检查等场景）
	EPalletState_MAX = 4,
};
//木板碰撞盒
class UBoxComponent
{
public:
	struct FVector                                BoxExtent;                                         // 0x05C0(0x0018)(Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	float                                         LineThickness;                                     // 0x05D8(0x0004)(Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	uint8                                         Pad_5DC[0x4];                                      // 0x05DC(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
};


//可以根据杀手的类名或主动技能分辨杀手角色
struct 杀手 {
	inline static const std::string 夹子 = "BP_Slasher_Character_01_C";
	inline static const std::string 小叮当 = "BP_Slasher_Character_02_C";
	inline static const std::string 瘸瘸 = "BP_Slasher_Character_03_C";
	inline static const std::string 护士 = "BP_Slasher_Character_04_C";
	inline static const std::string 迈叔 = "BP_Slasher_Character_05_C";
	inline static const std::string 李奶奶 = "BP_Slasher_Character_06_C";
	inline static const std::string 老杨 = "BP_Slasher_Character_07_C";
	inline static const std::string 兔妈 = "BP_Slasher_Character_08_C";
	inline static const std::string 皮脸 = "BP_Slasher_Character_09_C";
	inline static const std::string 梦魇 = "BP_Slasher_Character_10_C";
	inline static const std::string 猪妹 = "BP_Slasher_Character_11_C";
	inline static const std::string 小丑 = "BP_Slasher_Character_12_C";
	inline static const std::string 零妹 = "BP_Slasher_Character_13_C";
	inline static const std::string 军团 = "BP_Slasher_Character_14_C";
	inline static const std::string 吐妈 = "BP_Slasher_Character_15_C";
	inline static const std::string 鬼面 = "BP_Slasher_Character_16_C";
	inline static const std::string 魔王 = "BP_Slasher_Character_17_C";
	inline static const std::string 鬼武士 = "BP_Slasher_Character_18_C";
	inline static const std::string 抢手 = "BP_Slasher_Character_19_C";
	inline static const std::string 三角头 = "BP_Slasher_Character_20_C";
	inline static const std::string 枯魔 = "BP_Slasher_Character_21_C";
	inline static const std::string 连体婴 = "BP_Slasher_Character_22_C";
	inline static const std::string 哥哥 = "BP_Slasher_Character_23_C";
	inline static const std::string 追击者 = "BP_Slasher_Character_24_C";
	inline static const std::string 钉子头 = "BP_Slasher_Character_25_C";
	inline static const std::string 鸟姐 = "BP_Slasher_Character_26_C";
	inline static const std::string 贞子 = "BP_Slasher_Character_27_C";
	inline static const std::string 影魔 = "BP_Slasher_Character_28_C";
	inline static const std::string 威斯克 = "BP_Slasher_Character_29_C";
	inline static const std::string 恶骑士 = "BP_Slasher_Character_30_C";
	inline static const std::string 白骨商人 = "BP_Slasher_Character_31_C";
	inline static const std::string 奇点 = "BP_Slasher_Character_32_C";
	inline static const std::string 异形 = "BP_Slasher_Character_33_C";
	inline static const std::string 好孩子 = "BP_Slasher_Character_34_C";
	inline static const std::string 未知恶物 = "BP_Slasher_Character_35_C";
	inline static const std::string 巫妖 = "BP_Slasher_Character_36_C";
	inline static const std::string 德古拉 = "BP_Slasher_Character_37_C";
	inline static const std::string 潘红 = "BP_Slasher_Character_38_C";
	inline static const std::string 金木研 = "BP_Slasher_Character_39_C";
	inline static const std::string 老登 = "BP_Slasher_Character_40_01_C";


	inline static const std::string 夹子主动技能 = "Item_Slasher_Beartrap";
	inline static const std::string 小叮当主动技能 = "Item_Slasher_CloakBell";
	inline static const std::string 瘸瘸主动技能 = "Item_Slasher_Chainsaw";
	inline static const std::string 护士主动技能 = "Item_Slasher_Blinker";
	inline static const std::string 迈叔主动技能 = "Item_Slasher_Stalker";
	inline static const std::string 李奶奶主动技能 = "Item_Slasher_PhantomTrap";
	inline static const std::string 老杨主动技能 = "Item_Slasher_Killer07Item";
	inline static const std::string 兔妈主动技能 = "Item_Slasher_Hatchet";
	inline static const std::string 皮脸主动技能 = "Item_Slasher_LFChainsaw";
	inline static const std::string 梦魇主动技能 = "Item_Slasher_DreamInducer";
	inline static const std::string 猪妹主动技能 = "Item_Slasher_ReverseBearTrap";
	inline static const std::string 小丑主动技能 = "Item_Slasher_GasBomb";
	inline static const std::string 零妹主动技能 = "Item_Slasher_PhaseWalker";
	inline static const std::string 军团主动技能 = "Item_Slasher_Frenzy";
	inline static const std::string 吐妈主动技能 = "Item_Slasher_PlaguePower";
	inline static const std::string 鬼面主动技能 = "Item_Slasher_GhostPower";
	inline static const std::string 魔王主动技能 = "Item_Slasher_QatarKillerPower";
	inline static const std::string 鬼武士主动技能 = "Item_Slasher_Kanobo";
	inline static const std::string 抢手主动技能 = "Item_Slasher_HarpoonRifle";
	inline static const std::string 三角头主动技能 = "Item_Slasher_TormentMode";
	inline static const std::string 枯魔主动技能 = "Item_Slasher_K21Power";
	inline static const std::string 连体婴主动技能 = "Item_Slasher_K22Power";
	inline static const std::string 哥哥主动技能 = "Item_Slasher_ThrowingKnives";
	inline static const std::string 追击者主动技能 = "Item_Slasher_K24Power";
	inline static const std::string 钉子头主动技能 = "Item_Slasher_K25Power";
	inline static const std::string 鸟姐主动技能 = "Item_Slasher_K26Power";
	inline static const std::string 贞子主动技能 = "Item_Slasher_K27Power";
	inline static const std::string 影魔主动技能 = "Item_Slasher_K28Power";
	inline static const std::string 威斯克主动技能 = "Item_Slasher_K29Power";
	inline static const std::string 恶骑士主动技能 = "Item_Slasher_K30Power";
	inline static const std::string 白骨商人主动技能 = "Item_Slasher_K31Power";
	inline static const std::string 奇点主动技能 = "Item_Slasher_K32Power";
	inline static const std::string 异形主动技能 = "Item_Slasher_K33Power";
	inline static const std::string 好孩子主动技能 = "Item_Slasher_K34Power";
	inline static const std::string 未知恶物主动技能 = "Item_Slasher_K35Power";
	inline static const std::string 巫妖主动技能 = "Item_Slasher_K36Power";
	inline static const std::string 德古拉主动技能 = "Item_Slasher_K37Power";
	inline static const std::string 潘红主动技能 = "Item_Slasher_K38Power";
	inline static const std::string 金木研主动技能 = "Item_Slasher_K39Power";
	inline static const std::string 老登主动技能 = "Item_Slasher_K40Power";

};

// 杀手名称映射表
static const std::unordered_map<std::string, std::wstring> nameMapping = {
	{杀手::夹子, L"夹子"},
	{杀手::小叮当, L"小叮当"},
	{杀手::瘸瘸, L"瘸瘸"},
	{杀手::护士, L"护士"},
	{杀手::迈叔, L"迈叔"},
	{杀手::李奶奶, L"李奶奶"},
	{杀手::老杨, L"老杨"},
	{杀手::皮脸, L"皮脸"},
	{杀手::兔妈, L"兔妈"},
	{杀手::梦魇, L"梦魇"},
	{杀手::猪妹, L"猪妹"},
	{杀手::小丑, L"小丑"},
	{杀手::零妹, L"零妹"},
	{杀手::军团, L"军团"},
	{杀手::吐妈, L"吐妈"},
	{杀手::鬼面, L"鬼面"},
	{杀手::魔王, L"魔王"},
	{杀手::鬼武士, L"鬼武士"},
	{杀手::抢手, L"抢手"},
	{杀手::三角头, L"三角头"},
	{杀手::枯魔, L"枯魔"},
	{杀手::连体婴, L"连体婴"},
	{杀手::哥哥, L"哥哥"},
	{杀手::追击者, L"追击者"},
	{杀手::钉子头, L"钉子头"},
	{杀手::鸟姐, L"鸟姐"},
	{杀手::贞子, L"贞子"},
	{杀手::影魔, L"影魔"},
	{杀手::威斯克, L"威斯克"},
	{杀手::恶骑士, L"恶骑士"},
	{杀手::白骨商人, L"白骨商人"},
	{杀手::奇点, L"奇点"},
	{杀手::异形, L"异形"},
	{杀手::好孩子, L"好孩子"},
	{杀手::未知恶物, L"未知恶物"},
	{杀手::巫妖, L"巫妖"},
	{杀手::德古拉, L"德古拉"},
	{杀手::潘红, L"潘红"},
	{杀手::金木研, L"金木研"},
	{杀手::老登, L"老登"},


	{杀手::夹子主动技能, L"夹子"},
	{杀手::小叮当主动技能, L"小叮当"},
	{杀手::瘸瘸主动技能, L"瘸瘸"},
	{杀手::护士主动技能, L"护士"},
	{杀手::迈叔主动技能, L"迈叔"},
	{杀手::李奶奶主动技能, L"李奶奶"},
	{杀手::老杨主动技能, L"老杨"},
	{杀手::兔妈主动技能, L"兔妈"},
	{杀手::皮脸主动技能, L"皮脸"},
	{杀手::梦魇主动技能, L"梦魇"},
	{杀手::猪妹主动技能, L"猪妹"},
	{杀手::小丑主动技能, L"小丑"},
	{杀手::零妹主动技能, L"零妹"},
	{杀手::军团主动技能, L"军团"},
	{杀手::吐妈主动技能, L"吐妈"},
	{杀手::鬼面主动技能, L"鬼面"},
	{杀手::魔王主动技能, L"魔王"},
	{杀手::鬼武士主动技能, L"鬼武士"},
	{杀手::抢手主动技能, L"抢手"},
	{杀手::三角头主动技能, L"三角头"},
	{杀手::枯魔主动技能, L"枯魔主"},
	{杀手::连体婴主动技能, L"连体婴"},
	{杀手::哥哥主动技能, L"哥哥"},
	{杀手::追击者主动技能, L"追击者"},
	{杀手::钉子头主动技能, L"钉子头"},
	{杀手::鸟姐主动技能, L"鸟姐"},
	{杀手::贞子主动技能, L"贞子"},
	{杀手::影魔主动技能, L"影魔"},
	{杀手::威斯克主动技能, L"威斯克"},
	{杀手::恶骑士主动技能, L"恶骑士"},
	{杀手::白骨商人主动技能, L"白骨商人"},
	{杀手::奇点主动技能, L"奇点"},
	{杀手::异形主动技能, L"异形"},
	{杀手::好孩子主动技能, L"好孩子"},
	{杀手::未知恶物主动技能, L"未知恶物"},
	{杀手::巫妖主动技能, L"巫妖"},
	{杀手::德古拉主动技能, L"德古拉"},
	{杀手::潘红主动技能, L"潘红"},
	{杀手::金木研主动技能, L"金木研"},
	{杀手::老登主动技能, L"老登"}

};


struct 幸存者配置技能 {
	inline static const std::string 不惜一切1 = "";
	inline static const std::string 不惜一切2 = "AnyMeansNecessary";
	inline static const std::string 不痛不痒 = "NoMither";
	inline static const std::string 不祥预感 = "Premonition";
	inline static const std::string 与我共舞 = "Dance_with_me";
	inline static const std::string 东山再起 = "S26P02";
	inline static const std::string 为生而生 = "S37P02";
	inline static const std::string 亡父警言 = "S30P01";
	inline static const std::string 交稿日 = "S40P03";
	inline static const std::string 人之勇气 = "TheMettleOfMan";
	inline static const std::string 似曾相识 = "Deja_Vu";
	inline static const std::string 低调行事 = "S32P03";
	inline static const std::string 余光长存 = "S31P02";
	inline static const std::string 保姆 = "Babysitter";
	inline static const std::string 光明勇士 = "S40P01";
	inline static const std::string 克服难关 = "S29P01";
	inline static const std::string 兵不厌诈 = "S24P02";
	inline static const std::string 内在专注 = "S31P01";
	inline static const std::string 内心之力 = "InnerStrength";
	inline static const std::string 冲刺爆发 = "Sprint_Burst";
	inline static const std::string 分崩离析 = "Breakdown";
	inline static const std::string 切勿伤害 = "S46P01";
	inline static const std::string 劫后余生 = "AfterCare";
	inline static const std::string 勇往直前 = "HeadOn";
	inline static const std::string 化学陷阱 = "S39P02";
	inline static const std::string 千里眼 = "S28P01";
	inline static const std::string 即兴表演 = "S38P01";
	inline static const std::string 压抑同盟 = "RepressedAlliance";
	inline static const std::string 友谊赛 = "S35P02";
	inline static const std::string 反制之力 = "S26P01";
	inline static const std::string 各自为安 = "S45P02";
	inline static const std::string 同志情谊 = "Camaraderie";
	inline static const std::string 同族 = "Kindred";
	inline static const std::string 同生共死 = "No_One_Left_Behind";
	inline static const std::string 吟游诗人激励 = "S42P02";
	inline static const std::string 咬紧牙关 = "S27P01";
	inline static const std::string 善有善报 = "Deliverance";
	inline static const std::string 团结一致 = "Solidarity";
	inline static const std::string 团队合作_二人之力 = "S35P03";
	inline static const std::string 团队合作_全体隐蔽 = "S36P03";
	inline static const std::string 坚不可摧 = "SelfSufficient";
	inline static const std::string 声东击西 = "Diversion";
	inline static const std::string 复苏之风 = "SecondWind";
	inline static const std::string 好运会传染 = "Up_The_Ante";
	inline static const std::string 守夜人 = "Vigil";
	inline static const std::string 安抚生灵 = "Calm_Spirit";
	inline static const std::string 定心丸 = "S33P02";
	inline static const std::string 对策治疗 = "S32P02";
	inline static const std::string 希望之翼 = "Hope";
	inline static const std::string 幕后玩家 = "S36P01";
	inline static const std::string 平稳着陆 = "Balanced_Landing";
	inline static const std::string 并行进展 = "FlipFlop";
	inline static const std::string 幸运喘息 = "LuckyBreak";
	inline static const std::string 幸运星 = "S39P01";
	inline static const std::string 强力挣扎 = "S24P03";
	inline static const std::string 强盗直觉 = "Plunderers_Instinct";
	inline static const std::string 当机立断 = "S35P01";
	inline static const std::string 心灵共鸣 = "Empathy";
	inline static const std::string 心静眼明 = "S42P03";
	inline static const std::string 快速反应 = "S46P03";
	inline static const std::string 快速策略 = "S34P03";
	inline static const std::string 急于求成 = "S31P03";
	inline static const std::string 急速静谧 = "QuickQuiet";
	inline static const std::string 恩赐_指数增长 = "S29P03";
	inline static const std::string 恩赐_明亮 = "S40P02";
	inline static const std::string 恩赐_暗影步 = "S28P03";
	inline static const std::string 恩赐_治疗之环 = "S28P02";
	inline static const std::string 恩赐_黑暗理论 = "S30P03";
	inline static const std::string 感同身受 = "S30P02";
	inline static const std::string 成功捷径 = "S25P01";
	inline static const std::string 技术员 = "Technician";
	inline static const std::string 挣脱激励 = "Breakout";
	inline static const std::string 携手合作 = "Bond";
	inline static const std::string 新人精神 = "S27P03";
	inline static const std::string 时不我待 = "BorrowedTime";
	inline static const std::string 明察秋毫 = "S24P01";
	inline static const std::string 暗黑感知 = "Dark_Sense";
	inline static const std::string 暴走族 = "Urban_Evasion";
	inline static const std::string 最后的生还者 = "SoleSurvivor";
	inline static const std::string 未雨绸缪 = "BuckleUp";
	inline static const std::string 末日残兵 = "LeftBehind";
	inline static const std::string 机遇之窗 = "WindowsOfOpportunity";
	inline static const std::string 果断反击 = "DecisiveStrike";
	inline static const std::string 毛骨悚然 = "Spine_Chill";
	inline static const std::string 气场失真 = "Distortion";
	inline static const std::string 永不言弃 = "Tenacity";
	inline static const std::string 求生欲 = "S43P02";
	inline static const std::string 浴血冲刺 = "S36P02";
	inline static const std::string 清道夫 = "S37P03";
	inline static const std::string 潜力无限 = "S34P01";
	inline static const std::string 灵魂守卫 = "SoulGuard";
	inline static const std::string 焕然一新 = "S33P01";
	inline static const std::string 爆炸地雷 = "S26P03";
	inline static const std::string 狩猎经验 = "Small_Game";
	inline static const std::string 生死与共 = "WereGonnaLiveForever";
	inline static const std::string 百折不挠 = "Resilience";
	inline static const std::string 矫健身手 = "S43P01";
	inline static const std::string 矫正措施 = "S29P02";
	inline static const std::string 破坏手 = "Saboteur";
	inline static const std::string 破案心切 = "StakeOut";
	inline static const std::string 祈祷_织网蜘蛛 = "S41P01";
	inline static const std::string 祈祷_险恶乌鸦 = "S45P01";
	inline static const std::string 稳操胜券 = "Open_Handed";
	inline static const std::string 窃听器 = "S32P01";
	inline static const std::string 答疑解惑 = "S37P01";
	inline static const std::string 精研百草 = "Botany_Knowledge";
	inline static const std::string 经久耐用 = "BuiltToLast";
	inline static const std::string 绝境对策 = "DesperateMeasures";
	inline static const std::string 老本行 = "S43P03";
	inline static const std::string 联袂主演 = "S38P02";
	inline static const std::string 聚精会神 = "S33P03";
	inline static const std::string 肩负重担 = "S45P03";
	inline static const std::string 肾上腺素 = "Adrenaline";
	inline static const std::string 胜利喜悦 = "S44P02";
	inline static const std::string 脱缰野马 = "BoilOver";
	inline static const std::string 自学成才 = "Autodidact";
	inline static const std::string 自我保护 = "S25P03";
	inline static const std::string 自我疗愈 = "Self_Care";
	inline static const std::string 舍己为人 = "ForThePeople";
	inline static const std::string 荣耀时刻 = "S44P03";
	inline static const std::string 药到病除 = "Pharmacy";
	inline static const std::string 蓄势待发 = "Poised";
	inline static const std::string 警戒 = "Alert";
	inline static const std::string 警探直觉 = "DetectivesHunch";
	inline static const std::string 认定目标 = "Fixated";
	inline static const std::string 证明自己 = "Prove_Thyself";
	inline static const std::string 谨慎责任 = "S46P02";
	inline static const std::string 贝尔蒙特之眼 = "S44P01";
	inline static const std::string 跌宕剧情 = "S38P03";
	inline static const std::string 转移注意 = "RedHerring";
	inline static const std::string 轰动演出 = "S25P02";
	inline static const std::string 轻如鸿毛 = "Lightweight";
	inline static const std::string 轻盈敏捷 = "Lithe";
	inline static const std::string 轻盈猫步 = "S39P03";
	inline static const std::string 远见卓识 = "Visionary";
	inline static const std::string 迷雾慧眼 = "S34P02";
	inline static const std::string 逃之夭夭 = "Slippery_Meat";
	inline static const std::string 邪祟之力 = "S41P03";
	inline static const std::string 都市生存 = "St";//Streetwise改为St
	inline static const std::string 醒醒！！！ = "Wakeup";
	inline static const std::string 钢筋铁骨 = "DeadHard";
	inline static const std::string 钢铁意志 = "Iron_Will";
	inline static const std::string 锦囊妙计 = "Ace_In_The_Hole";
	inline static const std::string 镜影幻象 = "S42P01";
	inline static const std::string 闪光弹 = "S27P02";
	inline static const std::string 隐匿之势 = "S41P02";
	inline static const std::string 难以置信 = "This_Is_Not_Happening";
	inline static const std::string 非正式调查 = "OffTheRecord";
	inline static const std::string 领袖群雄 = "Leader";
	inline static const std::string 马到成功 = "WellMakeIt";
	inline static const std::string 魂牵梦绕 = "ObjectOfObsession";
	inline static const std::string 鲜血契约 = "BloodPact";
	inline static const std::string 齐心协力 = "BetterTogether";

};


struct 杀手配置技能 {

	inline static const std::string 小丑恐惧症 = "Coulrophobia";
	inline static const std::string 压迫性气场 = "OverwhelmingPresence";
	inline static const std::string 恶灵低语 = "Whispers";
	inline static const std::string 死锁 = "K25P01";
	inline static const std::string 不屈不挠 = "Unrelenting";
	inline static const std::string 了如指掌 = "Surveillance";
	inline static const std::string 人性贪婪 = "K37P02";
	inline static const std::string 人肉烧烤 = "BBQAndChilli";
	inline static const std::string 做出你的选择 = "MakeYourChoice";
	inline static const std::string 光明之眼 = "Lightborn";
	inline static const std::string 冷酷之拥 = "K26P01";
	inline static const std::string	倦怠之触 = "K36P02";
	inline static const std::string 击倒出局 = "InTheDark";
	inline static const std::string 切肤之痛 = "K22P02";
	inline static const std::string 厄咒_不死 = "HexUndying";
	inline static const std::string 厄咒_以牙还牙 = "K34P01";
	inline static const std::string 厄咒_众生皆苦 = "K39P01";
	inline static const std::string 厄咒_余痕 = "K26P03";
	inline static const std::string 厄咒_吞噬希望 = "Hex_Devour_Hope";
	inline static const std::string 厄咒_悲惨命运 = "K37P01";
	inline static const std::string 厄咒_惩戒 = "HexRetribution";
	inline static const std::string 厄咒_毁灭 = "Hex_Ruin";
	inline static const std::string 厄咒_摇篮曲 = "Hex_HuntressLullaby";
	inline static const std::string 厄咒_猎杀戾气 = "Hex_Thrill_Of_The_Hunt";
	inline static const std::string 厄咒_万物 = "K25P02";
	inline static const std::string 厄咒_直面黑暗 = "K30P02";
	inline static const std::string 厄咒_第三封印 = "Hex_The_Third_Seal";
	inline static const std::string 厄咒_群体控制 = "K23P02";
	inline static const std::string 厄咒_闹鬼之地 = "Hex_HauntedGround";
	inline static const std::string 厄咒_难逃一死 = "No_One_Escapes_Death";
	inline static const std::string 厄咒_鲜血恩惠 = "HexBloodFavor";
	inline static const std::string 厄咒_压迫性气场 = "OverwhelmingPresence";
	inline static const std::string 友谊天长地久 = "K34P02";
	inline static const std::string 哐哐作响 = "K31P02";
	inline static const std::string 喘鸣感知 = "Stridor";
	inline static const std::string 嗜血凶兽 = "BeastOfPrey";
	inline static const std::string 囤积者 = "K22P01";
	inline static const std::string 基因锁 = "K32P01";
	inline static const std::string 大发雷霆 = "K38P01";
	inline static const std::string 天灾钩子_刽子手的把戏 = "HangmansTrick";
	inline static const std::string 天灾钩子_狂怒之潮 = "K27P01";
	inline static const std::string 天灾钩子_痛苦共鸣 = "K26P02";
	inline static const std::string 天灾钩子_痛苦礼物 = "K25P03";
	inline static const std::string 天灾钩子_锯齿罗盘 = "K38P02";
	inline static const std::string 天灾钩子_鬼魔神龛 = "Monstrous_Shrine";
	inline static const std::string 失效开关 = "DeadMansSwitch";
	inline static const std::string 好戏开场 = "K31P01";
	inline static const std::string 富兰克林之死 = "FranklinsLoss";
	inline static const std::string 强制忏悔 = "ForcedPenance";
	inline static const std::string 工匠 = "Tinkerer";
	inline static const std::string 巨龙之握 = "DragonsGrip";
	inline static const std::string 异形本能 = "K33P03";
	inline static const std::string 心暗气傲 = "K36P03";
	inline static const std::string 心惊肉跳 = "pop_goes_the_weasel";
	inline static const std::string 怒火中烧 = "FireUp";
	inline static const std::string 怨气冲天 = "Rancor";
	inline static const std::string 恐惧传染 = "InfectiousFright";
	inline static const std::string 恐慌降临 = "Unnerving_Presence";
	inline static const std::string 恐惧释放 = "Distressing";
	inline static const std::string 恩赐解脱 = "K22P03";
	inline static const std::string 惊险战栗 = "ThrillingTremors";
	inline static const std::string 感染之触手 = "K28P03";
	inline static const std::string 感知觉醒 = "K29P02";
	inline static const std::string 不要玩弄你的食物 = "Play_With_Your_Food";
	inline static const std::string 所向无敌 = "Brutal_Strength";
	inline static const std::string 把最好的留在最后 = "Save_The_Best_For_Last";
	inline static const std::string 折磨路径 = "TrailofTorment";
	inline static const std::string 护士的呼唤 = "NurseCalling";
	inline static const std::string 报应 = "Nemesis";
	inline static const std::string 支配之力 = "K37P03";
	inline static const std::string 无一幸免 = "K39P03";
	inline static const std::string 无处可藏 = "K30P01";
	inline static const std::string 无情风暴 = "K27P03";
	inline static const std::string 无效 = "K35P03";
	inline static const std::string 无束 = "K35P01";
	inline static const std::string 无路可逃 = "K23P03";
	inline static const std::string 无迹 = "K35P02";
	inline static const std::string 暗夜之晴 = "Shadowborn";
	inline static const std::string 暴怒怨灵 = "SpiritFury";
	inline static const std::string 机器学习 = "K32P03";
	inline static const std::string 杠杆效应 = "K31P03";
	inline static const std::string 欣喜若狂 = "Agitation";
	inline static const std::string 歇斯底里 = "K24P02";
	inline static const std::string 死亡恐惧 = "Thanatophobia";
	inline static const std::string 死亡羁绊 = "Deathbound";
	inline static const std::string 残心 = "ZanshinTactics";
	inline static const std::string 残酷禁制 = "CruelConfinement";
	inline static const std::string 毫不留情 = "K38P03";
	inline static const std::string 永恒交织 = "K39P02";
	inline static const std::string 洗耳恭听 = "ImAllEars";
	inline static const std::string 湮灭之光 = "Dying_Light";
	inline static const std::string 烟消云散 = "K28P01";
	inline static const std::string 爆发 = "K24P03";
	inline static const std::string 牢牢紧握 = "Iron_Grasp";
	inline static const std::string 猎鹿者 = "Deerstalker";
	inline static const std::string 瓦解意志 = "Mindbreaker";
	inline static const std::string 电涌 = "Surge";
	inline static const std::string 电量超载 = "GeneratorOvercharge";
	inline static const std::string 疯狂勇气 = "MadGrit";
	inline static const std::string 疾速残暴 = "K33P02";
	inline static const std::string 穷追不舍 = "Predator";
	inline static const std::string 粉碎希望 = "BoonDestroyer";
	inline static const std::string 终局战术 = "K29P03";
	inline static const std::string 盐水召唤 = "K27P02";
	inline static const std::string 编织共鸣 = "K36P01";
	inline static const std::string 耐力持久 = "Enduring";
	inline static const std::string 腐烂干预 = "CorruptIntervention";
	inline static const std::string 终极武器 = "K33P01";
	inline static const std::string 花言巧语 = "Bamboozle";
	inline static const std::string 苦涩低语 = "Bitter_Murmur";
	inline static const std::string 血气追踪 = "Bloodhound";
	inline static const std::string 致命追踪者 = "K24P01";
	inline static const std::string 行监坐守 = "MonitorAndAbuse";
	inline static const std::string 记忆犹新 = "RememberMe";
	inline static const std::string 设备发烧友 = "Gearhead";
	inline static const std::string 血腥狱长 = "BloodWarden";
	inline static const std::string 超人类体能 = "K29P01";
	inline static const std::string 踯躅不前 = "K32P02";
	inline static const std::string 辣手屠夫 = "Sloppy_Butcher";
	inline static const std::string 追星狂 = "K23P01";
	inline static const std::string 配合失谐 = "Discordance";
	inline static const std::string 配套电池 = "K34P03";
	inline static const std::string 铁处女 = "Ironmaiden";
	inline static const std::string 禁止隐身 = "Insidious";
	inline static const std::string 领地意识 = "TerritorialImperative";
	inline static const std::string 骄傲自大 = "K30P03";
	inline static const std::string 鬼祟追杀 = "FurtiveChase";
	inline static const std::string 鬼鸦谍影 = "Spies_From_The_Shadows";
	inline static const std::string 鲜血回响 = "BloodEcho";
	inline static const std::string 黑暗启迪 = "K28P02";
	inline static const std::string 黑暗奉献 = "DarkDevotion";

	inline static const std::string 招聘启事 = "K40P01";
	inline static const std::string 幻影恐惧 = "K40P02";
	inline static const std::string 设备故障 = "K40P03";

};



// 技能常量与本地化名称映射
static const std::unordered_map<std::string, std::wstring> PeiZhiPerkNameMap = {
	{ 杀手配置技能::小丑恐惧症, L"小丑恐惧症" },
	{ 杀手配置技能::压迫性气场, L"压迫性气场" },
	{ 杀手配置技能::恶灵低语, L"恶灵低语" },
	{ 杀手配置技能::死锁, L"死锁" },
	{ 杀手配置技能::不屈不挠, L"不屈不挠" },
	{ 杀手配置技能::了如指掌, L"了如指掌" },
	{ 杀手配置技能::人性贪婪, L"人性贪婪" },
	{ 杀手配置技能::人肉烧烤, L"人肉烧烤" },
	{ 杀手配置技能::做出你的选择, L"做出你的选择" },
	{ 杀手配置技能::光明之眼, L"光明之眼" },
	{ 杀手配置技能::冷酷之拥, L"冷酷之拥" },
	{ 杀手配置技能::倦怠之触, L"倦怠之触" },
	{ 杀手配置技能::击倒出局, L"击倒出局" },
	{ 杀手配置技能::切肤之痛, L"切肤之痛" },

	// 厄咒类技能
	{ 杀手配置技能::厄咒_不死, L"厄咒_不死" },
	{ 杀手配置技能::厄咒_以牙还牙, L"厄咒_以牙还牙" },
	{ 杀手配置技能::厄咒_众生皆苦, L"厄咒_众生皆苦" },
	{ 杀手配置技能::厄咒_余痕, L"厄咒_余痕" },
	{ 杀手配置技能::厄咒_吞噬希望, L"厄咒_吞噬希望" },
	{ 杀手配置技能::厄咒_悲惨命运, L"厄咒_悲惨命运" },
	{ 杀手配置技能::厄咒_惩戒, L"厄咒_惩戒" },
	{ 杀手配置技能::厄咒_毁灭, L"厄咒_毁灭" },
	{ 杀手配置技能::厄咒_摇篮曲, L"厄咒_摇篮曲" },
	{ 杀手配置技能::厄咒_猎杀戾气, L"厄咒_猎杀戾气" },
	{ 杀手配置技能::厄咒_万物, L"厄咒_万物" },
	{ 杀手配置技能::厄咒_直面黑暗, L"厄咒_直面黑暗" },
	{ 杀手配置技能::厄咒_第三封印, L"厄咒_第三封印" },
	{ 杀手配置技能::厄咒_群体控制, L"厄咒_群体控制" },
	{ 杀手配置技能::厄咒_闹鬼之地, L"厄咒_闹鬼之地" },
	{ 杀手配置技能::厄咒_难逃一死, L"厄咒_难逃一死" },
	{ 杀手配置技能::厄咒_鲜血恩惠, L"厄咒_鲜血恩惠" },

	// 天灾钩子类技能
	{ 杀手配置技能::天灾钩子_刽子手的把戏, L"天灾钩子_刽子手的把戏" },
	{ 杀手配置技能::天灾钩子_狂怒之潮, L"天灾钩子_狂怒之潮" },
	{ 杀手配置技能::天灾钩子_痛苦共鸣, L"天灾钩子_痛苦共鸣" },
	{ 杀手配置技能::天灾钩子_痛苦礼物, L"天灾钩子_痛苦礼物" },
	{ 杀手配置技能::天灾钩子_锯齿罗盘, L"天灾钩子_锯齿罗盘" },
	{ 杀手配置技能::天灾钩子_鬼魔神龛, L"天灾钩子_鬼魔神龛" },

	// 其他技能
	{ 杀手配置技能::友谊天长地久, L"友谊天长地久" },
	{ 杀手配置技能::哐哐作响, L"哐哐作响" },
	{ 杀手配置技能::喘鸣感知, L"喘鸣感知" },
	{ 杀手配置技能::嗜血凶兽, L"嗜血凶兽" },
	{ 杀手配置技能::囤积者, L"囤积者" },
	{ 杀手配置技能::基因锁, L"基因锁" },
	{ 杀手配置技能::大发雷霆, L"大发雷霆" },
	{ 杀手配置技能::失效开关, L"失效开关" },
	{ 杀手配置技能::好戏开场, L"好戏开场" },
	{ 杀手配置技能::富兰克林之死, L"富兰克林之死" },
	{ 杀手配置技能::强制忏悔, L"强制忏悔" },
	{ 杀手配置技能::工匠, L"工匠" },
	{ 杀手配置技能::巨龙之握, L"巨龙之握" },
	{ 杀手配置技能::异形本能, L"异形本能" },
	{ 杀手配置技能::心暗气傲, L"心暗气傲" },
	{ 杀手配置技能::心惊肉跳, L"心惊肉跳" },
	{ 杀手配置技能::怒火中烧, L"怒火中烧" },
	{ 杀手配置技能::怨气冲天, L"怨气冲天" },
	{ 杀手配置技能::恐惧传染, L"恐惧传染" },
	{ 杀手配置技能::恐慌降临, L"恐慌降临" },
	{ 杀手配置技能::恐惧释放, L"恐惧释放" },
	{ 杀手配置技能::恩赐解脱, L"恩赐解脱" },
	{ 杀手配置技能::惊险战栗, L"惊险战栗" },
	{ 杀手配置技能::感染之触手, L"感染之触手" },
	{ 杀手配置技能::感知觉醒, L"感知觉醒" },
	{ 杀手配置技能::不要玩弄你的食物, L"不要玩弄你的食物" },
	{ 杀手配置技能::所向无敌, L"所向无敌" },
	{ 杀手配置技能::把最好的留在最后, L"把最好的留在最后" },
	{ 杀手配置技能::折磨路径, L"折磨路径" },
	{ 杀手配置技能::护士的呼唤, L"护士的呼唤" },
	{ 杀手配置技能::报应, L"报应" },
	{ 杀手配置技能::支配之力, L"支配之力" },
	{ 杀手配置技能::无一幸免, L"无一幸免" },
	{ 杀手配置技能::无处可藏, L"无处可藏" },
	{ 杀手配置技能::无情风暴, L"无情风暴" },
	{ 杀手配置技能::无效, L"无效" },
	{ 杀手配置技能::无束, L"无束" },
	{ 杀手配置技能::无路可逃, L"无路可逃" },
	{ 杀手配置技能::无迹, L"无迹" },
	{ 杀手配置技能::暗夜之晴, L"暗夜之晴" },
	{ 杀手配置技能::暴怒怨灵, L"暴怒怨灵" },
	{ 杀手配置技能::机器学习, L"机器学习" },
	{ 杀手配置技能::杠杆效应, L"杠杆效应" },
	{ 杀手配置技能::欣喜若狂, L"欣喜若狂" },
	{ 杀手配置技能::歇斯底里, L"歇斯底里" },
	{ 杀手配置技能::死亡恐惧, L"死亡恐惧" },
	{ 杀手配置技能::死亡羁绊, L"死亡羁绊" },
	{ 杀手配置技能::残心, L"残心" },
	{ 杀手配置技能::残酷禁制, L"残酷禁制" },
	{ 杀手配置技能::毫不留情, L"毫不留情" },
	{ 杀手配置技能::永恒交织, L"永恒交织" },
	{ 杀手配置技能::洗耳恭听, L"洗耳恭听" },
	{ 杀手配置技能::湮灭之光, L"湮灭之光" },
	{ 杀手配置技能::烟消云散, L"烟消云散" },
	{ 杀手配置技能::爆发, L"爆发" },
	{ 杀手配置技能::牢牢紧握, L"牢牢紧握" },
	{ 杀手配置技能::猎鹿者, L"猎鹿者" },
	{ 杀手配置技能::瓦解意志, L"瓦解意志" },
	{ 杀手配置技能::电涌, L"电涌" },
	{ 杀手配置技能::电量超载, L"电量超载" },
	{ 杀手配置技能::疯狂勇气, L"疯狂勇气" },
	{ 杀手配置技能::疾速残暴, L"疾速残暴" },
	{ 杀手配置技能::穷追不舍, L"穷追不舍" },
	{ 杀手配置技能::粉碎希望, L"粉碎希望" },
	{ 杀手配置技能::终局战术, L"终局战术" },
	{ 杀手配置技能::盐水召唤, L"盐水召唤" },
	{ 杀手配置技能::编织共鸣, L"编织共鸣" },
	{ 杀手配置技能::耐力持久, L"耐力持久" },
	{ 杀手配置技能::腐烂干预, L"腐烂干预" },
	{ 杀手配置技能::终极武器, L"终极武器" },
	{ 杀手配置技能::花言巧语, L"花言巧语" },
	{ 杀手配置技能::苦涩低语, L"苦涩低语" },
	{ 杀手配置技能::血气追踪, L"血气追踪" },
	{ 杀手配置技能::致命追踪者, L"致命追踪者" },
	{ 杀手配置技能::行监坐守, L"行监坐守" },
	{ 杀手配置技能::记忆犹新, L"记忆犹新" },
	{ 杀手配置技能::设备发烧友, L"设备发烧友" },
	{ 杀手配置技能::血腥狱长, L"血腥狱长" },
	{ 杀手配置技能::超人类体能, L"超人类体能" },
	{ 杀手配置技能::踯躅不前, L"踯躅不前" },
	{ 杀手配置技能::辣手屠夫, L"辣手屠夫" },
	{ 杀手配置技能::追星狂, L"追星狂" },
	{ 杀手配置技能::配合失谐, L"配合失谐" },
	{ 杀手配置技能::配套电池, L"配套电池" },
	{ 杀手配置技能::铁处女, L"铁处女" },
	{ 杀手配置技能::禁止隐身, L"禁止隐身" },
	{ 杀手配置技能::领地意识, L"领地意识" },
	{ 杀手配置技能::骄傲自大, L"骄傲自大" },
	{ 杀手配置技能::鬼祟追杀, L"鬼祟追杀" },
	{ 杀手配置技能::鬼鸦谍影, L"鬼鸦谍影" },
	{ 杀手配置技能::鲜血回响, L"鲜血回响" },
	{ 杀手配置技能::黑暗启迪, L"黑暗启迪" },
	{ 杀手配置技能::黑暗奉献, L"黑暗奉献" },
	{ 杀手配置技能::招聘启事, L"招聘启事" },
	{ 杀手配置技能::幻影恐惧, L"幻影恐惧" },
	{ 杀手配置技能::设备故障, L"设备故障" },


	{ 幸存者配置技能::不惜一切1, L"不惜一切" },
	{ 幸存者配置技能::不惜一切2, L"不惜一切" },
	{ 幸存者配置技能::不痛不痒, L"不痛不痒" },
	{ 幸存者配置技能::不祥预感, L"不祥预感" },
	{ 幸存者配置技能::与我共舞, L"与我共舞" },
	{ 幸存者配置技能::东山再起, L"东山再起" },
	{ 幸存者配置技能::为生而生, L"为生而生" },
	{ 幸存者配置技能::亡父警言, L"亡父警言" },
	{ 幸存者配置技能::交稿日, L"交稿日" },
	{ 幸存者配置技能::人之勇气, L"人之勇气" },
	{ 幸存者配置技能::似曾相识, L"似曾相识" },
	{ 幸存者配置技能::低调行事, L"低调行事" },
	{ 幸存者配置技能::余光长存, L"余光长存" },
	{ 幸存者配置技能::保姆, L"保姆" },
	{ 幸存者配置技能::光明勇士, L"光明勇士" },
	{ 幸存者配置技能::克服难关, L"克服难关" },
	{ 幸存者配置技能::兵不厌诈, L"兵不厌诈" },
	{ 幸存者配置技能::内在专注, L"内在专注" },
	{ 幸存者配置技能::内心之力, L"内心之力" },
	{ 幸存者配置技能::冲刺爆发, L"冲刺爆发" },
	{ 幸存者配置技能::分崩离析, L"分崩离析" },
	{ 幸存者配置技能::切勿伤害, L"切勿伤害" },
	{ 幸存者配置技能::劫后余生, L"劫后余生" },
	{ 幸存者配置技能::勇往直前, L"勇往直前" },
	{ 幸存者配置技能::化学陷阱, L"化学陷阱" },
	{ 幸存者配置技能::千里眼, L"千里眼" },
	{ 幸存者配置技能::即兴表演, L"即兴表演" },
	{ 幸存者配置技能::压抑同盟, L"压抑同盟" },
	{ 幸存者配置技能::友谊赛, L"友谊赛" },
	{ 幸存者配置技能::反制之力, L"反制之力" },
	{ 幸存者配置技能::各自为安, L"各自为安" },
	{ 幸存者配置技能::同志情谊, L"同志情谊" },
	{ 幸存者配置技能::同族, L"同族" },
	{ 幸存者配置技能::同生共死, L"同生共死" },
	{ 幸存者配置技能::吟游诗人激励, L"吟游诗人激励" },
	{ 幸存者配置技能::咬紧牙关, L"咬紧牙关" },
	{ 幸存者配置技能::善有善报, L"善有善报" },
	{ 幸存者配置技能::团结一致, L"团结一致" },
	{ 幸存者配置技能::团队合作_二人之力, L"团队合作_二人之力" },
	{ 幸存者配置技能::团队合作_全体隐蔽, L"团队合作_全体隐蔽" },
	{ 幸存者配置技能::坚不可摧, L"坚不可摧" },
	{ 幸存者配置技能::声东击西, L"声东击西" },
	{ 幸存者配置技能::复苏之风, L"复苏之风" },
	{ 幸存者配置技能::好运会传染, L"好运会传染" },
	{ 幸存者配置技能::守夜人, L"守夜人" },
	{ 幸存者配置技能::安抚生灵, L"安抚生灵" },
	{ 幸存者配置技能::定心丸, L"定心丸" },
	{ 幸存者配置技能::对策治疗, L"对策治疗" },
	{ 幸存者配置技能::希望之翼, L"希望之翼" },
	{ 幸存者配置技能::幕后玩家, L"幕后玩家" },
	{ 幸存者配置技能::平稳着陆, L"平稳着陆" },
	{ 幸存者配置技能::并行进展, L"并行进展" },
	{ 幸存者配置技能::幸运喘息, L"幸运喘息" },
	{ 幸存者配置技能::幸运星, L"幸运星" },
	{ 幸存者配置技能::强力挣扎, L"强力挣扎" },
	{ 幸存者配置技能::强盗直觉, L"强盗直觉" },
	{ 幸存者配置技能::当机立断, L"当机立断" },
	{ 幸存者配置技能::心灵共鸣, L"心灵共鸣" },
	{ 幸存者配置技能::心静眼明, L"心静眼明" },
	{ 幸存者配置技能::快速反应, L"快速反应" },
	{ 幸存者配置技能::快速策略, L"快速策略" },
	{ 幸存者配置技能::急于求成, L"急于求成" },
	{ 幸存者配置技能::急速静谧, L"急速静谧" },
	{ 幸存者配置技能::恩赐_指数增长, L"恩赐_指数增长" },
	{ 幸存者配置技能::恩赐_明亮, L"恩赐_明亮" },
	{ 幸存者配置技能::恩赐_暗影步, L"恩赐_暗影步" },
	{ 幸存者配置技能::恩赐_治疗之环, L"恩赐_治疗之环" },
	{ 幸存者配置技能::恩赐_黑暗理论, L"恩赐_黑暗理论" },
	{ 幸存者配置技能::感同身受, L"感同身受" },
	{ 幸存者配置技能::成功捷径, L"成功捷径" },
	{ 幸存者配置技能::技术员, L"技术员" },
	{ 幸存者配置技能::挣脱激励, L"挣脱激励" },
	{ 幸存者配置技能::携手合作, L"携手合作" },
	{ 幸存者配置技能::新人精神, L"新人精神" },
	{ 幸存者配置技能::时不我待, L"时不我待" },
	{ 幸存者配置技能::明察秋毫, L"明察秋毫" },
	{ 幸存者配置技能::暗黑感知, L"暗黑感知" },
	{ 幸存者配置技能::暴走族, L"暴走族" },
	{ 幸存者配置技能::最后的生还者, L"最后的生还者" },
	{ 幸存者配置技能::未雨绸缪, L"未雨绸缪" },
	{ 幸存者配置技能::末日残兵, L"末日残兵" },
	{ 幸存者配置技能::机遇之窗, L"机遇之窗" },
	{ 幸存者配置技能::果断反击, L"果断反击" },
	{ 幸存者配置技能::毛骨悚然, L"毛骨悚然" },
	{ 幸存者配置技能::气场失真, L"气场失真" },
	{ 幸存者配置技能::永不言弃, L"永不言弃" },
	{ 幸存者配置技能::求生欲, L"求生欲" },
	{ 幸存者配置技能::浴血冲刺, L"浴血冲刺" },
	{ 幸存者配置技能::清道夫, L"清道夫" },
	{ 幸存者配置技能::潜力无限, L"潜力无限" },
	{ 幸存者配置技能::灵魂守卫, L"灵魂守卫" },
	{ 幸存者配置技能::焕然一新, L"焕然一新" },
	{ 幸存者配置技能::爆炸地雷, L"爆炸地雷" },
	{ 幸存者配置技能::狩猎经验, L"狩猎经验" },
	{ 幸存者配置技能::生死与共, L"生死与共" },
	{ 幸存者配置技能::百折不挠, L"百折不挠" },
	{ 幸存者配置技能::矫健身手, L"矫健身手" },
	{ 幸存者配置技能::矫正措施, L"矫正措施" },
	{ 幸存者配置技能::破坏手, L"破坏手" },
	{ 幸存者配置技能::破案心切, L"破案心切" },
	{ 幸存者配置技能::祈祷_织网蜘蛛, L"祈祷_织网蜘蛛" },
	{ 幸存者配置技能::祈祷_险恶乌鸦, L"祈祷_险恶乌鸦" },
	{ 幸存者配置技能::稳操胜券, L"稳操胜券" },
	{ 幸存者配置技能::窃听器, L"窃听器" },
	{ 幸存者配置技能::答疑解惑, L"答疑解惑" },
	{ 幸存者配置技能::精研百草, L"精研百草" },
	{ 幸存者配置技能::经久耐用, L"经久耐用" },
	{ 幸存者配置技能::绝境对策, L"绝境对策" },
	{ 幸存者配置技能::老本行, L"老本行" },
	{ 幸存者配置技能::联袂主演, L"联袂主演" },
	{ 幸存者配置技能::聚精会神, L"聚精会神" },
	{ 幸存者配置技能::肩负重担, L"肩负重担" },
	{ 幸存者配置技能::肾上腺素, L"肾上腺素" },
	{ 幸存者配置技能::胜利喜悦, L"胜利喜悦" },
	{ 幸存者配置技能::脱缰野马, L"脱缰野马" },
	{ 幸存者配置技能::自学成才, L"自学成才" },
	{ 幸存者配置技能::自我保护, L"自我保护" },
	{ 幸存者配置技能::自我疗愈, L"自我疗愈" },
	{ 幸存者配置技能::舍己为人, L"舍己为人" },
	{ 幸存者配置技能::荣耀时刻, L"荣耀时刻" },
	{ 幸存者配置技能::药到病除, L"药到病除" },
	{ 幸存者配置技能::蓄势待发, L"蓄势待发" },
	{ 幸存者配置技能::警戒, L"警戒" },
	{ 幸存者配置技能::警探直觉, L"警探直觉" },
	{ 幸存者配置技能::认定目标, L"认定目标" },
	{ 幸存者配置技能::证明自己, L"证明自己" },
	{ 幸存者配置技能::谨慎责任, L"谨慎责任" },
	{ 幸存者配置技能::贝尔蒙特之眼, L"贝尔蒙特之眼" },
	{ 幸存者配置技能::跌宕剧情, L"跌宕剧情" },
	{ 幸存者配置技能::转移注意, L"转移注意" },
	{ 幸存者配置技能::轰动演出, L"轰动演出" },
	{ 幸存者配置技能::轻如鸿毛, L"轻如鸿毛" },
	{ 幸存者配置技能::轻盈敏捷, L"轻盈敏捷" },
	{ 幸存者配置技能::轻盈猫步, L"轻盈猫步" },
	{ 幸存者配置技能::远见卓识, L"远见卓识" },
	{ 幸存者配置技能::迷雾慧眼, L"迷雾慧眼" },
	{ 幸存者配置技能::逃之夭夭, L"逃之夭夭" },
	{ 幸存者配置技能::邪祟之力, L"邪祟之力" },
	{ 幸存者配置技能::都市生存, L"都市生存" },
	{ 幸存者配置技能::醒醒！！！, L"醒醒！！！" },
	{ 幸存者配置技能::钢筋铁骨, L"钢筋铁骨" },
	{ 幸存者配置技能::钢铁意志, L"钢铁意志" },
	{ 幸存者配置技能::锦囊妙计, L"锦囊妙计" },
	{ 幸存者配置技能::镜影幻象, L"镜影幻象" },
	{ 幸存者配置技能::闪光弹, L"闪光弹" },
	{ 幸存者配置技能::隐匿之势, L"隐匿之势" },
	{ 幸存者配置技能::难以置信, L"难以置信" },
	{ 幸存者配置技能::非正式调查, L"非正式调查" },
	{ 幸存者配置技能::领袖群雄, L"领袖群雄" },
	{ 幸存者配置技能::马到成功, L"马到成功" },
	{ 幸存者配置技能::魂牵梦绕, L"魂牵梦绕" },
	{ 幸存者配置技能::鲜血契约, L"鲜血契约" },
	{ 幸存者配置技能::齐心协力, L"齐心协力" }
};


