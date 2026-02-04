// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"
#include <AbilitySystemComponent.h>

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;;
}
// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitializePrimaryAttributes() const
{
    // 1. 安全检查：确保 AbilitySystemComponent 有效
    check(IsValid(GetAbilitySystemComponent()));

    // 2. 安全检查：确保已经在编辑器里指定了默认属性的 GE 资产
    check(DefaultPrimaryAttributes);

    // 3. 创建 EffectContextHandle：它包含了效果的来源、施放者等上下文信息
    const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();

    // 4. 创建 EffectSpecHandle：将静态的 GE 类转换为一个可应用的实例（Spec）
    // 这里设置 Level 为 1.0f，如果你的 GE 内部有随等级变化的曲线，这个值会生效
    const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, ContextHandle);

    // 5. 应用效果：将生成的 Spec 应用给目标（即角色自己）
    // *SpecHandle.Data.Get() 是为了获取 Spec 的原始指针并解引用
    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitAbilityActorInfo()
{

}

