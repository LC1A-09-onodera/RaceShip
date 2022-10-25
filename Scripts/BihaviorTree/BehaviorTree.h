#pragma once
#include <string>
#include <list>
using namespace std;

namespace BehavirTree
{
	class GUIWindow
	{
	private:
		list<GUIWindow> _parent;
		list<GUIWindow> _children;
	public:
		void GetParent();
		void GUIDraw(string &f_name);
	};
	class Element
	{
	private:
		//優先度(使用割合)
		int _priority;
		string _name;
	public:
		int GetPriority()
		{
			return _priority;
		};
		string GetName()
		{
			return _name;
		}
	};

	//ルートオブジェクト
	class RootBehavior : public Element, public  GUIWindow
	{
		bool _isActive = false;
		bool GetIsActive()
		{
			return _isActive;
		};
		void StartElement()
		{
			_isActive = true;
		};
	};
	//セレクター
	class Selector : public  Element, public GUIWindow
	{
		
	};
	//シーケンス
	class Sequence : public Element, public GUIWindow
	{
		
	};
	//

}
