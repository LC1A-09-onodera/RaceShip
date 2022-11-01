#include "BehaviorTree.h"
#include <sstream>
#include <fstream>
#include <iostream>

namespace BehaviorTree
{
	Node BehavierImGui::rootObject = Node();
	char BehavierImGui::nameBuf[256] = {};
	const char* BehavierImGui::nodeName = " ";
	char BehavierImGui::treeNameBuf[256] = {};
	const char* BehavierImGui::treeName = " ";

	ImGuiWindowFlags BehavierImGui::beharviorWindowFlags = 0;
	ImGuiWindowFlags BehavierImGui::beharviorButtonWindowFlags = 0;
	ImVector<ImVec2> BehavierImGui::behaviarWindowPosisions;
	Node* BehavierImGui::selectObject;
	const ImVec2 Node::WindowSize = { 200.0f, 160.0f };

	void Node::Init(Node* f_parent, string f_nodeName, NodeType f_type)
	{
		//ウィンドウ生成時親ノードとどれだけずらすかを設定
		static const ImVec2 WindowSponeDiff = { 220.0f, 30.0f };
		datas[defaultDatas[je_NodeName]] = f_nodeName;
		datas[defaultDatas[je_NodeType]] = f_type;
		//ルートオブジェクトの場合親ノードがない
		if (f_parent == nullptr)
		{
			datas[defaultDatas[je_Priority]] = 0;
			datas[defaultDatas[je_WindowPos]] = { 100.0f, 30.0f };
			return;
		}
		datas[defaultDatas[je_Priority]] = static_cast<int>(f_parent->Children()->size());
		datas[defaultDatas[je_WindowPos]] = { f_parent->datas[defaultDatas[je_WindowPos]][0] + WindowSponeDiff.x, f_parent->datas[defaultDatas[je_WindowPos]][1] + WindowSponeDiff.y + static_cast<float>(f_parent->Children()->size()) * WindowSize.y };
		datas[defaultDatas[je_ParentName]] = f_parent->datas[defaultDatas[je_NodeName]];
		SetParentAndChild(*f_parent, *this);
	}

	void Node::GUIDraw()
	{
		ImGuiContext& g = *GImGui;

		//iniを読み込み書き込みをしない
		_flags |= ImGuiWindowFlags_NoSavedSettings;

		ImVec4* style = ImGui::GetStyle().Colors;
		//ノードのカラー変更
		{
			//NONEノードは基本なしなので描画しない
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
		//座標の設定
		ImGui::SetNextWindowPos({ datas[defaultDatas[je_WindowPos]][0],datas[defaultDatas[je_WindowPos]][1] }, ImGuiCond_Appearing);
		string titlebarName = datas[defaultDatas[je_NodeName]];
		ImGui::Begin(titlebarName.c_str(), nullptr, _flags);//ウィンドウの名前
		ImGui::SetWindowSize(WindowSize, ImGuiCond_::ImGuiCond_FirstUseEver);
		if (static_cast<string>(datas[defaultDatas[je_NodeName]]) == (GImGui)->Windows[GImGui->Windows.size() - 1]->Name)
		{
			BehavierImGui::selectObject = this;
		}

		int priority = datas[defaultDatas[je_Priority]];
		ImGui::DragInt("Priority", &priority);
		datas[defaultDatas[je_Priority]] = priority;

		//ノードに表示する情報
		{
			//jsonデータにアクセス
			for (auto& [key, value] : datas.items())
			{
				string tex = key + ":" + to_string(datas[key]);
				ImGui::Text(tex.c_str());
			}
		}

		//guiのウィンドウ取得
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		datas[defaultDatas[je_WindowPos]] = {window->Pos.x, window->Pos.y};
		//_windowPos = window->Pos;
		if (GetParent() != nullptr)
		{
			//親子を線で結ぶための座標保管
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
		//名前一致してたらそのオブジェクトを返す
		if (f_node->GetData(defaultDatas[je_NodeName]) == f_nodeName)
		{
			return f_node;
		}
		//子がいなければ戻る
		if (f_node->Children()->size() < 1) return nullptr;

		for each (Node * node in *f_node->Children())
		{
			return SearchChildrenNode(node, f_nodeName);
		}
		return nullptr;
	}

	void Node::DrawGUIChildren(Node* f_node)
	{
		//ノードの描画
		f_node->GUIDraw();

		//子がいなければ戻る
		if (f_node->Children()->size() < 1) return;

		for each (Node * node in *f_node->Children())
		{
			node->DrawGUIChildren(node);
		}
	}

	void BehavierImGui::CreateNode(const char* f_nodeName, NodeType f_type, Node* f_parent)
	{
		//最初のノードつくりの時の処理
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
		//実際にノードを作る
		{
			//タスクノードの下にノードを作らせない
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
		//キャンパスのスクロール
		static ImVec2 origin = { 0, 0 };
		behaviarWindowPosisions.clear();

		//ビヘイビアの生成ボタン用ウィンドウ
		{
			ImVec4* style = ImGui::GetStyle().Colors;
			style[ImGuiCol_TitleBg] = { 0.3f, 0.3f, 0.3f, 1.0f };
			style[ImGuiCol_TitleBgCollapsed] = { 0.3f, 0.3f, 0.3f, 1.0f };
			style[ImGuiCol_TitleBgActive] = { 0.3f, 0.3f, 0.3f, 1.0f };
			ImGui::Begin("BehaviorWindow", nullptr, beharviorButtonWindowFlags);//ウィンドウの名前
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
				if (nameBuf[0] != '\0')CreateNode(nameBuf, NodeType::e_Selector, selectObject);
			}
			if (ImGui::Button("Add Sequence"))
			{
				if (nameBuf[0] != '\0')CreateNode(nameBuf, NodeType::e_Sequence, selectObject);
			}
			if (ImGui::Button("Add Task"))
			{
				if (nameBuf[0] != '\0')CreateNode(nameBuf, NodeType::e_Task, selectObject);
			}

			if (ImGui::Button("Behavior Export"))
			{
				if (treeNameBuf[0] != '\0')ExportFile(treeNameBuf, &rootObject);
			}
			if (ImGui::Button("Behavior Inport"))
			{
				if (treeNameBuf[0] != '\0')InportFile(treeNameBuf, &rootObject);
			}
			ImGui::End();
		}

		//個々のノードを描画
		rootObject.Init(nullptr, "RootNode", e_Root);
		rootObject.DrawGUIChildren(&rootObject);

		//グリッドキャンパス
		//ウィンドウサイズは各自で設定してください
		{
			//ウィンドウサイズを画面全体に
			static const float windowWid = 1280;
			static const float windowHi = 720;
			ImGui::SetNextWindowSize(ImVec2(windowWid, windowHi), ImGuiCond_Appearing);
			//座標を左上に
			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Appearing);
			//ウィンドウの背景と外枠を描画しない
			beharviorWindowFlags |= ImGuiWindowFlags_NoBackground;
			//タイトルバーを付けない
			beharviorWindowFlags |= ImGuiWindowFlags_NoTitleBar;
			//右下のサイズ変更を出来なくする
			beharviorWindowFlags |= ImGuiWindowFlags_NoResize;
			//動かないようにする
			beharviorWindowFlags |= ImGuiWindowFlags_NoMove;
			//最前面に来ないように
			beharviorWindowFlags |= ImGuiWindowFlags_NoFocusOnAppearing;
			//iniを読み込み書き込みをしない
			beharviorWindowFlags |= ImGuiWindowFlags_NoSavedSettings;
			//背景にいかない？
			beharviorWindowFlags |= ImGuiWindowFlags_NoBackground;
			beharviorWindowFlags |= ImGuiWindowFlags_NoCollapse;
			//フォーカスを受けても前に来ない(ノードより前に来ない)
			beharviorWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			ImGui::Begin("BehaviarTree", nullptr, beharviorWindowFlags);//ウィンドウの名前
			ImGui::SetWindowSize(ImVec2(windowWid, windowHi), ImGuiCond_::ImGuiCond_FirstUseEver);

			//マウス入力などを取得
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
			//背景色
			draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
			//?
			draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(0, 0, 2, 255));
			//画面をボタンとして見立ててる？
			//マウスの入力キーの取得がしたいのかも
			ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); // Hovered
			const bool is_active = ImGui::IsItemActive();   // Held
			origin = { canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y }; // Lock scrolled origin
			//キャンパス内でのマウス位置

			//スクロール
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
			//描画前準備
			draw_list->PushClipRect(canvas_p0, canvas_p1, true);

			//グリッドの描画
			{
				//グリッドサイズ
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
			//描画終了処理
			draw_list->PopClipRect();
			ImGui::End();
		}
	}

	void BehavierImGui::ClearNodes()
	{
		rootObject.Children()->clear();
	}

	void BehavierImGui::Init()
	{
		rootObject.Init(nullptr, "RootNode", e_Root);
		selectObject = &rootObject;
	}

	void ExportFile(string f_fileName, Node* f_rootNode)
	{
		string particleFileName = f_fileName;
		//ファイルを作る
		string saveFileName = "Resource/TextData/Behavior/" + particleFileName + ".json";
		ofstream ofs(saveFileName);
		string dataStr = "{\n";	
		GetChildName(f_rootNode, dataStr);
		dataStr += "\n}";
		ofs << dataStr;
	}

	void GetChildName(Node* f_node, string &f_names)
	{
		//ノードの文字列化
		f_names += "\"";
		f_names += f_node->GetData(defaultDatas[je_NodeName]);
		f_names += "\":";
		f_names += f_node->GetData().dump();
		
		//子がいなければ戻る
		if (f_node->Children()->size() < 1) return;

		for each (Node * node in *f_node->Children())
		{
			f_names += ",\n";
			GetChildName(node, f_names);
		}
	}

	void InportFile(string f_fileName, Node *f_rootNode)
	{
		//ファイル読み込み
		string pathName = f_fileName;
		string fullPath = "Resource/TextData/Behavior/" + pathName + ".json";

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
		//jsonデータにアクセス
		for (auto& [key, value] : loadData.items())
		{
			if (key == "RootNode")continue;
			Node *nodeData = new Node();
			nodeData->SetData(loadData[key]);
			NodeType type = nodeData->GetData()[defaultDatas[je_NodeType]];
			string parentName = nodeData->GetData()[defaultDatas[je_ParentName]];
			Node *targetNode =  f_rootNode->SearchChildrenNode(f_rootNode, parentName);
			BehavierImGui::CreateNode(key.c_str(), type, targetNode);
		}
	}
}