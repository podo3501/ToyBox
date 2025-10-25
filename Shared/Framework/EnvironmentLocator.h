#pragma once
#include <memory>
#include <string>
#include "DirectXTK12/SimpleMath.h"
#include "Shared/Framework/Locator.h"
#include "Shared/Framework/Environment.h"

using EnvironmentLocator = Locator<Environment>;

[[nodiscard]]
inline std::unique_ptr<Environment> InitializeEnvironment(
    const std::wstring& resourcePath, const DirectX::SimpleMath::Vector2& resolution)
{
    auto env = std::make_unique<Environment>(resourcePath, resolution);
    EnvironmentLocator::Provide(env.get());
    return env; //Ŭ������ �����ϰ� �����ؼ� ������ Ŭ������ ����ɶ� �Ҹ��� ȣ��
}

//EnvironmentLocator::GetService() �ؼ� ȣ���ϴ°� �Ⱦ �����ϰ� �� ���̶� �� �Լ����� ���Ǹ� 
//���ؼ� ����Ŷ� ���� �Ű澲�� �ʾƵ� ��.
template <typename R>
decltype(auto) EnvCall(R(Environment::* getter)() const) noexcept 
{
    return (EnvironmentLocator::GetService()->*getter)();
}

template <typename Fn, typename... Args>
decltype(auto) EnvCall(Fn&& fn, Args&&... args) noexcept
{
    return std::invoke(std::forward<Fn>(fn),
        EnvironmentLocator::GetService(),
        std::forward<Args>(args)...);
}

//Locator�� �����Լ�ȭ ��Ŵ. �̷��� �ϴ� ������ ���������� Locator ������ ���� ���� ���߱� ���ؼ�.
//��Ÿ�ӿ� ��ü�� �Ͼ�� �ʴ� ���� �κ��̱� ������ Locator�� ���ߴ°� �ùٸ� �� ����.
inline const std::wstring& GetResourcePath() noexcept { return EnvCall(&Environment::GetResourcePath); }
inline const std::wstring& GetResourceFontPath() noexcept { return EnvCall(&Environment::GetResourceFontPath); }
inline const DirectX::SimpleMath::Vector2& GetResolution() noexcept { return EnvCall(&Environment::GetResolution); }
inline DirectX::SimpleMath::Rectangle GetRectResolution() noexcept { return EnvCall(&Environment::GetRectResolution); }

inline std::wstring GetResourceFullFilename(const std::wstring& filename) noexcept { 
    return EnvCall(&Environment::GetResourceFullFilename, filename); }
inline std::wstring GetRelativePath(const std::wstring& fullPath) noexcept {
    return EnvCall(&Environment::GetRelativePath, fullPath);
}
