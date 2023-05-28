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
};
