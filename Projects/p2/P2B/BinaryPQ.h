// Project identifier: 19034C8F3B1196BF8E0C6E1C0F973D2FD550B88F

#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include "Eecs281PQ.h"

// A specialized version of the 'heap' ADT implemented as a binary heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty heap with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
    } // BinaryPQ


    // Description: Construct a heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, data{ start, end } {
        updatePriorities();     // sort data
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automaticslly.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Heapify! (define fixUp & fixDown)
        for (int i = getParent(size() - 1); i >= 0; --i) {
            fixDown(size() - 1, i);
        }
    } // updatePriorities()


    // Description: Add a new element to the heap.
    // Runtime: O(log(n))
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
        // TODO: push() then fixUp()
        data.push_back(val);
        fixUp(size());
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop() {
        // TODO: replace top w/ bottom then fixDown()
        if (size() > 1) {
            data[0] = data.back();
            data.pop_back();
            fixDown(size() - 1, 0);
        }
        else
            data.pop_back();
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        // TODO: Implement this function.
        return data[0];
    } // top()


    // Description: Get the number of elements in the heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return data.size();
    } // size()


    // Description: Return true if the heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return data.empty();
    } // empty()


private:
    // Note: This vector *must* be used your heap implementation.
    std::vector<TYPE> data;

    // TODO: Add any additional member functions or data you require here.
    // For instance, you might add fixUp() and fixDown().

    int getParent(size_t x) {
        int i = (int)x;
        if (i % 2 == 0)
            --i;

        return (i / 2);
    }

    TYPE& getElement(std::size_t i) {
        return data[i - 1];
    } // getElement()
    
    const TYPE &getElement(std::size_t i) const {
        return data[i - 1];
    } // getElement()

    void fixUp(size_t k) {
        while (k > 1 && this->compare(getElement(k / 2), getElement(k))) {
            std::swap(getElement(k), getElement(k / 2));
            k /= 2;
        }
    }

    void fixDown(size_t end, size_t k) {
        while ((2 * k) + 1 <= end) {
            size_t i = (2 * k) + 1;
            if (i < end && this->compare(data[i], data[i + 1])) ++i;
            if (this->compare(data[i], data[k])) break;
            std::swap(data[k], data[i]);
            k = i;
        }
    }

}; // BinaryPQ


#endif // BINARYPQ_H
