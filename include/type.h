#pragma once

#include <vector>

enum DataType : int {
  VOID,
  INT,
  STRING,
  IFTYPE,
  DUMMY
};

class FuncType {
public:
  FuncType(DataType retType);
  virtual ~FuncType();

  DataType RetType() const;
  std::vector<DataType> &ArgTypes();

  DataType retType;
  std::vector<DataType> argTypes;
};
