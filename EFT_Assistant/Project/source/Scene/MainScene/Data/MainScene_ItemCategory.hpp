#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class ItemCategoryList : public ListParent<ItemCategoryID> {};
	class ItemCategoryData : public DataParent<ItemCategoryID, ItemCategoryList> {
	public:
		ItemCategoryData() noexcept {
			SetDirList("data/itemcategory/");
		}
		~ItemCategoryData() noexcept {}
	};
};
