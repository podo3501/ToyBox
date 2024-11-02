#pragma once

struct IRenderer;
struct IRenderItem;

unique_ptr<IRenderItem> CreateComponent(const string& component, IRenderer* renderer);
tuple<unique_ptr<IRenderItem>, Vector2> GetComponent(const string& component, const nlohmann::json& data);
