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

public:
    // construct/copy/destruct
    devector() noexcept : Alloc(), arr_(nullptr), size_(0), capacity_(0), front_capacity_(0) {}

    explicit devector(const allocator_type& allocator) noexcept : Alloc(allocator), arr_(nullptr), size_(0), capacity_(0), front_capacity_(0) {}

    devector(size_type new_capacity, reserve_only_tag_t, const allocator_type& allocator = allocator_type()) : devector(allocator) {
        if (new_capacity == 0) return;
        pointer newarr = allocator_traits_type::allocate(get_allocator(), new_capacity);
        arr_ = newarr;
        capacity_ = new_capacity;
    }

    devector(size_type front_cap, size_type back_cap, reserve_only_tag_t, const allocator_type& allocator = allocator_type())
            : devector(front_cap + back_cap, reserve_only_tag_t{}, allocator) {
        front_capacity_ = front_cap;
    }

    explicit devector(size_type new_capacity, const allocator_type& allocator = allocator_type()) : devector(new_capacity, reserve_only_tag_t{}, allocator) {
        size_type index = 0;
        try{
            for(; index < capacity_; ++index){
                allocator_traits_type::construct(get_allocator(), arr_ + index);
            }
        }
        catch(...){
            for(size_type constructed_index = 0; constructed_index < index; ++constructed_index){
                allocator_traits_type::destroy(get_allocator(), arr_ + constructed_index);
            }
            allocator_traits_type::deallocate(get_allocator(), arr_, capacity_);
            throw;
        }
        size_ = capacity_;
    }

    devector(size_type new_capacity, const_reference value, const allocator_type& allocator = allocator_type())
            : devector(new_capacity, reserve_only_tag_t{}, allocator) {
        size_type index = 0;
        try{
            for(; index < capacity_; ++index){
                allocator_traits_type::construct(get_allocator(), arr_ + index, value);
            }
        }
        catch(...){
            for(size_type constructed_index = 0; constructed_index < index; ++constructed_index){
                allocator_traits_type::destroy(get_allocator(), arr_ + constructed_index);
            }
            allocator_traits_type::deallocate(get_allocator(), arr_, capacity_);
            throw;
        }
        size_ = capacity_;
    }

    template<typename InputIterator>
    requires std::input_iterator<InputIterator>
    devector(InputIterator first, InputIterator last, const allocator_type & allocator = allocator_type()) 
            : devector(static_cast<size_type>(std::distance(first, last)), reserve_only_tag_t{}, allocator){
        size_type index = 0;
        try{
            for(; index < capacity_; ++index, ++first){
                allocator_traits_type::construct(get_allocator(), arr_ + index, *first);
            }
        }
        catch(...){
            for(size_type constructed_index = 0; constructed_index < index; ++constructed_index){
                allocator_traits_type::destroy(get_allocator(), arr_ + constructed_index);
            }
            allocator_traits_type::deallocate(get_allocator(), arr_, capacity_);
            throw;
        }
        size_ = capacity_;
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
};

}

#endif