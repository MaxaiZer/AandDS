#include <iostream>
#include "BST.h"

using namespace std;
typedef unsigned long long INT_64;

static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;

void setFirstRandomDigit() { srand(time(0)); RRand = (INT_64)rand(); }

//�������� ������������ ��������� Xi+1=(a*Xi+c)%m
INT_64 lineRand()
{
	INT_64 y1, y2;
	y1 = (aRand * RRand + cRand) % mRand;
	y2 = (aRand * y1 + cRand) % mRand;
	RRand = y1 & 0xFFFFFFFF00000000LL ^ (y2 &
		0xFFFFFFFF00000000LL) >> 32;
	return RRand;
}

INT_64 randomKey(int treeSize)
{
	return lineRand();
}

INT_64 randomOddKey(int treeSize)
{
	INT_64 k = lineRand() % (10000 * treeSize);
	k = k + !(k % 2);
	return k;
}

INT_64 randomEvenKey(int treeSize)
{
	INT_64 k = lineRand() % (10000 * treeSize);
	k = k + k % 2;
	return k;
}

template<class Key, class Data>
void testTree(Lab2::BST<Key, Data>& tree, Key* keys, Key (*getMissKey)(int), Key (*getSuccessKey)(int), double theoreticalComplexity)
{
	const int size = tree.GetSize();
	cout << "������ ������ �� �����: " << size << endl;

	int addReadedElements = 0;
	int removeReadedElements = 0;
	int searchReadedElements = 0;

	const int probabilityOfMiss = 10;
	const int operationsCount = size / 2;

	for (int i = 0; i < operationsCount; i++)
	{
		if (i % probabilityOfMiss == 0) //miss
		{
			tree.Remove(getMissKey(size));
			removeReadedElements += tree.GetReadedElementsCount();

			tree.Add(keys[lineRand() % size], 1);
			addReadedElements += tree.GetReadedElementsCount();

			try 
			{
				tree[getMissKey(size)];
			}
			catch (const char* ex) {}
			searchReadedElements += tree.GetReadedElementsCount();
		}
		else //success
		{
			int index = lineRand() % size;
			tree.Remove(keys[index]);
			removeReadedElements += tree.GetReadedElementsCount();

			Key key = getSuccessKey(size);
			tree.Add(key, 1);
			addReadedElements += tree.GetReadedElementsCount();
			keys[index] = key;

			try 
			{
				tree[keys[lineRand() % size]];
			}
			catch (const char* ex) {}
			searchReadedElements += tree.GetReadedElementsCount();
		}

	}

	cout << "������ ������ ����� �����:" << tree.GetSize() << endl;

	cout << "������������� �����������:" << theoreticalComplexity << endl;

	cout << "����������� �������: " << addReadedElements * 1.0 / operationsCount << endl;
	cout << "����������� ��������: " << removeReadedElements * 1.0 / operationsCount << endl;
	cout << "����������� ������: " << searchReadedElements * 1.0 / operationsCount << endl;
}

void testRandomTree(int size)
{
	BST< INT_64, int > tree;
	INT_64* keys = new INT_64[size];

	setFirstRandomDigit();

	for (int i = 0; i < size; i++)
	{
		keys[i] = lineRand();
		tree.Add(keys[i], 1);
	}

	double theoreticalComplexity = 1.39 * (log((double)size) / log(2.0));
	testTree(tree, keys, randomKey, randomKey, theoreticalComplexity);

	delete[] keys;
}

void testOrdinaryTree(int size)
{
	BST< INT_64, int > tree;
	INT_64* keys = new INT_64[size];

	setFirstRandomDigit();

	for (int i = 0; i < size; i++) {
		keys[i] = i * 10000;
		tree.Add(keys[i], 1);
	}
	
	double theoreticalComplexity = tree.GetSize() * 1.0 / 2;
	testTree(tree, keys, randomOddKey, randomEvenKey, theoreticalComplexity);

	delete[] keys;
} 

void printCommands()
{
	cout << "0 - ������� ������" << endl;
	cout <<	"1 - �������� ������" << endl; 
	cout << "2 - �������� �������" << endl; 
	cout << "3 - �������� ������� �� �����" << endl;
	cout << "4 - �������� ������� �� �����" << endl;
	cout << "5 - ������� ������� �� �����" << endl; 
	cout << "6 - �������� ������ ������" << endl; 
	cout << "7 - ���������� ������������� ���������" << endl; 
	cout << "8 - ������� ������ ������ (L -> R -> t)" << endl;
	cout << "9 - ���� ����������� ���������� ������" << endl; 
	cout << "10 - ���� ����������� ������������ ������" << endl;
	cout << "11 - ������ ��������: ���������� �� ������ ������" << endl;
	cout << "12 - ������ ��������: ��������� �������" << endl;
	cout << "13 - ������ ��������: ���������� �������" << endl;
	cout << "14 - ������ ��������: ������������" << endl;
	cout << "15 - ������ ��������: ����� �� End()?" << endl;
	cout << "16 - �������� ��������: ���������� �� ����� ������" << endl;
	cout << "17 - �������� ��������: ��������� �������" << endl;
	cout << "18 - �������� ��������: ���������� �������" << endl;
	cout << "19 - �������� ��������: ������������" << endl;
	cout << "20 - �������� ��������: ����� �� Rend()?" << endl;
}

int inputValue(string hintForUser)
{
	cout << hintForUser << ":";
	int input;
	while (!(cin >> input))
	{
		cin.clear(); //���������� ��, ����� ����
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	return input;
}

struct Iterators
{
	Lab2::BST<int, int>::Iterator forward;
	Lab2::BST<int, int>::ReverseIterator reverse;
};

void handleInput(int input, Lab2::BST<int,int>& bst, Iterators& iters)
{
	switch (input)
	{
	case 0:
		bst.Print();
		break;
	case 1:
		bst.Clear();
		break;
	case 2:
		cout << "����� ������: " << bst.Add(inputValue("����"), inputValue("��������")) << endl;
		break;
	case 3:
	{
		int e;
		try { e = bst[inputValue("����")]; }
		catch (const char* ex)
		{
			cout << ex << endl;
			break;
		};
		cout << e << endl;
		break;
	}
	case 4:
		try { bst[inputValue("����")] = inputValue("��������"); }
		catch (const char* ex)
		{
			cout << ex << endl;
			break;
		};
		break;
	case 5:
		cout << "����� ������: " << bst.Remove(inputValue("����")) << endl;
		break;
	case 6:
		cout << bst.GetSize() << endl;
		break;
	case 7:
		cout << bst.GetReadedElementsCount() << endl;
		break;
	case 8:
	{
		Lab1::List<int> keys = bst.GetKeysList();
		keys.Print();
		break;
	}
	case 9:
		testRandomTree(inputValue("������ ������������ ������"));
		break;
	case 10:
		testOrdinaryTree(inputValue("������ ������������ ������"));
		break;
	case 11:
		iters.forward = bst.Begin();
		break;
	case 12:
		cout << "����� ������: " << iters.forward++ << endl;
		break;
	case 13:
		cout << "����� ������: " << iters.forward-- << endl;
		break;
	case 14:
		cout << *iters.forward << endl;
		break;
	case 15:
		cout << (iters.forward == bst.End()) << endl;
		break;
	case 16:
		iters.reverse = bst.Rbegin();
		break;
	case 17:
		cout << "����� ������: " << iters.reverse++ << endl;
		break;
	case 18:
		cout << "����� ������: " << iters.reverse-- << endl;
		break;
	case 19:
		cout << *iters.reverse << endl;
		break;
	case 20:
		cout << (iters.reverse == bst.Rend()) << endl;
		break;
	default:
		cout << "������������ ����� �������" << endl;
		break;
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	Lab2::BST<int, int> bst;
	Iterators iters;

	printCommands();
	
	while (true)
	{
		handleInput(inputValue("����� �������"), bst, iters);
	}
}