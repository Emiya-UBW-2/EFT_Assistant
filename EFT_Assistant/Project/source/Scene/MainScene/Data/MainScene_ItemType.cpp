#include"../../../Header.hpp"

namespace FPS_n2 {
	void	ItemTypeList::Load_Sub() noexcept {
		m_CategoryID = DataBase::Instance()->GetItemCategoryData()->FindID(m_CategoryBuffer.c_str());
	}
};
