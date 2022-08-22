#ifndef BST_H_
#define BST_H_

#include <memory>
#include "node.h"
#include "map.h"

template <class keyT, class dataT>
class BST {
    private:
        static std::shared_ptr<Node<keyT, dataT>> InsertAux(std::shared_ptr<Node<keyT, dataT>> root, 
                                                            std::shared_ptr<Node<keyT, dataT>> toInsert); 
        static int GetBF(std::shared_ptr<Node<keyT, dataT>> node);
        static int GetHeight(std::shared_ptr<Node<keyT, dataT>> node);
        static std::shared_ptr<Node<keyT, dataT>> LLRotation(std::shared_ptr<Node<keyT, dataT>>& root);
        static std::shared_ptr<Node<keyT, dataT>> LRRotation(std::shared_ptr<Node<keyT, dataT>>& root);
        static std::shared_ptr<Node<keyT, dataT>> RLRotation(std::shared_ptr<Node<keyT, dataT>>& root);
        static std::shared_ptr<Node<keyT, dataT>> RRRotation(std::shared_ptr<Node<keyT, dataT>>& root);
        static std::shared_ptr<Node<keyT, dataT>> RemoveAux(std::shared_ptr<Node<keyT, dataT>>& root, const keyT& key);
        static std::shared_ptr<Node<keyT, dataT>> FindNextInOrder(std::shared_ptr<Node<keyT, dataT>> root);
        static void SaveInOrder(const std::shared_ptr<Node<keyT, dataT>> root, std::shared_ptr<keyT> *keyArr, std::shared_ptr<dataT> *dataArr, int *i);
        static void MergeArr(std::shared_ptr<keyT> *keyArr1, std::shared_ptr<keyT> *keyArr2, std::shared_ptr<dataT> *dataArr1,
                                std::shared_ptr<dataT> *dataArr2, std::shared_ptr<keyT> *keyMergedArr, std::shared_ptr<dataT> *dataMergedArr,
                                int size1, int size2);
        static void InsertElements(std::shared_ptr<Node<keyT, dataT>> root, std::shared_ptr<keyT> *keyArr,
                                   std::shared_ptr<dataT> *dataArr, int size, int *i);
        static BST<keyT, dataT> BuildEmptyTree(int n);
        static void removeRightLeafs(std::shared_ptr<Node<keyT, dataT>> root, int *removecount, int leafPathLen, int currPathLen);
        static int FindHeightOfComplete(int num);
        static std::shared_ptr<Node<keyT, dataT>> BuildCompleteTree(int h);
        static int ComputeSizeOfComplete(int height);
        static int IntMax(int a, int b);

    public:
        std::shared_ptr<Node<keyT, dataT>> root;
        int size;

        BST() : root(nullptr), size(0) {}
        BST(std::shared_ptr<Node<keyT, dataT>> root, int size) : root(root), size(size) {}
        ~BST() = default;
        std::shared_ptr<dataT> Get(const keyT& target);
        bool Find(const keyT& target);
        void Insert(const keyT key, std::shared_ptr<dataT>& data);
        void Remove(const keyT& key);
        static BST<keyT, dataT> Merge(const BST<keyT, dataT>& tree1, const BST<keyT, dataT>& tree2);
        dataT& GetMax();
        dataT& GetMin();
        static Map* MergeToArr(const BST<keyT, dataT>& tree1, const BST<keyT, dataT>& tree2);
        static BST<keyT, dataT> ArrToBST(Map* map, int size, int oldSize);  
        BST<keyT, dataT>& operator=(const BST<keyT, dataT>& copy);   
};

template <class keyT, class dataT>
int BST<keyT, dataT>::IntMax(int a, int b)
{
    return a > b ? a : b;
}

template <class keyT, class dataT>
BST<keyT, dataT>& BST<keyT, dataT>::operator=(const BST<keyT, dataT>& copy)
{
    this->root = copy.root;
    this->size = copy.size;
    return *this;
}     


template <class keyT, class dataT>
std::shared_ptr<dataT> BST<keyT, dataT>::Get(const keyT& target)
{
    std::shared_ptr<Node<keyT, dataT>> curr = root;
    while(curr != nullptr)
    {
        if(curr->key == target)
            return curr->data;
        
        if(curr->key < target)
            curr = curr->right;
        else
            curr = curr->left;
    }
    return nullptr;
}

template <class keyT, class dataT>
bool BST<keyT, dataT>::Find(const keyT& target)
{
    if(this->Get(target) == nullptr)
        return false;
    return true;
}

template <class keyT, class dataT>
void BST<keyT, dataT>::Insert(const keyT key, std::shared_ptr<dataT>& dataPtr)
{
    if(this->Find(key))
        return;
    
    std::shared_ptr<dataT> copyData = std::shared_ptr<dataT>(dataPtr); 
    auto toInsert = std::shared_ptr<Node<keyT, dataT>>(new Node<keyT, dataT>(key,copyData));

    this->root = BST<keyT, dataT>::InsertAux(this->root, toInsert);
    this->size++;
}

template <class keyT, class dataT>
int BST<keyT, dataT>::GetHeight(std::shared_ptr<Node<keyT, dataT>> node)
{
    if(node == nullptr)
        return -1;
    return node->height;
}

template <class keyT, class dataT>
int BST<keyT, dataT>::GetBF(std::shared_ptr<Node<keyT, dataT>> node)
{
    return BST<keyT, dataT>::GetHeight(node->left) - BST<keyT, dataT>::GetHeight(node->right);
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::LLRotation(std::shared_ptr<Node<keyT, dataT>>& root)
{
    std::shared_ptr<Node<keyT, dataT>> B = root;
    std::shared_ptr<Node<keyT, dataT>> A = B->left;
    std::shared_ptr<Node<keyT, dataT>> AL = A->left;
    std::shared_ptr<Node<keyT, dataT>> AR = A->right;

    B->left = AR;
    A->right = B;
    
    B->height = IntMax(GetHeight(B->left), GetHeight(B->right)) + 1;
    A->height = IntMax(GetHeight(A->left), GetHeight(A->right)) + 1;   
     
    return A;
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::LRRotation(std::shared_ptr<Node<keyT, dataT>>& root)
{
    std::shared_ptr<Node<keyT, dataT>> A = root->left;
    std::shared_ptr<Node<keyT, dataT>> B = A->right;
    std::shared_ptr<Node<keyT, dataT>> C = root;
    std::shared_ptr<Node<keyT, dataT>> BL = B->left;
    std::shared_ptr<Node<keyT, dataT>> BR = B->right;

    C->left = BR;
    A->right = BL;
    B->left = A;
    B->right = C;

    A->height = IntMax(GetHeight(A->left), GetHeight(A->right)) + 1;
    C->height = IntMax(GetHeight(C->left), GetHeight(C->right)) + 1;
    B->height = IntMax(GetHeight(B->left), GetHeight(B->right)) + 1;
    
    return B;
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::RLRotation(std::shared_ptr<Node<keyT, dataT>>& root)
{
    std::shared_ptr<Node<keyT, dataT>> A = root->right;
    std::shared_ptr<Node<keyT, dataT>> B = A->left;
    std::shared_ptr<Node<keyT, dataT>> C = root;
    std::shared_ptr<Node<keyT, dataT>> BL = B->left;
    std::shared_ptr<Node<keyT, dataT>> BR = B->right;

    C->right = BL;
    A->left = BR;
    B->right = A;
    B->left = C;

    A->height = IntMax(GetHeight(A->left), GetHeight(A->right)) + 1;
    C->height = IntMax(GetHeight(C->left), GetHeight(C->right)) + 1;
    B->height = IntMax(GetHeight(B->left), GetHeight(B->right)) + 1;

    return B;
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::RRRotation(std::shared_ptr<Node<keyT, dataT>>& root)
{
    std::shared_ptr<Node<keyT, dataT>> B = root;
    std::shared_ptr<Node<keyT, dataT>> A = B->right;
    std::shared_ptr<Node<keyT, dataT>> AL = A->left;
    std::shared_ptr<Node<keyT, dataT>> AR = A->right;

    B->right = AL;
    A->left = B;
    
    B->height = IntMax(GetHeight(B->left), GetHeight(B->right)) + 1;
    A->height = IntMax(GetHeight(A->left), GetHeight(A->right)) + 1;
    
    return A;
}


template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::InsertAux(std::shared_ptr<Node<keyT, dataT>> root, 
                                                            std::shared_ptr<Node<keyT, dataT>> toInsert)
{
    if (root == nullptr) 
        return toInsert;
    
    if (root->key < toInsert->key)
        root->right = BST<keyT, dataT>::InsertAux(root->right, toInsert);
    else
        root->left = BST<keyT, dataT>::InsertAux(root->left, toInsert);
    
    root->height = IntMax(GetHeight(root->left), GetHeight(root->right)) + 1;
    int balanceFactor = GetBF(root);

    if (balanceFactor == 2) {
        if (BST<keyT, dataT>::GetBF(root->left) >= 0)
            return BST<keyT, dataT>::LLRotation(root);
        else if (BST<keyT, dataT>::GetBF(root->left) == -1)
            return BST<keyT, dataT>::LRRotation(root);
    }
    else if (balanceFactor == -2) {
        if (BST<keyT, dataT>::GetBF(root->right) <= 0)
            return BST<keyT, dataT>::RRRotation(root);
        else if (BST<keyT, dataT>::GetBF(root->right) == 1)
            return BST<keyT, dataT>::RLRotation(root);
    }  
    return root;
}

template <class keyT, class dataT>
void BST<keyT, dataT>::Remove(const keyT& key)
{
    if(!this->Find(key))
        return;
    
    this->root = BST<keyT, dataT>::RemoveAux(this->root, key);
    this->size--;
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::RemoveAux(std::shared_ptr<Node<keyT, dataT>>& root, const keyT& key)
{
    if (root == nullptr)
        return nullptr;
    
    if (root->key < key)
        root->right = BST<keyT, dataT>::RemoveAux(root->right, key);
    else if (root->key > key)
        root->left = BST<keyT, dataT>::RemoveAux(root->left, key);
    
    else {
        if (!root->left && !root->right)
            root = nullptr;
        
        else if (root->left && root->right) {
            std::shared_ptr<Node<keyT, dataT>> leaf = BST<keyT, dataT>::FindNextInOrder(root);
            root->key = leaf->key;
            root->data = leaf->data;
            root->right = BST<keyT, dataT>::RemoveAux(root->right, leaf->key);
        }

        else if (root->left)
            root = root->left;
        else
            root = root->right;   
    }
    
    if (root == nullptr)
        return nullptr;
    
    root->height = IntMax(GetHeight(root->left), GetHeight(root->right)) + 1;
    int balanceFactor = GetBF(root);

    if (balanceFactor == 2) {
        if (BST<keyT, dataT>::GetBF(root->left) >= 0)
            return BST<keyT, dataT>::LLRotation(root);
        else if (BST<keyT, dataT>::GetBF(root->left) == -1)
            return BST<keyT, dataT>::LRRotation(root);
    }
    else if (balanceFactor == -2) {
        if (BST<keyT, dataT>::GetBF(root->right) <= 0)
            return BST<keyT, dataT>::RRRotation(root);
        else if (BST<keyT, dataT>::GetBF(root->right) == 1)
            return BST<keyT, dataT>::RLRotation(root);
    }  
    return root;
}

template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::FindNextInOrder(std::shared_ptr<Node<keyT, dataT>> root)
{
    root = root->right;
    while(root->left != nullptr)
        root = root->left;

    return root;
}

template <class keyT, class dataT>
void BST<keyT, dataT>::MergeArr(std::shared_ptr<keyT> *keyArr1, std::shared_ptr<keyT> *keyArr2, std::shared_ptr<dataT> *dataArr1,
                                std::shared_ptr<dataT> *dataArr2, std::shared_ptr<keyT> *keyMergedArr, std::shared_ptr<dataT> *dataMergedArr,
                                int size1, int size2)
{
    while (size1 > 0 && size2 > 0) {
        if (**keyArr1 < **keyArr2) {
            *keyMergedArr = std::shared_ptr<keyT>(*keyArr1);
            *dataMergedArr = std::shared_ptr<dataT>(*dataArr1);
            keyArr1++;
            dataArr1++;
            size1--;
        }
        else {
            *keyMergedArr = std::shared_ptr<keyT>(*keyArr2);
            *dataMergedArr = std::shared_ptr<dataT>(*dataArr2);
            keyArr2++;
            dataArr2++;
            size2--;
        }
        keyMergedArr++;
        dataMergedArr++;
    }

    while (size1 > 0) {    
        *keyMergedArr = std::shared_ptr<keyT>(*keyArr1);
        *dataMergedArr = std::shared_ptr<dataT>(*dataArr1);
        keyArr1++;
        dataArr1++;
        keyMergedArr++;
        dataMergedArr++;
        size1--;
    }
    
    while (size2 > 0) {    
        *keyMergedArr = std::shared_ptr<keyT>(*keyArr2);
        *dataMergedArr = std::shared_ptr<dataT>(*dataArr2);
        keyArr2++;
        dataArr2++;
        keyMergedArr++;
        dataMergedArr++;
        size2--;
    }
}

template <class keyT, class dataT>
void BST<keyT, dataT>::SaveInOrder(const std::shared_ptr<Node<keyT, dataT>> root,
                                     std::shared_ptr<keyT> *keyArr, std::shared_ptr<dataT> *dataArr, int *i)
{
    if(root == nullptr)
        return;
    BST<keyT, dataT>::SaveInOrder(root->left, keyArr, dataArr, i);
    keyArr[*i] = std::make_shared<keyT>(keyT(root->key));
    dataArr[*i] = std::shared_ptr<dataT>(root->data);
    (*i)++;
    BST<keyT, dataT>::SaveInOrder(root->right, keyArr, dataArr, i);
}

template <class keyT, class dataT>
BST<keyT, dataT> BST<keyT, dataT>::Merge(const BST<keyT, dataT>& tree1, const BST<keyT, dataT>& tree2)
{
    Map* map = BST<keyT, dataT>::MergeToArr(tree1, tree2);
    BST<keyT, dataT> mergedBST = BST<keyT, dataT>::BuildEmptyTree(tree1.size + tree2.size);
    int i = 0;
    BST<keyT, dataT>::InsertElements(mergedBST.root, (std::shared_ptr<keyT> *)(map->key), (std::shared_ptr<dataT> *)(map->data), tree1.size + tree2.size, &i);
    delete[] (std::shared_ptr<keyT> *)(map->key);
    delete[] (std::shared_ptr<dataT> *)(map->data);
    MapDestroy(map);
    return mergedBST;   
}

template <class keyT, class dataT>
Map* BST<keyT, dataT>::MergeToArr(const BST<keyT, dataT>& tree1, const BST<keyT, dataT>& tree2)
{
    std::shared_ptr<dataT> *dataArr1 = new std::shared_ptr<dataT>[tree1.size];
    std::shared_ptr<dataT> *dataArr2 = new std::shared_ptr<dataT>[tree2.size];
    std::shared_ptr<dataT> *dataMergedArr = new std::shared_ptr<dataT>[tree1.size + tree2.size];
    std::shared_ptr<keyT> *keyArr1 = new std::shared_ptr<keyT>[tree1.size];
    std::shared_ptr<keyT> *keyArr2 = new std::shared_ptr<keyT>[tree2.size];
    std::shared_ptr<keyT> *keyMergedArr = new std::shared_ptr<keyT>[tree1.size + tree2.size];
    int i = 0;
    SaveInOrder(tree1.root, keyArr1, dataArr1, &i);
    i = 0;
    SaveInOrder(tree2.root, keyArr2, dataArr2, &i);
    MergeArr(keyArr1, keyArr2, dataArr1, dataArr2, keyMergedArr, dataMergedArr, tree1.size, tree2.size);
    delete[] dataArr1;
    delete[] dataArr2;
    delete[] keyArr1;
    delete[] keyArr2;
    Map* map = MapCreate((void*)keyMergedArr, (void*)dataMergedArr);
    return map;  
}

template <class keyT, class dataT>
BST<keyT, dataT> BST<keyT, dataT>::ArrToBST(Map* map, int size, int oldSize)
{
    BST<keyT, dataT> mergedBST = BST<keyT, dataT>::BuildEmptyTree(size);
    int i = 0;
    BST<keyT, dataT>::InsertElements(mergedBST.root, (std::shared_ptr<keyT> *)(map->key), (std::shared_ptr<dataT> *)(map->data), oldSize, &i);
    delete[] (std::shared_ptr<keyT> *)(map->key);
    delete[] (std::shared_ptr<dataT> *)(map->data);

    return mergedBST;   
}


template <class keyT, class dataT>
void BST<keyT, dataT>::InsertElements(std::shared_ptr<Node<keyT, dataT>> root, std::shared_ptr<keyT> *keyArr,
                                      std::shared_ptr<dataT> *dataArr, int size, int *i)
{
    if(root == nullptr)
        return;

    BST<keyT, dataT>::InsertElements(root->left, keyArr, dataArr, size, i);
    
    while (*i < size && dataArr[*i] == nullptr) {
        (*i)++;
    }
    root->data = dataArr[*i];
    root->key = *(keyArr[*i]);
    (*i)++;
    BST<keyT, dataT>::InsertElements(root->right, keyArr, dataArr, size, i);

    return;
}

template <class keyT, class dataT>
BST<keyT, dataT> BST<keyT, dataT>::BuildEmptyTree(int n)
{
    int completeHeight = BST<keyT, dataT>::FindHeightOfComplete(n + 1);
    int completeSize = BST<keyT, dataT>::ComputeSizeOfComplete(completeHeight);
    BST<keyT, dataT> res = BST<keyT, dataT>(BST<keyT, dataT>::BuildCompleteTree(completeHeight), completeSize);
    int removeCount = completeSize - n;
    removeRightLeafs(res.root, &removeCount, completeHeight, 0);

    res.size = n;
    return res;
}

template <class keyT, class dataT>
void BST<keyT, dataT>::removeRightLeafs(std::shared_ptr<Node<keyT, dataT>> root, int* removecount, int leafPathLen, int currPathLen)
{
    if(root == nullptr || *removecount == 0)
        return;

    BST<keyT, dataT>::removeRightLeafs(root->right, removecount, leafPathLen, currPathLen + 1);

    if (currPathLen + 1 == leafPathLen) {
        root->right = nullptr;
        (*removecount)--;
    }
    if (currPathLen + 1 == leafPathLen && *removecount > 0) {
        root->left = nullptr;
        (*removecount)--;
        root->height--;
    }

    BST<keyT, dataT>::removeRightLeafs(root->left, removecount, leafPathLen, currPathLen + 1);
    return;
}


template <class keyT, class dataT>
std::shared_ptr<Node<keyT, dataT>> BST<keyT, dataT>::BuildCompleteTree(int h)
{
    if (h == -1)
        return nullptr;
    std::shared_ptr<Node<keyT, dataT>> root = std::shared_ptr<Node<keyT, dataT>>(new Node<keyT, dataT>(h));
    root->right = BST<keyT, dataT>::BuildCompleteTree(h - 1);
    root->left = BST<keyT, dataT>::BuildCompleteTree(h - 1);
    return root;
}

template <class keyT, class dataT>
int BST<keyT, dataT>::FindHeightOfComplete(int num)
{
    int twoPow = 1;
    int height = -1;
    while(twoPow < num)
    {
        twoPow *= 2;
        height++;
    }
    return height;
}

template <class keyT, class dataT>
int BST<keyT, dataT>::ComputeSizeOfComplete(int height)
{
    int n = 1;
    int count = 1;

    while(count <= height + 1) {
        n *= 2;
        count++;
    }

    return n - 1;
}

template <class keyT, class dataT>
dataT& BST<keyT, dataT>::GetMax()
{
    std::shared_ptr<Node<keyT, dataT>> curr = this->root;
    while(curr->right != nullptr)
        curr = curr->right;

    return *(curr->data);
}

template <class keyT, class dataT>
dataT& BST<keyT, dataT>::GetMin()
{
    std::shared_ptr<Node<keyT, dataT>> curr = this->root;
    while(curr->left != nullptr)
        curr = curr->left;

    return *(curr->data);
}

#endif /* BST_H */
