#pragma once

#include "NodeDef.h"

namespace MR
{
	class IAnimSource;

	class NodeDefAnim : public NodeDef
	{
	public:
		bool m_generateDeltas;
		bool m_loop;
		bool m_playBackwards;
		float m_clipStartFraction;
		float m_clipEndFraction;
		IAnimSource* m_animSource;
	};
}
