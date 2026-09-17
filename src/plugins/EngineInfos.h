#pragma once

namespace hn
{
	struct EngineInfos {
		void* rendering_interface=nullptr;

		//do not use if plugin is runtime only
		void* editor_main_window=nullptr;
	};
}