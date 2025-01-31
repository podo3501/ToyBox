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
T UIComponent::GetComponent(const string& name) noexcept
{
    UIComponent* find = GetUIComponentEx().GetComponent(name);
    return ComponentCast<T>(find);
}

//template<typename T>
//void UIComponent::GetComponent(const string& name, T** outComponent) const noexcept
//{
//	UIComponent* component = UIComponentEx::GetComponent(name);
//	*outComponent = ComponentCast<T*>(component);
//}