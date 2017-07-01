#pragma once

#include <string>
#include <utility>
#include <vector>

class BlockBody;
class Expression;
class FuncType;
enum DataType : int;

class ArgInfo {
public:
  ArgInfo(BlockBody *owner, int idx, const char *name, FuncType *type);
  virtual ~ArgInfo();

  BlockBody *Owner();
  int Idx() const;
  std::string &Name();
  FuncType *Type();

  void SetName(const char *name);

private:
  BlockBody *owner;
  int idx;
  std::string name;
  FuncType *type;
};

class BlockBody {
public:
BlockBody(const char *name, DataType retType,
	  const std::vector<std::pair<const char *, std::vector<DataType>>> &info,
	  Expression *expr);
  virtual ~BlockBody();

  std::string &Name();
  DataType RetType() const;
  std::vector<ArgInfo *> &ArgInfos();
  int ArgNum() const;
  Expression *Expr();

  void SetName(const char *name);
  void Set(const std::vector<std::pair<const char *, std::vector<DataType>>> &infos);

  //private:
  std::string name;
  DataType retType;
  std::vector<ArgInfo *> argInfos;
  Expression *expr;
};
