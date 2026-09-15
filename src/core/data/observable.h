#ifndef HGE_OBSERVABLE_H
#define HGE_OBSERVABLE_H

#include "../api/build_dll.h"
#include <functional>
#include <utility>

class ENGINE_API IObservable {
public:
	virtual ~IObservable()=default;
	virtual void Tick()=0;
};


//not ENGINE_API because of the template
template<typename T>
class Observable : public IObservable {
public:
	Observable(T& _ref, std::function<void()> _callback):
		ref_(_ref),
		callback_(std::move(_callback))
		{}

	void Tick() final
	{
		if (last_ != ref_)
		{
			callback_();
		}
		last_ = ref_;
	}

private:
	T last_;
	T& ref_;
	std::function<void()> callback_;
};

#endif