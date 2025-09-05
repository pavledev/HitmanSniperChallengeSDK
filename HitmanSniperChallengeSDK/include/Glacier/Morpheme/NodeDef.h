#pragma once

namespace MR
{
	class NodeDef
	{
	public:
		virtual ~NodeDef() = 0;

		unsigned int m_typeID;
		unsigned int m_nodeID;
		const char* m_pName;
		const NodeDef* m_pParent;
	};
}
