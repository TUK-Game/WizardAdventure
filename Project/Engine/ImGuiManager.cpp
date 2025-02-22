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
		//CEngine::GetInst()->GetCurrentLevel()->RemoveGameObject(selectedObject);
		//selectedObject = nullptr;
	}

	ImGui::End();
}

void CImGuiManager::DrawInspectorWindow()
{
	if (!m_SelectedObject)
		return;

	ImGui::Begin("Inspector");
	std::string objName = ws2s(m_SelectedObject->GetName());
	ImGui::Text("Editing: %s", objName.c_str());

	bool isStatic = m_SelectedObject->IsStatic();
	if (ImGui::Checkbox("Is Static", &isStatic))
	{
		m_SelectedObject->SetStatic(isStatic);
	}

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



	ImGui::End();
}
