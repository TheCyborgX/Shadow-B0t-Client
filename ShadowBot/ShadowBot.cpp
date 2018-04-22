#include "main.hpp"
#include <stdio.h>
#include <detours.h>
#pragma warning(disable: 4996)



BYTE ShadowBot::ID[4];
//BYTE ShadowBot::HWID[12];
BYTE ShadowBot::LOCALIP[4];
DWORD* ShadowBot::SECONDARY_HWID_ADDRESS;
BYTE* ShadowBot::XUID_ADDRESS;
BYTE* ShadowBot::PRIMARY_ID_ADDRESS;
BYTE* ShadowBot::STEAMID_ADDRESS;
xrand ShadowBot::gen;
int ShadowBot::LocalIPBlockType;


void ShadowBot::GenerateNewID()
{	
	gen.GetRandBytesInVector(ID, 4, BYTE_MIN_VALUE, BYTE_MAX_VALUE);
}

/*
void ShadowBot::GenerateNewHWID()
{
	gen.GetRandBytesInVector(HWID, 12, BYTE_MIN_VALUE, BYTE_MAX_VALUE);
}
*/

void ShadowBot::GenerateNewLocalIP()
{
	if (LocalIPBlockType == 1)
		LOCALIP[0] = 0xC0, LOCALIP[1] = 0xA8, gen.GetRandBytesInVector(&LOCALIP[2], 2, BYTE_MIN_VALUE, BYTE_MAX_VALUE);
	else if (LocalIPBlockType == 2)
		LOCALIP[0] = 0xA, gen.GetRandBytesInVector(&LOCALIP[1], 3, BYTE_MIN_VALUE, BYTE_MAX_VALUE);
	else if (LocalIPBlockType == 3)
		LOCALIP[0] = 0xAC, LOCALIP[1] = gen.GetRandByte(16, 31), gen.GetRandBytesInVector(&LOCALIP[2], 2, BYTE_MIN_VALUE, BYTE_MAX_VALUE);
}



int ShadowBot::GetLocalIPBlockType()
{
	void* ptrthis;
	BYTE vec[128]; // 72

	((void(__cdecl*)(int ptr))0x43B370)((int)&ptrthis);
	((void*(__thiscall*)(void* ptr, void* vec))0x6EC2A0)(ptrthis, vec);
	
	if (vec[0] == 0xC0 && vec[1] == 0xA8) // 192 - 168 - x - x
		return 1;
	else if (vec[0] == 0xA && (vec[1] >= 0x0 && vec[1] <= 0xFF)) // 24 bit block type...
		return 2;
	else if (vec[0] == 0xAC && (vec[1] >= 0x10 && vec[1] <= 0x1F)) // 20 bit block type...
		return 3;

	return 0;
}


/*

int(*PGetFinalFormatedData)(char* buff, size_t sz, char* format, ...) = (int(*)(char*, size_t, char*, ...))0x450CF0;

int ShadowBot::GetFomratedData(char* buff, size_t sz, char* format, ...)
{
	// do magic stuffs...
	

	int n;
	va_list va;
	va_start(va, format);
	n = _vsnprintf(buff, sz, format, va);


	if (strstr(buff, "connect"))
		MessageBoxA(NULL, buff, "Sending formted data: ", MB_OK | MB_ICONASTERISK);


	buff[sz - 1] = 0;
	return n;
}
*/
//bool(__cdecl*GetHWID)(DWORD* hwid) = (bool(__cdecl*)(DWORD*))0x1001A700;



// init GetSystemFirmwareTable hook...
		
UINT(WINAPI*OrgGetSystemFirmwareTable)(
	DWORD FirmwareTableProviderSignature,
	DWORD FirmwareTableID,
	PVOID pFirmwareTableBuffer,
	DWORD BufferSize) = GetSystemFirmwareTable;

UINT WINAPI NewGetSystemFirmwareTable(DWORD FirmwareTableProviderSignature, DWORD FirmwareTableID, PVOID pFirmwareTableBuffer, DWORD BufferSize)
{
	if (pFirmwareTableBuffer == NULL)
		return OrgGetSystemFirmwareTable(FirmwareTableProviderSignature, FirmwareTableID, pFirmwareTableBuffer, BufferSize);

	UINT lenwritted = OrgGetSystemFirmwareTable(FirmwareTableProviderSignature, FirmwareTableID, pFirmwareTableBuffer, BufferSize);



	BYTE* pbuff = (BYTE*)pFirmwareTableBuffer;

	RawSMBIOSData* pRawSMBIOSData = (RawSMBIOSData*)pFirmwareTableBuffer;




	while (pbuff - (BYTE*)pFirmwareTableBuffer < pRawSMBIOSData->Length)
	{
		SMBiosStructHeader* head = (SMBiosStructHeader*)pbuff;


		pbuff += head->length;

		for (int it = 0; it < pRawSMBIOSData->Length; it++) // i dont like the infinite bucles...
		{


			const static BYTE matcherx[2] = { 0x0, 0x0 };

			if (!memcmp(pbuff, matcherx, 2)) // new start struct found!
			{
				pbuff += 2;
				break;
			}
			pbuff++;


		}

		// do the magic alteration
		static xrand gen;
		static AsciiTypes bytetype;

		switch (head->type)
		{
		case SYSTEM_INFO_TYPE:
		{
			int len = pbuff - (BYTE*)head - 1;
			BYTE* byteval = (BYTE*)head;
			// change magic values
			for (int i = 0; i < len; i++)
				if (i != 0 && i != (len - 1))
					if (bytetype.IsChar(byteval[i]))
						byteval[i] = gen.GetRandCharInByte();
					else if (bytetype.IsInt(byteval[i]))
						byteval[i] = gen.GetRandIntInByte();
		
			break;
		}
		case BASEBOARD_INFO_TYPE:
		{
			int len = pbuff - (BYTE*)head - 1;
			BYTE* byteval = (BYTE*)head;
			// change magic values
			for (int i = 0; i < len; i++)
				if (i != 0 && i != (len - 1))
					if (bytetype.IsChar(byteval[i]))
						byteval[i] = gen.GetRandCharInByte();
					else if (bytetype.IsInt(byteval[i]))
						byteval[i] = gen.GetRandIntInByte();
			
			break;
		}
		case PROCESSOR_INFO_TYPE:
		{
			int len = pbuff - (BYTE*)head - 1;
			BYTE* byteval = (BYTE*)head;
			// change magic values
			for (int i = 0; i < len; i++)
				if (i != 0 && i != (len - 1))
					if (bytetype.IsChar(byteval[i]))
						byteval[i] = gen.GetRandCharInByte();
					else if (bytetype.IsInt(byteval[i]))
						byteval[i] = gen.GetRandIntInByte();
			
			break;
		}
		case OEMSTRING_INFO_TYPE: // this doesn't nothing but just in case...
		{
			// k
			break;
		}

		default:
			break;
		}

	}




	return lenwritted;
}




typedef void(__stdcall **** ret_T)(DWORD);

void(__stdcall****(__cdecl* OrgGetXNAA)(int a1))(DWORD) = (void(__stdcall****(__cdecl* )(int))(DWORD))0x43B370;

void(__stdcall ****__cdecl NewGetXNAA(int a1))(DWORD)
{
	ret_T a = OrgGetXNAA(a1);
	
	// change the magic values....
	memcpy(&((BYTE*)*(DWORD*)a1)[20], ShadowBot::ID, 4);



	return a;
}



/*
void __cdecl NewGetRealXNAA(int a1)
{


	// get the real xnaa
	//OrgGetRealXNAA(a1);
	//	char buff[128];

	// alther the real data
	// XNAA

	//((BYTE*)*(DWORD*)a1)[1] = 60;

	// UID


//	const BYTE idvec[4] = { 40, 20, 40, 40 };
	
	((BYTE*)*(DWORD*)a1)[0] = 40;
	((BYTE*)*(DWORD*)a1)[1] = 20;
	((BYTE*)*(DWORD*)a1)[2] = 40;
	((BYTE*)*(DWORD*)a1)[3] = 40;
	



	//memcpy(&((BYTE*)*(DWORD*)a1)[20], ShadowBot::ID, 4);


//	sprintf_s(buff, sizeof(buff), "Value: %x", a1);
//	MessageBoxA(NULL, buff, "Success!", MB_OK | MB_ICONASTERISK);

}
*/
// this looks something like a byte packer, i not want waste my time reversing this, it haven't sense...
void*(__fastcall* OrgByteDataPacker)(void* pThis, void* thisx, void* outbuff) = (void*(__fastcall*)(void*, void*, void*))0x6EC2A0;

void* __fastcall NewByteDataPacker(void* pThis, void* thisx, void* outbuff)
{

	void* rtptr = OrgByteDataPacker(pThis, thisx, outbuff);

	memcpy(outbuff, ShadowBot::LOCALIP, 4);

	return rtptr;
}

// updated to teknomw3 2.8.0.2
bool(*OrgGetHWID)(DWORD*) = (bool(*)(DWORD*))0x10018860; //0x1001A700;

bool NewGetHWID(DWORD* hwid)
{
	bool result = OrgGetHWID(hwid);

	DWORD hwidencypted[3] = { hwid[1], hwid[2], hwid[3] };

	hwidencypted[0] ^= 0xDEADDEAD;
	hwidencypted[1] ^= 0xDEADDEAD;
	hwidencypted[2] ^= 0xDEADDEAD;

	// send a valid hwid
	ShadowBot::ChangeSecondaryHWID(hwidencypted);
	
	return result;
}




void ShadowBot::ChangeID()
{
	memcpy(XUID_ADDRESS, ID, 4);
	memcpy(PRIMARY_ID_ADDRESS, ID, 4);
	memcpy(STEAMID_ADDRESS, ID, 4);
}

/* // disabled because to xor encryption fix check
void ShadowBot::ChangeHWID()
{
	memcpy(SECONDARY_HWID_ADDRESS, HWID, 12);
}
*/

void ShadowBot::ChangeSecondaryHWID(DWORD* hwid)
{
	SECONDARY_HWID_ADDRESS[0] = hwid[0];
	SECONDARY_HWID_ADDRESS[1] = hwid[1];
	SECONDARY_HWID_ADDRESS[2] = hwid[2];
}


void ShadowBot::Init()
{
	LocalIPBlockType = GetLocalIPBlockType();
	SECONDARY_HWID_ADDRESS = (DWORD*)0x1CDDFAC;
	XUID_ADDRESS = (BYTE*)0x5A7B1D8;
	PRIMARY_ID_ADDRESS = (BYTE*)0x1CDDB75;
	STEAMID_ADDRESS = (BYTE*)(*(DWORD*)0x5CCB138 + 0x13A);
}


BYTE(*ConnectStage)() = (BYTE(*)())0x552820;


BYTE OnConnectStage()
{
	// do the magic stuffs...
	//ShadowBot::GenerateNewHWID();
	ShadowBot::GenerateNewLocalIP();
	ShadowBot::GenerateNewID();
	ShadowBot::ChangeID();

	return ConnectStage();
}


void Init()
{
	*(DWORD*)0x1004FC6C = (DWORD)"^5ShadowBot client";

	*(DWORD*)0x4A36E1 = (DWORD)"^6Status: ^2Waiting...";



	while (!*(BYTE*)0x10051EFE) Sleep(1);

	ShadowBot::Init();

	
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach(&(PVOID&)OrgGetSystemFirmwareTable, NewGetSystemFirmwareTable);

	DetourAttach(&(PVOID&)OrgGetHWID, NewGetHWID);

	DetourAttach(&(PVOID&)OrgGetXNAA, NewGetXNAA);

	DetourAttach(&(PVOID&)OrgByteDataPacker, NewByteDataPacker);

	DetourAttach(&(PVOID&)ConnectStage, OnConnectStage);


	if (DetourTransactionCommit() != NO_ERROR)
		return;

	*(DWORD*)0x4A36E1 = (DWORD)"^6Status: ^2Success!";

	Sleep(4000);

	*(DWORD*)0x4A36E1 = (DWORD)"^6Game: ^2%s.%i ^6Tekno: ^22.8.0.2";

	//MessageBox(NULL, L"GetSystemFirmwareTable(); hook success!", L"Success!", MB_OK | MB_ICONASTERISK);

}