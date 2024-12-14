#include "pch.h"
#include "JsonOperation.h"
#include "UIType.h"
#include "Panel.h"
#include "ImageGrid1.h"
#include "ImageGrid3.h"
#include "ImageGrid9.h"
#include "Dialog.h"
#include "BGImage.h"
#include "TextArea.h"
#include "ImagePartSet.h"
#include "OButton.h"

void JsonOperation::UpdateJson(const unique_ptr<UIComponent>& data) noexcept
{
    JsonOperation jsOp{};
    data->SerializeIO(jsOp);
    //static_cast를 해서 json으로 할당하면 json내의 타입값이 바뀌면서 새로운 타입이 되어 이전 정보 (여기서는 position)값이 사라진다. 그래서 update를 해서 병합하는 것.
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
    if (typeName == "class Dialog") comp = make_unique<Dialog>();
    if (typeName == "class BGImage") comp = make_unique<BGImage>();
    if (typeName == "class TextArea") comp = make_unique<TextArea>();
    if (typeName == "class OButton") comp = make_unique<OButton>();
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