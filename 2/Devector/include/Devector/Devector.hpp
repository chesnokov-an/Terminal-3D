#ifndef DEVECTOR_H
#define DEVECTOR_H

#include <memory>
#include <concepts>
#include <exception>
#include <algorithm>

namespace DevectorNameSpace{

struct reserve_only_tag_t {};

template<typename T, typename Alloc = std::allocator<T>>
class devector final : private Alloc {
public:
    using value_type =                                  T;
    using allocator_type =                              Alloc;
    using allocator_traits_type =                       std::allocator_traits<allocator_type>;
    using stored_allocator_type =                       allocator_type;
    using pointer =                                     typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer =                               typename std::allocator_traits<allocator_type>::const_pointer;
    using reference =                                   value_type&;
    using const_reference =                             const value_type&;
    using size_type =                                   typename std::allocator_traits<allocator_type>::size_type;
    using difference_type =                             typename std::allocator_traits<allocator_type>::difference_type;
    using iterator =                                    pointer;
    using const_iterator =                              const_pointer;
    using reverse_iterator =                            std::reverse_iterator<iterator>;
    using const_reverse_iterator =                      std::reverse_iterator<const_iterator>;

private:
    using propagate_on_container_copy_assignment =      typename std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment;
    using propagate_on_container_move_assignment =      typename std::allocator_traits<allocator_type>::propagate_on_container_move_assignment;
    using propagate_on_container_swap =                 typename std::allocator_traits<allocator_type>::propagate_on_container_swap;
    using is_always_equal =                             typename std::allocator_traits<allocator_type>::is_always_equal;

private:
    pointer arr_;
    size_type size_;
    size_type capacity_;
    size_type front_capacity_;

private:
    void destroy_n(size_type n){
        for(size_type index = 0; index < n; ++index){
            allocator_traits_type::destroy(get_stored_allocator(), arr_ + front_capacity_ + index);
        }
    }
    void destroy_n_and_deallocate(size_type n){
        if (arr_ == nullptr) return;
        destroy_n(n);
        allocator_traits_type::deallocate(get_stored_allocator(), arr_, capacity_);
        arr_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        front_capacity_ = 0;
    }

public:
    // construct/copy/destruct
    devector() noexcept : Alloc(), arr_(nullptr), size_(0), capacity_(0), front_capacity_(0) {}

    explicit devector(const allocator_type& allocator) noexcept : Alloc(allocator), arr_(nullptr), size_(0), capacity_(0), front_capacity_(0) {}

    devector(size_type new_capacity, reserve_only_tag_t, const allocator_type& allocator = allocator_type()) : devector(allocator) {
        if (new_capacity == 0) return;
        pointer newarr = allocator_traits_type::allocate(get_stored_allocator(), new_capacity);
        arr_ = newarr;
        capacity_ = new_capacity;
    }

    devector(size_type front_cap, size_type back_cap, reserve_only_tag_t, const allocator_type& allocator = allocator_type())
            : devector(front_cap + back_cap, reserve_only_tag_t{}, allocator) {
        front_capacity_ = front_cap;
    }

    devector(size_type new_capacity, const_reference value, const allocator_type& allocator = allocator_type())
            : devector(new_capacity, reserve_only_tag_t{}, allocator) {
        std::uninitialized_fill_n(arr_, new_capacity, value);
        size_ = new_capacity;
    }

    explicit devector(size_type new_capacity, const allocator_type& allocator = allocator_type()) 
            : devector(new_capacity, value_type{}, allocator) {}

    template<typename ForwardIterator>
    requires std::forward_iterator<ForwardIterator>
    devector(ForwardIterator first, ForwardIterator last, const allocator_type& allocator = allocator_type()) 
            : devector(static_cast<size_type>(std::distance(first, last)), reserve_only_tag_t{}, allocator){
        std::uninitialized_copy_n(first, capacity_, arr_);
        size_ = capacity_;
    }

    template<typename InputIterator>
    requires std::input_iterator<InputIterator> && (!std::forward_iterator<InputIterator>)
    devector(InputIterator first, InputIterator last, const allocator_type& allocator = allocator_type()) : devector(allocator) {
        std::copy(first, last, std::back_inserter(*this));
    }
    
    devector(const devector& other, const allocator_type& allocator)
            : devector(other.begin(), other.end(), allocator) {}

    devector(const devector& other) : devector(other, allocator_traits_type::select_on_container_copy_construction(other.get_stored_allocator())) {}

    // До сюда дошли по проверке

    devector(devector&& other, const allocator_type& allocator) : devector(allocator) {
        if(get_stored_allocator() == other.get_stored_allocator() || is_always_equal::value){
            swap_resources(other);
            return;
        }
        pointer newarr = allocator_traits_type::allocate(get_stored_allocator(), other.size_);
        try{
            std::uninitialized_move_n(other.begin(), other.size_, newarr);
        }
        catch(...){
            allocator_traits_type::deallocate(get_stored_allocator(), newarr, other.size_);
            throw;
        }
        arr_ = newarr;
        size_ = other.size_;
        capacity_ = size_;
        other.destroy_n_and_deallocate(other.size_);
    }

    devector(devector&& other) noexcept
            : Alloc(std::move(other.get_stored_allocator()))
            , arr_(other.arr_)
            , size_(other.size_)
            , capacity_(other.capacity_)
            , front_capacity_(other.front_capacity_) {
        other.arr_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        other.front_capacity_ = 0;
    }

    devector(const std::initializer_list<T>& il, const allocator_type& allocator = allocator_type())
            : devector(il.begin(), il.end(), allocator) {}

    devector& operator=(const devector& other){
        if(this == &other) return *this;
        if(!propagate_on_container_copy_assignment::value && (capacity_ >= other.size_)){
            clear();
            std::uninitialized_copy_n(other.begin(), other.size_, arr_);
            size_ = other.size_;
            return *this;
        }
        allocator_type new_allocator = propagate_on_container_copy_assignment::value ? other.get_stored_allocator() : get_stored_allocator();
        pointer newarr = allocator_traits_type::allocate(new_allocator, other.size_);
        try{
            std::uninitialized_copy_n(other.begin(), other.size_, newarr);
        }
        catch(...){
            allocator_traits_type::deallocate(new_allocator, newarr, other.size_);
            throw;
        }
        destroy_n_and_deallocate(size_);
        get_stored_allocator() = new_allocator;
        arr_ = newarr;
        size_ = other.size_;
        capacity_ = other.size_;
        return *this;
    }

    devector& operator=(devector&& other) noexcept(propagate_on_container_move_assignment::value || is_always_equal::value){
        if(this == &other){ return *this; }
        if(propagate_on_container_move_assignment::value || (get_stored_allocator() == other.get_stored_allocator()) || is_always_equal::value){
            if constexpr (propagate_on_container_move_assignment::value){
                get_stored_allocator() = std::move(other.get_stored_allocator());
            }
            swap_resources(other);
            return *this;
        }
        if(capacity_ >= other.size_){
            clear();
            std::uninitialized_move_n(other.begin(), other.size_, arr_);
            size_ = other.size_;
            return *this;
        }
        pointer newarr = allocator_traits_type::allocate(get_stored_allocator(), other.size_);
        try{
            std::uninitialized_move_n(other.begin(), other.size_, newarr);
        }
        catch(...){
            allocator_traits_type::deallocate(get_stored_allocator(), newarr, other.size_);
            throw;
        }
        destroy_n_and_deallocate(size_);
        arr_ = newarr;
        size_ = other.size_;
        capacity_ = other.size_;
        return *this;
    }

    devector& operator=(std::initializer_list<T> il){
        assign(il.begin(), il.end());
        return *this;
    }

    ~devector(){
        destroy_n_and_deallocate(size_);
    }

    // public member functions
    template<typename InputIterator> 
    requires std::input_iterator<InputIterator>
    void assign(InputIterator first, InputIterator last){
        devector tmp = devector(first, last, get_stored_allocator());
        swap(tmp);
    }
    void assign(size_type new_capacity, const_reference value){
        devector tmp = devector(new_capacity, value, get_stored_allocator());
        swap(tmp);
    }
    void assign(std::initializer_list<T> il){
        assign(il.begin(), il.end());
    }
    allocator_type get_allocator() const noexcept{
        return static_cast<const allocator_type&>(*this);
    }
    const allocator_type& get_stored_allocator() const noexcept{
        return static_cast<const allocator_type&>(*this);
    }
    allocator_type& get_stored_allocator() noexcept{
        return static_cast<allocator_type&>(*this);
    }
    iterator begin() noexcept{
        return &arr_[front_capacity_];
    }
    const_iterator begin() const noexcept{
        return &arr_[front_capacity_];
    }
    iterator end() noexcept{
        return &arr_[front_capacity_ + size_];
    }
    const_iterator end() const noexcept{
        return &arr_[front_capacity_ + size_];
    }
    reverse_iterator rbegin() noexcept{
        return std::reverse_iterator<iterator>(end());
    }
    const_reverse_iterator rbegin() const noexcept{
        return std::reverse_iterator<const_iterator>(end());
    }
    reverse_iterator rend() noexcept{
        return std::reverse_iterator<iterator>(begin());
    }
    const_reverse_iterator rend() const noexcept{
        return std::reverse_iterator<const_iterator>(begin());
    }
    const_iterator cbegin() const noexcept{
        return begin();
    }
    const_iterator cend() const noexcept{
        return end();
    }
    const_reverse_iterator crbegin() const noexcept{
        return rbegin();
    }
    const_reverse_iterator crend() const noexcept{
        return rend();
    }
    bool empty() const noexcept{
        return (size() == 0);
    }
    size_type size() const noexcept{
        return size_;
    }
    size_type max_size() const noexcept{
        return allocator_traits_type::max_size(get_stored_allocator());
    }
    size_type capacity() const noexcept{
        return capacity_;
    }
    size_type front_free_capacity() const noexcept{
        return front_capacity_;
    }
    size_type back_free_capacity() const noexcept{
        return capacity_ - (front_capacity_ + size_);
    }
    void resize(size_type new_size){
        resize_back(new_size);
    }
    void resize(size_type new_size, const_reference value){
        resize_back(new_size, value);
    }
    void resize_front(size_type new_size, const_reference value){
        if (new_size < size_) {
            erase(begin(), begin() + (size_ - new_size));
            return;
        }
        insert(begin(), new_size - size_, value);
    }
    void resize_front(size_type new_size){
        resize_front(new_size, value_type{});
    }
    void resize_back(size_type new_size, const_reference value){
        if (new_size < size_) {
            erase(begin() + new_size, end());
            return;
        }
        insert(begin() + size_, new_size - size_, value);
    }
    void resize_back(size_type new_size){
        resize_back(new_size, value_type{});
    }
    void reserve(size_type new_capacity){
        reserve_back(new_capacity);
    }
    void reserve_front(size_type new_capacity){
        if(new_capacity < size_){ return; }
        size_type n = new_capacity - size_;
        if(n <= front_capacity_){ return; }
        if (new_capacity > max_size()) {
            throw std::length_error("devector::reserve_front");
        }
        if(new_capacity <= capacity_){
            size_type shift = n - front_capacity_;
            for (size_type index = size_; index-- > 0;) {
                allocator_traits_type::construct(get_stored_allocator(), arr_ + front_capacity_ + index + shift, std::move_if_noexcept(arr_[front_capacity_ + index]));
                allocator_traits_type::destroy(get_stored_allocator(), arr_ + front_capacity_ + index);
            }
            front_capacity_ = n;
            return;
        }
        pointer newarr = allocator_traits_type::allocate(get_stored_allocator(), new_capacity);
        size_type index = 0;
        try{
            for(;index < size_; ++index){
                allocator_traits_type::construct(get_stored_allocator(), newarr + n + index, std::move_if_noexcept((*this)[index]));
            }
        }
        catch(...){
            for(size_type bad_index = 0; bad_index < index; ++bad_index){
                allocator_traits_type::destroy(get_stored_allocator(), newarr + n + bad_index);
            }
            allocator_traits_type::deallocate(get_stored_allocator(), newarr, new_capacity);
            throw;
        }
        size_type old_size = size_;
        destroy_n_and_deallocate(size_);
        arr_ = newarr;
        size_ = old_size;
        capacity_ = new_capacity;
        front_capacity_ = n;
    }
    void reserve_back(size_type new_capacity){
        if(new_capacity < size_){ return; }
        size_type n = new_capacity - size_;
        if(n <= back_free_capacity()){ return; }
        if (new_capacity > max_size()) {
            throw std::length_error("devector::reserve_back");
        }
        if(new_capacity <= capacity_){
            size_type new_front_capacity = capacity_ - n - size_;
            for(size_type index = 0; index < size_; ++index){
                allocator_traits_type::construct(get_stored_allocator(), arr_ + new_front_capacity + index, std::move_if_noexcept(arr_[front_capacity_ + index]));
                allocator_traits_type::destroy(get_stored_allocator(), arr_ + front_capacity_ + index);
            }
            front_capacity_ = new_front_capacity;
            return;
        }
        pointer newarr = allocator_traits_type::allocate(get_stored_allocator(), new_capacity);
        size_type index = 0;
        try{
            for(;index < size_; ++index){
                allocator_traits_type::construct(get_stored_allocator(), newarr + index, std::move_if_noexcept((*this)[index]));
            }
        }
        catch(...){
            for(size_type bad_index = 0; bad_index < index; ++bad_index){
                allocator_traits_type::destroy(get_stored_allocator(), newarr + bad_index);
            }
            allocator_traits_type::deallocate(get_stored_allocator(), newarr, new_capacity);
            throw;
        }
        size_type old_size = size_;
        destroy_n_and_deallocate(size_);
        arr_ = newarr;
        size_ = old_size;
        capacity_ = new_capacity;
        front_capacity_ = 0;
    }
    void shrink_to_fit(){
        if(capacity_ == size_){ return; }
        pointer newarr = allocator_traits_type::allocate(get_stored_allocator(), size_);
        size_type index = 0;
        try{
            for(;index < size_; ++index){
                allocator_traits_type::construct(get_stored_allocator(), newarr + index, std::move_if_noexcept((*this)[index]));
            }
        }
        catch(...){
            for(size_type bad_index = 0; bad_index < index; ++bad_index){
                allocator_traits_type::destroy(get_stored_allocator(), newarr + bad_index);
            }
            allocator_traits_type::deallocate(get_stored_allocator(), newarr, size_);
            throw;
        }
        size_type old_size = size_;
        destroy_n_and_deallocate(size_);
        arr_ = newarr;
        size_ = old_size;
        capacity_ = size_;
        front_capacity_ = 0;
    }
    template<typename ... Args> void emplace_front(Args&& ... args){
        if(front_capacity_ == 0){
            reserve_front(capacity_ * 2 + 1);
        }
        allocator_traits_type::construct(get_stored_allocator(), arr_ + front_capacity_ - 1, std::forward<Args>(args)...);
        --front_capacity_;
        ++size_;
    }
    void push_front(const_reference value){
        emplace_front(value);
    }
    void push_front(value_type&& value){
        emplace_front(std::move(value));
    }
    void pop_front() noexcept{
        allocator_traits_type::destroy(get_stored_allocator(), arr_ + front_capacity_);
        ++front_capacity_;
        --size_;
    }
    template<typename ... Args> void emplace_back(Args&& ... args){
        if(back_free_capacity() == 0){
            reserve_back(capacity_ * 2 + 1);
        }
        allocator_traits_type::construct(get_stored_allocator(), arr_ + front_capacity_ + size_, std::forward<Args>(args)...);
        ++size_;
    }
    void push_back(const_reference value){
        emplace_back(value);
    }
    void push_back(value_type&& value){
        emplace_back(std::move(value));
    }
    void pop_back() noexcept{
        allocator_traits_type::destroy(get_stored_allocator(), arr_ + front_capacity_ + size_ - 1);
        --size_;
    }
    template<class... Args> 
    iterator emplace(const_iterator position, Args&& ... args){
        if(position == cbegin()){
            emplace_front(std::forward<Args>(args)...);
            return begin();
        }
        if(position == cend()){
            emplace_back(std::forward<Args>(args)...);
            return begin() + size_ - 1;
        }
        difference_type pos_from_begin = position - cbegin();
        if(capacity_ == size_){
            reserve(capacity_ * 2 + 1);
            position = cbegin() + pos_from_begin;
        }
        if(back_free_capacity() > 0){
            for (size_type i = size_; i-- > pos_from_begin;) {
                allocator_traits_type::construct(get_stored_allocator(), arr_ + front_capacity_ + i + 1, std::move_if_noexcept(arr_[front_capacity_ + i]));
                allocator_traits_type::destroy(get_stored_allocator(), arr_ + front_capacity_ + i);
            }
            allocator_traits_type::construct(get_stored_allocator(), arr_ + front_capacity_ + pos_from_begin, std::forward<Args>(args)...);
            ++size_;
            return begin() + pos_from_begin;
        }
        for(size_type i = 0; i < pos_from_begin; ++i){
            allocator_traits_type::construct(get_stored_allocator(), arr_ + front_capacity_ + i - 1, std::move_if_noexcept(arr_[front_capacity_ + i]));
            allocator_traits_type::destroy(get_stored_allocator(), arr_ + front_capacity_ + i);
        }
        allocator_traits_type::construct(get_stored_allocator(), arr_ + front_capacity_ - 1 + pos_from_begin, std::forward<Args>(args)...);
        ++size_;
        --front_capacity_;
        return begin() + pos_from_begin;
    }
    iterator insert(const_iterator position, const_reference value){
        return emplace(position, value);
    }
    iterator insert(const_iterator position, value_type&& value){
        return emplace(position, std::move(value));
    } 
    iterator insert(const_iterator position, size_type n, const_reference value){
        difference_type pos_from_begin = position - cbegin();
        if(n == 0){ return begin() + pos_from_begin; }
        reserve(size_ + n);
        position = cbegin() + pos_from_begin;
        for(size_type i = 0; i < n; ++i){
            position = emplace(position, value);
        }
        return begin() + pos_from_begin;
    }
    template<typename InputIterator>
    requires std::input_iterator<InputIterator>
    iterator insert(const_iterator position, InputIterator first, InputIterator last){
        difference_type pos_from_begin = position - cbegin();
        if(first == last){ return begin() + pos_from_begin; }
        if constexpr (std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>){
            reserve(size_ + std::distance(first, last));
            position = cbegin() + pos_from_begin;
        }
        auto rb = std::reverse_iterator<InputIterator>(last);
        auto re = std::reverse_iterator<InputIterator>(first);
        for(; rb != re; ++rb){
            position = emplace(position, *rb);
        }
        return begin() + pos_from_begin;
    }
    iterator insert(const_iterator position, std::initializer_list<T> il){
        return insert(position, il.begin(), il.end());
    }
    iterator erase(const_iterator position){
        difference_type pos_from_begin = position - cbegin();
        allocator_traits_type::destroy(get_stored_allocator(), arr_ + front_capacity_ + pos_from_begin);
        for(size_type i = pos_from_begin + 1; i < size_; ++i){
            allocator_traits_type::construct(get_stored_allocator(), arr_ + front_capacity_ + i - 1, std::move_if_noexcept(arr_[front_capacity_ + i]));
            allocator_traits_type::destroy(get_stored_allocator(), arr_ + front_capacity_ + i);
        }
        --size_;
        return begin() + pos_from_begin;
    }
    iterator erase(const_iterator first, const_iterator last){
        difference_type pos_from_begin = first - cbegin();
        difference_type n = last - first;
        for(size_type i = 0; i < n; ++i){
            allocator_traits_type::destroy(get_stored_allocator(), arr_ + front_capacity_ + pos_from_begin + i);
        }
        for(size_type i = pos_from_begin + n; i < size_; ++i){
            allocator_traits_type::construct(get_stored_allocator(), arr_ + front_capacity_ + i - n, std::move_if_noexcept(arr_[front_capacity_ + i]));
            allocator_traits_type::destroy(get_stored_allocator(), arr_ + front_capacity_ + i);
        }
        size_ -= n;
        return begin() + pos_from_begin;
    }
    iterator erase(iterator first, iterator last){
        return erase(static_cast<const_iterator>(first), static_cast<const_iterator>(last));
    }

    reference operator[](size_type index) noexcept{
        return arr_[front_capacity_ + index];
    }
    const_reference operator[](size_type index) const noexcept{
        return arr_[front_capacity_ + index];
    }
    reference at(size_type index){
        if(index >= size_) throw std::out_of_range("devector::at");
        return (*this)[index];
    }
    const_reference at(size_type index) const{
        if(index >= size_) throw std::out_of_range("devector::at");
        return (*this)[index];
    }
    reference front() noexcept{
        return (*this)[0];
    }
    const_reference front() const noexcept{
        return (*this)[0];
    }
    reference back() noexcept{
        return (*this)[size_ - 1];
    }
    const_reference back() const noexcept{
        return (*this)[size_ - 1];
    }
    pointer data() noexcept{
        return arr_ + front_capacity_;
    }
    const_pointer data() const noexcept{
        return arr_ + front_capacity_;
    }
    void swap_resources(devector& other) noexcept{
        std::swap(arr_, other.arr_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(front_capacity_, other.front_capacity_);
    }
    void swap(devector& other) noexcept(propagate_on_container_swap::value || is_always_equal::value){
        if constexpr (propagate_on_container_swap::value){
            std::swap(get_stored_allocator(), other.get_stored_allocator());
        }
        else if constexpr (!is_always_equal::value){
            if(get_stored_allocator() != other.get_stored_allocator()){
                throw(std::runtime_error("devector::swap"));
            }
        }
        swap_resources(other);
    }
    void clear() noexcept{
        destroy_n(size_);
        size_ = 0;
    }
};

}

#endif