#ifndef NODE_H
#define NODE_H

#include <bits/stdc++.h>
using namespace std;

template<typename T> class BTree;

template <typename T>
class Node {
private:
        unsigned int degree;
        unsigned int nKey;
        std::vector< unsigned int > keys;
        std::vector< Node<T>* > children;
        bool isLeaf;

private:
        int findKey(T data){
            int idx = 0;
            while(idx < this->nKey && this->keys[idx] < data){
                ++idx;
            }
            return idx;
        }

        void removeFromLeaf(int idx){
            for(int i = idx; i < this->nKey - 1; ++i){
                this->keys[i] = this->keys[i + 1];
            }
            --this->nKey;
        }

        void removeFromNonLeaf(int idx){
            auto k = this->keys[idx];

            if(this->children[idx]->nKey >= (this->degree + 1) / 2){
                int newKey = getPreviousKey(idx);
                this->keys[idx] = newKey;
                this->children[idx]->remove(newKey);
            }
            else if(this->children[idx + 1]->nKey >= (this->degree + 1) / 2){
                int newKey = getNextKey(idx);
                this->keys[idx] = newKey;
                this->children[idx + 1]->remove(newKey);
            }
            else{
                this->mergeNodes(idx);
                this->children[idx]->remove(k);
            }

        }

        T getPreviousKey(T idx){
            auto current = this->children[idx];

            while(!current->isLeaf){
                current = current->children[0];
            }

            return current->keys[0];
        }

        T getNextKey(T idx){
            auto current = this->children[idx + 1];

            while(!current->isLeaf){
                current = current->children[current->nKey];
            }

            return current->keys[current->nKey - 1];
        }

        void fillNode(int idx) {
            int m = std::ceil((this->degree - 1) / 2);

            if(idx && this->children[idx - 1]->nKey > m){
                this->borrowFromPrev(idx);
            }

            else if(idx != this->nKey && this->children[idx + 1]->nKey > m){
                this->borrowFromNext(idx);
            }

            else {
                if (idx == this->nKey) {
                    this->mergeNodes(idx - 1);
                }
                else {
                    this->mergeNodes(idx);
                }
            }
        }

        void borrowFromPrev(int idx) {
            int m = std::ceil((this->degree - 1) / 2);

            auto current = this->children[idx];
            auto sibling = this->children[idx - 1];

            for(int i = current->nKey; i > 0; --i){
                current->keys[i] = current->keys[i - 1];
            }

            if(!current->isLeaf){
                for(int i = current->nKey + 1; i > 0; --i){
                    current->children[i] = current->children[i - 1];
                }
            }

            current->keys[0] = this->keys[idx - 1];

            if(!sibling->isLeaf){
                current->children[0] = sibling->children[sibling->nKey];
            }

            this->keys[idx - 1] = sibling->keys[sibling->nKey - 1];

            ++current->nKey;
            --sibling->nKey;
        }

        void borrowFromNext(int idx) {
            int m = std::ceil((this->degree - 1) / 2);

            auto current = this->children[idx];
            auto sibling = this->children[idx + 1];

            current->keys[current->nKey] = this->keys[idx];

            if(!sibling->isLeaf){
                current->children[current->nKey + 1] = sibling->children[0];
            }

            this->keys[idx] = sibling->keys[0];

            for(int i = 1; i < sibling->nKey; ++i){
                sibling->keys[i - 1] = sibling->keys[i];
            }

            if(!current->isLeaf){
                for(int i = 1; i < sibling->nKey + 1; ++i){
                    sibling->children[i - 1] = sibling->children[i];
                }
            }

            ++current->nKey;
            --sibling->nKey;
        }

        void mergeNodes(int idx){
            int m = std::ceil((this->degree - 1) / 2);

            auto current = this->children[idx];
            auto sibling = this->children[idx + 1];

            current->keys[m] = this->keys[idx];

            for(int i = 0; i < sibling->nKey; ++i){
                current->keys[m + 1 + i] = sibling->keys[i];
            }

            if (!sibling->isLeaf) {
                for (int i = 0; i <= sibling->nKey; ++i) {
                    current->children[m + 1 + i] = sibling->children[i];
                }
            }

            for (int i = idx; i < this->nKey - 1; ++i) {
                this->keys[i] = this->keys[i + 1];
            }

            for (int i = idx + 1; i < this->nKey; ++i) {
                this->children[i] = this->children[i + 1];
            }

            current->nKey += 1 + sibling->nKey;
            --this->nKey;
        }

public:
        explicit Node(unsigned int degree, bool isLeaf = true) : degree(degree), isLeaf(isLeaf), nKey(0) {
            this->keys.resize(degree - 1);
            this->children.resize(degree);
        }


        bool search(T data){
            int i = 0;

            while(i < this->nKey && data > this->keys[i]) {
                ++i;
            }
            if(this->keys[i] == data) {
                return true;
            }
            if(this->isLeaf) {
                return false;
            }

            return this->children[i]->search(data);
        }

        /*
         * An alternative is to create two different nodes (Internal and Leaf) that inherited from Node
         * an implement this function
         */
        //virtual bool isLeaf() = 0;

        void insert(T data){
            int index = this->nKey;
            if(this->isLeaf){
                while(index > 0 && data < this->keys[index - 1]){
                    this->keys[index] = this->keys[index - 1];
                    --index;
                }

                this->keys[index] = data;
                ++this->nKey;
            }
            else{
                while(index > 0 && data < this->keys[index - 1]){
                    --index;
                }

                if(this->children[index]->nKey == this->degree - 1){
                    this->splitChild(index, this->children[index]);
                    if(data > this->keys[index]){
                        ++index;
                    }
                }

                this->children[index]->insert(data);
            }
        }

        void splitChild(int index, Node<T>* nodeToSplit) {
            auto newNode = new Node<T>(this->degree, nodeToSplit->isLeaf);
            int m = std::ceil((this->degree - 1) / 2);

            newNode->nKey = m - (this->degree % 2);

            for (int i = 0; i < newNode->nKey; ++i) {
                newNode->keys[i] = nodeToSplit->keys[m + 1 + i];
            }

            if (!nodeToSplit->isLeaf) {
                for (int i = 0; i <= newNode->nKey; ++i) {
                    newNode->children[i] = nodeToSplit->children[m + 1 + i];
                }
            }

            nodeToSplit->nKey = m;

            for (int i = this->nKey + 1; i > index + 1; --i) {
                this->children[i] = this->children[i-1];
            }

            this->children[index + 1] = newNode;

            for(int i = this->nKey; i > index; --i){
                this->keys[i] = this->keys[i - 1];
            }

            this->keys[index] = nodeToSplit->keys[m];
            ++this->nKey;
        }

        void remove(T data){
            int index = this->findKey(data);

            if (index < this->nKey && this->keys[index] == data) {
                if (this->isLeaf) { this->removeFromLeaf(index); }
                else { this->removeFromNonLeaf(index); }
            }
            else {
                if (this->children[index]->nKey < std::ceil((this->degree + 1) / 2)) {
                    this->fillNode(index);
                }

                if (index > this->nKey) { this->children[index - 1]->remove(data); }
                else{ this->children[index]->remove(data); }
            }
        }

        void print() {
            int i = 0;
            for (i = 0; i < this->nKey; ++i)
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
            delete this;
        }

    friend class BTree<T>;
};

#endif