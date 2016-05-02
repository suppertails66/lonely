#ifndef LAYLAPATTERNTOKEN_H
#define LAYLAPATTERNTOKEN_H


#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "structs/Tstring.h"

namespace Lonely {


/**
 * Container for enum of pattern types.
 * The layout of an area (asteroid) in Layla is an array of pattern data,
 * which is read sequentially as the player progresses through the level so
 * the game knows what metatiles to load next. Most values in the pattern
 * data are pattern numbers, but a few special IDs are "script commands" that
 * affect particular parameters when loaded.
 */
class LaylaPatternTypes {
public:
  const static int numLaylaPatternTypes = 9;
  enum LaylaPatternType {
    none = 0,
    spawnPoint,       // not a real pattern type
    standardPattern,  // 0x00-0xF9
    spawnBoss,        // 0xFA
    setScrolling,     // 0xFB
    setArea,          // 0xFC
    setMap,           // 0xFD
    setElevators,     // 0xFE
    jumpToPosition    // 0xFF
  };
};

/**
 * Abstract base for pattern tokens.
 */
class LaylaPatternToken {
public:
  LaylaPatternToken();
  
  virtual ~LaylaPatternToken();
  
  static LaylaPatternToken* loadAndAllocateToken(const Tbyte* src);
  
  virtual int readFromData(const Tbyte* src) =0;
  
  virtual int writeToData(Tbyte* dst) const =0;
  
  virtual int saveInternal(Tbyte* dst) const;
  
  virtual int loadInternal(const Tbyte* dst);
  
  virtual int serializedSize() const;
  
  virtual int dataSerializedSize() const;
  
  /**
   * Returns the type of the pattern represented by the token.
   */
  virtual LaylaPatternTypes::LaylaPatternType type() const =0;
  
  /**
   * Returns the raw size in bytes of the token's data.
   */
  virtual int size() const =0;
  
  virtual int save(Tstring& dst) const;
  
  virtual LaylaPatternToken* cloneNew() const =0;
  
  virtual Tstring descriptionString() const =0;
protected:
  
private:
  const static int maxDataSize_ = 8;
};

/**
 * Spawn point "pseudo-token".
 * Spawn points are not actually part of the pattern data, and are instead
 * realized using a jump table. However, it's more convenient for editing
 * purposes to put them in the pattern data stream.
 */
class SpawnPointToken : public LaylaPatternToken {
public:
  SpawnPointToken();
  SpawnPointToken(int spawnNum__);
  
  virtual int readFromData(const Tbyte* src);
  
  virtual int writeToData(Tbyte* dst) const;

  virtual LaylaPatternTypes::LaylaPatternType type() const;
  virtual int size() const;
  
  int spawnNum() const;
  void setSpawnNum(int spawnNum__);
  
  virtual int saveInternal(Tbyte* dst) const;
  
  virtual int loadInternal(const Tbyte* dst);
  
  virtual int serializedSize() const;
  
  virtual int dataSerializedSize() const;
  
  virtual LaylaPatternToken* cloneNew() const;
  
  virtual Tstring descriptionString() const;
protected:
  int spawnNum_;
};

/**
 * A standard pattern identifier.
 */
class StandardPatternToken : public LaylaPatternToken {
public:
  StandardPatternToken();
  StandardPatternToken(Tbyte patternNum__);
  
  virtual int readFromData(const Tbyte* src);
  
  virtual int writeToData(Tbyte* dst) const;

  virtual LaylaPatternTypes::LaylaPatternType type() const;
  virtual int size() const;
  
  Tbyte patternNum() const;
  void setPatternNum(Tbyte patternNum__);
  
  virtual LaylaPatternToken* cloneNew() const;
  
  virtual Tstring descriptionString() const;
protected:
  Tbyte patternNum_;
};

/**
 * Boss spawn point token.
 */
class SpawnBossToken : public LaylaPatternToken {
public:
  SpawnBossToken();
  SpawnBossToken(Tbyte bossNum__);
  
  virtual int readFromData(const Tbyte* src);
  
  virtual int writeToData(Tbyte* dst) const;

  virtual LaylaPatternTypes::LaylaPatternType type() const;
  virtual int size() const;
  
  Tbyte bossNum() const;
  void setBossNum(Tbyte bossNum__);
  
  virtual LaylaPatternToken* cloneNew() const;
  
  virtual Tstring descriptionString() const;
protected:
  Tbyte bossNum_;
};

/**
 * Scrolling change token.
 */
class SetScrollingToken : public LaylaPatternToken {
public:
  const static Tbyte scrollingEnabledValue = 0;
  const static Tbyte scrollingDisabledValue = 1;

  SetScrollingToken();
  SetScrollingToken(Tbyte killsToNextFood__,
                    Tbyte scrollingDisabled__);
  
  virtual int readFromData(const Tbyte* src);
  
  virtual int writeToData(Tbyte* dst) const;

  virtual LaylaPatternTypes::LaylaPatternType type() const;
  virtual int size() const;
  
  Tbyte killsToNextFood() const;
  void setKillsToNextFood(Tbyte killsToNextFood__);
  
  Tbyte scrollingDisabled() const;
  void setScrollingDisabled(Tbyte scrollingDisabled__);
  
  virtual LaylaPatternToken* cloneNew() const;
  
  virtual Tstring descriptionString() const;
  
  bool scrollingIsDisabled() const;
  
protected:
  
  Tbyte killsToNextFood_;
  Tbyte scrollingDisabled_;
};

/**
 * Area change token.
 */
class SetAreaToken : public LaylaPatternToken {
public:
  const static int caveID = 0x00;
  const static int baseFlag = 0x01;
  const static int bossFlag = 0x80;
  
  enum AreaType {
    cave,
    base,
    boss
  };

  SetAreaToken();
  SetAreaToken(Tbyte backgroundMetatile__,
               Tbyte areaType__);
  
  virtual int readFromData(const Tbyte* src);
  
  virtual int writeToData(Tbyte* dst) const;

  virtual LaylaPatternTypes::LaylaPatternType type() const;
  virtual int size() const;
  
  Tbyte backgroundMetatile() const;
  void setBackgroundMetatile(Tbyte backgroundMetatile__);
  
  Tbyte areaType() const;
  void setAreaType(Tbyte areaType__);
  
  virtual LaylaPatternToken* cloneNew() const;
  
  virtual Tstring descriptionString() const;
  
  AreaType realAreaType() const;
  void setRealAreaType(AreaType realAreaType__);
  
protected:
  
  Tbyte backgroundMetatile_;
  Tbyte areaType_;
};

/**
 * Map change token.
 */
class SetMapToken : public LaylaPatternToken {
public:
  SetMapToken();
  SetMapToken(Tbyte mapNum__,
              Tbyte param2__);
  
  virtual int readFromData(const Tbyte* src);
  
  virtual int writeToData(Tbyte* dst) const;

  virtual LaylaPatternTypes::LaylaPatternType type() const;
  virtual int size() const;
  
  Tbyte mapNum() const;
  void setMapNum(Tbyte mapNum__);
  
  Tbyte param2() const;
  void setParam2(int param2__);
  
  virtual LaylaPatternToken* cloneNew() const;
  
  virtual Tstring descriptionString() const;
  
protected:
  
  Tbyte mapNum_;
  Tbyte param2_;
};

/**
 * Elevator destination change token.
 */
class SetElevatorsToken : public LaylaPatternToken {
public:
  const static Tbyte noDestination = 0xFF;
  const static Tbyte levelExit = 0xFE;

  SetElevatorsToken();
  SetElevatorsToken(Tbyte upDestination__,
              Tbyte downDestination__);
  
  virtual int readFromData(const Tbyte* src);
  
  virtual int writeToData(Tbyte* dst) const;

  virtual LaylaPatternTypes::LaylaPatternType type() const;
  virtual int size() const;
  
  Tbyte upDestination() const;
  void setUpDestination(Tbyte upDestination__);
  
  Tbyte downDestination() const;
  void setDownDestination(Tbyte downDestination__);
  
  virtual LaylaPatternToken* cloneNew() const;
  
  virtual Tstring descriptionString() const;
  
protected:
  
  Tbyte upDestination_;
  Tbyte downDestination_;
};

/**
 * Pattern load position change token.
 */
class JumpToPositionToken : public LaylaPatternToken {
public:
  JumpToPositionToken();
  JumpToPositionToken(int offset__);
  
  virtual int readFromData(const Tbyte* src);
  
  virtual int saveInternal(Tbyte* dst) const;
  
  virtual int loadInternal(const Tbyte* dst);
  
  virtual int writeToData(Tbyte* dst) const;
  
  virtual int writeToData(Tbyte* dst,
                          Taddress address) const;
  
  virtual int serializedSize() const;

  virtual LaylaPatternTypes::LaylaPatternType type() const;
  virtual int size() const;
  
  int offset() const;
  void setOffset(int offset__);
  
  virtual LaylaPatternToken* cloneNew() const;
  
  virtual Tstring descriptionString() const;
protected:
  int offset_;
};


};


#endif
