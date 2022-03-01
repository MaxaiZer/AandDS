const int NO_INDEX = -1;

namespace Lab1
{

	template <class T>
	class List
	{
		class Node;

	public:
		class Iterator
		{
		public:
			Iterator(List<T>& list, int index) : list(list) { current = &list.array[index]; }
			T& operator *() { return current->value; }
			bool operator++(int value);
			bool operator--(int value);
			bool operator == (Iterator iterator);
			bool operator != (Iterator iterator);
		private:
			List<T>& list;
			Node* current;

			friend class List;
		};

		List(int capacity); //�����������
		List(const List<T>&); //����������� �����������
		~List() { delete array; }; //����������
		int GetSize() const { return size; }//����� ������� ������
		void Clear(); //������� ������
		bool IsEmpty() const { return size == 0; } //�������� ������ �� �������
		bool Contains(T value); //����� ������� ��������� ��������
		T& operator[] (const int index); //������/��������� �������� � �������� ������� � ������
		int GetPos(T value); //��������� ������� � ������ ��� ��������� ��������
		bool Add(T value); //��������� ������ ��������
		bool Add(T value, int pos); //��������� ������ �������� � ������� � �������� �������		
		bool RemoveByValue(T value); //�������� ��������� �������� �� ������
		bool RemoveByPos(int pos); //�������� �������� �� ������� � �������� �������
		Iterator Begin(); //������ ������� ��������� begin()
		Iterator End(); //������ ����������������� ������� ��������� end()
		void Print();

	protected:
		int capacity;
		int size = 0;
		int startIndex = NO_INDEX;
		int endIndex = NO_INDEX;
		int firstFreeIndex = 0;
		Node* array;
		void Remove(Node& node);
		Node& GetFreeNode(int& index);
		bool LinkAsPrevAndNext(int index1, int index2);

		class Node
		{
		public:
			Node(const Node& node) :
				prevIndex(node.prevIndex), nextIndex(node.nextIndex), value(node.value), index(index) {}
			Node() { }
			void ResetIndexes() { prevIndex = nextIndex = NO_INDEX; }
			bool operator == (Node node);
			bool operator != (Node node);
			int prevIndex = NO_INDEX;
			int index = NO_INDEX;
			int nextIndex = NO_INDEX;
			T value;
		};

	};


}
#pragma once 
#include "ListMethods.h"