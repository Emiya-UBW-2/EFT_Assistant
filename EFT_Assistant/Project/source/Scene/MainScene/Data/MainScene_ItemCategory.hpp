#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class ItemCategoryList : public ListParent<ItemCategoryID> {
		void			SetSub(const std::string&, const std::vector<std::string>&) noexcept override {}
	};
	class ItemCategoryData : public DataParent<ItemCategoryID, ItemCategoryList> {
	public:
		ItemCategoryData() noexcept {
			SetDirList("data/itemcategory/");
		}
		~ItemCategoryData() noexcept {}
	};
};
