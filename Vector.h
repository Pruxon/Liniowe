#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

    template <typename Type>
    class Vector
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
        size_type current_size = 0;
        size_type capacity = 8;
        pointer buffer;

    public:
        Vector()
        {
            buffer = new value_type[capacity];
        }

        Vector(std::initializer_list<Type> l)
        {
            current_size = 0;
            capacity = l.size();
            buffer = new value_type[capacity];
            for (auto i = l.begin(); i != l.end(); i++)
                append(*i);
        }

        Vector(const Vector& other)
        {
            current_size = other.current_size;
            capacity = other.capacity;
            buffer = new value_type[capacity];

            for (size_type i = 0; i < current_size; i++)
                buffer[i] = other.buffer[i];
        }

        Vector(Vector&& other)
        {
            current_size = other.current_size;
            capacity = other.capacity;
            if (this != &other)
            {
                buffer = other.buffer;
                other.buffer = nullptr;
            }
        }

        ~Vector()
        {
            delete[] buffer;
        }

        Vector& operator=(const Vector& other)
        {
            if (this == &other)
                return *this;
            current_size = other.current_size;
            capacity = other.capacity;
            delete[] buffer;
            buffer = new value_type[capacity];
            for (size_type i = 0; i < current_size; i++)
                buffer[i] = other.buffer[i];
            return *this;
        }

        Vector& operator=(Vector&& other)
        {
            if (this == &other)
                return *this;
            current_size = other.current_size;
            capacity = other.capacity;
            delete[] buffer;
            buffer = other.buffer;
            other.buffer = nullptr;
            return *this;
        }

        bool isEmpty() const
        {
            if (current_size == 0)
                return 1;
            else
                return 0;
        }

        size_type getSize() const
        {
            return this->current_size;
        }

        size_type getCapacity() const
        {
            return this->capacity;
        }

        void add_memory()
        {
            capacity *= 2;
            pointer new_buffer = new value_type [capacity];
            for (size_type i = 0; i < current_size; i++)
                new_buffer[i] = buffer [i];
            delete[] buffer;
            buffer = new_buffer;
        }

        void append(const Type& item)
        {
            if (current_size+1 == capacity)
                add_memory();
            buffer[current_size] = item;
            current_size++;
        }

        void prepend(const Type& item)
        {
            if (current_size+1 == capacity)
                add_memory();
            pointer new_buffer;
            new_buffer = new value_type[current_size+1];
            new_buffer[0] = item;
            for (size_type i = 0; i < current_size; i++)
                new_buffer[i+1] = buffer[i];
            delete[] buffer;
            buffer = new_buffer;
            current_size++;
        }

        void insert(const const_iterator& insertPosition, const Type& item)
        {
            if (insertPosition == end())
                append(item);
            else if (insertPosition == begin())
                prepend(item);
            else
            {
                size_type to_alloc = 0;
                if (current_size+1 == capacity)
                {
                    add_memory();
                    to_alloc = capacity;
                }
                pointer new_buffer = new value_type [capacity+to_alloc];
                size_type index = 0;

                for (auto i = begin(); i != insertPosition; i++)
                {
                    new_buffer[index] = buffer[index];
                    index++;
                }
                new_buffer[index] = item;

                for (auto i = iterator (insertPosition); i != end(); i++)
                {
                    new_buffer [index+1] = buffer[index];
                    index++;
                }
                delete[] buffer;
                buffer = new_buffer;
                current_size++;
            }
        }

        Type popFirst()
        {
            if (current_size == 0)
                throw std::logic_error("You are trying to pop an empty vector!");
            auto Ret = *(begin());
            for (size_type i = 0; i < current_size; i++)
                buffer[i] = buffer[i+1];
            current_size--;
            return Ret;
        }

        Type popLast()
        {
            if (current_size == 0)
                throw std::logic_error("You are trying to pop an empty vector!");
            auto Ret = *(end()-1);
            current_size--;
            return Ret;
        }

        void erase(const const_iterator& possition)
        {
            if (possition == end())
                throw std::out_of_range("Erasing vector end!");
            else if (possition == begin ())
            {
                popFirst();
            }
            else if (possition == --end())
            {
                popLast();
            }
            else
            {
                for (iterator it = possition; it!=end(); it++)
                    buffer[(it.actual_element)] = buffer[(it.actual_element+1)];
                current_size--;

            }
        }

        void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
        {
            if (firstIncluded == end())
                throw std::out_of_range("Erasing vector end!");
            if (firstIncluded == lastExcluded)
                return;
            size_type dif = lastExcluded.actual_element - firstIncluded.actual_element;
            for (iterator it = lastExcluded; it != end(); it++)
                buffer[(it.actual_element - dif)] = buffer [it.actual_element];

            current_size-=dif;
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
            return ConstIterator(0, this);
        }

        const_iterator cend() const
        {
            return ConstIterator(current_size, this);
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
    class Vector<Type>::ConstIterator
    {
    public:
        friend class Vector;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename Vector::value_type;
        using difference_type = typename Vector::difference_type;
        using pointer = typename Vector::const_pointer;
        using reference = typename Vector::const_reference;


    private:

        size_type actual_element;
        Vector<Type> const * buffer_pointer;

    public:

        explicit ConstIterator(size_type actual_element_, Vector<Type> const * buffer_pointer_)
        {
            actual_element = actual_element_;
            buffer_pointer = buffer_pointer_;
        }

        reference operator*() const
        {
            if (actual_element == buffer_pointer->current_size)
            {
                throw std::out_of_range("Dereferencing vector end!");
            }
            return buffer_pointer->buffer[actual_element];
        }

        ConstIterator& operator++()
        {
            if (actual_element == buffer_pointer->current_size)
                throw std::out_of_range("Incrementing last element!");
            else
                actual_element++;
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
            if (actual_element == 0)
                throw std::out_of_range("Decrementing first element!");
            else
                actual_element--;
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
            if (d+actual_element > buffer_pointer->current_size)
                throw std::out_of_range("NOPE");
            auto Ret = *this;
            while (d--)
                Ret++;
            return Ret;
        }

        ConstIterator operator-(difference_type d) const
        {
            if (d-actual_element < 0)
                throw std::out_of_range("NOPEx2");
            auto Ret = *this;
            while (d--)
                Ret--;
            return Ret;
        }

        bool operator==(const ConstIterator& other) const
        {
            if (actual_element == other.actual_element)
                return true;
            return false;
        }

        bool operator!=(const ConstIterator& other) const
        {
            if (actual_element != other.actual_element)
                return true;
            return false;
        }
    };

    template <typename Type>
    class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
    {
    public:
        using pointer = typename Vector::pointer;
        using reference = typename Vector::reference;

        explicit Iterator(/*size_type actual_element_, Vector<Type> const * buffer_pointer_*/)
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

#endif // AISDI_LINEAR_VECTOR_H
