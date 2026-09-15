#pragma once

#include "../../src/plugins/iplugin.h"

namespace hn::plugins
{
	class FBXImporter : public IPlugin {
	public:
		~FBXImporter() override;
		void Init(EngineInfos *engine_infos) override;

		void Tick(double dt) override;
	};
}

HORIZON_PLUGIN(hn::plugins::FBXImporter);