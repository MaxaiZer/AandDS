#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Graph.h"
#include "VertexMap.h"
#include "Input.h"
#include "Task.h"

using namespace std;

using commandView = pair<int, string>;
using _Vertex = Vertex<string, int>;
using _Edge = Edge<_Vertex, int, string>;
using RGZGraph = Graph<_Vertex, _Edge>;

#define SUCCESS_INPUT 0
#define BACK_TO_MENU -1

RGZGraph* graph = new RGZGraph();
VertexMap<_Vertex, _Edge> vertexMap(*graph);
RGZGraph::VertexesIterator vIterator(*graph);
RGZGraph::EdgesIterator eIterator(*graph);
RGZGraph::OutputEdgesIterator outEIterator(*graph, nullptr);

Task1v13<_Vertex, _Edge>* task1 = nullptr;
Task2v14<_Vertex, _Edge>* task2 = nullptr;

void printCommands(vector<commandView> commandsView)
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

		cout << string(70 - len - commandsView[i].second.length(), ' ');
		printCommand(commandsView[j]);

		cout << endl;
	}

	if (commandsView.size() % 2 != 0)
	{
		printCommand(commandsView.back());
		cout << endl;
	}

}

bool GetVertex(_Vertex** vertex, string hint)
{
	*vertex = vertexMap.GetVertex(Input<string>::Get(hint));

	if (*vertex == nullptr)
	{
		cout << "������� �� �������\n";
		return false;
	}
	return true;
}

bool GetTwoVertixes(_Vertex** array)
{
	for (int i = 0; i < 2; i++)
	{
		if (!GetVertex(&array[i], "��� ������� #" + to_string(i + 1)))
			return false;
	}
	return true;
}

bool GetEdge(_Edge** edge)
{
	_Vertex* vertexes[2];

	if (!GetTwoVertixes(vertexes))
		return false;

	*edge = graph->GetEdge(vertexes[0], vertexes[1]);

	if (*edge == nullptr)
	{
		cout << "����� �� �������\n";
		return false;
	}
		
	return true;
}

void UpdateGraphHelpers()
{
	vIterator = RGZGraph::VertexesIterator(*graph);
	eIterator = RGZGraph::EdgesIterator(*graph);
	outEIterator = RGZGraph::OutputEdgesIterator(*graph, nullptr);
	vertexMap = VertexMap <_Vertex, _Edge>(*graph);
}

class CommandsHandler
{
public:
	virtual int HandleInput() = 0;
protected:
	virtual void PrintCommands() = 0;
};

class GraphCommands : public CommandsHandler
{
public:
	GraphCommands() { PrintCommands(); }

	enum class Command { Create, CreateVDF, CreateVEDF, IsDirected, GetForm, SetForm, 
		GetEdgesCount, AddEdge, GetEdge, SetEdgeData, GetEdgeData, SetEdgeWeight, GetEdgeWeight,
		RemoveEdge, GetVertexCount, AddVertex, SetVertexData, GetVertexData, RemoveVertex, 
		Print, PrintCommands, ReturnToMenu
	};

	int HandleInput()
	{
		int input = Input<int>::Get("����� �������");

		#define TRY_GET_EDGE _Edge* edge; if (!GetEdge(&edge)) break;
		#define TRY_GET_VERTEX _Vertex* vertex = vertexMap.GetVertex(Input<string>::Get("��� �������")); if (vertex == nullptr) break;

		switch (input)
		{
		case (int)Command::Create:
			delete graph;
			graph = new RGZGraph();
			UpdateGraphHelpers();
			break;
		case (int)Command::CreateVDF:
		{
			delete graph;
			RGZGraph::Form form = (RGZGraph::Form)(Input<bool>::Get("����� ����� L/M"));
			graph = new RGZGraph(Input<int>::Get("���������� ������"), Input<bool>::Get("������������ �� ����"), form);
			UpdateGraphHelpers();
			vertexMap.SetNamesToAllVertexes();
			break;
		}
		case (int)Command::CreateVEDF:
		{
			delete graph;
			RGZGraph::Form form = (RGZGraph::Form)(Input<bool>::Get("����� ����� L/M"));
			graph = new RGZGraph(Input<int>::Get("���������� ������"), Input<int>::Get("���������� ��������� ����"), Input<bool>::Get("������������ �� ����"), form);
			UpdateGraphHelpers();
			vertexMap.SetNamesToAllVertexes();
			break;
		}
		case (int)Command::PrintCommands:
			PrintCommands();
			break;
		case (int)Command::ReturnToMenu:
			return BACK_TO_MENU;
		case (int)Command::IsDirected:
			cout << graph->IsDirected() << endl;
			break;
		case (int)Command::GetForm:
			cout << (graph->GetForm() == RGZGraph::Form::L ? "L\n" : "M\n");
			break;
		case (int)Command::Print:
			graph->Print();
			break;
		case (int)Command::SetForm:
			graph->SetForm(RGZGraph::Form(Input<bool>::Get("����� ����� (L/M)")));
			break;
		case (int)Command::AddVertex:
		{
			cout << "����� ������: " << vertexMap.AddVertex(Input<string>::Get("��� �������")) << endl;
			break;
		}
		case (int)Command::RemoveVertex:
		{
			cout << "����� ������: " << vertexMap.RemoveVertex(Input<string>::Get("��� �������")) << endl;
			break;
		}
		case (int)Command::AddEdge:
		{
			_Vertex* vertexes[2];
			if (!GetTwoVertixes(vertexes))
				break;

			_Edge* edge = graph->AddEdge(vertexes[0], vertexes[1]);
			cout << (edge == nullptr ? "Fail\n" : "Success\n");
			break;
		}
		case (int)Command::RemoveEdge:
		{
			_Vertex* vertexes[2];
			if (!GetTwoVertixes(vertexes))
				break;

			cout << "����� ������: " << graph->RemoveEdge(vertexes[0], vertexes[1]) << endl;
			break;
		}
		case (int)Command::GetEdge:
		{
			TRY_GET_EDGE
				edge->Print();
			cout << endl;
			break;
		}
		case (int)Command::SetEdgeData:
		{
			TRY_GET_EDGE
				edge->SetData(Input<string>::Get("������"));
			break;
		}
		case (int)Command::GetEdgeData:
		{
			TRY_GET_EDGE
				if (edge->HasData())
					cout << edge->GetData() << endl;
				else
					cout << "������ �� ������\n";
			break;
		}
		case (int)Command::SetEdgeWeight:
		{
			TRY_GET_EDGE
				edge->SetWeight(Input<int>::Get("���"));
			break;
		}
		case (int)Command::GetEdgeWeight:
		{
			TRY_GET_EDGE
				if (edge->HasWeight())
					cout << edge->GetWeight() << endl;
				else
					cout << "��� �� �����\n";;
			break;
		}
		case (int)Command::SetVertexData:
		{
			TRY_GET_VERTEX
				vertex->SetData(Input<int>::Get("������"));
			break;
		}
		case (int)Command::GetVertexData:
		{
			TRY_GET_VERTEX
				if (vertex->HasData())
					cout << vertex->GetData() << endl;
				else
					cout << "������ �� ������\n";
			break;
		}
		case (int)Command::GetEdgesCount:
			cout << graph->GetEdgesCount() << endl;
			break;
		case (int)Command::GetVertexCount:
			cout << graph->GetVertexCount() << endl;
			break;
		default:
			cout << "�������� ����� �������\n";
		};

		return SUCCESS_INPUT;
	}

protected:

	vector<commandView> commands = {
	{(int)Command::Create, "������� ����� L-����"},
	{(int)Command::CreateVDF, "������� ���� � ������ ���-�� ������, ���, �����"},
	{(int)Command::CreateVEDF, "������� ���� � ������ ���-�� ������ � ����. ����, ���, �����"},
	{(int)Command::IsDirected, "��������������� �� ����"},
	{(int)Command::GetForm, "�������� �����"},
	{(int)Command::SetForm, "������ �����"},
	{(int)Command::GetEdgesCount, "���������� ����"},
	{(int)Command::AddEdge, "�������� �����"},
	{(int)Command::GetEdge, "�������� �����"},
	{(int)Command::SetEdgeData, "������ ������ �����"},
	{(int)Command::GetEdgeData, "�������� ������ �����"},
	{(int)Command::SetEdgeWeight, "������ ��� �����"},
	{(int)Command::GetEdgeWeight, "�������� ��� �����"},
	{(int)Command::RemoveEdge, "������� �����"},
	{(int)Command::GetVertexCount, "���������� ������"},
	{(int)Command::AddVertex, "�������� �������"},
	{(int)Command::SetVertexData, "������ ������ �������"},
	{(int)Command::GetVertexData, "�������� ������ �������"},
	{(int)Command::RemoveVertex, "������� �������"},
	{(int)Command::Print, "������� ����"},
	{(int)Command::PrintCommands, "�������� ����� � ������� �������"},
	{(int)Command::ReturnToMenu, "����� �� �������� ����"} };

	void PrintCommands() { system("CLS"); printCommands(commands); }
};

class IteratorsCommands : public CommandsHandler
{
public:
	IteratorsCommands() { PrintCommands(); }

	enum class Command {
		VIteratorBegin, VIteratorValue, VIteratorInc, VIteratorEnd,
		EIteratorBegin, EIteratorValue, EIteratorInc, EIteratorEnd,
		OutEIteratorBegin, OutEIteratorValue, OutEIteratorInc, OutEIteratorEnd, 
		PrintCommands, ReturnToMenu
	};

	int HandleInput()
	{
		int input = Input<int>::Get("����� �������");

		switch (input)
		{
		case (int)Command::VIteratorBegin:
			vIterator = vIterator.Begin();
			break;
		case (int)Command::VIteratorValue:
		{
			try { (*vIterator).Print(); }
			catch (const char* ex) { cout << ex; }
			cout << endl;
			break;
		}
		case (int)Command::VIteratorInc:
			cout << "����� ������: " << vIterator++ << endl;
			break;
		case (int)Command::VIteratorEnd:
			vIterator = vIterator.End();
			break;
		case (int)Command::EIteratorBegin:
			eIterator = eIterator.Begin();
			break;
		case (int)Command::EIteratorValue:
		{
			try { (*eIterator).Print(); }
			catch (const char* ex) { cout << ex; }
			cout << endl;
			break;
		}
		case (int)Command::EIteratorInc:
			cout << "����� ������: " << eIterator++ << endl;
			break;
		case (int)Command::EIteratorEnd:
			eIterator = eIterator.End();
			break;
		case (int)Command::OutEIteratorBegin:
		{
			_Vertex* vertex;

			if (!GetVertex(&vertex, "��� �������"))
				break;
			
			outEIterator = RGZGraph::OutputEdgesIterator(*graph, vertex);
			break;
		}
		case (int)Command::OutEIteratorValue:
		{
			try { (*outEIterator).Print(); }
			catch (const char* ex) { cout << ex; }
			cout << endl;
			break;
		}
		case (int)Command::OutEIteratorInc:
			cout << "����� ������: " << outEIterator++ << endl;
			break;
		case (int)Command::OutEIteratorEnd:
			outEIterator = outEIterator.End();
			break;
		case (int)Command::PrintCommands:
			PrintCommands();
			break;
		case (int)Command::ReturnToMenu:
			return BACK_TO_MENU;
		default:
			cout << "�������� ����� �������\n";
		};

		return SUCCESS_INPUT;
	}

protected:

	vector<commandView> commands = {
	{(int)Command::VIteratorBegin, "�������� ������: ���������� � ������"},
	{(int)Command::VIteratorValue, "�������� ������: �������� ��������"},
	{(int)Command::VIteratorInc, "�������� ������: ��������� �������"},
	{(int)Command::EIteratorBegin, "�������� ����: ���������� � ������"},
	{(int)Command::EIteratorValue, "�������� ����: �������� ��������"},
	{(int)Command::EIteratorInc, "�������� ����: ��������� �����"},
	{(int)Command::OutEIteratorBegin, "�������� ���. ����: ���������� � ������"},
	{(int)Command::OutEIteratorValue, "�������� ���. ����: �������� ��������"},
	{(int)Command::OutEIteratorInc, "�������� ���. ����: ��������� �����"},
	{(int)Command::PrintCommands, "�������� ����� � ������� �������"},
	{(int)Command::ReturnToMenu, "����� �� �������� ����"} };

	void PrintCommands() { system("CLS"); printCommands(commands); }
};

template <class T>
class TaskCommands : public CommandsHandler
{
public:
	TaskCommands(T** task, string description): task(task), taskDescription(description) { PrintCommands(); }

	enum class Command {
		Create, Restart, Result, PrintCommands, ReturnToMenu
	};

	int HandleInput()
	{
		int input = Input<int>::Get("����� �������");

		switch (input)
		{
		case (int)Command::Create:
		{
			delete* task;
			*task = new T(*graph);
			break;
		}
		case (int)Command::Restart:
			if (*task == nullptr)
			{
				cout << "�������� ������ �������\n";
				break;
			}
			static_cast<Task<_Vertex, _Edge>*>(*task)->Restart();
			break;
		case (int)Command::Result:
			if (*task == nullptr)
			{
				cout << "�������� ������ �������\n";
				break;
			}
			static_cast<Task<_Vertex, _Edge>*>(*task)->Result();
			break;
		case (int)Command::PrintCommands:
			PrintCommands();
			break;
		case (int)Command::ReturnToMenu:
			return BACK_TO_MENU;
		default:
			cout << "�������� ����� �������\n";
		};

		return SUCCESS_INPUT;
	}

protected:

	vector<commandView> commands = {
	{(int)Command::Create, "������� ������, ����������� � ����� � ������"},
	{(int)Command::Restart, "������ ������� ������"},
	{(int)Command::Result, "������� ���������"},
	{(int)Command::PrintCommands, "�������� ����� � ������� �������"},
	{(int)Command::ReturnToMenu, "����� �� �������� ����"} };

	T** task;
	string taskDescription;

	void PrintCommands() { system("CLS"); cout << taskDescription << endl; printCommands(commands); }
};

class MainMenu
{
public:

	void HandleCurrentMenuInput()
	{
		if (handler != nullptr)
		{
			int code = handler->HandleInput();

			if (code == BACK_TO_MENU)
			{
				delete handler;
				handler = nullptr;
				system("CLS");
			}

			return;
		}

		cout << "1 - ����\n";
		cout << "2 - ������� ��� ����������\n";
		cout << "3 - ������� �2\n";
		cout << "4 - ������� �3\n";
		cout << "5 - �����\n";

		int input = Input<int>::Get("����� �������");

		switch (input)
		{
		case 1:
			handler = new GraphCommands();
			break;
		case 2:
			handler = new IteratorsCommands();
			break;
		case 3:
			handler = new TaskCommands<Task1v13<_Vertex, _Edge>>(&task1, task1Description);
			break;
		case 4:
			handler = new TaskCommands<Task2v14<_Vertex, _Edge>>(&task2, task2Description);
			break;
		case 5:
			exit(0);
			break;
		default:
			cout << "������������ ����� �������\n";
			break;
		}

	}

protected:

	CommandsHandler* handler = nullptr;
};

int main()
{
	setlocale(LC_ALL, "Russian");

	srand(time(NULL));

	MainMenu menu;

	while (true)
	{
		menu.HandleCurrentMenuInput();
	}
}