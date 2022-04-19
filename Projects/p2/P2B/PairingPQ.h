// Project identifier: 19034C8F3B1196BF8E0C6E1C0F973D2FD550B88F

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the 'priority_queue' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // TODO: After you add add one extra pointer (see below), be sure to
            // initialize it here.
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }, parent{ nullptr }
            {}

            // Description: Allows access to the element at that Node's position.
			// There are two versions, getElt() and a dereference operator, use
			// whichever one seems more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data members of this
            // Node class from within the PairingPQ class. (ie: myNode.elt is a legal
            // statement in PairingPQ's add_node() function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            Node *parent;
    }; // Node


    // Description: Construct an empty priority_queue with an optional comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root{ nullptr }, numNodes{ 0 }
    {
        // TODO: Implement this function.
    } // PairingPQ()


    // Description: Construct a priority_queue out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root{ nullptr }, numNodes{ 0 } {
        for (InputIterator i = start; i != end; ++i) {
            push(*i);
        }
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ& other) :
        BaseClass{ other.compare }, root{ nullptr }, numNodes{ 0 } {
        // TODO: Implement this function.
        std::deque<Node *> dq;
        dq.push_back(other.root);
        while (!dq.empty()) {
            Node* curr = dq.front();
            dq.pop_front();

            if (curr->child)
                dq.push_back(curr->child);

            if (curr->sibling)
                dq.push_back(curr->sibling);

            push(curr->elt);
        }

    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    // TODO: when you implement this function, uncomment the parameter names.
    PairingPQ& operator=(const PairingPQ& rhs) {
        // TODO: L6, best copying (copy-swap)
        PairingPQ temp(rhs);

        std::swap(root, temp.root);
        std::swap(numNodes, temp.numNodes);

        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        // TODO: Implement this function.
        if (!empty()) {
            std::deque<Node*> dq;
            dq.push_back(root);
            while (!dq.empty()) {
                Node* curr = dq.front();
                dq.pop_front();

                if (curr->child)
                    dq.push_back(curr->child);

                if (curr->sibling)
                    dq.push_back(curr->sibling);

                delete curr;
            }
        }
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the priority_queue are out of order and
    //              'rebuilds' the priority_queue by fixing the priority_queue invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.
        std::deque<Node*> dq;
        dq.push_back(root);
        root = nullptr;
        while (!dq.empty()) {
            Node* curr = dq.front();
            dq.pop_front();

            if (curr->child) {
                dq.push_back(curr->child);
                curr->child = nullptr;
            }

            if (curr->sibling) {
                dq.push_back(curr->sibling);
                curr->sibling = nullptr;
            }

            curr->parent = nullptr;

            if (root)
                root = meld(root, curr);
            else
                root = curr;
        }
    } // updatePriorities()


    // Description: Add a new element to the priority_queue. This is almost done,
    //              in that you should implement push functionality in the addNode()
    //              function, and this function should call addNode().
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the priority_queue.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the priority_queue is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
        Node* node = root;
        if (size() < 3 && !empty()) {   // size == 1 or 2
            if (root->child) {
                root = root->child;
                root->parent = nullptr;
            }
            else
                root = nullptr;

            delete node;
        }
        else {
            (root->child)->parent = nullptr;
            node = node->child;
            delete root;
            root = nullptr;

            std::deque<Node*> dq;
            for (Node* ptr = node; ptr != nullptr; ptr = ptr->sibling) {
                dq.push_back(ptr);
            }

            size_t i = 0;
            while (dq.size() != 1) {
                if (i + 1 < dq.size()) {
                    Node* a = dq.at(i);
                    Node* b = dq.at(i + 1);

                    // Break siblings & parent links
                    a->sibling = nullptr;
                    b->sibling = nullptr;
                    a->parent = nullptr;
                    b->parent = nullptr;

                    // Push melded nodes to back then pop_front
                    dq.push_back(meld(a, b));
                    dq.pop_front();
                    dq.pop_front();
                }
            }

            root = dq.front();
        }
        --numNodes;
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        return root->elt;
    } // top()


    // Description: Get the number of elements in the priority_queue.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return numNodes;
    } // size()

    // Description: Return true if the priority_queue is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return (numNodes == 0);
    } // empty()


    // Description: Updates the priority of an element already in the priority_queue by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain priority_queue invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    // TODO: when you implement this function, uncomment the parameter names.
    void updateElt(Node* node, const TYPE & new_value) {
        // TODO: Implement this function
        // NOTDO: call updatePriorities()
        
        node->elt = new_value;

        if (this->compare((node->parent)->elt, new_value)) {
            // Left-most
            if ((node->parent)->child == node) {
                (node->parent)->child = node->sibling;
            }
            else {  // Middle or right-most
                Node* prev;
                for (prev = (node->parent)->child; prev->sibling != node; prev = prev->sibling) {} // Find prev

                // Link siblings
                prev->sibling = node->sibling;
            }
            node->sibling = nullptr;
            node->parent = nullptr;
            root = meld(root, node);
        }
    } // updateElt()


    // Description: Add a new element to the priority_queue. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    // NOTE: Whenever you create a node, and thus return a Node *, you must be sure to
    //       never move or copy/delete that node in the future, until it is eliminated
    //       by the user calling pop().  Remember this when you implement updateElt() and
    //       updatePriorities().
    Node* addNode(const TYPE & val) {
        // TODO: Implement this function
        Node* node = new Node(val);
        if (empty()) {
            root = node;
        }
        else {
            root = meld(root, node);
        }
        ++numNodes;
        return node;
    } // addNode()


private:
    // TODO: Add any additional member functions or data you require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap papers).
    Node* root;
    int numNodes;

    Node* meld(Node* pa, Node* pb) {
        // pa and pb have no parent/prev and no sibling
        // use this->compare(pa->elt, pb->elt)
        // assume whoever calls meld does not pass nullptrs
        if (!pa->parent && !pa->sibling && !pb->parent && !pb->sibling) {
            if (this->compare(pa->elt, pb->elt)) {
                pa->parent = pb;
                pa->sibling = pb->child;
                pb->child = pa;
                return pb;
            }
            else {
                pb->parent = pa;
                pb->sibling = pa->child;
                pa->child = pb;
                return pa;
            }
        }
        return nullptr;
    }
};


#endif // PAIRINGPQ_H
