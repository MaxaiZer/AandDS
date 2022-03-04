namespace Lab1
{

	const int NO_INDEX = -1;

	template <class T>
	class List
	{
		class Node;

	public:
		class Iterator
		{
		public:
			Iterator(List<T>& list, int pos);
			T& operator *();
			Iterator& operator=(const Iterator& iter) {
				list = iter.list; current = iter.current; isInstalled = iter.isInstalled;
				return *this;
			};
			bool operator++(int value);
			bool operator--(int value);
			bool operator == (Iterator iterator);
			bool operator != (Iterator iterator);
		private:
			List<T>* list;
			Node* current = nullptr;
			bool isInstalled = false;

			friend class List;
		};

		List(): List(1) {} //����������� �� ���������
		List(int capacity); //����������� � ����������
		List(const List<T>&); //����������� �����������
		~List() { delete array; }; //����������
		int GetSize() const { return size; }//����� ������� ������
		void Clear(); //������� ������
		bool IsEmpty() const { return size == 0; } //�������� ������ �� �������
		bool Contains(T value); //����� ������� ��������� ��������
		T& operator[] (int index); //������/��������� �������� � �������� ������� � ������
		int GetPos(T value); //��������� ������� � ������ ��� ��������� ��������
		void Add(T value); //��������� ������ ��������
		bool Add(T value, int pos); //��������� ������ �������� � ������� � �������� �������		
		bool RemoveByValue(T value); //�������� ��������� �������� �� ������
		bool RemoveByPos(int pos); //�������� �������� �� ������� � �������� �������
		Iterator Begin(); //������ ������� ��������� begin()
		Iterator End(); //������ ����������������� ������� ��������� end()
		int GetReadedElemetsCount() const { return readedElements; } //������ ����� ��������� ������, ������������� ���������� ���������
		void Print(); //����� ������ �� �����

	protected:
		int capacity;
		int size = 0;
		int readedElements = 0;
		int startIndex = NO_INDEX;
		int endIndex = NO_INDEX;
		int firstFreeIndex = 0;
		Node* array;
		void CreateArrayWithFreeNodes(int capacity);
		void Remove(Node& node);
		Node& GetFreeNode(int& index);
		bool FindNodeByPos(int &index, int pos);
		bool FindNodeByValue(int &index, int& pos, T value);
		bool LinkAsPrevAndNext(int index1, int index2);
		void IncreaseArray();

		class Node
		{
		public:
			Node(const Node& node) :
				prevIndex(node.prevIndex), nextIndex(node.nextIndex), value(node.value), index(node.index) {}
			Node() { }
			void ResetIndexes() { prevIndex = nextIndex = NO_INDEX; }
			int prevIndex = NO_INDEX;
			int index = NO_INDEX;
			int nextIndex = NO_INDEX;
			T value;
		};

	};


}
#pragma once 
#include "ListMethods.h"