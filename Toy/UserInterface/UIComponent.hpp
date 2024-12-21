template<typename T>
T ComponentCast(UIComponent* component)
{
	using NonPointerType = typename std::remove_pointer<T>::type;

	//assert로 하는 이유는 dynamic cast를 안 쓰고 싶어서. 타입이 다를 경우에는 debug일때는 여기서 걸리고 release는 타입변환후 미정의 행동
	assert(component->GetType() == string(typeid(NonPointerType).name()));
	return static_cast<T>(component);
}

template<typename T>
bool UIComponent::GetComponent(const string& name, T** outComponent) const noexcept
{
	UIComponent* component = GetComponent(name);
	*outComponent = ComponentCast<T*>(component);

	return false;
}