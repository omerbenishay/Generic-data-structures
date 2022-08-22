#ifndef SORTED_LIST_H_
#define SORTED_LIST_H_

#include <assert.h>
#include <stdexcept>
namespace mtm {

    template<class T>
    class SortedList;

    /* 
    * class Node - class that represents a "node" that has as a private members:
    * T data - Will hold the data of the generic T type
    * Node<T>* next - a pointer to next node that keeps list of nodes connected to each other.
    */
    template<class T>
    class Node
    {
        public:
            //We wont let the user build an empty node by himself
            Node() = delete;
            
            /**
            * ~Node(destructor): destructs a Node object. 
            * The d'ctor will be the default one since we dont have to explicitly free something
            * @param this - pointer to the Node object we destruct.
            */
            ~Node() = default;
            
            /**
            * Node(constructor): create a Node object. 
            * @param this - pointer to the Node object we create.
            * @param val - the type T object we would like to intiate the node with.
            * The c'tor is explicit because we dont want implicit conversions and copy-initialization.      
            */
            explicit Node(const T val) : data(val), next(nullptr) {}
            
            /**
            * Node(copy constructor): copies an Node object. 
            * @param this - pointer to the Node object we copy into.
            * @param node - pointer to the Node we want to copy.   
            * the copy node is completely independent.
            */
            Node(const Node<T>* node) : data(node->data), next(node->next) {}
            
            //We want access to the members of class "Node" from class "SortedList".
            friend class SortedList<T>;
        private:
            T data;
            Node<T>* next;
            
    };

    //SortedList Class:
    template<class T>
    class SortedList 
    {
        public:
            /**
            * SortedList(constructor): create an empty SortedList object. 
            * @param this - pointer to the SortedList object we create.
            * This c'tor creates an empty sortedlist.
            */
            SortedList();

            /**
            * ~SortedList(destructor): destructs a SortedList object. 
            * @param this - pointer to the SortedList object we destruct.
            */
            ~SortedList();
            
            /**
            * SortedList(copy constructor): copies a SortedList object. 
            * @param this - pointer to the SortedList object we copy into.
            * @param list - reference to the SortedList we want to copy.   
            * the copy SortedList is completely independent.
            */
            SortedList(const SortedList<T>& list);
            
            /**
            * operator=(assignment operator): copies list into *this. 
            * @param this - pointer to the SortedList object we assign into.
            * @param list - the list we want to assign.
            * 
            * @return A refrence to *this after the assignment.
            */
            SortedList<T>& operator=(const SortedList<T>& list);

            /**
            * insert: inserts element(type T) into the SortedList object. The list remain sorted after 
            * this function ends. 
            * @param this - pointer to the SortedList object that we want to insert into.
            * @param element - the T element we would like to insert.
            * 
            * @throw std::bad_alloc - in case of an allocation error.
            */
            void insert(const T& element);
            
            /**
            * remove: The function removes the Node iter is pointing on. 
            * The function keeps the list linked and sorted. 
            * @param this - pointer to the SortedList object that we want to remove from.
            * @param iter - iterator that points on the node we want to remove.
            * 
            */
            void remove(typename SortedList<T>::const_iterator iter);
            
            /**
            * length: The function returns the length of the SortedList object.
            * @param this - pointer to the SortedList object that we want it's length.
            * 
            * @return An int which represents the length of the list.
            */
            int length() const;

            /**
            * filter: The function creates and returns a new SortedList object.
            * The new SortedList object contains all of the objects in *this that
            * matches the condition in the predicate function.
            * @param this - pointer to the SortedList object that we want to filter.
            * @param predicate_func - a predicate function, recieves a T and returns bool.
            */
            template<class Predicate>
            SortedList<T> filter(Predicate predicate_func) const;

            /**
            * apply: The function creates and returns a new SortedList object.
            * The new SortedList object contains all of the objects in *this after the 
            * operation of the apply function.
            * @param this - pointer to the SortedList object that we want to apply.
            * @param apply_func - an apply function, recieves a T and returns a T.
            */
            template<class Apply>
            SortedList<T> apply(Apply apply_func) const;

            /**
            * class const_iterator: this class allows the user to iterate over
            * the SortedList object.
            */
            class const_iterator;
            
            /**
            * begin: The function creates and returns a new const_iterator object.
            * The new const_iterator object points on the head of the list.
            * @param this - pointer to the SortedList object.
            */
            typename SortedList<T>::const_iterator begin() const;
            
            /**
            * end: The function creates and returns a new const_iterator object.
            * The new const_iterator object points on the end of the list.
            * @param this - pointer to the SortedList object.
            */
            typename SortedList<T>::const_iterator end() const;
            
        private:
            //Node that is the head of the SortedList
            Node<T>* head;

            /**
            * checkIfEmpty: A private function that checks if *this is an empty list. 
            * @param this - pointer to the SortedList object that we want to check if empty.
            * 
            * @return true - if *this is an empty list. 
            * false - otherwise.
            */
            bool checkIfEmpty() const;
    };

    template<class T>
    SortedList<T>::SortedList() : head(nullptr) {}

    template<class T>
    SortedList<T>::~SortedList()
    {
        while (head != nullptr) {
            Node<T> *temp = head;
            head = head->next;
            delete temp;
        }
    }

    template<class T>
    SortedList<T>::SortedList(const SortedList<T>& list) : head(nullptr) 
    {
        Node<T> *temp = list.head;
        while (temp != nullptr) {
            this->insert(temp->data);
            temp = temp->next;
        }

    }

    template<class T>
    SortedList<T>& SortedList<T>::operator=(const SortedList<T>& list)
    {
        SortedList<T> temp(list);//Copying list to temp
        Node<T> *temp_head = head;
        head = temp.head;//this->head now is a new copied list  
        temp.head = temp_head;//we did this because now the old list will be destroyed. 
        
        return *this;
    }

    template<class T>
    bool SortedList<T>::checkIfEmpty() const
    {
        if (head == nullptr) {
            return true;
        }
        return false;
    }

    template<class T>
    void SortedList<T>::insert(const T& element)
    {
        Node<T> *node_to_insert = new Node<T>(element);//if the alloc fail then it throws an exception
        //and its ok because we have nothing to free
        
        if (this->checkIfEmpty()) {
            head = node_to_insert;
            return;
        }
        Node<T> *temp = head;
        if (element < temp->data) {
            node_to_insert->next = head;
            head = node_to_insert;
            return;
        }
        while (temp->next != nullptr && temp->next->data < element) {
            temp = temp->next;
        }
        if (temp->next == nullptr) {
            temp->next = node_to_insert;
        }
        else {
            node_to_insert->next = temp->next;
            temp->next = node_to_insert;
        }
    }

    template<class T>
    void SortedList<T>::remove(typename SortedList<T>::const_iterator iter)
    {
         if (this->checkIfEmpty()) {
            return;
        }
        if (head == iter.currNode()) {
            Node<T> *temp_next = head->next;
            delete head;
            head = temp_next;
            return;
        }
        Node<T> *temp = head;
        while (temp->next != nullptr) {
             if (temp->next == iter.currNode()) {
                 Node<T> *temp_next_of_deleted_node = temp->next->next;
                 delete temp->next;
                 temp->next = temp_next_of_deleted_node;
                 return;
             }
             temp = temp->next; 
        } 
    }

    template<class T>
    int SortedList<T>::length() const
    {
        int len = 0;
        Node<T> *temp = head;
        while (temp != nullptr) {
            len++;
            temp = temp->next;
        }
        return len;
    }

    template<class T>
    template<class Predicate>
    SortedList<T> SortedList<T>::filter(Predicate predicate_func) const 
    {
        SortedList<T> result;
        Node<T> *temp = head;
        while (temp != nullptr) {
            if (predicate_func(temp->data)) {
                 result.insert(temp->data);
            }
            temp = temp->next;
        }
        return result;
    }

    template<class T>
    template<class Apply>
    SortedList<T> SortedList<T>::apply(Apply apply_func) const 
    {
        SortedList<T> result;
        Node<T> *temp = head;
        while (temp != nullptr) {
            result.insert(apply_func(temp->data));
            temp = temp->next;
        }
        return result;
    }

    template<class T>
    typename SortedList<T>::const_iterator SortedList<T>::begin() const 
    {
        return const_iterator(this, 0);
    }

    template<class T>
    typename SortedList<T>::const_iterator SortedList<T>::end() const 
    { 
        return const_iterator(this, this->length());
    } 

    template<class T>
    class SortedList<T>::const_iterator 
    {    
        private:
            
            const SortedList<T>* sorted_list;
            
            int index;
            
            /**
            * const_iterator(constructor): create a const_iterator object. 
            * @param this - pointer to the const_iterator object we create.
            * @param index - an index that indicates which node in the list we looking on.
            * The c'tor is private because we dont want that the user will be able to construct a
            * const iterator by himself(only with begin and end). 
            */
            const_iterator(const SortedList<T> *list, int index);
            
            //We want access to the members of class "const_iterator" from class "SortedList".
            friend class SortedList<T>;
            
            /**
            * currNode: The function returns a pointer to the current node that *this is pointing at.
            * @param this - pointer to the const_iterator object.
            * 
            * @return A pointer to the node that iter is pointing at.
            */
            Node<T>* currNode() const;

        public:
            /**
            * operator*: returns the value(by reference) of the node that the iterator pointing at.
            * @param this - pointer to the const_iterator object.
            * 
            * @return A const reference to the object(type T) that the iterator is pointing at.
            */
            const T& operator*() const;
            
            /**
            * operator++(prefix): advance the iterator to point on the next node in the SortedList object.
            * and returns a reference to the advanced iterator.
            * @param this - pointer to the const_iterator object.
            * 
            * @return A reference to the advanced iterator.
            */
            const_iterator& operator++();
            
            /**
            * operator++(postfix): advance the iterator to point on the next node in the SortedList object.
            * and returns a const iterator before the advance.
            * @param this - pointer to the const_iterator object.
            * 
            * @return A const iterator before the advance.
            */
            const_iterator operator++(int);
            
            /**
            * operator==: checks if two iterators points on the very same node in the same list.
            * or if both iterators points on the end of some lists(could be different lists). 
            * @param this - pointer to the const_iterator object.
            * @param iterator - a const_iterator object.
            * 
            * @return True - if both iterators points on the very same node in the same list.
            * or if both iterators points on the end of some lists(could be different lists).
            * False - otherwise.
            */
            bool operator==(const const_iterator& iterator) const;
            
            /**
            * operator!=: checks if two iterators not points on the very same node in the same list.
            * or if both iterators not points on the end of some lists(could be different lists). 
            * @param this - pointer to the const_iterator object.
            * @param iterator - a const_iterator object.
            * 
            * @return True - if both iterators not points on the very same node in the same list.
            * or if both iterators not points on the end of some lists(could be different lists).
            * False - otherwise.
            */
            bool operator!=(const const_iterator& iterator) const;
            
            /**
            * const_iterator(copy constructor): copies a const_iterator object. 
            * @param this - pointer to the const_iterator object we copy into.
            * @param iterator - reference to the const_iterator we want to copy. 
            * could be the default one because it has only two simple fields:
            * a pointer to class and an int number. 
            */
            const_iterator(const const_iterator& iterator) = default;
            
            /**
            * const_iterator(assignment operator): copies const_iterator into *this. 
            * could be the default one because it has only two simple fields:
            * a pointer to class and an int number.
            * @param this - pointer to the const_iterator object we assign into.
            * @param iterator - the iterator we want to assign.
            *  
            * @return A refrence to *this after the assignment.
            */
            const_iterator& operator=(const const_iterator& iterator) = default;
            
            /**
            * ~const_iterator(destructor): destructs a const_iterator object. 
            * could be the default one because we have only two simple fields
            * (a pointer to class and an int number) to destroy.
            * @param this - pointer to the const_iterator object we destruct.
            */
            ~const_iterator() = default;
    };

    template<class T>
    SortedList<T>::const_iterator::const_iterator(const SortedList<T> *list, int index) :
        sorted_list(list),
        index(index)
    {}

    template<class T>
    const T& SortedList<T>::const_iterator::operator*() const
    {   
        if(index >= sorted_list->length()){
            throw std::out_of_range("Error: Iterator out of range.");
        }
        Node<T> *temp = sorted_list->head;
        for (int i = 0; i < index; i++) {
            temp = temp->next;
        }
        return temp->data;
    }

    template<class T>
    typename SortedList<T>::const_iterator& SortedList<T>::const_iterator::operator++()
    {
        if(index >= this->sorted_list->length()){
            throw std::out_of_range("Error: Iterator out of range.");
        }
        ++index;
        return *this;
    }

    template<class T>
    typename SortedList<T>::const_iterator SortedList<T>::const_iterator::operator++(int)
    {
        const_iterator result = *this;
        ++*this;
        return result;
    }

    template<class T>
    bool SortedList<T>::const_iterator::operator==(const const_iterator& iterator) const
    {
        /*if both iterators points on the end of the list(maybe they're not on the same list)
         then both iterators are equal because the end of the list is the same for every list.*/
        if(index == sorted_list->length() && iterator.index == iterator.sorted_list->length()){
            return true;
        }
        if(sorted_list != iterator.sorted_list){
            return false;
        }
        return index == iterator.index;
    }

    template<class T>
    bool SortedList<T>::const_iterator::operator!=(const const_iterator& iterator) const
    {
        return !(*this == iterator);
    }

    template<class T>
    Node<T>* SortedList<T>::const_iterator::currNode() const
    {
        if(index >= sorted_list->length()){
            throw std::out_of_range("Error: Iterator out of range.");
        }
        Node<T> *node_to_return = sorted_list->head;
        for(int i = 0; i < index; i++){
            node_to_return = node_to_return->next;
        }
        return node_to_return;
    }
}


#endif /*SORTED_LIST_H_*/
