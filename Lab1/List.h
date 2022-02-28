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
			Iterator(List<T>* list, int index);
			T& operator *();
			bool operator++(int value);
			bool operator--(int value);
			bool operator == (Iterator iterator);
			bool operator != (Iterator iterator);
		private:
			List<T>* list = nullptr;
			Node& current;

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
		Node* array;
		void Remove(Node& node);
		int FindFreeIndex();

		class Node
		{
		public:
			Node(T value, int prevIndex, int nextIndex) :
				prevIndex(prevIndex), nextIndex(nextIndex), value(value) { }
			Node(const Node& node) :
				prevIndex(node.prevIndex), nextIndex(node.nextIndex), value(node.value) {}
			Node() {
				ResetIndexes();
			}

			bool IsEmpty()
			{
				return prevIndex == NO_INDEX && nextIndex == NO_INDEX;
			}
			void MakeEmpty() { ResetIndexes(); }
			bool operator == (Node node);
			bool operator != (Node node);
			int prevIndex;
			int nextIndex;
			T value;
		private:
			void ResetIndexes() { prevIndex = nextIndex= NO_INDEX; }
		};

	};


}
#pragma once 
#include "ListMethods.h"