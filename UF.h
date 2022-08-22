#ifndef UF_H_
#define UF_H_

#include <memory>

template <class dataT>
class UF {
    public:
        int k;
        int* size;
        int* parent;
        std::shared_ptr<dataT>* elements;

        UF(int k);
        ~UF();
        std::shared_ptr<dataT>& Find(int elementId);
        void Union(int p, int q);
};

template <class dataT>
UF<dataT>::UF(int k) : k(k) {
    size = new int[k];
    parent = new int[k];
    elements = new std::shared_ptr<dataT>[k];
    
    for (int i = 0; i < k; i++) {
        size[i] = 1;
        parent[i] = 0;
        elements[i] = std::shared_ptr<dataT>(new dataT(i + 1));
    }
}

template <class dataT>
UF<dataT>::~UF() {
    delete [] size;
    delete [] parent;
    delete [] elements;
}

template <class dataT>
std::shared_ptr<dataT>& UF<dataT>::Find(int elementId) {
    int root = elementId;
    while (parent[root - 1] != 0)
        root = parent[root - 1];
    
    int curr = elementId;
    while (curr != root) {
        int temp = parent[curr - 1];
        parent[curr - 1] = root;
        curr = temp;
    }

    return elements[root - 1];
}

template <class dataT>
void UF<dataT>::Union(int p, int q) {
    if (size[p - 1] < size[q - 1]) {
        parent[p - 1] = q;
        size[q - 1] += size[p - 1];
        size[p - 1] = 0;
    } else {
        parent[q - 1] = p;
        size[p - 1] += size[q - 1];
        size[q - 1] = 0;
    }
}

#endif /* UF_H */