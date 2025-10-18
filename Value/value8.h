#pragma once

#include <vector>
#include <stdexcept>
#include <cmath>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

class Value8 {
public:

    Value8(); 
    
    Value8(std::vector<double>& input_vector, size_t fraction_bits);


    Value8(std::vector<int8_t>&& input_vector);


    Value8(Value8&& other) noexcept
    : fixed_point_vector(std::move(other.fixed_point_vector)), 
        fraction_bits(other.fraction_bits),shareType(other.shareType){}

    Value8(const Value8& other) noexcept
        : fixed_point_vector(std::move(other.fixed_point_vector)), 
          fraction_bits(other.fraction_bits),shareType(other.shareType){}


    Value8& operator=(Value8&& other) noexcept {
        if (this != &other) {
            fixed_point_vector = std::move(other.fixed_point_vector); 
            fraction_bits = other.fraction_bits; 
            shareType=other.shareType;
        }
        return *this;
    }

        Value8& operator=(const Value8& other) noexcept {
        if (this != &other) {
            fixed_point_vector = std::move(other.fixed_point_vector); 
            fraction_bits = other.fraction_bits; 
            shareType=other.shareType;
        }
        return *this;
    }



    const std::vector<int8_t>& get_fixed_point_vector() const;
    
    double get_Value(size_t index) const;

    size_t get_fraction_bits() const;

    void set_fraction_bits(size_t fp);

    void set_share_type(bool sp);

    bool get_share_type() const;

    uint32_t size() const;

    int8_t& operator[](size_t index);
    int8_t operator[](size_t index) const;

private:
    std::vector<int8_t> fixed_point_vector; 
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

