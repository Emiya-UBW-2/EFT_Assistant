#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class ItemTypeList : public ListParent<ItemTypeID> {
		ItemCategoryID	m_CategoryID{ InvalidID };
	private:
		std::string		m_CategoryBuffer;
	private:
		//í«â¡ê›íË
		void			SetSub(const std::string& LEFT, const std::vector<std::string>& Args) noexcept override {
			if (LEFT == "Category") { this->m_CategoryBuffer = Args[0]; }
		}
		void	Load_Sub() noexcept override;
		void	WhenAfterLoad_Sub() noexcept override {}
	public:
		const auto&	GetCategoryID() const noexcept { return this->m_CategoryID; }
	};
	class ItemTypeData : public DataParent<ItemTypeID, ItemTypeList> {
	public:
		ItemTypeData() noexcept {
			std::string Path = "data/itemtype/";
			GetDirList(Path.c_str(), [&](const char* RetPath2) {
				SetDirList((Path + RetPath2 + "/").c_str());
			});
		}
		~ItemTypeData() noexcept {}
	};
};