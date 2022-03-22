namespace Lab1
{

	const int NO_INDEX = -1; //��������, ���� ������� �� ����������

	template <class T>
	class List
	{
		class Node;

	public:
		class Iterator
		{
		public:
			Iterator() {}; 
			Iterator(List<T>& list, int pos);
			T& operator *() const; //���������� ������ �� �������� �������� ��������
			Iterator& operator=(const Iterator& iter) {
				list = iter.list; current = iter.current;
				return *this;
			};
			bool operator++(int value); //������� � ���������� ��������
			bool operator--(int value); //������� � ����������� ��������
			bool operator == (Iterator iterator) const; 
			bool operator != (Iterator iterator) const { return !(*this == iterator); }
		protected:
			List<T>* list = nullptr; //��������� �� ������
			Node* current = nullptr; //��������� �� ������� ������� ������

			friend class List;
		};

		List(): List(1) {} 
		List(int capacity); 
		List(const List<T>&);
		~List() { delete[] array; }; 
		int GetSize() const { return size; }
		void Clear(); 
		bool IsEmpty() const { return size == 0; } 
		bool Contains(T value) const; 
		T& operator[] (int pos); //������/��������� �������� � �������� ������� � ������
		int GetPos(T value) const; 
		void Add(T value);
		bool Add(T value, int pos); 
		bool RemoveByValue(T value); 
		bool RemoveByPos(int pos); 
		Iterator Begin(); //������ ������� ���������
		Iterator End(); //������ ����������������� ������� ���������
		int GetReadedElementsCount() const { return readedElements; } //������ ����� ���������, ������������� ��������� ���������
		void Print(); 

	protected:
		int capacity;
		int size = 0; 
		mutable int readedElements = 0; //���������� ������������� ��������� ��������� ���������
		int startIndex = NO_INDEX; //������ ������� ��������
		int endIndex = NO_INDEX; //������ ���������� ��������
		int firstFreeIndex = NO_INDEX; //������ ��������� ������ � �������
		Node* array = nullptr; //�������. ������ ���������

		void CreateArrayWithFreeNodes(int capacity); //�������� ������� �� ���������� ���������� � ����������� ������
		void IncreaseArray(); //���������� ������� ������� �� 1
		void Remove(Node& node); 
		Node& GetFreeNode(int& index); //���������� ��������� ������� � �������
		bool FindNodeByPos(int &index, int pos) const;
		bool FindNodeByValue(int &index, int& pos, T value) const;
		bool LinkAsPrevAndNext(int index1, int index2);	 //������ ��������� ��� ���������� � �����������

		class Node
		{
		public:
			int prevIndex = NO_INDEX; //������ ����������� ��������
			int index = NO_INDEX; //����������� ������ 
			int nextIndex = NO_INDEX; //������ ���������� ��������
			T value;

			Node() { } 
			void ResetIndexes() { prevIndex = nextIndex = NO_INDEX; } //����� ��������
		};

	};


}
#pragma once 
#include "ListMethods.h"