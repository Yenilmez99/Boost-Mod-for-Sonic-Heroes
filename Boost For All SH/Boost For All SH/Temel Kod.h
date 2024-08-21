#pragma once
using namespace std;

int TemelKod() {
	bool PauseBilgi;
	float Hiz, AnaHiz;
	float HizTick, AnaHizTick;

	while (true){

		if (GetAsyncKeyState(VK_F2) != 0){
			break;
		}

		else{

		}

	}

	HWND hwnd_SonicHeroesTM = FindWindowA(NULL, "SONIC HEROES(TM)");
	DWORD ProcessIDSonicHeroes = NULL;
	GetWindowThreadProcessId(hwnd_SonicHeroesTM, &ProcessIDSonicHeroes);
	HANDLE HandleSonicHeroes = NULL;
	HandleSonicHeroes = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessIDSonicHeroes);

	char SonicHeroesGameModule[] = "Tsonic_win.exe";
	DWORD SonicHeroesAnaAdress = GetModuleBaseAddress(_T(SonicHeroesGameModule), ProcessIDSonicHeroes);

	DWORD PauseAdresi = 0x000041F4;
	vector<DWORD> PauseOffseti{ 0x0 };
	DWORD KullanPause = GetPointerAddress(hwnd_SonicHeroesTM, SonicHeroesAnaAdress, PauseAdresi, PauseOffseti);

	while (true){
		ReadProcessMemory(HandleSonicHeroes, (PBYTE*)KullanPause, &PauseBilgi, sizeof(PauseBilgi), 0);

		if (PauseBilgi != 1) {

		}

		else{
			DWORD AnaHizAdresi = 0x005CE820;
			vector<DWORD> AnaHizOffseti{ 0xDC };
			DWORD KullanAnaHiz = GetPointerAddress(hwnd_SonicHeroesTM, SonicHeroesAnaAdress, AnaHizAdresi, AnaHizOffseti);

			ReadProcessMemory(HandleSonicHeroes, (PBYTE*)KullanAnaHiz, &AnaHiz, sizeof(AnaHiz), 0);
			
			if (AnaHiz != 0){
				
				if (GetAsyncKeyState(0x43)!=0){

					AnaHizTick = sqrt(2916* AnaHiz);
					HizTick = AnaHizTick + 1.0f;
					if (HizTick>=270){
						HizTick = 270;
					}

					else{

					}

					Hiz = HizTick*HizTick / (2916);
					WriteProcessMemory(HandleSonicHeroes, (PBYTE*)KullanAnaHiz, &Hiz, sizeof(Hiz), 0);

					Sleep(10);
				}
				
				else{

				}
			}

			else{

			}

		}

	}

	return 0;
}