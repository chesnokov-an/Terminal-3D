#ifndef DEVECTOR_H
#define DEVECTOR_H

#include <memory>
#include <concepts>

namespace DevectorNameSpace{

struct reserve_only_tag_t {};

template<typename T, typename Alloc = std::allocator<T>>
class devector final : private Alloc {
public:
    typedef T                                                                  value_type;
    typedef Alloc                                                              allocator_type;
    typedef std::allocator_traits<allocator_type>                              allocator_traits_type;
    typedef allocator_type                                                     stored_allocator_type;
    typedef typename std::allocator_traits<allocator_type>::pointer            pointer;
    typedef typename std::allocator_traits<allocator_type>::const_pointer      const_pointer;
    typedef value_type&                                                        reference;
    typedef const value_type&                                                  const_reference;
    typedef typename std::allocator_traits<allocator_type>::size_type          size_type;
    typedef typename std::allocator_traits<allocator_type>::difference_type    difference_type;
    typedef pointer                                                            iterator;
    typedef const_pointer                                                      const_iterator;
    typedef std::reverse_iterator<iterator>                                    reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                              const_reverse_iterator;

    typedef typename std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment    propagate_on_container_copy_assignment;
    typedef typename std::allocator_traits<allocator_type>::propagate_on_container_move_assignment    propagate_on_container_move_assignment;
    typedef typename std::allocator_traits<allocator_type>::propagate_on_container_swap               propagate_on_container_swap;
    typedef typename std::allocator_traits<allocator_type>::is_always_equal                           is_always_equal;

private:
    pointer arr_;
    size_type size_;
    size_type capacity_;
    size_type front_capacity_;

private:
    void destroy_n(size_type n){
        for(size_type index = 0; index < n; ++index){
            allocator_traits_type::destroy(get_stored_allocator(), arr_ + index);
        }
    }
    void destroy_n_and_deallocate(size_type n){
        destroy_n(n);
        allocator_traits_type::deallocate(get_stored_allocator(), arr_, capacity_);
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
        size_type index = 0;
        try{
            for(; index < new_capacity; ++index){
                allocator_traits_type::construct(get_stored_allocator(), arr_ + index, value);
            }
        }
        catch(...){
            destroy_n_and_deallocate(index);
            throw;
        }
        size_ = new_capacity;
    }

    explicit devector(size_type new_capacity, const allocator_type& allocator = allocator_type()) : devector(new_capacity, value_type(), allocator) {}

    template<typename ForwardIterator>
    requires std::forward_iterator<ForwardIterator>
    devector(ForwardIterator first, ForwardIterator last, const allocator_type & allocator = allocator_type()) 
            : devector(static_cast<size_type>(std::distance(first, last)), reserve_only_tag_t{}, allocator){
        size_type index = 0;
        try{
            for(; index < capacity_; ++index, ++first){
                allocator_traits_type::construct(get_stored_allocator(), arr_ + index, *first);
            }
        }
        catch(...){
            destroy_n_and_deallocate(index);
            throw;
        }
        size_ = capacity_;
    }

    template<typename InputIterator>
    requires std::input_iterator<InputIterator> && (!std::forward_iterator<InputIterator>)
    devector(InputIterator first, InputIterator last, const allocator_type& allocator = allocator_type())
            : devector(allocator) {
        try{
            for (; first != last; ++first) {
                push_back(*first);
            }
        }
        catch(...){
            destroy_n_and_deallocate(size_);
            throw;
        }
    }
    
    devector(const devector& other, const allocator_type& allocator)
            : devector(other.begin(), other.end(), allocator) {}

    devector(const devector& other) : devector(other, allocator_traits_type::select_on_container_copy_construction(other.get_stored_allocator())) {}

    devector(devector&& other, const allocator_type& allocator) : devector(allocator) {
        if(allocator == other.get_stored_allocator()){
            std::swap(arr_, other.arr_);
            std::swap(size_, other.size_);
            std::swap(capacity_, other.capacity_);
            std::swap(front_capacity_, other.front_capacity_);
            return;
        }
        pointer newarr = allocator_traits_type::allocate(get_stored_allocator(), other.size_);
        size_type index = 0;
        try{
            for(; index < other.size_; ++index){
                allocator_traits_type::construct(get_stored_allocator(), newarr + index, std::move(other[index]));
            }
        }
        catch(...){
            for(size_type constructed_index = 0; constructed_index < index; ++constructed_index){
                allocator_traits_type::destroy(get_stored_allocator(), newarr + constructed_index);
            }
            allocator_traits_type::deallocate(get_stored_allocator(), newarr, other.size_);
            throw;
        }
        arr_ = newarr;
        size_ = other.size_;
        capacity_ = other.size_;
        other.arr_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        other.front_capacity_ = 0;
    }

    devector(devector&& other) noexcept : devector(other, other.get_stored_allocator()) {}

    devector(const std::initializer_list<T>& il, const allocator_type& allocator = allocator_type()) : devector(il.begin(), il.end(), allocator) {}

    devector& operator=(const devector& other){
        if(*this == other) return *this;
        allocator_type new_allocator = propagate_on_container_copy_assignment::value ? other.get_stored_allocator() : get_stored_allocator();
        pointer newarr = allocator_traits_type::allocate(new_allocator, other.capacity_);
        size_type index = 0;
        try{
            for(; index < other.size_; ++index){
                allocator_traits_type::construct(new_allocator, newarr + index, *(other.arr_ + index));
            }
        }
        catch(...){
            for(size_type constructed_index = 0; constructed_index < index; ++constructed_index){
                allocator_traits_type::destroy(new_allocator, newarr + constructed_index);
            }
            allocator_traits_type::deallocate(new_allocator, newarr, other.capacity_);
            throw;
        }
        destroy_n_and_deallocate(size_);
        get_stored_allocator() = new_allocator;
        arr_ = newarr;
        size_ = other.size_;
        capacity_ = other.capacity_;
        front_capacity_ = other.front_capacity_;
        return *this;
    }
    

    // public member functions
    allocator_type get_allocator() const noexcept{
        return static_cast<const allocator_type&>(*this);
    }
    const allocator_type& get_stored_allocator() const noexcept{
        return static_cast<const allocator_type&>(*this);
    }
    allocator_type& get_stored_allocator() noexcept{
        return static_cast<allocator_type&>(*this);
    }
    reference operator[](size_type index) noexcept{
        return arr_[front_capacity_ + index];
    }
    const_reference operator[](size_type index) const noexcept{
        return arr_[front_capacity_ + index];
    }
    reference at(size_type index){
        if(index >= size_) throw std::out_of_range();
        return (*this)[index];
    }
    const_reference at(size_type index) const{
        if(index >= size_) throw std::out_of_range();
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

    // void swap(devector& other) noexcept(propagate_on_container_swap::value || is_always_equal::value){
    //     if constexpr (propagate_on_container_copy_assignment::value){
    //         get_stored_allocator() = other.get_stored_allocator();
    //     }
    //     std::swap(arr_, other.arr_);
    //     std::swap(size_, other.size_);
    //     std::swap(capacity_, other.capacity_);
    //     std::swap(front_capacity_, other.front_capacity_);
    // }
};

}

#endif