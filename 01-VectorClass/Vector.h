#pragma once

#include <array>
#include <iostream>

namespace Pratik
{

#define DIM2 2
#define DIM3 3

#define x 0
#define y 1
#define z 2

    template <typename coordinate_type, size_t dimensions = DIM3>
    class Vector
    {
        std::array<coordinate_type, dimensions> coords = {};
        bool isNormalized = false;

        template <typename coordinate_type, size_t dimensions>
        friend float dotProduct(const Vector<coordinate_type, dimensions> &v1, const Vector<coordinate_type, dimensions> &v2);

    public:
        Vector() {}

        Vector(std::array<coordinate_type, dimensions> _coords) : coords(_coords) {}

        Vector(coordinate_type _x, coordinate_type _y, coordinate_type _z) : coords(_x, _y, _z) {}

        Vector(coordinate_type _x, coordinate_type _y) : coords(_x, _y) {}

        // Equal to
        bool operator==(const Vector<coordinate_type, dimensions> &) const;

        // Not equal to
        bool operator!=(const Vector<coordinate_type, dimensions> &) const;

        // Greater than
        bool operator>(const Vector<coordinate_type, dimensions> &) const;

        // Less than
        bool operator<(const Vector<coordinate_type, dimensions> &) const;

        //
        Vector<coordinate_type, dimensions> operator*(coordinate_type value);

        // Substract
        Vector<coordinate_type, dimensions> operator-(const Vector<coordinate_type, dimensions> &) const;

        // Additions
        Vector<coordinate_type, dimensions> operator+(const Vector<coordinate_type, dimensions> &) const;

        // Dot Product
        float dot(Vector<coordinate_type, dimensions> &v1, Vector<coordinate_type, dimensions> &v2);

        // Corss Product
        Vector<coordinate_type, dimensions> corss(const Vector<coordinate_type, dimensions> &v1);

        void assign(const unsigned int dim, coordinate_type value);

        // Magnitude of Vector
        float magnitude() const;

        // Normalize the Vector
        void normalize();

        // Return Coordinate Array
        std::array<coordinate_type, dimensions> data();
    };

};
