#include "BehaviorTree.h"

namespace BehaviorTree
{
	Node BehavierImGui::rootObject = Node();

	char BehavierImGui::nameBuf[256] = {};
	const char* BehavierImGui::nodeName = " ";
	char BehavierImGui::treeNameBuf[256] = {};
	const char* BehavierImGui::treeName = " ";

	ImGuiWindowFlags BehavierImGui::beharviorWindowFlags = 0;
	ImGuiWindowFlags BehavierImGui::beharviorButtonWindowFlags = 0;
	bool BehavierImGui::isSetFunctionError = false;
	ImVector<pair<ImVec2, NodeType>> BehavierImGui::behaviarWindowPosisions;
	Node* BehavierImGui::selectObject;

	const ImVec2 Node::WindowSize = { 220.0f, 140.0f };

	bool BehavierImGui::isBehaviorError = false;

	BehavierImGui::FunctionsTab BehavierImGui::funcTab = BehavierImGui::FunctionsTab::Enemey;
	list<FuncElement*> Enemy::functions = list<FuncElement*>();

	int BehavierImGui::loadNumber = 0;
	bool isExportError = false;

	void Node::Init(Node* f_parent, string f_nodeName, NodeType f_type)
	{
		//ウィンドウ生成時親ノードとどれだけずらすかを設定
		static const ImVec2 WindowSponeDiff = { 5.0f, 5.0f };
		datas[defaultDatas[je_NodeName]] = f_nodeName;
		datas[defaultDatas[je_NodeType]] = f_type;

		datas[defaultDatas[je_Priority]] = 0;
		datas[defaultDatas[je_WindowPos]] = { 100.0f, 30.0f };
		datas[defaultDatas[je_WindowSize]] = { WindowSize.x, WindowSize.y };
		datas[defaultDatas[je_LoadNumber]] = BehavierImGui::loadNumber;
		BehavierImGui::loadNumber++;
		//ルートオブジェクトの場合親ノードがない場合戻る
		if (f_parent == nullptr)return;
		datas[defaultDatas[je_Priority]] = static_cast<int>(f_parent->Children()->size());
		datas[defaultDatas[je_WindowPos]] = { f_parent->datas[defaultDatas[je_WindowPos]][0] + WindowSize.x + WindowSponeDiff.x, f_parent->datas[defaultDatas[je_WindowPos]][1] + (WindowSize.y / 2.0f) + (f_parent->Children()->size() * WindowSize.y) };
		datas[defaultDatas[je_ParentName]] = f_parent->datas[defaultDatas[je_NodeName]];
		SetParentAndChild(*f_parent, *this);
	}
	void Node::Init(Node* f_parent, string f_nodeName, NodeType f_type, string f_functionName, int f_functionNumber)
	{
		//ウィンドウ生成時親ノードとどれだけずらすかを設定
		static const ImVec2 WindowSponeDiff = { 5.0f, 5.0f };
		datas[defaultDatas[je_NodeName]] = f_nodeName;
		datas[defaultDatas[je_NodeType]] = f_type;

		datas[defaultDatas[je_Priority]] = 0;
		datas[defaultDatas[je_WindowPos]] = { 100.0f, 30.0f };
		datas[defaultDatas[je_WindowSize]] = { WindowSize.x, WindowSize.y };
		//ルートオブジェクトの場合親ノードがない場合戻る
		if (f_parent == nullptr)return;
		datas[defaultDatas[je_Priority]] = static_cast<int>(f_parent->Children()->size());
		datas[defaultDatas[je_WindowPos]] = { f_parent->datas[defaultDatas[je_WindowPos]][0] + WindowSize.x + WindowSponeDiff.x, f_parent->datas[defaultDatas[je_WindowPos]][1] + (WindowSize.y / 2.0f) + (f_parent->Children()->size() * WindowSize.y) };
		datas[defaultDatas[je_ParentName]] = f_parent->datas[defaultDatas[je_NodeName]];

		datas["FunctionName"] = f_functionName;
		datas["FunctionNumber"] = f_functionNumber;
		SetParentAndChild(*f_parent, *this);
	}

	NowState Node::RootNodeRun()
	{
		for (auto childItr = _children.begin(); childItr != _children.end(); ++childItr)
		{
			//childItr
		}
		return Error;
	}
	NowState Node::SelectorRun()
	{
		return Error;
	}
	NowState Node::SequenceRun()
	{
		return Error;
	}
	NowState Node::TaskRun()
	{
		if (nowState == NowState::Error)
		{
			assert(0);
		}
		if (_function())
		{
			return NowState::Success;
		}
		else
		{
			return NowState::Failure;
		}
		
	}
	void Node::GUIDraw()
	{
		//iniを読み込み書き込みをしない
		_flags |= ImGuiWindowFlags_NoSavedSettings;
		//ノードのカラー変更
		ImVec4* style = ImGui::GetStyle().Colors;
		SetDefaultColor(style);

		//座標の設定
		ImGui::SetNextWindowPos({ datas[defaultDatas[je_WindowPos]][0],datas[defaultDatas[je_WindowPos]][1] }, ImGuiCond_Appearing);
		string titlebarName = datas[defaultDatas[je_NodeName]];
		ImGui::Begin(titlebarName.c_str(), nullptr, _flags);//ウィンドウの名前
		ImGui::SetWindowSize(WindowSize, ImGuiCond_::ImGuiCond_FirstUseEver);
		if (static_cast<string>(datas[defaultDatas[je_NodeName]]) == (GImGui)->Windows[GImGui->Windows.size() - 1]->Name)
		{
			BehavierImGui::selectObject = this;
		}


		//優先度の変更
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
		datas[defaultDatas[je_WindowPos]] = { window->Pos.x, window->Pos.y };
		datas[defaultDatas[je_WindowSize]] = { window->Size.x, window->Size.y };

		if (GetParent() != nullptr)
		{
			//ライン描画用セット
			SetLines(window);
		}

		ImGui::End();
	}
	Node* Node::SearchParentNode(string f_nodeName)
	{

		return nullptr;
	}
	void Node::SetDefaultColor(ImVec4* f_color)
	{
		if (f_color == nullptr) return;
		//NONEノードは基本なしなので描画しない
		if (datas[defaultDatas[je_NodeType]] == NodeType::e_NONE)return;
		if (datas[defaultDatas[je_NodeType]] == NodeType::e_Root)
		{
			f_color[ImGuiCol_TitleBg] = { 0.3f, 0.3f, 0.01f, 1.0f };
			f_color[ImGuiCol_TitleBgCollapsed] = { 0.35f, 0.35f, 0.01f, 1.0f };
			f_color[ImGuiCol_TitleBgActive] = { 0.5f, 0.5f, 0.01f, 1.0f };
		}
		else if (datas[defaultDatas[je_NodeType]] == NodeType::e_Selector)
		{
			f_color[ImGuiCol_TitleBg] = { 0.01f, 0.3f, 0.3f, 1.0f };
			f_color[ImGuiCol_TitleBgCollapsed] = { 0.01f, 0.35f, 0.35f, 1.0f };
			f_color[ImGuiCol_TitleBgActive] = { 0.01f, 0.5f, 0.5f, 1.0f };
		}
		else if (datas[defaultDatas[je_NodeType]] == NodeType::e_Sequence)
		{
			f_color[ImGuiCol_TitleBg] = { 0.21f, 0.21f, 0.6f, 1.0f };
			f_color[ImGuiCol_TitleBgCollapsed] = { 0.25f, 0.25f, 0.65f, 1.0f };
			f_color[ImGuiCol_TitleBgActive] = { 0.4f, 0.4f, 0.75f, 1.0f };
		}
		else if (datas[defaultDatas[je_NodeType]] == NodeType::e_Task)
		{
			f_color[ImGuiCol_TitleBg] = { 0.3f, 0.01f, 0.3f, 1.0f };
			f_color[ImGuiCol_TitleBgCollapsed] = { 0.35f, 0.01f, 0.35f, 1.0f };
			f_color[ImGuiCol_TitleBgActive] = { 0.5f, 0.01f, 0.5f, 1.0f };
		}
	}
	void Node::SetLines(ImGuiWindow* f_window)
	{
		if (f_window == nullptr) return;
		if (_parent == nullptr) return;
		if (_parent->GetData(defaultDatas[je_WindowPos])[0] + WindowSize.x / 2.0f > f_window->Pos.x)
		{
			f_window->Pos.x = _parent->GetData(defaultDatas[je_WindowPos])[0] + WindowSize.x / 2.0f;
		}
		if (_parent->GetData(defaultDatas[je_WindowPos])[1] + WindowSize.y / 2.0f > f_window->Pos.y)
		{
			f_window->Pos.y = _parent->GetData(defaultDatas[je_WindowPos])[1] + WindowSize.y / 2.0f;
		}

		float parentX;
		parentX = _parent->GetData(defaultDatas[je_WindowPos])[0];
		parentX += _parent->GetData(defaultDatas[je_WindowSize])[0] / 2.0f;
		float parentY;
		parentY = _parent->GetData(defaultDatas[je_WindowPos])[1];
		parentY += _parent->GetData(defaultDatas[je_WindowSize])[1] / 2.0f;
		//親子を線で結ぶための座標保管
		BehavierImGui::behaviarWindowPosisions.push_back({ {parentX, parentY}, _parent->GetData(defaultDatas[je_NodeType]) });
		BehavierImGui::behaviarWindowPosisions.push_back({ f_window->Pos, datas[defaultDatas[je_NodeType]] });
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
	void BehavierImGui::CreateNode(const char* f_nodeName, NodeType f_type, Node* f_parent, string f_functionName, int f_functionNumber)
	{
		//最初のノードつくりの時の処理
		{
			if (selectObject == nullptr)
			{
				rootObject.Init(nullptr, "RootNode", e_Root);
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
				node->Init(f_parent, f_nodeName, e_Task, f_functionName, f_functionNumber);
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
				else isBehaviorError = true;
			}
			if (ImGui::Button("Add Sequence"))
			{
				if (nameBuf[0] != '\0')CreateNode(nameBuf, NodeType::e_Sequence, selectObject);
				else isBehaviorError = true;
			}
			if (ImGui::Button("Add Task"))
			{
				if (nameBuf[0] != '\0')CreateNode(nameBuf, NodeType::e_Task, selectObject);
				else isBehaviorError = true;
			}

			if (ImGui::Button("Behavior Export"))
			{
				if (treeNameBuf[0] != '\0')ExportFile(treeNameBuf, &rootObject);
				else isBehaviorError = true;
			}
			if (ImGui::Button("Behavior Inport"))
			{
				if (treeNameBuf[0] != '\0')InportFile(treeNameBuf, &rootObject);
				else isBehaviorError = true;
			}
			ImGui::End();
		}
		
		{
			//関数セット用ウィンドウ
			ImVec4* style = ImGui::GetStyle().Colors;
			style[ImGuiCol_TitleBg] = { 0.3f, 0.3f, 0.3f, 1.0f };
			style[ImGuiCol_TitleBgCollapsed] = { 0.3f, 0.3f, 0.3f, 1.0f };
			style[ImGuiCol_TitleBgActive] = { 0.3f, 0.3f, 0.3f, 1.0f };

			ImGuiWindowFlags flag = 0;
			flag |= ImGuiWindowFlags_MenuBar;

			ImGui::Begin("Functions", nullptr, flag);//ウィンドウの名前
			ImGui::SetWindowSize(ImVec2(static_cast<float>(200), static_cast<float>(200)), ImGuiCond_::ImGuiCond_FirstUseEver);
			
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::MenuItem("Ememy"))
				{
					funcTab = FunctionsTab::Enemey;
				}
				ImGui::EndMenuBar();
			}
			
			//エネミーの情報表示
			if (funcTab == FunctionsTab::Enemey)
			{
				Enemy::GUIShow();
			}
			ImGui::End();
		}

		//個々のノードを描画
		
		rootObject.DrawGUIChildren(&rootObject);

		

		//エラーの表示
		if (isBehaviorError)
		{
			static const float sizeX = 300;
			static const float sizeY = 50;
			ImVec4* style = ImGui::GetStyle().Colors;
			style[ImGuiCol_TitleBg] = { 0.8f, 0.3f, 0.3f, 1.0f };
			style[ImGuiCol_TitleBgCollapsed] = { 0.8f, 0.3f, 0.3f, 1.0f };
			style[ImGuiCol_TitleBgActive] = { 0.8f, 0.3f, 0.3f, 1.0f };
			ImGui::SetNextWindowSize(ImVec2(sizeX, sizeY), ImGuiCond_Appearing);
			//座標を左上に
			ImGui::SetNextWindowPos(ImVec2((1280.0f / 2.0f) - (sizeX / 2), (720.0f / 2.0f) - (sizeY / 2)), ImGuiCond_Appearing);
			ImGui::Begin("BehaviorError", &isBehaviorError, beharviorButtonWindowFlags);//ウィンドウの名前

			ImGui::Text("Enter the name of the node or node tree");
			ImGui::End();
		}
		if (isSetFunctionError)
		{
			static const float sizeX = 300;
			static const float sizeY = 50;
			ImVec4* style = ImGui::GetStyle().Colors;
			style[ImGuiCol_TitleBg] = { 0.8f, 0.3f, 0.3f, 1.0f };
			style[ImGuiCol_TitleBgCollapsed] = { 0.8f, 0.3f, 0.3f, 1.0f };
			style[ImGuiCol_TitleBgActive] = { 0.8f, 0.3f, 0.3f, 1.0f };
			ImGui::SetNextWindowSize(ImVec2(sizeX, sizeY), ImGuiCond_Appearing);
			//座標を左上に
			ImGui::SetNextWindowPos(ImVec2((1280.0f / 2.0f) - (sizeX / 2), (720.0f / 2.0f) - (sizeY / 2)), ImGuiCond_Appearing);
			ImGui::Begin("FunctionError", &isSetFunctionError, beharviorButtonWindowFlags);//ウィンドウの名前

			ImGui::Text("The specified node is not a task node");
			ImGui::End();
		}
		if (isExportError)
		{
			static const float sizeX = 400;
			static const float sizeY = 50;
			ImVec4* style = ImGui::GetStyle().Colors;
			style[ImGuiCol_TitleBg] = { 0.8f, 0.3f, 0.3f, 1.0f };
			style[ImGuiCol_TitleBgCollapsed] = { 0.8f, 0.3f, 0.3f, 1.0f };
			style[ImGuiCol_TitleBgActive] = { 0.8f, 0.3f, 0.3f, 1.0f };
			ImGui::SetNextWindowSize(ImVec2(sizeX, sizeY), ImGuiCond_Appearing);
			//座標を左上に
			ImGui::SetNextWindowPos(ImVec2((1280.0f / 2.0f) - (sizeX / 2), (720.0f / 2.0f) - (sizeY / 2)), ImGuiCond_Appearing);
			ImGui::Begin("ExportError", &isExportError, beharviorButtonWindowFlags);//ウィンドウの名前

			ImGui::Text("Some functions are not registered in the task node.");
			ImGui::End();
		}
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
					ImVec4 col;
					static float hi = 200.0f;
					static float low = 70.0f;
					if (behaviarWindowPosisions[n + 1].second == NodeType::e_Selector)
					{
						col = { low, hi, hi, 255.0f };
					}
					else if (behaviarWindowPosisions[n + 1].second == NodeType::e_Sequence)
					{
						col = { low, low, hi, 255.0f };
					}
					else if (behaviarWindowPosisions[n + 1].second == NodeType::e_Task)
					{
						col = { hi, low, hi ,255.0f };
					}
					draw_list->AddLine(behaviarWindowPosisions[n].first, behaviarWindowPosisions[n + 1].first, IM_COL32(col.x, col.y, col.z, col.w), 2.0f);
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
		loadNumber = 0;
	}
	void BehavierImGui::Init()
	{
		rootObject.Init(nullptr, "RootNode", e_Root);
		selectObject = &rootObject;
	}
	void BehavierImGui::SetFunction(string f_name, int f_number)
	{
		if (selectObject == nullptr) return;
		if (selectObject->GetData(defaultDatas[je_NodeType]) != NodeType::e_Task)
		{
			isSetFunctionError = true;
			return;
		}
		selectObject->datas["FunctionName"] = f_name;
		selectObject->datas["FunctionNumber"] = f_number;
	}
	
	bool StartNode(Node& f_rootNode)
	{
		//f_rootNode;
		return false;
	}

	void ExportFile(string f_fileName, Node* f_rootNode)
	{
		string particleFileName = f_fileName;
		//ファイルを作る
		string saveFileName = "Resource/TextData/Behavior/" + particleFileName + ".json";
		ofstream ofs(saveFileName);
		string dataStr = "{\n";
		isExportError = GetChildName(f_rootNode, dataStr);
		dataStr += "\n}";
		if (!isExportError)
		{
			ofs << dataStr;
		}
	}
	bool GetChildName(Node* f_node, string& f_names)
	{
		if (f_node->GetData()[defaultDatas[je_NodeType]] == e_Task && f_node->GetData()["FunctionName"] == nullptr)
		{
			return true;
		}
		//ノードの文字列化
		f_names += "\"";
		f_names += f_node->GetData(defaultDatas[je_NodeName]);
		f_names += "\":";
		f_names += f_node->GetData().dump();

		//子がいなければ戻る
		if (f_node->Children()->size() < 1) return false;

		for each (Node * node in *f_node->Children())
		{
			f_names += ",\n";
			isExportError = GetChildName(node, f_names);
			if (isExportError) return true;
		}
		return false;
	}
	void InportFile(string f_fileName, Node* f_rootNode)
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
		vector<Node> datas;
		//jsonデータにアクセス
		for (auto& [key, value] : loadData.items())
		{
			if (key == "RootNode")continue;
			Node *nodeData = new Node();
			nodeData->SetData(loadData[key]);
			datas.push_back(*nodeData);
		}
		sort(datas.begin(), datas.end());

		for each (Node var in datas)
		{
			NodeType type = var.GetData()[defaultDatas[je_NodeType]];
			string parentName = var.GetData()[defaultDatas[je_ParentName]];
			Node* targetNode = f_rootNode->SearchChildrenNode(f_rootNode, parentName);
			if (var.GetData()[defaultDatas[je_NodeType]] != e_Task)
			{
				string name = var.datas[defaultDatas[je_NodeName]];
				BehavierImGui::CreateNode(name.c_str(), type, targetNode);
			}
			else
			{
				string name = var.datas[defaultDatas[je_NodeName]];
				BehavierImGui::CreateNode(name.c_str(), type, targetNode, var.datas["FunctionName"], var.datas["FunctionNumber"]);
			}
		}
	}
}