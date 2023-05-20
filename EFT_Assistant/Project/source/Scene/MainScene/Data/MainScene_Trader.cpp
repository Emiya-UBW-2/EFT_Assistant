#include"../../../Header.hpp"

namespace FPS_n2 {
	void			SetTraderLv(std::vector<TraderGetData>* Data, const std::string& mes) noexcept {
		auto L = mes.rfind("x");
		if (L != std::string::npos) {
			auto ID = mes.substr(0, L);
			if (std::find_if(Data->begin(), Data->end(), [&](const TraderGetData& obj) {return obj.GetID() == TraderData::Instance()->FindID(ID.c_str()); }) == Data->end()) {
				TraderGetData tmp;
				tmp.Set(ID, std::stoi(mes.substr(L + 1)));
				Data->emplace_back(tmp);
			}
		}
		else {
			//int a = 0;
		}
	};

};