#include "pch.h"
#include "ImGuiManager.h"
#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "LevelManager.h"
#include "Level.h"
#include "Layer.h"
#include "AssetManager.h"
#include "Camera.h"
#include "RenderManager.h"
#include "RigidBody.h"
#include "Light.h"
#include "../../3rdParty/ImGuizmo/ImGuizmo.h"
#include "../../3rdParty/ImGui/imgui_internal.h"
#include "SubLevel.h"
#include "WidgetWindow.h"
#include "Widget.h"
static int objCounter = 0;

CImGuiManager::CImGuiManager()
{
}

CImGuiManager::~CImGuiManager()
{
	CleanUp();
}

void CImGuiManager::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui_ImplWin32_Init(CEngine::GetInst()->GetWindowInfo().hWnd);

	ImGui_ImplDX12_InitInfo init_info = {};
	init_info.Device = DEVICE.Get();
	init_info.CommandQueue = CDevice::GetInst()->GetCmdQueue()->GetCmdQueue().Get();
	init_info.NumFramesInFlight = SWAP_CHAIN_BUFFER_COUNT;
	init_info.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	init_info.DSVFormat = DXGI_FORMAT_UNKNOWN;
	init_info.SrvDescriptorHeap = CDevice::GetInst()->GetImGuiDescHeap()->GetDescriptorHeap().Get();
	init_info.LegacySingleSrvCpuDescriptor = CDevice::GetInst()->GetImGuiDescHeap()->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	init_info.LegacySingleSrvGpuDescriptor = CDevice::GetInst()->GetImGuiDescHeap()->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	ImGui_ImplDX12_Init(&init_info);
}

void CImGuiManager::ReadyWindow()
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX12_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();

	DrawLevelWindow();
	DrawInspectorWindow();

	//static float f = 0.0f;
	//static int counter = 0;
	//
	//ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	//
	//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//
	//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//
	//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	//	counter++;
	//ImGui::SameLine();
	//ImGui::Text("counter = %d", counter);
	//
	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//ImGui::End();
}

void CImGuiManager::Render()
{
	ImGui::Render();

	ID3D12DescriptorHeap* descriptorHeaps[] = { CDevice::GetInst()->GetImGuiDescHeap()->GetDescriptorHeap().Get() };
	GRAPHICS_CMD_LIST->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GRAPHICS_CMD_LIST.Get());
}

void CImGuiManager::CleanUp()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (m_SelectedObject)
	{
		int refCount = m_SelectedObject->GetRefCount();
		if (refCount > 0)
			m_SelectedObject->ReleaseRef();
	}
}


void CImGuiManager::DrawLevelWindow()
{
	ImGui::SetNextWindowSize(ImVec2(300, 400));
	ImGui::Begin("Level Objects");

	static int selectedLayer = -1;  
	
	if (ImGui::Button("Add Object"))
	{
		CGameObject* newObj = new CGameObject();
		newObj->SetName(s2ws("Object " + std::to_string(objCounter++)));
		newObj->AddComponent(new CTransform());
		newObj->AddComponent(new CMeshRenderer());
		newObj->GetTransform()->SetRelativeScale(500.f, 500.f, 500.f);
		newObj->GetTransform()->SetRelativeRotation(0.f, 0.f, 0.f);
		newObj->GetTransform()->SetRelativePosition(100.f, 100.f, 100.f);
		newObj->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(L"Cube"));
		newObj->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(L"Kita"));
		CLevelManager::GetInst()->GetCurrentLevel()->AddGameObject(newObj, 3, false);

	}
	DrawLightCreateUI();

	ImGui::Text("Objects:");

	int index = 0;
	for (int j = 0; j < MAX_LAYER; ++j)
	{

		std::vector<CGameObject*> gameObjects;
		if (j == LAYER_MAP)
		{
			auto subLevel = CLevelManager::GetInst()->GetCurrentLevel()->m_SubLevel;
			if (subLevel)
				subLevel->PickGameObject(gameObjects);
		}
		else
			gameObjects = CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(j)->GetParentObjects();

		for (auto& gameObject : gameObjects)
		{
			std::string objName = ws2s(gameObject->GetName()); // Wide String -> String 변환
			objName = Trim(objName); // 공백 제거

			if (objName.size() == 1)
			{
				objName.clear(); // 강제로 문자열을 비워서 ID 충돌 방지
			}

			if (objName.empty()) // 이름이 비어 있으면 기본 이름 설정
			{
				objName = "Unnamed Object##" + std::to_string(index);
			}
			else
			{
				objName += "##" + std::to_string(index); // 기존 이름에도 고유 ID 추가 (ID 충돌 방지)
			}

			if (ImGui::Selectable(objName.c_str(), m_SelectedObject == gameObject))
			{
				m_SelectedObject = gameObject;
			}
			index++; // 객체마다 고유 ID 부여
		}
	}

	auto& windows = CLevelManager::GetInst()->GetCurrentLevel()->GetWidgetwindows();

	for (auto& window : windows)
	{
		if (!window)
			continue;

		std::vector<CGameObject*> gameObjects;

		for (int i = 0; i < window->GetWidgetCount(); ++i)
		{
			gameObjects.push_back(window->GetWidget(i));
		}

		for (auto& gameObject : gameObjects)
		{
			std::string objName = ws2s(gameObject->GetName()); // Wide String -> String 변환
			objName = Trim(objName); // 공백 제거

			if (objName.size() == 1)
			{
				objName.clear(); // 강제로 문자열을 비워서 ID 충돌 방지
			}

			if (objName.empty()) // 이름이 비어 있으면 기본 이름 설정
			{
				objName = "Unnamed Object##" + std::to_string(index);
			}
			else
			{
				objName += "##" + std::to_string(index); // 기존 이름에도 고유 ID 추가 (ID 충돌 방지)
			}

			if (ImGui::Selectable(objName.c_str(), m_SelectedObject == gameObject))
			{
				m_SelectedObject = gameObject;
			}
			index++; // 객체마다 고유 ID 부여
		}
	}

	if (m_SelectedObject && ImGui::Button("Delete Object"))
	{
		
		CLevelManager::GetInst()->GetCurrentLevel()->RemoveGameObject(m_SelectedObject);
		m_SelectedObject = nullptr;
	}

	ImGui::End();
}

void CImGuiManager::DrawInspectorWindow()
{
	if (!m_SelectedObject)
		return;

	ImGui::SetNextWindowSize(ImVec2(300, 400));
	ImGui::Begin("Inspector");

	// 이름 변경 UI
	DrawNameUI();

	// Gizmo 활성화 버튼
	static bool gizmoEnabled = true;
	ImGui::Checkbox("Enable Gizmo", &gizmoEnabled);

	// 태그 선택
	DrawTagUI();

	// 레이어 설정
	int currentLayer = m_SelectedObject->GetLayerIndex();
	if (ImGui::InputInt("Layer", &currentLayer))
	{
		currentLayer = std::clamp(currentLayer, 0, MAX_LAYER - 1);
		m_SelectedObject->SetLayerIndex(currentLayer);
	}

	// 정적 여부
	bool isStatic = m_SelectedObject->IsStatic();
	if (ImGui::Checkbox("Is Static", &isStatic))
		m_SelectedObject->SetStatic(isStatic);

	// 컴포넌트 추가 UI
	DrawComponentAddUI();

	// 트랜스폼 UI
	DrawTransformUI();

	// 조명 UI
	DrawLightUI();

	// Rigidbody UI
	DrawRigidbodyUI();

	// MeshRenderer UI
	DrawMeshRendererUI();

	ImGui::End();

	// Gizmo 활성화 시 그리기
	if (gizmoEnabled)
		DrawGizmo();
}

void CImGuiManager::DrawTagUI()
{
	static std::vector<std::wstring> tagList = { L"Default", L"Player", L"Enemy", L"NPC", L"Item" };
	std::wstring currentTag = m_SelectedObject->GetTag();
	int selectedTagIndex = std::find(tagList.begin(), tagList.end(), currentTag) - tagList.begin();

	if (ImGui::BeginCombo("Tag", ws2s(tagList[selectedTagIndex]).c_str()))
	{
		for (int i = 0; i < tagList.size(); ++i)
		{
			bool isSelected = (selectedTagIndex == i);
			if (ImGui::Selectable(ws2s(tagList[i]).c_str(), isSelected))
				m_SelectedObject->SetTag(tagList[i]);

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void CImGuiManager::DrawNameUI()
{
	// 오브젝트 이름 수정 기능 추가
	static char objNameBuffer[256]; // 이름을 저장할 버퍼 (최대 255자)
	std::string currentObjName = ws2s(m_SelectedObject->GetName());
	strncpy_s(objNameBuffer, currentObjName.c_str(), sizeof(objNameBuffer) - 1);
	objNameBuffer[sizeof(objNameBuffer) - 1] = '\0'; // 문자열 종료 보장

	ImGui::Text("Editing:");
	if (ImGui::InputText("##ObjectName", objNameBuffer, sizeof(objNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		// 사용자가 엔터를 눌렀을 때 이름 변경 적용
		m_SelectedObject->SetName(s2ws(std::string(objNameBuffer)));
	}

}

void CImGuiManager::DrawComponentAddUI()
{
	ImGui::Text("Add Component:");

	struct ComponentButton { const char* name; EComponent_Type type; };
	std::vector<ComponentButton> buttons = {
		//{"Collider", EComponent_Type::Collider},
		//{"Light", EComponent_Type::Light},
		{"Rigidbody", EComponent_Type::Rigidbody}
	};

	for (size_t i = 0; i < buttons.size(); ++i)
	{
		if (ImGui::Button(buttons[i].name))
			m_SelectedObject->AddComponent(buttons[i].type);

		//if ((i + 1) % 3 != 0) // 3개씩 한 줄
		//	ImGui::SameLine();
	}
}

void CImGuiManager::DrawTransformUI()
{
	ImGui::Text("Transform");

	Vec3 position = m_SelectedObject->GetTransform()->GetRelativePosition();
	Vec3 rotation = m_SelectedObject->GetTransform()->GetRelativeRotation();
	Vec3 scale = m_SelectedObject->GetTransform()->GetRelativeScale();

	if (ImGui::DragFloat3("Position", &position.x, 0.1f))
		m_SelectedObject->GetTransform()->SetRelativePosition(position);

	if (ImGui::DragFloat3("Rotation", &rotation.x, 1.0f))
		m_SelectedObject->GetTransform()->SetRelativeRotation(rotation);

	if (ImGui::DragFloat3("Scale", &scale.x, 0.1f))
		m_SelectedObject->GetTransform()->SetRelativeScale(scale);

	//// 기존 방식으로 만든 회전 행렬 (Euler 각 사용)
	//Matrix matRotationEuler = XMMatrixRotationX(rotation.x);
	//matRotationEuler *= XMMatrixRotationY(rotation.y);
	//matRotationEuler *= XMMatrixRotationZ(rotation.z);

	//Matrix matRotationQuat = XMMatrixRotationQuaternion(m_SelectedObject->GetTransform()->GetRelativeRotationQuaternion());
	//// 행렬 데이터를 가져오기
	//XMFLOAT4X4 matEuler, matQuat;
	//XMStoreFloat4x4(&matEuler, matRotationEuler);
	//XMStoreFloat4x4(&matQuat, matRotationQuat);

	//// Euler 방식의 회전 행렬 출력
	//ImGui::Text("Rotation Matrix (Euler)");
	//for (int i = 0; i < 4; i++)
	//{
	//	ImGui::Text("%f %f %f %f", matEuler.m[i][0], matEuler.m[i][1], matEuler.m[i][2], matEuler.m[i][3]);
	//}

	//// 쿼터니언 방식의 회전 행렬 출력
	//ImGui::Text("Rotation Matrix (Quaternion)");
	//for (int i = 0; i < 4; i++)
	//{
	//	ImGui::Text("%f %f %f %f", matQuat.m[i][0], matQuat.m[i][1], matQuat.m[i][2], matQuat.m[i][3]);
	//}
}

void CImGuiManager::DrawRigidbodyUI()
{
	CRigidBody* rigidbody = (CRigidBody*)m_SelectedObject->GetComponent(EComponent_Type::Rigidbody);
	if (!rigidbody) return;

	ImGui::Text("RigidBody Settings");

	bool useGravity = rigidbody->GetGravity();
	if (ImGui::Checkbox("Use Gravity", &useGravity))
		rigidbody->SetGravity(useGravity);

	bool isKinematic = rigidbody->GetKinematic();
	if (ImGui::Checkbox("Kinematic", &isKinematic))
		rigidbody->SetKinematic(isKinematic);

	float mass = rigidbody->GetMass();
	if (ImGui::DragFloat("Mass", &mass, 0.1f, 0.01f, 100.0f))
		rigidbody->SetMass(mass);

	float drag = rigidbody->GetDrag();
	if (ImGui::DragFloat("Drag", &drag, 0.01f, 0.0f, 10.0f))
		rigidbody->SetDrag(drag);

	float angularDrag = rigidbody->GetAngularDrag();
	if (ImGui::DragFloat("Angular Drag", &angularDrag, 0.01f, 0.0f, 10.0f))
		rigidbody->SetAngularDrag(angularDrag);

	Vector3 velocity = rigidbody->GetVelocity();
	if (ImGui::DragFloat3("Velocity", &velocity.x, 0.1f))
		rigidbody->SetVelocity(velocity);

	Vector3 angularVelocity = rigidbody->GetAngularVelocity();
	if (ImGui::DragFloat3("Angular Velocity", &angularVelocity.x, 0.1f))
		rigidbody->SetAngularVelocity(angularVelocity);

	static Vector3 force = { 0.0f, 0.0f, 0.0f };
	ImGui::DragFloat3("Force", &force.x, 0.1f);
	if (ImGui::Button("Apply Force"))
		rigidbody->ApplyForce(force);

	static Vector3 impulse = { 0.0f, 0.0f, 0.0f };
	ImGui::DragFloat3("Impulse", &impulse.x, 0.1f);
	if (ImGui::Button("Apply Impulse"))
		rigidbody->ApplyImpulse(impulse);

	static Vector3 torque = { 0.0f, 0.0f, 0.0f };
	ImGui::DragFloat3("Torque", &torque.x, 0.1f);
	if (ImGui::Button("Apply Torque"))
		rigidbody->ApplyTorque(torque);
}

void CImGuiManager::DrawMeshRendererUI()
{
	if (!m_SelectedObject->GetComponent(EComponent_Type::MeshRenderer)) return;

	ImGui::Text("MeshRenderer");
	std::vector<std::wstring> meshList = CAssetManager::GetInst()->GetAllAssetNames<CMesh>();
	static int selectedMeshIndex = 0;

	if (ImGui::BeginCombo("Mesh", ws2s(meshList[selectedMeshIndex]).c_str()))
	{
		for (int i = 0; i < meshList.size(); ++i)
		{
			if (ImGui::Selectable(ws2s(meshList[i]).c_str(), selectedMeshIndex == i))
			{
				selectedMeshIndex = i;
				m_SelectedObject->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(meshList[i]));
			}
		}
		ImGui::EndCombo();
	}

	std::vector<std::wstring> allMaterials = CAssetManager::GetInst()->GetAllAssetNames<CMaterial>();
	std::vector<std::wstring> filteredMaterialList;

	for (const auto& materialName : allMaterials)
	{
		auto material = CAssetManager::GetInst()->FindAsset<CMaterial>(materialName);
		if (material)
		{
			auto shader = material->GetGraphicsShader();
			if (shader && shader->GetShaderType() == SHADER_TYPE::DEFERRED)
			{
				filteredMaterialList.push_back(materialName);
			}
		}
	}

	static int selectedMaterialIndex = 0;
	if (!filteredMaterialList.empty() && selectedMaterialIndex >= filteredMaterialList.size())
	{
		selectedMaterialIndex = 0;
	}

	if (ImGui::BeginCombo("Material", ws2s(filteredMaterialList[selectedMaterialIndex]).c_str()))
	{
		for (int i = 0; i < filteredMaterialList.size(); ++i)
		{
			bool isSelected = (selectedMaterialIndex == i);
			if (ImGui::Selectable(ws2s(filteredMaterialList[i]).c_str(), isSelected))
			{
				selectedMaterialIndex = i;
				m_SelectedObject->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(filteredMaterialList[i]));
			}


		}
		ImGui::EndCombo();
	}
}

void CImGuiManager::DrawLightUI()
{
	CLight* light = (CLight*)m_SelectedObject->GetComponent(EComponent_Type::Light);
	if (!light) return;

	ImGui::Text("Light Settings");

	// Light 타입 변경
	static const char* lightTypes[] = { "Directional", "Point", "Spot" };
	LIGHT_TYPE currentType = light->GetLightType();
	int selectedType = static_cast<int>(currentType);

	if (ImGui::Combo("Light Type", &selectedType, lightTypes, IM_ARRAYSIZE(lightTypes)))
	{
		light->SetLightType(static_cast<LIGHT_TYPE>(selectedType));
	}

	// 색상 설정
	Vec3 diffuse = light->GetDiffuse();
	if (ImGui::ColorEdit3("Diffuse", &diffuse.x))
		light->SetDiffuse(diffuse);

	Vec3 ambient = light->GetAmbient();
	if (ImGui::ColorEdit3("Ambient", &ambient.x))
		light->SetAmbient(ambient);

	Vec3 specular = light->GetSpecular();
	if (ImGui::ColorEdit3("Specular", &specular.x))
		light->SetSpecular(specular);

	// 방향성 조명 (Directional Light) - 방향 설정
	if (currentType == LIGHT_TYPE::DIRECTIONAL_LIGHT)
	{
		Vec3 direction = Vec3(light->GetLightInfo().direction.x,
			light->GetLightInfo().direction.y,
			light->GetLightInfo().direction.z);
		if (ImGui::DragFloat3("Direction", &direction.x, 0.1f, -1.0f, 1.0f))
		{
			light->SetLightDirection(direction);
		}
	}

	// 거리 (Point Light, Spot Light)
	if (currentType == LIGHT_TYPE::POINT_LIGHT || currentType == LIGHT_TYPE::SPOT_LIGHT)
	{
		float range = light->GetLightInfo().range;
		if (ImGui::DragFloat("Range", &range, 0.1f, 0.1f, 100.0f))
		{
			light->SetLightRange(range);
		}
	}

	// 각도 (Spot Light)
	if (currentType == LIGHT_TYPE::SPOT_LIGHT)
	{
		float angle = light->GetLightInfo().angle;
		if (ImGui::DragFloat("Spot Angle", &angle, 0.1f, 1.0f, 90.0f))
		{
			light->SetLightAngle(angle);
		}
	}
}

void CImGuiManager::DrawGizmo()
{
	if (!m_SelectedObject)
		return;

	ImGuiIO& io = ImGui::GetIO();


	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));

	ImVec2 fixedSize = ImVec2(500.0f, 400.0f);
	ImGui::SetNextWindowSize(fixedSize, ImGuiCond_Always);

	// 창 이동 방지 및 크기 변경 방지
	static ImGuiWindowFlags window_flags = 0;


	ImGui::Begin("Gizmo", nullptr, window_flags);
	ImGuizmo::SetDrawlist();

	float windowWidth = ImGui::GetWindowWidth();
	float windowHeight = ImGui::GetWindowHeight();
	ImVec2 winPos = ImGui::GetWindowPos();
	ImGuizmo::SetRect(winPos.x, winPos.y, windowWidth, windowHeight);
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window_flags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;

	// 카메라 및 오브젝트 행렬 가져오기
	CCamera* camera = CRenderManager::GetInst()->GetMainCamera();
	Matrix viewMatrix = camera->GetViewMat();
	Matrix projectionMatrix = camera->GetProjMat();
	Matrix transform = m_SelectedObject->GetTransform()->GetWorldMatrix();
	static bool isManipulating = false;

	// 큐브 그리기
	ImGuizmo::DrawCubes(*viewMatrix.m, *projectionMatrix.m, *transform.m, 1);

	// ViewManipulate 위젯의 위치 및 크기 정의
	float viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
	float viewManipulateTop = ImGui::GetWindowPos().y;
	ImVec2 viewManipulatePos = ImVec2(viewManipulateRight - 128, viewManipulateTop);
	ImVec2 viewManipulateSize = ImVec2(128, 128);

	viewMatrix = viewMatrix.Invert();
	ImGuizmo::ViewManipulate(*viewMatrix.m, 7.f, viewManipulatePos, viewManipulateSize, 0x10101010);
	viewMatrix = viewMatrix.Invert();

	// 마우스를 클릭하면 조작 상태 활성화
	if (ImGui::IsMouseHoveringRect(viewManipulatePos, ImVec2(viewManipulatePos.x + 128, viewManipulatePos.y + 128))
		&& ImGui::IsMouseDown(0)) {
		isManipulating = true;
	}

	
	if (isManipulating) camera->SetCustomMatView(viewMatrix);

	if (ImGui::IsMouseReleased(0)) {
		isManipulating = false;
	}



	// Gizmo 모드 선택 (이동, 회전, 크기 조절)
	static ImGuizmo::OPERATION gizmoOperation = ImGuizmo::TRANSLATE;

	if (ImGui::RadioButton("Translate", gizmoOperation == ImGuizmo::TRANSLATE)) { gizmoOperation = ImGuizmo::TRANSLATE; }
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", gizmoOperation == ImGuizmo::ROTATE)) { gizmoOperation = ImGuizmo::ROTATE; }
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", gizmoOperation == ImGuizmo::SCALE)) { gizmoOperation = ImGuizmo::SCALE; }

	static bool useSnap = false;
	ImGui::Checkbox("Snap", &useSnap);

	static float snapTranslate[3] = { 0.1f, 0.1f, 0.1f };  // 이동 Snap (m 단위)
	static float snapRotate = 5.0f;  // 회전 Snap (각도)
	static float snapScale[3] = { 0.1f, 0.1f, 0.1f };  // 크기 Snap

	if (useSnap)
	{
		if (gizmoOperation == ImGuizmo::TRANSLATE)
			ImGui::InputFloat3("Translate Snap", snapTranslate);
		else if (gizmoOperation == ImGuizmo::ROTATE)
			ImGui::InputFloat("Rotate Snap (Degrees)", &snapRotate);
		else if (gizmoOperation == ImGuizmo::SCALE)
			ImGui::InputFloat3("Scale Snap", snapScale);
	}

	// Snap 값 설정
	float* snapValue = nullptr;
	if (useSnap)
	{
		if (gizmoOperation == ImGuizmo::TRANSLATE)
			snapValue = snapTranslate;
		else if (gizmoOperation == ImGuizmo::ROTATE)
			snapValue = &snapRotate;
		else if (gizmoOperation == ImGuizmo::SCALE)
			snapValue = snapScale;
	}
	SimpleMath::Vector3 scale, translation;
	SimpleMath::Quaternion rotation;

	// Gizmo Manipulate 실행
	if (ImGuizmo::Manipulate(*viewMatrix.m, *projectionMatrix.m,
		gizmoOperation, ImGuizmo::LOCAL, *transform.m, NULL, snapValue))
	{
		transform.Decompose(scale, rotation, translation);
		m_SelectedObject->GetTransform()->SetRelativePosition(translation);
		m_SelectedObject->GetTransform()->SetRelativeRotation(rotation);
		m_SelectedObject->GetTransform()->SetRelativeScale(scale);
	}




	ImGui::End();
	ImGui::PopStyleColor(1);
}

void CImGuiManager::DrawLightCreateUI()
{

	if (ImGui::Button("Add Directional Light"))
	{
		CreateLight(LIGHT_TYPE::DIRECTIONAL_LIGHT);
	}

	if (ImGui::Button("Add Point Light"))
	{
		CreateLight(LIGHT_TYPE::POINT_LIGHT);
	}

	if (ImGui::Button("Add Spot Light"))
	{
		CreateLight(LIGHT_TYPE::SPOT_LIGHT);
	}
}


void CImGuiManager::CreateLight(LIGHT_TYPE type)
{
	CGameObject* light = new CGameObject;
	light->SetName(L"Light");
	light->AddComponent(new CTransform);
	light->AddComponent(new CLight);
	CLight* lightComponent = (CLight*)light->GetComponent(EComponent_Type::Light);

	lightComponent->SetLightType(type);

	switch (type)
	{
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
		light->SetName(s2ws("Object " + std::to_string(objCounter++)));
		light->GetTransform()->SetRelativePosition(0.f, 500.f, 100.f);
		lightComponent->SetLightDirection(Vec3(0.f, -1.f, 0.5f));
		lightComponent->SetDiffuse(Vec3(0.5f, 0.5f, 0.5f));
		lightComponent->SetAmbient(Vec3(0.1f, 0.1f, 0.0f));
		lightComponent->SetSpecular(Vec3(0.5f, 0.5f, 0.5f));
		break;

	case LIGHT_TYPE::POINT_LIGHT:
		light->SetName(s2ws("Object " + std::to_string(objCounter++)));
		light->GetTransform()->SetRelativePosition(0.f, 10.f, 0.f);
		lightComponent->SetDiffuse(Vec3(1.0f, 0.5f, 0.5f));
		lightComponent->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		lightComponent->SetSpecular(Vec3(1.0f, 1.0f, 1.0f));
		lightComponent->SetLightRange(10.0f);
		break;

	case LIGHT_TYPE::SPOT_LIGHT:
		light->SetName(s2ws("Object " + std::to_string(objCounter++)));
		light->GetTransform()->SetRelativePosition(0.f, 15.f, 0.f);
		lightComponent->SetDiffuse(Vec3(0.5f, 0.5f, 1.0f));
		lightComponent->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		lightComponent->SetSpecular(Vec3(1.0f, 1.0f, 1.0f));
		lightComponent->SetLightRange(15.0f);
		lightComponent->SetLightAngle(45.0f);
		break;
	}

	// **렌더링 매니저에 등록**
	CRenderManager::GetInst()->RegisterLight(lightComponent);

	// 현재 레벨에 오브젝트 추가
	CLevelManager::GetInst()->GetCurrentLevel()->AddGameObject(light, 3, false);
}
