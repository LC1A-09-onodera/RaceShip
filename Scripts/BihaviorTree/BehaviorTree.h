#pragma once
#include <string>
#include <list>
#include <memory>
//�C���N���[�h�͎����Œ������Ă�������
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx12.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"

namespace BehaviorTree
{
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
	//�m�[�h�̊�{�N���X
	class Node
	{
	private:
		std::list<Node*> _children;

		//�D��x(�g�p����)
		int _priority = 0;
		//�m�[�h�̖��O
		std::string _name = "";
		//�m�[�h�̎��s���t���O
		bool _isActive = false;
		//�X�e�[�^�X
		NowState _nowState = NowState::Stay;
		//Node�̎�ނ��i�[
		NodeType _nodeType = NodeType::e_NONE;
		//GUI�̕`�惋�[��
		ImGuiWindowFlags _flags = 0;
		//GUI�E�B���h�E�̍��W
		ImVec2 _windowPos = { 0, 0 };
	public:
		//�e�m�[�h
		Node* _parent = nullptr;
		//�����ɂ��ׂẴm�[�h���i�[
		//�`����s��
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
		//�R���X�g���N�^�E�f�X�g���N�^
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
	//�e�q������
	static void SetParentAndChild(Node& f_parent, Node& f_child)
	{
		f_parent.Children()->push_back(&f_child);
		if (f_child._parent == nullptr)
		{
			f_child._parent = new Node();
		}
		f_child._parent = &f_parent;
	};

	//----------------------�ȉ��e��m�[�h-----------------------
	//���[�g�I�u�W�F�N�g
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
	//�Z���N�^�[
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
	//�V�[�P���X
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
	//�^�X�N
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
		//���ۂɍs���֐��̏������z�֐�
		//�p����Ŋ֐��̃A�N�V���������߂�
		//virtual bool Task() = 0;
	};
	//-----------------------------------------------------------


	//-----------------------�ȉ�GUI����p-----------------------
	class BehavierImGui
	{
	private:
		//Behavior�̃��[�g�I�u�W�F�N�g
		static RootBehavior rootObject;
		
		//�m�[�h�̖��O���i�[
		static char nameBuf[256];
		static const char *nodeName;
	public:
		static Node *selectObject;
		//����ŕ`�悷�邽�߂ɂ܂Ƃ߂Ă���
		static Node::NodeManager nodeManager;
		static ImGuiWindowFlags beharviorWindowFlags;
		static ImVector<ImVec2> behaviarWindowPosisions;
		static void CreateNode(const char* f_nodeName, NodeType f_type, Node*f_parent);
		static void DrawImGui();
		static void ClearNodes();
	};
}
