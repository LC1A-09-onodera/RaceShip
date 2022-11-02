#pragma once
#include <string>
#include <list>
#include <memory>
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
		je_Max
	};
	static array<string, je_Max> defaultDatas = { "NodeType", "NodeName", "ParentName", "WindowPosition", "Priority" , "WindowSize"};

	//ノードの基本クラス
	class Node
	{
	private:
		static const ImVec2 WindowSize;
		json datas;

		function<bool()> _function;

		NowState nowState = Stay;

		NowState RootNodeRun();
		NowState SelectorRun();
		NowState SequenceRun();
		NowState TaskRun();

	public:
		//list<string> jsonNames;
		//GUIの描画ルール
		ImGuiWindowFlags _flags = 0;
		//子ノード
		list<Node*> _children;
		//親ノード
		Node* _parent = nullptr;
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
		void SetDedaltColor(ImVec4 *f_color);
		//ノード同士の線を構成する
		void SetLines(ImGuiWindow* f_window);
		//標準初期化-最低限しか行っていない
		virtual void Init(Node* f_parent, string f_nodeName, NodeType f_type);
		//ノードの描画-デフォルトではjsonデータを表示するだけ
		virtual void GUIDraw();
		//関数ポインタのセット
		void SetFunction(function<bool()> f_function)
		{
			_function = f_function;
		}
		//保管しておいた関数を実行
		bool RunFunc()
		{
			return _function();
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
	static void GetChildName(Node* f_node, string& f_names);
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
	public:
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
		//全体の描画←これでグリットからなんやらを描画してもらう
		static void DrawImGui();
		//GUIのノードを一掃してもらう
		static void ClearNodes();
		//GUIウィンドウが [非表示→表示] の時に毎回呼び出すと動作安定
		static void Init();
	};
	//-----------------------------------------------------------
	
	//Demo
	class Hoge
	{
	public:
		bool GetTrue()
		{
			return true;
		}
		bool GetFalse()
		{
			return false;
		}
	};
	class ExBehavior
	{
	public:
		static Hoge hoge;
		static Node rootNode;
		static void Init(Hoge &hoge)
		{
			rootNode.SetFunction(function<bool()>([&]() {return hoge.GetFalse(); }));
		}
		static bool IsFunc()
		{
			return (bool)rootNode.RunFunc();
		}
	};

	//Demo2
	class Element
	{
		int num;
		string actionName;
		//bool isAction
		virtual bool Action() = 0;
	};
	class ExAI
	{
	public:
		enum Actions
		{
			Run,
			Stay,
		};
		array<string, 10> actions = {"Run", "Stay"};
		
	};
}
