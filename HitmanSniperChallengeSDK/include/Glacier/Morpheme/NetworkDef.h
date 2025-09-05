#pragma once

#include "NodeDef.h"

#include "Common.h"

namespace MR
{
	class NetworkDef
	{
	public:
		PAD(0x14);
		unsigned int m_numNodeDefs;
		unsigned int m_numDummyBlendTreeDefs;
		NodeDef** m_nodeDefs;
	};
}
