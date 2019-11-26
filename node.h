#ifndef NODE_H
#define NODE_H

#include <bits/stdc++.h>
using namespace std;

template<typename T> class BTree;

template <typename T>
class Node {
private:
        unsigned int capacity;
        unsigned int size;
        std::vector< unsigned int > keys;
        std::vector< Node<T>* > children;
        bool isLeaf;

private:
        int findKey(T k){
            int idx = 0;
            while(idx < this->size && this->keys[idx] < k){
                ++idx;
            }
            return idx;
        }

        void removeFromLeaf(int idx){
            for(int i = idx; i < this->size - 1; ++i){
                this->keys[i] = this->keys[i + 1];
            }
            --this->size;
        }

        void removeFromNonLeaf(int idx){
            auto k = this->keys[idx];

            if(this->children[idx]->size >= (this->capacity + 1) / 2){
                int newKey = getPreviousKey(idx);
                this->keys[idx] = newKey;
                this->children[idx]->remove(newKey);
            }
            else if(this->children[idx + 1]->size >= (this->capacity + 1) / 2){
                int newKey = getNextKey(idx);
                this->keys[idx] = newKey;
                this->children[idx + 1]->remove(newKey);
            }
            else{
                this->mergeNodes(idx);
                this->children[idx]->remove(k);
            }

        }

        int getPreviousKey(int idx){
            auto current = this->children[idx];

            while(!current->isLeaf){
                current = current->children[0];
            }

            return current->keys[0];
        }

        int getNextKey(int idx){
            auto current = this->children[idx + 1];

            while(!current->isLeaf){
                current = current->children[current->size];
            }

            return current->keys[current->size - 1];
        }

        void fillNode(int idx) {
            int m = std::ceil((this->capacity - 1) / 2);

            if(idx && this->children[idx - 1]->size > m){
                this->borrowPrev(idx);
            }

            else if(idx != this->size && this->children[idx + 1]->size > m){
                this->borrowNext(idx);
            }

            else{
                if(idx == this->size){
                    this->mergeNodes(idx - 1);
                }
                else{
                    this->mergeNodes(idx);
                }
            }
        }

        void borrowPrev(int idx) {
            int m = std::ceil((this->capacity - 1) / 2);

            auto current = this->children[idx];
            auto toBorrow = this->children[idx - 1];

            for(int i = current->size; i > 0; --i){
                current->keys[i] = current->keys[i - 1];
            }

            if(!current->isLeaf){
                for(int i = current->size + 1; i > 0; --i){
                    current->children[i] = current->children[i - 1];
                }
            }

            current->keys[0] = this->keys[idx - 1];

            if(!toBorrow->isLeaf){
                current->children[0] = toBorrow->children[toBorrow->size];
            }

            this->keys[idx - 1] = toBorrow->keys[toBorrow->size - 1];

            ++current->size;
            --toBorrow->size;
        }

        void borrowNext(int idx) {
            int m = std::ceil((this->capacity - 1) / 2);

            auto current = this->children[idx];
            auto toBorrow = this->children[idx + 1];

            current->keys[current->size] = this->keys[idx];

            if(!toBorrow->isLeaf){
                current->children[current->size + 1] = toBorrow->children[0];
            }

            this->keys[idx] = toBorrow->keys[0];

            for(int i = 1; i < toBorrow->size; ++i){
                toBorrow->keys[i - 1] = toBorrow->keys[i];
            }

            if(!current->isLeaf){
                for(int i = 1; i < toBorrow->size + 1; ++i){
                    toBorrow->children[i - 1] = toBorrow->children[i];
                }
            }

            ++current->size;
            --toBorrow->size;
        }

        void mergeNodes(int idx){
            int m = std::ceil((this->capacity - 1) / 2);

            auto current = this->children[idx];
            auto toMerge = this->children[idx + 1];

            current->keys[m] = this->keys[idx];

            for(int i = 0; i < toMerge->size; ++i){
                current->keys[m + 1 + i] = toMerge->keys[i];
            }

            if (!toMerge->isLeaf) {
                for (int i = 0; i <= toMerge->size; ++i) {
                    current->children[m + 1 + i] = toMerge->children[i];
                }
            }

            for (int i = idx; i < this->size - 1; ++i) {
                this->keys[i] = this->keys[i + 1];
            }

            for (int i = idx + 1; i < this->size; ++i) {
                this->children[i] = this->children[i + 1];
            }

            current->size += 1 + toMerge->size;
            --this->size;
//            delete toMerge;
        }

public:
        explicit Node(unsigned int capacity, bool isLeaf = true) : capacity(capacity), isLeaf(isLeaf), size(0) {
            this->keys.resize(capacity - 1);
            this->children.resize(capacity);
        }


        bool search(T k){
            int i = 0;

            while(i < this->size && k > this->keys[i]) {
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
            int index = this->size;
            if(this->isLeaf){
                while(index > 0 && k < this->keys[index - 1]){
                    this->keys[index] = this->keys[index - 1];
                    --index;
                }

                this->keys[index] = k;
                ++this->size;
            }
            else{
                while(index > 0 && k < this->keys[index - 1]){
                    --index;
                }

                if(this->children[index]->size == this->capacity - 1){
                    this->splitChild(index, this->children[index]);
                    if(k > this->keys[index]){
                        ++index;
                    }
                }

                this->children[index]->insert(k);
            }
        }

        void splitChild(int index, Node<T>* nodeToSplit) {
            auto newNode = new Node<T>(this->capacity, nodeToSplit->isLeaf);
            int m = std::ceil((this->capacity - 1) / 2);

            newNode->size = m - (this->capacity % 2);

            for (int i = 0; i < newNode->size; ++i) {
                newNode->keys[i] = nodeToSplit->keys[m + 1 + i];
            }

            if (!nodeToSplit->isLeaf) {
                for (int i = 0; i <= newNode->size; ++i) {
                    newNode->children[i] = nodeToSplit->children[m + 1 + i];
                }
            }

            nodeToSplit->size = m;

            for (int i = this->size + 1; i > index + 1; --i) {
                this->children[i] = this->children[i-1];
            }

            this->children[index + 1] = newNode;

            for(int i = this->size; i > index; --i){
                this->keys[i] = this->keys[i - 1];
            }

            this->keys[index] = nodeToSplit->keys[m];
            ++this->size;
        }

        void remove(T k){
            int idx = this->findKey(k);

            if (idx < this->size && this->keys[idx] == k) {
                if (this->isLeaf) { this->removeFromLeaf(idx); }
                else { this->removeFromNonLeaf(idx); }
            }
            else {
                if (this->children[idx]->size < std::ceil((this->capacity + 1) / 2)) {
                    this->fillNode(idx);
                }

                if (idx > this->size) { this->children[idx - 1]->remove(k); }
                else{ this->children[idx]->remove(k); }
            }
        }

        void print() {
            int i = 0;
            for (i = 0; i < this->size; ++i)
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

        /* BEGIN OF ACCESES */
        unsigned int getSize() { return this->capacity; }
        unsigned int getCurrentNumOfKeys() { return this->size; }
        std::vector< unsigned int > getKeys() { return this->keys; }
        std::vector< Node<T>* > getChildren() { return this->children; }
        bool getIsLeaf() { return this->isLeaf; }
        /* END OF ACCESES */

    friend class BTree<T>;
};

#endif