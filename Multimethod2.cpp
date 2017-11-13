#include <iostream>
#include <functional>
#include <string>
#include <map>
#include <typeinfo>
#include <typeindex>
//using namespace std;

// базовый класс фигуры (полиморфный)
struct Shape;
struct Rectangle;
struct Triangle;

bool is_intersect(Rectangle * a, Rectangle * b) { std::cout << "R x R\n"; return true; }
bool is_intersect(Rectangle * a, Triangle  * b) { std::cout << "R x T\n"; return true; }
bool is_intersect(Triangle  * a, Triangle  * b) { std::cout << "T x T\n"; return true; }
struct Shape 
{
	virtual ~Shape() {}
	virtual bool intersect(Rectangle * r) = 0;
	virtual bool intersect(Triangle  * t) = 0;
	virtual bool intersect(Shape     * s) = 0;
};

// прямоугольник
struct Rectangle : Shape 
{
	bool intersect(Rectangle * r) { return is_intersect(this, r); }
	bool intersect(Triangle  * t) { return is_intersect(this, t); }
	bool intersect(Shape     * s) {
		return s->intersect(this);
	}
};

// треугольник
struct Triangle : Shape 
{
	bool intersect(Rectangle * r) { return is_intersect(r, this); }
	bool intersect(Triangle  * t) { return is_intersect(t, this); }
	bool intersect(Shape     * s) {
		return s->intersect(this);
	}
};
bool is_intersect(Shape * a, Shape * b)
{
	return a->intersect(b);
}
// функция для проверки пересечения двух прямоугольников
bool is_intersect_r_r(Shape * a, Shape * b) {
	if (Rectangle* ra = dynamic_cast<Rectangle*>(a)) {
		if (Rectangle* rb = dynamic_cast<Rectangle*>(b)) {
			return a->intersect(b);
		}
	}
	return false;
}

// функция для проверки пересечения прямоугольника и треугольника
bool is_intersect_r_t(Shape * a, Shape * b) { 
	if (Rectangle* ra = dynamic_cast<Rectangle*>(a)) {
		if (Triangle* tb = dynamic_cast<Triangle*>(b)) {
			return a->intersect(b);
		}
	}
	return false;
}

// Base - базовый класс иерархии
// Result - тип возвращаемого значения мультиметода
// Commutative - флаг, который показывает, что
// мультиметод коммутативный (т.е. f(x,y) = f(y,x)).
template<class Base, class Result, bool Commutative>
struct Multimethod2
{
	std::map<std::pair<std::type_index, std::type_index>, std::function<Result(Base*, Base*)>> type_names;
	// устанавливает реализацию мультиметода
	// для типов t1 и t2 заданных через typeid 
	// f - это функция или функциональный объект
	// принимающий два указателя на Base 
	// и возвращающий значение типа Result
	template<class T1, class T2>
	void addImpl(T1&& t1, T1&& t2, T2&& f)
	{
		auto p = std::make_pair<std::type_index, std::type_index>(t1, t2);
		type_names[p] = f;
	}

	// проверяет, есть ли реализация мультиметода
	// для типов объектов a и b
	bool hasImpl(Base * a, Base * b) const
	{
		// возвращает true, если реализация есть
		// если операция коммутативная, то нужно 
		// проверить есть ли реализация для b и а 
		if (Commutative) {

			auto p1 = std::make_pair<std::type_index, std::type_index>(typeid(*a), typeid(*b));
			auto p2 = std::make_pair<std::type_index, std::type_index>(typeid(*b), typeid(*a));
			return type_names.count(p1) ? true 
				: type_names.count(p2) ? true : false;
		}
		else {
			auto p = std::make_pair<std::type_index, std::type_index>(typeid(*a), typeid(*b));
			return type_names.count(p) ? true : false;
		}
	}

	// Применяет мультиметод к объектам
	// по указателям a и b
	Result call(Base * a, Base * b) const
	{
		// возвращает результат применения реализации
		// мультиметода к a и b
		if (Commutative) {

			auto p1 = std::make_pair<std::type_index, std::type_index>(typeid(*a), typeid(*b));
			auto p2 = std::make_pair<std::type_index, std::type_index>(typeid(*b), typeid(*a));
			
			if (type_names.count(p1)) {
				return type_names.at(p1)(a, b);
			}
			else if (type_names.count(p2)) {
				return type_names.at(p2)(b, a);
			}
		}
		else {
			auto p = std::make_pair<std::type_index, std::type_index>(typeid(*a), typeid(*b));
			return type_names.at(p)(a, b);
		}
	}
};


int main() {
	// мультиметод для наследников Shape
		// возращающий bool и являющийся коммутативным 
	Multimethod2<Shape, bool, true> is_intersect;

	// добавляем реализацию мультиметода для двух прямоугольников
	is_intersect.addImpl(typeid(Rectangle), typeid(Rectangle), is_intersect_r_r);

	// добавляем реализацию мультиметода для прямоугольника и треугольника
	is_intersect.addImpl(typeid(Rectangle), typeid(Triangle), is_intersect_r_t);

	// создаём две фигуры    
	//Shape * s1 = new Triangle();
	//Shape * s2 = new Rectangle();

	//auto b = is_intersect_r_t(s1, s2);
	//std::cout << b << std::endl;
	
	Shape * s1 = new Triangle();
	Shape * s2 = new Rectangle();

	// проверяем, что реализация для s1 и s2 есть
	if (is_intersect.hasImpl(s1, s2))
	{
		// вызывается функция is_intersect_r_t(s2, s1)
		bool res = is_intersect.call(s1, s2);
		std::cout << "Result = " << res << std::endl;
		// Замечание: is_intersect_r_t ожидает,
		// что первым аргументом будет прямоугольник
		// а вторым треугольник, а здесь аргументы
		// передаются в обратном порядке. 
		// ваша реализация должна самостоятельно 
		// об этом позаботиться
	}
	return 0;
};
