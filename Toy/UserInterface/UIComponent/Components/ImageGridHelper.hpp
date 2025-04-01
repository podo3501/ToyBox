#pragma once

template<typename ChildImageGrid>
bool GetImageGridComponents(vector<UIComponent*> components, vector<ChildImageGrid>& outComponents) noexcept
{
    if (components.size() != 3) return false;

    outComponents.clear();
    for (UIComponent* component : components)
    {
        ChildImageGrid childImgGrid = ComponentCast<ChildImageGrid>(component);
        if (!childImgGrid) return false;

        outComponents.push_back(childImgGrid);
    }

    return true;
}