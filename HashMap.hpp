#ifndef EX3_CPP_HASHMAP_HPP
#define EX3_CPP_HASHMAP_HPP

static const int INITCAPACITY = 16;
static const double DECREASE = 0.5;
static const double INCREASE = 2;

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>

using std::vector;
using std::pair;

/**
 * A class representing a hashmap.
 * @tparam KeyT
 * @tparam ValueT
 */
template<class KeyT , class ValueT>
class HashMap
{
	using bucket = std::vector<std::pair<KeyT , ValueT>>;
private:
	/**
	 * The STL to contain the map in.
	 */
	bucket *_myMap = new bucket[INITCAPACITY];
	/**
	 * The upper factor.
	 */
	double _topFactor = 0.75;
	/**
	 * The lower factor.
	 */
	double _botFactor = 0.25;
	/**
	 * The amount of items in the hashmap.
	 */
	int _size = 0;
	/**
	 * The amount of items the map can hold.
	 */
	int _capacity = INITCAPACITY;

	/**
	 * nested class iterator.
	 */
	class const_iterator
	{
	public:
		/**
		 * A ctr.
		 * @param map the items of the hash map to go over.
		 * @param isEnd was the ctr called from begin or end. 1 for end 0 for begin.
		 * @param capacity the size of the pointer map.(length of list).
		 */
		const_iterator (bucket *map , int isEnd , int capacity) : _map (map) , _cap (capacity)
		{
			int i = 0;
			while (i < _cap)
			{
				if (map[i].empty ())
				{
					++i;
				}
				else
				{
					current = map[i].begin ();
					currentidx = i;
					break;
				}
			}
			i = capacity - 1;
			while (i > -1)
			{
				if (map[i].empty ())
				{
					i--;
				}
				else
				{
					ending = map[i].end ();
					if (isEnd)
					{
						current = ending;
					}
					break;
				}
			}
		}

		/**
		 * A default copy ctr.
		 * @param other
		 */
		const_iterator (const const_iterator &other) = default;

		pair<KeyT , ValueT> &operator* () const
		{
			return *current;
		}


		/**
		 * operator overload.
		 * @return
		 */
		pair<KeyT , ValueT> *operator-> () const
		{
			return &(*current);
		}

		/**
		 * operator overload.
		 * @return
		 */
		const_iterator &operator++ ()
		{
			if (current == ending)
			{
				return *this;
			}

			++current;

			if (current == ending)
			{
				return *this;
			}

			if (current == _map[currentidx].end ())
			{
				++currentidx;
				current = _map[currentidx].begin ();
				while (currentidx < _cap)
				{
					if (_map[currentidx].empty ())
					{
						++currentidx;
					}
					else
					{
						current = _map[currentidx].begin ();
						break;
					}
				}
			}
			return *this;
		}

		/**
		 * operator overload.
		 * @return
		 */
		const const_iterator operator++ (int)
		{
			const_iterator tmp = *this;
			++(*this);
			return tmp;
		}

		/**
		 * operator overload.
		 * @return
		 */
		bool operator== (const const_iterator  &rhs) const
		{
			return current == rhs.current && _map == rhs._map;
		}

		/**
		 * operator overload.
		 * @return
		 */
		bool operator!= (const const_iterator  &rhs) const
		{
			return !(*this == rhs);
		}

	private:
		/**
		 * The items of the hashmap to go over.
		 */
		bucket *_map;
		/**
		 * The capacity of *map.
		 */
		int _cap;
		/**
		 * The current iterator in the current vector we are at, and the ending iterator.
		 */
		typename bucket::iterator current , ending;
		/**
		 * The current vector we are at in map.
		 */
		int currentidx;
	};

	/**
	 * returns the hashing for a given key.
	 * @param key
	 * @return
	 */
	int _hashfunc (KeyT key) const
	{
		int ind = std::hash<KeyT>{} (key);
		return ind & (_capacity - 1);
	}

	/**
	 * A function to resize the array *map.
	 * @param factor
	 */
	void _resize (double factor)
	{
		auto oldmap = _myMap;

		HashMap::const_iterator b = begin () , e = end ();
		_myMap = new bucket[(int) (_capacity * factor)];
		_capacity = static_cast<int>(_capacity * factor);
		_size = 0;
		for (auto c = b; c != e; ++c)
		{
			insert (c->first , c->second);
		}

		delete[] oldmap;
	}

	/**
	 * A function that returns a reference to the value given a key. throws an exception if not
	 * found.
	 * @param key
	 * @return
	 */
	ValueT& _getVal (KeyT &key)
	{
		pair<KeyT , ValueT> &p = _getPair (key);
		return p.second;
	}
	/**
	 * A function that returns the value  given a key. throws an exception if not
	 * found.
	 * @param key
	 * @return
	 */
	ValueT _getVal (KeyT &key)const
	{
		pair<KeyT , ValueT> p = _getPair (key);
		return p.second;
	}

	/**
	 * A function that returns a referrence to the pair given a key. throws an exception if not
	 * found.
	 * @param key
	 * @return
	 */
	pair<KeyT , ValueT>& _getPair (KeyT key)
	{
		if (_myMap != nullptr)
		{
			int ind = _hashfunc (key);
			for (int i = 0; i < (int)_myMap[ind].size (); ++i)
			{
				if (_myMap[ind][i].first == key)
				{
					return _myMap[ind][i];
				}
			}
		}
		throw std::invalid_argument ("Bad input");
		// check this syntax.
	}

	/**
	 * A function that returns a  the pair given a key. throws an exception if not
	 * found.
	 * @param key
	 * @return
	 */
	pair<KeyT , ValueT> _getPair (KeyT key)const
	{
		if (_myMap != nullptr)
		{
			int ind = _hashfunc (key);
			for (int i = 0; i < (int)_myMap[ind].size (); ++i)
			{
				if (_myMap[ind][i].first == key)
				{
					return _myMap[ind][i];
				}
			}
		}
		throw std::invalid_argument ("bad input");
	}

	/**
	 * A function that inserts a key and a value , if flag is 1 can overwrite existing key.
	 * @param key
	 * @param val
	 * @param flag
	 * @return
	 */
	bool _insertOverWrite (KeyT key , ValueT val , int flag = 0)
	{
		if (containsKey (key))
		{
			if (flag)
			{
				auto &p = _getPair (key);
				p.second = val;
				return true;
			}
			return false;
		}
		++_size;
		if (getLoadFactor () > _topFactor)
		{
			_resize (INCREASE);
			++_size;
			// because i assigned size = 0 ;
		}
		int ind = _hashfunc (key);
		_myMap[ind].push_back (std::make_pair (key , val));
		return true;
	}

public:
	//	class const_iterator;
	typedef HashMap<KeyT , ValueT>::const_iterator const_iterator;

	/**
	 * A default ctr.
	 */
	HashMap () = default;

	/**
	 * A ctr accepting lower and upper factor.
	 * @param bot
	 * @param top
	 */
	HashMap (double bot , double top)
	{
		if (top >= 1 || top <= 0 || bot >= 1 || bot <= 0 || bot >= top )
		{
			throw std::invalid_argument ("bad input");
		}
		else
		{
			_topFactor = top;
			_botFactor = bot;
		}
	}

	/**
	 * A ctr that accepts two vectors, and generates from them a hashmap.
	 * @param keyVec
	 * @param valVec
	 */
	HashMap (vector<KeyT> keyVec , vector<ValueT> valVec)
	{
		if ((int)valVec.size () != (int)keyVec.size ())
		{
			throw std::invalid_argument ("bad input");
		}
		for (int i = 0; i < (int)keyVec.size (); ++i)
		{
			_insertOverWrite (keyVec[i] , valVec[i] , 1);
		}
	}

	/**
	 * A copy ctr.
	 * @param other
	 */
	HashMap (const HashMap &other) : _myMap (new bucket[other.capacity ()]) , _topFactor (other
	._topFactor) , _botFactor (other._botFactor) , _capacity (other._capacity)
	{
		for (auto i = other.begin (); i != other.end (); ++i)
		{
			insert (i->first , i->second);
		}
	}

	/**
	 * A move ctr.
	 * @param other
	 */
	HashMap (HashMap && other) : _myMap (other._myMap) , _topFactor (other._topFactor) ,
								_botFactor (other._botFactor) , _capacity (other._capacity) ,
								_size (other._size)
	{
		other._myMap = nullptr;
	}

	/**
	 * A destructor.
	 */
	~HashMap ()
	{
		delete[] _myMap;
	}

	/**
	 * A function that returns the amount of elements in the hashmap.
	 * @return
	 */
	int size () const
	{
		return _size;
	}

	/**
	 * A function that returns the amount of elements the map can hold.
	 * @return
	 */
	int capacity () const
	{
		return _capacity;
	}

	/**
	 * A function that returns the current load factor of the map.
	 * @return
	 */
	double getLoadFactor () const
	{
		return _size / (double) _capacity;
	}

	/**
	 * A function that returns 1 if the map is empty.
	 * @return
	 */
	bool empty ()
	{
		return _size == 0;
	}

	/**
	 * A function that inserts a key and a value.
	 * @param key
	 * @param val
	 * @return true on success else false.
	 */
	bool insert (KeyT key , ValueT val)
	{
		return _insertOverWrite (key , val);
	}

	/**
	 * A function that returns a ref to a value given a key, if not found throws an exception.
	 * @param key
	 * @return
	 */
	ValueT &at (KeyT key)
	{
		return _getVal (key);
	}

	/**
	 * A function that erases a pair given the pair's key.
	 * @param key
	 * @return
	 */
	bool erase (KeyT key)
	{
		try
		{
			auto p = _getPair (key);
			int ind = _hashfunc (key);
			for (int i = 0; i < (int)_myMap[ind].size (); ++i)
			{
				if (_myMap[ind][i] == p)
				{
					_myMap[ind].erase (_myMap[ind].begin () + i);
				}
			}
			--_size;
			if (getLoadFactor () < _botFactor)
			{
				_resize (DECREASE);
			}
			return true;
		}
		catch (std::invalid_argument &a)
		{
			return false;
		}
	}

	/**
	 * A function that clears the map.
	 */
	void clear ()
	{
		delete[] _myMap;
		_size = 0;
		_myMap = new bucket[_capacity];
	}

	/**
	 * returns 1 if there exists a pair with that key.
	 * @param key
	 * @return
	 */
	bool containsKey (KeyT key)const
	{
		try
		{
			_getVal (key);
			return true;
		}
		catch (std::invalid_argument &a)
		{
			return false;
		}
	}

	/**
	 * overload ctr.
	 * @param other
	 * @return
	 */
	HashMap &operator= (const HashMap &other)
	{
		if (this == &other)
		{
			return *this;
		}
		_topFactor = other._topFactor;
		_botFactor = other._botFactor;
		delete[] _myMap;
		_myMap = new bucket[other._capacity];
		_capacity = other._capacity;
		for (int i = 0; i < other._capacity; ++i)
		{
			for (auto it = other._myMap[i].begin (); it != other._myMap[i].end (); ++it)
			{
				insert (it->first , it->second);
			}
		}
		return *this;
	}
	/**
	 * overload ctr.
	 * @param other
	 * @return
	 */
	HashMap& operator= ( HashMap && other)noexcept
	{
		_topFactor = other._topFactor;
		_botFactor = other._botFactor;
		delete[] _myMap;
		_myMap = new bucket[other._capacity];
		_capacity = other._capacity;
		for (int i = 0; i < other._capacity; ++i)
		{
			for (auto it = other._myMap[i].begin (); it != other._myMap[i].end (); ++it)
			{
				insert (it->first , it->second);
			}
		}
		other._myMap = nullptr;
		return *this;
	}

	/**
	 * overload ctr.
	 * @param key
	 * @return
	 */
	ValueT &operator[] (KeyT key)
	{
		try
		{
			return _getVal (key);
		}
		catch (std::invalid_argument & a)
		{
			ValueT t;
			// is alright because it exists in function
			_insertOverWrite (key , t , 1);
			return _getVal (key);
		}
	}

	/**
	 * overload ctr.
	 * @param key
	 * @return
	 */
	ValueT operator[] (KeyT key) const
	{
		int ind = _hashfunc (key);
		for (int i = 0; i < (int)_myMap[ind].size (); ++i)
		{
			if (_myMap[ind][i].first == key)
			{
				return _myMap[ind][i].second;
			}
		}
		return ValueT ();
	}

	/**
	 * overload ctr.
	 * @param key
	 * @return
	 */
	bool operator== (const HashMap &other) const
	{
		int count = 0;
		if (_size != other._size || _capacity != other._capacity || _topFactor != other._topFactor
			|| _botFactor != other._botFactor)
		{
			return false;
		}
		for (auto i = other.begin (); i != other.end (); ++i)
		{
			try
			{
				if (!containsKey (i->first) || i->second != _getVal (i->first))
				{
					return false;
				}
			}
			catch (std::invalid_argument &a)
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * overload ctr.
	 */
	bool operator!= (const HashMap &other)
	{
		return !(*this == other);
	}

	/**
	 * A function that returns the amount of items in the same bucket with a given key.
	 * @param key
	 * @return
	 */
	int bucketSize (KeyT key)
	{
		if (containsKey (key))
		{
			int ind = _hashfunc (key);
			return (int) (_myMap[ind].size ());
		}
		throw std::invalid_argument ("bad input");
	}

	/**
	 * A begin func.
	 * @return
	 */
	const_iterator begin () const noexcept
	{
		return const_iterator (this->_myMap , 0 , this->capacity ());
	}

	/**
	 * A end func.
	 * @return
	 */
	const_iterator end () const noexcept
	{
		return const_iterator (this->_myMap , 1 , this->capacity ());
	}

	/**
	 * A cbegin func.
	 * @return
	 */
	const_iterator cbegin () const noexcept
	{
		return const_iterator (this->_myMap , 0 , this->capacity ());
	}

	/**
	 * A cend func.
	 * @return
	 */
	const_iterator cend () const noexcept
	{
		return const_iterator (this->_myMap , 1 , this->capacity ());
	}
};


#endif //EX3_CPP_HASHMAP_HPP
