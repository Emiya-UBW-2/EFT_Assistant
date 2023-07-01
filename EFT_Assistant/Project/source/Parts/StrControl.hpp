#pragma once
#include"../Header.hpp"

namespace FPS_n2 {
	//
	static const auto		SubStrs(std::string* Target, const char* str) {
		while (true) {
			auto now = Target->find(str);
			if (now != std::string::npos) {
				*Target = Target->substr(0, now) + Target->substr(now + strlenDx(str));
			}
			else {
				break;
			}
		}
	};
	//
	static const auto		ReplaceStrs(std::string* Target, const char* str, const char* str2) {
		auto now = Target->find(str);
		while (true) {
			if (now != std::string::npos) {
				Target->replace(now, strlenDx(str), str2);
				now = Target->find(str, now + strlenDx(str2));
			}
			else {
				break;
			}
		}
	};
	//
	static const auto		SubRIGHTStrs(std::string* Target, const char* str) {
		auto now = Target->find(str);
		if (now != std::string::npos) {
			*Target = Target->substr(0, now);
		}
	};
	//
	static const auto		GetArgs(const std::string& RIGHT) noexcept {
		std::vector<std::string> Args;
		auto L = RIGHT.find("[");
		auto R = RIGHT.find("]");
		if (L != std::string::npos && R != std::string::npos) {
			std::string RIGHTBuf = RIGHT;
			RIGHTBuf = RIGHTBuf.substr(L + 1);
			while (true) {
				auto div = RIGHTBuf.find(DIV_STR);
				if (div != std::string::npos) {
					Args.emplace_back(RIGHTBuf.substr(0, div));
					RIGHTBuf = RIGHTBuf.substr(div + 2);
				}
				else {
					Args.emplace_back(RIGHTBuf.substr(0, RIGHTBuf.find("]")));
					break;
				}
			}
		}
		else {
			Args.emplace_back(RIGHT);
		}
		return Args;
	}
	//
	static const auto		UTF8toSjis(std::string srcUTF8) {
		//Unicodeへ変換後の文字列長を得る
		int lenghtUnicode = (int)MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), (int)(srcUTF8.size()) + 1, NULL, 0);

		//必要な分だけUnicode文字列のバッファを確保
		wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

		//UTF8からUnicodeへ変換
		MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), (int)(srcUTF8.size()) + 1, bufUnicode, lenghtUnicode);

		//ShiftJISへ変換後の文字列長を得る
		int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

		//必要な分だけShiftJIS文字列のバッファを確保
		char* bufShiftJis = new char[lengthSJis];

		//UnicodeからShiftJISへ変換
		WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

		std::string strSJis(bufShiftJis);

		delete bufUnicode;
		delete bufShiftJis;

		return strSJis;
	}
};
