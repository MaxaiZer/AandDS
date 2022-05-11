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

string task1Description = "Определение диаметра связного неориентированного графа";

template <class Vertex, class Edge>
class Task1v4 : public Task<Vertex, Edge>
{
public:
	Task1v4(Graph<Vertex, Edge>& graph) : Task<Vertex, Edge>(graph)
	{
		if (graph.IsDirected())
			throw "Граф должен быть неориентированным";
		Restart();
	};
	virtual void Restart();
	virtual void Result();
protected:
	int result;
	vector<vector<int>> GetWeightMatrix();
	vector<Vertex*> GetAllVertexes();
	int GetMaxWeight(vector<vector<int>> weights, int vertexIndex);
	bool IsGraphConnected(vector<vector<int>> weights);
};

template<class Vertex, class Edge>
inline void Task1v4<Vertex, Edge>::Result()
{
	cout << result << endl;
}

template<class Vertex, class Edge>
inline void Task1v4<Vertex, Edge>::Restart()
{
	result = -1;

	int vertexes = Task<Vertex, Edge>::graph->GetVertexesCount();

	if (vertexes == 0)
		return;

	vector<vector<int>> weights = GetWeightMatrix();

	for (int i = 0; i < vertexes; i++)
	{
		for (int j = 0; j < vertexes; j++)
		{
			for (int s = 0; s < vertexes; s++)
			{
				int possibleMin = weights[j][i] + weights[i][s];
				if (weights[j][i] == numeric_limits<int>::max() || weights[i][s] == numeric_limits<int>::max())
					possibleMin = numeric_limits<int>::max();

				weights[j][s] = std::min(weights[j][s], possibleMin);
			}
		}
	}

	if (IsGraphConnected(weights) == false)
		throw "Граф не связный";

	vector<int> mins(vertexes);
	int diameter = -1;
	for (int i = 0; i < vertexes; i++)
	{
		mins[i] = GetMaxWeight(weights, i);
		diameter = std::max(diameter, mins[i]);
	}

	result = diameter;
}

template<class Vertex, class Edge>
inline vector<vector<int>> Task1v4<Vertex, Edge>::GetWeightMatrix()
{
	int vertexes = Task<Vertex, Edge>::graph->GetVertexesCount();

	vector<vector<int>> weights;

	for (int i = 0; i < vertexes; i++)
	{
		weights.push_back(vector<int>());
		for (int j = 0; j < vertexes; j++)
			weights[i].push_back(numeric_limits<int>::max());
	}

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
inline vector<Vertex*> Task1v4<Vertex, Edge>::GetAllVertexes()
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
inline int Task1v4<Vertex, Edge>::GetMaxWeight(vector<vector<int>> weights, int vertexIndex)
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

template<class Vertex, class Edge>
inline bool Task1v4<Vertex, Edge>::IsGraphConnected(vector<vector<int>> weights)
{
	bool isConnected = true;

	for (int i = 0; i < weights.size(); i++)
	{
		for (int j = 0; j < weights.size(); j++)
		{
			if (j == i) continue;

			if (weights[i][j] == numeric_limits<int>::max())
			{
				isConnected = false;
				break;
			}
		}
	}

	return isConnected;
}

#pragma endregion

#pragma region Task2

string task2Description = "Определение центра взвешенного орграфа на основе алгоритма Флойда";

template <class Vertex, class Edge>
class Task2v14 : public Task<Vertex, Edge>
{
public:
	Task2v14(Graph<Vertex, Edge>& graph) : Task<Vertex, Edge>(graph) 
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
	int GetMaxWeight(vector<vector<int>> weights, int vertexIndex);
};

template<class Vertex, class Edge>
inline void Task2v14<Vertex, Edge>::Result()
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
inline void Task2v14<Vertex, Edge>::Restart()
{
	result.clear();
	int vertexes = Task<Vertex, Edge>::graph->GetVertexesCount();

	if (vertexes == 0)
		return;

	vector<vector<int>> weights = GetWeightMatrix();

	for (int i = 0; i < vertexes; i++)
	{
		for (int j = 0; j < vertexes; j++)
		{
			for (int s = 0; s < vertexes; s++)
			{
				int possibleMin = weights[j][i] + weights[i][s];
				if (weights[j][i] == numeric_limits<int>::max() || weights[i][s] == numeric_limits<int>::max())
					possibleMin = numeric_limits<int>::max();

				weights[j][s] = std::min(weights[j][s], possibleMin);
			}
		}
	}

	vector<int> mins(vertexes);
	int min = numeric_limits<int>::max();
	for (int i = 0; i < vertexes; i++)
	{
		mins[i] = GetMaxWeight(weights, i);
		min = std::min(min, mins[i]);
	}

	if (min == numeric_limits<int>::max())
		return;

	vector<Vertex*> graphVertexes = GetAllVertexes();

	for (int i = 0; i < mins.size(); i++)
	{
		if (min == mins[i])
			result.push_back(graphVertexes[i]);
	}

}

template<class Vertex, class Edge>
inline vector<vector<int>> Task2v14<Vertex, Edge>::GetWeightMatrix()
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
inline vector<Vertex*> Task2v14<Vertex, Edge>::GetAllVertexes()
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
inline int Task2v14<Vertex, Edge>::GetMaxWeight(vector<vector<int>> weights, int vertexIndex)
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