#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	typedef int ItemTypeID;
	class ItemTypeList : public ListParent<ItemTypeID> {
		ItemCategoryID	m_CategoryID{ InvalidID };
	private:
		//í«â¡ê›íË
		void			Set_Sub(const std::string& LEFT, const std::string& RIGHT, const std::vector<std::string>&) noexcept override {
			if (LEFT == "Category") {
				m_CategoryID = ItemCategoryData::Instance()->FindID(RIGHT.c_str());
			}
		}
		void	Load_Sub() noexcept override {
		}
		void	WhenAfterLoad_Sub() noexcept override {
		}
	public:
		const auto&	GetCategoryID() const noexcept { return m_CategoryID; }
	};
	class ItemTypeData : public SingletonBase<ItemTypeData>, public DataParent<ItemTypeID, ItemTypeList> {
	private:
		friend class SingletonBase<ItemTypeData>;
	private:
		ItemTypeData() noexcept {
			std::string Path = "data/itemtype/";
			auto data_t = GetFileNamesInDirectory(Path.c_str());
			for (auto& d : data_t) {
				if (d.cFileName[0] != '.') {
					SetList((Path + d.cFileName + "/").c_str());
				}
			}
		}
		~ItemTypeData() noexcept {}
	};
};