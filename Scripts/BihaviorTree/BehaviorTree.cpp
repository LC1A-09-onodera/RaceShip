#include "BehaviorTree.h"
#include <sstream>
#include <fstream>
#include <iostream>

namespace BehaviorTree
{
	RootNode BehavierImGui::rootObject = RootNode();
	char BehavierImGui::nameBuf[256] = {};
	const char* BehavierImGui::nodeName = " ";
	char BehavierImGui::treeNameBuf[256] = {};
	const char* BehavierImGui::treeName = " ";

	ImGuiWindowFlags BehavierImGui::beharviorWindowFlags = 0;
	ImGuiWindowFlags BehavierImGui::beharviorButtonWindowFlags = 0;
	ImVector<ImVec2> BehavierImGui::behaviarWindowPosisions;
	Node* BehavierImGui::selectObject;
	const ImVec2 Node::WindowSize = { 200.0f, 80.0f };

	void Node::Init(Node* f_parent, string f_nodeName, NodeType f_type)
	{
		//�E�B���h�E�������e�m�[�h�Ƃǂꂾ�����炷����ݒ�
		static const ImVec2 WindowSponeDiff = { 220.0f, 30.0f };
		datas[defaultDatas[je_NodeName]] = f_nodeName;
		datas[defaultDatas[je_NodeType]] = f_type;
		//���[�g�I�u�W�F�N�g�̏ꍇ�e�m�[�h���Ȃ�
		if (f_parent == nullptr)
		{
			datas[defaultDatas[je_Priority]] = 0;
			datas[defaultDatas[je_WindowPos]] = { 100.0f, 30.0f };
			return;
		}
		datas[defaultDatas[je_Priority]] = static_cast<int>(f_parent->Children()->size());
		datas[defaultDatas[je_WindowPos]] = { f_parent->datas[defaultDatas[je_WindowPos]][0] + WindowSponeDiff.x, f_parent->datas[defaultDatas[je_WindowPos]][1] + WindowSponeDiff.y + static_cast<float>(f_parent->Children()->size()) * WindowSize.y };
		SetParentAndChild(*f_parent, *this);
	}

	void Node::GUIDraw()
	{
		ImGuiContext& g = *GImGui;

		//ini��ǂݍ��ݏ������݂����Ȃ�
		_flags |= ImGuiWindowFlags_NoSavedSettings;

		ImVec4* style = ImGui::GetStyle().Colors;
		//�m�[�h�̃J���[�ύX
		{
			//NONE�m�[�h�͊�{�Ȃ��Ȃ̂ŕ`�悵�Ȃ�
			if (datas[defaultDatas[je_NodeType]] == NodeType::e_NONE)return;
			if (datas[defaultDatas[je_NodeType]] == NodeType::e_Root)
			{
				style[ImGuiCol_TitleBg] = { 0.3f, 0.3f, 0.01f, 1.0f };
				style[ImGuiCol_TitleBgCollapsed] = { 0.35f, 0.35f, 0.01f, 1.0f };
				style[ImGuiCol_TitleBgActive] = { 0.5f, 0.5f, 0.01f, 1.0f };
			}
			else if (datas[defaultDatas[je_NodeType]] == NodeType::e_Selector)
			{
				style[ImGuiCol_TitleBg] = { 0.01f, 0.3f, 0.3f, 1.0f };
				style[ImGuiCol_TitleBgCollapsed] = { 0.01f, 0.35f, 0.35f, 1.0f };
				style[ImGuiCol_TitleBgActive] = { 0.01f, 0.5f, 0.5f, 1.0f };
			}
			else if (datas[defaultDatas[je_NodeType]] == NodeType::e_Sequence)
			{
				style[ImGuiCol_TitleBg] = { 0.21f, 0.21f, 0.6f, 1.0f };
				style[ImGuiCol_TitleBgCollapsed] = { 0.25f, 0.25f, 0.65f, 1.0f };
				style[ImGuiCol_TitleBgActive] = { 0.4f, 0.4f, 0.75f, 1.0f };
			}
			else if (datas[defaultDatas[je_NodeType]] == NodeType::e_Task)
			{
				style[ImGuiCol_TitleBg] = { 0.3f, 0.01f, 0.3f, 1.0f };
				style[ImGuiCol_TitleBgCollapsed] = { 0.35f, 0.01f, 0.35f, 1.0f };
				style[ImGuiCol_TitleBgActive] = { 0.5f, 0.01f, 0.5f, 1.0f };
			}
		}
		//���W�̐ݒ�
		ImGui::SetNextWindowPos({ datas[defaultDatas[je_WindowPos]][0],datas[defaultDatas[je_WindowPos]][1] }, ImGuiCond_Appearing);
		string titlebarName = datas[defaultDatas[je_NodeName]];
		ImGui::Begin(titlebarName.c_str(), nullptr, _flags);//�E�B���h�E�̖��O
		ImGui::SetWindowSize(WindowSize, ImGuiCond_::ImGuiCond_FirstUseEver);
		if (static_cast<string>(datas[defaultDatas[je_NodeName]]) == (GImGui)->Windows[GImGui->Windows.size() - 1]->Name)
		{
			BehavierImGui::selectObject = this;
		}

		int priority = datas[defaultDatas[je_Priority]];
		ImGui::DragInt("Priority", &priority);
		datas[defaultDatas[je_Priority]] = priority;

		//�m�[�h�ɕ\��������
		{
			//json�f�[�^�ɃA�N�Z�X
			for (auto& [key, value] : datas.items())
			{
				string tex = key + ":" + to_string(datas[key]);
				ImGui::Text(tex.c_str());
			}
		}

		//gui�̃E�B���h�E�擾
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		datas[defaultDatas[je_WindowPos]] = {window->Pos.x, window->Pos.y};
		//_windowPos = window->Pos;
		if (GetParent() != nullptr)
		{
			//�e�q����Ō��Ԃ��߂̍��W�ۊ�
			BehavierImGui::behaviarWindowPosisions.push_back({_parent->GetData(defaultDatas[je_WindowPos])[0], _parent->GetData(defaultDatas[je_WindowPos])[1] });
			BehavierImGui::behaviarWindowPosisions.push_back(window->Pos);
		}

		ImGui::End();
	}
	Node* Node::SearchParentNode(string f_nodeName)
	{

		return nullptr;
	}

	Node* Node::SearchChildrenNode(Node* f_node, string f_nodeName)
	{
		//���O��v���Ă��炻�̃I�u�W�F�N�g��Ԃ�
		if (f_node->GetData(defaultDatas[je_NodeName]) == f_nodeName)
		{
			return f_node;
		}
		//�q�����Ȃ���Ζ߂�
		if (f_node->Children()->size() < 1) return nullptr;

		for each (Node * node in *f_node->Children())
		{
			return SearchChildrenNode(node, f_nodeName);
		}
		return nullptr;
	}

	void Node::DrawGUIChildren(Node* f_node)
	{
		//�m�[�h�̕`��
		f_node->GUIDraw();

		//�q�����Ȃ���Ζ߂�
		if (f_node->Children()->size() < 1) return;

		for each (Node * node in *f_node->Children())
		{
			node->DrawGUIChildren(node);
		}
	}

	void Node::ToJson(json& j)
	{
		if (_parent == nullptr)
		{
			j = json{
				{"NodeType", datas[defaultDatas[je_NodeType]]},
				{"NodeName", datas[defaultDatas[je_NodeName]]},
				{"Priority", datas[defaultDatas[je_Priority]]},
				{"WindowPosition", {_windowPos.x, _windowPos.y}},
			};
		}
		else
		{
			j = json{
				{"NodeType", datas[defaultDatas[je_NodeType]]},
				{"NodeName", datas[defaultDatas[je_NodeName]]},
				{"ParentName", _parent->GetData(defaultDatas[je_NodeName])},
				{"Priority", datas[defaultDatas[je_Priority]]},
				{"WindowPosition", {_windowPos.x, _windowPos.y}},

			};
		}
	}

	void Node::FromJson(json& j)
	{
		j.at("NodeType").get_to(datas[defaultDatas[je_NodeType]]);
		j.at("NodeName").get_to(datas[defaultDatas[je_NodeName]]);
		j.at("Priority").get_to(datas[defaultDatas[je_Priority]]);
		string parentName;
		j.at("ParentName").get_to(parentName);
		j.at("WindowPosition").get_to(datas[defaultDatas[je_WindowPos]]);
	}

	void Node::JsonUpdate()
	{

	}

	void BehavierImGui::CreateNode(const char* f_nodeName, NodeType f_type, Node* f_parent)
	{
		//�ŏ��̃m�[�h����̎��̏���
		{
			if (selectObject == nullptr)
			{
				rootObject.Init(nullptr, "RootNode", e_Root);
				rootObject.GetData(defaultDatas[je_WindowPos]) = { 100.0f, 30.0f };
				selectObject = &rootObject;
			}
			if (f_parent == nullptr)
			{
				f_parent = new Node();
				f_parent = selectObject;
			}
		}
		//���ۂɃm�[�h�����
		{
			//�^�X�N�m�[�h�̉��Ƀm�[�h����点�Ȃ�
			if (f_parent->GetData(defaultDatas[je_NodeType]) == NodeType::e_Task)return;
			Node* node = new Node();
			if (f_type == e_Selector)
			{
				node->Init(f_parent, f_nodeName, e_Selector);
			}
			else if (f_type == e_Sequence)
			{
				node->Init(f_parent, f_nodeName, e_Sequence);
			}
			else if (f_type == e_Task)
			{
				node->Init(f_parent, f_nodeName, e_Task);
			}
			else
			{
				node->Init(f_parent, f_nodeName, e_NONE);
			}
		}
	}

	void BehavierImGui::DrawImGui()
	{
		//�L�����p�X�̃X�N���[��
		static ImVec2 origin = { 0, 0 };
		behaviarWindowPosisions.clear();

		//�r�w�C�r�A�̐����{�^���p�E�B���h�E
		{
			ImVec4* style = ImGui::GetStyle().Colors;
			style[ImGuiCol_TitleBg] = { 0.3f, 0.3f, 0.3f, 1.0f };
			style[ImGuiCol_TitleBgCollapsed] = { 0.3f, 0.3f, 0.3f, 1.0f };
			style[ImGuiCol_TitleBgActive] = { 0.3f, 0.3f, 0.3f, 1.0f };
			ImGui::Begin("BehaviorWindow", nullptr, beharviorButtonWindowFlags);//�E�B���h�E�̖��O
			ImGui::SetWindowSize(ImVec2(static_cast<float>(200), static_cast<float>(200)), ImGuiCond_::ImGuiCond_FirstUseEver);
			if (ImGui::Button("Clear Nodes"))
			{
				ClearNodes();

			}
			if (ImGui::TreeNode("Tree Name"))
			{
				ImGui::InputText(treeName, treeNameBuf, 256);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Node Name"))
			{
				ImGui::InputText(nodeName, nameBuf, 256);
				ImGui::TreePop();
			}
			if (ImGui::Button("Add Selector"))
			{
				CreateNode(nameBuf, NodeType::e_Selector, selectObject);
			}
			if (ImGui::Button("Add Sequence"))
			{
				CreateNode(nameBuf, NodeType::e_Sequence, selectObject);
			}
			if (ImGui::Button("Add Task"))
			{
				CreateNode(nameBuf, NodeType::e_Task, selectObject);
			}

			if (ImGui::Button("Behavior Export"))
			{
				//rootObject.ExportFile("smp");
			}
			if (ImGui::Button("Behavior Inport"))
			{
				rootObject.InportFile("smp");
			}
			ImGui::End();
		}

		//�X�̃m�[�h��`��
		rootObject.Init(nullptr, "RootNode", e_Root);
		rootObject.DrawGUIChildren(&rootObject);

		//�O���b�h�L�����p�X
		//�E�B���h�E�T�C�Y�͊e���Őݒ肵�Ă�������
		{
			//�E�B���h�E�T�C�Y����ʑS�̂�
			static const float windowWid = 1280;
			static const float windowHi = 720;
			ImGui::SetNextWindowSize(ImVec2(windowWid, windowHi), ImGuiCond_Appearing);
			//���W�������
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Appearing);
			//�E�B���h�E�̔w�i�ƊO�g��`�悵�Ȃ�
			beharviorWindowFlags |= ImGuiWindowFlags_NoBackground;
			//�^�C�g���o�[��t���Ȃ�
			beharviorWindowFlags |= ImGuiWindowFlags_NoTitleBar;
			//�E���̃T�C�Y�ύX���o���Ȃ�����
			beharviorWindowFlags |= ImGuiWindowFlags_NoResize;
			//�����Ȃ��悤�ɂ���
			beharviorWindowFlags |= ImGuiWindowFlags_NoMove;
			//�őO�ʂɗ��Ȃ��悤��
			beharviorWindowFlags |= ImGuiWindowFlags_NoFocusOnAppearing;
			//ini��ǂݍ��ݏ������݂����Ȃ�
			beharviorWindowFlags |= ImGuiWindowFlags_NoSavedSettings;
			//�w�i�ɂ����Ȃ��H
			beharviorWindowFlags |= ImGuiWindowFlags_NoBackground;
			beharviorWindowFlags |= ImGuiWindowFlags_NoCollapse;
			//�t�H�[�J�X���󂯂Ă��O�ɗ��Ȃ�(�m�[�h���O�ɗ��Ȃ�)
			beharviorWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			ImGui::Begin("BehaviarTree", nullptr, beharviorWindowFlags);//�E�B���h�E�̖��O
			ImGui::SetWindowSize(ImVec2(windowWid, windowHi), ImGuiCond_::ImGuiCond_FirstUseEver);

			//�}�E�X���͂Ȃǂ��擾
			ImGuiIO& io = ImGui::GetIO();

			static ImVector<ImVec2> points;
			static ImVec2 scrolling(0.0f, 0.0f);

			static bool opt_enable_context_menu = true;
			// Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
			ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
			ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
			float hoge = 0.0f;
			if (canvas_sz.x < hoge) canvas_sz.x = hoge;
			if (canvas_sz.y < hoge) canvas_sz.y = hoge;
			ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			//�w�i�F
			draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
			//?
			draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(0, 0, 2, 255));
			//��ʂ��{�^���Ƃ��Č����ĂĂ�H
			//�}�E�X�̓��̓L�[�̎擾���������̂���
			ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); // Hovered
			const bool is_active = ImGui::IsItemActive();   // Held
			origin = { canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y }; // Lock scrolled origin
			//�L�����p�X���ł̃}�E�X�ʒu

			//�X�N���[��
			const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
			if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
			{
				scrolling.x += io.MouseDelta.x;
				scrolling.y += io.MouseDelta.y;
			}
			ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
			if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
				ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

			// Draw grid + all lines in the canvas
			//�`��O����
			draw_list->PushClipRect(canvas_p0, canvas_p1, true);

			//�O���b�h�̕`��
			{
				//�O���b�h�T�C�Y
				const float GRID_STEP = 32.0f;
				for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
					draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
				for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
					draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));

				for (int n = 0; n < behaviarWindowPosisions.Size; n += 2)
				{
					draw_list->AddLine(ImVec2(behaviarWindowPosisions[n].x, behaviarWindowPosisions[n].y), ImVec2(behaviarWindowPosisions[n + 1].x, behaviarWindowPosisions[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
				}
			}
			//�`��I������
			draw_list->PopClipRect();
			ImGui::End();
		}
	}

	void BehavierImGui::ClearNodes()
	{
		rootObject.Children()->clear();
		//nodeManager.nodes.clear();
	}

	void ExportFile(string f_fileName)
	{
		string particleFileName = f_fileName;
		//�t�@�C�������
		string saveFileName = "Resource/TextData/Behavior/" + particleFileName + ".csv";
		ofstream ofs(saveFileName);
		string dataStr = "";
		json js;

		ofs << dataStr;
	}

	void RootNode::InportFile(string f_fileName)
	{
		//�t�@�C���ǂݍ���
		string pathName = f_fileName;
		string fullPath = "Resource/TextData/Behavior/" + pathName + ".csv";

		ifstream file;
		file.open(fullPath);
		if (file.fail())
		{
			assert(0);
		}
		string keyType;
		string allFileData = "";

		while (getline(file, keyType))
		{
			allFileData += keyType;
		}
		json loadData = json::parse(allFileData);
		this->FromJson(loadData);
	}

	void RootNode::DrawGUIChildren(Node* f_node)
	{
		//�m�[�h�̕`��
		f_node->GUIDraw();

		//�q�����Ȃ���Ζ߂�
		if (f_node->Children()->size() < 1) return;

		for each (Node * node in *f_node->Children())
		{
			node->DrawGUIChildren(node);
		}
	}
}