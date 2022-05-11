#pragma once
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <sstream>
#include <bitset>
#include <numeric>
#include "PrimeNumbers.h"

using std::cout;
using std::endl;

namespace Lab4
{

	#define WrongKeyEx "Wrong key" 
	#define IterOutOfRangeEx "Iterator out of range"

	using INT_64 = unsigned long long;

	enum class FormName { OpenAddressing, ChainsOfCollisions };

	template <class K, class V> // Key, Value
	class HashTable
	{
		class Cell;
		class Form;

	public:
		
		class Iterator
		{
		public:
			Iterator() {};
			Iterator(Form& form, int index, Cell* cell) : tableForm(&form), cellIndex(index), cell(cell) {};
			V& operator *() const
			{
				if (cell != nullptr)
					return cell->value;

				throw IterOutOfRangeEx;
			}
			bool operator++(int value) 
			{
				tableForm->GetNextCell(&cellIndex, &cell);  return cell != nullptr;
			}
			bool operator == (Iterator iter) const { return tableForm == iter.tableForm && cell == iter.cell; }
			bool operator != (Iterator iter) const { return !(*this == iter); }
		protected:
			int cellIndex;
			Cell* cell;
			Form* tableForm;

			friend class HashTable;
			friend class OpenAddressing;
			friend class ChainsOfCollisions;
		};
		
		HashTable(int elems) { if (elems <= 0) throw "Wrong value";  form = new OpenAddressing(elems); }
		~HashTable() { delete form; };
		int GetSize() const { return form->GetSize(); }
		float GetHashQuality() { return form->GetHashQuality(); };
		void Clear() { form->Clear(); };
		bool IsEmpty() const { return form->IsEmpty(); }
		V& operator[](K key) { return (*form)[key]; }
		bool Add(K key, V value) { return form->Add(key, value); }
		bool Remove(K key) { return form->Remove(key); }
		FormName GetForm() { return formName; }
		void SetForm(FormName newForm);
		int GetCapacity() const { return form->GetCapacity(); }
		int GetTrialsCount() const { return form->GetTrialsCount(); }
		void Print() const { form->Print(); }

		Iterator Begin() { return form->Begin(); }
		Iterator End() { return form->End(); }

	private:
		Form* form;
		FormName formName = FormName::OpenAddressing;

		friend class Iterator;

		class Cell
		{
		public:
			K key;
			V value;

			enum class State { Free, Deleted, Busy };
			State state = State::Free;

			Cell() {};
			Cell(K key, V value) : key(key), value(value), state(State::Busy) {};
			void Print() 
			{
				if (state == State::Free)
					cout << "Free";
				else if (state == State::Deleted)
					cout << "Deleted";
				else
					cout << "Busy [" << key << "]";
			};
		};

		class CellChain: public Cell
		{
		public:
			CellChain() {};
			CellChain(K key, V value) : Cell(key, value) {};
			CellChain* next = nullptr;
		};

		class Form
		{
		public:
			Form(int elems) {};
			Form(const Form& form);
			int GetSize() const { return size; }
			int GetCapacity() const { return capacity; }
			int GetTrialsCount() const { return trialsCount; }
			float GetHashQuality();
			bool IsEmpty() const { return size == 0; }
			Iterator Begin();
			Iterator End() { return Iterator(*this, -1, nullptr); }

			virtual ~Form() {};
			virtual void Clear() = 0;
			virtual void Print() const = 0;
			virtual bool Add(K key, V value) = 0;
			virtual bool Remove(K key) = 0;
			virtual V& operator[](K key) = 0;
		protected:
			int GetHash(float key);
			int ModuleHash(int digit) { return digit % capacity; };
			int GetHash2(INT_64 digit) { return 1 + (digit % (capacity - 1)); }
			int Hashing(INT_64 digit, int trialsCount);

			virtual void FindFirstBusyCell(int startIndex, int* index, Cell** cell) const = 0;
			virtual void GetNextCell(int* index, Cell** cell) const = 0;

			int size = 0;
			int capacity;
			int trialsCount = 0;

			friend class Iterator;
		};

		class OpenAddressing : public Form
		{
		public:
			OpenAddressing(int elems);
			OpenAddressing(const Form& form) : OpenAddressing(form.GetCapacity()) {};
			~OpenAddressing() { free(array); }

			virtual void Clear();
			virtual void Print() const;
			virtual bool Add(K key, V value);
			virtual bool Remove(K key);
			virtual V& operator[](K key);
		private:
			Cell* array;

			bool Find(K key, Cell** lastCell);
			virtual void FindFirstBusyCell(int startIndex, int* index, Cell** cell) const;
			virtual void GetNextCell(int* index, Cell** cell) const { FindFirstBusyCell(*index + 1, index, cell); }
		};

		class ChainsOfCollisions : public Form
		{
		public:
			ChainsOfCollisions(int elems);
			ChainsOfCollisions(const Form& form) : ChainsOfCollisions(form.GetCapacity()) {};
			~ChainsOfCollisions() { Clear(); free(array); }

			virtual void Clear();
			virtual void Print() const;
			virtual bool Add(K key, V value);
			virtual bool Remove(K key);
			virtual V& operator[](K key);
		private:
			CellChain* array;

			bool Find(K key, CellChain** lastCell, CellChain** lastCellPrev);
			virtual void GetNextCell(int* index, Cell** cell) const;
			virtual void FindFirstBusyCell(int startIndex, int* index, Cell** cell) const;
		};

	};

	template<class K, class V>
	inline void HashTable<K, V>::SetForm(FormName newForm)
	{
		if (this->formName == newForm)
			return;

		Form* _form = nullptr;
		int size = std::max(form->GetCapacity(), form->GetSize());

		if (newForm == FormName::OpenAddressing)
		{
			_form = new OpenAddressing(size);
		}
		else if (newForm == FormName::ChainsOfCollisions)
		{
			_form = new ChainsOfCollisions(size);
		}

		for (Iterator iter = this->Begin(); iter != this->End(); iter++) //добавляем все пары ключ-значение из одной формы в другую
			_form->Add(iter.cell->key, iter.cell->value);

		delete form;
		form = _form;
		formName = newForm;
	}

	template<class K, class V>
	inline float HashTable<K, V>::Form::GetHashQuality()
	{
		if (IsEmpty())
			return 0;

		int* rows = (int*)calloc(capacity, sizeof(int));
		float pm = size * 1.0 / capacity;
		float sum = 0;
		
		for (Iterator iter = this->Begin(); iter != this->End(); iter++)
			rows[iter.cellIndex]++;

		for (int i = 0; i < capacity; i++)
			sum += pow(rows[i] - pm, 2);

		free(rows);
		return sum * capacity / size;
	}

	template<class K, class V>
	inline int HashTable<K, V>::Form::GetHash(float key)
	{
		//float 10000.0000 - 15000.0000

		int digit = (int)(key * 10000); //преобразование к натуральному значению с точностью 10^-4

		//100 000 000 <= digit <= 150 000 000

		//свёртка к значению [10000,20000]

		std::vector<int> values;

		while (digit > 0)
		{
			values.insert(values.begin(), digit % 1000);
			digit /= 1000;
		}

		int sum = 0;

		for (int i = 0; i < values.size(); i++)
			sum += (values[i] xor (i + 1));

		sum %= 2501;

		// 0 <= sum <= 2500

		int result = 10000 + sum * 4;

		 //10000 <= result <= 20000

		return ModuleHash(result);
	}

	template<class K, class V>
	inline int HashTable<K, V>::Form::Hashing(INT_64 digit, int i)
	{ //квадратичное хэширование
		float c1 = 1;
		float c2 = 2;
		INT_64 d = (INT_64)ModuleHash(digit) + c1 * i + c2 * i * i;
		return d % this->capacity; //модульное хэширование
	}

	template<class K, class V>
	inline typename HashTable<K,V>::Iterator HashTable<K, V>::Form::Begin()
	{
		if (size == 0)
			return End();

		int index;
		Cell* cell;
		FindFirstBusyCell(0, &index, &cell);
		return Iterator(*this, index, cell);
	}

	template<class K, class V>
	inline HashTable<K, V>::OpenAddressing::OpenAddressing(int elems): Form(elems)
	{
		PrimeNumbers numbers;
		Form::capacity = numbers.FindFirstGE(elems);
		array = (Cell*)calloc(Form::capacity, sizeof(Cell));
	}

	template<class K, class V>
	inline void HashTable<K, V>::OpenAddressing::Print() const
	{
		for (int i = 0; i < this->capacity; i++)
		{
			cout << i << ": ";
			array[i].Print();
			cout << endl;
		}
		cout << endl;
	}

	template<class K, class V>
	inline bool HashTable<K, V>::OpenAddressing::Add(K key, V value)
	{
		if (Form::size == Form::capacity)
			return false;

		Cell* lastCell;

		if (Find(key, &lastCell) == false)
		{
			*lastCell = Cell(key, value);
			Form::size++;
			return true;
		}

		return false;
	}

	template<class K, class V>
	inline bool HashTable<K, V>::OpenAddressing::Remove(K key)
	{
		Cell* cell;
		if (Find(key, &cell) == false)
			return false;

		cell->state = Cell::State::Deleted;
		Form::size--;
		return true;
	}

	template<class K, class V>
	inline void HashTable<K, V>::OpenAddressing::Clear()
	{
		Iterator iter = Form::Begin();

		while (iter != Form::End())
		{
			iter.cell->state = Cell::State::Free;
			iter++;
		}

		Form::size = 0;
	}

	template<class K, class V>
	inline V& HashTable<K, V>::OpenAddressing::operator[](K key)
	{
		Cell* cell;
		if (Find(key, &cell) == false)
			throw WrongKeyEx;
		return cell->value;
	}

	//метод ищет клетку с данным ключом, последнюю посещённую клетку сохраняет через указатель lastCell
	template<class K, class V>
	inline bool HashTable<K, V>::OpenAddressing::Find(K key, Cell** lastCell)
	{ 
		Form::trialsCount = 1;
		int index = Form::GetHash(key);

		Cell* cellDeleted = nullptr; //если посетили клетку с состоянием Deleted (значение удалено)

		*lastCell = &array[index];
		while (array[index].state != Cell::State::Free) //пока не дойдём до свободной клетки (т.е. искомый ключ не найден)
		{
			if (Form::trialsCount == Form::capacity)
				break;

			if (array[index].state == Cell::State::Deleted)
				cellDeleted = &array[index];
			else if (array[index].state == Cell::State::Busy && array[index].key == key) //клетка найдена
				return true;

			index = Form::Hashing(index, Form::trialsCount++);
			*lastCell = &array[index];
		}

		if (cellDeleted != nullptr)
			*lastCell = cellDeleted;
		return false;
	}

	//поиск первой незанятой клетки начиная с индекса startIndex (метод используется итератором)
	template<class K, class V>
	inline void HashTable<K, V>::OpenAddressing::FindFirstBusyCell(int startIndex, int* index, Cell** cell) const
	{
		for (int i = startIndex; i < this->capacity; i++)
		{
			if (array[i].state == Cell::State::Busy)
			{
				*index = i;
				*cell = &array[i];
				return;
			}
		}

		*index = -1;
		*cell = nullptr;
	}

	template<class K, class V>
	inline HashTable<K, V>::ChainsOfCollisions::ChainsOfCollisions(int elems): Form(elems)
	{
		PrimeNumbers numbers;
		Form::capacity = numbers.FindFirstGE(elems);
		array = (CellChain*)calloc(Form::capacity, sizeof(CellChain));
	}

	template<class K, class V>
	inline void HashTable<K, V>::ChainsOfCollisions::Print() const
	{
		for (int i = 0; i < this->capacity; i++)
		{
			cout << i << ": ";
			array[i].Print();
			
			CellChain* cell = &array[i];

			while (cell->next != nullptr)
			{
				cell = cell->next;
				cout << " [" << cell->key << "]";
			}

			cout << endl;
		}
		cout << endl;
	}

	template<class K, class V>
	inline bool HashTable<K, V>::ChainsOfCollisions::Add(K key, V value)
	{
		CellChain* cellPrev;
		CellChain* cell;

		if (Find(key, &cell, &cellPrev))
			return false;

		if (cellPrev == nullptr && cell->state == Cell::State::Free) //если клетка - первая в цепочке коллизии
			*cell = CellChain(key, value);
		else
			cell->next = new CellChain(key, value); //иначе - вставляем в конец цепочки

		Form::size++;
		return true;
	}

	template<class K, class V>
	inline bool HashTable<K, V>::ChainsOfCollisions::Remove(K key)
	{
		CellChain* cellToRemove;
		CellChain* prev;

		if (Find(key, &cellToRemove, &prev) == false)
			return false;

		CellChain* next = cellToRemove->next;

		removing:

		if (prev == nullptr && next == nullptr) //первая и единственная в цепочке коллизий
		{
			cellToRemove->state = Cell::State::Deleted;
		}
		else if (prev == nullptr) //первая, но не единственная в цепочке коллизий
		{
			cellToRemove->key = next->key;
			cellToRemove->value = next->value;

			prev = cellToRemove; //теперь удаляемая ячейка - следующая
			cellToRemove = next;
			next = cellToRemove->next;

			goto removing;
		}
		else if (next == nullptr) //последняя, но не единственная в цепочке коллизий
		{
			prev->next = nullptr;
			delete cellToRemove;
		}
		else //не первая, не последняя, не единственная в цепочке коллизий
		{
			prev->next = next;
			delete cellToRemove;
		}
		
		Form::size--;
		return true;
	}

	template<class K, class V>
	inline void HashTable<K, V>::ChainsOfCollisions::Clear()
	{
		Iterator iter = Form::Begin();

		int prevIndex = -1;
		while (iter != Form::End())
		{
			if (prevIndex == iter.cellIndex) //проход по цепочке коллизии
			{
				CellChain* cell = static_cast<CellChain*>(iter.cell);
				iter++;
				delete cell;
			}
			else
			{
				prevIndex = iter.cellIndex;
				iter.cell->state = Cell::State::Free;
				iter++;
				array[prevIndex].next = nullptr;
			}
		}

		Form::size = 0;
	}

	template<class K, class V>
	inline V& HashTable<K, V>::ChainsOfCollisions::operator[](K key)
	{
		CellChain* cell;
		CellChain* prev;
		if (Find(key, &cell, &prev) == false)
			throw WrongKeyEx;
		return cell->value;
	}

	template<class K, class V>
	inline bool HashTable<K, V>::ChainsOfCollisions::Find(K key, CellChain** lastCell, CellChain** lastCellPrev)
	{
		Form::trialsCount = 1;
		int index = Form::GetHash(key);

		*lastCellPrev = nullptr;
		*lastCell = &array[index];

		if ((*lastCell)->key == key)
			return true;

		while ((*lastCell)->next != nullptr) //проход по цепочке коллизии
		{
			Form::trialsCount++;
			*lastCellPrev = *lastCell;
			*lastCell = (*lastCell)->next;

			if ((*lastCell)->key == key)
				return true;
		}

		return false;
	}

	//поиск следующей незанятой клетки (метод используется итератором)
	template<class K, class V>
	inline void HashTable<K, V>::ChainsOfCollisions::GetNextCell(int* index, Cell** cell) const
	{
		if (*cell == nullptr)
			return;

		*cell = static_cast<CellChain*>(*cell)->next;
		if (*cell != nullptr)
			return;

		FindFirstBusyCell(*index + 1, index, cell);
	}

	//поиск первой незанятой клетки начиная с индекса startIndex (метод используется итератором)
	template<class K, class V>
	inline void HashTable<K, V>::ChainsOfCollisions::FindFirstBusyCell(int startIndex, int* index, Cell** cell) const
	{
		for (int i = startIndex; i < this->capacity; i++)
		{
			if (array[i].state == Cell::State::Busy)
			{
				*index = i;
				*cell = &array[i];
				return;
			}
		}

		*index = -1;
		*cell = nullptr;
	}

}
