#ifndef DEVECTOR_H
#define DEVECTOR_H

#include <memory>

namespace DevectorNameSpace{

template<typename T, typename Alloc = std::allocator<T>>
class devector : private Alloc {
public:
    typedef T                                                                  value_type;
    typedef Alloc                                                              allocator_type;
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
    
};

}

#endif