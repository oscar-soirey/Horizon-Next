#include "factory.h"

#include <iostream>

static hge::factory::Factory game_factory_;

namespace hge
{
	namespace gamefactory
	{
		factory::ObjectConstructor GetObjectConstructor(const char* _name)
		{
			auto it = game_factory_.find(_name);
			if (it == game_factory_.end())
			{
				return nullptr;
			}
			return it->second;
		}

		factory::Factory GetFactory()
		{
			return game_factory_;
		}

		void InsertFactory(factory::Factory _moduleFactory)
		{
			game_factory_.insert(_moduleFactory.begin(), _moduleFactory.end());
		}

		void ClearFactory()
		{
			game_factory_.clear();
		}

	}
}