#ifndef LAYLAOBJECTPATTERNTOKEN_H
#define LAYLAOBJECTPATTERNTOKEN_H


#include "structs/Tbyte.h"
#include "structs/Tstring.h"

namespace Lonely {


class LaylaObjectPatternTypes {
public:
  const static int numObjectPatternTypes = 4;
  enum LaylaObjectPatternType {
    none = 0,
    delay,
    standardSpawn,
    expandedSpawn
  };
};

class LaylaObjectPatternToken {
public:
  LaylaObjectPatternToken();
  
  // number of extra bytes to add to size() to get actual number of bytes
  // read after a call to loadAndAllocateToken
  const static int loadOverhead = 1;
  
  static LaylaObjectPatternToken*
    loadAndAllocateToken(const Tbyte* src);
  
  virtual ~LaylaObjectPatternToken();
  
  virtual int writeToData(Tbyte* dst) const =0;
  
  virtual int size() const =0;
  
  virtual LaylaObjectPatternTypes::LaylaObjectPatternType type() const =0;
  
  virtual LaylaObjectPatternToken* cloneNew() const =0;
  
  virtual int save(Tstring& dst) const;
  
  virtual Tstring descriptionString() const =0;
protected:

private:
  // this is very lazy (avoiding dynamically allocated save buffers
  // so I don't have to go write smart pointers),
  // but should work given the limited scope of input
  const static int maxDataSize_ = 8;
};

class LaylaObjectDelayToken : public LaylaObjectPatternToken {
public:
  LaylaObjectDelayToken(Tbyte delayAmount__);
  
  LaylaObjectDelayToken(const Tbyte* src);
  
  virtual int writeToData(Tbyte* dst) const;
  
  virtual int size() const;
  
  virtual LaylaObjectPatternTypes::LaylaObjectPatternType type() const;
  
  Tbyte delayAmount() const;
  void setDelayAmount(Tbyte delayAmount__);
  
  virtual LaylaObjectPatternToken* cloneNew() const;
  
  virtual Tstring descriptionString() const;
protected:
  Tbyte delayAmount_;
};

class LaylaObjectStandardSpawnToken : public LaylaObjectPatternToken {
public:
  LaylaObjectStandardSpawnToken(Tbyte objectType__,
                                bool continueInterpreting__);
  
  LaylaObjectStandardSpawnToken(const Tbyte* src);
  
  virtual int writeToData(Tbyte* dst) const;
  
  virtual int size() const;
  
  virtual LaylaObjectPatternTypes::LaylaObjectPatternType type() const;
  
  Tbyte objectType() const;
  void setObjectType(Tbyte objectType__);
  
  bool continueInterpreting() const;
  void setContinueInterpreting(bool continueInterpreting__);
  
  virtual LaylaObjectPatternToken* cloneNew() const;
  
  virtual Tstring descriptionString() const;
protected:
  Tbyte objectType_;
  bool continueInterpreting_;
};

class LaylaObjectExpandedSpawnToken : public LaylaObjectStandardSpawnToken {
public:
  LaylaObjectExpandedSpawnToken(Tbyte objectType__,
                                bool continueInterpreting__,
                                Tbyte spawnCount__,
                                bool positionFlag__,
                                Tbyte yNybble__);
  
  LaylaObjectExpandedSpawnToken(const Tbyte* src);
  
  virtual int writeToData(Tbyte* dst) const;
  
  virtual int size() const;
  
  virtual LaylaObjectPatternTypes::LaylaObjectPatternType type() const;
  
  Tbyte spawnCount() const;
  void setSpawnCount(Tbyte spawnCount__);
  
  bool positionFlag() const;
  void setPositionFlag(bool positionFlag__);
  
  Tbyte yNybble() const;
  void setYNybble(Tbyte yNybble__);
  
  virtual LaylaObjectPatternToken* cloneNew() const;
  
  int realSpawnCount() const;
  
  int realY() const;
  
  virtual Tstring descriptionString() const;
protected:
  Tbyte spawnCount_;
  bool positionFlag_;
  Tbyte yNybble_;
};


};


#endif
