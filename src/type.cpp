#include "type.h"

FuncType::FuncType(DataType retType) {
  this->retType = retType;
}

FuncType::~FuncType() {
}

DataType FuncType::RetType() const {
  return retType;
}

std::vector<DataType> &FuncType::ArgTypes() {
  return argTypes;
}
