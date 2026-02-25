// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C262F4E

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include <deque>
#include <utility>

#include "Eecs281PQ.hpp"

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
    public:
        // Description: Custom constructor that creates a node containing
        //              the given value.
        explicit Node(const TYPE &val)
            : elt { val } {}

        // Description: Allows access to the element at that Node's position.
        //              There are two versions, getElt() and a dereference
        //              operator, use whichever one seems more natural to you.
        // Runtime: O(1) - this has been provided for you.
        const TYPE &getElt() const { return elt; }
        const TYPE &operator*() const { return elt; }

        // The following line allows you to access any private data
        // members of this Node class from within the PairingPQ class.
        // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
        // function).
        friend PairingPQ;

    private:
        TYPE elt;
        Node *child = nullptr;
        Node *sibling = nullptr;
	Node *parent = nullptr; // i think parent is easier to work with ngl
    };  // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass { comp }, root(nullptr), count(0) {
        // TODO: Implement this function.
    }  // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass { comp } {
	while (start != end) push(*(start++));
    }  // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other)
        : BaseClass { other.compare } {
	if (other.empty()) {
		count = 0;
		root = nullptr;
		return;
	}
	std::deque<Node*> search; // proj 1 search algo, queue based
	while (!search.empty()) {
		Node *current = search.front();
		search.pop_front();
		if (current->sibling) search.push_back(current->sibling);
		if (current->child) search.push_back(current->child);
		push(current-> getElt());
	}
    }  // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
	PairingPQ copy(rhs);
	std::swap(root, copy.root);
	std::swap(count, copy.count);
        return *this;
    }  // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
	if (empty()) return;
	std::deque<Node*> search; // proj 1 search algo to find all nodes and delete them lol
	search.push_back(root);
	while (!search.empty()) {
		Node *current = search.front();
		search.pop_front();
		if (current->sibling) search.push_back(current->sibling);
		if (current->child) search.push_back(current->child);
		delete current;
	}
	
    }  // ~PairingPQ()


    // Description: Move constructor and assignment operators don't need any
    //              code, the members will be reused automatically.
    PairingPQ(PairingPQ &&) noexcept = default;
    PairingPQ &operator=(PairingPQ &&) noexcept = default;


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant.  You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
	if (empty()) return;
	std::deque<Node*> search; search.push_back(root);
	root = nullptr;
	while (!search.empty()) {
		Node *current = search.front();
		search.pop_front();
		if (current->sibling) search.push_back(current->sibling);
		if (current->child) search.push_back(current->child);
		current->parent = current->sibling = current->child = nullptr;
		if (empty()) root = current;
		else root = meld(root, current);
	}
    }  // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely
    //              in the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) { addNode(val); }  // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    // an element when the pairing heap is empty. Though you are welcome to
    // if you are familiar with them, you do not need to use exceptions in
    // this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
	--count;
	Node *nodeptr = root->child; delete root;
	if (nodeptr) return;
	std::deque<Node*> search;
	while (nodeptr) {
		nodeptr->parent = nullptr;
		Node *sibling = nodeptr; nodeptr = nodeptr->sibling;
		sibling->sibling = nullptr;
		search.push_back(sibling);
	}
	while (search.size() > 1) {
		Node *one = search.front(); search.pop_front();
		Node *two = search.front(); search.pop_front();
		search.push_back(meld(one, two));
	} root = search.front(); 
    }  // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        return root->elt;  // TODO: Delete or change this line
    }  // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    [[nodiscard]] virtual std::size_t size() const {
        // TODO: Implement this function
        return count;  // TODO: Delete or change this line
    }  // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    [[nodiscard]] virtual bool empty() const {
        // TODO: Implement this function
        return !root;  // TODO: Delete or change this line
    }  // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value.  Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //              extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node *node, const TYPE &new_value) {
	node->elt = new_value;
	if (node == root || this->compare(node->elt , node->parent->elt)) return;
	if (node->parent->child == node) node->parent->child = node->sibling;
	else {
		Node *other= node->parent->child;
		while (node != other->sibling) other = other->sibling;
		other->sibling = node->sibling;
	}
	node->parent = nullptr; node->sibling = nullptr;
	root = meld(root, node); 
    }  // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // Runtime: O(1)
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    Node *addNode(const TYPE &val) {
	Node *nodeptr = new Node(val);
	++count; 
	if (empty()) {
		root = nodeptr;
		return nodeptr;
	}
	root = meld(root, nodeptr);
	return nodeptr;
    }  // addNode()


private:
	Node *root;
	size_t count;

	Node *meld (Node *l, Node *r) {
		Node *Parent, *Child;
		if (this->compare(r->elt, l->elt)) { Parent = l; Child = r; }
		else { Parent = r; Child = l;
		if (Parent->child) Child->sibling = Parent->child;
		Parent->child = Child;
		Child->parent = Parent;
		}
		return Parent;
	}
};

#endif  // PAIRINGPQ_H
