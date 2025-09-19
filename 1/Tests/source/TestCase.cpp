#include <gtest/gtest.h>
#include <Matrix/Matrix.h>
#include <Polyline/Polyline.h>
#include <vector>
#include <array>
#include <numeric>

using namespace PolylineNameSpace;

// ==================== Helper Functions ====================

template <typename MatrixType>
auto& matrix_at(MatrixType& mat, size_t i, size_t j) {
    return mat[i, j];
}

template <typename MatrixType>
auto matrix_at(const MatrixType& mat, size_t i, size_t j) {
    return mat[i, j];
}

// ==================== Matrix Tests ====================

TEST(MatrixTest, DefaultConstructor) {
    Matrix<int, 2, 3> mat;
    EXPECT_EQ(mat.get_col_size(), 2);
    EXPECT_EQ(mat.get_row_size(), 3);
    EXPECT_EQ(mat.get_size(), 6);
    
    // Check all elements are default initialized (0 for int)
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            EXPECT_EQ(matrix_at(mat, i, j), 0);
        }
    }
}

TEST(MatrixTest, ValueConstructor) {
    Matrix<double, 2, 2> mat(5.5);
    EXPECT_DOUBLE_EQ(matrix_at(mat, 0, 0), 5.5);
    EXPECT_DOUBLE_EQ(matrix_at(mat, 0, 1), 5.5);
    EXPECT_DOUBLE_EQ(matrix_at(mat, 1, 0), 5.5);
    EXPECT_DOUBLE_EQ(matrix_at(mat, 1, 1), 5.5);
}

TEST(MatrixTest, IteratorConstructor) {
    std::vector<int> data = {1, 2, 3, 4, 5, 6};
    Matrix<int, 2, 3> mat(data.begin(), data.end());
    
    EXPECT_EQ(matrix_at(mat, 0, 0), 1);
    EXPECT_EQ(matrix_at(mat, 0, 1), 2);
    EXPECT_EQ(matrix_at(mat, 0, 2), 3);
    EXPECT_EQ(matrix_at(mat, 1, 0), 4);
    EXPECT_EQ(matrix_at(mat, 1, 1), 5);
    EXPECT_EQ(matrix_at(mat, 1, 2), 6);
}

TEST(MatrixTest, InitializerListConstructor) {
    Matrix<int, 2, 3> mat = {1, 2, 3, 4, 5, 6};
    
    EXPECT_EQ(matrix_at(mat, 0, 0), 1);
    EXPECT_EQ(matrix_at(mat, 0, 1), 2);
    EXPECT_EQ(matrix_at(mat, 0, 2), 3);
    EXPECT_EQ(matrix_at(mat, 1, 0), 4);
    EXPECT_EQ(matrix_at(mat, 1, 1), 5);
    EXPECT_EQ(matrix_at(mat, 1, 2), 6);
}

TEST(MatrixTest, CopyConstructor) {
    Matrix<int, 2, 2> original = {1, 2, 3, 4};
    Matrix<int, 2, 2> copy(original);
    
    EXPECT_EQ(matrix_at(copy, 0, 0), 1);
    EXPECT_EQ(matrix_at(copy, 0, 1), 2);
    EXPECT_EQ(matrix_at(copy, 1, 0), 3);
    EXPECT_EQ(matrix_at(copy, 1, 1), 4);
}

TEST(MatrixTest, AssignmentOperator) {
    Matrix<int, 2, 2> mat1 = {1, 2, 3, 4};
    Matrix<int, 2, 2> mat2;
    mat2 = mat1;
    
    EXPECT_EQ(matrix_at(mat2, 0, 0), 1);
    EXPECT_EQ(matrix_at(mat2, 0, 1), 2);
    EXPECT_EQ(matrix_at(mat2, 1, 0), 3);
    EXPECT_EQ(matrix_at(mat2, 1, 1), 4);
}

TEST(MatrixTest, InitializerListAssignment) {
    Matrix<int, 2, 2> mat;
    mat = {5, 6, 7, 8};
    
    EXPECT_EQ(matrix_at(mat, 0, 0), 5);
    EXPECT_EQ(matrix_at(mat, 0, 1), 6);
    EXPECT_EQ(matrix_at(mat, 1, 0), 7);
    EXPECT_EQ(matrix_at(mat, 1, 1), 8);
}

TEST(MatrixTest, AdditionAssignment) {
    Matrix<int, 2, 2> mat1 = {1, 2, 3, 4};
    Matrix<int, 2, 2> mat2 = {5, 6, 7, 8};
    mat1 += mat2;
    
    EXPECT_EQ(matrix_at(mat1, 0, 0), 6);
    EXPECT_EQ(matrix_at(mat1, 0, 1), 8);
    EXPECT_EQ(matrix_at(mat1, 1, 0), 10);
    EXPECT_EQ(matrix_at(mat1, 1, 1), 12);
}

TEST(MatrixTest, SubtractionAssignment) {
    Matrix<int, 2, 2> mat1 = {10, 20, 30, 40};
    Matrix<int, 2, 2> mat2 = {1, 2, 3, 4};
    mat1 -= mat2;
    
    EXPECT_EQ(matrix_at(mat1, 0, 0), 9);
    EXPECT_EQ(matrix_at(mat1, 0, 1), 18);
    EXPECT_EQ(matrix_at(mat1, 1, 0), 27);
    EXPECT_EQ(matrix_at(mat1, 1, 1), 36);
}

TEST(MatrixTest, AdditionOperator) {
    Matrix<int, 2, 2> mat1 = {1, 2, 3, 4};
    Matrix<int, 2, 2> mat2 = {5, 6, 7, 8};
    auto result = mat1 + mat2;
    
    EXPECT_EQ(matrix_at(result, 0, 0), 6);
    EXPECT_EQ(matrix_at(result, 0, 1), 8);
    EXPECT_EQ(matrix_at(result, 1, 0), 10);
    EXPECT_EQ(matrix_at(result, 1, 1), 12);
}

TEST(MatrixTest, SubtractionOperator) {
    Matrix<int, 2, 2> mat1 = {10, 20, 30, 40};
    Matrix<int, 2, 2> mat2 = {1, 2, 3, 4};
    auto result = mat1 - mat2;
    
    EXPECT_EQ(matrix_at(result, 0, 0), 9);
    EXPECT_EQ(matrix_at(result, 0, 1), 18);
    EXPECT_EQ(matrix_at(result, 1, 0), 27);
    EXPECT_EQ(matrix_at(result, 1, 1), 36);
}

TEST(MatrixTest, MultiplicationOperator) {
    Matrix<int, 2, 3> mat1 = {1, 2, 3, 4, 5, 6};
    Matrix<int, 3, 2> mat2 = {7, 8, 9, 10, 11, 12};
    auto result = mat1 * mat2;
    
    EXPECT_EQ(result.get_col_size(), 2);
    EXPECT_EQ(result.get_row_size(), 2);
    EXPECT_EQ(matrix_at(result, 0, 0), 58);   // 1*7 + 2*9 + 3*11
    EXPECT_EQ(matrix_at(result, 0, 1), 64);   // 1*8 + 2*10 + 3*12
    EXPECT_EQ(matrix_at(result, 1, 0), 139);  // 4*7 + 5*9 + 6*11
    EXPECT_EQ(matrix_at(result, 1, 1), 154);  // 4*8 + 5*10 + 6*12
}

TEST(MatrixTest, Fill) {
    Matrix<int, 2, 3> mat;
    mat.fill(42);
    
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            EXPECT_EQ(matrix_at(mat, i, j), 42);
        }
    }
}

TEST(MatrixTest, Transposed) {
    Matrix<int, 2, 3> original = {1, 2, 3, 4, 5, 6};
    auto transposed = original.transposed();
    
    EXPECT_EQ(transposed.get_col_size(), 3);
    EXPECT_EQ(transposed.get_row_size(), 2);
    EXPECT_EQ(matrix_at(transposed, 0, 0), 1);
    EXPECT_EQ(matrix_at(transposed, 0, 1), 4);
    EXPECT_EQ(matrix_at(transposed, 1, 0), 2);
    EXPECT_EQ(matrix_at(transposed, 1, 1), 5);
    EXPECT_EQ(matrix_at(transposed, 2, 0), 3);
    EXPECT_EQ(matrix_at(transposed, 2, 1), 6);
}

TEST(MatrixTest, Iterators) {
    Matrix<int, 2, 3> mat = {1, 2, 3, 4, 5, 6};
    
    // Forward iterators
    auto it = mat.begin();
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(*(it + 1), 2);
    EXPECT_EQ(*(it + 5), 6);
    
    // Const iterators
    const auto& const_mat = mat;
    auto cit = const_mat.begin();
    EXPECT_EQ(*cit, 1);
    
    // Reverse iterators
    auto rit = mat.rbegin();
    EXPECT_EQ(*rit, 6);
    EXPECT_EQ(*(rit + 1), 5);
}

TEST(MatrixTest, ColumnIterators) {
    Matrix<int, 2, 3> mat = {1, 2, 3, 4, 5, 6};
    
    // Test column iteration (should go through columns first)
    auto col_it = mat.col_begin();
    EXPECT_EQ(*col_it, 1);    // [0,0]
    ++col_it;
    EXPECT_EQ(*col_it, 4);    // [1,0]
    ++col_it;
    EXPECT_EQ(*col_it, 2);    // [0,1]
    ++col_it;
    EXPECT_EQ(*col_it, 5);    // [1,1]
    ++col_it;
    EXPECT_EQ(*col_it, 3);    // [0,2]
    ++col_it;
    EXPECT_EQ(*col_it, 6);    // [1,2]
}

TEST(MatrixTest, RowIters) {
    Matrix<int, 2, 3> mat = {1, 2, 3, 4, 5, 6};
    
    auto [row0_begin, row0_end] = mat.row_iters(0);
    EXPECT_EQ(*row0_begin, 1);
    EXPECT_EQ(*(row0_begin + 1), 2);
    EXPECT_EQ(*(row0_begin + 2), 3);
    EXPECT_EQ(row0_end - row0_begin, 3);
    
    auto [row1_begin, row1_end] = mat.row_iters(1);
    EXPECT_EQ(*row1_begin, 4);
    EXPECT_EQ(*(row1_begin + 1), 5);
    EXPECT_EQ(*(row1_begin + 2), 6);
}

TEST(MatrixTest, ColIters) {
    Matrix<int, 2, 3> mat = {1, 2, 3, 4, 5, 6};
    
    auto [col0_begin, col0_end] = mat.col_iters(0);
    EXPECT_EQ(*col0_begin, 1);
    EXPECT_EQ(*(col0_begin + 1), 4);
    EXPECT_EQ(col0_end - col0_begin, 2);
    
    auto [col1_begin, col1_end] = mat.col_iters(1);
    EXPECT_EQ(*col1_begin, 2);
    EXPECT_EQ(*(col1_begin + 1), 5);
}

// ==================== Point Tests ====================

TEST(PointTest, Distance) {
    Point<int> p1{0, 0, 0, 'A'};
    Point<int> p2{3, 4, 0, 'B'};
    
    EXPECT_DOUBLE_EQ(p1.distance(p2), 5.0);
    
    Point<double> p3{1.0, 1.0, 1.0, 'C'};
    Point<double> p4{2.0, 2.0, 2.0, 'D'};
    
    EXPECT_DOUBLE_EQ(p3.distance(p4), std::sqrt(3.0));
}

TEST(PointTest, MatrixConversion) {
    Point<int> original{1, 2, 3, 'A'};
    auto matrix = get_matrix_from_point(original);
    Point<int> converted = get_point_from_matrix(matrix, 'B');
    
    EXPECT_EQ(converted.x, 1);
    EXPECT_EQ(converted.y, 2);
    EXPECT_EQ(converted.z, 3);
    EXPECT_EQ(converted.name_, 'B');
}

// ==================== Polyline Tests ====================

TEST(PolylineTest, DefaultConstructor) {
    Polyline<int> polyline;
    EXPECT_EQ(polyline.points_count(), 0);
}

TEST(PolylineTest, CopyConstructor) {
    Polyline<int> original;
    original.add_point(1, 2, 3, 'A');
    original.add_point(4, 5, 6, 'B');
    
    Polyline<int> copy(original);
    EXPECT_EQ(copy.points_count(), 2);
    EXPECT_EQ(copy[0].x, 1);
    EXPECT_EQ(copy[1].name_, 'B');
}

TEST(PolylineTest, MoveConstructor) {
    Polyline<int> original;
    original.add_point(1, 2, 3, 'A');
    
    Polyline<int> moved(std::move(original));
    EXPECT_EQ(moved.points_count(), 1);
    EXPECT_EQ(original.points_count(), 0); // NOLINT(bugprone-use-after-move)
}

TEST(PolylineTest, AssignmentOperator) {
    Polyline<int> poly1;
    poly1.add_point(1, 2, 3, 'A');
    
    Polyline<int> poly2;
    poly2 = poly1;
    
    EXPECT_EQ(poly2.points_count(), 1);
    EXPECT_EQ(poly2[0].x, 1);
}

TEST(PolylineTest, MoveAssignmentOperator) {
    Polyline<int> poly1;
    poly1.add_point(1, 2, 3, 'A');
    
    Polyline<int> poly2;
    poly2 = std::move(poly1);
    
    EXPECT_EQ(poly2.points_count(), 1);
    EXPECT_EQ(poly1.points_count(), 0); // NOLINT(bugprone-use-after-move)
}

TEST(PolylineTest, AddPoint) {
    Polyline<int> polyline;
    polyline.add_point(1, 2, 3, 'A');
    polyline.add_point(4, 5, 6, 'B');
    
    EXPECT_EQ(polyline.points_count(), 2);
    EXPECT_EQ(polyline[0].x, 1);
    EXPECT_EQ(polyline[1].z, 6);
    EXPECT_EQ(polyline[0].name_, 'A');
}

TEST(PolylineTest, AddPointObject) {
    Polyline<int> polyline;
    Point<int> point{7, 8, 9, 'C'};
    polyline.add_point(point);
    
    EXPECT_EQ(polyline.points_count(), 1);
    EXPECT_EQ(polyline[0].y, 8);
}

TEST(PolylineTest, AddPolyline) {
    Polyline<int> poly1;
    poly1.add_point(1, 2, 3, 'A');
    
    Polyline<int> poly2;
    poly2.add_point(4, 5, 6, 'B');
    
    poly1.add_polyline(poly2);
    EXPECT_EQ(poly1.points_count(), 2);
    EXPECT_EQ(poly1[1].x, 4);
}

TEST(PolylineTest, AddMovePolyline) {
    Polyline<int> poly1;
    poly1.add_point(1, 2, 3, 'A');
    
    Polyline<int> poly2;
    poly2.add_point(4, 5, 6, 'B');
    
    poly1.add_polyline(std::move(poly2));
    EXPECT_EQ(poly1.points_count(), 2);
    EXPECT_EQ(poly2.points_count(), 0); // NOLINT(bugprone-use-after-move)
}

TEST(PolylineTest, AccessOperator) {
    Polyline<int> polyline;
    polyline.add_point(1, 2, 3, 'A');
    
    EXPECT_EQ(polyline[0].x, 1);
    polyline[0].x = 10;
    EXPECT_EQ(polyline[0].x, 10);
    
    const auto& const_polyline = polyline;
    EXPECT_EQ(const_polyline[0].y, 2);
}

TEST(PolylineTest, Length) {
    Polyline<int> polyline;
    polyline.add_point(0, 0, 0, 'A');
    polyline.add_point(3, 4, 0, 'B');
    polyline.add_point(3, 4, 5, 'C');
    
    EXPECT_DOUBLE_EQ(polyline.length(), 5.0 + 5.0); // 5 (A-B) + 5 (B-C)
}

TEST(PolylineTest, Shift) {
    Polyline<int> polyline;
    polyline.add_point(1, 2, 3, 'A');
    polyline.add_point(4, 5, 6, 'B');
    
    polyline.shift(10, 20, 30);
    
    EXPECT_EQ(polyline[0].x, 11);
    EXPECT_EQ(polyline[0].y, 22);
    EXPECT_EQ(polyline[0].z, 33);
    EXPECT_EQ(polyline[1].x, 14);
    EXPECT_EQ(polyline[1].z, 36);
}

TEST(PolylineTest, RotateFromOrigin) {
    Polyline<double> polyline;
    polyline.add_point(1.0, 0.0, 0.0, 'A');
    
    // Rotate 90 degrees around Z-axis
    polyline.rotate_from_origin(0, 0, 90);
    
    EXPECT_NEAR(polyline[0].x, 0.0, 1e-10);
    EXPECT_NEAR(polyline[0].y, 1.0, 1e-10);
    EXPECT_NEAR(polyline[0].z, 0.0, 1e-10);
}

TEST(PolylineTest, RotateByVector) {
    Polyline<double> polyline;
    polyline.add_point(1.0, 0.0, 0.0, 'A');
    
    Point<double> start{0.0, 0.0, 0.0, 'S'};
    Point<double> end{0.0, 0.0, 1.0, 'E'}; // Z-axis
    
    // Rotate 90 degrees around Z-axis
    polyline.rotate_by_vector(start, end, 90);
    
    EXPECT_NEAR(polyline[0].x, 0.0, 1e-10);
    EXPECT_NEAR(polyline[0].y, 1.0, 1e-10);
    EXPECT_NEAR(polyline[0].z, 0.0, 1e-10);
}

TEST(PolylineTest, FindAndRemoveDistant) {
    Polyline<int> polyline;
    // Create points where middle point is farthest from neighbors
    polyline.add_point(0, 0, 0, 'A');    // distance to B: 1
    polyline.add_point(1, 0, 0, 'B');    // distance to A:1 + to C:100 = 101
    polyline.add_point(101, 0, 0, 'C');  // distance to B:100
    
    size_t distant_index = polyline.find_distant();
    EXPECT_EQ(distant_index, 1); // Middle point should be the most distant
    
    polyline.remove_distant();
    EXPECT_EQ(polyline.points_count(), 2);
    EXPECT_EQ(polyline[0].name_, 'A');
    EXPECT_EQ(polyline[1].name_, 'C');
}

TEST(PolylineTest, Iterators) {
    Polyline<int> polyline;
    polyline.add_point(1, 2, 3, 'A');
    polyline.add_point(4, 5, 6, 'B');
    
    // Forward iteration
    int sum = 0;
    for (const auto& point : polyline) {
        sum += point.x;
    }
    EXPECT_EQ(sum, 5);
    
    // Reverse iteration
    sum = 0;
    for (auto it = polyline.rbegin(); it != polyline.rend(); ++it) {
        sum += it->x;
    }
    EXPECT_EQ(sum, 5);
    
    // Const iteration
    const auto& const_polyline = polyline;
    sum = 0;
    for (auto it = const_polyline.begin(); it != const_polyline.end(); ++it) {
        sum += it->x;
    }
    EXPECT_EQ(sum, 5);
}

TEST(PolylineTest, Resize) {
    Polyline<int> polyline;
    // Add more points than initial capacity to trigger resize
    for (int i = 0; i < 20; ++i) {
        polyline.add_point(i, i, i, 'A' + i);
    }
    
    EXPECT_EQ(polyline.points_count(), 20);
    for (int i = 0; i < 20; ++i) {
        EXPECT_EQ(polyline[i].x, i);
    }
}

TEST(PolylineTest, Swap) {
    Polyline<int> poly1;
    poly1.add_point(1, 2, 3, 'A');
    
    Polyline<int> poly2;
    poly2.add_point(4, 5, 6, 'B');
    
    poly1.swap(poly2);
    
    EXPECT_EQ(poly1.points_count(), 1);
    EXPECT_EQ(poly1[0].x, 4);
    EXPECT_EQ(poly2.points_count(), 1);
    EXPECT_EQ(poly2[0].x, 1);
}