#include "pch.h"
#include "JsonOperation.h"
#include "UIType.h"
#include "Component/Panel.h"
#include "Component/ImageGrid1.h"
#include "Component/ImageGrid3.h"
#include "Component/ImageGrid9.h"
#include "Component/Button.h"
#include "Component/Dialog.h"
#include "Component/TextArea.h"

void JsonOperation::UpdateJson(const unique_ptr<UIComponent>& data) noexcept
{
    JsonOperation jsOp{};
    data->SerializeIO(jsOp);
    //static_cast�� �ؼ� json���� �Ҵ��ϸ� json���� Ÿ�԰��� �ٲ�鼭 ���ο� Ÿ���� �Ǿ� ���� ���� (���⼭�� position)���� �������. �׷��� update�� �ؼ� �����ϴ� ��.
    if (jsOp.GetWrite().empty())
        return;
    m_write->GetCurrent().update(jsOp.GetWrite());
}

unique_ptr<UIComponent> JsonOperation::CreateComponentFromType(const string& typeName)
{
    unique_ptr<UIComponent> comp{ nullptr };
    if (typeName == "class Panel") comp = make_unique<Panel>();
    if (typeName == "class ImageGrid1") comp = make_unique<ImageGrid1>();
    if (typeName == "class ImageGrid3") comp = make_unique<ImageGrid3>();
    if (typeName == "class ImageGrid9") comp = make_unique<ImageGrid9>();
    if (typeName == "class Button") comp = make_unique<Button>();
    if (typeName == "class Dialog") comp = make_unique<Dialog>();
    if (typeName == "class TextArea") comp = make_unique<TextArea>();
    if (comp == nullptr) 
        return nullptr;

    comp->SerializeIO(*this);
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
        ProcessReadKey(key, [&data, this](auto& currentJson) {
            string curType{ currentJson["Type"] };
            data = move(CreateComponentFromType(curType));
            });
    }
}