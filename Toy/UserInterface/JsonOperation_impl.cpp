#include "pch.h"
#include "JsonOperation.h"
#include "UIType.h"
#include "UIComponent.h"
#include "Dialog.h"
#include "BGImage.h"
#include "TextArea.h"
#include "ImagePartSet.h"
#include "Button.h"

void JsonOperation::UpdateJson(const unique_ptr<UIComponent>& data) noexcept
{
    JsonOperation jsOp{};
    data->SerializeIO(&jsOp);
    //static_cast�� �ؼ� json���� �Ҵ��ϸ� json���� Ÿ�԰��� �ٲ�鼭 ���ο� Ÿ���� �Ǿ� ���� ���� (���⼭�� position)���� �������. �׷��� update�� �ؼ� �����ϴ� ��.
    if (jsOp.GetWrite().empty())
        return;
    m_write->GetCurrent().update(jsOp.GetWrite());
}

unique_ptr<UIComponent> JsonOperation::CreateComponentFromType(const string& typeName)
{
    unique_ptr<UIComponent> comp{ nullptr };
    if (typeName == "class UIComponent") comp = std::make_unique<UIComponent>();
    if (typeName == "class Dialog") comp = make_unique<Dialog>();
    if (typeName == "class BGImage") comp = make_unique<BGImage>();
    if (typeName == "class TextArea") comp = make_unique<TextArea>();
    if (typeName == "class Button") comp = make_unique<Button>();
    if (comp == nullptr) 
        return nullptr;

    comp->SerializeIO(this);
    return comp;
}

void JsonOperation::Process(const string& key, unique_ptr<UIComponent>& data)
{
    if (IsWrite())
    {
        if (data == nullptr)
            return;

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

void JsonOperation::Process(const string& key, map<ButtonState, unique_ptr<ImagePartSet>>& data)
{
    if (IsWrite())
    {
        if (data.empty())
            return;

        ProcessWriteKey(key, [&data](auto& currentJson) {
            for (auto& iter : data) 
            {
                JsonOperation jsOp{};
                jsOp.Process(EnumToString<ButtonState>(iter.first), iter.second);
                currentJson.push_back(jsOp.GetWrite());
            }
            });
    }
    else
    {
        data.clear();
        ProcessReadKey(key, [&data, this](const auto& currentJson) {
            for (const auto& keyJ : currentJson) 
                for (const auto& [k, v] : keyJ.items())
                    data.insert(make_pair( StringToEnum<ButtonState>(k), CreateData<ImagePartSet>(v)));
            });
    }
}