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
#include "../../3rdParty/ImGuizmo/ImGuizmo.h"
#include "../../3rdParty/ImGui/imgui_internal.h"

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
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GRAPHICS_CMD_LIST.Get());
}

void CImGuiManager::CleanUp()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (m_SelectedObject)
	{
		delete m_SelectedObject;
		m_SelectedObject = nullptr;
	}
}


void CImGuiManager::DrawLevelWindow()
{
	ImGui::SetNextWindowSize(ImVec2(300, 400));
	ImGui::Begin("Level Objects");

	static int selectedLayer = -1;  
	
	if (ImGui::Button("Add Object"))
	{
		static int objCounter = 0;
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
	

	int index = 0;
	for (int j = 0; j < MAX_LAYER; ++j)
	{
		auto& gameObjects = CLevelManager::GetInst()->GetCurrentLevel()->GetLayer(j)->GetObjects();

		for (auto& gameObject : gameObjects)
		{
			std::string objName = ws2s(gameObject->GetName()); // Wide String -> String ��ȯ
			objName = Trim(objName); // ���� ����

			if (objName.size() == 1)
			{
				objName.clear(); // ������ ���ڿ��� ����� ID �浹 ����
			}

			if (objName.empty()) // �̸��� ��� ������ �⺻ �̸� ����
			{
				objName = "Unnamed Object##" + std::to_string(index);
			}
			else
			{
				objName += "##" + std::to_string(index); // ���� �̸����� ���� ID �߰� (ID �浹 ����)
			}

			if (ImGui::Selectable(objName.c_str(), m_SelectedObject == gameObject))
			{
				m_SelectedObject = gameObject;
			}
			index++; // ��ü���� ���� ID �ο�
		}
	}

	if (m_SelectedObject && ImGui::Button("Delete Object"))
	{
		//CEngine::GetInst()->GetCurrentLevel()->RemoveGameObject(selectedObject);
		//selectedObject = nullptr;
	}

	ImGui::End();
}

void CImGuiManager::DrawInspectorWindow()
{
	if (!m_SelectedObject)
		return;

	ImGui::SetNextWindowSize(ImVec2(300, 400));
	ImGui::Begin("Inspector");
	std::string objName = ws2s(m_SelectedObject->GetName());
	ImGui::Text("Editing: %s", objName.c_str());

	// Gizmo Ȱ��ȭ ��ư �߰�
	static bool gizmoEnabled = true;
	ImGui::Checkbox("Enable Gizmo", &gizmoEnabled);

	// Tag
	static std::vector<std::wstring> tagList = { L"Default", L"Player", L"Enemy", L"NPC", L"Item" };
	std::wstring currentTag = m_SelectedObject->GetTag();
	int selectedTagIndex = std::find(tagList.begin(), tagList.end(), currentTag) - tagList.begin();

	if (ImGui::BeginCombo("Tag", ws2s(tagList[selectedTagIndex]).c_str()))
	{
		for (int i = 0; i < tagList.size(); ++i)
		{
			bool isSelected = (selectedTagIndex == i);
			if (ImGui::Selectable(ws2s(tagList[i]).c_str(), isSelected))
			{
				m_SelectedObject->SetTag(tagList[i]);
			}
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	// Layer
	int currentLayer = m_SelectedObject->GetLayerIndex();
	if (ImGui::InputInt("Layer", &currentLayer))
	{
		currentLayer = std::clamp(currentLayer, 0, MAX_LAYER - 1);
		m_SelectedObject->SetLayerIndex(currentLayer);
	}

	// Static
	bool isStatic = m_SelectedObject->IsStatic();
	if (ImGui::Checkbox("Is Static", &isStatic))
	{
		m_SelectedObject->SetStatic(isStatic);
	}


	// SRT
	Vec3 position = m_SelectedObject->GetTransform()->GetRelativePosition();
	Vec3 rotation = m_SelectedObject->GetTransform()->GetRelativeRotation();
	Vec3 scale = m_SelectedObject->GetTransform()->GetRelativeScale();

	if (ImGui::DragFloat3("Position", &position.x, 0.1f))
	{
		m_SelectedObject->GetTransform()->SetRelativePosition(position);
	}

	if (ImGui::DragFloat3("Rotation", &rotation.x, 1.0f))
	{
		m_SelectedObject->GetTransform()->SetRelativeRotation(rotation);
	}

	if (ImGui::DragFloat3("Scale", &scale.x, 0.1f))
	{
		m_SelectedObject->GetTransform()->SetRelativeScale(scale);
	}


	 // Mesh 
	std::vector<std::wstring> meshList = CAssetManager::GetInst()->GetAllAssetNames<CMesh>();
	static int selectedMeshIndex = 0;

    if (ImGui::BeginCombo("Mesh", ws2s(meshList[selectedMeshIndex]).c_str()))
    {
        for (int i = 0; i < meshList.size(); ++i)
        {
            bool isSelected = (selectedMeshIndex == i);
            if (ImGui::Selectable(ws2s(meshList[i]).c_str(), isSelected))
            {
                selectedMeshIndex = i;
                m_SelectedObject->GetMeshRenderer()->SetMesh(CAssetManager::GetInst()->FindAsset<CMesh>(meshList[i]));
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // Material 
	std::vector<std::wstring> materialList = CAssetManager::GetInst()->GetAllAssetNames<CMaterial>();
	static int selectedMaterialIndex = 0;

    if (ImGui::BeginCombo("Material", ws2s(materialList[selectedMaterialIndex]).c_str()))
    {
        for (int i = 0; i < materialList.size(); ++i)
        {
            bool isSelected = (selectedMaterialIndex == i);
            if (ImGui::Selectable(ws2s(materialList[i]).c_str(), isSelected))
            {
                selectedMaterialIndex = i;
                m_SelectedObject->GetMeshRenderer()->SetMaterial(CAssetManager::GetInst()->FindAsset<CMaterial>(materialList[i]));
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

	if (ImGuizmo::IsUsing())
	{
		ImGui::Text("Using gizmo");
	}
	else
	{
		ImGui::Text(ImGuizmo::IsOver() ? "Over gizmo" : "");
		ImGui::SameLine();
		ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");
		ImGui::SameLine();
		ImGui::Text(ImGuizmo::IsOver(ImGuizmo::ROTATE) ? "Over rotate gizmo" : "");
		ImGui::SameLine();
		ImGui::Text(ImGuizmo::IsOver(ImGuizmo::SCALE) ? "Over scale gizmo" : "");
	}


	ImGui::End();

	// Gizmo Ȱ��ȭ ���¶�� DrawGizmo ȣ��
	if (gizmoEnabled)
	{
		DrawGizmo();
	}

}

//void CImGuiManager::DrawGizmo()
//{
//	if (!m_SelectedObject)
//		return;
//
//	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
//	ImGui::Begin("Gizmo");
//	ImGuizmo::SetDrawlist();
//
//	float windowWidth = ImGui::GetWindowWidth();
//	float windowHeight = ImGui::GetWindowHeight();
//	ImVec2 winPos = ImGui::GetWindowPos();
//	ImGuizmo::SetRect(winPos.x, winPos.y, windowWidth, windowHeight);
//
//	// ī�޶� �� ������Ʈ ��� ��������
//	CCamera* camera = CRenderManager::GetInst()->GetMainCamera();
//	Matrix viewMatrix = camera->GetViewMat();
//	Matrix projectionMatrix = camera->GetProjMat();
//	Matrix transform = m_SelectedObject->GetTransform()->GetWorldMatrix();
//
//	// Gizmo Manipulate ����
//	static bool useSnap = false;
//	static float snap[3] = { 0.1f, 0.1f, 0.1f };
//
//		//  Gizmo ���� (�̵�, ȸ��, ũ�� ����)
//	if (ImGuizmo::Manipulate(*viewMatrix.m, *projectionMatrix.m,
//		ImGuizmo::TRANSLATE, ImGuizmo::WORLD, *transform.m, NULL, useSnap ? &snap[0] : NULL))
//	{
//		//  ��ȯ�� Transform ���� ������ GameObject�� ����
//		DirectX::SimpleMath::Vector3 newPosition, newRotation, newScale;
//		ImGuizmo::DecomposeMatrixToComponents(MatrixToFloatPtr(transform),
//			&newPosition.x, &newRotation.x, &newScale.x);
//
//		//  ��ȯ�� ���� �ٽ� ����
//		m_SelectedObject->GetTransform()->SetRelativePosition(newPosition);
//		m_SelectedObject->GetTransform()->SetRelativeRotation(newRotation);
//		m_SelectedObject->GetTransform()->SetRelativeScale(newScale);
//
//		m_SelectedObject->GetTransform()->SetWorldMatrix(transform);
//	}
//
//	ImGui::End();
//	ImGui::PopStyleColor(1);
//}


void CImGuiManager::DrawGizmo()
{
	if (!m_SelectedObject)
		return;

	ImGuiIO& io = ImGui::GetIO();

	// Gizmo�� Ȱ��ȭ�Ǹ� ImGui�� ���콺 �Է��� ����
	bool gizmoActive = ImGuizmo::IsOver() || ImGuizmo::IsUsing();
	if (gizmoActive)
	{
		io.WantCaptureMouse = false;
	}

	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));

	ImVec2 fixedSize = ImVec2(500.0f, 400.0f);
	ImGui::SetNextWindowSize(fixedSize, ImGuiCond_Always);

	// â �̵� ���� �� ũ�� ���� ����
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

	if (gizmoActive)
	{
		window_flags |= ImGuiWindowFlags_NoMove;
	}

	ImGui::Begin("Gizmo", nullptr, window_flags);
	ImGuizmo::SetDrawlist();

	float windowWidth = ImGui::GetWindowWidth();
	float windowHeight = ImGui::GetWindowHeight();
	ImVec2 winPos = ImGui::GetWindowPos();
	ImGuizmo::SetRect(winPos.x, winPos.y, windowWidth, windowHeight);

	// ī�޶� �� ������Ʈ ��� ��������
	CCamera* camera = CRenderManager::GetInst()->GetMainCamera();
	Matrix viewMatrix = camera->GetViewMat();
	Matrix projectionMatrix = camera->GetProjMat();
	Matrix transform = m_SelectedObject->GetTransform()->GetWorldMatrix();

	// Gizmo Manipulate ����
	static bool useSnap = false;
	static float snap[3] = { 0.1f, 0.1f, 0.1f };

	if (ImGuizmo::Manipulate(*viewMatrix.m, *projectionMatrix.m,
		ImGuizmo::TRANSLATE, ImGuizmo::WORLD, *transform.m, NULL, useSnap ? &snap[0] : NULL))
	{
		// ��ȯ�� Transform ���� ������ GameObject�� ����
		DirectX::SimpleMath::Vector3 newPosition, newRotation, newScale;
		ImGuizmo::DecomposeMatrixToComponents(MatrixToFloatPtr(transform),
			&newPosition.x, &newRotation.x, &newScale.x);

		// ��ȯ�� ���� �ٽ� ����
		m_SelectedObject->GetTransform()->SetRelativePosition(newPosition);
		m_SelectedObject->GetTransform()->SetRelativeRotation(newRotation);
		m_SelectedObject->GetTransform()->SetRelativeScale(newScale);

		m_SelectedObject->GetTransform()->SetWorldMatrix(transform);
	}

	ImGui::End();
	ImGui::PopStyleColor(1);
}
