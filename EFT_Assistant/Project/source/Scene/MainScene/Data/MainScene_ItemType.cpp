#include"MainScene_ItemType.hpp"
#include "../Data/MainScene_Base.hpp"
#include "../../../Parts/WindowParts.hpp"
#include "../../../Parts/StrControl.hpp"
#include "../../../Parts/DrawSystem.hpp"
#include "../../../Parts/InputParts.hpp"
#include "../../../Parts/PlayerDataParts.hpp"
#include "../../../Parts/InterruptParts.hpp"

namespace FPS_n2 {
	void	ItemTypeList::Load_Sub() noexcept {
		m_CategoryID = DataBase::Instance()->GetItemCategoryData()->FindID(m_CategoryBuffer.c_str());
	}
};
