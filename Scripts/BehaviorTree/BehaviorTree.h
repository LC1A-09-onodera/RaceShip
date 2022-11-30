#pragma once
#include <string>
#include <list>
#include <memory>
#include <sstream>
#include <fstream>
#include <iostream>
#include <functional>
#include <algorithm>
//�C���N���[�h�͎����Œ������Ă�������
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx12.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"
// Json���C�u����
// https://github.com/nlohmann/json
#pragma warning(push)
#pragma warning(disable:4996)
#include "../single_include/nlohmann/json.hpp"
#pragma warning(pop)

namespace BehaviorTree
{
	using namespace std;
	//Json��using
	using json = nlohmann::json;

	//�m�[�h�̎��
	enum NodeType
	{
		e_Root,
		e_Selector,
		e_Sequence,
		e_Task,
		e_NONE,
	};
	//�m�[�h�̏�ԑJ��
	enum NowState
	{
		//�Ώۂ���Ȃ��A���s���Ă��Ȃ�
		Stay,
		//���s��
		Run,
		//����
		Success,
		//���s
		Failure,
		//�G���[
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

	//�m�[�h�̊�{�N���X
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
		//GUI�̕`�惋�[��
		ImGuiWindowFlags _flags = 0;
		//�q�m�[�h
		list<Node*> _children;
		//�e�m�[�h
		Node* _parent = nullptr;
		
		//���ݍĐ����̃m�[�h
		Node *_playbBackNode;
		int _playBackPriorityNum = 0;

		//GUI�E�B���h�E�̍��W
		//ImVec2 _windowPos = { 0, 0 };
		Node() {}
		~Node() {}
		/*-----------json�֘A--------------*/
		//json���̂̎擾
		json GetData()
		{
			return datas;
		}
		//json�Ȃ��̎w��key�̃I�u�W�F�N�g�擾
		auto GetData(string f_jsonName)
		{
			return datas[f_jsonName];
		}
		//json�S�̂�����������
		void SetData(json& f_js)
		{
			datas = f_js;
		}
		/*---------------------------------*/
		//�e�m�[�h���擾
		Node* GetParent() { return _parent; };
		//�m�[�h�̎�ނ𕶎���Ŏ擾
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
		//���g���e�̃I�u�W�F�N�g�̒����疼�O��v����m�[�h���擾
		Node* SearchParentNode(string f_nodeName);
		//���g���q�̃I�u�W�F�N�g�̒����疼�O��v����m�[�h���擾
		Node* SearchChildrenNode(Node* f_node, string f_nodeName);
		//�q�I�u�W�F�N�g�̃��X�g�擾
		list<Node*>* Children() { return &_children; }
		//���g��Ԃ�
		Node* GetNode() { return this; }
		//�����ȉ���GUI�ɕ`��(���[�g�m�[�h�ōs�����Ƃ𐄏�)
		void DrawGUIChildren(Node* f_node);
		//GUI�̃J���[�ݒ�
		void SetDefaultColor(ImVec4 *f_color);
		//�m�[�h���m�̐����\������
		void SetLines(ImGuiWindow* f_window);
		//�W��������-�Œ�������s���Ă��Ȃ�
		virtual void Init(Node* f_parent, string f_nodeName, NodeType f_type);
		virtual void Init(Node* f_parent, string f_nodeName, NodeType f_type, string f_functionName, int f_functionNumber);
		//�m�[�h�̕`��-�f�t�H���g�ł�json�f�[�^��\�����邾��
		virtual void GUIDraw();
		//�֐��|�C���^�̃Z�b�g
		void SetFunction(string f_nodeName, function<bool()> f_function)
		{
			SearchChildrenNode(this, f_nodeName)->_function = f_function;
		}
		//�ۊǂ��Ă������֐������s
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
	/// GUI�̃f�[�^���o��
	/// </summary>
	/// <param name="f_fileName">�o�̓t�@�C����</param>
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
	/// �e�q������
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

	//-----------------------�ȉ�GUI����p-----------------------
	class BehavierImGui
	{
	private:
		//GUI��Behavior�̃��[�g�I�u�W�F�N�g
		//�m�[�h���쐬����Ύ����ō쐬�����
		static Node rootObject;
		//�m�[�h�̖��O���i�[
		static char nameBuf[256];
		static const char* nodeName;
		//�c���[���̖̂��O���i�[
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
		//�e�m�[�h�ɂ���I�u�W�F�N�g��ۊǂ��Ă���
		static Node* selectObject;
		//GUI�E�B���h�E�̕`�惋�[��
		static ImGuiWindowFlags beharviorWindowFlags;
		//�{�^�����z�u����Ă���E�B���h�E�̕`�惋�[��
		static ImGuiWindowFlags beharviorButtonWindowFlags;
		//�������p�̃f�[�^�R���e�i
		//ImVec2-���������߂̍��W
		//NodeType-�^�C�v�ɂ���ĐF��ς���
		static ImVector<pair<ImVec2, NodeType>> behaviarWindowPosisions;
		/// <summary>
		/// �m�[�h�����
		/// </summary>
		/// <param name="f_nodeName">�m�[�h�̖��O</param>
		/// <param name="f_type">�m�[�h�̎��</param>
		/// <param name="f_parent">�e�m�[�h�ɂȂ�m�[�h</param>
		static void CreateNode(const char* f_nodeName, NodeType f_type, Node* f_parent);
		static void CreateNode(const char* f_nodeName, NodeType f_type, Node* f_parent, string f_functionName, int f_functionNumber);
		//�S�̂̕`�恩����ŃO���b�g����Ȃ����`�悵�Ă��炤
		static void DrawImGui();
		//GUI�̃m�[�h����|���Ă��炤
		static void ClearNodes();
		//GUI�E�B���h�E�� [��\�����\��] �̎��ɖ���Ăяo���Ɠ������
		static void Init();
		//Task�m�[�h�Ɋ֐�������
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
