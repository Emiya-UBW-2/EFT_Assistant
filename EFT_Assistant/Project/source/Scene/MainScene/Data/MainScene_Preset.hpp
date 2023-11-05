#pragma once
#include "../../../Header.hpp"
#include "../Data/MainScene_Common.hpp"
#include "../Page/MainScene_PageBase.hpp"
#include "MainScene_Item.hpp"

namespace FPS_n2 {
	class PresetList : public ListParent<PresetID> {
		const ItemList*											m_Base{ nullptr };
		std::vector<const ItemList*>							m_Parts;
	private:
		std::string												m_BaseIDBuffer;
		std::vector<std::string>								m_PartsIDBuffer;
	private:
		//í«â¡ê›íË
		void			SetSub(const std::string& LEFT, const std::vector<std::string>& Args) noexcept override {
			if (LEFT == "Base") {
				m_BaseIDBuffer = Args[0];
			}
			if (LEFT == "Parts") {
				m_PartsIDBuffer.emplace_back(Args[0]);
			}
		}
		void	Load_Sub() noexcept override;
		void	WhenAfterLoad_Sub() noexcept override {}
	public:
		const auto&	GetBase() const noexcept { return this->m_Base; }
		const auto&	GetParts() const noexcept { return this->m_Parts; }
	};
	class PresetData : public DataParent<PresetID, PresetList> {
	public:
		PresetData() noexcept {
			std::string Path = "data/Preset/";
			GetDirList(Path.c_str(), [&](const char* RetPath2) {
				SetDirList((Path + RetPath2 + "/").c_str());
			});
		}
		~PresetData() noexcept {}
	};
};
