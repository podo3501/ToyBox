#pragma once
#include "Mocks/MockComponents.h"
#include "Mocks/MockUtils.h"

class UIComponentT_Single : public ::testing::Test
{
protected:
    virtual void SetUp() override;
    virtual void TearDown() override {};

    unique_ptr<MockComponent> m_main{ nullptr };
    unique_ptr<MockComponent> m_mock{ nullptr };
    MockComponent* m_mockPtr{ nullptr };
};

///////////////////////////////////////////////////////////////

void UIComponentT_Single::SetUp()
{
    UILayout layout{ {800, 600}, Origin::LeftTop };
    tie(m_main, std::ignore) = CreateMockComponent<MockComponent>(layout);
    tie(m_mock, m_mockPtr) = CreateMockComponent<MockComponent>();
}