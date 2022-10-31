#pragma once
#include <string>
#include <list>
#include <memory>
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
	};
	static array<string, 10> defaultDatas = { "NodeType", "NodeName", "ParentName", "WindowPosition", "Priority"};
	
	//�m�[�h�̊�{�N���X
	class Node
	{
	private:
		static const ImVec2 WindowSize;
		json datas;
	public:
		
		list<string> jsonNames;
		//GUI�̕`�惋�[��
		ImGuiWindowFlags _flags = 0;
		//�q�m�[�h
		list<Node*> _children;
		
		//�e�m�[�h
		Node* _parent = nullptr;
		//GUI�E�B���h�E�̍��W
		ImVec2 _windowPos = { 0, 0 };
		Node(){}
		~Node(){}

		auto GetData(string f_jsonName)
		{
			return datas[f_jsonName];
		}
		//�e�m�[�h���擾
		Node* GetParent(){return _parent;};
		//�D��x���擾
		//const int GetPriority(){return _priority;};
		//void SetPriority(int f_priority) { _priority = f_priority; };
		//�m�[�h�̖��O�擾
		//string GetName(){return _name;}
		//�m�[�h�̖��O�����߂�
		//void SetName(string f_name){_name = f_name;}
		//
		//const NowState GetIsReturnParent(){return _nowState;}
		//�m�[�h�̎�ނ����߂�
		//void SetNodeType(NodeType f_type){_nodeType = f_type;}
		//�m�[�h�̎�ނ��擾
		//const NodeType GetNodeType(){return _nodeType;}
		//�m�[�h�̎�ނ𕶎���Ŏ擾
		const string GetNodeTypeName() 
		{
			if(datas[je_NodeType] == e_Root)
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
		Node *SearchParentNode(string f_nodeName);
		//���g���q�̃I�u�W�F�N�g�̒����疼�O��v����m�[�h���擾
		Node* SearchChildrenNode(Node *f_node, string f_nodeName);
		//�q�I�u�W�F�N�g�̃��X�g�擾
		list<Node*> *Children(){return &_children;}
		//���g��Ԃ�
		Node* GetNode(){return this;}
		
		void DrawGUIChildren(Node* f_node);

		/*------------�p����������������-------------*/
		//�W��������
		virtual void Init(Node *f_parent, string f_nodeName, NodeType f_type);
		//�f�[�^��Json��
		virtual void ToJson(json& j);
		//Json������f�[�^��
		virtual void FromJson(json& j);
		//json�ϐ�datas������������
		virtual void JsonUpdate();
		//�m�[�h�̕`��
		virtual void GUIDraw();
		/*-------------------------------------------*/
	};

	class RootNode : public Node
	{
	public:
		RootNode(){}
		/// <summary>
		/// GUI�`��p
		/// </summary>
		/// <param name="rootNode"></param>
		/// <param name="f_fileName"></param>
		virtual void InportFile(string f_fileName);
		
		/// <summary>
		/// �o�͗p������쐬json�ɔC���悤����
		/// </summary>
		/// <param name="dataStr">�o�͗p������ϐ�</param>
		/// <param name="f_node">�ŏ��ɌĂяo���Ƃ��͐e�m�[�h�A�����I�Ɏq�m�[�h��n�����Ƃ�����</param>
		/// <param name="indextCount">����Ȃ�(���K�w�ڂȂ̂��͂킩�邩��)</param>
		//virtual void GetChildrenDataString(string& dataStr, Node* f_node, int indextCount);
		//�����ȉ���GUI�ɕ`��(���[�g�m�[�h�ōs�����Ƃ𐄏�)
		virtual void DrawGUIChildren(Node* f_node);
	};

	class GUIRootExm : public RootNode
	{

	};

	/// <summary>
	/// GUI�̃f�[�^���o��
	/// </summary>
	/// <param name="f_fileName">�o�̓t�@�C����</param>
	static void ExportFile(string f_fileName);
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
		static RootNode rootObject;
		//�m�[�h�̖��O���i�[
		static char nameBuf[256];
		static const char *nodeName;
		//�m�[�h�̖��O���i�[
		static char treeNameBuf[256];
		static const char* treeName;
	public:
		static Node *selectObject;
		//����ŕ`�悷�邽�߂ɂ܂Ƃ߂Ă���
		//static NodeManager nodeManager;
		//GUI�E�B���h�E�̕`�惋�[��
		static ImGuiWindowFlags beharviorWindowFlags;
		//�{�^�����z�u����Ă���E�B���h�E�̕`�惋�[��
		static ImGuiWindowFlags beharviorButtonWindowFlags;
		//GUI�E�B���h�E�̍��W�R���e�i
		static ImVector<ImVec2> behaviarWindowPosisions;
		/// <summary>
		/// �m�[�h�����
		/// </summary>
		/// <param name="f_nodeName">�m�[�h�̖��O</param>
		/// <param name="f_type">�m�[�h�̎��</param>
		/// <param name="f_parent">�e�m�[�h�ɂȂ�m�[�h</param>
		static void CreateNode(const char* f_nodeName, NodeType f_type, Node*f_parent);
		//�S�̂̕`�恩����ŃO���b�g����Ȃ����`�悵�Ă��炤
		static void DrawImGui();
		//GUI�̃m�[�h����|���Ă��炤
		static void ClearNodes();
	};
}
