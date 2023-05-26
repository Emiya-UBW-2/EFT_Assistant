#pragma once
#include"../../../Header.hpp"

namespace FPS_n2 {
	class ItemCategoryList : public ListParent<ItemCategoryID> {
		//í«â¡ê›íË
		void			SetSub(const std::string&, const std::string&, const std::vector<std::string>&) noexcept override {}
	};
	class ItemCategoryData : public SingletonBase<ItemCategoryData>, public DataParent<ItemCategoryID, ItemCategoryList> {
	private:
		friend class SingletonBase<ItemCategoryData>;
	private:
		ItemCategoryData() noexcept {
			SetList("data/itemcategory/");
		}
		~ItemCategoryData() noexcept {}
	};
};