#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class PresetList : public ListParent<PresetID> {
		const ItemList*											m_Base;
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
		void	Load_Sub() noexcept override {
			m_Base = ItemData::Instance()->FindPtr(ItemData::Instance()->FindID(m_BaseIDBuffer.c_str()));
			for (auto& P : m_PartsIDBuffer) {
				m_Parts.emplace_back(ItemData::Instance()->FindPtr(ItemData::Instance()->FindID(P.c_str())));
			}
		}
		void	WhenAfterLoad_Sub() noexcept override {}
	public:
		const auto&	GetBase() const noexcept { return m_Base; }
		const auto&	GetParts() const noexcept { return m_Parts; }
	};
	class PresetData : public SingletonBase<PresetData>, public DataParent<PresetID, PresetList> {
	private:
		friend class SingletonBase<PresetData>;
	private:
		PresetData() noexcept {
			std::string Path = "data/Preset/";
			GetDirList(Path.c_str(), [&](const char* RetPath2) {
				SetDirList((Path + RetPath2 + "/").c_str());
			});
		}
		~PresetData() noexcept {}
	};
};
