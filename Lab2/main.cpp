#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "BST.h"

#define commandView pair<Command, const char*>

using namespace std;
using namespace Lab2;

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

template<class K, class V>
void testTree(Lab2::BST<K, V>& tree, K* keys, K (*getMissKey)(int), K (*getSuccessKey)(int), double theoreticalComplexity)
{
	const int size = tree.GetSize();
	cout << "������ ������ �� �����: " << size << endl;

	int addReadElements = 0;
	int removeReadElements = 0;
	int searchReadElements = 0;

	const int probabilityOfMiss = 10;
	const int operationsCount = size / 2;

	for (int i = 0; i < operationsCount; i++)
	{
		if (i % probabilityOfMiss == 0) //miss
		{
			tree.Remove(getMissKey(size));
			removeReadElements += tree.GetReadElementsCount();

			tree.Add(keys[lineRand() % size], 1);
			addReadElements += tree.GetReadElementsCount();

			try 
			{
				tree[getMissKey(size)];
			}
			catch (const char* ex) {}
			searchReadElements += tree.GetReadElementsCount();
		}
		else //success
		{
			int index = lineRand() % size;
			tree.Remove(keys[index]);
			removeReadElements += tree.GetReadElementsCount();

			K key = getSuccessKey(size);
			tree.Add(key, 1);
			addReadElements += tree.GetReadElementsCount();
			keys[index] = key;

			try 
			{
				tree[keys[lineRand() % size]];
			}
			catch (const char* ex) {}
			searchReadElements += tree.GetReadElementsCount();
		}

	}

	cout << "������ ������ ����� �����:" << tree.GetSize() << endl;

	cout << "������������� �����������:" << theoreticalComplexity << endl;

	cout << "����������� �������: " << addReadElements * 1.0 / operationsCount << endl;
	cout << "����������� ��������: " << removeReadElements * 1.0 / operationsCount << endl;
	cout << "����������� ������: " << searchReadElements * 1.0 / operationsCount << endl;
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

enum class Command { Fulfill, Print, Clear,  Add, GetByKey, ChangeByKey,
					RemoveByKey, GetSize, GetReadElemsCount, 
					GetKeys, TestRandomTree, TestOrdinaryTree,
					IterSetBegin, IterNext, IterPrev, IterGet, IsIterEnd,
					RIterSetRBegin, RIterNext, RIterPrev, RIterGet, IsRIterRend, PrintCommands, Exit};

vector<pair<Command, const char*>> commandsView = { 
{Command::Fulfill, "��������� ������ ��� �����"},
{Command::IterSetBegin, "������ ��������: ���������� �� ������ ������"},
{Command::Print, "������� ������"},
{Command::IterNext, "������ ��������: ��������� �������"},
{Command::Clear, "�������� ������"},
{Command::IterPrev, "������ ��������: ���������� �������"},
{Command::Add, "�������� �������"},
{Command::IterGet, "������ ��������: �������� ��������"},
{Command::GetByKey, "�������� ������� �� �����"},
{Command::IterGet, "������ ��������: �������� ��������"},
{Command::ChangeByKey, "�������� ������� �� �����"},
{Command::IsIterEnd, "������ ��������: ����� �� End()?"},
{Command::RemoveByKey, "������� ������� �� �����"},
{Command::RIterSetRBegin, "�������� ��������: ���������� �� ����� ������"},
{Command::GetSize, "�������� ������ ������"},
{Command::RIterNext, "�������� ��������: ��������� �������"},
{Command::GetReadElemsCount, "���������� ������������� ���������"},
{Command::RIterPrev, "�������� ��������: ���������� �������"},
{Command::GetKeys, "������� ������ ������ (L -> R -> t)"},
{Command::RIterGet, "�������� ��������: �������� ��������"},
{Command::TestRandomTree, "���� ����������� ���������� ������"},
{Command::IsRIterRend, "�������� ��������: ����� �� Rend()?"},
{Command::TestOrdinaryTree, "���� ����������� ������������ ������"},
{Command::PrintCommands, "������� �������"},
{Command::Exit, "�����"}
};

void printCommands()
{
	auto printCommand = [](commandView view)
	{ 
		cout << (int)view.first << " - " << view.second; 
	};

	for (int i = 0, j = commandsView.size() / 2; i < commandsView.size() / 2; i++, j++)
	{
		printCommand(commandsView[i]);

		int k = (int)commandsView[i].first / 10;
		int len = 1;
		
		while (k != 0)
		{
			len++; k /= 10;
		}

		for (int s = strlen(commandsView[i].second); s < 60 - len; s++)
			cout << " ";

		if (j < commandsView.size())
			printCommand(commandsView[j]);

		cout << endl;
	}
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
	case (int)Command::Print:
		bst.Print();
		break;
	case (int)Command::Clear:
		bst.Clear();
		break;
	case (int)Command::Add:
		cout << "����� ������: " << bst.Add(inputValue("����"), inputValue("��������")) << endl;
		break;
	case (int)Command::GetByKey:
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
	case (int)Command::ChangeByKey:
		try { bst[inputValue("����")] = inputValue("��������"); }
		catch (const char* ex)
		{
			cout << ex << endl;
			break;
		};
		break;
	case (int)Command::RemoveByKey:
		cout << "����� ������: " << bst.Remove(inputValue("����")) << endl;
		break;
	case (int)Command::GetSize:
		cout << bst.GetSize() << endl;
		break;
	case (int)Command::GetReadElemsCount:
		cout << bst.GetReadElementsCount() << endl;
		break;
	case (int)Command::GetKeys:
	{
		Lab1::List<int> keys = bst.GetKeysList();
		keys.Print();
		break;
	}
	case (int)Command::TestRandomTree:
		testRandomTree(inputValue("������ ������������ ������"));
		break;
	case (int)Command::TestOrdinaryTree:
		testOrdinaryTree(inputValue("������ ������������ ������"));
		break;
		case (int)Command::IterSetBegin:
		iters.forward = bst.Begin();
		break;
	case (int)Command::IterNext:
		cout << "����� ������: " << iters.forward++ << endl;
		break;
	case (int)Command::IterPrev:
		cout << "����� ������: " << iters.forward-- << endl;
		break;
	case (int)Command::IterGet:
		cout << *iters.forward << endl;
		break;
	case (int)Command::IsIterEnd:
		cout << (iters.forward == bst.End()) << endl;
		break;
	case (int)Command::RIterSetRBegin:
		iters.reverse = bst.Rbegin();
		break;
	case (int)Command::RIterNext:
		cout << "����� ������: " << iters.reverse++ << endl;
		break;
	case (int)Command::RIterPrev:
		cout << "����� ������: " << iters.reverse-- << endl;
		break;
	case (int)Command::RIterGet:
		cout << *iters.reverse << endl;
		break;
	case (int)Command::IsRIterRend:
		cout << (iters.reverse == bst.Rend()) << endl;
		break;
	case (int)Command::PrintCommands:
		system("CLS");
		printCommands();
		break;
	case(int)Command::Fulfill:
	{
		int size = inputValue("������");
		if (size <= 1)
			return;

		bst.Clear();

		int* keys = new int[size];
		bool* isAdded = new bool[size];
		for (int i = 0; i < size; i++)
		{
			keys[i] = i+1; isAdded[i] = false;
		}

		bst.Add(keys[size / 2], keys[size / 2]);
		isAdded[size / 2] = true;

		int distance = 0;
		int inserts = 0;
		while (inserts != size - 1)
		{
			for (int i = 0; i < size; i++)
			{
				if (isAdded[i] || i == size - 1)
				{
					if (distance == 0 && i != size - 1)
						continue;
					int id = i - (distance+1) / 2;

					if (isAdded[id])
						continue;

					inserts++;
					bst.Add(keys[id], keys[id]);
					isAdded[id] = true;
					distance = 0;
				}
				else distance++;
			}
		}

		break;
	}
	case(int)Command::Exit:
	{
		exit(0);
		break;
	}
	default:
		cout << "������������ ����� �������" << endl;
		break;
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	auto sortCommands = [](commandView p1, commandView p2)
	{
		return (int)p1.first < (int)p2.first;
	};
	sort(commandsView.begin(), commandsView.end(), sortCommands);

	Lab2::BST<int, int> bst;
	Iterators iters;

	printCommands();
	
	while (true)
	{
		handleInput(inputValue("����� �������"), bst, iters);
	}
}