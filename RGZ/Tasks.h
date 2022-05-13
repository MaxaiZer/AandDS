#pragma once
#include "Graph.h"
#include "Input.h"
#include <algorithm>

template <class Vertex, class Edge>
class Task
{
public:
	Task(Graph<Vertex, Edge>& graph): graph(&graph) { }
	void Set(Graph<Vertex, Edge>& graph) { this->graph = &graph; Restart(); }
	static string GetDescription() {};
	virtual ~Task() = default;
	virtual void Restart() = 0;
	virtual void Result() = 0;
protected:
	Graph<Vertex, Edge>* graph;
};

#pragma region Task1

string task1Description = "Поиск всех простых циклов, включающих заданную вершину орграфа";

template <class Vertex, class Edge>
class Task1v8 : public Task<Vertex, Edge>
{
public:
	Task1v8(Graph<Vertex, Edge>& graph) : Task<Vertex, Edge>(graph)
	{
		if (graph.IsDirected() == false)
			throw "Граф должен быть направленным";
		Restart();
	};
	virtual void Restart();
	virtual void Result();
protected:
	enum State { ProbablyInCycle, NotInCycle };

	vector<vector<Vertex*>> result;
	bool FindVertex(Vertex** vertex);
	void FindCycle(Vertex* v, vector<State>& colors, vector<Vertex*> path);
	void GetAllNeighboringVertexes(Vertex* vertex, vector<Vertex*>& neighbors);
};

template<class Vertex, class Edge>
inline void Task1v8<Vertex, Edge>::Result()
{
	if (result.size() == 0)
	{
		cout << "Циклы не найдены\n";
		return;
	}

	for (auto path : result)
	{
		for (auto elem : path)
			cout << elem->GetName() << " ";
		cout << endl;
	}
}

template<class Vertex, class Edge>
inline void Task1v8<Vertex, Edge>::Restart()
{
	result.clear();

	Vertex* v;
	if (!FindVertex(&v))
	{
		cout << "Вершина не найдена\n";
		return;
	}

	vector<Vertex*> path;
	path.push_back(v);
	vector <State> states(Task<Vertex,Edge>::graph->GetVertexesCount(), ProbablyInCycle);

	FindCycle(v, states, path);
}

template<class Vertex, class Edge>
inline bool Task1v8<Vertex, Edge>::FindVertex(Vertex** vertex)
{
	typename Graph<Vertex, Edge>::VertexesIterator iter(*Task<Vertex, Edge>::graph);
	string name = Input<string>::Get("Имя вершины");

	while (iter != iter.End())
	{
		if ((*iter).GetName() == name)
		{
			*vertex = &((*iter));
			return true;
		}
		iter++;
	}
	return false;
}

template<class Vertex, class Edge>
inline void Task1v8<Vertex, Edge>::FindCycle(Vertex* v, vector<State>& states, vector<Vertex*> curPath)
{
	vector<Vertex*> neighbors;
	GetAllNeighboringVertexes(v, neighbors);

	if (neighbors.size() == 0)
	{
		states[v->index] = NotInCycle;
	}

	for (int i = 0; i < neighbors.size(); i++)
	{
		Vertex* curVertex = neighbors[i];

		if (curVertex == curPath[0])
		{
			vector<Vertex*> successPath = curPath;
			successPath.push_back(curVertex);
			result.push_back(successPath);
		}

		if (std::find(curPath.begin(), curPath.end(), curVertex) != curPath.end())
			continue;

		if (states[neighbors[i]->index] == ProbablyInCycle)
		{
			curPath.push_back(neighbors[i]);
			FindCycle(neighbors[i], states, curPath);
		}
	}

}

template<class Vertex, class Edge>
inline void Task1v8<Vertex, Edge>::GetAllNeighboringVertexes(Vertex* vertex, vector<Vertex*>& neighbors)
{
	typename Graph<Vertex, Edge>::OutputEdgesIterator iter(*Task<Vertex, Edge>::graph, vertex);

	while (iter != iter.End())
	{
		neighbors.push_back((*iter).V2());
		iter++;
	}
}

#pragma endregion

#pragma region Task2

string task2Description = "Определение периферии взвешенного орграфа на основе алгоритма Флойда";

template <class Vertex, class Edge>
class Task2v9 : public Task<Vertex, Edge>
{
public:
	Task2v9(Graph<Vertex, Edge>& graph) : Task<Vertex, Edge>(graph)
	{
		if (graph.IsDirected() == false)
			throw "Граф должен быть направленным";
		Restart();
	};
	virtual void Restart();
	virtual void Result();
protected:
	vector<Vertex*> result;
	vector<vector<int>> GetWeightMatrix();
	vector<Vertex*> GetAllVertexes();
	void Floyd(vector<vector<int>>& weights);
	int GetMaxWeight(vector<vector<int>> weights, int vertexIndex);
};

template<class Vertex, class Edge>
inline void Task2v9<Vertex, Edge>::Result()
{
	if (result.empty())
		cout << "Нет таких вершин\n";
	for (auto v : result)
	{
		v->Print();
		cout << endl;
	}
}

template<class Vertex, class Edge>
inline void Task2v9<Vertex, Edge>::Restart()
{
	result.clear();
	int vertexes = Task<Vertex, Edge>::graph->GetVertexesCount();

	if (vertexes == 0)
		return;

	vector<vector<int>> weights = GetWeightMatrix();
	Floyd(weights);

	vector<int> mins(vertexes);
	int diameter = -1;
	for (int i = 0; i < vertexes; i++)
	{
		mins[i] = GetMaxWeight(weights, i);
		diameter = std::max(diameter, mins[i]);
	}

	vector<Vertex*> graphVertexes = GetAllVertexes();

	for (int i = 0; i < mins.size(); i++)
	{
		if (diameter == mins[i])
			result.push_back(graphVertexes[i]);
	}
}

template<class Vertex, class Edge>
inline vector<vector<int>> Task2v9<Vertex, Edge>::GetWeightMatrix()
{
	int vertexes = Task<Vertex, Edge>::graph->GetVertexesCount();

	vector<vector<int>> weights;

	for (int i = 0; i < vertexes; i++)
	{
		weights.push_back(vector<int>());
		for (int j = 0; j < vertexes; j++)
			weights[i].push_back(numeric_limits<int>::max());
	}

	result.clear();
	typename Graph<Vertex, Edge>::EdgesIterator iter(*Task<Vertex, Edge>::graph);

	while (iter != iter.End())
	{
		Vertex* v1 = (*iter).V1();
		Vertex* v2 = (*iter).V2();
		weights[v1->index][v2->index] = (*iter).GetWeight();

		if (Task<Vertex, Edge>::graph->IsDirected() == false)
			weights[v2->index][v1->index] = (*iter).GetWeight();

		iter++;
	}

	return weights;
}

template<class Vertex, class Edge>
inline vector<Vertex*> Task2v9<Vertex, Edge>::GetAllVertexes()
{
	vector<Vertex*> vertexes;
	typename Graph<Vertex, Edge>::VertexesIterator iter(*Task<Vertex, Edge>::graph);

	while (iter != iter.End())
	{
		vertexes.push_back(&(*iter));
		iter++;
	}

	return vertexes;
}

template<class Vertex, class Edge>
inline void Task2v9<Vertex, Edge>::Floyd(vector<vector<int>>& weights)
{
	for (int i = 0; i < weights.size(); i++)
	{
		for (int j = 0; j < weights.size(); j++)
		{
			for (int s = 0; s < weights.size(); s++)
			{
				int possibleMin = weights[j][i] + weights[i][s];
				if (weights[j][i] == numeric_limits<int>::max() || weights[i][s] == numeric_limits<int>::max())
					possibleMin = numeric_limits<int>::max();

				weights[j][s] = std::min(weights[j][s], possibleMin);
			}
		}
	}
}

template<class Vertex, class Edge>
inline int Task2v9<Vertex, Edge>::GetMaxWeight(vector<vector<int>> weights, int vertexIndex)
{
	int max = (vertexIndex == 0 ? -1 : weights[0][vertexIndex]);
	if (weights.size() == 1) max = weights[0][0];

	for (int i = 0; i < weights.size(); i++)
	{
		if (i == vertexIndex) continue;

		max = std::max(max, weights[i][vertexIndex]);
	}

	return max;
}

#pragma endregion