#include "MainScene_ItemType.hpp"
#include "../Data/MainScene_Base.hpp"
#include "../../../PartsHeader.hpp"

namespace FPS_n2 {
	void	ItemTypeList::Load_Sub() noexcept {
		m_CategoryID = DataBase::Instance()->GetItemCategoryData()->FindID(m_CategoryBuffer.c_str());
	}
};
