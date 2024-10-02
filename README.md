# Soulslike_Combat
https://www.youtube.com/watch?v=5rWJYEPz2hc

# 목표
플레이어와 보스간의 **전투** 구현.

# 구현 개요

---

GameplayAbilitySystem기반으로 구현.
모든 기능은 C++로 작성. UI, AnimBP는 제외
블루프린트는 프로퍼티 값만 할당하는 용도로 사용.

## 어빌리티

공격, 구르기, 막기 등 각종 액션
`Source/GASToy/AbilitySystem/Ability`

## 입력

EnhancedInput사용, 조작감 향상을 위한 선입력 시스템 구현`AGTPlayerCharacter::OnAbilityInputActionPressed`

## 캐릭터 이동 및 카메라 제어

캐릭터 상태 (걷기, 달리기, 락온중) 에 따라 이동 속도, 캐릭터 회전, 카메라 제어를 관리.
`UGTMovementControlComponent`

## 충돌처리

무기와 캐릭터의 정밀한 충돌검사를 위한 시스템 구현 /`UGTCollisionCheckComponent`
다수 발사체가 중복 충돌시 한번의 이벤트만 발생하기 위한 관리자 구현 /`UGTCollisionSubsystem`

## 데미지 처리

데미지를 가할때 가드, 패링 가능 유무 및 피격자 상태를 파악해 막기 결과를 도출.
결과에 따라서 이펙트, 히트리액션, 데미지, 그로기 수치등 일련의 처리를 구현.
`UGTGameplayAbility::ApplyDamage`

## 애니메이션

캐릭터의 상태에 따라 블렌드 스페이스, 몽타주 재생
노티파이를 활용하여 애니메이션에 기반한 게임플레이 구현.
`Source/GASToy/Animation`

## AI

BehaviorTree를 사용해 보스전투 구현.
근접공격, 대쉬공격, 원거리 공격등을 거리기반으로 적절하게 구성.
`Source/GASToy/AI`

# 구현 상세

---

## 1. 어빌리티

### 공격

`UGTGameplayAbility_MontageAttack`

어빌리티 시작시 몽타주를 재생하고 공격 이벤트를 수신받아 처리한다.

### 콤보

https://youtu.be/n73wdq9_Ois

![공격 몽타주.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/8bfea7e2-8b1c-4494-a611-6e709ccd1821/8cdff32b-208c-4cb4-bbc8-e08e77e0c309/%EA%B3%B5%EA%B2%A9_%EB%AA%BD%ED%83%80%EC%A3%BC.png)

공격 애니메이션은 크게 3단계로 구성되어 있다.
준비 → 공격 → 복귀
공격하는 구간에 선입력을 저장하고 공격이 끝나고 복귀하는 시점에 선입력을 실행해 다음 공격으로 진행.
복귀 시점에 공격 종료 상태로 만들어 애니메이션이 완전히 종료 되기전 다음 액션으로 넘어갈 수 있게 처리

어빌리티마다 태그를 부여하고 ActivationRequiredTags를 설정해 실행조건을 판단.
[AbilityTags / ActivationRequiredTags]
[Attack.Light.1 /  None ]
[Attack.Light.2 /  Attack.Light.1 ]
[Attack.Light.3 /  Attack.Light.2 ]
[Attack.Light.4 /  Attack.Light.3 ]

### 닷지

https://youtu.be/gG2GezGSreo

`UGTGameplayAbility_Dodge`

유저의 입력 방향으로 구른다, 무적 상태를 부여한다. 
8개의 방향 몽타주중 가장 적합한 몽타주를 재생한다.
ApplyRootMotionTask를 사용하여 캐릭터를 이동시킨다.

### 가드와 패링

https://youtu.be/xwJz1_U-2IE

`UGTGameplayAbility_GuardAndParry`

가드를 하자마자 막은 경우 패링을 한다.
패링 가능 시간이 지난후 막으면 가드한다.
막은 방향에 맞게 성공 애니메이션을 재생한다.

가드 광클로 계속 패링하는 것을 방지하기 위해 쿨다운 적용.
패링을 했다면 재입력시 바로 패링가능 상태로 만든다.

### 그로기 및 처형

https://youtu.be/un50ikkDANI

`UGTGameplayAbility_Execute`

`UGTGameplayAbility_Groggy`

`UGTGameplayAbility_ScanGroggy`

그로기 수치가 최대가 되면 그로기 상태가 된다. 이때 조작과 다른 어빌리티 실행을 할 수 없다. : 영상: 처형시 캐릭터 워프까지 되도록 

스캔 그로기 어빌리티가 주기적으로 그로기 액터를 찾는다.
찾았고 공격버튼을 누르면 처형을 시작한다.

처형 어빌리티가 그로기 액터를 이벤트로 넘겨받아 처형시킨다.

### 히트 리액션

`Source/GASToy/AbilitySystem/Ability/HitReaction`

히트 리액션을 기능별로 하나의 객체로 만들어 다양한 조합을 할 수 있게 만듬. 

`ApplyRootMotion` : 넉백, 에어본
`PlayMontage` : 피격, 에어본
`TimeDilation` : 슬로우 효과

## 2. 데미지 처리

![DamageData.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/8bfea7e2-8b1c-4494-a611-6e709ccd1821/78e54f0d-ec40-47c0-8b54-29927df9e310/DamageData.png)

`UGTGameplayAbility::ApplyDamage`

각 어빌리티는 이벤트태그에 해당하는 데미지 데이터를 가짐. / `TMap<FGameplayTag, FGTDamageData> EventTagToDamageData`
데미지 적용 이벤트를 받으면 데미지를 타깃에게 적용.

데미지 적용은 일련의 처리를 한다.

- 막기 검사를 하고 결과에 따라 게임플레이 이펙트(데미지, 그로기)적용, 게임플레이큐(VFX, SFX), 히트리액션, 가드와 패링을 처리한다.
- 막기 결과를 조건으로 사용해 적절한 처리를 할 수 있게 구현.

## 3. 캐릭터 이동 및 카메라 제어

`UGTMovementControlComponent`

걷기, 달리기, 락온 상태등 캐릭터의 현재 상태에 따라 이동방식 및 이동속도를 다르게 처리해야 한다.

특히 락온시 타깃과 가까울때 카메라 회전이 빈번해 어지러움을 유발함.
이때 타깃과 가까울 수록 카메라 회전 보간 속도를 줄여 어지러움을 해결함.

## 4. 선입력 시스템

`AGTPlayerCharacter::OnAbilityInputActionPressed`

액션 도중 다음입력을 미리 저장해 조작감을 향상.

입력 데이터에 선입력 저장 유무를 설정한다.

선입력 사용안하면 입력을 받자마자 실행.

선입력 사용하면 상황에 따라 다르게 처리.

- 입력 저장기간이 아니면 바로 실행.
- 저장 기간이면 입력을 저장하고, 기간이 끝나면 저장한 입력을 실행.

## 5. 충돌 처리

https://youtu.be/isbs40ilnK8

`UGTCollisionCheckComponent`

정밀한 충돌검사를 통해 게임플레이를 향상.

기존 오버랩 이벤트는 ImpactLocation을 구할 수 없음. 따라서 트레이스 검사를 해야한다.
무기의 모양에 맞춰 트레이스 구체를 연속적으로 설정해 정밀한 충돌검사를 수행한다.

`UGTCollisionSubsystem`

충돌 이벤트를 관리한다.

중복 충돌 방지 : 어떤 스킬에서 발사체를 동시에 발사하고, 여러개를 맞더라도 하나의 충돌 이벤트만 발생 시키기 위해 범위와 아이디를 설정해 구현.

## 6. 애니메이션

`Source/GASToy/Animation`

블렌드스페이스, 몽타주, 루트모션, 애님 노티파이를 활용.

블랜드 스페이스

- 걷기 ↔ 달리기 : 1방향
- 락온상태시 이동 : 8방향

몽타주

노티파이를 배치하여 애니메이션 기반한 게임플레이 구현.

루트모션

기본 루트모션을 사용하거나, 키프레임을 적용해 루트모션 구현.
모션워핑을 이용해 루트 모션의 가중치를 설정하거나, 타깃 위치로 트랜스폼을 변경할 때 애니메이션과 동기화한다.

## 7. AI

`Source/GASToy/AI`

BehaviorTree기반으로 AI를 구현. 

전투 시작시 인트로 어빌리티 실행, 이후 플레이어 거리에 따라 처리.

크게 3가지 타입으로 구성 : 근거리 스킬, 대쉬 스킬, 원거리 스킬
각 거리에 해당하는 스킬을 여러개 배치해 순차적으로 실행하여 다양한 전투 구현.
주기적으로 플레이어와 거리를 벌려 근접, 원거리 전투를 번갈아가며 경험하게 구현.

![BT 전체](https://prod-files-secure.s3.us-west-2.amazonaws.com/8bfea7e2-8b1c-4494-a611-6e709ccd1821/ef2b62cf-8756-454b-b6f6-9a3cdf545545/AI_All.png)

BT 전체

![인트로](https://prod-files-secure.s3.us-west-2.amazonaws.com/8bfea7e2-8b1c-4494-a611-6e709ccd1821/f17a4766-fd8c-44f3-8b8e-543ca6fbc018/AI_Intro.png)

인트로

![근거리](https://prod-files-secure.s3.us-west-2.amazonaws.com/8bfea7e2-8b1c-4494-a611-6e709ccd1821/d683a5b2-4e7d-4366-ab55-ffe1aa37ea65/AI_Short.png)

근거리

![원거리 1](https://prod-files-secure.s3.us-west-2.amazonaws.com/8bfea7e2-8b1c-4494-a611-6e709ccd1821/1d820491-8001-4f4f-b8af-e5ff938e33db/AI_Long_Left.png)

원거리 1

![원거리 2](https://prod-files-secure.s3.us-west-2.amazonaws.com/8bfea7e2-8b1c-4494-a611-6e709ccd1821/db9ad63e-61a8-4586-bd8b-df01fcf980d5/AI_Long_Right.png)

원거리 2

![엄청 멀때](https://prod-files-secure.s3.us-west-2.amazonaws.com/8bfea7e2-8b1c-4494-a611-6e709ccd1821/08ff573f-84e0-4dde-9a0d-f4a9abaa2a7f/AI_LongLong.png)

엄청 멀때
