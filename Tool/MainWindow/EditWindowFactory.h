#pragma once

struct IRenderer;
class EditWindow;
class UIComponent;
class CommandList;
enum class ComponentID;

class EditWindowFactory 
{
    using FactoryFunction = function<unique_ptr<EditWindow>(UIComponent*, IRenderer*, CommandList*)>;

public:
    static void RegisterFactories();
    static unique_ptr<EditWindow> CreateEditWindow(UIComponent* component, IRenderer* renderer, CommandList* cmdList);

private:
    static inline void Register(ComponentID id, FactoryFunction factory) noexcept { (*m_factoryMap)[id] = factory; }
    static unique_ptr<EditWindow> Create(UIComponent* component, IRenderer* renderer, CommandList* cmdList);

    template <typename EditType, typename ComponentType, typename... Args>
    static unique_ptr<EditWindow> CreateEdit(UIComponent* component, Args&&... args)
    {
        return make_unique<EditType>(ComponentCast<ComponentType>(component), forward<Args>(args)...);
    }

    template <typename EditType, typename ComponentType>
    static void RegisterEditWindow(ComponentID id)
    {
        Register(id, [](UIComponent* component, IRenderer* renderer, CommandList* cmdList) {
            return CreateEdit<EditType, ComponentType>(component, cmdList);
            });
    }

    template <typename EditType, typename ComponentType>
    static void RegisterEditWindowWithRenderer(ComponentID id)
    {
        Register(id, [](UIComponent* component, IRenderer* renderer, CommandList* cmdList) {
            return CreateEdit<EditType, ComponentType>(component, renderer, cmdList);
            });
    }

    static optional<unordered_map<ComponentID, FactoryFunction>> m_factoryMap;
};