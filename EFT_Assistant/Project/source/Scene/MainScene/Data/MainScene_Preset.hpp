#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	typedef int PresetID;


	class PresetList : public ListParent<PresetID> {
		const ItemList*											m_Base;
		std::vector<const ItemList*>							m_Parts;
	private:
		//í«â¡ê›íË
		void			Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>&) noexcept override {
			if (LEFT == "Base") {
				auto ID = ItemData::Instance()->FindID(RIGHT.c_str());
				if (ID != InvalidID) {
					m_Base = ItemData::Instance()->FindPtr(ID);
				}
			}
			if (LEFT == "Parts") {
				auto ID = ItemData::Instance()->FindID(RIGHT.c_str());
				if (ID != InvalidID) {
					m_Parts.emplace_back(ItemData::Instance()->FindPtr(ID));
				}
			}
		}
		void	Load_Sub() noexcept override {
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
			auto data_t = GetFileNamesInDirectory(Path.c_str());
			for (auto& d : data_t) {
				if (d.cFileName[0] != '.') {
					SetList((Path + d.cFileName + "/").c_str());
				}
			}
		}
		~PresetData() noexcept {}
	};
};