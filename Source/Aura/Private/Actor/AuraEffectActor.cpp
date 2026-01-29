// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include "Components/SphereComponent.h"
#include <AbilitySystemInterface.h>
#include <AbilitySystem/AuraAttributeSet.h>
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
}



// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

//将效果应用给目标
void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// 1. 获取目标的 Ability System Component (ASC)
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetASC == nullptr) return;

	// 2. 检查效果类是否有效
	check(GameplayEffectClass);

	// 3. 创建效果上下文（包含谁发起的、来源是谁等信息）
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);//将当前这个actor记录为效果的源头


	// 4. 创建效果规格（Spec），这是 GE 的运行时实例
	// 参数：效果类, 等级(Level), 上下文
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	/*
	GE类知识一个静态蓝图，只规定了这个类里面有什么(加50血)
	EffectContext注明了这个GE类的背景信息，谁施加的（某个物品类），应用给哪个actor（玩家）
	EffctSpec是一个运行时实例，将GE静态蓝图和EffectContext结合起来，变成一个可以应用给具体actor的实例
	*/

	// 5. 将效果应用给目标自身
	// .Data.Get() 获取智能指针中的原始数据，* 执行解引用
	// 应用效果并获得句柄
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	// 检查是否是无限效果且需要移除
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	// 根据策略应用 Instant, Duration, Infinite 效果
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	// 1. 处理应用策略为 ApplyOnEndOverlap 的情况
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	// 2. 移除策略为 RemoveOnEndOverlap 的无限效果
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		// 遍历 Map 寻找该 Target 拥有的句柄
		for (auto HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				// 从目标身上移除效果
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		// 从本地 Map 中清除这些已移除的句柄
		for (auto& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}

}




