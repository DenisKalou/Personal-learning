// try_it.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <cstddef> // size_t
using namespace std;

struct Expression
{
	virtual double evaluate() const = 0;
	virtual ~Expression() {}
};

struct Number : Expression
{
	Number(double value)
		: value(value)
	{}
	double evaluate() const { return value; }
	~Number() {}
private:
	double value;
};

struct BinaryOperation : Expression
{
	/*
	Здесь op это один из 4 символов: '+', '-', '*' или '/', соответствующих операциям,
	которые вам нужно реализовать.
	*/
	BinaryOperation(Expression const * left, char op, Expression const * right)
		: left(left), op(op), right(right)
	{ }
	double evaluate() const {
		double value = 0;
		if (op == '+') {
			value = left->evaluate() + right->evaluate();
		}
		else if (op == '-') {
			value = left->evaluate() - right->evaluate();
		}
		else if (op == '*') {
			value = left->evaluate() * right->evaluate();
		}
		else if (op == '/') {
			value = left->evaluate() / right->evaluate();
		}
		return value;
	}
	~BinaryOperation() {
		delete left;
		delete right;
	}
private:
	Expression const * left;
	Expression const * right;
	char op;
};

struct SharedPtr
{
	// реализуйте следующие методы
	//
	explicit SharedPtr(Expression *ptr = 0)
		: ptr_(ptr), count(0) 
	{
		cout << "Construct(" << ptr_ << "): ";
		if (ptr_) {
			cout << " not";
			count = new size_t(1);
		}
		cout << " NULL" << endl;
	}
	~SharedPtr() {
		cout << "Destruct: ";
		if (count && (--*count == 0)) {
			cout << " ptr: "<< ptr_;
			delete ptr_;
			cout << " count: " << count << endl;
			delete count;
		}
	}
	SharedPtr(const SharedPtr& ptr)
		:ptr_(ptr.ptr_), count(ptr.count)
	{
		cout << "Copy(" << ptr_ << "): ";
		if (count) {
			++*count;
			cout << *count << endl;
		}
	}
	SharedPtr& operator=(SharedPtr& ptr) 
	{
		cout << "=: ";
		if (this != &ptr) {
			ptr_ = ptr.ptr_;
			count = ptr.count;
			++*count;
		}
		return *this;
	}
	Expression* get() const {
		return ptr_;
	}
	void reset(Expression *ptr = 0) {
		cout << "Reset: ";
		this->~SharedPtr();
		ptr_ = 0;
		count = 0;
		if (ptr) {
			count = new size_t(1);
			ptr_ = ptr;
		}
	}
	Expression& operator*() const {
		return *ptr_;
	}
	Expression* operator->() const {
		return ptr_;
	}

private:	
	size_t *count;
	Expression *ptr_;
};

int main() {
	Expression *a = new Number(10);
	SharedPtr p1(a);
	//SharedPtr p2(p1);
	SharedPtr p3 = p1;
	//p1.reset();
	//p3.reset(a);
	//cout << p1.get() << endl;
	return 0;
	if (_CrtDumpMemoryLeaks())
		std::cout << "memory leak is detected" << std::endl;
	else
		std::cout << "no memory leaks" << std::endl;
}
