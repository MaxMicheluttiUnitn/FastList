#ifndef __FAST_LIST_H__
#define __FAST_LIST_H__
#endif
using namespace std;
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <utility>
#include <random>

/**
 * Heuristics:
 * 
 * when pushing longo full FastList -> multiply size by 3/2
 * 
 * when deleting from FastList -> shrink to 2/3 of original size when under 1/2 of used storage
 * 
 * 
*/

const long MIN_SIZE = 16;

const long MAX_SIZE = 1024*1024;

class UnreachableException{
    string msg;
    
    public:
    UnreachableException(string msg){
        this->msg=msg;
    }
};

class EmptyListException{
    string msg;
    
    public:
    EmptyListException(string msg){
        this->msg=msg;
    }
};

class MemoryException{
    string msg;
    
    public:
    MemoryException(string msg){
        this->msg=msg;
    }
};

class ItemNotFoundException{
    string msg;
    
    public:
    ItemNotFoundException(string msg){
        this->msg=msg;
    }
};

class IndexOutOfBoundsException{
    string msg;
    
    public:
    IndexOutOfBoundsException(string msg){
        this->msg=msg;
    }
};

class NullPolongerDerefException{
    string msg;
    
    public:
    NullPolongerDerefException(string msg){
        this->msg=msg;
    }
};

class EmptyListRemovalexception{
    string msg;
    
    public:
    EmptyListRemovalexception(string msg){
        this->msg=msg;
    }
};

template <typename T> class FastList;
template <typename T> ostream & operator<<(ostream& os, FastList<T> const & list);
template <typename T> ostream & operator<<(ostream& os, FastList<FastList<T>*> const & list);

template <typename T> class FastIter{
    private: long current;
    private: FastList<T>* list;

    public:
    FastIter(FastList<T>* list){
        this->current = 0;
        this->list = list;
    }

    FastIter(FastList<T>* list, long starting_index){
        this->current = starting_index;
        this->list = list;
    }

    ~FastIter(){}

    FastIter<T>& operator++(){
        if(this->current >= this->list->getLength()){
            throw NullPolongerDerefException("FastIter++");
        }
        this->current++;
        return *this;
    }

    FastIter<T>& operator++(int){
        if(this->current >= this->list->getLength()){
            throw NullPolongerDerefException("FastIter++");
        }
        this->current++;
        return *this;
    }

    friend bool operator==(FastIter<T>const& a,FastIter<T>const& b){
        return a.current==b.current && a.list==b.list;
    }

    friend bool operator!=(FastIter<T>const& a,FastIter<T>const& b){
        return ! (a == b);
    }

    T& operator*(){
        return this->list->getReference(this->current);
    }
};

template <typename T> class FastIterConst{
    private: long current;
    private: const FastList<T>* list;

    public:
    FastIterConst(const FastList<T>* list){
        this->current = 0;
        this->list = list;
    }

    FastIterConst(const FastList<T>* list, long starting_index){
        this->current = starting_index;
        this->list = list;
    }

    ~FastIterConst(){}

    FastIterConst<T>& operator++(){
        if(this->current >= this->list->getLength()){
            throw NullPolongerDerefException("FastIterConst++");
        }
        this->current++;
        return *this;
    }

    FastIterConst<T>& operator++(int){
        if(this->current >= this->list->getLength()){
            throw NullPolongerDerefException("FastIterConst++");
        }
        this->current++;
        return *this;
    }

    friend bool operator==(FastIterConst<T>const& a,FastIterConst<T>const& b){
        return a.current==b.current && a.list==b.list;
    }

    friend bool operator!=(FastIterConst<T>const& a,FastIterConst<T>const& b){
        return ! (a == b);
    }

    T operator*(){
        return this->list->get(this->current);
    }
};

template <typename T> class FastIterReverse{
    private: long current;
    private: FastList<T>* list;

    public:
    FastIterReverse(FastList<T>* list){
        this->current = list->getLength()-1;
        this->list = list;
    }

    FastIterReverse(FastList<T>* list, long starting_index){
        this->current = starting_index;
        this->list = list;
    }

    ~FastIterReverse(){}

    FastIterReverse<T>& operator++(){
        if(this->current < 0){
            throw NullPolongerDerefException("FastIter++");
        }
        this->current--;
        return *this;
    }

    FastIterReverse<T>& operator++(int){
        if(this->current < 0){
            throw NullPolongerDerefException("FastIter++");
        }
        this->current--;
        return *this;
    }

    friend bool operator==(FastIterReverse<T>const& a,FastIterReverse<T>const& b){
        return a.current==b.current && a.list==b.list;
    }

    friend bool operator!=(FastIterReverse<T>const& a,FastIterReverse<T>const& b){
        return ! (a == b);
    }

    T& operator*(){
        return this->list->getReference(this->current);
    }
};

template <typename T> class FastIterConstReverse{
    private: long current;
    private: const FastList<T>* list;

    public:
    FastIterConstReverse(const FastList<T>* list){
        this->current = list->getLength()-1;
        this->list = list;
    }

    FastIterConstReverse(const FastList<T>* list, long starting_index){
        this->current = starting_index;
        this->list = list;
    }

    ~FastIterConstReverse(){}

    FastIterConstReverse<T>& operator++(){
        if(this->current < 0){
            throw NullPolongerDerefException("FastIterConst++");
        }
        this->current--;
        return *this;
    }

    FastIterConstReverse<T>& operator++(int){
        if(this->current < 0){
            throw NullPolongerDerefException("FastIterConst++");
        }
        this->current--;
        return *this;
    }

    friend bool operator==(FastIterConstReverse<T>const& a,FastIterConstReverse<T>const& b){
        return a.current==b.current && a.list==b.list;
    }

    friend bool operator!=(FastIterConstReverse<T>const& a,FastIterConstReverse<T>const& b){
        return ! (a == b);
    }

    T operator*(){
        return this->list->get(this->current);
    }
};

template <typename T> class FastList{
    private: T* array;
    private: long size;
    private: long length;
    private: mt19937 generator;

    private:

    /**
     * Quicksort implementation
    */
    void quick_sort(long start,long end){
        if(start<end){
            long pivot = partition(start,end);
            quick_sort(start,pivot-1);
            quick_sort(pivot+1,end);
        }   
    }
    long partition(long start,long end){
        long mid = (start + end) / 2;
        this->swap(start,mid);
        long pivot_index = start;
        long pivot_value = this->array[start];

        long scan = start;
        while(++scan <= end){
            if(this->array[scan] < pivot_value)
                this->swap(++pivot_index,scan);
        }
        this->swap(start,pivot_index);
        return pivot_index;
    }

    /**
     * Swaps two nodes in the list
    */
    void swap(long id1,long id2){
        if(id1==id2)
            return;
        if(id1<0 || id1>=this->getLength() || id2<0 || id2>=this->getLength())
            throw IndexOutOfBoundsException("swap");
        T temp = this->array[id1];
        this->array[id1] = this->array[id2];
        this->array[id2] = temp;
    }

    void shuffle_from_generator(std::mt19937 gen){
        long len = this->getLength();
        for(long index=0;index<len-1;index++){
            std::uniform_int_distribution<> distrib(index,len-1);
            long swapped = distrib(gen);
            this->swap(index,swapped);
        }
    }

    public:
    FastList(){
        this->array = new T[MIN_SIZE];
        if(this->array==nullptr)
            throw MemoryException("constructor");
        this->size = MIN_SIZE;
        this->length = 0;
        this->generator = mt19937((unsigned)time(nullptr));
    }

    FastList(long size){
        this->size = max(MIN_SIZE,size);
        this->array = new T[this->size];
        if(this->array == nullptr)
            throw MemoryException("constructor");
        this->length = 0;
        this->generator = mt19937((unsigned)time(nullptr));
    }

    FastList(const FastList& list){
        this->size = max(MIN_SIZE,list.size);
        this->length = list.length;
        this->array = new T[size];
        if(this->array == nullptr)
            throw MemoryException("constructor");
        this->generator = mt19937((unsigned)time(nullptr));
        for(long i=0;i<this->length;i++){
            this->array[i]=T(list.get(i));
        }
    }

    FastList(FastList<T>&& list){
        this->size = list.size;
        this->length = list.length;
        this->array = list.array;
        list.array = nullptr;
    }

    FastList(std::initializer_list<T> init){
        this->generator = mt19937((unsigned)time(nullptr));
        this->size=MIN_SIZE;
        this->length=0;
        this->array=new T[MIN_SIZE];
        if(this->array == nullptr)
            throw MemoryException("constructor");
        for(auto iter = init.begin();iter!=init.end();iter++){
            this->push(*iter);
        }
    }

    ~FastList(){
        if(this->array!=nullptr)
            delete[] array;
    }

    FastList<T>& operator=(const FastList& list){
        FastList<T> new_list = FastList<T>(list);
        (*this) = move(new_list);
        return (*this);
    }

    FastList<T>& operator=(FastList&& list){
        this->array = list.array;
        this->size = list.size;
        this->length = list.length;
        list.array = nullptr;
        return *this;
    }

    /**
     * Sort the list
    */
    void sort(){
        if(this->getLength()<=1)
            return;
        this->quick_sort(0,this->getLength()-1);
    }

    /**
     * returns a list that is the same as the initial one, but sorted, without modifying the original list
    */
    FastList<T> getSorted(){
        FastList<T> res = FastList(*this);
        res.sort();
        return res;
    }

    /**
     * returns a polonger to a list that is the same as the initial one, but sorted, without modifying the original list
    */
    FastList<T>* getSortedPolonger(){
        auto res = new FastList(*this);
        if(res==nullptr)
            throw MemoryException("get sorted polonger");
        res->sort();
        return res;
    }

    /**
     * Reverse the list
    */
    void reverse(){
        long len = this->getLength();
        if(len<=1)
            return;
        long mid = (len-1)/2;
        for(long i = 0; i<=mid; i++)
            this->swap(i,len-i-1);
    }

    /**
     * Returns a list that is the same as the initial one, but reversed, without modifying the original list
    */
    FastList<T> getReversed(){
        auto res = FastList(*this);
        res.reverse();
        return res;
    }

    /**
     * Returns a polonger to a list that is the same as the initial one, but reversed, without modifying the original list
    */
    FastList<T>* getReversedPolonger(){
        auto res = new FastList(*this);
        if(res == nullptr)
            throw MemoryException("get reversed polonger");
        res->reverse();
        return res;
    }

    /**
     * Shuffle the list randomly
    */
    void shuffle(){
        this->shuffle_from_generator(this->generator);
    }

    /**
     * Shuffle the list randomly given the seed
    */
    void shuffle(unsigned long seed){
        std::mt19937 seed_generator(seed);
        this->shuffle_from_generator(seed_generator);
    }

    /**
     * Returns a list that is the same as the initial one, but shuffled, without modifying the original list
    */
    FastList<T> getShuffled(){
        auto res = FastList(*this);
        res.shuffle();
        return res;
    }

    /**
     * Returns a list that is the same as the initial one, but shuffled given the seed, without modifying the original list
    */
    FastList<T> getShuffled(unsigned long seed){
        auto res = FastList(*this);
        res.shuffle(seed);
        return res;
    }

    /**
     * Returns a polonger to a list that is the same as the initial one, but shuffled, without modifying the original list
    */
    FastList<T>* getShuffledPolonger(){
        auto res = new FastList(*this);
        if(res==nullptr)
            throw MemoryException("get shuffled polonger");
        res->shuffle();
        return res;
    }

    /**
     * Returns a polonger to a list that is the same as the initial one, but shuffled given the seed, without modifying the original list
    */
    FastList<T>* getShuffledPolonger(unsigned long seed){
        auto res = new FastList(*this);
        if(res==nullptr)
            throw MemoryException("get shuffled polonger seeded");
        res->shuffle(seed);
        return res;
    }

    /**
     * Returns an item at the given index
    */
    T get(long index)const{
        if((index<0)||(index>=this->getLength()))
            throw IndexOutOfBoundsException("get");
        return this->array[index];
    }

    /**
     * Returns a reference to the item at the given index
    */
    T& getReference(long index){
        if((index<0)||(index>=this->getLength()))
            throw IndexOutOfBoundsException("get reference");
        return (this->array[index]);
    }

    /**
     * Returns a copy of the front of the list
    */
    T front()const{
        if(this->isEmpty())
            throw EmptyListException("get front");
        return this->array[0];
    }

    /**
     * Returns a reference to the front of the list
    */
    T& frontReference(){
        if(this->isEmpty())
            throw EmptyListException("get front reference");
        return this->array[0];
    }

    /**
     * Returns a copy of the back of the list
    */
    T back()const{
        if(this->isEmpty())
            throw EmptyListException("get back");
        return this->array[this->length-1];
    }

    /**
     * Returns a reference to the back of the list
    */
    T& backReference(){
        if(this->isEmpty())
            throw EmptyListException("get back reference");
        return (this->array[this->length-1]);
    }

    /**
     * returns a random element of the list
    */
    T sample(){
        if(this->isEmpty())
            throw EmptyListException("sample");
        std::uniform_int_distribution<> distrib(0, this->getLength()-1);
        long index = distrib(this->generator);
        return this->get(index);
    }

    /**
     * returns a random reference to an element of the list
    */
    T& sampleReference(){
        if(this->isEmpty())
            throw EmptyListException("sample reference");
        std::uniform_int_distribution<> distrib(0, this->getLength()-1);
        long index = distrib(this->generator);
        return this->getReference(index);
    }

    /**
     * returns a random element of the list
    */
    T sample(unsigned long seed){
        if(this->isEmpty())
            throw EmptyListException("sample");
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> distrib(0, this->getLength()-1);
        long index = distrib(gen);
        return this->get(index);
    }

    /**
     * returns a random reference to an element of the list
    */
    T& sampleReference(unsigned long seed){
        if(this->isEmpty())
            throw EmptyListException("sample reference");
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> distrib(0, this->getLength()-1);
        long index = distrib(gen);
        return this->getReference(index);
    }

    /**
     * makes array larger
    */
    private:
    void widen(long shift,long start_position){
        this->size = max(this->size * 3 / 2, MIN_SIZE);
        T* old_array = this->array;
        this->array = new T[this->size];
        if(this->array==nullptr)
            throw MemoryException("widen");
        for(long i=0;i<this->getLength();i++){
            if(i>=start_position)
                this->array[i+shift]=old_array[i];
            else
                this->array[i]=old_array[i];
        }
        if(old_array!=nullptr)
            delete[] old_array;
    }

    /**
     * Shrinks the fastlist
    */
    void shrink(long shift,long start_position){
        this->size = max(this->size * 2 / 3, MIN_SIZE);
        T* old_array = this->array;
        this->array = new T[this->size];
        if(this->array==nullptr)
            throw MemoryException("shrink");
        for(long i=0;i<this->getLength() - shift;i++){
            if(i>=start_position)
                this->array[i]=old_array[i+shift];
            else
                this->array[i]=old_array[i];
        }
        if(old_array!=nullptr)
            delete[] old_array;
    }

    /**
     * Shifts all elements in the array to the right by x positions
    */
    void shift_right(long shift, long start_position){
        for(long i=this->getLength()-1;i>=start_position;i++){
            this->array[i+shift]=this->array[i];
        }
    }

    /**
     * Shifts all elements in the array to the right by x positions
    */
    void shift_left(long shift, long start_position){
        for(long i=shift+start_position;i<this->getLength();i++){
            this->array[i-shift]=this->array[i];
        }
    }

    public:
    /**
     * Add an element to the end of the list
    */
    bool enqueue(T item){
        if(this->length == this->size){
            this->widen(0,0);
        }
        this->array[this->length++]=T(item);
        return true;
    }

    /**
     * Add an element to the end of the list
    */
    bool push(T item){
        return this->enqueue(item);
    }

    /**
     * Add an element to the beginning of the list
    */
    bool add_first(T item){
        if(this->length == this->size){
            this->widen(1,0);
        }else{
            this->shift_right(1,0);
        }
        this->array[0]=T(item);
        this->length++;
        return true;
    }

    /**
     * Add an element at the specified index of the list
    */
    bool add(T item,long index){
        if((index<0)||(index>this->getLength()))
            return false;
        if(index==this->getLength())
            return this->push(item);
        if(this->length==this->size){
            this->widen(1,index);
        }else{
            this->shift_right(1,index);
        }
        this->array[index]=T(item);
        this->length++;
    }

    /**
     * Check if the list is empty
    */
    bool isEmpty()const{
        return this->length == 0;
    }

    /**
     * Returns the length of the list
    */
    long getLength()const{
        return (this->length);
    }

    /**
     * Remove an element from the start of the list and return the element. Returns nullptr if the list is empty
    */
    T dequeue(){
        if (this->isEmpty()){
            throw EmptyListRemovalexception("dequeue");
        }
        long threshold =  this->size / 2;
        long new_size = this->size * 2 / 3;
        T res = this->array[0];
        if(this->getLength()-1 < threshold && new_size >= MIN_SIZE){
            this->shrink(1,0);
        }else{
            this->shift_left(1,0);
        }
        this->length-=1;
        return res;
    }

    /**
     * Remove an element from the end of the list and return the element
    */
    T pop(){
        if (this->isEmpty()){
            throw EmptyListRemovalexception("pop");
        }
        T res = this->back();
        long threshold =  this->size / 2;
        long new_size = this->size * 2 / 3;
        if(this->getLength()-1 < threshold && new_size >= MIN_SIZE){
            this->shrink(0,0);
        }
        this->length-=1;
        return res;
    }

    /**
     * Removes an element at the given index
    */
    T remove_at(long index){
        if (this->isEmpty()){
            throw EmptyListRemovalexception("pop");
        }
        if (this->getLength()<=index || index<0){
            throw IndexOutOfBoundsException("remove at");
        }
        T res = this->array[index];
        long threshold =  this->size / 2;
        long new_size = this->size * 2 / 3;
        if(this->getLength()-1 < threshold && new_size >= MIN_SIZE){
            this->shrink(1,index);
        }else{
            this->shift_left(1,index);
        }
        this->length-=1;
        return res;
    }

    /**
     * Removes the first item in the list which equals the item parameter
    */
    T remove(T item){
        if(!this->contains(item))
            throw ItemNotFoundException("remove");
        long index = this->index_of(item);
        return this->remove_at(index);
    }

    /**
     * Removes all occureences of the item in the list which equals the item parameter
    */
    T remove_every(T item){
        if(!this->contains(item))
            throw ItemNotFoundException("remove all");
        long index;
        T res;
        while((index = this->index_of(item)) != -1){
            res = this->remove_at(index);
        }
        return res;
    }

    /**
     * Concatenate the list at the end of the original one
    */
    void concat(FastList<T> const& list){
        (*this)+=list;
    }

    /**
     * Returns an iterator to the beginning of the list
    */
    FastIter<T> begin(){
        return FastIter<T>(this);
    }

    /**
     * Returns an iterator to the end of the list
    */
    FastIter<T> end(){
        return FastIter<T>(this,this->getLength());
    }

    /**
     * Returns a constant iterator to the beginning of the list
    */
    FastIterConst<T> cbegin()const{
        return FastIterConst<T>(this);
    }

    /**
     * Returns a costant iterator to the end of the list
    */
    FastIterConst<T> cend()const{
        return FastIterConst<T>(this,this->getLength());
    }

    /**
     * Returns an iterator to the end of the list
    */
    FastIterReverse<T> rbegin(){
        return FastIterReverse<T>(this);
    }

    /**
     * Returns an iterator to the beginning of the list
    */
    FastIterReverse<T> rend(){
        return FastIterReverse<T>(this,-1);
    }

    /**
     * Returns a constant iterator to the end of the list
    */
    FastIterConstReverse<T> crbegin()const{
        return FastIterConstReverse<T>(this);
    }

    /**
     * Returns a costant iterator to the beginning of the list
    */
    FastIterConstReverse<T> crend()const{
        return FastIterConstReverse<T>(this,-1);
    }


    private:
    FastList<T> divide_et_impera(long index){
        if(index<0 || index>=this->getLength()){
            throw IndexOutOfBoundsException("divide et impera");
        }
        
        FastList<T> res=FastList<T>((this->length-index) * 3 / 2);
        for(long j=0;j<this->length-index;j++){
            res.push(this->array[j+index]);
        }
        this->length = index;
        long threshold =  this->size / 2;
        long new_size = this->size * 2 / 3;
        if(this->getLength()-1 < threshold && new_size >= MIN_SIZE){
            T* old_array = this->array;
            long new_size = this->getLength() * 3 / 2;
            this->array = new T[new_size];
            if(this->array == nullptr)
                throw MemoryException("filter");
            for(long i=0;i<this->length;i++){
                this->array[i]=old_array[i];
            }
            this->size = new_size;
            delete[] old_array;
        }

        return res;
    }

    FastList<T> divide_et_impera_removing_index(long index){
        if(index<0 || index>=this->getLength()){
            throw IndexOutOfBoundsException("divide et impera");
        }
        
        FastList<T> res=FastList<T>((this->length-index) * 3 / 2);
        for(long j=1;j<this->length-index;j++){
            res.push(this->array[j+index]);
        }
        this->length = index;
        long threshold =  this->size / 2;
        long new_size = this->size * 2 / 3;
        if(this->getLength()-1 < threshold && new_size >= MIN_SIZE){
            T* old_array = this->array;
            long new_size = this->getLength() * 3 / 2;
            this->array = new T[new_size];
            if(this->array == nullptr)
                throw MemoryException("filter");
            for(long i=0;i<this->length;i++){
                this->array[i]=old_array[i];
            }
            this->size = new_size;
            delete[] old_array;
        }

        return res;
    }

    public:

    /**
     * Splits the list longo 2 fastists at the given index
    */
    FastList<FastList<T>> split_at_index(long index){
        if(index<0 || index>this->getLength()){
            throw IndexOutOfBoundsException("split at index");
        }
        if(index==this->getLength()){
            FastList<FastList<T>> res = FastList<FastList<T>>();
            res.enqueue(*this);
            res.enqueue(FastList<T>());
            return res;
        }
        FastList<T> second = this->divide_et_impera(index);
        FastList<FastList<T>> res = FastList<FastList<T>>();
        res.enqueue(*this);
        res.enqueue(FastList<T>(move(second)));
        return res;
    }

    /**
     * Splits the list longo multiple lists when the given item is found, removing the instances of the item.
    */
    FastList<FastList<T>> split(T item){
        if(!this->contains(item)){
            FastList<FastList<T>> res = FastList<FastList<T>>();
            res.enqueue(*this);
            return res;
        }
        FastList<FastList<T>> res = FastList<FastList<T>>();
        long index_of_item = this->index_of(item);
        FastList<T> second = this->divide_et_impera_removing_index(index_of_item);
        res.push(FastList<T>(*this));
        res.push(move(second));
        while((index_of_item = res.back().index_of(item)) != -1){
            res.push(res.backReference().divide_et_impera_removing_index(index_of_item));
        }
        return res;
    }

    /**
     * Splits the list longo multiple lists when the given item is found, removing the instances of the item, without modifying the original list
    */
    FastList<FastList<T>> split_const(T item)const{
        FastList<T> new_list=FastList<T>(*this);
        return new_list.split(item);
    }

    /**
     * Splits the list longo 2 fastists at the given index
    */
    FastList<FastList<T>> split_at_index_const(long index)const{
        FastList<T> new_list=FastList<T>(*this);
        return new_list.split_at_index(index);
    }

    /**
     * Returns true if the list contains the specified item
    */
    bool contains(T item)const{
        for(FastIterConst iter=this->cbegin();iter!=this->cend();iter++){
            if((*iter)==item){
                return true;
            }
        }
        return false;
    }

    /**
     * Returns the index of the first appearence of the item in the list. Returns -1 if the item is not in the list.
    */
    long index_of(T item){
        long index=0;
        for(FastIterConst iter=this->cbegin();iter!=this->cend();iter++){
            if((*iter)==item){
                return index;
            }
            index++;
        }
        return -1;
    }

    /**
     * Returns the number of instances of the item in the list
    */
    long count_instances_of(T item)const{
        long count = 0;
        for(auto it=this->cbegin();it!=this->cend();it++){
            if((*it)==item){
                count++;
            }
        }
        return count;
    }

    /**
     * Merges a list longo this list without modifying the parameter of the merge
    */
    void merge(FastList<T>const& list){
        (*this)+=list;
    }

    /**
     * Applyes the specified function to all the elements of the list
    */
    void apply(T (*func)(T)){
        for(auto it=this->begin();it!=this->end();it++){
            (*it)=func(*it);
        }
    }

    /**
     * Applies an incrementative function to the list 
    */
    T accumulate(T (*func)(const T,const T), T init_value)const{
        for(FastIterConst iter=this->cbegin(); iter != this->cend(); iter++){
            init_value = func(init_value,*iter);
        } 
        return init_value;
    }

    /**
     * Applyes the specified function to all the elements of the list without editing the original list
    */
    FastList<T> map(T (*func)(T)){
        FastList<T> res(*this);
        res.apply(func);
        return res;
    }

    /**
     * Reset the seed for the generator
    */
    void reset_generator(){
        this->generator = std::mt19937((unsigned)time(nullptr));
    }

    /**
     * Set the seed for the generator
    */
    void set_generator(unsigned long seed){
        this->generator = std::mt19937(seed);
    }

    /**
     * Applyes the specified function to all the elements of the list and removes all elements that make it true
    */
    void filter(bool (*func)(T)){
        long index = 0;
        long count = 0;
        long len = this->getLength();
        for(;index<len;index++){
            if(func(this->array[index])){
                count++;
            }else{
                this->array[index-count] = this->array[index];
            }
        }
        this->length-=count;
        long threshold =  this->size / 2;
        long new_size = this->size * 2 / 3;
        if(this->getLength()-1 < threshold && new_size >= MIN_SIZE){
            T* old_array = this->array;
            long new_size = this->getLength() * 3 / 2;
            this->array = new T[new_size];
            if(this->array == nullptr)
                throw MemoryException("filter");
            for(long i=0;i<this->length;i++){
                this->array[i]=old_array[i];
            }
            this->size = new_size;
            delete[] old_array;
        }
    }

    FastList<T>& operator+=(const FastList<T>& arg_list){
        long old_len = this->getLength();
        long other_len = arg_list.getLength();
        this->length+=other_len;
        if(this->length >= this->size){
            T* old_array = this->array;
            this->size = this->getLength() * 3 / 2;
            this->array = new T[this->size];
            if(this->array == nullptr)
                throw MemoryException("operator +=");
            for(long i=0;i<old_len;i++){
                this->array[i]=old_array[i];
            }
            delete[] old_array;
        }
        for(long j=0;j<other_len;j++){
            this->array[j+old_len]=arg_list.get(j);
        }
        return *this;
    }
 
    friend FastList<T> operator+(const FastList<T>& a,const FastList<T>& b){
        FastList<T> res = FastList<T>();
        res += a;
        res += b;
        return res;
    }

    friend ostream& operator<< <>(ostream& os, FastList<T> const & list);

    friend ostream& operator<< <>(ostream& os, FastList<FastList<T>*> const & list);

    T& operator[](long index){
        return this->getReference(index);
    }

    friend bool operator==(const FastList<T>& a,const FastList<T>& b){
        if(a.getLength()!=b.getLength()){
            return false;
        }
        auto a_iter = a.cbegin();
        auto b_iter = b.cbegin();
        while(a_iter != a.cend() && b_iter != b.cend()){
            if((*a_iter) != (*b_iter)){
                return false;
            }
            a_iter++;
            b_iter++;
        }
        return true;
    }

    /*friend bool operator==(const FastList<T*>& a,const FastList<T*>& b){
        if(a.getLength()!=b.getLength()){
            return false;
        }
        auto a_iter = a.cbegin();
        auto b_iter = b.cbegin();
        while(a_iter != a.cend() && b_iter != b.cend()){
            if((**a_iter) != (**b_iter)){
                return false;
            }
            a_iter++;
            b_iter++;
        }
        return true;
    }*/

    friend bool operator!=(const FastList<T>& a,const FastList<T>& b){
        return ! (a == b);
    }

    bool enqueue_all(T item){
        return this->enqueue(item);
    }

    template <typename ...Ts> bool enqueue_all(T item,Ts... items){
        bool longermediate = this->enqueue(item);
        return longermediate && this->enqueue_all(items...);
    }

    bool push_all(T arg){
        return this->push(arg);   
    }

    template <typename ...Ts> bool push_all(T arg, Ts...args){
        bool longermediate = this->push(arg);
        return longermediate && this->push_all(args...); 
    }

    FastList<T> remove_all(T arg){
        FastList<T> removed = FastList<T>();
        removed.push(this->remove(arg));
        return removed;
    }

    template <typename ...Ts> FastList<T> remove_all(T arg, Ts... args){
        FastList<T> removed = FastList<T>();
        removed.push(this->remove(arg));
        return removed + this->remove_all(args...);
    }

    FastList<T> remove_all_every(T arg){
        FastList<T> removed = FastList<T>();
        removed.push(this->remove_every(arg));
        return removed;
    }

    template <typename ...Ts> FastList<T> remove_all_every(T arg, Ts... args){
        FastList<T> removed = FastList<T>();
        removed.push(this->remove_every(arg));
        return removed + remove_all(args...);
    }
};

template <typename T> ostream & operator<<(ostream& os,FastList<T>const & list){
    os<<list.getLength()<<" ";
    os<<"[";
    for(auto iter=list.cbegin();iter!=list.cend();iter++){
        if(iter!=list.cbegin()){
            os<<",";
        }
        os<<(*iter);
    }
    os<<"]";
    return os;
}

template <typename T> ostream & operator<<(ostream& os,FastList<FastList<T>*>const & list){
    os<<list.getLength()<<" ";
    os<<"[";
    for(auto iter=list.cbegin();iter!=list.cend();iter++){
        if(iter!=list.cbegin()){
            os<<",";
        }
        os<<(**iter);
    }
    os<<"]";
    return os;
}