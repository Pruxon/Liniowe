#include <cstddef>
#include <cstdlib>
#include <string>
#include <chrono>
#include <iostream>
#include "Vector.h"
#include "LinkedList.h"

namespace
{

template <typename T>
using LinearCollection = aisdi::LinkedList<T>;

void perfomTest()
{
  LinearCollection<std::string> collection;
  collection.append("TODO");
}

} // namespace

template<typename Func>
long long measureTime(Func f) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto finish = std::chrono::high_resolution_clock::now();
    auto dur = finish - start;
    std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
}

void fillVector(aisdi::Vector<int> &vector, int elements)
{
    for (int i = 0; i < elements; ++i) {
        vector.append(i);
    }
}

void fillLinkedList(aisdi::LinkedList<int> &linkedList, int elements)
{
    for (int i = 0; i < elements; ++i) {
        linkedList.append(i);
    }
}

void performAppend (int elements, int new_elements)
    {
        aisdi::Vector<int> vector_;
        aisdi::LinkedList<int> list_;

        fillVector(vector_, elements);
        fillLinkedList(list_, elements);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < new_elements; i++) {
        vector_.append(i);
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "> Vector elapsed time: " << elapsed.count() << " s\n";

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < new_elements; i++) {
        list_.append(i);
    }
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << ">   List elapsed time: " << elapsed.count() << " s\n\n";
    }

void test_getCapacity()
{
    aisdi::Vector<int> vector_;
    if (vector_.getCapacity() == 8)
        std::cout<< "Get capacity works." << std::endl;
    else
        std::cout <<"ERROR getcapacity" << std::endl;
}


void test_add_memory()
{
    aisdi::Vector<int> vector_;
    size_t first = vector_.getCapacity();
    fillVector(vector_, 120);
    int second = vector_.getCapacity();
    if (first == 8 && second == 128)
        std::cout <<"Add_memory works."<< std::endl;
    else
        std::cout <<"ERROR add memory" << std::endl;
}

void test_popFirst_vector()
{
    aisdi::Vector<int> vector_;
    vector_.append(1);
    if (vector_.popFirst()==1)
        std::cout<< "pop vector works" << std::endl;
    else
        std::cout<< "pop vector error" << std::endl;
}

void test_popFirst_list()
{
    aisdi::LinkedList<int> list_;
    list_.append(1);
    if (list_.popFirst()==1)
        std::cout<< "pop list works" << std::endl;
    else
        std::cout<< "pop list error" << std::endl;
}

int main(int argc, char** argv)
{
  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest();

  test_popFirst_list();
  test_popFirst_vector();
  test_getCapacity();
  test_add_memory();
  performAppend(1000, 10);
  performAppend(10000, 100);
  performAppend(100000, 100);
  performAppend(1000000, 100);
  return 0;
}
