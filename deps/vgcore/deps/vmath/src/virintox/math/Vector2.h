//
// Created by tv on 17.07.23.
//

#ifndef SPOONTOOL_VECTOR2_H
#define SPOONTOOL_VECTOR2_H

namespace Math {
    template <typename t>
    struct Vector2 {
        union{ t x,r,u; };
        union{ t y,g,v; };

        Vector2(t f1 = 0,t f2 = 0): x(f1),y(f2) {}

        Vector2<t> operator+(const Vector2<t>& vec) const{
            return Vector2(x+vec.x,y+vec.y);
        }

        Vector2<t> operator-(const Vector2<t>& vec) const{
            return Vector2(x-vec.x,y-vec.y);
        }

        bool operator==(const Vector2<t>& vec) const{
            return (x == vec.x) && (y == vec.y);
        }
        /*bool operator!=(const Vector2<t>& vec) const{
            return (x != vec.x) && (y != vec.y);
        }*/

        t length() const{
            return sqrtf( x*x + y*y );
        }

        t distance(const Vector2 &vec) const{
            return (*this - vec).length();
        }

        template <typename t2>
        operator Vector2<t2>(){
            return {(t2)x,(t2)y};
        }
    };


    using Vector2f = Vector2<float>;
    using Vector2i = Vector2<int>;
    using Vector2l = Vector2<long>;
    using Vector2u = Vector2<unsigned>;


} // Math

#endif //SPOONTOOL_VECTOR2_H
