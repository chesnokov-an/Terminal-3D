#include <gtest/gtest.h>
#include <vector>
#include <list>
#include <memory>
#include <stdexcept>
#include <Devector/Devector.hpp>
#include <vector>

using namespace DevectorNameSpace;

// Test fixture for devector tests
class DevectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset any static state if needed
    }

    void TearDown() override {
        // Clean up after each test
    }
};

// Basic construction and destruction tests
TEST_F(DevectorTest, DefaultConstructor) {
    devector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_EQ(v.front_free_capacity(), 0);
    EXPECT_EQ(v.back_free_capacity(), 0);
    EXPECT_TRUE(v.empty());
}

TEST_F(DevectorTest, ConstructorWithAllocator) {
    std::allocator<int> alloc;
    devector<int, std::allocator<int>> v(alloc);
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_TRUE(v.empty());
}

TEST_F(DevectorTest, ReserveOnlyConstructor) {
    devector<int> v(10, reserve_only_tag_t{});
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 10);
    EXPECT_EQ(v.front_free_capacity(), 0);
    EXPECT_EQ(v.back_free_capacity(), 10);
}

TEST_F(DevectorTest, FrontBackReserveConstructor) {
    devector<int> v(5, 10, reserve_only_tag_t{});
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 15);
    EXPECT_EQ(v.front_free_capacity(), 5);
    EXPECT_EQ(v.back_free_capacity(), 10);
}

TEST_F(DevectorTest, SizeValueConstructor) {
    devector<int> v(5, 42);
    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], 42);
    }
}

TEST_F(DevectorTest, SizeConstructor) {
    devector<int> v(5);
    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], 0); // value-initialized
    }
}

TEST_F(DevectorTest, ForwardIteratorConstructor) {
    std::vector<int> src = {1, 2, 3, 4, 5};
    devector<int> v(src.begin(), src.end());
    EXPECT_EQ(v.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i + 1));
    }
}

TEST_F(DevectorTest, InputIteratorConstructor) {
    std::list<int> src = {1, 2, 3, 4, 5};
    devector<int> v(src.begin(), src.end());
    EXPECT_EQ(v.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i + 1));
    }
}

TEST_F(DevectorTest, CopyConstructor) {
    devector<int> original(5, 42);
    devector<int> copy(original);
    EXPECT_EQ(original.size(), copy.size());
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(copy[i], 42);
    }
}

TEST_F(DevectorTest, CopyConstructorWithAllocator) {
    devector<int> original(5, 42);
    devector<int> copy(original, std::allocator<int>());
    EXPECT_EQ(original.size(), copy.size());
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(copy[i], 42);
    }
}

TEST_F(DevectorTest, MoveConstructor) {
    devector<int> original(5, 42);
    devector<int> moved(std::move(original));
    
    EXPECT_EQ(moved.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(moved[i], 42);
    }
    
    // Original should be in valid but empty state
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(original.capacity(), 0);
}

TEST_F(DevectorTest, MoveConstructorWithAllocator) {
    devector<std::vector<int>> original(5, std::vector<int>{1, 2, 3});
    devector<std::vector<int>> moved(std::move(original), std::allocator<std::vector<int>>());
    
    EXPECT_EQ(moved.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(moved[i], (std::vector<int>{1, 2, 3}));
    }
}

TEST_F(DevectorTest, InitializerListConstructor) {
    devector<int> v = {1, 2, 3, 4, 5};
    EXPECT_EQ(v.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i + 1));
    }
}

// Assignment operator tests
TEST_F(DevectorTest, CopyAssignment) {
    devector<int> original(5, 42);
    devector<int> copy;
    copy = original;
    
    EXPECT_EQ(original.size(), copy.size());
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(copy[i], 42);
    }
}

TEST_F(DevectorTest, MoveAssignment) {
    devector<int> original(5, 42);
    devector<int> moved;
    moved = std::move(original);
    
    EXPECT_EQ(moved.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(moved[i], 42);
    }
    
    // Original should be in valid but empty state
    EXPECT_EQ(original.size(), 0);
}

TEST_F(DevectorTest, InitializerListAssignment) {
    devector<int> v;
    v = {1, 2, 3, 4, 5};
    
    EXPECT_EQ(v.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i + 1));
    }
}

// Assignment methods tests
TEST_F(DevectorTest, AssignIterators) {
    devector<int> v;
    std::vector<int> src = {1, 2, 3, 4, 5};
    v.assign(src.begin(), src.end());
    
    EXPECT_EQ(v.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i + 1));
    }
}

TEST_F(DevectorTest, AssignSizeValue) {
    devector<int> v;
    v.assign(5, 42);
    
    EXPECT_EQ(v.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], 42);
    }
}

TEST_F(DevectorTest, AssignInitializerList) {
    devector<int> v;
    v.assign({1, 2, 3, 4, 5});
    
    EXPECT_EQ(v.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i + 1));
    }
}

// Iterator tests
TEST_F(DevectorTest, Iterators) {
    devector<int> v = {1, 2, 3, 4, 5};
    
    // Forward iteration
    int expected = 1;
    for (auto it = v.begin(); it != v.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
    
    // Const iteration
    expected = 1;
    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
    
    // Reverse iteration
    expected = 5;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
    
    // Const reverse iteration
    expected = 5;
    for (auto it = v.crbegin(); it != v.crend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
}

// Element access tests
TEST_F(DevectorTest, ElementAccess) {
    devector<int> v = {1, 2, 3, 4, 5};
    
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[4], 5);
    
    EXPECT_EQ(v.at(0), 1);
    EXPECT_EQ(v.at(2), 3);
    EXPECT_EQ(v.at(4), 5);
    
    EXPECT_THROW(v.at(5), std::out_of_range);
    EXPECT_THROW(v.at(10), std::out_of_range);
    
    EXPECT_EQ(v.front(), 1);
    EXPECT_EQ(v.back(), 5);
    
    EXPECT_EQ(*(v.data()), 1);
    EXPECT_EQ(v.data()[2], 3);
}

// Capacity tests
TEST_F(DevectorTest, CapacityMethods) {
    devector<int> v(5, 10, reserve_only_tag_t{});
    
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 15);
    EXPECT_EQ(v.front_free_capacity(), 5);
    EXPECT_EQ(v.back_free_capacity(), 10);
    EXPECT_TRUE(v.empty());
    
    v = {1, 2, 3};
    EXPECT_EQ(v.size(), 3);
    EXPECT_FALSE(v.empty());
    EXPECT_GT(v.max_size(), 0);
}

// Resize tests
// TEST_F(DevectorTest, ResizeBackWithValue) {
//     devector<int> v = {1, 2, 3};
//     v.resize_back(5, 42);
    
//     EXPECT_EQ(v.size(), 5);
//     EXPECT_EQ(v[0], 1);
//     EXPECT_EQ(v[1], 2);
//     EXPECT_EQ(v[2], 3);
//     EXPECT_EQ(v[3], 42);
//     EXPECT_EQ(v[4], 42);
    
//     v.resize_back(2);
//     EXPECT_EQ(v.size(), 2);
//     EXPECT_EQ(v[0], 1);
//     EXPECT_EQ(v[1], 2);
// }

// TEST_F(DevectorTest, ResizeBackDefault) {
//     devector<int> v = {1, 2, 3};
//     v.resize_back(5);
    
//     EXPECT_EQ(v.size(), 5);
//     EXPECT_EQ(v[0], 1);
//     EXPECT_EQ(v[1], 2);
//     EXPECT_EQ(v[2], 3);
//     EXPECT_EQ(v[3], 0);
//     EXPECT_EQ(v[4], 0);
// }

// TEST_F(DevectorTest, ResizeFrontWithValue) {
//     devector<int> v = {1, 2, 3};
//     v.resize_front(5, 42);
    
//     EXPECT_EQ(v.size(), 5);
//     EXPECT_EQ(v[0], 42);
//     EXPECT_EQ(v[1], 42);
//     EXPECT_EQ(v[2], 1);
//     EXPECT_EQ(v[3], 2);
//     EXPECT_EQ(v[4], 3);
    
//     v.resize_front(2);
//     EXPECT_EQ(v.size(), 2);
//     EXPECT_EQ(v[0], 1);
//     EXPECT_EQ(v[1], 2);
// }

// TEST_F(DevectorTest, ResizeFrontDefault) {
//     devector<int> v = {1, 2, 3};
//     v.resize_front(5);
    
//     EXPECT_EQ(v.size(), 5);
//     EXPECT_EQ(v[0], 0);
//     EXPECT_EQ(v[1], 0);
//     EXPECT_EQ(v[2], 1);
//     EXPECT_EQ(v[3], 2);
//     EXPECT_EQ(v[4], 3);
// }

// TEST_F(DevectorTest, ResizeWithValue) {
//     devector<int> v = {1, 2, 3};
//     v.resize(5, 42);
    
//     EXPECT_EQ(v.size(), 5);
//     // Default resize should resize back
//     EXPECT_EQ(v[0], 1);
//     EXPECT_EQ(v[1], 2);
//     EXPECT_EQ(v[2], 3);
//     EXPECT_EQ(v[3], 42);
//     EXPECT_EQ(v[4], 42);
// }

// TEST_F(DevectorTest, ResizeDefault) {
//     devector<int> v = {1, 2, 3};
//     v.resize(5);
    
//     EXPECT_EQ(v.size(), 5);
//     // Default resize should resize back
//     EXPECT_EQ(v[0], 1);
//     EXPECT_EQ(v[1], 2);
//     EXPECT_EQ(v[2], 3);
//     EXPECT_EQ(v[3], 0);
//     EXPECT_EQ(v[4], 0);
// }

// Clear test
TEST_F(DevectorTest, Clear) {
    devector<int> v = {1, 2, 3, 4, 5};
    v.clear();
    
    EXPECT_EQ(v.size(), 0);
    EXPECT_TRUE(v.empty());
}

// Allocator tests
TEST_F(DevectorTest, GetAllocator) {
    std::allocator<int> alloc;
    devector<int, std::allocator<int>> v(alloc);
    
    EXPECT_EQ(v.get_allocator(), alloc);
    EXPECT_EQ(v.get_stored_allocator(), alloc);
}

// Exception safety tests with custom type that can throw
struct ThrowOnCopy {
    int value;
    static int copy_count;
    static int throw_after;
    
    ThrowOnCopy(int v = 0) : value(v) {}
    
    ThrowOnCopy(const ThrowOnCopy& other) : value(other.value) {
        if (++copy_count >= throw_after) {
            throw std::runtime_error("Copy limit reached");
        }
    }
    
    ThrowOnCopy(ThrowOnCopy&& other) noexcept : value(other.value) {
        other.value = 0;
    }
    
    ThrowOnCopy& operator=(const ThrowOnCopy& other) {
        if (++copy_count >= throw_after) {
            throw std::runtime_error("Copy limit reached");
        }
        value = other.value;
        return *this;
    }
    
    ThrowOnCopy& operator=(ThrowOnCopy&& other) noexcept {
        value = other.value;
        other.value = 0;
        return *this;
    }
};

int ThrowOnCopy::copy_count = 0;
int ThrowOnCopy::throw_after = 0;

TEST_F(DevectorTest, ExceptionSafetyInCopyConstructor) {
    ThrowOnCopy::copy_count = 0;
    ThrowOnCopy::throw_after = 8; // Throw after 3 copies ??? NOT 8 ???
    devector<ThrowOnCopy> original(5, ThrowOnCopy(42));
    try {
        devector<ThrowOnCopy> copy(original);
        FAIL() << "Expected exception to be thrown";
    } catch (const std::runtime_error&) {
        // Exception should be thrown, check that no resources are leaked
        EXPECT_EQ(original.size(), 5);
        for (size_t i = 0; i < 5; ++i) {
            EXPECT_EQ(original[i].value, 42);
        }
    }
}

TEST_F(DevectorTest, ExceptionSafetyInCopyAssignment) {
    ThrowOnCopy::copy_count = 0;
    ThrowOnCopy::throw_after = 8; // Throw after 3 copies ??? NOT 8 ???
    
    devector<ThrowOnCopy> original(5, ThrowOnCopy(42));
    devector<ThrowOnCopy> target(2, ThrowOnCopy(10));
    
    try {
        target = original;
        FAIL() << "Expected exception to be thrown";
    } catch (const std::runtime_error&) {
        // Exception should be thrown, target should remain unchanged
        EXPECT_EQ(target.size(), 2);
        for (size_t i = 0; i < 2; ++i) {
            EXPECT_EQ(target[i].value, 10);
        }
        
        // Original should remain unchanged
        EXPECT_EQ(original.size(), 5);
        for (size_t i = 0; i < 5; ++i) {
            EXPECT_EQ(original[i].value, 42);
        }
    }
}

TEST_F(DevectorTest, ExceptionSafetyInMoveConstructorWithUnequalAllocators) {
    // This test requires different allocators to trigger the element-wise move path
    // For simplicity, we'll use the same allocator type but different instances
    // In real scenario, you might use stateful allocators
    
    ThrowOnCopy::copy_count = 0;
    ThrowOnCopy::throw_after = 100; // Don't throw on copy
    
    devector<ThrowOnCopy> original(5, ThrowOnCopy(42));
    
    // This should use the element-wise move path but won't throw since we're not copying
    devector<ThrowOnCopy> moved(std::move(original));
    
    EXPECT_EQ(moved.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(moved[i].value, 42);
    }
}

// Test with custom allocator for better memory tracking
template<typename T>
class TrackingAllocator : public std::allocator<T> {
public:
    static int allocations;
    static int deallocations;
    
    template<typename U>
    struct rebind {
        typedef TrackingAllocator<U> other;
    };
    
    T* allocate(std::size_t n) {
        allocations++;
        return std::allocator<T>::allocate(n);
    }
    
    void deallocate(T* p, std::size_t n) {
        deallocations++;
        std::allocator<T>::deallocate(p, n);
    }
    
    static void reset() {
        allocations = 0;
        deallocations = 0;
    }
};

template<typename T>
int TrackingAllocator<T>::allocations = 0;

template<typename T>
int TrackingAllocator<T>::deallocations = 0;

TEST_F(DevectorTest, MemoryManagementWithTrackingAllocator) {
    TrackingAllocator<int>::reset();
    
    {
        devector<int, TrackingAllocator<int>> v(10, reserve_only_tag_t{});
        EXPECT_EQ(TrackingAllocator<int>::allocations, 1);
        EXPECT_EQ(TrackingAllocator<int>::deallocations, 0);
        
        v = {1, 2, 3, 4, 5};
        // Assignment might reallocate, so we can't make precise assertions
        // But we can check that deallocations happen properly on destruction
    }
    
    // All allocations should be deallocated by now
    EXPECT_EQ(TrackingAllocator<int>::allocations, TrackingAllocator<int>::deallocations);
}

// Edge case tests
// TEST_F(DevectorTest, EmptyVectorOperations) {
//     devector<int> v;
    
//     EXPECT_TRUE(v.empty());
//     EXPECT_EQ(v.size(), 0);
//     EXPECT_EQ(v.capacity(), 0);
    
//     // These should not crash
//     v.clear();
//     v.resize(0);
//     v.resize_front(0);
//     v.resize_back(0);
// }

// TEST_F(DevectorTest, SingleElementVector) {
//     devector<int> v = {42};
    
//     EXPECT_EQ(v.size(), 1);
//     EXPECT_EQ(v[0], 42);
//     EXPECT_EQ(v.front(), 42);
//     EXPECT_EQ(v.back(), 42);
//     EXPECT_EQ(*(v.data()), 42);
    
//     v.resize_back(3, 10);
//     EXPECT_EQ(v.size(), 3);
//     EXPECT_EQ(v[0], 42);
//     EXPECT_EQ(v[1], 10);
//     EXPECT_EQ(v[2], 10);
    
//     v.resize_front(1);
//     EXPECT_EQ(v.size(), 1);
//     EXPECT_EQ(v[0], 42);
// }

TEST_F(DevectorTest, MaxSize) {
    devector<int> v;
    EXPECT_GT(v.max_size(), 0);
}

// Test with different types
// TEST_F(DevectorTest, StringVector) {
//     devector<std::string> v = {"hello", "world"};
    
//     EXPECT_EQ(v.size(), 2);
//     EXPECT_EQ(v[0], "hello");
//     EXPECT_EQ(v[1], "world");
    
//     v.resize_back(4, "test");
//     EXPECT_EQ(v.size(), 4);
//     EXPECT_EQ(v[2], "test");
//     EXPECT_EQ(v[3], "test");
    
//     v.resize_front(1);
//     EXPECT_EQ(v.size(), 1);
//     EXPECT_EQ(v[0], "hello");
// }

TEST_F(DevectorTest, UniquePtrVector) {
    devector<std::unique_ptr<int>> v;
    v.push_back(std::make_unique<int>(42));
    v.push_back(std::make_unique<int>(24));
    
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(*v[0], 42);
    EXPECT_EQ(*v[1], 24);
    
    // Move should work
    devector<std::unique_ptr<int>> moved = std::move(v);
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(moved.size(), 2);
    EXPECT_EQ(*moved[0], 42);
    EXPECT_EQ(*moved[1], 24);
}

// Self-assignment tests
TEST_F(DevectorTest, SelfCopyAssignment) {
    devector<int> v = {1, 2, 3};
    v = v;
    
    // Should remain unchanged
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(DevectorTest, SelfMoveAssignment) {
    devector<int> v = {1, 2, 3};
    v = std::move(v);
    
    // Should remain in valid state (implementation defined)
    // At minimum, shouldn't crash
    EXPECT_TRUE(v.size() == 3 || v.size() == 0);
}

// Тесты для разных аллокаторов
template<typename T>
class StatefulAllocator {
public:
    using value_type = T;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap = std::false_type;
    using is_always_equal = std::false_type;

    int id;

    StatefulAllocator(int id = 0) noexcept : id(id) {}

    template<typename U>
    StatefulAllocator(const StatefulAllocator<U>& other) noexcept : id(other.id) {}

    T* allocate(std::size_t n) {
        return std::allocator<T>().allocate(n);
    }

    void deallocate(T* p, std::size_t n) {
        std::allocator<T>().deallocate(p, n);
    }

    bool operator==(const StatefulAllocator& other) const noexcept {
        return id == other.id;
    }

    bool operator!=(const StatefulAllocator& other) const noexcept {
        return id != other.id;
    }
};

// Тест на перемещение с разными аллокаторами
TEST_F(DevectorTest, MoveConstructorWithDifferentAllocators) {
    devector<int, StatefulAllocator<int>> original(5, 42, StatefulAllocator<int>(1));
    devector<int, StatefulAllocator<int>> moved(std::move(original), StatefulAllocator<int>(2));
    
    EXPECT_EQ(moved.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(moved[i], 42);
    }
}

// Тест на перемещающее присваивание с разными аллокаторами
TEST_F(DevectorTest, MoveAssignmentWithDifferentAllocators) {
    devector<int, StatefulAllocator<int>> original(5, 42, StatefulAllocator<int>(1));
    devector<int, StatefulAllocator<int>> target(2, 10, StatefulAllocator<int>(2));
    
    target = std::move(original);
    
    EXPECT_EQ(target.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(target[i], 42);
    }
}

// // Тест на swap с разными аллокаторами (должен завершаться аварийно)
// TEST_F(DevectorTest, SwapWithDifferentAllocatorsTerminates) {
//     devector<int, StatefulAllocator<int>> v1(3, 1, StatefulAllocator<int>(1));
//     devector<int, StatefulAllocator<int>> v2(3, 2, StatefulAllocator<int>(2));
    
//     EXPECT_DEATH(v1.swap(v2), "");
// }

// Тест на swap с одинаковыми аллокаторами
TEST_F(DevectorTest, SwapWithSameAllocators) {
    StatefulAllocator<int> alloc(1);
    devector<int, StatefulAllocator<int>> v1(3, 1, alloc);
    devector<int, StatefulAllocator<int>> v2(3, 2, alloc);
    
    v1.swap(v2);
    
    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v2.size(), 3);
    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(v1[i], 2);
        EXPECT_EQ(v2[i], 1);
    }
}

// Тест на propagate_on_container_copy_assignment = true
template<typename T>
struct PropagatingAllocator {
    using value_type = T;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::false_type;

    int id;

    PropagatingAllocator(int id = 0) noexcept : id(id) {}

    template<typename U>
    PropagatingAllocator(const PropagatingAllocator<U>& other) noexcept : id(other.id) {}

    T* allocate(std::size_t n) {
        return std::allocator<T>().allocate(n);
    }

    void deallocate(T* p, std::size_t n) {
        std::allocator<T>().deallocate(p, n);
    }

    bool operator==(const PropagatingAllocator& other) const noexcept {
        return id == other.id;
    }

    bool operator!=(const PropagatingAllocator& other) const noexcept {
        return id != other.id;
    }
};

// Тест на копирующее присваивание с propagate_on_container_copy_assignment = true
TEST_F(DevectorTest, CopyAssignmentWithPropagatingAllocator) {
    devector<int, PropagatingAllocator<int>> original(5, 42, PropagatingAllocator<int>(1));
    devector<int, PropagatingAllocator<int>> copy(2, 10, PropagatingAllocator<int>(2));
    
    copy = original;
    
    EXPECT_EQ(copy.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(copy[i], 42);
    }
    // Аллокатор должен был скопироваться
    EXPECT_EQ(copy.get_stored_allocator().id, 1);
}

// Тест на перемещающее присваивание с propagate_on_container_move_assignment = true
TEST_F(DevectorTest, MoveAssignmentWithPropagatingAllocator) {
    devector<int, PropagatingAllocator<int>> original(5, 42, PropagatingAllocator<int>(1));
    devector<int, PropagatingAllocator<int>> target(2, 10, PropagatingAllocator<int>(2));
    
    target = std::move(original);
    
    EXPECT_EQ(target.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(target[i], 42);
    }
    // Аллокатор должен был переместиться
    EXPECT_EQ(target.get_stored_allocator().id, 1);
}

// Тест на swap с propagate_on_container_swap = true
TEST_F(DevectorTest, SwapWithPropagatingAllocator) {
    devector<int, PropagatingAllocator<int>> v1(3, 1, PropagatingAllocator<int>(1));
    devector<int, PropagatingAllocator<int>> v2(3, 2, PropagatingAllocator<int>(2));
    
    v1.swap(v2);
    
    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v2.size(), 3);
    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(v1[i], 2);
        EXPECT_EQ(v2[i], 1);
    }
    // Аллокаторы должны были обменяться
    EXPECT_EQ(v1.get_stored_allocator().id, 2);
    EXPECT_EQ(v2.get_stored_allocator().id, 1);
}

// Тест на конструктор с итераторами для input iterator с исключением
// TEST_F(DevectorTest, InputIteratorConstructorWithException) {
//     struct ThrowingInputIterator {
//         using iterator_category = std::input_iterator_tag;
//         using value_type = int;
//         using difference_type = std::ptrdiff_t;
//         using pointer = int*;
//         using reference = int&;
        
//         int count;
//         int max_before_throw;
        
//         ThrowingInputIterator(int max_throw) : count(0), max_before_throw(max_throw) {}
        
//         ThrowingInputIterator& operator++() {
//             ++count;
//             if (count >= max_before_throw) {
//                 throw std::runtime_error("Test exception");
//             }
//             return *this;
//         }
        
//         ThrowingInputIterator operator++(int) {
//             ThrowingInputIterator tmp = *this;
//             ++(*this);
//             return tmp;
//         }
        
//         int operator*() const {
//             return count;
//         }
        
//         bool operator==(const ThrowingInputIterator& other) const {
//             return count == other.count;
//         }
//     };
    
//     ThrowingInputIterator begin(3), end(5);
    
//     devector<int> v;
//     EXPECT_THROW(v.assign(begin, end), std::runtime_error);
// }

// Тест на само-перемещающее присваивание
TEST_F(DevectorTest, SelfMoveAssignmentWithNonEqualAllocators) {
    devector<int, StatefulAllocator<int>> v(3, 42, StatefulAllocator<int>(1));
    
    // Это не должно приводить к неопределенному поведению
    v = std::move(v);
    
    // Контейнер должен остаться в валидном состоянии
    EXPECT_GE(v.size(), 0);
}

// Тест на empty в destroy_n_and_deallocate
TEST_F(DevectorTest, DestroyEmptyDevector) {
    devector<int> v;
    // Не должно падать при разрушении пустого контейнера
}

// Тест на итераторы с непустым front_free_capacity
TEST_F(DevectorTest, IteratorsWithFrontCapacity) {
    devector<int> v(3, 7, reserve_only_tag_t{});
    v.assign({1, 2, 3});
    
    EXPECT_EQ(v.front_free_capacity(), 0);
    EXPECT_EQ(v.size(), 3);
    
    int expected = 1;
    for (auto it = v.begin(); it != v.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
}

// Тест на data() с front_capacity
TEST_F(DevectorTest, DataWithFrontCapacity) {
    devector<int> v(2, 3, reserve_only_tag_t{});
    v.assign({1, 2, 3});
    
    EXPECT_EQ(*v.data(), 1);
    EXPECT_EQ(v.data()[1], 2);
    EXPECT_EQ(v.data()[2], 3);
}