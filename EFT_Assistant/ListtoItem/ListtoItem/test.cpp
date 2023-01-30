#define NOMINMAX

#include "DxLib.h"

#include <vector>
#include <string>
#include <fstream>

static const int draw_x = 960;			//ウィンドウサイズX
static const int draw_y = 720;			//ウィンドウサイズY

static const int tile_size = 64;		//表示上のタイルサイズ
static const int tile_pic_size = 32;	//画像のタイルサイズ
//ベクトルのxy間の角度を取得
static const auto GetRadVec2Vec(const VECTOR& vec1, const VECTOR& vec2) { return std::atan2f(vec1.x - vec2.x, vec1.y - vec2.y); }
//角度をラジアンに変換
static const auto Deg2Rad(float value) { return value * DX_PI_F / 180.f; }

//いろいろまとめるクラス
class DXDraw {
public:
	//コンストラクタ
	DXDraw(void) noexcept {
		SetOutApplicationLogValidFlag(FALSE);           /*log*/
		SetMainWindowText("ListtoItem");                /*タイトル*/
		ChangeWindowMode(TRUE);                         /*窓表示*/
		SetUseDirect3DVersion(DX_DIRECT3D_11);          /*directX ver*/
		SetGraphMode(draw_x, draw_y, 32);               /*解像度*/
		SetUseDirectInputFlag(TRUE);                    /*DirectInput使用*/
		SetDirectInputMouseMode(TRUE);                  /*DirectInputマウス使用*/
		SetWindowSizeChangeEnableFlag(FALSE, FALSE);    /*ウインドウサイズを手動変更不可、ウインドウサイズに合わせて拡大もしないようにする*/
		SetUsePixelLighting(TRUE);                      /*ピクセルライティングの使用*/
		SetFullSceneAntiAliasingMode(4, 2);             /*アンチエイリアス*/
		SetWaitVSyncFlag(TRUE);                         /*垂直同期*/
		DxLib_Init();                                   /*ＤＸライブラリ初期化処理*/
		SetSysCommandOffFlag(TRUE);                     /*タスクスイッチを有効にするかどうかを設定する*/
		SetAlwaysRunFlag(TRUE);                         /*background*/
	}
	//デストラクタ
	~DXDraw(void) noexcept {
		DxLib_End();
	}
};

class getparams {
	static const int charLength = 1024;
public:
	//文字列から=より右の値取得
	static const std::string getleft(std::string tempname) { return tempname.substr(0, tempname.find('=')); }
	//文字列から=より右の値取得
	static const std::string getright(std::string tempname) { return tempname.substr(tempname.find('=') + 1); }
public:
	//左辺の情報もとる
	static const std::string		getcmd(int p1, int *p2) {
		char mstr[charLength]; /*tank*/
		FileRead_gets(mstr, charLength, p1);
		*p2 = std::stoi(getright(mstr));
		return getleft(mstr);
	}
	//1行そのまま取得
	static auto						Getstr(int p1) {
		char mstr[charLength] = "";
		FileRead_gets(mstr, charLength, p1);
		return std::string(mstr);
	}
	//=より右の値取得
	static const std::string		_str(int p1) {
		char mstr[charLength] = "";
		FileRead_gets(mstr, charLength, p1);
		return getright(mstr);
	}
	static const long int			_int(int p1) {
		char mstr[charLength] = "";
		FileRead_gets(mstr, charLength, p1);
		return std::stoi(getright(mstr));
	}
	static const long int			_long(int p1) {
		char mstr[charLength] = "";
		FileRead_gets(mstr, charLength, p1);
		return std::stol(getright(mstr));
	}
	static const unsigned long int	_ulong(int p2) {
		char mstr[charLength] = "";
		FileRead_gets(mstr, charLength, p2);
		return std::stoul(getright(mstr));
	}
	static const float				_float(int p1) {
		char mstr[charLength] = "";
		FileRead_gets(mstr, charLength, p1);
		return std::stof(getright(mstr));
	}
	static const bool				_bool(int p1) {
		char mstr[charLength] = "";
		FileRead_gets(mstr, charLength, p1);
		return (getright(mstr).find("true") != std::string::npos);
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	auto maincontrol = std::make_unique<DXDraw>();

	std::vector<std::vector<std::string>> Files;

	{
		int mdata = FileRead_open("data.txt", FALSE);
		while (true) {
			if (FileRead_eof(mdata) != 0) { break; }
			auto ALL = getparams::Getstr(mdata);
			if (ALL == "") { continue; }
			if (ALL.find("=") == std::string::npos) {
				if (Files.size() != 0) {
					Files.back().emplace_back("");
				}
				Files.resize(Files.size() + 1);
			}
			Files.back().emplace_back(ALL);
		}
		FileRead_close(mdata);
	}
	for (auto& f : Files) {
		std::string FileName = f[0];
		while (true) {
			auto SpacePlace = FileName.find(" ");
			if (SpacePlace != std::string::npos) {
				FileName = FileName.substr(0, SpacePlace) + "_" + FileName.substr(SpacePlace + 1);
			}
			else {
				break;
			}
		}
		if (FileName == "#ERROR!") {
			int o = 0;
		}

		std::string path = "result/" + FileName + ".txt";
		std::ofstream outputfile(path.c_str());

		outputfile << "Name" + f[1] + "\n";
		outputfile << "Itemtype=Receiver\n";
		if (f[2] != "=") {
			outputfile << "Recoil" + f[2] + "\n";
		}
		else {
			outputfile << "Recoil=0\n";
		}
		if (f[3] != "=") {
			outputfile << "Ergonomics" + f[3] + "\n";
		}
		else {
			outputfile << "Ergonomics=0\n";
		}
		//outputfile << "Name" + f[1] + "\n";
		outputfile.close();
	}
	//メインループ開始
	while (ProcessMessage() == 0) {
		LONGLONG WaitTime = GetNowHiPerformanceCount();//現時点の経過秒数を得る
		//描画
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		{
			//デバッグ表示
			clsDx();
			printfDx("FPS:%4.1f\n", GetFPS());
		}
		ScreenFlip();
	}
	return 0;// ソフトの終了 
}