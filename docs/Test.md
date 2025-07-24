# Scene

---

Scene 클래스를 상속받은 클래스는 해당 씬에서 생성된 모든 오브젝트를 관리합니다.

Scene 클래스는 SceneManager에 의해 관리되고 추가된 순서대로 고유 인덱스를 가집니다.

게임오브젝트는 Scene에서 추가될 때 objectsToAdd에 추가되고 다음 프레임에 activeObjects로 컨테이너를 이동한 이후 활성화 됩니다.

제거 또한 게임오브젝트의 shouldRemove 플래그가 활성화되면 FindRemoveObject에서 확인하고 objectsToDestory로 옮겨집니다. 그 이후 CleanUpDestoryObjects에서 해당 게임 오브젝트는 완전히 제거됩니다.
위 행동은 모든 업데이트가 끝나고 마지막에 실행됩니다.

## Scene 상태

```cpp
enum SceneState
{	
	Enter = 0,		// 씬 전환 후 시작 시
	Playing,		// 씬 업데이트 중이고 씬 교체가 없을 때
	ReadyToChange,	// 다음 프레임에 씬 교체가 시작될 때
	Exit,			// 씬 내 데이터 정리 후 Enter 전 
};
```

### 상태 목록

| 함수명 | 설명 |
| --- | --- |
| Enter | Scene의 OnEnter()가 호출될 때 해당 상태로 변합니다 OnEnterImpl() 함수가 실행되기 전 입니다. |
| Playing | Scene의 OnEnter()가 호출되고 OnEnterImpl()가 호출된 이후 해당 상태가 됩니다. 해당 상태는 SceneManager에서 변경을 요청하기 전까지 유지됩니다. |
| ReadyToChange | SceneManager에서 씬 변경을 요청했을 때 해당 상태로 변경됩니다. 해당 상태는 프레임의 마지막에 CheckSceneLoad()가 호출되고 OnExit()가 호출되기 전까지 유지됩니다. |
| Exit | Scene의 OnExit()가 호출될 때 변경됩니다. 해당 상태일 땐 Scene에 있는 모든 오브젝트 컨테이너의 내용을 해제하고 Clear합니다. |

## Scene

```cpp

// NOTE : AddGameObject한 객체는 Scene내부에서 delete를 호출해서 메모리 해제를 함

class Scene : public IGameObjectQuery
{
friend class SceneManager;

public:
	/// <summary>
	/// OnEnterImpl 실행
	/// </summary>
	void OnEnter();

	/// <summary>
	/// 게임 오브젝트 파괴 리스트 순환 후 UpdateImpl() 호출
	/// </summary>
	void Update();

	/// <summary>
	/// 게임 물리 업데이트
	/// </summary>
	void FixedUpdate(std::vector<CollisionInfo>& collisionInfos);

	/// <summary>
	/// 모든 업데이트 끝나고 실행
	/// </summary>
	void LateUpdate();

	/// <summary>
	/// 등록된 오브젝트 OnDestroy() 실행
	/// </summary>
	void OnExit();	

	/// <summary>
	/// 제거 목록에 등록된 모든 게임 오브젝트 제거
	/// </summary>
	void CleanUpDestroyedObjects();

	// GameObject 제어 함수
	void AddGameObject(GameObject* gameObject);
	void AddGameObject(GameObject* gameObject, const std::string& name);
	void FindRemoveObject();

	GameObject* FindByName(const std::string& name) const override;

protected:
	void AddCreatedObjects();
	void CheckGameObjectStartQueue();

	/// <summary>
	/// Scene을 상속 받은 클래스가 씬 시작 시 실행할 내용
	/// </summary>
	virtual void OnEnterImpl() = 0;

	/// <summary>
	/// Scene을 상속 받은 클래스가 씬 종료 시 실행할 내용
	/// </summary>
	virtual void OnExitImpl() = 0;

	/// <summary>
	/// Scene 상속 받은 클래스가 Update문 실행할 내용 ( Scene::Update() 에서 호출 )
	/// </summary>
	virtual void UpdateImpl() = 0;

	std::vector<GameObject*> activeObjects;		// 매 프레임마다 업데이트되는 오브젝트들
	std::vector<GameObject*> objectsToDestroy;	// 모든 업데이트가 끝난 시점에서 제거될 오브젝트들
	std::vector<GameObject*> objectsToAdd;		// 다음 프레임에 activeObjects에 포함될 오브젝트들

	SceneState state = SceneState::Exit;
};

```

## 멤버 함수

Public

| 함수명 | 설명 |
| --- | --- |
| OnEnter | 씬이 처음 실행될 때 호출되는 함수 |
| Update | 매 프레임마다 시스템 클래스가 업데이트되는 함수 |
| FixedUpdate | 매 ~~물리~~ 프레임마다 물리 시스템 클래스가 업데이트 되는 함수 |
| LateUpdate | Update, FixedUpdate가 끝나고 실행되는 업데이트 함수 |
| OnExit | 씬이 종료될 때 호출 되는 함수 |
| CleanUpDestroyedObjects | Application에서 모든 프레임이 끝날 때 제거될 예정인 오브젝트를 제거하는 함수 |
| AddGameObject | 씬에 오브젝트를 추가하는 함수 |
| FindRemoveObject | **CleanUpDestroyedObjects() 에서 맨 먼저 호출됨**, 제거될 예정인 게임오브젝트를 activeObjects에서 objectsToDestroy로 옮기는 함수 |
| FindByName | **IGameObjectQuery 인터페이스 정의 함수**, 게임 오브젝트가 다른 게임 오브젝트를 찾을 때 사용되는 함수 |

Protected

| 함수명 | 설명 |
| --- | --- |
| AddCreatedObjects | **매 첫 Update()에서 실행됨**, objectsToAdd에서 activeObjects로 게임 오브젝트를 추가하는 함수 |
| CheckGameObjectStartQueue | **MonoBehavior의 지연 등록 이후 실행**, 모든 게임 오브젝트에서 OnStart가 실행되지 않는 컴포넌트가 있는지 확인하고 실행하는 함수 |
| OnEnterImpl | 순수 가상 함수, 상속받은 클래스에서 OnEnter에 실행할 내용을 정의하는 함수 |
| OnExitImpl | 순수 가상 함수, 상속받은 클래스에서 OnExit에 실행할 내용을 정의하는 함수 |
| UpdateImpl | 순수 가상 함수, 상속받은 클래스에서 OnUpdate에 실행할 내용을 정의하는 함수 |

## 멤버 변수

| 함수명 | 설명 |
| --- | --- |
| activeObjects | 매 프레임마다 업데이트되는 오브젝트들 |
| objectsToDestroy | 모든 업데이트가 끝난 시점에서 제거될 오브젝트들 |
| objectsToAdd | 다음 프레임에 activeObjects에 포함될 오브젝트들 |
| state | 현재 Scene 상태 |
