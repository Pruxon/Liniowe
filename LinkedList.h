#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

    template <typename Type>
    class LinkedList
    {
public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type*;
        using reference = Type&;
        using const_pointer = const Type*;
        using const_reference = const Type&;

        class ConstIterator;
        class Iterator;
        using iterator = Iterator;
        using const_iterator = ConstIterator;


private:
        struct Node
        {
            Node* Next = nullptr;
            Node* Prev = nullptr;
            value_type Value;


        };

        size_type Size;
        Node* Head;
        Node* Tail;

public:
        LinkedList()
        {
            Head = Tail = new Node;
            Size = 0;
        }

        LinkedList(std::initializer_list<Type> l):LinkedList()
        {
            for (auto it = l.begin(); it != l.end(); it++)
                append (*it);

        }

        LinkedList(const LinkedList& other):LinkedList()
        {
            *this = other;
        }

        LinkedList(LinkedList&& other):LinkedList()
        {
            *this = std::move(other);
        }

        ~LinkedList()
        {
            if (Size > 0)
                erase (begin(), end());
            delete Tail;
        }

        LinkedList& operator=(const LinkedList& other)
        {
            if (this == &other)
                return *this;

            erase (begin(), end());
            for (auto it = other.begin(); it != other.end(); it++)
                append (*it);

            return *this;
        }

        LinkedList& operator=(LinkedList&& other)
        {
            if (this == &other)
                return *this;

            Head = other.Head;
            other.Head = nullptr;

            Tail = other.Tail;
            other.Tail = nullptr;

            Size = other.Size;
            other.Size = 0;

            return *this;
        }

        bool isEmpty() const
        {
            if (Size == 0)
                return 1;
            return 0;
        }

        size_type getSize() const
        {
            return Size;
        }

        void append(const Type& item)
        {
            Node* NewNode = new Node;
            NewNode->Value = item;
            if (Size == 0)
            {
                Head = NewNode;
                Head->Next = Tail;
                Tail->Prev = Head;
            }
            else
            {
                Tail->Prev->Next = NewNode;
                NewNode->Prev = Tail->Prev;
                Tail->Prev = NewNode;
                NewNode->Next = Tail;
            }
            Size++;
        }

        void prepend(const Type& item)
        {
            Node* NewNode = new Node;
            NewNode->Value = item;
            if (Size == 0)
            {
                Head = NewNode;
                Head->Next = Tail;
                Tail->Prev = Head;
            }
            else
            {
                NewNode->Next = Head;
                Head->Prev = NewNode;
                Head = NewNode;
            }
            Size++;
        }

        void insert(const const_iterator& insertPosition, const Type& item)
        {
            if (insertPosition == begin())
                prepend(item);
            else if (insertPosition == end())
                append(item);
            else
            {
                Node* NewNode = new Node;
                NewNode->Value = item;

                NewNode->Next = insertPosition.nodePointer;
                NewNode->Prev = insertPosition.nodePointer->Prev;
                insertPosition.nodePointer->Prev->Next = NewNode;
                insertPosition.nodePointer->Prev = NewNode;

                Size++;
            }
        }

        Type popFirst()
        {
            if (Size == 0)
                throw std::logic_error("No items to pop!");
            auto Ret = Head->Value;
            erase (begin());
            return Ret;
        }

        Type popLast()
        {
            if (Size == 0)
                throw std::logic_error("No items to pop!");
            auto Ret = Tail->Prev->Value;
            erase (end()-1);
            return Ret;
        }

        void erase(const const_iterator& possition)
        {
            if (possition == end())
                throw std::out_of_range("NOPE!");
            else
                erase (possition, possition+1);
        }

        void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
        {
            if (firstIncluded == lastExcluded)
                return;
            if (firstIncluded == end())
                throw std::out_of_range("NOPEx2!");
            else
            {
                Node* First = firstIncluded.nodePointer;
                Node* Last = lastExcluded.nodePointer->Prev;
                if (firstIncluded == begin())
                    {
                        Head = lastExcluded.nodePointer;
                        Last->Next = nullptr;
                        Head->Prev = nullptr;
                        while (First != nullptr)
                        {
                            auto Temp = First;
                            First = First->Next;
                            delete Temp;
                            Size--;
                        }
                    }
                else
                {
                    First->Prev->Next = lastExcluded.nodePointer;
                    lastExcluded.nodePointer->Prev = First->Prev;
                    Last->Next = nullptr;
                    while (First != nullptr)
                        {
                            auto Temp = First;
                            First = First->Next;
                            delete Temp;
                            Size--;
                        }
                }
            }
        }

        iterator begin()
        {
            return iterator (cbegin());
        }

        iterator end()
        {
            return iterator (cend());
        }

        const_iterator cbegin() const
        {
            return ConstIterator(Head, Head, Tail);
        }

        const_iterator cend() const
        {
            return ConstIterator(Tail, Head, Tail);
        }

        const_iterator begin() const
        {
            return cbegin();
        }

        const_iterator end() const
        {
            return cend();
        }
    };

    template <typename Type>
    class LinkedList<Type>::ConstIterator
    {
    public:
        friend class LinkedList;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename LinkedList::value_type;
        using difference_type = typename LinkedList::difference_type;
        using pointer = typename LinkedList::const_pointer;
        using reference = typename LinkedList::const_reference;

    private:
        Node *nodePointer;
        Node *head;
        Node *tail;

    public:
        explicit ConstIterator(Node* nodePointer_, Node* head_, Node* tail_)
        {
            nodePointer = nodePointer_;
            head = head_;
            tail = tail_;
        }


        reference operator*() const
        {
            if (nodePointer == tail)
                throw std::out_of_range("Dereferencing end of list!");
            return nodePointer->Value;
        }

        ConstIterator& operator++()
        {
            if (nodePointer == tail)
                throw std::out_of_range("Incrementing end of list!");
            nodePointer = nodePointer->Next;
            return *this;
        }

        ConstIterator operator++(int)
        {
            auto Ret = *this;
            operator++();
            return Ret;
        }

        ConstIterator& operator--()
        {
            if (nodePointer == head)
                throw std::out_of_range("Decrementing begin of list!");
            nodePointer = nodePointer->Prev;
            return *this;
        }

        ConstIterator operator--(int)
        {
            auto Ret = *this;
            operator--();
            return Ret;
        }

        ConstIterator operator+(difference_type d) const
        {
            auto Ret = *this;

            while (d > 0)
            {
                Ret++;
                d--;
            }
            return Ret;
        }

        ConstIterator operator-(difference_type d) const
        {
            auto Ret = *this;

            while (d > 0)
            {
                Ret--;
                d--;
            }

            return Ret;
        }

        bool operator==(const ConstIterator& other) const
        {
            if (nodePointer == other.nodePointer)
                return 1;
            return 0;
        }

        bool operator!=(const ConstIterator& other) const
        {
            if (nodePointer != other.nodePointer)
                return 1;
            return 0;
        }
    };

    template <typename Type>
    class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
    {
    public:
        using pointer = typename LinkedList::pointer;
        using reference = typename LinkedList::reference;

        explicit Iterator()
        {}

        Iterator(const ConstIterator& other)
                : ConstIterator(other)
        {}

        Iterator& operator++()
        {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int)
        {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator& operator--()
        {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int)
        {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        Iterator operator+(difference_type d) const
        {
            return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const
        {
            return ConstIterator::operator-(d);
        }

        reference operator*() const
        {
            // ugly cast, yet reduces code duplication.
            return const_cast<reference>(ConstIterator::operator*());
        }
    };

}

#endif // AISDI_LINEAR_LINKEDLIST_H
