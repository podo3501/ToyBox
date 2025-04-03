#pragma once

struct IRenderer;
class TextureResourceBinder;
class EditWindow;
class UIComponent;
class UICommandList;
enum class ComponentID;

class EditWindowFactory 
{
    using FactoryFunction = function<unique_ptr<EditWindow>(UIComponent*, IRenderer*, TextureResourceBinder*, UICommandList*)>;

public:
    static void RegisterFactories();
    static unique_ptr<EditWindow> CreateEditWindow(UIComponent* component, 
        IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList);

private:
    static inline void Register(ComponentID id, FactoryFunction factory) noexcept { (*m_factoryMap)[id] = factory; }
    static unique_ptr<EditWindow> Create(UIComponent* component, 
        IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList);

    template <typename EditType, typename ComponentType, typename... Args>
    static unique_ptr<EditWindow> CreateEdit(UIComponent* component, Args&&... args)
    {
        return make_unique<EditType>(ComponentCast<ComponentType>(component), forward<Args>(args)...);
    }

    template <typename EditType, typename ComponentType>
    static void RegisterEditWindow(ComponentID id)
    {
        Register(id, [](UIComponent* component, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) {
            return CreateEdit<EditType, ComponentType>(component, resBinder, cmdList);
            });
    }

    template <typename EditType, typename ComponentType>
    static void RegisterEditWindowWithRenderer(ComponentID id)
    {
        Register(id, [](UIComponent* component, IRenderer* renderer, TextureResourceBinder* resBinder, UICommandList* cmdList) {
            return CreateEdit<EditType, ComponentType>(component, renderer, resBinder, cmdList);
            });
    }

    static optional<unordered_map<ComponentID, FactoryFunction>> m_factoryMap;
};