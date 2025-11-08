#include "pch.h"
#include "MockComponent.h"

bool MockComponent::Setup() noexcept
{ 
	return true; 
} 

unique_ptr<UIComponent> MockComponent::CreateClone() const
{
	return unique_ptr<MockComponent>(new MockComponent(*this));
}
