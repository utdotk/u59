#include "protomanager.h"

#include "blockbody.h"
#include "ciproto.h"
#include "expr.h"
#include "proto.h"
#include "type.h"
#include "viproto.h"
#include "arproto.h"

#include <iostream>

ProtoManager::ProtoManager(int x, int y, int yy) {
  this->x = x;
  this->y = y;
  this->yy = yy;
}

ProtoManager::~ProtoManager() {
}

std::list<Proto *> ProtoManager::Protos() {
  return protos;
}

void ProtoManager::CreateSTD() {
  AddProto(new CIProto(0, 0, INT));
  AddProto(new CIProto(0, 0, STRING));

  
  AddProto(new VIProto(0, 0, INT));
  AddProto(new VIProto(0, 0, STRING));

  AddProto(new ArProto(0, 0, INT));
  AddProto(new ArProto(0, 0, STRING));

  std::vector<std::pair<const char *, std::vector<DataType>>> infos;
  infos.push_back({"val", {INT}});
  NewProto(new BlockBody("PrintInt", VOID, infos, new Print(INT)));

  infos.clear();
  infos.push_back({"val", {STRING}});
  NewProto(new BlockBody("PrintStr", VOID, infos, new Print(STRING)));

  infos.clear();
  infos.push_back({"proc1", {VOID}});
  infos.push_back({"proc2", {VOID}});
  NewProto(new BlockBody("Proc", VOID, infos, new Delay));
  
  infos.clear();
  infos.push_back({"condition", {INT}});
  infos.push_back({"then", {VOID}});
  infos.push_back({"else", {VOID}});
  NewProto(new BlockBody("If", VOID, infos, new If));

  infos.clear();
  infos.push_back({"condition", {INT}});
  infos.push_back({"proc", {VOID}});
  NewProto(new BlockBody("Loop", VOID, infos, new Loop));
  
  infos.clear();
  infos.push_back({"condition", {INT}});
  infos.push_back({"then", {INT}});
  infos.push_back({"else", {INT}});
  NewProto(new BlockBody("SelectInt", INT, infos, new IfVal(INT)));

  infos.clear();
  infos.push_back({"condition", {INT}});
  infos.push_back({"then", {STRING}});
  infos.push_back({"else", {STRING}});
  NewProto(new BlockBody("SelectStr", STRING, infos, new IfVal(STRING)));

  infos.clear();
  infos.push_back({"left", {INT}});
  infos.push_back({"right", {INT}});
  NewProto(new BlockBody("+", INT, infos, new Operation('+')));

  infos.clear();
  infos.push_back({"left", {INT}});
  infos.push_back({"right", {INT}});
  NewProto(new BlockBody("-", INT, infos, new Operation('-')));

  infos.clear();
  infos.push_back({"left", {INT}});
  infos.push_back({"right", {INT}});
  NewProto(new BlockBody("×", INT, infos, new Operation('*')));

  infos.clear();
  infos.push_back({"left", {INT}});
  infos.push_back({"right", {INT}});
  NewProto(new BlockBody("÷", INT, infos, new Operation('/')));

  infos.clear();
  infos.push_back({"left", {INT}});
  infos.push_back({"right", {INT}});
  NewProto(new BlockBody("mod", INT, infos, new Operation('%')));

  infos.clear();
  infos.push_back({"left", {INT}});
  infos.push_back({"right", {INT}});
  NewProto(new BlockBody("=", INT, infos, new Operation('=')));

  infos.clear();
  infos.push_back({"left", {INT}});
  infos.push_back({"right", {INT}});
  NewProto(new BlockBody("<", INT, infos, new Operation('<')));

  infos.clear();
  infos.push_back({"left", {INT}});
  infos.push_back({"right", {INT}});
  NewProto(new BlockBody("≦", INT, infos, new Operation('s')));
}

Proto *ProtoManager::NewProto(BlockBody *body) {
  auto *proto = new Proto(0, 0, body);
  protos.push_back(proto);
  return proto;
}

void ProtoManager::AddProto(Proto *proto) {
  protos.push_back(proto);
}

void ProtoManager::UpdateProtos() {
  int py = 0, c = 0;
  for (auto *proto : protos) {
    proto->UpdateSize();
    if (py + proto->H() > yy) {
      py = 0;
      c++;
    }
    proto->SetPos(x + 110 * c, y + py);
    py += proto->H() + 15;
  }
}
