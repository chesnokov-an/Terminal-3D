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

TEST_F(DevectorTest, CopyAssignmentBigCapacity) {
    devector<int> original(5, 42);
    devector<int> copy(5, reserve_only_tag_t{});
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

TEST_F(DevectorTest, SizeValueAssignment) {
    devector<int> v;
    v.assign(3, 50);
    
    EXPECT_EQ(v.size(), 3);
    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(v[i], 50);
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

TEST_F(DevectorTest, MoveAssignmentWithDifferentAllocatorsBigCapacity) {
    devector<int, StatefulAllocator<int>> original(5, 42, StatefulAllocator<int>(1));
    devector<int, StatefulAllocator<int>> target(5, 10, StatefulAllocator<int>(2));
    
    target = std::move(original);
    
    EXPECT_EQ(target.size(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(target[i], 42);
    }
}

// Тест на swap с разными аллокаторами (должен завершаться аварийно)
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
TEST_F(DevectorTest, InputIteratorConstructorWithException) {
    struct MyInputIterator {
        using iterator_category = std::input_iterator_tag;
        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using pointer = int*;
        using reference = int&;

        int ptr_;
        int counter_;
        
        MyInputIterator(int ptr) : ptr_(ptr), counter_(0) {}
        
        MyInputIterator& operator++() {
            ++ptr_;
            ++counter_;
            if(counter_ == 2){ throw std::runtime_error(""); }
            return *this;
        }
        
        MyInputIterator operator++(int) {
            MyInputIterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        int operator*() const {
            return ptr_;
        }
        
        bool operator==(const MyInputIterator& other) const {
            return ptr_ == other.ptr_;
        }
    };
    
    MyInputIterator begin(3), end(7);
    
    devector<int> v;
    EXPECT_THROW(v.assign(begin, end), std::runtime_error);
}

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

//----------------

// Тесты для reserve_front и reserve_back
TEST_F(DevectorTest, ReserveFrontWithoutReallocation) {
    devector<int> v(5, 10, reserve_only_tag_t{}); // capacity=15, front_capacity=5
    for(int i = 1; i < 4; ++i){
        v.push_back(i);
    }
    
    size_t old_capacity = v.capacity();
    v.reserve_front(3); // Уже есть 5, так что не должно перевыделять
    
    EXPECT_EQ(v.capacity(), old_capacity);
    EXPECT_EQ(v.front_free_capacity(), 5);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(DevectorTest, ReserveFrontWithReallocation) {
    devector<int> v(2, 2, reserve_only_tag_t{}); // capacity=4, front_capacity=2
    for(int i = 1; i < 3; ++i){
        v.push_back(i);
    }
    
    v.reserve_front(7); // Нужно больше front capacity
    
    EXPECT_GE(v.capacity(), 7); // 5 (новый front) + 2 (size)
    EXPECT_EQ(v.front_free_capacity(), 5);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST_F(DevectorTest, ReserveFrontWithTrivialMove) {
    devector<int> v(2, 2, reserve_only_tag_t{});
    for(int i = 1; i < 3; ++i){
        v.push_back(i);
    }
    
    v.reserve_front(5);
    
    EXPECT_GE(v.capacity(), 5);
    EXPECT_EQ(v.front_free_capacity(), 3);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST_F(DevectorTest, ReserveBackWithoutReallocation) {
    devector<int> v(5, 10, reserve_only_tag_t{}); // capacity=15, front_capacity=5
    for(int i = 1; i < 4; ++i){
        v.push_back(i);
    }
    
    size_t old_capacity = v.capacity();
    v.reserve_back(8);
    
    EXPECT_EQ(v.capacity(), old_capacity);
    EXPECT_EQ(v.back_free_capacity(), 7);
    EXPECT_EQ(v.size(), 3);
}

TEST_F(DevectorTest, ReserveBackWithReallocation) {
    devector<int> v(2, 2, reserve_only_tag_t{}); // capacity=4, front_capacity=2
    for(int i = 1; i < 3; ++i){
        v.push_back(i);
    }
    
    v.reserve_back(7); // Нужно больше back capacity
    
    EXPECT_GE(v.capacity(), 7); // 2 (front) + 2 (size) + 5 (новый back)
    EXPECT_EQ(v.back_free_capacity(), 5);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST_F(DevectorTest, ReserveBackWithTrivialMove) {
    devector<int> v(2, 2, reserve_only_tag_t{});
    for(int i = 1; i < 3; ++i){
        v.push_back(i);
    }
    
    v.reserve_back(5);
    
    EXPECT_GE(v.capacity(), 5);
    EXPECT_EQ(v.back_free_capacity(), 3);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST_F(DevectorTest, ReserveFrontExceptionSafety) {
    devector<std::vector<int>> v(2, 2, reserve_only_tag_t{});
    v.push_back(std::vector<int>{1, 2});
    v.push_back(std::vector<int>{3, 4});
    
    // Попытка резервирования с исключением при перемещении
    EXPECT_THROW(v.reserve_front(v.max_size() + 1), std::exception);
    
    // Состояние должно остаться валидным
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0].size(), 2);
    EXPECT_EQ(v[1].size(), 2);
}

TEST_F(DevectorTest, ReserveBackExceptionSafety) {
    devector<std::vector<int>> v(2, 2, reserve_only_tag_t{});
    v.push_back(std::vector<int>{1, 2});
    v.push_back(std::vector<int>{3, 4});
    
    // Попытка резервирования с исключением при перемещении
    EXPECT_THROW(v.reserve_back(v.max_size() + 1), std::exception);
    
    // Состояние должно остаться валидным
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0].size(), 2);
    EXPECT_EQ(v[1].size(), 2);
}

// Тесты для shrink_to_fit
TEST_F(DevectorTest, ShrinkToFitEmpty) {
    devector<int> v(10, 10, reserve_only_tag_t{});
    v.shrink_to_fit();
    
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.front_free_capacity(), 0);
}

TEST_F(DevectorTest, ShrinkToFitNonEmpty) {
    devector<int> v(5, 10, reserve_only_tag_t{}); // capacity=15
    for(int i = 1; i < 4; ++i){
        v.push_back(i);
    }
    
    v.shrink_to_fit();
    
    EXPECT_EQ(v.capacity(), 3);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.front_free_capacity(), 0);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(DevectorTest, ShrinkToFitExceptionSafety) {
    devector<std::vector<int>> v(5, 10, reserve_only_tag_t{});
    v.push_back(std::vector<int>{1});
    v.push_back(std::vector<int>{2});
    v.push_back(std::vector<int>{3});
    // shrink_to_fit не должен бросать исключения при успешном выполнении
    EXPECT_NO_THROW(v.shrink_to_fit());
    
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0].size(), 1);
    EXPECT_EQ(v[1].size(), 1);
    EXPECT_EQ(v[2].size(), 1);
}

// Тесты для push/pop операций
TEST_F(DevectorTest, PushBackWithoutReallocation) {
    devector<int> v(0, 5, reserve_only_tag_t{}); // back_capacity=5
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.back_free_capacity(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(DevectorTest, PushBackWithReallocation) {
    devector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    EXPECT_EQ(v.size(), 3);
    EXPECT_GE(v.capacity(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(DevectorTest, PushFrontWithoutReallocation) {
    devector<int> v(5, 0, reserve_only_tag_t{}); // front_capacity=5
    v.push_front(1);
    v.push_front(2);
    v.push_front(3);
    
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.front_free_capacity(), 2);
    EXPECT_EQ(v[0], 3);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 1);
}

TEST_F(DevectorTest, PushFrontWithReallocation) {
    devector<int> v;
    v.push_front(1);
    v.push_front(2);
    v.push_front(3);
    
    EXPECT_EQ(v.size(), 3);
    EXPECT_GE(v.capacity(), 3);
    EXPECT_EQ(v[0], 3);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 1);
}

TEST_F(DevectorTest, PopBackNonEmpty) {
    devector<int> v = {1, 2, 3};
    v.pop_back();
    
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v.back(), 2);
}

TEST_F(DevectorTest, PopFrontNonEmpty) {
    devector<int> v = {1, 2, 3};
    v.pop_front();
    
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 2);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v.front(), 2);
}

TEST_F(DevectorTest, EmplaceBack) {
    devector<std::pair<int, int>> v;
    v.emplace_back(1, 2);
    v.emplace_back(3, 4);
    
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0].first, 1);
    EXPECT_EQ(v[0].second, 2);
    EXPECT_EQ(v[1].first, 3);
    EXPECT_EQ(v[1].second, 4);
}

TEST_F(DevectorTest, EmplaceFront) {
    devector<std::pair<int, int>> v;
    v.emplace_front(1, 2);
    v.emplace_front(3, 4);
    
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0].first, 3);
    EXPECT_EQ(v[0].second, 4);
    EXPECT_EQ(v[1].first, 1);
    EXPECT_EQ(v[1].second, 2);
}

// Тесты для геттеров
TEST_F(DevectorTest, DataEmpty) {
    devector<int> v;
    EXPECT_EQ(v.data(), nullptr);
}

TEST_F(DevectorTest, DataNonEmpty) {
    devector<int> v = {1, 2, 3};
    EXPECT_NE(v.data(), nullptr);
    EXPECT_EQ(*v.data(), 1);
}

TEST_F(DevectorTest, FrontEmpty) {
    devector<int> v;
    // Поведение front() на пустом контейнере не определено, но хотя бы не должно падать
    EXPECT_NO_THROW(v.front());
}

TEST_F(DevectorTest, BackEmpty) {
    devector<int> v;
    // Поведение back() на пустом контейнере не определено, но хотя бы не должно падать
    EXPECT_NO_THROW(v.back());
}

TEST_F(DevectorTest, AtValidIndex) {
    devector<int> v = {1, 2, 3};
    EXPECT_EQ(v.at(0), 1);
    EXPECT_EQ(v.at(1), 2);
    EXPECT_EQ(v.at(2), 3);
}

TEST_F(DevectorTest, AtInvalidIndex) {
    devector<int> v = {1, 2, 3};
    EXPECT_THROW(v.at(3), std::out_of_range);
    EXPECT_THROW(v.at(10), std::out_of_range);
}

TEST_F(DevectorTest, AtEmpty) {
    devector<int> v;
    EXPECT_THROW(v.at(0), std::out_of_range);
}

TEST_F(DevectorTest, ConstAtValidIndex) {
    const devector<int> v = {1, 2, 3};
    EXPECT_EQ(v.at(0), 1);
    EXPECT_EQ(v.at(1), 2);
    EXPECT_EQ(v.at(2), 3);
}

TEST_F(DevectorTest, ConstAtInvalidIndex) {
    const devector<int> v = {1, 2, 3};
    EXPECT_THROW(v.at(3), std::out_of_range);
}

// Тесты для итераторов в особых случаях
TEST_F(DevectorTest, IteratorsAfterReallocation) {
    devector<int> v;
    v.push_back(1);
    v.push_back(2);
    
    auto old_begin = v.begin();
    auto old_end = v.end();
    
    // Принудительно вызываем реаллокацию
    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }
    
    // Итераторы должны стать невалидными после реаллокации,
    // но мы можем проверить, что новые итераторы работают
    EXPECT_EQ(*v.begin(), 1);
    EXPECT_EQ(*(v.end() - 1), 99);
}

TEST_F(DevectorTest, ReverseIteratorsWithFrontCapacity) {
    devector<int> v(3, 0, reserve_only_tag_t{}); // front_capacity=3
    for(int i = 1; i < 4; ++i){
        v.push_back(i);
    }
    
    int expected = 3;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
}

TEST_F(DevectorTest, IteratorsEmptyContainer) {
    devector<int> v;
    EXPECT_EQ(v.begin(), v.end());
    EXPECT_EQ(v.cbegin(), v.cend());
    EXPECT_EQ(v.rbegin(), v.rend());
    EXPECT_EQ(v.crbegin(), v.crend());
}

// Тесты для edge cases
TEST_F(DevectorTest, PushBackToCapacity) {
    devector<int> v(0, 3, reserve_only_tag_t{}); // back_capacity=3
    v.push_back(1);
    v.push_back(2);
    v.push_back(3); // Заполняем до конца
    
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.back_free_capacity(), 0);
    
    // Следующий push должен вызвать реаллокацию
    v.push_back(4);
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v.back(), 4);
}

TEST_F(DevectorTest, PushFrontToCapacity) {
    devector<int> v(3, 0, reserve_only_tag_t{}); // front_capacity=3
    v.push_front(1);
    v.push_front(2);
    v.push_front(3); // Заполняем до конца
    
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.front_free_capacity(), 0);
    
    // Следующий push должен вызвать реаллокацию
    v.push_front(4);
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v.front(), 4);
}

TEST_F(DevectorTest, MaxSizeReserve) {
    devector<int> v;
    EXPECT_THROW(v.reserve(v.max_size() + 1), std::length_error);
    EXPECT_THROW(v.reserve_front(v.max_size() + 1), std::length_error);
    EXPECT_THROW(v.reserve_back(v.max_size() + 1), std::length_error);
}

// Тест на тривиальные типы с reserve
TEST_F(DevectorTest, ReserveWithTrivialType) {
    devector<int> v(2, 2, reserve_only_tag_t{});
    for(int i = 1; i < 3; ++i){
        v.push_back(i);
    }
    
    // Не должно падать с тривиальными типами
    EXPECT_NO_THROW(v.reserve_front(5));
    EXPECT_NO_THROW(v.reserve_back(5));
}

// Тест на нетривиальные типы с reserve
TEST_F(DevectorTest, ReserveWithNonTrivialType) {
    devector<std::string> v(2, 2, reserve_only_tag_t{});
    v.push_back("a");
    v.push_back("b");
    
    // Не должно падать с нетривиальными типами
    EXPECT_NO_THROW(v.reserve_front(5));
    EXPECT_NO_THROW(v.reserve_back(5));
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], "a");
    EXPECT_EQ(v[1], "b");
}