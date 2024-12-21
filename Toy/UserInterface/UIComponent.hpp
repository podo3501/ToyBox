template<typename T>
T ComponentCast(UIComponent* component)
{
	using NonPointerType = typename std::remove_pointer<T>::type;

	//assert�� �ϴ� ������ dynamic cast�� �� ���� �;. Ÿ���� �ٸ� ��쿡�� debug�϶��� ���⼭ �ɸ��� release�� Ÿ�Ժ�ȯ�� ������ �ൿ
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