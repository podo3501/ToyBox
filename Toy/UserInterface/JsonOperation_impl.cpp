#include "pch.h"
#include "JsonOperation.h"
#include "UIComponent.h"
#include "Dialog.h"
#include "BGImage.h"
#include "TextArea.h"

void JsonOperation::UpdateJson(const unique_ptr<UIComponent>& data) noexcept
{
    JsonOperation jsOp{};
    data->SerializeIO(&jsOp);
    //static_cast�� �ؼ� json���� �Ҵ��ϸ� json���� Ÿ�԰��� �ٲ�鼭 ���ο� Ÿ���� �Ǿ� ���� ���� (���⼭�� position)���� �������. �׷��� update�� �ؼ� �����ϴ� ��.
    m_write->GetCurrent().update(jsOp.GetWrite());
}

unique_ptr<UIComponent> JsonOperation::CreateComponentFromType(const string& typeName)
{
    unique_ptr<UIComponent> comp{ nullptr };
    if (typeName == "class UIComponent") comp = std::make_unique<UIComponent>();
    if (typeName == "class Dialog") comp = make_unique<Dialog>();
    if (typeName == "class BGImage") comp = make_unique<BGImage>();
    if (typeName == "class TextArea") comp = make_unique<TextArea>();
    if (comp == nullptr) return nullptr;

    comp->SerializeIO(this);
    return comp;
}

void JsonOperation::Process(const string& key, unique_ptr<UIComponent>& data)
{
    if (IsWrite())
    {
        m_write->GotoKey(key);
        const string& type = data->GetType();
        Process("Type", const_cast<string&>(type));
        UpdateJson(data);
        m_write->GoBack();
    }
    else
    {
        m_read->GotoKey(key);
        string curType{};
        Process("Type", curType);
        data = move(CreateComponentFromType(curType));
        m_read->GoBack();
    }
}