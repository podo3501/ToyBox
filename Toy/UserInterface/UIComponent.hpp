template <typename TargetType>
TargetType ComponentCast(UIComponent* component) 
{
    if (!component) return nullptr;
    ComponentID typeId = component->GetTypeID();
    if (typeId == ComponentID::Unknown) return nullptr;

    using NonPointerTargetType = typename std::remove_pointer<TargetType>::type; //TargetŸ���� Button* �̷��� �����ͷ� �Ѿ���� ������ *�� ���
    if (typeId == NonPointerTargetType::GetTypeStatic())
        return static_cast<TargetType>(component);

    return nullptr;
}

template<typename T>
void UIComponent::GetComponent(const string& name, T** outComponent) const noexcept
{
	UIComponent* component = GetComponent(name);
	*outComponent = ComponentCast<T*>(component);
}