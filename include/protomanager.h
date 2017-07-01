#pragma once

#include <list>

class BlockBody;
class Proto;

class ProtoManager {
public:
  ProtoManager(int x, int y, int yy);
  virtual ~ProtoManager();

  std::list<Proto *> Protos();

  void CreateSTD();
  Proto *NewProto(BlockBody *body);
  void AddProto(Proto *proto);
  void UpdateProtos();

private:
  std::list<Proto *> protos;

  int x;
  int y;
  int yy;
};
