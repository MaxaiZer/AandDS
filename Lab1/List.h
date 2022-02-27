
template <class T>
class List
{
	class Node;

public:
	class Iterator
	{
	public:
		Iterator(List<T>* list, Node* node);
		T& operator *();
		void operator++(int value);
		void operator--(int value);
		bool operator == (Iterator iterator);
		bool operator != (Iterator iterator);
	private:
		List<T>* list;
		Node* cur;
	};

	List() {}; //�����������
	List(const List<T>&); //����������� �����������
	~List(); //����������
	int getSize() const; //����� ������� ������
	void clear(); //������� ������
	bool isEmpty() const; //�������� ������ �� �������
	bool contains(T value) const; //����� ������� ��������� ��������
	T read(int index) const; //������ �������� � �������� ������� � ������
	bool set(T value, int pos); //��������� �������� � �������� ������� � ������	
	int getPos(T value) const; //��������� ������� � ������ ��� ��������� ��������
	void add(T value); //��������� ������ ��������
	bool add(T value, int pos); //��������� ������ �������� � ������� � �������� �������		
	bool removeByValue(T value); //�������� ��������� �������� �� ������
	bool removeByPos(int pos); //�������� �������� �� ������� � �������� �������
	Iterator begin(); //������ ������� ��������� begin()
	Iterator end(); //������ ����������������� ������� ��������� end()
	void print();

protected:
	int size = 0;
	Node* start = nullptr;
	void remove(Node*);

	class Node
	{
	public:
		Node(T value, Node* prev, Node* next): prev(prev), next(next), value(value) { }
		Node* prev;
		Node* next;
		T value;
	};

};



#pragma once 
#include "ListMethods.h"