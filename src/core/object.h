/**
 *
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <variant>
#include <unordered_map>
#include <string>
#include <functional>

#include "common.h"
#include "api/build_dll.h"
#include "data/observable.h"
#include "data/ordered_map.h"
#include "data/data_enum.h"

namespace hge
{
	enum Access {
		Exposed,
		Editable
	};

	using PropVariantType = std::variant<int, float, bool, std::string, HGE_Vec2, HGE_Vec3, HGE_Vec4, HGE_Transform, HGE_Path>;
	using PropVariantTypePtr = std::variant<int*, float*, bool*, std::string*, HGE_Vec2*, HGE_Vec3*, HGE_Vec4*, HGE_Transform*, HGE_Path*>;

	struct ENGINE_API HGE_Property{
		PropVariantTypePtr property_member{};
		Access access{Exposed};
		IObservable* observable;

		/**
		 * @return The index of the type inside the variant list : 0:int, 1:float, 2:bool, etc...
		 */
		size_t GetType() const
			{ return property_member.index(); }

	};

	/**
	 * Returns the variant from a given string value : for example, input : "13.4", return 13.4 (<float>)
	 * Detect automatically the type
	 */
	ENGINE_API PropVariantType GetValueByString(const std::string& name);

	ENGINE_API std::string GetStringByValue(const PropVariantType& value);
	ENGINE_API std::string PropertyToString(const HGE_Property& prop);

	/**
	 * The base object of HGE engine, contains the basics control functions like init, beginplay, ...
	 * And contains the reflection system (access a member by its name)
	 */
	class ENGINE_API HGE_Object {
	public:
		std::string object_id_;

		HGE_Object();
		virtual ~HGE_Object();

		virtual void Init();

		//Gameplay logic functions
		virtual void StartGame();
		virtual void EndGame();

		/**
		 * Gameplay only
		 * @param _dt delta time
		 */
		virtual void Tick(double _dt);
		/**
		 * Called every frame (including when game doesn't simulate)
		 * @param _dt delta time
		 */
		virtual void Update(double _dt);

		template<typename T>
		T GetPropertyValue(const char* _name)
		{
			if (auto p = std::get_if<T*>(&properties_[_name].property_member))
			{
				return **p;
			}
			//LOG_ERROR("GetPropertyValue, Error with property : " + std::string(_name));
			return T{};
		}

		template<typename T>
		void SetPropertyValue(const char* _name, T _val)
		{
			if (auto p = std::get_if<T*>(&properties_[_name].property_member))
			{
				**p = _val;
			}
			else
			{
				//LOG_ERROR("SetPropertyValue, Error with property : " + std::string(_name));
			}
		}

		void SetPropertyValueStr(const char* _name, const char* _val);

		bool PropertyExists(const char* _name);
		Access GetPropertyAccess(const char* _name);

		const ordered_map<std::string, HGE_Property>& GetProperties() const;

	protected:
		hge::ordered_map<std::string, HGE_Property> properties_;
	};
}

#define HPROPERTY(var, access, ...) \
	properties_[#var] = hge::HGE_Property{ \
		&var, \
		access, \
		new Observable<decltype(var)>(var, [this](){ __VA_ARGS__; })}


#endif //OBJECT_H
