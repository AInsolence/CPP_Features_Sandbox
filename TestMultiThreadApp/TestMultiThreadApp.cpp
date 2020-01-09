// TestMultiThreadApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include "StaticArray.h"
// Example of creating preProcessor directives
#define tab "\t"

//extern template class StaticArray<double, 10>;

class MyClass
{
public:
	MyClass(std::string name);
	~MyClass();

	int sumNumbers(int firstNum, int secondNum);
	std::string getClassName();

	void print_1();
	void print_2();
	void printRecursively(int);
	void printUnique();

private:
	std::string _className;
	std::mutex mutex_1;
	std::mutex mutex_2;
	std::recursive_mutex rMutex_1;
};

MyClass::MyClass(std::string name)
{
	_className = name;
}

MyClass::~MyClass()
{
}

int MyClass::sumNumbers(int firstNum, int secondNum)
{
	std::cout << "MyClass sumNumbers method started.";
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	std::cout << "MyClass sumNumbers method stopped.";
	return firstNum + secondNum;
}

std::string MyClass::getClassName()
{
	std::cout << "MyClass getClassName method started.";
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	std::cout << "MyClass getClassName method stopped.";
	return _className;
}

void MyClass::print_1()
{
	std::lock_guard<std::mutex> guard(mutex_1);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	{// namespace to control lock_guard lifetime
		std::lock_guard<std::mutex> guard(mutex_2);
		for (size_t i = 0; i < 5; i++)
		{
			for (size_t j = 0; j < 10; j++)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
				std::cout << "$";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void MyClass::print_2()
{
	std::lock_guard<std::mutex> guard(mutex_2);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	{// namespace to control lock_guard lifetime
		std::lock_guard<std::mutex> guard(mutex_1);
		for (size_t i = 0; i < 5; i++)
		{
			for (size_t j = 0; j < 10; j++)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
				std::cout << "#";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void MyClass::printRecursively(int num)
{
	std::lock_guard<std::recursive_mutex> guard(rMutex_1);
	//rMutex_1.lock();
	if (num <= 0) {
		std::cout << std::endl;
		//rMutex_1.unlock();
		return;
	}
	else {
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		std::cout << num << " ";
		--num;
		printRecursively(num);
		//rMutex_1.unlock();
	}
}

void MyClass::printUnique()
{
	std::unique_lock<std::recursive_mutex> uLock(rMutex_1, std::defer_lock);
	std::unique_lock<std::recursive_mutex> uLock_1(rMutex_1, std::defer_lock);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	{// namespace to control lock_guard lifetime
		uLock.lock();
		for (size_t i = 0; i < 5; i++)
		{
			for (size_t j = 0; j < 10; j++)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
				std::cout << "#";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		uLock.unlock();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void doWork() {

	for (size_t i = 0; i < 10; i++)
	{
		std::cout << "Thread ID:" << std::this_thread::get_id() << " DoWork method " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(700));
	}
}

void mathWork(int firstNum, int secondNum) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		std::cout << "Thread ID:" << std::this_thread::get_id() << " !!!!!!!!!!! mathWork STARTED! " << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "Result a + b = " << firstNum + secondNum << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "Thread ID:" << std::this_thread::get_id() << " !!!!!!!!!!! mathWork STOPPED! " << std::endl;
}

void changeRefExample(int& num) {
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	num *= 2;
}

int sumNumbers(int firstNum, int secondNum) {
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	return firstNum + secondNum;
}

int main()
{
	StaticArray<double, 1> MyArrayObject;
	MyArrayObject.push(10);
	MyArrayObject.push(10);
	std::cout << MyArrayObject.pop() << std::endl;
	MyArrayObject.pop();

	{// explicit template instantiation implementation example
		StaticArray<double, 10> ExplicitArray;
		ExplicitArray.push(10);
		ExplicitArray.pop(); 
		ExplicitArray.count();
	}

	std::thread thread_1(doWork);
	//thread_1.detach(); // DO NOT Wait completion of this thread
	std::thread thread_2(doWork);

	std::thread thread_3(mathWork, 10, 15);// pass parameters to thread

	int number = 5;
	std::thread thread_4(changeRefExample, std::ref(number));// use std::ref() to pass parameter by ref

	thread_4.join();// Put join before using result from this thread !!!
	std::cout << "NUmber after changes = " << number << std::endl;

	// Example with lambda function wrapper
	int result;
	std::thread thread_5([&result]() { result = sumNumbers(5, 10);} );

	// Example with class methods inside thread

	MyClass myClassInstance("Anton");
	int classSumResult;
	std::string classGetNameResult;

	std::thread thread_6([&myClassInstance, &classSumResult]() {
		classSumResult = myClassInstance.sumNumbers(2, 2); });

	std::thread thread_7([&myClassInstance, &classGetNameResult]() {
		classGetNameResult = myClassInstance.getClassName(); });

	/// MAIN thread START
	for (size_t i = 0; i < 10; i++)
	{
		std::cout << "Thread ID:" << std::this_thread::get_id() << " MAIN method " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	/// END main thread

	thread_5.join();
	std::cout << "Sum function result = " << result << std::endl;

	thread_6.join();
	thread_7.join();
	std::cout << "Class sum method result = " << tab << result << std::endl;
	std::cout << "Class getName method result = " << tab << result << std::endl;

	/// !NOTE: we want to wait completion of thread_1 here, after MAIN code
	thread_1.join(); // lock main thread and wait thread_1 execution
	thread_2.join();
	thread_3.join();

	// Pointer to function with lambda example
	auto f = [=]() {
		return 1.0;
	};

	auto res = f();

	//Recursive mutex example
	std::thread thread_10(&MyClass::printRecursively, &myClassInstance, 4);
	std::thread thread_11(&MyClass::printRecursively, &myClassInstance, 5);

	// Unique lock example
	std::thread thread_12(&MyClass::printUnique, &myClassInstance);
	thread_12.join();

	// Deadlock & lock_guard example
	std::thread thread_8(&MyClass::print_1, &myClassInstance);// NOTE second argument is a pointer
	std::thread thread_9([&myClassInstance, &classGetNameResult]() {
		myClassInstance.print_2(); });

	thread_10.join();
	thread_11.join();
	thread_8.join();
	thread_9.join();

	return 0;
}






// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
