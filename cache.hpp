//cache implementation for web applications in cpp
#include <string>
#include <mutex>
#include <iostream>
#include <time.h>
#include "../dict/dict.hpp"

template<class Value>
class Node{
public:
    Node(Value _value){
        value = _value;
        entry = time(0);
    }
    Value value;
    time_t entry;
};

template<class Value>
class Cache{
    public:
        Cache(int _time);
        void add(string key, Value val);
        Value get(string key);
        void pop(string key);
        bool find(string key);
    private:
        mutex mu; //used for thread safe operations, and not accesing at the same time one node;
        void tidy(); //update the dict depending on the current time;
        int time; //time a value is in the cache 
        dict<string, Node<Value>*> *data; //were the data is stored;
};

template<class Value>
Cache<Value>::Cache(int _time){
    time = _time;
    data = new dict<string, Node<Value>*>(50);
}

template<class Value>
bool Cache<Value>::find(string key){
    tidy();
    return data->find(key);
}

template<class Value>
void Cache<Value>::add(string key, Value val){
    tidy();
    Node<Value> *newnode = new Node<Value>(val);
    mu.lock();
    data->add(key, newnode);
    mu.unlock();
}

template<class Value>
Value Cache<Value>::get(string key){
    tidy();
    mu.lock();
    Node<Value> *result = data->get(key);
    mu.unlock();
    return result->value;
}

template<class Value>
void Cache<Value>::pop(string key){
    tidy();
    mu.lock();
    data->pop(key);
    mu.unlock();
}

template<class Value>
void Cache<Value>::tidy(){
    DictListNode<string, Node<Value>*> *iter = data->nextNode();
    mu.lock();
    int diff;
    while(iter != NULL){
        diff = difftime(std::time(0), iter->value->entry);
        if(diff > time){
            data->pop(iter->key);
        }
        iter = data->nextNode();
    }
    mu.unlock();
}