#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "HashTable.h"
#include "TableTest.h"

#define commandView pair<Command, string>

using namespace std;
using namespace Lab4;

enum class Command {
	Print, Clear, Add, GetByKey, ChangeByKey, RemoveByKey, 
	GetSize, GetForm, SwitchToOpenAddr, SwitchToChainsColl,
	TestOpenAddr, TestChainsColl,
	GetTrialsCount, IterSetBegin, IterNext, 
	IterGet, IsIterEnd, PrintCommands, Exit
};

vector<commandView> commandsView = {
{Command::IterSetBegin, "Прямой итератор: установить на начало таблицы"},
{Command::Print, "Вывести таблицу"},
{Command::IterNext, "Прямой итератор: следующий элемент"},
{Command::Clear, "Очистить таблицу"},
{Command::Add, "Добавить элемент"},
{Command::IterGet, "Прямой итератор: получить значение"},
{Command::GetByKey, "Получить значение по ключу"},
{Command::IsIterEnd, "Прямой итератор: равен ли End()?"},
{Command::RemoveByKey, "Удалить значение по ключу"},
{Command::GetSize, "Получить размер таблицы"},
{Command::GetTrialsCount, "Получить количество проб"},
{Command::PrintCommands, "Вывести команды"},
{Command::Exit, "Выход"},
{Command::SwitchToOpenAddr, "Сменить форму таблицы: открытая адресация"},
{Command::SwitchToChainsColl, "Сменить форму таблицы: цепочки коллизий"},
{Command::TestOpenAddr, "Тестирование таблицы: открытая адресация"},
{Command::TestChainsColl, "Тестирование таблицы: цепочки коллизий"},
{Command::ChangeByKey, "Изменить элемент по ключу"},
{Command::GetForm, "Опрос формы представления"}
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

		cout << string(60 - len - commandsView[i].second.length(), ' ');
		printCommand(commandsView[j]);

		cout << endl;
	}

	if (commandsView.size() % 2 != 0)
	{
		printCommand(commandsView.back());
		cout << endl;
	}

}

int inputInt(string hintForUser)
{
	cout << hintForUser << ":";
	int input;
	cin.clear();
	while (!(cin >> input))
	{
		cin.clear(); //игнорируем всё, кроме цифр
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Only numbers are required" << endl;
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return input;
}

float inputFloat(string hintForUser)
{
	cout << hintForUser << " (10000.0000 - 15000.0000):";

	float input;

	do
	{
		cin >> input;

		if (input < 10000.0 || input > 15000.0)
			goto error;

		break;
	error:
		cout << "Wrong value" << endl;
	} while (true);
	
	return input;
}

void handleInput(int input, HashTable<float, int>& table, HashTable<float,int>::Iterator& iter)
{
	switch (input)
	{
	case (int)Command::Print:
		table.Print();
		break;
	case (int)Command::Clear:
		table.Clear();
		break;
	case (int)Command::Add:
		printf("Метод вернул: %d\n", table.Add(inputFloat("Ключ"), 1));
		break;
	case (int)Command::GetByKey:
	{
		int e;
		try { e = table[inputFloat("Ключ")]; }
		catch (const char* ex)
		{
			cout << ex << endl;
			break;
		};
		cout << e << endl;
		break;
	}
	case (int)Command::ChangeByKey:
		try { table[inputFloat("Ключ")] =  inputInt("Значение"); }
		catch (const char* ex)
		{
			cout << ex << endl;
			break;
		};
		break; 
	case (int)Command::RemoveByKey:
		printf("Метод вернул: %d\n", table.Remove(inputFloat("Ключ")));
		break;
	case (int)Command::GetSize:
		cout << table.GetSize() << endl;
		break;
	case (int)Command::GetTrialsCount:
		cout << table.GetTrialsCount() << endl;
		break;
	case (int)Command::IterSetBegin:
		iter = table.Begin();
		break;
	case (int)Command::IterNext:
		cout << "Метод вернул: " << iter++ << endl;
		break;
	case (int)Command::IterGet:
		try { cout << *iter << endl; }
		catch (const char* ex) { cout << ex << endl; }
		break;
	case (int)Command::IsIterEnd:
		cout << (iter == table.End()) << endl;
		break;
	case (int)Command::SwitchToOpenAddr:
		table.SetForm(FormName::OpenAddressing);
		break;
	case (int)Command::SwitchToChainsColl:
		table.SetForm(FormName::ChainsOfCollisions);
		break;
	case (int)Command::TestOpenAddr:
	{
		int keysCount = inputInt("Количество ключей");
		testOpenAddr(keysCount);
		break;
	}
	case (int)Command::TestChainsColl:
	{
		int keysCount = inputInt("Количество ключей");
		testChainsColl(keysCount);
		break;
	}
	case (int)Command::GetForm:
		if (table.GetForm() == FormName::OpenAddressing)
			cout << "Открытая адресация" << endl;
		else
			cout << "С цепочками коллизий" << endl;
		break;
	case (int)Command::PrintCommands:
		system("CLS");
		printCommands();
		break;
	case(int)Command::Exit:
	{
		exit(0);
		break;
	}
	default:
		cout << "Неправильный номер команды" << endl;
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

	HashTable<float, int> table(10);
	HashTable<float, int>::Iterator iter;

	printCommands();

	while (true)
	{
		handleInput(inputInt("Номер команды"), table, iter);
	}
}