#ifndef NODE_H
#define NODE_H

#include <bits/stdc++.h>
using namespace std;

template<typename T> class BTree;

template <typename T>
class Node {
        unsigned int size;
        unsigned int currentNumberOfKeys;
        std::vector< unsigned int > keys;
        std::vector< Node<T>* > children;
        bool isLeaf;
public:
        explicit Node(unsigned int size, bool isLeaf = true) : size(size), isLeaf(isLeaf), currentNumberOfKeys(0) {
            this->keys.resize(size - 1);
            this->children.resize(size);
        }


        bool search(T k){
            int i = 0;

            while(i < this->currentNumberOfKeys && this->keys[i] < k) {
                ++i;
            }
            if(this->keys[i] == k) {
                return true;
            }
            if(this->isLeaf) {
                return false;
            }

            return this->children[i]->search(k);
        }

        /*
         * An alternative is to create two different nodes (Internal and Leaf) that inherite from Node 
         * an implement this function
         */
        //virtual bool isLeaf() = 0;

        void insert(T k){
            int index = this->currentNumberOfKeys;
            if(this->isLeaf){
                while(index > 0 && k < this->keys[index - 1]){
                    this->keys[index] = this->keys[index - 1];
                    --index;
                }

                this->keys[index] = k;
                ++this->currentNumberOfKeys;
            }
            else{
                while(index > 0 && k < this->keys[index - 1]){
                    --index;
                }

                if(this->children[index]->currentNumberOfKeys == this->size - 1){
                    splitChild(index, this->children[index]);
                    if(k > this->keys[index]){
                        ++index;
                    }
                }

                this->children[index]->insert(k);
            }
        }


        void splitChild(int index, Node<T>* nodeToSplit) {
            auto newNode = new Node<T>(this->size, nodeToSplit->isLeaf);
            int m = (this->size - 1) / 2;

            newNode->currentNumberOfKeys = m - (this->size % 2);

            for (int i = 0; i < newNode->currentNumberOfKeys; ++i) {
                newNode->keys[i] = nodeToSplit->keys[m + 1 + i];
            }

            if (!nodeToSplit->isLeaf) {
                for (int i = 0; i <= newNode->currentNumberOfKeys; ++i) {
                    newNode->children[i] = nodeToSplit->children[m + 1 + i];
                }
            }

            nodeToSplit->currentNumberOfKeys = m;

            for (int i = this->currentNumberOfKeys + 1; i > index + 1; --i) {
                this->children[i] = this->children[i-1];
            }

            this->children[index + 1] = newNode;

            for(int i = this->currentNumberOfKeys; i > index; --i){
                this->keys[i] = this->keys[i - 1];
            }

            this->keys[index] = nodeToSplit->keys[m];
            ++this->currentNumberOfKeys;
        }


        void print() {
            int i = 0;
            for (i = 0; i < this->currentNumberOfKeys; ++i)
            {
                if (!this->isLeaf) {
                    this->children[i]->print();
                }
                std::cout << this->keys[i] << " ";
            }
            if (!this->isLeaf) {
                this->children[i]->print();
            }
        }


        void killSelf() {
            for(auto && child: this->children)
            {
                if (child) {
                    child->killSelf();
                }
            }
        }

    friend class BTree<T>;
};

#endif