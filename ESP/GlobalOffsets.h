#pragma once

#include <cstdint>
#include"Vector.h"
#include <unordered_map>
#include <ostream>
#include <imgui.h>

// �������ͱ���
typedef unsigned char uint8_t;
typedef uint8_t      uint8;


extern int screenWidth;
extern int screenHeight;



// Game.exe ȫ�ֱ���: GWorld ��ƫ��
static constexpr int32_t OFFSET_GWorld = 0xAB956E0;
// Game.exe ȫ�ֱ���: GName �������ƫ��
static constexpr int32_t OFFSET_GNames = 0xA93D140;
// Game.exe ȫ�ֱ���: GUObject �б��ƫ��
static constexpr int32_t OFFSET_GUObjects = 0xa9ff990;


//���ж���λ�û�ȡ

// UWorld::Ulevel_PersistentLevel ��Աƫ��
static constexpr int32_t OFFSET_UWorld_ULevel_PersistentLevel = 0x0038;
// ULevel::TArray<AActor*>_actor
static constexpr int32_t OFFSET_ULevel_Actors = 0x00A0;
// AActor::RootComponent ��Աƫ��
static constexpr int32_t OFFSET_AActor_USceneComponent_RootComponent = 0x01A8;

// AActor::USceneComponent::RelativeLocation ��Աƫ��
static constexpr int32_t OFFSET_USceneComponent_RelativeLocation = 0x0150;



//������������ȡ


// UWorld::OwningGameInstance ��Աƫ��
static constexpr int32_t OFFSET_GWorld_OwningGameInstance = 0x01C8;
// UGameInstance::LocalPlayers ��Աƫ�� (TArray<UPlayer*>)
static constexpr int32_t OFFSET_OwningGameInstance_LocalPlayers = 0x0040;
// ULocalPlayer::PlayerController ��Աƫ��
static constexpr int32_t OFFSET_LocalPlayer_PlayerController = 0x0038;

// APlayerController::AcknowledgedPawn ��Աƫ��
static constexpr int32_t OFFSET_PlayerController_AcknowledgedPawn = 0x0350;
// APlayerController::CameraManager ��Աƫ��
static constexpr int32_t OFFSET_PlayerController_CameraManager = 0x0360;
// APlayerCameraManager::CameraCachePrivate ��Աƫ��
static constexpr int32_t OFFSET_CameraManager_CameraCachePrivate = 0x22D0;


//������һ�ȡ

// UWorld::GameState ��Աƫ�� (AGameStateBase*)
static constexpr int32_t OFFSET_GWorld_GameState = 0x0168;

// AGameStateBase::PlayerArray ��Աƫ�� (TArray<APlayerState*>)
static constexpr int32_t OFFSET_GameState_PlayerState = 0x02B8;

// APlayerState::PlayerNamePrivate ��Աƫ�� (FString)
static constexpr int32_t OFFSET_PlayerState_PlayerNamePrivate = 0x03A0;

// APlayerState::PawnPrivate ��Աƫ�� (AActor*)
static constexpr int32_t OFFSET_PlayerState_PawnPrivate = 0x0320;

// DBDPlayerState::CamperData ��Աƫ�� (Ƕ��ʽ FCharacterStateData)
static constexpr int32_t OFFSET_PlayerState_FCharacterStateData_CamperData = 0x0410;

// DBDPlayerState::SlasherData ��Աƫ�� (Ƕ��ʽ FCharacterStateData)
static constexpr int32_t OFFSET_PlayerState_FCharacterStateData_SlasherData = 0x0430;

// DBDPlayerState::PlayerData ��Աƫ�� (Ƕ��ʽ FPlayerStateData)
static constexpr int32_t OFFSET_PlayerState_FPlayerStateData_PlayerData = 0x0450;

//ͼ��״̬

// ATotem�̳�Aactor
static constexpr int32_t OFFSET_ATotem_ETotemState = 0x03F8;
// AGenerator�̳�Aactor
static constexpr int32_t OFFSET_AGenerator_UChargeableComponent = 0x0620;
static constexpr int32_t OFFSET_AGenerator_UChargeableComponent_FSpeedBasedNetSyncedValue = 0x0198;
static constexpr int32_t OFFSET_AGenerator_UChargeableComponent_FSpeedBasedNetSyncedValue_replicatedValue = 0x0018;


struct Perks {


	uint64_t PerkNameData;// ����������������ָ��
	int32_t perkNameIdx;// ������������
	std::string perkName; // ������������

};

struct  Player {
public:
	uint64_t PlayerData;//���ָ��
	std::string name;//�������
	uint64_t  PerkStruct;// Perk�ṹ��
	uint64_t EquipedPerkIds; // װ���ı�������ID����ָ��
	int32_t  EquipedPerkIdsNum; // װ���ı�������ID����

	uint64_t Character;// ��������ָ��
	uint32_t powerId;// ��������������
	std::string powerName; // ������������

	ImU32 color;//����ɱ�ֺ��Ҵ�����ɫ

	std::vector<Perks> perks; // ��������ID����

	uint64_t	PawnPrivate;
	uint64_t	rootComponent;//�����

};

class Engine {

public:

	//�������ж���
	uint64_t pLevel;
	uint64_t actors;
	uint32_t Num;

	//����������
	uint64_t GameInstance;
	uint64_t LocalPlayer;
	uint64_t PlayerController;
	uint64_t AcknowledgedPawn;
	uint64_t PlayerCameraManager;

	//���������������
	uint64_t GameState;
	uint64_t baseArray;
	uint32_t PlayerCount;
	uint32_t PlayerCountMax;


	std::vector<uint64_t> TotemsCache;	// ͼ��
	std::vector<uint64_t> GeneratorCache;	// �����
	std::vector<uint64_t>  Pallets;
	std::vector<uint64_t>  ����;
	uint64_t HatchCache;//�ؽ�
	uint64_t ConjoinedTwin;//����Ӥ��ɫ�Ķ���

	std::vector<Player> player; // �������

};



// ��ʾ��ά�ռ��е�λ�û���
struct FVector {
public:
	double X; // 0x0000: X ������
	double Y; // 0x0008: Y ������
	double Z; // 0x0010: Z ������
};

// ��ʾ��ά�ռ��е���ת�Ƕ�
struct FRotator {
public:
	double Pitch; // 0x0000: �� X �ḩ����
	double Yaw;   // 0x0008: �� Y ��ƫ����
	double Roll;  // 0x0010: �� Z �ᷭ����
};

// �洢�������С��ͼ��Ϣ
struct FMinimalViewInfo {
public:
	FVector Location;   // 0x0000: �������λ��
	FRotator Rotation;  // 0x0018: �����ת
	float    FOV;       // 0x0030: ��Ұ��Χ
	uint8    Padding[4]; // 0x0034: �������
};

// ���ڻ������ĳһʱ�̵���ͼ״̬
struct FCameraCacheEntry {
public:
	float            Timestamp; // 0x0000: ʱ���
	uint8            Pad[12];   // 0x0004: ������
	FMinimalViewInfo POV;       // 0x0010: �����ͼ��Ϣ
};



// Ԥ���㳣��
static constexpr float DEG_TO_RAD = 0.01745329251994329577f; // ��/180
static constexpr float FOV_DEG_TO_RAD = 0.00872664625997164788f; // ��/360
inline __forceinline Vector2 WorldToScreen(const FMinimalViewInfo& viewinfo, const Vector3& world)
{
	// ��ȡ��ת�ǶȲ�ת��Ϊ����
	const float radPitch = viewinfo.Rotation.Pitch * DEG_TO_RAD;
	const float radYaw = viewinfo.Rotation.Yaw * DEG_TO_RAD;
	const float radRoll = viewinfo.Rotation.Roll * DEG_TO_RAD;

	// һ���Լ����������Ǻ���
	const float SP = sinf(radPitch);
	const float CP = cosf(radPitch);
	const float SY = sinf(radYaw);
	const float CY = cosf(radYaw);
	const float SR = sinf(radRoll);
	const float CR = cosf(radRoll);

	// Ԥ���㳣�ó˻�
	const float SRSP = SR * SP;
	const float CRSP = CR * SP;

	// ֱ�Ӽ�����ת����������ⴴ��Vector3����
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

	// �����������������λ�õĲ�ֵ
	const float deltaX = world.x - viewinfo.Location.X;
	const float deltaY = world.y - viewinfo.Location.Y;
	const float deltaZ = world.z - viewinfo.Location.Z;

	// �ֶ���������������任
	const float transformedX = deltaX * axisY_x + deltaY * axisY_y + deltaZ * axisY_z;
	const float transformedY = deltaX * axisZ_x + deltaY * axisZ_y + deltaZ * axisZ_z;
	const float transformedZ = deltaX * axisX_x + deltaY * axisX_y + deltaZ * axisX_z;

	// �������͸�Zֵ
	const float z = (transformedZ < 1.0f) ? 1.0f : transformedZ;

	// Ԥ������Ļ���ĺ�FOV���ֵ
	const float centerX = screenWidth * 0.5f;
	const float centerY = screenHeight * 0.5f;
	const float fovScale = centerX / tanf(viewinfo.FOV * FOV_DEG_TO_RAD);
	const float invZ = fovScale / z;

	// ����������Ļ����
	const float screenX = centerX + transformedX * invZ;
	const float screenY = centerY - transformedY * invZ;

	return { screenX, screenY };
}


struct FNameEntryHeader final
{
public:
	uint16_t                                        bIsWide : 1;       // 0x0000(0x0002) �ַ��������־��0=ANSI(���ֽ�)��1=Wide(˫�ֽ�)
	uint16_t                                        Len : 15;          // 0x0000(0x0002) �ַ�������(0-32767)�����֧��32767���ַ�
};


union FStringData final
{
public:
	char                                          AnsiName[0x400];    // 0x0000(0x0800) ANSI�ַ����洢��(1024�ֽ�)
	wchar_t                                       WideName[0x400];    // 0x0000(0x0800) Wide�ַ����洢��(2048�ֽڣ����ַ�ռ2�ֽ�)
};


struct FNameEntry final
{
public:
	uint8                                         Pad_0[0x4];         // 0x0000(0x0004) �ڴ�������
	struct FNameEntryHeader                       Header;             // 0x0004(0x0002) �ַ���Ԫ����(�������ͺͳ���)
	union FStringData                             Name;               // 0x0006(0x0800) �ַ���ʵ�ʴ洢��(����Headerѡ��ANSI��Wide)
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
	int32_t ComparisonIndex;  //0x0 // ����������� DisplayIndex�����ֱ��±꣩+ ���ɱ�־λ
	int32_t Number;           //0x4 // ʵ����ţ���������ͬ����ͬʵ��
	int32_t DisplayIndex;     //0x8// ���������ֱ��±꣨�°����棩
};


//�洢��ɫս��״̬��������������������ܡ��������λ��
struct FCharacterStateData final
{
	int32_t                                         _pips;                  // ��ɫ�Ķ�λ/���֣�Ӱ��ƥ��ȼ��������ȣ�
	class FName                                   _powerId;                // ��ɫ��������/����ID��ɱ��ר������"Power_Teleport"���ͣ��Ҵ���ͨ����ʵ�����壩
	TArray<class FName>                           _addonIds;              // װ���ĸ�����ID���飨ǿ����������Ч����������ɱ��������ȴʱ�䣩

	// ---------------------------
	// �ڴ沼��˵���������Զ����ɣ���
	// 0x0000(0x0004)��_pips ռ4�ֽ�
	// 0x0004(0x000C)��_powerId ռ12�ֽڣ�FName�ڲ��ṹ��
	// 0x0010(0x0010)��_addonIds ����ָ�뼰Ԫ����ռ16�ֽ�
	// �ܴ�С��0x0020�ֽڣ�32�ֽڣ�
	// ---------------------------
};
//�洢����Զ��������뱻�����ܣ���װ���ı������ܡ��ȼ������£�
struct FPlayerStateData final
{
public:
	int32_t                                         CharacterLevel;            // ��ɫ�ȼ���Ӱ�챻�����ܽ�����ȼ����ޣ�
	class FName                                   EquipedFavorId;             // װ���ļ�ƷID��Ӱ����Ϸ��������ƣ���Ѫ���ȡЧ�ʣ�
	TArray<class FName>                           EquipedPerkIds;            // **�����ֶ�**��װ���ı�������ID���飨��"Perk_RepairSpeedBoost"������٣�
	TArray<int32_t>                                 EquipedPerkLevels;      // ��Ӧ�������ܵĵȼ���1-3������������Ч��ǿ�ȣ�
	//	class FString                                 EquippedBannerId;        // װ���ĺ��ID�����չʾ���뼼���޹أ�
	//	class FString                                 EquippedBadgeId;        // װ���Ļ���ID���ɾ�չʾ���뼼���޹أ�
	//	class FName                                   EquippedCharacterClass;// ��ɫְҵID����"Survivor_Leon"��"Killer_Michael"��������װ���ļ��ܳأ�
	//	EGameState                                    _playerGameState;     // �����Ϸ״̬�����/����/�����ȣ��뼼�ܴ���������أ�
	//	uint8                                         Pad_5D[0x3];         // �ڴ������䣨�����Զ����ɣ������ֶ�����
	//	int32_t                                         _prestigeLevel;     // �����ȼ���������ɫ������ۻ�������·����
	//	uint8                                         Pad_64[0x4];       // �ڴ������䣨ȷ���ṹ���ܴ�СΪ4�ֽڱ�����

		// ---------------------------
		// �ڴ沼��˵���������Զ����ɣ���
		// 0x0000(0x0004)��CharacterLevel ռ4�ֽ�
		// 0x0004(0x000C)��EquipedFavorId ռ12�ֽ�
		// 0x0010(0x0010)��EquipedPerkIds ����ռ16�ֽ�
		// 0x0020(0x0010)��EquipedPerkLevels ����ռ16�ֽ�
		// 0x0030(0x0010)��EquippedBannerId ռ16�ֽڣ�FString�ڲ��ṹ��
		// 0x0040(0x0010)��EquippedBadgeId ռ16�ֽ�
		// 0x0050(0x000C)��EquippedCharacterClass ռ12�ֽ�
		// 0x005C(0x0001)��_playerGameState ռ1�ֽ�
		// 0x005D(0x0003)��Pad_5D ���3�ֽڣ����뵽0x0060��
		// 0x0060(0x0004)��_prestigeLevel ռ4�ֽ�
		// 0x0064(0x0004)��Pad_64 ���4�ֽڣ��ܴ�С���뵽0x0068��
		// �ܴ�С��0x0068�ֽڣ�104�ֽڣ�
		// ---------------------------
};

//���ܼ춨
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

//ͼ����ɫ��״̬
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
	Cleansed = 0,  // ͼ���ѱ����������ڳ�ʼ/����״̬
	Dull = 1,      // ͼ��ʧȥ�����������޻���״̬
	Hex = 2,       // ͼ�ڱ����䣬ʩ�Ӹ���Ч��
	Boon = 3,      // ͼ�ڱ�ף�����ṩ��������
	ETotemState_MAX = 4,
};
// ľ��״̬ö�����ͣ����ڱ�ʾ��Ϸ��ľ��Ĳ�ͬ״̬
enum class EPalletState : uint8
{
	// ľ��ֱ��״̬��δ�����£�
	Up = 0,

	// ľ��������������У���ֱ�������µĶ����׶Σ�
	Falling = 1,

	// ľ���ѷ���״̬����ҿ������赲��Խ��
	Fallen = 2,

	// ľ���ѱ��ƻ�״̬���޷��ٽ�����
	Destroyed = 3,

	// ö��ֵ���ޱ�ǣ���������߽���ȳ�����
	EPalletState_MAX = 4,
};
//ľ����ײ��
class UBoxComponent
{
public:
	struct FVector                                BoxExtent;                                         // 0x05C0(0x0018)(Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	float                                         LineThickness;                                     // 0x05D8(0x0004)(Edit, BlueprintVisible, ExportObject, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
	uint8                                         Pad_5DC[0x4];                                      // 0x05DC(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
};


//���Ը���ɱ�ֵ��������������ֱܷ�ɱ�ֽ�ɫ
struct ɱ�� {
	inline static const std::string ���� = "BP_Slasher_Character_01_C";
	inline static const std::string С���� = "BP_Slasher_Character_02_C";
	inline static const std::string ȳȳ = "BP_Slasher_Character_03_C";
	inline static const std::string ��ʿ = "BP_Slasher_Character_04_C";
	inline static const std::string ���� = "BP_Slasher_Character_05_C";
	inline static const std::string ������ = "BP_Slasher_Character_06_C";
	inline static const std::string ���� = "BP_Slasher_Character_07_C";
	inline static const std::string ���� = "BP_Slasher_Character_08_C";
	inline static const std::string Ƥ�� = "BP_Slasher_Character_09_C";
	inline static const std::string ���� = "BP_Slasher_Character_10_C";
	inline static const std::string ���� = "BP_Slasher_Character_11_C";
	inline static const std::string С�� = "BP_Slasher_Character_12_C";
	inline static const std::string ���� = "BP_Slasher_Character_13_C";
	inline static const std::string ���� = "BP_Slasher_Character_14_C";
	inline static const std::string ���� = "BP_Slasher_Character_15_C";
	inline static const std::string ���� = "BP_Slasher_Character_16_C";
	inline static const std::string ħ�� = "BP_Slasher_Character_17_C";
	inline static const std::string ����ʿ = "BP_Slasher_Character_18_C";
	inline static const std::string ���� = "BP_Slasher_Character_19_C";
	inline static const std::string ����ͷ = "BP_Slasher_Character_20_C";
	inline static const std::string ��ħ = "BP_Slasher_Character_21_C";
	inline static const std::string ����Ӥ = "BP_Slasher_Character_22_C";
	inline static const std::string ��� = "BP_Slasher_Character_23_C";
	inline static const std::string ׷���� = "BP_Slasher_Character_24_C";
	inline static const std::string ����ͷ = "BP_Slasher_Character_25_C";
	inline static const std::string ��� = "BP_Slasher_Character_26_C";
	inline static const std::string ���� = "BP_Slasher_Character_27_C";
	inline static const std::string Ӱħ = "BP_Slasher_Character_28_C";
	inline static const std::string ��˹�� = "BP_Slasher_Character_29_C";
	inline static const std::string ����ʿ = "BP_Slasher_Character_30_C";
	inline static const std::string �׹����� = "BP_Slasher_Character_31_C";
	inline static const std::string ��� = "BP_Slasher_Character_32_C";
	inline static const std::string ���� = "BP_Slasher_Character_33_C";
	inline static const std::string �ú��� = "BP_Slasher_Character_34_C";
	inline static const std::string δ֪���� = "BP_Slasher_Character_35_C";
	inline static const std::string ���� = "BP_Slasher_Character_36_C";
	inline static const std::string �¹��� = "BP_Slasher_Character_37_C";
	inline static const std::string �˺� = "BP_Slasher_Character_38_C";
	inline static const std::string ��ľ�� = "BP_Slasher_Character_39_C";
	inline static const std::string �ϵ� = "BP_Slasher_Character_40_01_C";


	inline static const std::string ������������ = "Item_Slasher_Beartrap";
	inline static const std::string С������������ = "Item_Slasher_CloakBell";
	inline static const std::string ȳȳ�������� = "Item_Slasher_Chainsaw";
	inline static const std::string ��ʿ�������� = "Item_Slasher_Blinker";
	inline static const std::string ������������ = "Item_Slasher_Stalker";
	inline static const std::string �������������� = "Item_Slasher_PhantomTrap";
	inline static const std::string ������������ = "Item_Slasher_Killer07Item";
	inline static const std::string ������������ = "Item_Slasher_Hatchet";
	inline static const std::string Ƥ���������� = "Item_Slasher_LFChainsaw";
	inline static const std::string ������������ = "Item_Slasher_DreamInducer";
	inline static const std::string ������������ = "Item_Slasher_ReverseBearTrap";
	inline static const std::string С���������� = "Item_Slasher_GasBomb";
	inline static const std::string ������������ = "Item_Slasher_PhaseWalker";
	inline static const std::string ������������ = "Item_Slasher_Frenzy";
	inline static const std::string ������������ = "Item_Slasher_PlaguePower";
	inline static const std::string ������������ = "Item_Slasher_GhostPower";
	inline static const std::string ħ���������� = "Item_Slasher_QatarKillerPower";
	inline static const std::string ����ʿ�������� = "Item_Slasher_Kanobo";
	inline static const std::string ������������ = "Item_Slasher_HarpoonRifle";
	inline static const std::string ����ͷ�������� = "Item_Slasher_TormentMode";
	inline static const std::string ��ħ�������� = "Item_Slasher_K21Power";
	inline static const std::string ����Ӥ�������� = "Item_Slasher_K22Power";
	inline static const std::string ����������� = "Item_Slasher_ThrowingKnives";
	inline static const std::string ׷������������ = "Item_Slasher_K24Power";
	inline static const std::string ����ͷ�������� = "Item_Slasher_K25Power";
	inline static const std::string ����������� = "Item_Slasher_K26Power";
	inline static const std::string ������������ = "Item_Slasher_K27Power";
	inline static const std::string Ӱħ�������� = "Item_Slasher_K28Power";
	inline static const std::string ��˹���������� = "Item_Slasher_K29Power";
	inline static const std::string ����ʿ�������� = "Item_Slasher_K30Power";
	inline static const std::string �׹������������� = "Item_Slasher_K31Power";
	inline static const std::string ����������� = "Item_Slasher_K32Power";
	inline static const std::string ������������ = "Item_Slasher_K33Power";
	inline static const std::string �ú����������� = "Item_Slasher_K34Power";
	inline static const std::string δ֪������������ = "Item_Slasher_K35Power";
	inline static const std::string ������������ = "Item_Slasher_K36Power";
	inline static const std::string �¹����������� = "Item_Slasher_K37Power";
	inline static const std::string �˺��������� = "Item_Slasher_K38Power";
	inline static const std::string ��ľ���������� = "Item_Slasher_K39Power";
	inline static const std::string �ϵ��������� = "Item_Slasher_K40Power";

};

// ɱ������ӳ���
static const std::unordered_map<std::string, std::wstring> nameMapping = {
	{ɱ��::����, L"����"},
	{ɱ��::С����, L"С����"},
	{ɱ��::ȳȳ, L"ȳȳ"},
	{ɱ��::��ʿ, L"��ʿ"},
	{ɱ��::����, L"����"},
	{ɱ��::������, L"������"},
	{ɱ��::����, L"����"},
	{ɱ��::Ƥ��, L"Ƥ��"},
	{ɱ��::����, L"����"},
	{ɱ��::����, L"����"},
	{ɱ��::����, L"����"},
	{ɱ��::С��, L"С��"},
	{ɱ��::����, L"����"},
	{ɱ��::����, L"����"},
	{ɱ��::����, L"����"},
	{ɱ��::����, L"����"},
	{ɱ��::ħ��, L"ħ��"},
	{ɱ��::����ʿ, L"����ʿ"},
	{ɱ��::����, L"����"},
	{ɱ��::����ͷ, L"����ͷ"},
	{ɱ��::��ħ, L"��ħ"},
	{ɱ��::����Ӥ, L"����Ӥ"},
	{ɱ��::���, L"���"},
	{ɱ��::׷����, L"׷����"},
	{ɱ��::����ͷ, L"����ͷ"},
	{ɱ��::���, L"���"},
	{ɱ��::����, L"����"},
	{ɱ��::Ӱħ, L"Ӱħ"},
	{ɱ��::��˹��, L"��˹��"},
	{ɱ��::����ʿ, L"����ʿ"},
	{ɱ��::�׹�����, L"�׹�����"},
	{ɱ��::���, L"���"},
	{ɱ��::����, L"����"},
	{ɱ��::�ú���, L"�ú���"},
	{ɱ��::δ֪����, L"δ֪����"},
	{ɱ��::����, L"����"},
	{ɱ��::�¹���, L"�¹���"},
	{ɱ��::�˺�, L"�˺�"},
	{ɱ��::��ľ��, L"��ľ��"},
	{ɱ��::�ϵ�, L"�ϵ�"},


	{ɱ��::������������, L"����"},
	{ɱ��::С������������, L"С����"},
	{ɱ��::ȳȳ��������, L"ȳȳ"},
	{ɱ��::��ʿ��������, L"��ʿ"},
	{ɱ��::������������, L"����"},
	{ɱ��::��������������, L"������"},
	{ɱ��::������������, L"����"},
	{ɱ��::������������, L"����"},
	{ɱ��::Ƥ����������, L"Ƥ��"},
	{ɱ��::������������, L"����"},
	{ɱ��::������������, L"����"},
	{ɱ��::С����������, L"С��"},
	{ɱ��::������������, L"����"},
	{ɱ��::������������, L"����"},
	{ɱ��::������������, L"����"},
	{ɱ��::������������, L"����"},
	{ɱ��::ħ����������, L"ħ��"},
	{ɱ��::����ʿ��������, L"����ʿ"},
	{ɱ��::������������, L"����"},
	{ɱ��::����ͷ��������, L"����ͷ"},
	{ɱ��::��ħ��������, L"��ħ��"},
	{ɱ��::����Ӥ��������, L"����Ӥ"},
	{ɱ��::�����������, L"���"},
	{ɱ��::׷������������, L"׷����"},
	{ɱ��::����ͷ��������, L"����ͷ"},
	{ɱ��::�����������, L"���"},
	{ɱ��::������������, L"����"},
	{ɱ��::Ӱħ��������, L"Ӱħ"},
	{ɱ��::��˹����������, L"��˹��"},
	{ɱ��::����ʿ��������, L"����ʿ"},
	{ɱ��::�׹�������������, L"�׹�����"},
	{ɱ��::�����������, L"���"},
	{ɱ��::������������, L"����"},
	{ɱ��::�ú�����������, L"�ú���"},
	{ɱ��::δ֪������������, L"δ֪����"},
	{ɱ��::������������, L"����"},
	{ɱ��::�¹�����������, L"�¹���"},
	{ɱ��::�˺���������, L"�˺�"},
	{ɱ��::��ľ����������, L"��ľ��"},
	{ɱ��::�ϵ���������, L"�ϵ�"}

};


struct �Ҵ������ü��� {
	inline static const std::string ��ϧһ��1 = "";
	inline static const std::string ��ϧһ��2 = "AnyMeansNecessary";
	inline static const std::string ��ʹ���� = "NoMither";
	inline static const std::string ����Ԥ�� = "Premonition";
	inline static const std::string ���ҹ��� = "Dance_with_me";
	inline static const std::string ��ɽ���� = "S26P02";
	inline static const std::string Ϊ������ = "S37P02";
	inline static const std::string �������� = "S30P01";
	inline static const std::string ������ = "S40P03";
	inline static const std::string ��֮���� = "TheMettleOfMan";
	inline static const std::string ������ʶ = "Deja_Vu";
	inline static const std::string �͵����� = "S32P03";
	inline static const std::string ��ⳤ�� = "S31P02";
	inline static const std::string ��ķ = "Babysitter";
	inline static const std::string ������ʿ = "S40P01";
	inline static const std::string �˷��ѹ� = "S29P01";
	inline static const std::string ������թ = "S24P02";
	inline static const std::string ����רע = "S31P01";
	inline static const std::string ����֮�� = "InnerStrength";
	inline static const std::string ��̱��� = "Sprint_Burst";
	inline static const std::string �ֱ����� = "Breakdown";
	inline static const std::string �����˺� = "S46P01";
	inline static const std::string �ٺ����� = "AfterCare";
	inline static const std::string ����ֱǰ = "HeadOn";
	inline static const std::string ��ѧ���� = "S39P02";
	inline static const std::string ǧ���� = "S28P01";
	inline static const std::string ���˱��� = "S38P01";
	inline static const std::string ѹ��ͬ�� = "RepressedAlliance";
	inline static const std::string ������ = "S35P02";
	inline static const std::string ����֮�� = "S26P01";
	inline static const std::string ����Ϊ�� = "S45P02";
	inline static const std::string ͬ־���� = "Camaraderie";
	inline static const std::string ͬ�� = "Kindred";
	inline static const std::string ͬ������ = "No_One_Left_Behind";
	inline static const std::string ����ʫ�˼��� = "S42P02";
	inline static const std::string ҧ������ = "S27P01";
	inline static const std::string �����Ʊ� = "Deliverance";
	inline static const std::string �Ž�һ�� = "Solidarity";
	inline static const std::string �ŶӺ���_����֮�� = "S35P03";
	inline static const std::string �ŶӺ���_ȫ������ = "S36P03";
	inline static const std::string �᲻�ɴ� = "SelfSufficient";
	inline static const std::string �������� = "Diversion";
	inline static const std::string ����֮�� = "SecondWind";
	inline static const std::string ���˻ᴫȾ = "Up_The_Ante";
	inline static const std::string ��ҹ�� = "Vigil";
	inline static const std::string �������� = "Calm_Spirit";
	inline static const std::string ������ = "S33P02";
	inline static const std::string �Բ����� = "S32P02";
	inline static const std::string ϣ��֮�� = "Hope";
	inline static const std::string Ļ����� = "S36P01";
	inline static const std::string ƽ����½ = "Balanced_Landing";
	inline static const std::string ���н�չ = "FlipFlop";
	inline static const std::string ���˴�Ϣ = "LuckyBreak";
	inline static const std::string ������ = "S39P01";
	inline static const std::string ǿ������ = "S24P03";
	inline static const std::string ǿ��ֱ�� = "Plunderers_Instinct";
	inline static const std::string �������� = "S35P01";
	inline static const std::string ���鹲�� = "Empathy";
	inline static const std::string �ľ����� = "S42P03";
	inline static const std::string ���ٷ�Ӧ = "S46P03";
	inline static const std::string ���ٲ��� = "S34P03";
	inline static const std::string ������� = "S31P03";
	inline static const std::string ���پ��� = "QuickQuiet";
	inline static const std::string ����_ָ������ = "S29P03";
	inline static const std::string ����_���� = "S40P02";
	inline static const std::string ����_��Ӱ�� = "S28P03";
	inline static const std::string ����_����֮�� = "S28P02";
	inline static const std::string ����_�ڰ����� = "S30P03";
	inline static const std::string ��ͬ���� = "S30P02";
	inline static const std::string �ɹ��ݾ� = "S25P01";
	inline static const std::string ����Ա = "Technician";
	inline static const std::string ���Ѽ��� = "Breakout";
	inline static const std::string Я�ֺ��� = "Bond";
	inline static const std::string ���˾��� = "S27P03";
	inline static const std::string ʱ���Ҵ� = "BorrowedTime";
	inline static const std::string ������� = "S24P01";
	inline static const std::string ���ڸ�֪ = "Dark_Sense";
	inline static const std::string ������ = "Urban_Evasion";
	inline static const std::string ���������� = "SoleSurvivor";
	inline static const std::string δ����� = "BuckleUp";
	inline static const std::string ĩ�ղб� = "LeftBehind";
	inline static const std::string ����֮�� = "WindowsOfOpportunity";
	inline static const std::string ���Ϸ��� = "DecisiveStrike";
	inline static const std::string ë���Ȼ = "Spine_Chill";
	inline static const std::string ����ʧ�� = "Distortion";
	inline static const std::string �������� = "Tenacity";
	inline static const std::string ������ = "S43P02";
	inline static const std::string ԡѪ��� = "S36P02";
	inline static const std::string ����� = "S37P03";
	inline static const std::string Ǳ������ = "S34P01";
	inline static const std::string ������� = "SoulGuard";
	inline static const std::string ��Ȼһ�� = "S33P01";
	inline static const std::string ��ը���� = "S26P03";
	inline static const std::string ���Ծ��� = "Small_Game";
	inline static const std::string �����빲 = "WereGonnaLiveForever";
	inline static const std::string ���۲��� = "Resilience";
	inline static const std::string �ý����� = "S43P01";
	inline static const std::string ������ʩ = "S29P02";
	inline static const std::string �ƻ��� = "Saboteur";
	inline static const std::string �ư����� = "StakeOut";
	inline static const std::string ��_֯��֩�� = "S41P01";
	inline static const std::string ��_�ն���ѻ = "S45P01";
	inline static const std::string �Ȳ�ʤȯ = "Open_Handed";
	inline static const std::string ������ = "S32P01";
	inline static const std::string ���ɽ�� = "S37P01";
	inline static const std::string ���аٲ� = "Botany_Knowledge";
	inline static const std::string �������� = "BuiltToLast";
	inline static const std::string �����Բ� = "DesperateMeasures";
	inline static const std::string �ϱ��� = "S43P03";
	inline static const std::string �������� = "S38P02";
	inline static const std::string �۾����� = "S33P03";
	inline static const std::string �縺�ص� = "S45P03";
	inline static const std::string �������� = "Adrenaline";
	inline static const std::string ʤ��ϲ�� = "S44P02";
	inline static const std::string ����Ұ�� = "BoilOver";
	inline static const std::string ��ѧ�ɲ� = "Autodidact";
	inline static const std::string ���ұ��� = "S25P03";
	inline static const std::string �������� = "Self_Care";
	inline static const std::string �ἺΪ�� = "ForThePeople";
	inline static const std::string ��ҫʱ�� = "S44P03";
	inline static const std::string ҩ������ = "Pharmacy";
	inline static const std::string ���ƴ��� = "Poised";
	inline static const std::string ���� = "Alert";
	inline static const std::string ��ֱ̽�� = "DetectivesHunch";
	inline static const std::string �϶�Ŀ�� = "Fixated";
	inline static const std::string ֤���Լ� = "Prove_Thyself";
	inline static const std::string �������� = "S46P02";
	inline static const std::string ��������֮�� = "S44P01";
	inline static const std::string ��崾��� = "S38P03";
	inline static const std::string ת��ע�� = "RedHerring";
	inline static const std::string �䶯�ݳ� = "S25P02";
	inline static const std::string �����ë = "Lightweight";
	inline static const std::string ��ӯ���� = "Lithe";
	inline static const std::string ��ӯè�� = "S39P03";
	inline static const std::string Զ��׿ʶ = "Visionary";
	inline static const std::string ������� = "S34P02";
	inline static const std::string ��֮زز = "Slippery_Meat";
	inline static const std::string а��֮�� = "S41P03";
	inline static const std::string �������� = "St";//Streetwise��ΪSt
	inline static const std::string ���ѣ����� = "Wakeup";
	inline static const std::string �ֽ����� = "DeadHard";
	inline static const std::string ������־ = "Iron_Will";
	inline static const std::string ������� = "Ace_In_The_Hole";
	inline static const std::string ��Ӱ���� = "S42P01";
	inline static const std::string ���ⵯ = "S27P02";
	inline static const std::string ����֮�� = "S41P02";
	inline static const std::string �������� = "This_Is_Not_Happening";
	inline static const std::string ����ʽ���� = "OffTheRecord";
	inline static const std::string ����Ⱥ�� = "Leader";
	inline static const std::string ���ɹ� = "WellMakeIt";
	inline static const std::string ��ǣ���� = "ObjectOfObsession";
	inline static const std::string ��Ѫ��Լ = "BloodPact";
	inline static const std::string ����Э�� = "BetterTogether";

};


struct ɱ�����ü��� {

	inline static const std::string С��־�֢ = "Coulrophobia";
	inline static const std::string ѹ�������� = "OverwhelmingPresence";
	inline static const std::string ������� = "Whispers";
	inline static const std::string ���� = "K25P01";
	inline static const std::string �������� = "Unrelenting";
	inline static const std::string ����ָ�� = "Surveillance";
	inline static const std::string ����̰�� = "K37P02";
	inline static const std::string �����տ� = "BBQAndChilli";
	inline static const std::string �������ѡ�� = "MakeYourChoice";
	inline static const std::string ����֮�� = "Lightborn";
	inline static const std::string ���֮ӵ = "K26P01";
	inline static const std::string	�뵡֮�� = "K36P02";
	inline static const std::string �������� = "InTheDark";
	inline static const std::string �з�֮ʹ = "K22P02";
	inline static const std::string ����_���� = "HexUndying";
	inline static const std::string ����_�������� = "K34P01";
	inline static const std::string ����_�����Կ� = "K39P01";
	inline static const std::string ����_��� = "K26P03";
	inline static const std::string ����_����ϣ�� = "Hex_Devour_Hope";
	inline static const std::string ����_�������� = "K37P01";
	inline static const std::string ����_�ͽ� = "HexRetribution";
	inline static const std::string ����_���� = "Hex_Ruin";
	inline static const std::string ����_ҡ���� = "Hex_HuntressLullaby";
	inline static const std::string ����_��ɱ���� = "Hex_Thrill_Of_The_Hunt";
	inline static const std::string ����_���� = "K25P02";
	inline static const std::string ����_ֱ��ڰ� = "K30P02";
	inline static const std::string ����_������ӡ = "Hex_The_Third_Seal";
	inline static const std::string ����_Ⱥ����� = "K23P02";
	inline static const std::string ����_�ֹ�֮�� = "Hex_HauntedGround";
	inline static const std::string ����_����һ�� = "No_One_Escapes_Death";
	inline static const std::string ����_��Ѫ���� = "HexBloodFavor";
	inline static const std::string ����_ѹ�������� = "OverwhelmingPresence";
	inline static const std::string �����쳤�ؾ� = "K34P02";
	inline static const std::string �������� = "K31P02";
	inline static const std::string ������֪ = "Stridor";
	inline static const std::string ��Ѫ���� = "BeastOfPrey";
	inline static const std::string �ڻ��� = "K22P01";
	inline static const std::string ������ = "K32P01";
	inline static const std::string ������ = "K38P01";
	inline static const std::string ���ֹ���_�����ֵİ�Ϸ = "HangmansTrick";
	inline static const std::string ���ֹ���_��ŭ֮�� = "K27P01";
	inline static const std::string ���ֹ���_ʹ�๲�� = "K26P02";
	inline static const std::string ���ֹ���_ʹ������ = "K25P03";
	inline static const std::string ���ֹ���_������� = "K38P02";
	inline static const std::string ���ֹ���_��ħ���� = "Monstrous_Shrine";
	inline static const std::string ʧЧ���� = "DeadMansSwitch";
	inline static const std::string ��Ϸ���� = "K31P01";
	inline static const std::string ��������֮�� = "FranklinsLoss";
	inline static const std::string ǿ����� = "ForcedPenance";
	inline static const std::string ���� = "Tinkerer";
	inline static const std::string ����֮�� = "DragonsGrip";
	inline static const std::string ���α��� = "K33P03";
	inline static const std::string �İ����� = "K36P03";
	inline static const std::string �ľ����� = "pop_goes_the_weasel";
	inline static const std::string ŭ������ = "FireUp";
	inline static const std::string Թ������ = "Rancor";
	inline static const std::string �־崫Ⱦ = "InfectiousFright";
	inline static const std::string �ֻŽ��� = "Unnerving_Presence";
	inline static const std::string �־��ͷ� = "Distressing";
	inline static const std::string ���ͽ��� = "K22P03";
	inline static const std::string ����ս�� = "ThrillingTremors";
	inline static const std::string ��Ⱦ֮���� = "K28P03";
	inline static const std::string ��֪���� = "K29P02";
	inline static const std::string ��Ҫ��Ū���ʳ�� = "Play_With_Your_Food";
	inline static const std::string �����޵� = "Brutal_Strength";
	inline static const std::string ����õ�������� = "Save_The_Best_For_Last";
	inline static const std::string ��ĥ·�� = "TrailofTorment";
	inline static const std::string ��ʿ�ĺ��� = "NurseCalling";
	inline static const std::string ��Ӧ = "Nemesis";
	inline static const std::string ֧��֮�� = "K37P03";
	inline static const std::string ��һ���� = "K39P03";
	inline static const std::string �޴��ɲ� = "K30P01";
	inline static const std::string ����籩 = "K27P03";
	inline static const std::string ��Ч = "K35P03";
	inline static const std::string ���� = "K35P01";
	inline static const std::string ��·���� = "K23P03";
	inline static const std::string �޼� = "K35P02";
	inline static const std::string ��ҹ֮�� = "Shadowborn";
	inline static const std::string ��ŭԹ�� = "SpiritFury";
	inline static const std::string ����ѧϰ = "K32P03";
	inline static const std::string �ܸ�ЧӦ = "K31P03";
	inline static const std::string ��ϲ���� = "Agitation";
	inline static const std::string Ъ˹���� = "K24P02";
	inline static const std::string �����־� = "Thanatophobia";
	inline static const std::string ����� = "Deathbound";
	inline static const std::string ���� = "ZanshinTactics";
	inline static const std::string �п���� = "CruelConfinement";
	inline static const std::string �������� = "K38P03";
	inline static const std::string ���㽻֯ = "K39P02";
	inline static const std::string ϴ������ = "ImAllEars";
	inline static const std::string ����֮�� = "Dying_Light";
	inline static const std::string ������ɢ = "K28P01";
	inline static const std::string ���� = "K24P03";
	inline static const std::string ���ν��� = "Iron_Grasp";
	inline static const std::string ��¹�� = "Deerstalker";
	inline static const std::string �߽���־ = "Mindbreaker";
	inline static const std::string ��ӿ = "Surge";
	inline static const std::string �������� = "GeneratorOvercharge";
	inline static const std::string ������� = "MadGrit";
	inline static const std::string ���ٲб� = "K33P02";
	inline static const std::string ��׷���� = "Predator";
	inline static const std::string ����ϣ�� = "BoonDestroyer";
	inline static const std::string �վ�ս�� = "K29P03";
	inline static const std::string ��ˮ�ٻ� = "K27P02";
	inline static const std::string ��֯���� = "K36P01";
	inline static const std::string �����־� = "Enduring";
	inline static const std::string ���ø�Ԥ = "CorruptIntervention";
	inline static const std::string �ռ����� = "K33P01";
	inline static const std::string �������� = "Bamboozle";
	inline static const std::string ��ɬ���� = "Bitter_Murmur";
	inline static const std::string Ѫ��׷�� = "Bloodhound";
	inline static const std::string ����׷���� = "K24P01";
	inline static const std::string �м����� = "MonitorAndAbuse";
	inline static const std::string �������� = "RememberMe";
	inline static const std::string �豸������ = "Gearhead";
	inline static const std::string Ѫ������ = "BloodWarden";
	inline static const std::string ���������� = "K29P01";
	inline static const std::string ���ǰ = "K32P02";
	inline static const std::string �������� = "Sloppy_Butcher";
	inline static const std::string ׷�ǿ� = "K23P01";
	inline static const std::string ���ʧг = "Discordance";
	inline static const std::string ���׵�� = "K34P03";
	inline static const std::string ����Ů = "Ironmaiden";
	inline static const std::string ��ֹ���� = "Insidious";
	inline static const std::string �����ʶ = "TerritorialImperative";
	inline static const std::string �����Դ� = "K30P03";
	inline static const std::string ����׷ɱ = "FurtiveChase";
	inline static const std::string ��ѻ��Ӱ = "Spies_From_The_Shadows";
	inline static const std::string ��Ѫ���� = "BloodEcho";
	inline static const std::string �ڰ����� = "K28P02";
	inline static const std::string �ڰ����� = "DarkDevotion";

	inline static const std::string ��Ƹ���� = "K40P01";
	inline static const std::string ��Ӱ�־� = "K40P02";
	inline static const std::string �豸���� = "K40P03";

};



// ���ܳ����뱾�ػ�����ӳ��
static const std::unordered_map<std::string, std::wstring> PeiZhiPerkNameMap = {
	{ ɱ�����ü���::С��־�֢, L"С��־�֢" },
	{ ɱ�����ü���::ѹ��������, L"ѹ��������" },
	{ ɱ�����ü���::�������, L"�������" },
	{ ɱ�����ü���::����, L"����" },
	{ ɱ�����ü���::��������, L"��������" },
	{ ɱ�����ü���::����ָ��, L"����ָ��" },
	{ ɱ�����ü���::����̰��, L"����̰��" },
	{ ɱ�����ü���::�����տ�, L"�����տ�" },
	{ ɱ�����ü���::�������ѡ��, L"�������ѡ��" },
	{ ɱ�����ü���::����֮��, L"����֮��" },
	{ ɱ�����ü���::���֮ӵ, L"���֮ӵ" },
	{ ɱ�����ü���::�뵡֮��, L"�뵡֮��" },
	{ ɱ�����ü���::��������, L"��������" },
	{ ɱ�����ü���::�з�֮ʹ, L"�з�֮ʹ" },

	// �����༼��
	{ ɱ�����ü���::����_����, L"����_����" },
	{ ɱ�����ü���::����_��������, L"����_��������" },
	{ ɱ�����ü���::����_�����Կ�, L"����_�����Կ�" },
	{ ɱ�����ü���::����_���, L"����_���" },
	{ ɱ�����ü���::����_����ϣ��, L"����_����ϣ��" },
	{ ɱ�����ü���::����_��������, L"����_��������" },
	{ ɱ�����ü���::����_�ͽ�, L"����_�ͽ�" },
	{ ɱ�����ü���::����_����, L"����_����" },
	{ ɱ�����ü���::����_ҡ����, L"����_ҡ����" },
	{ ɱ�����ü���::����_��ɱ����, L"����_��ɱ����" },
	{ ɱ�����ü���::����_����, L"����_����" },
	{ ɱ�����ü���::����_ֱ��ڰ�, L"����_ֱ��ڰ�" },
	{ ɱ�����ü���::����_������ӡ, L"����_������ӡ" },
	{ ɱ�����ü���::����_Ⱥ�����, L"����_Ⱥ�����" },
	{ ɱ�����ü���::����_�ֹ�֮��, L"����_�ֹ�֮��" },
	{ ɱ�����ü���::����_����һ��, L"����_����һ��" },
	{ ɱ�����ü���::����_��Ѫ����, L"����_��Ѫ����" },

	// ���ֹ����༼��
	{ ɱ�����ü���::���ֹ���_�����ֵİ�Ϸ, L"���ֹ���_�����ֵİ�Ϸ" },
	{ ɱ�����ü���::���ֹ���_��ŭ֮��, L"���ֹ���_��ŭ֮��" },
	{ ɱ�����ü���::���ֹ���_ʹ�๲��, L"���ֹ���_ʹ�๲��" },
	{ ɱ�����ü���::���ֹ���_ʹ������, L"���ֹ���_ʹ������" },
	{ ɱ�����ü���::���ֹ���_�������, L"���ֹ���_�������" },
	{ ɱ�����ü���::���ֹ���_��ħ����, L"���ֹ���_��ħ����" },

	// ��������
	{ ɱ�����ü���::�����쳤�ؾ�, L"�����쳤�ؾ�" },
	{ ɱ�����ü���::��������, L"��������" },
	{ ɱ�����ü���::������֪, L"������֪" },
	{ ɱ�����ü���::��Ѫ����, L"��Ѫ����" },
	{ ɱ�����ü���::�ڻ���, L"�ڻ���" },
	{ ɱ�����ü���::������, L"������" },
	{ ɱ�����ü���::������, L"������" },
	{ ɱ�����ü���::ʧЧ����, L"ʧЧ����" },
	{ ɱ�����ü���::��Ϸ����, L"��Ϸ����" },
	{ ɱ�����ü���::��������֮��, L"��������֮��" },
	{ ɱ�����ü���::ǿ�����, L"ǿ�����" },
	{ ɱ�����ü���::����, L"����" },
	{ ɱ�����ü���::����֮��, L"����֮��" },
	{ ɱ�����ü���::���α���, L"���α���" },
	{ ɱ�����ü���::�İ�����, L"�İ�����" },
	{ ɱ�����ü���::�ľ�����, L"�ľ�����" },
	{ ɱ�����ü���::ŭ������, L"ŭ������" },
	{ ɱ�����ü���::Թ������, L"Թ������" },
	{ ɱ�����ü���::�־崫Ⱦ, L"�־崫Ⱦ" },
	{ ɱ�����ü���::�ֻŽ���, L"�ֻŽ���" },
	{ ɱ�����ü���::�־��ͷ�, L"�־��ͷ�" },
	{ ɱ�����ü���::���ͽ���, L"���ͽ���" },
	{ ɱ�����ü���::����ս��, L"����ս��" },
	{ ɱ�����ü���::��Ⱦ֮����, L"��Ⱦ֮����" },
	{ ɱ�����ü���::��֪����, L"��֪����" },
	{ ɱ�����ü���::��Ҫ��Ū���ʳ��, L"��Ҫ��Ū���ʳ��" },
	{ ɱ�����ü���::�����޵�, L"�����޵�" },
	{ ɱ�����ü���::����õ��������, L"����õ��������" },
	{ ɱ�����ü���::��ĥ·��, L"��ĥ·��" },
	{ ɱ�����ü���::��ʿ�ĺ���, L"��ʿ�ĺ���" },
	{ ɱ�����ü���::��Ӧ, L"��Ӧ" },
	{ ɱ�����ü���::֧��֮��, L"֧��֮��" },
	{ ɱ�����ü���::��һ����, L"��һ����" },
	{ ɱ�����ü���::�޴��ɲ�, L"�޴��ɲ�" },
	{ ɱ�����ü���::����籩, L"����籩" },
	{ ɱ�����ü���::��Ч, L"��Ч" },
	{ ɱ�����ü���::����, L"����" },
	{ ɱ�����ü���::��·����, L"��·����" },
	{ ɱ�����ü���::�޼�, L"�޼�" },
	{ ɱ�����ü���::��ҹ֮��, L"��ҹ֮��" },
	{ ɱ�����ü���::��ŭԹ��, L"��ŭԹ��" },
	{ ɱ�����ü���::����ѧϰ, L"����ѧϰ" },
	{ ɱ�����ü���::�ܸ�ЧӦ, L"�ܸ�ЧӦ" },
	{ ɱ�����ü���::��ϲ����, L"��ϲ����" },
	{ ɱ�����ü���::Ъ˹����, L"Ъ˹����" },
	{ ɱ�����ü���::�����־�, L"�����־�" },
	{ ɱ�����ü���::�����, L"�����" },
	{ ɱ�����ü���::����, L"����" },
	{ ɱ�����ü���::�п����, L"�п����" },
	{ ɱ�����ü���::��������, L"��������" },
	{ ɱ�����ü���::���㽻֯, L"���㽻֯" },
	{ ɱ�����ü���::ϴ������, L"ϴ������" },
	{ ɱ�����ü���::����֮��, L"����֮��" },
	{ ɱ�����ü���::������ɢ, L"������ɢ" },
	{ ɱ�����ü���::����, L"����" },
	{ ɱ�����ü���::���ν���, L"���ν���" },
	{ ɱ�����ü���::��¹��, L"��¹��" },
	{ ɱ�����ü���::�߽���־, L"�߽���־" },
	{ ɱ�����ü���::��ӿ, L"��ӿ" },
	{ ɱ�����ü���::��������, L"��������" },
	{ ɱ�����ü���::�������, L"�������" },
	{ ɱ�����ü���::���ٲб�, L"���ٲб�" },
	{ ɱ�����ü���::��׷����, L"��׷����" },
	{ ɱ�����ü���::����ϣ��, L"����ϣ��" },
	{ ɱ�����ü���::�վ�ս��, L"�վ�ս��" },
	{ ɱ�����ü���::��ˮ�ٻ�, L"��ˮ�ٻ�" },
	{ ɱ�����ü���::��֯����, L"��֯����" },
	{ ɱ�����ü���::�����־�, L"�����־�" },
	{ ɱ�����ü���::���ø�Ԥ, L"���ø�Ԥ" },
	{ ɱ�����ü���::�ռ�����, L"�ռ�����" },
	{ ɱ�����ü���::��������, L"��������" },
	{ ɱ�����ü���::��ɬ����, L"��ɬ����" },
	{ ɱ�����ü���::Ѫ��׷��, L"Ѫ��׷��" },
	{ ɱ�����ü���::����׷����, L"����׷����" },
	{ ɱ�����ü���::�м�����, L"�м�����" },
	{ ɱ�����ü���::��������, L"��������" },
	{ ɱ�����ü���::�豸������, L"�豸������" },
	{ ɱ�����ü���::Ѫ������, L"Ѫ������" },
	{ ɱ�����ü���::����������, L"����������" },
	{ ɱ�����ü���::���ǰ, L"���ǰ" },
	{ ɱ�����ü���::��������, L"��������" },
	{ ɱ�����ü���::׷�ǿ�, L"׷�ǿ�" },
	{ ɱ�����ü���::���ʧг, L"���ʧг" },
	{ ɱ�����ü���::���׵��, L"���׵��" },
	{ ɱ�����ü���::����Ů, L"����Ů" },
	{ ɱ�����ü���::��ֹ����, L"��ֹ����" },
	{ ɱ�����ü���::�����ʶ, L"�����ʶ" },
	{ ɱ�����ü���::�����Դ�, L"�����Դ�" },
	{ ɱ�����ü���::����׷ɱ, L"����׷ɱ" },
	{ ɱ�����ü���::��ѻ��Ӱ, L"��ѻ��Ӱ" },
	{ ɱ�����ü���::��Ѫ����, L"��Ѫ����" },
	{ ɱ�����ü���::�ڰ�����, L"�ڰ�����" },
	{ ɱ�����ü���::�ڰ�����, L"�ڰ�����" },
	{ ɱ�����ü���::��Ƹ����, L"��Ƹ����" },
	{ ɱ�����ü���::��Ӱ�־�, L"��Ӱ�־�" },
	{ ɱ�����ü���::�豸����, L"�豸����" },


	{ �Ҵ������ü���::��ϧһ��1, L"��ϧһ��" },
	{ �Ҵ������ü���::��ϧһ��2, L"��ϧһ��" },
	{ �Ҵ������ü���::��ʹ����, L"��ʹ����" },
	{ �Ҵ������ü���::����Ԥ��, L"����Ԥ��" },
	{ �Ҵ������ü���::���ҹ���, L"���ҹ���" },
	{ �Ҵ������ü���::��ɽ����, L"��ɽ����" },
	{ �Ҵ������ü���::Ϊ������, L"Ϊ������" },
	{ �Ҵ������ü���::��������, L"��������" },
	{ �Ҵ������ü���::������, L"������" },
	{ �Ҵ������ü���::��֮����, L"��֮����" },
	{ �Ҵ������ü���::������ʶ, L"������ʶ" },
	{ �Ҵ������ü���::�͵�����, L"�͵�����" },
	{ �Ҵ������ü���::��ⳤ��, L"��ⳤ��" },
	{ �Ҵ������ü���::��ķ, L"��ķ" },
	{ �Ҵ������ü���::������ʿ, L"������ʿ" },
	{ �Ҵ������ü���::�˷��ѹ�, L"�˷��ѹ�" },
	{ �Ҵ������ü���::������թ, L"������թ" },
	{ �Ҵ������ü���::����רע, L"����רע" },
	{ �Ҵ������ü���::����֮��, L"����֮��" },
	{ �Ҵ������ü���::��̱���, L"��̱���" },
	{ �Ҵ������ü���::�ֱ�����, L"�ֱ�����" },
	{ �Ҵ������ü���::�����˺�, L"�����˺�" },
	{ �Ҵ������ü���::�ٺ�����, L"�ٺ�����" },
	{ �Ҵ������ü���::����ֱǰ, L"����ֱǰ" },
	{ �Ҵ������ü���::��ѧ����, L"��ѧ����" },
	{ �Ҵ������ü���::ǧ����, L"ǧ����" },
	{ �Ҵ������ü���::���˱���, L"���˱���" },
	{ �Ҵ������ü���::ѹ��ͬ��, L"ѹ��ͬ��" },
	{ �Ҵ������ü���::������, L"������" },
	{ �Ҵ������ü���::����֮��, L"����֮��" },
	{ �Ҵ������ü���::����Ϊ��, L"����Ϊ��" },
	{ �Ҵ������ü���::ͬ־����, L"ͬ־����" },
	{ �Ҵ������ü���::ͬ��, L"ͬ��" },
	{ �Ҵ������ü���::ͬ������, L"ͬ������" },
	{ �Ҵ������ü���::����ʫ�˼���, L"����ʫ�˼���" },
	{ �Ҵ������ü���::ҧ������, L"ҧ������" },
	{ �Ҵ������ü���::�����Ʊ�, L"�����Ʊ�" },
	{ �Ҵ������ü���::�Ž�һ��, L"�Ž�һ��" },
	{ �Ҵ������ü���::�ŶӺ���_����֮��, L"�ŶӺ���_����֮��" },
	{ �Ҵ������ü���::�ŶӺ���_ȫ������, L"�ŶӺ���_ȫ������" },
	{ �Ҵ������ü���::�᲻�ɴ�, L"�᲻�ɴ�" },
	{ �Ҵ������ü���::��������, L"��������" },
	{ �Ҵ������ü���::����֮��, L"����֮��" },
	{ �Ҵ������ü���::���˻ᴫȾ, L"���˻ᴫȾ" },
	{ �Ҵ������ü���::��ҹ��, L"��ҹ��" },
	{ �Ҵ������ü���::��������, L"��������" },
	{ �Ҵ������ü���::������, L"������" },
	{ �Ҵ������ü���::�Բ�����, L"�Բ�����" },
	{ �Ҵ������ü���::ϣ��֮��, L"ϣ��֮��" },
	{ �Ҵ������ü���::Ļ�����, L"Ļ�����" },
	{ �Ҵ������ü���::ƽ����½, L"ƽ����½" },
	{ �Ҵ������ü���::���н�չ, L"���н�չ" },
	{ �Ҵ������ü���::���˴�Ϣ, L"���˴�Ϣ" },
	{ �Ҵ������ü���::������, L"������" },
	{ �Ҵ������ü���::ǿ������, L"ǿ������" },
	{ �Ҵ������ü���::ǿ��ֱ��, L"ǿ��ֱ��" },
	{ �Ҵ������ü���::��������, L"��������" },
	{ �Ҵ������ü���::���鹲��, L"���鹲��" },
	{ �Ҵ������ü���::�ľ�����, L"�ľ�����" },
	{ �Ҵ������ü���::���ٷ�Ӧ, L"���ٷ�Ӧ" },
	{ �Ҵ������ü���::���ٲ���, L"���ٲ���" },
	{ �Ҵ������ü���::�������, L"�������" },
	{ �Ҵ������ü���::���پ���, L"���پ���" },
	{ �Ҵ������ü���::����_ָ������, L"����_ָ������" },
	{ �Ҵ������ü���::����_����, L"����_����" },
	{ �Ҵ������ü���::����_��Ӱ��, L"����_��Ӱ��" },
	{ �Ҵ������ü���::����_����֮��, L"����_����֮��" },
	{ �Ҵ������ü���::����_�ڰ�����, L"����_�ڰ�����" },
	{ �Ҵ������ü���::��ͬ����, L"��ͬ����" },
	{ �Ҵ������ü���::�ɹ��ݾ�, L"�ɹ��ݾ�" },
	{ �Ҵ������ü���::����Ա, L"����Ա" },
	{ �Ҵ������ü���::���Ѽ���, L"���Ѽ���" },
	{ �Ҵ������ü���::Я�ֺ���, L"Я�ֺ���" },
	{ �Ҵ������ü���::���˾���, L"���˾���" },
	{ �Ҵ������ü���::ʱ���Ҵ�, L"ʱ���Ҵ�" },
	{ �Ҵ������ü���::�������, L"�������" },
	{ �Ҵ������ü���::���ڸ�֪, L"���ڸ�֪" },
	{ �Ҵ������ü���::������, L"������" },
	{ �Ҵ������ü���::����������, L"����������" },
	{ �Ҵ������ü���::δ�����, L"δ�����" },
	{ �Ҵ������ü���::ĩ�ղб�, L"ĩ�ղб�" },
	{ �Ҵ������ü���::����֮��, L"����֮��" },
	{ �Ҵ������ü���::���Ϸ���, L"���Ϸ���" },
	{ �Ҵ������ü���::ë���Ȼ, L"ë���Ȼ" },
	{ �Ҵ������ü���::����ʧ��, L"����ʧ��" },
	{ �Ҵ������ü���::��������, L"��������" },
	{ �Ҵ������ü���::������, L"������" },
	{ �Ҵ������ü���::ԡѪ���, L"ԡѪ���" },
	{ �Ҵ������ü���::�����, L"�����" },
	{ �Ҵ������ü���::Ǳ������, L"Ǳ������" },
	{ �Ҵ������ü���::�������, L"�������" },
	{ �Ҵ������ü���::��Ȼһ��, L"��Ȼһ��" },
	{ �Ҵ������ü���::��ը����, L"��ը����" },
	{ �Ҵ������ü���::���Ծ���, L"���Ծ���" },
	{ �Ҵ������ü���::�����빲, L"�����빲" },
	{ �Ҵ������ü���::���۲���, L"���۲���" },
	{ �Ҵ������ü���::�ý�����, L"�ý�����" },
	{ �Ҵ������ü���::������ʩ, L"������ʩ" },
	{ �Ҵ������ü���::�ƻ���, L"�ƻ���" },
	{ �Ҵ������ü���::�ư�����, L"�ư�����" },
	{ �Ҵ������ü���::��_֯��֩��, L"��_֯��֩��" },
	{ �Ҵ������ü���::��_�ն���ѻ, L"��_�ն���ѻ" },
	{ �Ҵ������ü���::�Ȳ�ʤȯ, L"�Ȳ�ʤȯ" },
	{ �Ҵ������ü���::������, L"������" },
	{ �Ҵ������ü���::���ɽ��, L"���ɽ��" },
	{ �Ҵ������ü���::���аٲ�, L"���аٲ�" },
	{ �Ҵ������ü���::��������, L"��������" },
	{ �Ҵ������ü���::�����Բ�, L"�����Բ�" },
	{ �Ҵ������ü���::�ϱ���, L"�ϱ���" },
	{ �Ҵ������ü���::��������, L"��������" },
	{ �Ҵ������ü���::�۾�����, L"�۾�����" },
	{ �Ҵ������ü���::�縺�ص�, L"�縺�ص�" },
	{ �Ҵ������ü���::��������, L"��������" },
	{ �Ҵ������ü���::ʤ��ϲ��, L"ʤ��ϲ��" },
	{ �Ҵ������ü���::����Ұ��, L"����Ұ��" },
	{ �Ҵ������ü���::��ѧ�ɲ�, L"��ѧ�ɲ�" },
	{ �Ҵ������ü���::���ұ���, L"���ұ���" },
	{ �Ҵ������ü���::��������, L"��������" },
	{ �Ҵ������ü���::�ἺΪ��, L"�ἺΪ��" },
	{ �Ҵ������ü���::��ҫʱ��, L"��ҫʱ��" },
	{ �Ҵ������ü���::ҩ������, L"ҩ������" },
	{ �Ҵ������ü���::���ƴ���, L"���ƴ���" },
	{ �Ҵ������ü���::����, L"����" },
	{ �Ҵ������ü���::��ֱ̽��, L"��ֱ̽��" },
	{ �Ҵ������ü���::�϶�Ŀ��, L"�϶�Ŀ��" },
	{ �Ҵ������ü���::֤���Լ�, L"֤���Լ�" },
	{ �Ҵ������ü���::��������, L"��������" },
	{ �Ҵ������ü���::��������֮��, L"��������֮��" },
	{ �Ҵ������ü���::��崾���, L"��崾���" },
	{ �Ҵ������ü���::ת��ע��, L"ת��ע��" },
	{ �Ҵ������ü���::�䶯�ݳ�, L"�䶯�ݳ�" },
	{ �Ҵ������ü���::�����ë, L"�����ë" },
	{ �Ҵ������ü���::��ӯ����, L"��ӯ����" },
	{ �Ҵ������ü���::��ӯè��, L"��ӯè��" },
	{ �Ҵ������ü���::Զ��׿ʶ, L"Զ��׿ʶ" },
	{ �Ҵ������ü���::�������, L"�������" },
	{ �Ҵ������ü���::��֮زز, L"��֮زز" },
	{ �Ҵ������ü���::а��֮��, L"а��֮��" },
	{ �Ҵ������ü���::��������, L"��������" },
	{ �Ҵ������ü���::���ѣ�����, L"���ѣ�����" },
	{ �Ҵ������ü���::�ֽ�����, L"�ֽ�����" },
	{ �Ҵ������ü���::������־, L"������־" },
	{ �Ҵ������ü���::�������, L"�������" },
	{ �Ҵ������ü���::��Ӱ����, L"��Ӱ����" },
	{ �Ҵ������ü���::���ⵯ, L"���ⵯ" },
	{ �Ҵ������ü���::����֮��, L"����֮��" },
	{ �Ҵ������ü���::��������, L"��������" },
	{ �Ҵ������ü���::����ʽ����, L"����ʽ����" },
	{ �Ҵ������ü���::����Ⱥ��, L"����Ⱥ��" },
	{ �Ҵ������ü���::���ɹ�, L"���ɹ�" },
	{ �Ҵ������ü���::��ǣ����, L"��ǣ����" },
	{ �Ҵ������ü���::��Ѫ��Լ, L"��Ѫ��Լ" },
	{ �Ҵ������ü���::����Э��, L"����Э��" }
};


