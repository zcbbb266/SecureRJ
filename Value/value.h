#pragma once

#include <vector>
#include <stdexcept>
#include <cmath>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

class Value {
public:

    Value(); 
    
    Value(std::vector<double>& input_vector, size_t fraction_bits);


    Value(std::vector<int64_t>&& input_vector);


    Value(Value&& other) noexcept
    : fixed_point_vector(std::move(other.fixed_point_vector)), 
        fraction_bits(other.fraction_bits),shareType(other.shareType){}

    Value(const Value& other) noexcept
        : fixed_point_vector(std::move(other.fixed_point_vector)), 
          fraction_bits(other.fraction_bits),shareType(other.shareType){}


    Value& operator=(Value&& other) noexcept {
        if (this != &other) {
            fixed_point_vector = std::move(other.fixed_point_vector); 
            fraction_bits = other.fraction_bits; 
            shareType=other.shareType;
        }
        return *this;
    }

    Value& operator=(std::vector<int64_t>&& other) noexcept {
        fixed_point_vector = std::move(other); 
        return *this;
    }

    Value& operator=(const Value& other) noexcept {
        if (this != &other) {
            fixed_point_vector = std::move(other.fixed_point_vector); 
            fraction_bits = other.fraction_bits; 
            shareType=other.shareType;
        }
        return *this;
    }



    std::vector<int64_t>& get_fixed_point_vector();
    const std::vector<int64_t>& get_const_vector() const;
    
    double get_value(size_t index) const;

    size_t get_fraction_bits() const;

    void set_fraction_bits(size_t fp);

    void set_share_type(bool sp);

    bool get_share_type() const;

    uint32_t size() const;

    int64_t& operator[](size_t index);
    int64_t operator[](size_t index) const;

private:
    std::vector<int64_t> fixed_point_vector; 
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

