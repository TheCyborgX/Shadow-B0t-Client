
// app Entry point

#define SYSTEM_INFO_TYPE 0x1
#define BASEBOARD_INFO_TYPE 0x2
#define PROCESSOR_INFO_TYPE 0x4
#define OEMSTRING_INFO_TYPE 0xB


typedef struct tagRawSMBIOSData
{
	BYTE	Used20CallingMethod;
	BYTE	SMBIOSMajorVersion;
	BYTE	SMBIOSMinorVersion;
	BYTE	DmiRevision;
	DWORD	Length;
	BYTE	SMBIOSTableData[];
} RawSMBIOSData, *PRawSMBIOSData;

typedef struct tagSMBiosStructHeader
{
	BYTE type;
	BYTE length;
	WORD handle;
	BYTE data[];
} SMBiosStructHeader, *PSMBiosStructHeader;


void Init();



class ShadowBot
{
public:
	static void Init();

	static xrand gen;
	static BYTE ID[4];
//	static BYTE HWID[12];
	static BYTE LOCALIP[4];
	static int LocalIPBlockType;
	static void GenerateNewID();
	static void ChangeID();
	static void GenerateNewHWID();
	static void ChangeSecondaryHWID(DWORD* hwid);
	static void GenerateNewLocalIP();
	
	
private:
	static int GetLocalIPBlockType();

	static DWORD* SECONDARY_HWID_ADDRESS;
	static BYTE* XUID_ADDRESS;
	static BYTE* STEAMID_ADDRESS;
	static BYTE* PRIMARY_ID_ADDRESS;
	static BYTE* SECUNDARY_ID_ADDRESS;
};