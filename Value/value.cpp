#pragma once

#include "value.h"

// 模板构造函数实现
Value::Value(std::vector<double>& input_vector, size_t fraction_bits) {

    if (fraction_bits < 0) {
        throw std::invalid_argument("Fraction bits must be non-negative for non-integer inputs.");
    }

    this->fraction_bits = fraction_bits;

    for (double& value : input_vector) {
        int64_t fixed_point_value = static_cast<int64_t>(std::round(static_cast<double>(value) * std::pow(2, fraction_bits)));
        fixed_point_vector.push_back(fixed_point_value);
    }
};

Value::Value(std::vector<int64_t>&& input_vector) {
    fixed_point_vector=std::move(input_vector);
    fraction_bits=0;
    shareType=0;
};

Value::Value() {
    fraction_bits = 0; 
    fixed_point_vector.clear();
};

std::vector<int64_t>& Value::get_fixed_point_vector() {
    return fixed_point_vector;
}

const std::vector<int64_t>& Value::get_const_vector() const {
    return fixed_point_vector;
}

uint32_t Value::size() const {
    return fixed_point_vector.size();
}

void Value::set_share_type(bool sp){
    shareType=sp;
}

bool Value::get_share_type() const {
    return shareType;
}

int64_t& Value::operator[](size_t index){
    if (index >= fixed_point_vector.size()) {
        throw std::out_of_range("Index out of range.");
    }
    return fixed_point_vector[index];
}

int64_t Value::operator[](size_t index) const{
    if (index >= fixed_point_vector.size()) {
        throw std::out_of_range("Index out of range.");
    }
    return fixed_point_vector[index];
}

double Value::get_value(size_t index) const {
    if (index >= fixed_point_vector.size()) {
        throw std::out_of_range("Index out of range.");
    }

    if (fraction_bits == 0) {
        return static_cast<int64_t>(fixed_point_vector[index]);
    }

    return static_cast<double>(fixed_point_vector[index]) / std::pow(2, fraction_bits);
}


size_t Value::get_fraction_bits() const {
    return fraction_bits;
}

void Value::set_fraction_bits(size_t fp) {
    fraction_bits=fp;
}