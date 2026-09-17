#pragma once

namespace hn
{
	struct EngineInfos {
		void* rendering_interface=nullptr;

		bool editor=false;

		//do not use if plugin is runtime only
		void* editor_main_window=nullptr;
	};
}