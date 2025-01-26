#pragma once

template<typename ChildImageGrid, typename ImageGrid>
ChildImageGrid GetFirstImageGrid(ImageGrid imageGrid) noexcept
{
	vector<UIComponent*> components = imageGrid->GetTransformContainer().GetComponents();
	if (components.empty()) return nullptr;

	return ComponentCast<ChildImageGrid>(components[0]);
}

template<typename ChildImageGrid, typename ImageGrid>
bool GetImageGridComponents(ImageGrid imageGrid, vector<ChildImageGrid>& outComponents) noexcept
{
    vector<UIComponent*> components = imageGrid->GetTransformContainer().GetComponents();
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