#pragma once
#include <string>
#include <list>
#include <memory>
#include <sstream>
#include <fstream>
#include <iostream>
#include <functional>
#include <algorithm>
//インクルードは自分で調整してください
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx12.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"
// Jsonライブラリ
// https://github.com/nlohmann/json
#pragma warning(push)
#pragma warning(disable:4996)
#include "../single_include/nlohmann/json.hpp"
#pragma warning(pop)

namespace BehaviorTree
{
	using namespace std;
	//Jsonのusing
	using json = nlohmann::json;

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

	enum JsonNameNum
	{
		je_NodeType,
		je_NodeName,
		je_ParentName,
		je_WindowPos,
		je_Priority,
		je_WindowSize,
		je_LoadNumber,
		je_Max
	};
	static array<string, je_Max> defaultDatas = { "NodeType", "NodeName", "ParentName", "WindowPosition", "Priority" , "WindowSize", "LoadNumber"};

	//ノードの基本クラス
	class Node
	{
	private:
		static const ImVec2 WindowSize;

		function<bool()> _function;

		NowState nowState = Stay;

		NowState RootNodeRun();
		NowState SelectorRun();
		NowState SequenceRun();
		NowState TaskRun();

	public:
		json datas;
		int _loadNumber = 0;
		bool operator<(const Node& right) const {
			int loadNum1 = datas[defaultDatas[je_LoadNumber]];
			int loadNum2 = right.datas[defaultDatas[je_LoadNumber]];
			bool loadnum = loadNum1 < loadNum2;
			if (loadNum1 == loadNum2)
			{
				int priority1 = datas[defaultDatas[je_Priority]];
				int priority2 = right.datas[defaultDatas[je_Priority]];
				return priority1 < priority2;
			}
			return loadnum;
		}
		//list<string> jsonNames;
		//GUIの描画ルール
		ImGuiWindowFlags _flags = 0;
		//子ノード
		list<Node*> _children;
		//親ノード
		Node* _parent = nullptr;
		
		//現在再生中のノード
		Node *_playbBackNode;
		int _playBackPriorityNum = 0;

		//GUIウィンドウの座標
		//ImVec2 _windowPos = { 0, 0 };
		Node() {}
		~Node() {}
		/*-----------json関連--------------*/
		//json自体の取得
		json GetData()
		{
			return datas;
		}
		//jsonないの指定keyのオブジェクト取得
		auto GetData(string f_jsonName)
		{
			return datas[f_jsonName];
		}
		//json全体を書き換える
		void SetData(json& f_js)
		{
			datas = f_js;
		}
		/*---------------------------------*/
		//親ノードを取得
		Node* GetParent() { return _parent; };
		//ノードの種類を文字列で取得
		const string GetNodeTypeName()
		{
			if (datas[je_NodeType] == e_Root)
			{
				return "Root";
			}
			else if (datas[je_NodeType] == e_Selector)
			{
				return "Selector";
			}
			else if (datas[je_NodeType] == e_Sequence)
			{
				return "Sequence";
			}
			else if (datas[je_NodeType] == e_Task)
			{
				return "Task";
			}
			else if (datas[je_NodeType] == e_NONE)
			{
				return "Error";
			}
		}
		//自身より親のオブジェクトの中から名前一致するノードを取得
		Node* SearchParentNode(string f_nodeName);
		//自身より子のオブジェクトの中から名前一致するノードを取得
		Node* SearchChildrenNode(Node* f_node, string f_nodeName);
		//子オブジェクトのリスト取得
		list<Node*>* Children() { return &_children; }
		//自身を返す
		Node* GetNode() { return this; }
		//自分以下をGUIに描画(ルートノードで行うことを推奨)
		void DrawGUIChildren(Node* f_node);
		//GUIのカラー設定
		void SetDefaultColor(ImVec4 *f_color);
		//ノード同士の線を構成する
		void SetLines(ImGuiWindow* f_window);
		//標準初期化-最低限しか行っていない
		virtual void Init(Node* f_parent, string f_nodeName, NodeType f_type);
		virtual void Init(Node* f_parent, string f_nodeName, NodeType f_type, string f_functionName, int f_functionNumber);
		//ノードの描画-デフォルトではjsonデータを表示するだけ
		virtual void GUIDraw();
		//関数ポインタのセット
		void SetFunction(string f_nodeName, function<bool()> f_function)
		{
			SearchChildrenNode(this, f_nodeName)->_function = f_function;
		}
		//保管しておいた関数を実行
		bool RunFunc()
		{
			return _function();
		}

		NowState StartNode()
		{
			if (datas[je_NodeType] == NodeType::e_Task)
			{

			}
			if (_children.size() == 0)
			{
				
			}
			for each (Node *var in _children)
			{
				if (var)
				if (var->Children()->size() == 0) return NowState::Stay;
				var->StartNode();
				return NowState::Stay;
			}
		}
	};

	/// <summary>
	/// GUIのデータを出力
	/// </summary>
	/// <param name="f_fileName">出力ファイル名</param>
	static void ExportFile(string f_fileName, Node* f_rootNode);
	/// <summary>
	/// </summary>
	/// </summary>
	/// <param name="rootNode"></param>
	/// <param name="f_fileName"></param>
	void InportFile(string f_fileName, Node* f_rootNode);
	//static bool isExportError;
	static bool GetChildName(Node* f_node, string& f_names);
	/// <summary>
	/// 親子を結ぶ
	/// </summary>
	static void SetParentAndChild(Node& f_parent, Node& f_child)
	{
		f_parent.Children()->push_back(&f_child);
		if (f_child._parent == nullptr)
		{
			f_child._parent = new Node();
		}
		f_child._parent = &f_parent;
	};

	//-----------------------以下GUI制御用-----------------------
	class BehavierImGui
	{
	private:
		//GUIのBehaviorのルートオブジェクト
		//ノードを作成すれば自動で作成される
		static Node rootObject;
		//ノードの名前を格納
		static char nameBuf[256];
		static const char* nodeName;
		//ツリー自体の名前を格納
		static char treeNameBuf[256];
		static const char* treeName;
		static bool isBehaviorError;
		static bool isSetFunctionError;
		
		
	public:
		BehavierImGui()
		{

		}
		enum FunctionsTab
		{
			Enemey,
			None,
		};
		static int loadNumber;
		static FunctionsTab funcTab;
		//親ノードにするオブジェクトを保管しておく
		static Node* selectObject;
		//GUIウィンドウの描画ルール
		static ImGuiWindowFlags beharviorWindowFlags;
		//ボタンが配置されているウィンドウの描画ルール
		static ImGuiWindowFlags beharviorButtonWindowFlags;
		//線引く用のデータコンテナ
		//ImVec2-線引くための座標
		//NodeType-タイプによって色を変える
		static ImVector<pair<ImVec2, NodeType>> behaviarWindowPosisions;
		/// <summary>
		/// ノードを作る
		/// </summary>
		/// <param name="f_nodeName">ノードの名前</param>
		/// <param name="f_type">ノードの種類</param>
		/// <param name="f_parent">親ノードになるノード</param>
		static void CreateNode(const char* f_nodeName, NodeType f_type, Node* f_parent);
		static void CreateNode(const char* f_nodeName, NodeType f_type, Node* f_parent, string f_functionName, int f_functionNumber);
		//全体の描画←これでグリットからなんやらを描画してもらう
		static void DrawImGui();
		//GUIのノードを一掃してもらう
		static void ClearNodes();
		//GUIウィンドウが [非表示→表示] の時に毎回呼び出すと動作安定
		static void Init();
		//Taskノードに関数をつける
		static void SetFunction(string f_name, int f_number);
	};
	//-----------------------------------------------------------

	class FuncElement
	{
	private:
		
	public:
		FuncElement(function<bool()> f_func, string f_name, int f_num)
		{
			_function = f_func;
			_functionName = f_name;
			_number = f_num;
		}
		string _functionName;
		int _number;
		function<bool()> _function;
	};
	
	class Enemy
	{
	public:
		static list<FuncElement *> functions;

		static bool HogeTrigger()
		{
			return false;
		}
		//static bool isAttack;
		//static bool isMove;
		//static bool isStay;
		/*static bool Attack()
		{ 
			if (isMove || isStay) return false; 
			isAttack = true;
			return isAttack;
		};
		static bool Move()
		{ 
			if (isStay || isAttack) return false;
			isMove = true;
			return isMove;
		};
		static bool Stay()
		{ 
			if (isAttack || isMove) return false;
			isStay = true;
			return isStay;
		};*/

		static void Init(string f_jsonFilePath)
		{
			
			FuncElement *func1 = new FuncElement(HogeTrigger, "Attack", 0);
			FuncElement *func2 = new FuncElement(HogeTrigger, "Stay", 1);
			FuncElement *func3 = new FuncElement(HogeTrigger, "Move", 2);
			functions.push_back(func1);
			functions.push_back(func2);
			functions.push_back(func3);
		}

		static void Update()
		{
			
		}

		static void GUIShow()
		{
			static int hoge = 0;
			string combo = "";
			for (auto itr = functions.begin(); itr != functions.end(); ++itr)
			{
				combo += (*itr)->_functionName;
				combo += '\0';
			}
			ImGui::Combo("functions", &hoge, combo.c_str());
			if (ImGui::Button("Add Function"))
			{
				auto itr = functions.begin();
				for (int i = 0; i < hoge; i++)
				{
					itr++;
				}
				BehavierImGui::SetFunction((*itr)->_functionName, hoge);
			}
		}
	};
}
