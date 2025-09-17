#include <gtest/gtest.h>
#include <Matrix/Matrix.h>
#include <Polyline/Polyline.h>

// Вспомогательная функция для доступа к элементам матрицы
template<typename T, size_t R, size_t C>
T& matrix_at(Matrix<T, R, C>& mat, size_t i, size_t j) {
    return mat[i, j];
}

template<typename T, size_t R, size_t C>
const T& matrix_at(const Matrix<T, R, C>& mat, size_t i, size_t j) {
    return mat[i, j];
}

// Тесты для класса Matrix
TEST(MatrixTest, DefaultConstructor) {
    Matrix<int, 2, 3> mat;
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            EXPECT_EQ(matrix_at(mat, i, j), 0);
        }
    }
}

TEST(MatrixTest, ValueConstructor) {
    Matrix<int, 2, 2> mat(5);
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            EXPECT_EQ(matrix_at(mat, i, j), 5);
        }
    }
}

TEST(MatrixTest, InitializerListConstructor) {
    Matrix<int, 2, 2> mat = {1, 2, 3, 4};
    EXPECT_EQ(matrix_at(mat, 0, 0), 1);
    EXPECT_EQ(matrix_at(mat, 0, 1), 2);
    EXPECT_EQ(matrix_at(mat, 1, 0), 3);
    EXPECT_EQ(matrix_at(mat, 1, 1), 4);
}

TEST(MatrixTest, CopyConstructor) {
    Matrix<int, 2, 2> mat1 = {1, 2, 3, 4};
    Matrix<int, 2, 2> mat2 = mat1;
    
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            EXPECT_EQ(matrix_at(mat1, i, j), matrix_at(mat2, i, j));
        }
    }
}

TEST(MatrixTest, AssignmentOperator) {
    Matrix<int, 2, 2> mat1 = {1, 2, 3, 4};
    Matrix<int, 2, 2> mat2;
    mat2 = mat1;
    
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            EXPECT_EQ(matrix_at(mat1, i, j), matrix_at(mat2, i, j));
        }
    }
}

TEST(MatrixTest, Addition) {
    Matrix<int, 2, 2> mat1 = {1, 2, 3, 4};
    Matrix<int, 2, 2> mat2 = {5, 6, 7, 8};
    Matrix<int, 2, 2> result = mat1 + mat2;
    
    EXPECT_EQ(matrix_at(result, 0, 0), 6);
    EXPECT_EQ(matrix_at(result, 0, 1), 8);
    EXPECT_EQ(matrix_at(result, 1, 0), 10);
    EXPECT_EQ(matrix_at(result, 1, 1), 12);
}

TEST(MatrixTest, Subtraction) {
    Matrix<int, 2, 2> mat1 = {5, 6, 7, 8};
    Matrix<int, 2, 2> mat2 = {1, 2, 3, 4};
    Matrix<int, 2, 2> result = mat1 - mat2;
    
    EXPECT_EQ(matrix_at(result, 0, 0), 4);
    EXPECT_EQ(matrix_at(result, 0, 1), 4);
    EXPECT_EQ(matrix_at(result, 1, 0), 4);
    EXPECT_EQ(matrix_at(result, 1, 1), 4);
}

TEST(MatrixTest, Multiplication) {
    Matrix<int, 2, 3> mat1 = {1, 2, 3, 4, 5, 6};
    Matrix<int, 3, 2> mat2 = {7, 8, 9, 10, 11, 12};
    auto result = mat1 * mat2;
    
    EXPECT_EQ(matrix_at(result, 0, 0), 58);
    EXPECT_EQ(matrix_at(result, 0, 1), 64);
    EXPECT_EQ(matrix_at(result, 1, 0), 139);
    EXPECT_EQ(matrix_at(result, 1, 1), 154);
}

TEST(MatrixTest, Transpose) {
    Matrix<int, 2, 3> mat = {1, 2, 3, 4, 5, 6};
    auto transposed = mat.transposed();
    
    EXPECT_EQ(matrix_at(transposed, 0, 0), 1);
    EXPECT_EQ(matrix_at(transposed, 0, 1), 4);
    EXPECT_EQ(matrix_at(transposed, 1, 0), 2);
    EXPECT_EQ(matrix_at(transposed, 1, 1), 5);
    EXPECT_EQ(matrix_at(transposed, 2, 0), 3);
    EXPECT_EQ(matrix_at(transposed, 2, 1), 6);
}

TEST(MatrixTest, Iterators) {
    Matrix<int, 2, 2> mat = {1, 2, 3, 4};
    
    // Forward iteration
    int sum = 0;
    for (auto it = mat.begin(); it != mat.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 10);
    
    // Range-based for loop
    sum = 0;
    for (int val : mat) {
        sum += val;
    }
    EXPECT_EQ(sum, 10);
}

// Тесты для класса Point
TEST(PointTest, DistanceCalculation) {
    Point<int> p1 = {0, 0, 0, 'A'};
    Point<int> p2 = {3, 4, 0, 'B'};
    
    EXPECT_DOUBLE_EQ(p1.distance(p2), 5.0);
}

// Тесты для класса Polyline
TEST(PolylineTest, DefaultConstructor) {
    Polyline<int> poly;
    EXPECT_EQ(poly.points_count(), 0);
}

TEST(PolylineTest, AddPoint) {
    Polyline<int> poly;
    poly.add_point(1, 2, 3, 'A');
    
    EXPECT_EQ(poly.points_count(), 1);
    EXPECT_EQ(poly[0].x, 1);
    EXPECT_EQ(poly[0].y, 2);
    EXPECT_EQ(poly[0].z, 3);
    EXPECT_EQ(poly[0].name_, 'A');
}

TEST(PolylineTest, CopyConstructor) {
    Polyline<int> poly1;
    poly1.add_point(1, 2, 3, 'A');
    poly1.add_point(4, 5, 6, 'B');
    
    Polyline<int> poly2 = poly1;
    
    EXPECT_EQ(poly2.points_count(), 2);
    EXPECT_EQ(poly2[0].x, 1);
    EXPECT_EQ(poly2[1].x, 4);
}

TEST(PolylineTest, AssignmentOperator) {
    Polyline<int> poly1;
    poly1.add_point(1, 2, 3, 'A');
    
    Polyline<int> poly2;
    poly2 = poly1;
    
    EXPECT_EQ(poly2.points_count(), 1);
    EXPECT_EQ(poly2[0].x, 1);
}

TEST(PolylineTest, LengthCalculation) {
    Polyline<int> poly;
    poly.add_point(0, 0, 0, 'A');
    poly.add_point(3, 0, 0, 'B');
    poly.add_point(3, 4, 0, 'C');
    
    // 0->3 = 3, 3->4 = 4, total = 7
    EXPECT_DOUBLE_EQ(poly.length(), 7.0);
}

TEST(PolylineTest, ShiftOperation) {
    Polyline<int> poly;
    poly.add_point(1, 2, 3, 'A');
    poly.add_point(4, 5, 6, 'B');
    
    poly.shift(1, 1, 1);
    
    EXPECT_EQ(poly[0].x, 2);
    EXPECT_EQ(poly[0].y, 3);
    EXPECT_EQ(poly[0].z, 4);
    EXPECT_EQ(poly[1].x, 5);
    EXPECT_EQ(poly[1].y, 6);
    EXPECT_EQ(poly[1].z, 7);
}

TEST(PolylineTest, FindAndRemoveDistant) {
    Polyline<int> poly;
    poly.add_point(0, 0, 0, 'A');
    poly.add_point(1, 0, 0, 'B');
    poly.add_point(2, 0, 0, 'C');
    poly.add_point(3, 0, 0, 'D');
    
    size_t distant_index = poly.find_distant();
    // Средняя точка должна быть самой удаленной
    EXPECT_TRUE(distant_index == 1 || distant_index == 2);
    
    size_t original_size = poly.points_count();
    poly.remove_distant();
    EXPECT_EQ(poly.points_count(), original_size - 1);
}

TEST(PolylineTest, AddPolyline) {
    Polyline<int> poly1;
    poly1.add_point(1, 2, 3, 'A');
    
    Polyline<int> poly2;
    poly2.add_point(4, 5, 6, 'B');
    
    poly1.add_polyline(poly2);
    
    EXPECT_EQ(poly1.points_count(), 2);
    EXPECT_EQ(poly1[0].x, 1);
    EXPECT_EQ(poly1[1].x, 4);
}

TEST(PolylineTest, Iterators) {
    Polyline<int> poly;
    poly.add_point(1, 2, 3, 'A');
    poly.add_point(4, 5, 6, 'B');
    
    // Forward iteration
    int sum_x = 0;
    for (auto it = poly.begin(); it != poly.end(); ++it) {
        sum_x += it->x;
    }
    EXPECT_EQ(sum_x, 5);
    
    // Range-based for loop
    sum_x = 0;
    for (const auto& point : poly) {
        sum_x += point.x;
    }
    EXPECT_EQ(sum_x, 5);
}

// Тесты для преобразований между Point и Matrix
TEST(PointMatrixConversionTest, Conversion) {
    Point<int> point = {1, 2, 3, 'A'};
    Matrix<int, 1, 3> matrix = get_matrix_from_point(point);
    
    EXPECT_EQ(matrix_at(matrix, 0, 0), 1);
    EXPECT_EQ(matrix_at(matrix, 0, 1), 2);
    EXPECT_EQ(matrix_at(matrix, 0, 2), 3);
    
    Point<int> converted_point = get_point_from_matrix(matrix, 'B');
    EXPECT_EQ(converted_point.x, 1);
    EXPECT_EQ(converted_point.y, 2);
    EXPECT_EQ(converted_point.z, 3);
    EXPECT_EQ(converted_point.name_, 'B');
}

TEST(MatrixTest, SizeGetters) {
    Matrix<int, 3, 4> mat;
    EXPECT_EQ(mat.get_col_size(), 3);
    EXPECT_EQ(mat.get_row_size(), 4);
    EXPECT_EQ(mat.get_size(), 12);
}

TEST(PolylineTest, EmptyPolylineLength) {
    Polyline<int> empty_poly;
    EXPECT_DOUBLE_EQ(empty_poly.length(), 0.0);
}

TEST(PolylineTest, SinglePointLength) {
    Polyline<int> poly;
    poly.add_point(1, 2, 3, 'A');
    EXPECT_DOUBLE_EQ(poly.length(), 0.0);
}