#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_


#include <memory>
#include "listNode.h"
#include <stdbool.h>


template <class keyT, class dataT>
class HashTable {
    private:
        void ChangeSize(bool expand);

    public:
        int m;
        int size;
        std::shared_ptr<ListNode<keyT, dataT>>* arr;

        HashTable();
        HashTable(const HashTable<keyT, dataT>& copy);
        HashTable<keyT, dataT>& operator=(const HashTable<keyT, dataT>& copy);
        ~HashTable();
        void Insert(keyT key, std::shared_ptr<dataT>& data);
        void Remove(keyT key);
        std::shared_ptr<dataT> Get(keyT key) const;
        bool IfExists(keyT key) const;
        static std::shared_ptr<HashTable<keyT, dataT>> Merge(const HashTable<keyT, dataT>& ht1, const HashTable<keyT, dataT>& ht2);
};

template <class keyT, class dataT>
HashTable<keyT, dataT>::HashTable() : m(3), size(0) {
    arr = new std::shared_ptr<ListNode<keyT, dataT>>[m];
}

template <class keyT, class dataT>
HashTable<keyT, dataT>::HashTable(const HashTable<keyT, dataT>& copy) : m(copy.m), size(copy.size) {
    arr = new std::shared_ptr<ListNode<keyT, dataT>>[m];
    for (int i = 0; i < m; i++) {
        std::shared_ptr<ListNode<keyT, dataT>> curr = copy.arr[i];
        while (curr) {
            std::shared_ptr<ListNode<keyT, dataT>> toAdd = std::shared_ptr<ListNode<keyT, dataT>>(new ListNode<keyT, dataT>(curr->key, curr->data));
            std::shared_ptr<ListNode<keyT, dataT>> head = arr[curr->key % m];
            toAdd->next = head;
            arr[curr->key % m] = std::make_shared<ListNode<keyT, dataT>>(*toAdd);
            curr = curr->next;
        }
    }
}

template <class keyT, class dataT>
HashTable<keyT, dataT>& HashTable<keyT, dataT>::operator=(const HashTable<keyT, dataT>& copy) {
    std::shared_ptr<ListNode<keyT, dataT>>* newArr = new std::shared_ptr<ListNode<keyT, dataT>>[copy.m];
    for (int i = 0; i < copy.m; i++) {
        std::shared_ptr<ListNode<keyT, dataT>> curr = copy.arr[i];
        while (curr) {
            std::shared_ptr<ListNode<keyT, dataT>> toAdd = std::shared_ptr<ListNode<keyT, dataT>>(new ListNode<keyT, dataT>(curr->key, curr->data));
            std::shared_ptr<ListNode<keyT, dataT>> head = newArr[curr->key % copy.m];
            toAdd->next = head;
            newArr[curr->key % copy.m] = std::make_shared<ListNode<keyT, dataT>>(*toAdd);
            curr = curr->next;
        }
    }

    delete [] arr;
    this->arr = newArr;
    this->m = copy.m;
    this->size = copy.size;

    return *this;
}

template <class keyT, class dataT>
HashTable<keyT, dataT>::~HashTable() {
    delete [] arr;
}


template <class keyT, class dataT>
std::shared_ptr<dataT> HashTable<keyT, dataT>::Get(keyT key) const {
    std::shared_ptr<ListNode<keyT, dataT>> curr = arr[key % m];
    while (curr != nullptr) {
        if (curr->key == key)
            return curr->data;
        curr = curr->next;
    }

    return nullptr;
}

template <class keyT, class dataT>
bool HashTable<keyT, dataT>::IfExists(keyT key) const {
    if (this->Get(key) != nullptr)
        return true;
    return false;
}

template <class keyT, class dataT>
void HashTable<keyT, dataT>::Insert(keyT key, std::shared_ptr<dataT>& data) {
    if (this->IfExists(key))
        return;
    
    std::shared_ptr<ListNode<keyT, dataT>> toAdd = std::shared_ptr<ListNode<keyT, dataT>>(new ListNode<keyT, dataT>(key, data));
    std::shared_ptr<ListNode<keyT, dataT>> head = arr[key % m];
    toAdd->next = head;
    arr[key % m] = std::make_shared<ListNode<keyT, dataT>>(*toAdd);
    size++;

    if (size == m)
        this->ChangeSize(true);
    
}

template <class keyT, class dataT>
void HashTable<keyT, dataT>::Remove(keyT key) {
    if (!this->IfExists(key))
        return;

    std::shared_ptr<ListNode<keyT, dataT>> curr = arr[key % m];
    if (curr->key == key) {
            arr[key % m] = curr->next;
    }
    else {
        while (curr != nullptr && curr->next != nullptr) {
            if (curr->next->key == key) {
                curr->next = curr->next->next;
                break;
            }
            curr = curr->next;
        }
    }
    size--;
    if (((double)size / m) <= 1/9 && size != 0)
        this->ChangeSize(false);
}

template <class keyT, class dataT>
void HashTable<keyT, dataT>::ChangeSize(bool expand) {
    int newM = m;
    std::shared_ptr<ListNode<keyT, dataT>>* newArr;

    if (expand) {
        newM = m * 3;
        newArr = new std::shared_ptr<ListNode<keyT, dataT>>[newM];
    } else {
        newM = m / 3;
        newArr = new std::shared_ptr<ListNode<keyT, dataT>>[newM];
    }

    for (int i = 0; i < m; i++) {
        std::shared_ptr<ListNode<keyT, dataT>> curr = this->arr[i];
        while (curr != nullptr) {
            std::shared_ptr<ListNode<keyT, dataT>> toAdd = std::shared_ptr<ListNode<keyT, dataT>>(new ListNode<keyT, dataT>(curr->key, curr->data));
            std::shared_ptr<ListNode<keyT, dataT>> head = newArr[curr->key % newM];
            toAdd->next = head;
            newArr[curr->key % newM] = std::make_shared<ListNode<keyT, dataT>>(*toAdd);
            curr = curr->next;
        }
    }
    this->m = newM;

    delete [] this->arr;
    this->arr = newArr;
}

template <class keyT, class dataT>
std::shared_ptr<HashTable<keyT, dataT>> HashTable<keyT, dataT>::Merge(const HashTable<keyT, dataT>& ht1, const HashTable<keyT, dataT>& ht2) {
    std::shared_ptr<HashTable<keyT, dataT>> merged = std::shared_ptr<HashTable<keyT, dataT>>(new HashTable<keyT, dataT>());
    for (int i = 0; i < ht1.m; i++) {
        std::shared_ptr<ListNode<keyT, dataT>> curr = ht1.arr[i];
        while (curr) {
            merged->Insert(curr->key, curr->data);
            curr = curr->next;
        }
    }

    for (int i = 0; i < ht2.m; i++) {
        std::shared_ptr<ListNode<keyT, dataT>> curr = ht2.arr[i];
        while (curr) {
            merged->Insert(curr->key, curr->data);
            curr = curr->next;
        }
    }
    return merged;
}


#endif /* HASH_TABLE_H_ */