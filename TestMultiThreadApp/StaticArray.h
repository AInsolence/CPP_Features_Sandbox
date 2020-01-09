#pragma once
#include <iostream>

using array_size_t = int;

template<class T, const array_size_t size>
class StaticArray
{
public:

	StaticArray();
	~StaticArray();

	int count() const;
	void push(T&&);
	T pop();

private:
	T* _objectArray;
	const array_size_t _size = size;
	int _numberOfElements = 0;
};

template<class T, array_size_t size>
inline StaticArray<T, size>::StaticArray()
{
	_objectArray = new T[_size];
}

template<class T, array_size_t size>
inline StaticArray<T, size>::~StaticArray()
{
	delete[] _objectArray;
}

template<class T, array_size_t size>
inline int StaticArray<T, size>::count() const
{
	return _numberOfElements;
}

template<class T, array_size_t size>
inline void StaticArray<T, size>::push(T&& newElement)
{
	if (newElement) {
		if (_numberOfElements < _size)
		{
			_objectArray[_numberOfElements] = newElement;
			_numberOfElements++;

			std::cout << newElement << " " << "New element is pushed" << std::endl;
		}
		else
		{
			std::cout << "Array is full! Try to pop out before push()" << std::endl;
		}
	}
}

template<class T, array_size_t size>
inline T StaticArray<T, size>::pop()
{
	if (_numberOfElements != 0)
	{
		std::cout << "Last element is popped out" << std::endl;
		_numberOfElements--;
		return _objectArray[_numberOfElements];
	}
	else
	{
		std::cout << "Array is empty! Try to push element before pop()" << std::endl;
		return double();
	}
}

/// Explicit version
template<>
class StaticArray<double, 10>
{
public:

	StaticArray();
	~StaticArray();

	int count() const;
	void push(double&&);
	double pop();

private:
	double* _objectArray;
	const array_size_t _size = 10;
	int _numberOfElements = 0;
};


inline StaticArray<double, 10>::StaticArray()
{
	_objectArray = new double[_size];
	std::cout << "This is an Explicit version of Constructor!";
	std::cout << std::endl;
}

inline StaticArray<double, 10>::~StaticArray()
{
	delete[] _objectArray;
	std::cout << "This is an Explicit version of Destructor!";
	std::cout << std::endl;
}

inline int StaticArray<double, 10>::count() const
{
	return _numberOfElements;
}

inline void StaticArray<double, 10>::push(double&&)
{
	std::cout << "This is an Explicit version of push!";
	std::cout << std::endl;
}

inline double StaticArray<double, 10>::pop()
{
	std::cout << "This is an Explicit version of pop!";
	std::cout << std::endl;
	return 0.0;
}