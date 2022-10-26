#include "BehaviorTree.h"
#include "../imgui/ImguiControl.h"

BehavirTree::RootBehavior BehavirTree::BehavierImGui::rootObject;
BehavirTree::Node::NodeManager BehavirTree::BehavierImGui::nodeManager;
char BehavirTree::BehavierImGui::nameBuf[256] = {};
const char* BehavirTree::BehavierImGui::nodeName = " ";
ImGuiWindowFlags BehavirTree::BehavierImGui::beharviorWindowFlags = 0;
ImVector<ImVec2> BehavirTree::BehavierImGui::behaviarWindowPosisions;
void BehavirTree::Node::GUIDraw()
{
	//ini��ǂݍ��ݏ������݂����Ȃ�
	_flags |= ImGuiWindowFlags_NoSavedSettings;

	ImVec4* style = ImGui::GetStyle().Colors;
	if (GetNodeType() == NodeType::e_Root)
	{
		style[ImGuiCol_TitleBg] = { 0.4f, 0.4f, 0.01f, 1.0f };
		style[ImGuiCol_TitleBgCollapsed] = { 0.4f, 0.4f, 0.01f, 1.0f };
		style[ImGuiCol_TitleBgActive] = { 0.4f, 0.4f, 0.01f, 1.0f };
	}
	else if (GetNodeType() == NodeType::e_Selector)
	{
		style[ImGuiCol_TitleBg] = { 0.01f, 0.4f, 0.4f, 1.0f };
		style[ImGuiCol_TitleBgCollapsed] = { 0.01f, 0.4f, 0.4f, 1.0f };
		style[ImGuiCol_TitleBgActive] = { 0.01f, 0.4f, 0.4f, 1.0f };
	}
	else if (GetNodeType() == NodeType::e_Sequence)
	{
		style[ImGuiCol_TitleBg] = { 0.31f, 0.31f, 0.7f, 1.0f };
		style[ImGuiCol_TitleBgCollapsed] = { 0.31f, 0.31f, 0.7f, 1.0f };
		style[ImGuiCol_TitleBgActive] = { 0.31f, 0.31f, 0.7f, 1.0f };
	}
	else if (GetNodeType() == NodeType::e_Task)
	{
		style[ImGuiCol_TitleBg] = { 0.4f, 0.01f, 0.4f, 1.0f };
		style[ImGuiCol_TitleBgCollapsed] = { 0.4f, 0.01f, 0.4f, 1.0f };
		style[ImGuiCol_TitleBgActive] = { 0.4f, 0.01f, 0.4f, 1.0f };
	}

	ImGui::Begin(GetName().c_str(), nullptr, _flags);//�E�B���h�E�̖��O
	ImGui::SetWindowSize(ImVec2(200.0f, 80.0f), ImGuiCond_::ImGuiCond_FirstUseEver);


	ImGui::DragInt("Priority", &_priority);


	//gui�̃E�B���h�E�擾
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	_windowPos = window->Pos;
	if (GetParent() != nullptr)
	{
		BehavierImGui::behaviarWindowPosisions.push_back(_parent->GetWindowPos());
		BehavierImGui::behaviarWindowPosisions.push_back(window->Pos);
	}

	ImGui::End();
}

void BehavirTree::BehavierImGui::CreateNode(const char* f_nodeName, NodeType f_type, Node* f_parent)
{
	if (nodeManager.nodes.size() == 0)
	{
		rootObject.SetName("RootNode");
		nodeManager.nodes.push_back(&rootObject);
	}
	if (f_type == BehavirTree::e_Selector)
	{
		Selector* selector = new Selector();
		selector->SetName(f_nodeName);
		SetParentAndChild(*f_parent, *selector);
		nodeManager.nodes.push_back(selector);
	}
	else if (f_type == BehavirTree::e_Sequence)
	{
		Sequence* sequence = new Sequence();
		sequence->SetName(f_nodeName);
		SetParentAndChild(*f_parent, *sequence);
		nodeManager.nodes.push_back(sequence);
	}
	else if (f_type == BehavirTree::e_Task)
	{
		Task* task = new Task();
		task->SetName(f_nodeName);
		SetParentAndChild(*f_parent, *task);
		nodeManager.nodes.push_back(task);
	}
}

void BehavirTree::BehavierImGui::DrawImGui()
{
	//�L�����p�X�̃X�N���[��
	static ImVec2 origin = { 0, 0 };
	behaviarWindowPosisions.clear();

	//�r�w�C�r�A�̐����{�^���p�E�B���h�E
	{
		ImGui::Begin("BehaviorWindow", nullptr, ImGuiWindowFlags_MenuBar);//�E�B���h�E�̖��O
		ImGui::SetWindowSize(ImVec2(static_cast<float>(200), static_cast<float>(200)), ImGuiCond_::ImGuiCond_FirstUseEver);
		if (ImGui::Button("Clear Nodes"))
		{
			ClearNodes();
		}
		ImGui::InputText(nodeName, nameBuf, 256);
		if (ImGui::Button("Add Selector"))
		{
			CreateNode(nameBuf, BehavirTree::NodeType::e_Selector, &rootObject);
		}
		if (ImGui::Button("Add Sequence"))
		{
			CreateNode(nameBuf, BehavirTree::NodeType::e_Sequence, &rootObject);
		}
		if (ImGui::Button("Add Task"))
		{
			CreateNode(nameBuf, BehavirTree::NodeType::e_Task, &rootObject);
		}
		ImGui::End();
	}

	BehavirTree::BehavierImGui::nodeManager.Draw();

	//�O���b�h�L�����p�X
	//�E�B���h�E�T�C�Y�͊e���Őݒ肵�Ă�������
	{
		//�E�B���h�E�T�C�Y����ʑS�̂�
		ImGui::SetNextWindowSize(ImVec2(static_cast<float>(window_width), static_cast<float>(window_height)), ImGuiCond_Appearing);
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
		beharviorWindowFlags |= ImGuiWindowFlags_NoBackground;
		beharviorWindowFlags |= ImGuiWindowFlags_NoCollapse;
		beharviorWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		ImGui::Begin("BehaviarTree", nullptr, beharviorWindowFlags);//�E�B���h�E�̖��O
		ImGui::SetWindowSize(ImVec2(static_cast<float>(window_width), static_cast<float>(window_height)), ImGuiCond_::ImGuiCond_FirstUseEver);

		// Draw border and background color
		ImGuiIO& io = ImGui::GetIO();

		static ImVector<ImVec2> points;
		static ImVec2 scrolling(0.0f, 0.0f);
		//
		static bool opt_enable_grid = true;
		static bool opt_enable_context_menu = true;
		static bool adding_line = false;
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
		if (opt_enable_grid)
		{
			//�O���b�h�T�C�Y
			const float GRID_STEP = 32.0f;
			for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
				draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
			for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
				draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
		}
		/*for (int n = 0; n < poss.Size; n += 2)
		{
			draw_list->AddLine(ImVec2(poss[n].x, poss[n].y), ImVec2(poss[n + 1].x, poss[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
		}*/
		for (int n = 0; n < behaviarWindowPosisions.Size; n += 2)
		{
			draw_list->AddLine(ImVec2(behaviarWindowPosisions[n].x, behaviarWindowPosisions[n].y), ImVec2(behaviarWindowPosisions[n + 1].x, behaviarWindowPosisions[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
		}
		//�`��I������
		draw_list->PopClipRect();
		ImGui::End();
	}
}

void BehavirTree::BehavierImGui::ClearNodes()
{
	rootObject.Children().clear();
	nodeManager.nodes.clear();
}
