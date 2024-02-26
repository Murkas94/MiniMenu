#ifndef MINIMENU_MENU_ENTRY_BASE_H
#define MINIMENU_MENU_ENTRY_BASE_H

#include "../MenuEntry.h"
#include "../Input.h"
#include "../MiniMenu.h"
#include <type_traits>
#include <functional>

namespace MiniMenu
{

template<typename DataType>
class TypedMenuEntryStaticBase
{
protected:
	DataType currentValue;
public:
	class ValueContainer
	{
	public:
		virtual ~ValueContainer(){}
		virtual DataType GetData() = 0;
	};

	class StaticReferenceValueContainer : public ValueContainer
	{
		const DataType& data;
	public:
		StaticReferenceValueContainer(const DataType& _data) : data(_data){}
		DataType GetData() override { return data; }
	};

	class ValueGetter
	{
	public:
		virtual ~ValueGetter(){}
		virtual ValueContainer* GetValue() = 0;
		virtual bool ReturnsNewInstances() = 0;
	};

	class ValueSetter
	{
	public:
		virtual ~ValueSetter(){}
		virtual void SetValue(const DataType& value) = 0;
	};

	class StaticReferenceValueGetter : public ValueGetter
	{
		StaticReferenceValueContainer data;
	public:
		ValueContainer* GetValue() override
		{
			return &data;
		}
		bool ReturnsNewInstances() override {return false;}

		StaticReferenceValueGetter(const DataType& _target) : data(_target){}
	};

	class StaticReferenceValueSetter : public ValueSetter
	{
		DataType& target;
	public:
		void SetValue(const DataType& value){target = value;}

		StaticReferenceValueSetter(DataType& _target) : target(_target){}
	};


	ValueGetter* getter = nullptr;
	ValueSetter* setter = nullptr;

	template<typename SpecificGetter, typename std::enable_if<std::is_base_of<ValueGetter, SpecificGetter>::value, bool>::type = true>
	SpecificGetter& SetGetter(SpecificGetter* newGetter)
	{
		if(getter != nullptr){delete getter;}
		getter = newGetter;
		return *newGetter;
	}
	StaticReferenceValueGetter& SetDefaultGetter(const DataType& target){
		return SetGetter(new StaticReferenceValueGetter(target));
	}

	template<typename SpecificSetter, typename std::enable_if<std::is_base_of<ValueSetter, SpecificSetter>::value, bool>::type = true>
	SpecificSetter& SetSetter(SpecificSetter* newSetter)
	{
		if(setter != nullptr){delete setter;}
		setter = newSetter;
		return *newSetter;
	}
	StaticReferenceValueSetter& SetDefaultSetter(DataType& target){
		return SetSetter(new StaticReferenceValueSetter(target));
	}

	class FunctionValueContainer : public ValueContainer
	{
	public:
		using getterFunctionType = std::function<DataType()>;
		//typedef DataType (*getterFunctionType)();
		getterFunctionType getterFunction;
		FunctionValueContainer(getterFunctionType function) : getterFunction(function) {}
		DataType GetData() override { return getterFunction(); }
	};

	class FunctionValueGetter : public ValueGetter
	{
		FunctionValueContainer data;
	public:
		FunctionValueGetter(typename FunctionValueContainer::getterFunctionType function)
			: data(function){}
		ValueContainer* GetValue() override { return &data; }
		bool ReturnsNewInstances() override {return false;}
	};

	class FunctionValueSetter : public ValueSetter
	{
	public:
		using setterFunctionType = std::function<void(const DataType& value)>;
	private:
		setterFunctionType setFunction;
	public:
		FunctionValueSetter(setterFunctionType function)
			: setFunction(function){}
		void SetValue(const DataType& value) override { setFunction(value); }
	};

	TypedMenuEntryStaticBase() { }
	TypedMenuEntryStaticBase(DataType& target) { SetDefaultGetter(target); SetDefaultSetter(target);}
	TypedMenuEntryStaticBase(const DataType& valueGetter, DataType& valueSetter) { SetDefaultGetter(valueGetter); SetDefaultSetter(valueSetter);}
	TypedMenuEntryStaticBase(typename FunctionValueContainer::getterFunctionType getterFunction, typename FunctionValueSetter::setterFunctionType setterFunction) { SetGetter(new FunctionValueGetter(getterFunction)); SetSetter(new FunctionValueSetter(setterFunction));}

	~TypedMenuEntryStaticBase(){ SetGetter<ValueGetter>(nullptr); SetSetter<ValueSetter>(nullptr); }

protected:
	bool HandleBase(bool isSelected, MiniMenu::Menu& menu);
	virtual bool CheckValueChange(MiniMenu::Input& input, DataType& value) = 0;
};

template <typename DataType>
bool TypedMenuEntryStaticBase<DataType>::HandleBase(bool isSelected, MiniMenu::Menu& menu)
{
	if(getter == nullptr){return false;}
	ValueContainer* data = getter->GetValue();
	if(data == nullptr){return false;}
	DataType newValue = data->GetData();
	if(getter->ReturnsNewInstances()){delete data;}
	bool valueChanged = newValue != currentValue;
	if(valueChanged)
	{
		currentValue = newValue;
	}else if(isSelected && setter != nullptr && CheckValueChange(menu.input, newValue))
	{
		valueChanged = true;
		setter->SetValue(newValue);
		currentValue = newValue;
	}
	return valueChanged;
}


template <typename T, typename DataType>
class TypedMenuEntryBase : public InheritedMenuEntry<T>, public TypedMenuEntryStaticBase<DataType>
{
public:
	TypedMenuEntryBase(Menu& menu) : InheritedMenuEntry<T>(menu), TypedMenuEntryStaticBase<DataType>() { }
	TypedMenuEntryBase(Menu& menu, DataType& target) : InheritedMenuEntry<T>(menu), TypedMenuEntryStaticBase<DataType>(target) { }
	TypedMenuEntryBase(Menu& menu, const DataType& valueGetter, DataType& valueSetter) : InheritedMenuEntry<T>(menu), TypedMenuEntryStaticBase<DataType>(valueGetter, valueSetter) { }
	TypedMenuEntryBase(Menu& menu, typename TypedMenuEntryStaticBase<DataType>::FunctionValueContainer::getterFunctionType getterFunction, typename TypedMenuEntryStaticBase<DataType>::FunctionValueSetter::setterFunctionType setterFunction) : InheritedMenuEntry<T>(menu), TypedMenuEntryStaticBase<DataType>(getterFunction, setterFunction) { }

	bool Handle(bool isSelected) override { return TypedMenuEntryStaticBase<DataType>::HandleBase(isSelected, MenuEntry::menu); }
};
}

#endif
