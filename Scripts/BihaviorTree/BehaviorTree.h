#pragma once
#include <string>
#include <list>
#include <memory>
//インクルードは自分で調整してください
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx12.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"

namespace BehaviorTree
{
	//ノードの種類
	enum NodeType
	{
		e_Root,
		e_Selector,
		e_Sequence,
		e_Task,
		e_NONE,
	};
	//ノードの状態遷移
	enum NowState
	{
		//対象じゃない、実行していない
		Stay,
		//実行中
		Run,
		//成功
		Success,
		//失敗
		Failure,
		//エラー
		Error,
	};
	//ノードの基本クラス
	class Node
	{
	private:
		std::list<Node*> _children;

		//優先度(使用割合)
		int _priority = 0;
		//ノードの名前
		std::string _name = "";
		//ノードの実行中フラグ
		bool _isActive = false;
		//ステータス
		NowState _nowState = NowState::Stay;
		//Nodeの種類を格納
		NodeType _nodeType = NodeType::e_NONE;
		//GUIの描画ルール
		ImGuiWindowFlags _flags = 0;
		//GUIウィンドウの座標
		ImVec2 _windowPos = { 0, 0 };
	public:
		//親ノード
		Node* _parent = nullptr;
		//ここにすべてのノードを格納
		//描画を行う
		class NodeManager
		{
		public:
			std::list<Node *> nodes;
			std::string dataStr;
			void Draw()
			{
				for each (auto node in nodes)
				{
					node->GUIDraw();
				}
			};
			void LoadFile(std::string f_fileName);
			void ExportFile(std::string f_fileName);
			void GetChildren(Node *f_node);
		};
		//コンストラクタ・デストラクタ
		Node(){}
		~Node(){}
		void GUIDraw();
		ImVec2 GetWindowPos(){return _windowPos;}
		Node* GetParent(){return _parent;};
		const int GetPriority(){return _priority;};
		const std::string GetName(){return _name;}
		void SetName(std::string f_name){_name = f_name;}
		const bool GetActive(){return _isActive;}
		const NowState GetIsReturnParent(){return _nowState;}
		void SetNodeType(NodeType f_type){_nodeType = f_type;}
		const NodeType GetNodeType(){return _nodeType;}
		const std::string GetNodeTypeName() 
		{
			switch (_nodeType)
			{
			case BehaviorTree::e_Root:
				return "Root";
				break;
			case BehaviorTree::e_Selector:
				return "Selector";
				break;
			case BehaviorTree::e_Sequence:
				return "Sequence";
				break;
			case BehaviorTree::e_Task:
				return "Task";
				break;
			case BehaviorTree::e_NONE:
				return "Error";
				break;
			default:
				return "Error";
				break;
			}
		}
		void SetActive(bool f_active){_isActive = f_active;}
		std::list<Node*> *Children(){return &_children;}
		Node* GetNode(){return this;}
	};
	//親子を結ぶ
	static void SetParentAndChild(Node& f_parent, Node& f_child)
	{
		f_parent.Children()->push_back(&f_child);
		if (f_child._parent == nullptr)
		{
			f_child._parent = new Node();
		}
		f_child._parent = &f_parent;
	};

	//----------------------以下各種ノード-----------------------
	//ルートオブジェクト
	class RootBehavior : public Node
	{
	public:
		RootBehavior()
		{
			SetNodeType(NodeType::e_Root);
		}
		~RootBehavior()
		{
		}
	};
	//セレクター
	class Selector : public Node
	{
	public:
		Selector()
		{
			SetNodeType(NodeType::e_Selector);
		}
		~Selector()
		{

		}
	};
	//シーケンス
	class Sequence : public Node
	{
	public:
		Sequence()
		{
			SetNodeType(NodeType::e_Sequence);
		}
		~Sequence()
		{

		}
		
	};
	//タスク
	class Task : public Node
	{
	public:
		Task()
		{
			SetNodeType(NodeType::e_Task);
		}
		~Task()
		{

		}
		//実際に行う関数の純粋仮想関数
		//継承先で関数のアクションを決める
		//virtual bool Task() = 0;
	};
	//-----------------------------------------------------------


	//-----------------------以下GUI制御用-----------------------
	class BehavierImGui
	{
	private:
		//Behaviorのルートオブジェクト
		static RootBehavior rootObject;
		
		//ノードの名前を格納
		static char nameBuf[256];
		static const char *nodeName;
	public:
		static Node *selectObject;
		//これで描画するためにまとめている
		static Node::NodeManager nodeManager;
		static ImGuiWindowFlags beharviorWindowFlags;
		static ImVector<ImVec2> behaviarWindowPosisions;
		static void CreateNode(const char* f_nodeName, NodeType f_type, Node*f_parent);
		static void DrawImGui();
		static void ClearNodes();
	};
}
