#pragma once

#include <vector>
#include <stdexcept>
#include <cmath>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

class Value128 {
public:

    Value128(); 
    
    Value128(std::vector<double>& input_vector, size_t fraction_bits);


    Value128(std::vector<__int128>&& input_vector);


    Value128(Value128&& other) noexcept
    : fixed_point_vector(std::move(other.fixed_point_vector)), 
        fraction_bits(other.fraction_bits),shareType(other.shareType){}

    Value128(const Value128& other) noexcept
        : fixed_point_vector(std::move(other.fixed_point_vector)), 
          fraction_bits(other.fraction_bits),shareType(other.shareType){}


    Value128& operator=(Value128&& other) noexcept {
        if (this != &other) {
            fixed_point_vector = std::move(other.fixed_point_vector); 
            fraction_bits = other.fraction_bits; 
            shareType=other.shareType;
        }
        return *this;
    }

        Value128& operator=(const Value128& other) noexcept {
        if (this != &other) {
            fixed_point_vector = std::move(other.fixed_point_vector); 
            fraction_bits = other.fraction_bits; 
            shareType=other.shareType;
        }
        return *this;
    }



    const std::vector<__int128>& get_fixed_point_vector() const;
    
    double get_Value(size_t index) const;

    size_t get_fraction_bits() const;

    void set_fraction_bits(size_t fp);

    void set_share_type(bool sp);

    bool get_share_type() const;

    uint32_t size() const;

    __int128& operator[](size_t index);
    __int128 operator[](size_t index) const;

private:
    std::vector<__int128> fixed_point_vector; 
    size_t fraction_bits;            
    bool shareType;
    friend class boost::serialization::access;
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(fixed_point_vector);
        ar & BOOST_SERIALIZATION_NVP(fraction_bits);
        ar & BOOST_SERIALIZATION_NVP(shareType);
    }          
};

