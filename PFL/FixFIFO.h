#pragma once

/*
    ###################################################################################
    FixFIFO.h
    Simple fixed-capacity FIFO container (queue), with continuous memory area (array) for element storage.
    This file is part of PFL (PR00F Foundation Library).
    Made by PR00F88
    2024
    ###################################################################################
*/

#include <cassert>
#include <vector>

namespace pfl
{
    /**
    * Simple fixed-capacity FIFO container (queue), with continuous memory area (array) for element storage.
    * 
    * Improvement idea: add begin(), end(), etc. iterators.
    */
    template <typename T>
    class FixFIFO
    {

    public:

        FixFIFO(const size_t& capacity) :
            m_nCapacity(capacity)
        {
            if (!capacity)
            {
                throw std::runtime_error("Capacity must be positive!");
            }

            //m_array = new T[capacity];
            m_array.resize(capacity);
        }

        ~FixFIFO() = default;

        FixFIFO(const FixFIFO&) = default;
        FixFIFO& operator=(const FixFIFO&) = default;
        FixFIFO(FixFIFO&&) = default;
        FixFIFO& operator=(FixFIFO&&) = default;

        /**
        * @return Size of the queue.
        */
        const size_t& size() const
        {
            return m_nSize;
        }

        /**
        * @return Capacity of the queue.
        */
        const size_t& capacity() const
        {
            return m_nCapacity;
        }

        /**
        * @return True if the queue is empty, false otherwise.
        */
        bool empty() const
        {
            return (size() == 0);
        }

        /**
        * @return True if the queue is full, false otherwise.
        */
        bool full() const
        {
            return (size() == capacity());
        }

        /**
        * Resets size of the queue to 0 i.e. the queue becomes empty.
        */
        void clear()
        {
            m_iBegin = 0;
            m_iEnd = 0;
            m_nSize = 0;
        }

        /**
        * Adds the elem to the back of the queue.
        * Complexity: O(1) constant.
        *
        * @param  elem The new elem to be added to the queue.
        *
        * @return True if push actually happened, false if push did not happen due to the queue being full.
        */
        bool push_back(const T& elem)
        {
            if (full())
            {
                return false;
            }

            // since we do "mod m_nCapacity" of course this will be always true but let's leave this here in case someone modifies something!
            assert(m_iEnd <= m_nCapacity);

            m_array[m_iEnd] = elem;
            m_iEnd = next_index(m_iEnd);
            m_nSize++;

            return true;
        }

        /**
        * Forcefully adds the elem to the back of the queue.
        * This means that the given elem will be pushed into the queue even if it is already full.
        * In such case, an implicit pop() will be done first to make space for the new elem.
        * Complexity: O(1) constant.
        *
        * @param  elem The new elem to be added to the queue.
        */
        void push_back_forced(const T& elem)
        {
            if (full())
            {
                pop_front();
            }

            // since we do "mod m_nCapacity" of course this will be always true but let's leave this here in case someone modifies something!
            assert(m_iEnd <= m_nCapacity);

            m_array[m_iEnd] = elem;
            m_iEnd = next_index(m_iEnd);
            m_nSize++;
        }

        /**
        * Removes the oldest elem from the queue.
        * Complexity: O(1) constant.
        *
        * @return The oldest elem of the queue that has just got removed.
        *         TODO if the queue is empty.
        */
        T pop_front()
        {
            if (empty())
            {
                throw std::runtime_error("Container is empty!");
            }

            // since we do "mod m_nCapacity" of course this will be always true but let's leave this here in case someone modifies something!
            assert(m_iBegin <= m_nCapacity);

            T& elem = m_array[m_iBegin];
            m_iBegin = next_index(m_iBegin);
            m_nSize--;

            return elem;
        }

        /**
        * @return The oldest elem of the queue.
        */
        const T& front() const
        {
            if (empty())
            {
                throw std::runtime_error("Container is empty!");
            }

            return m_array[m_iBegin];
        }

        /**
        * Can be used for iterating over underlying_array().
        * See next_index() for more info.
        * 
        * @return The index of the first elem in the queue, where the index is element index of underlying_array().
        *         Note that begin_index() can be greater than end_index(), depending on previous FIFO operations, since underlying array is treated as circular buffer.
        */
        const size_t& begin_index() const
        {
            return m_iBegin;
        }

        /**
        * Can be used for iterating over underlying_array().
        * See next_index() for more info.
        *
        * @return The index AFTER the last elem in the queue, where the index is element index of underlying_array().
        *         Note that end_index() can be smaller than begin_index(), depending on previous FIFO operations, since underlying array is treated as circular buffer.
        */
        const size_t& end_index() const
        {
            return m_iEnd;
        }

        /**
        * Can be used for iterating over underlying_array().
        * See prev_index() for more info.
        *
        * @return The index of the last elem in the queue, where the index is element index of underlying_array().
        */
        const size_t rbegin_index() const
        {
            // since m_iEnd is always the elem index where push_back() writes, the last elem must be before that
            return prev_index(m_iEnd);
        }

        /**
        * Can be used for iterating over underlying_array().
        * 
        * Example:
        * size_t i = fifo.begin_index();  // might not be 0
        * for (size_t n = 0; n < fifo.size(); n++)
        * {
        *   const auto& elem = fifo.underlying_array()[i];
        *   elem. ...(); // do something with elem
        *   i = fifo.next_index(i);
        * }
        * 
        * @param curr_index The index of the current elem in the queue.
        *                   Valid only if returned by begin_index(), or a prev_index() or an earlier next_index() call.
        *
        * @return The index of the next elem in the queue relative to the given curr_index, where the index is element index of underlying_array().
        *         Note that next_index() can be smaller than curr_index, depending on previous FIFO operations, since underlying array is treated as circular buffer.
        */
        size_t next_index(const size_t& curr_index) const
        {
            assert(m_nCapacity);  // ctor ensures
            return (curr_index + 1) % m_nCapacity;
        }

        /**
        * Can be used for reverse-iterating over underlying_array().
        *
        * Example:
        * size_t i = fifo.rbegin_index();
        * for (size_t n = 0; n < fifo.size(); n++
        * {
        *   const auto& elem = fifo.underlying_array()[i];
        *   elem. ...(); // do something with elem
        *   i = fifo.prev_index(i);
        * }
        *
        * @param curr_index The index of the current elem in the queue.
        *                   Valid only if returned by rbegin_index(), or a next_index() or an earlier prev_index() call.
        *
        * @return The index of the previous elem in the queue relative to the given curr_index, where the index is element index of underlying_array().
        *         Note that prev_index() can be greater than curr_index, depending on previous FIFO operations, since underlying array is treated as circular buffer.
        */
        size_t prev_index(const size_t& curr_index) const
        {
            assert(m_nCapacity);  // ctor ensures
            return (curr_index == 0) ? (m_nCapacity - 1) : (curr_index - 1) % m_nCapacity;
        }

        /**
        * For debug purpose only, the underlying array of elements can be observed.
        * 
        * @return The underlying fixed-size array where elements are stored.
        *         Note that order of elements in the queue might be different than order of elements in this array, since this array is treated as circular buffer.
        */
        const T* underlying_array() const
        {
            return m_array.data();
        }

    private:
        const size_t m_nCapacity = 0;   /**< Max number of elements in the queue, i.e. size of the underlying array. */

        //std::unique_ptr<int[]> m_array;
        std::vector<T> m_array;
        size_t m_iBegin = 0;      /**< Queue begin index, aka read index, where the oldest elem is placed. */
        size_t m_iEnd = 0;        /**< Queue end index, aka write index, where new elem will be placed. */
        size_t m_nSize = 0;       /**< Number of actual elements in the queue. */

    }; // class FixFIFO

} // namespace
