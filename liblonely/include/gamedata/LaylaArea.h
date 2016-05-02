#ifndef LAYLAAREA_H
#define LAYLAAREA_H


#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "structs/Tarray.h"
#include "structs/TpointerVector.h"
#include "structs/Tstring.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "gamedata/LaylaPatternToken.h"
#include "gamedata/LaylaPatternDefinitionTable.h"
#include "nes/NesRom.h"
#include <vector>

namespace Lonely {


typedef TpointerVector<LaylaPatternToken> LaylaPatternTokenVector;
typedef std::vector<int> TokenIndexCollection;
typedef std::vector<Tbyte> PatternPreviewAreaTypeCollection;

class LaylaArea {
public:
  LaylaArea();
  
  LaylaArea(const Tbyte* src,
            int size,
            Taddress startAddress,
            const Tarray<Taddress>& spawnTable);
  
  LaylaArea(const LaylaArea& a);
  LaylaArea& operator=(const LaylaArea& a);
  
  ~LaylaArea();
  
  void readFromData(const Tbyte* src,
                    int size,
                    Taddress startAddress,
                    const Tarray<Taddress>& spawnTable);
  
  void writeToData(NesRom& dst,
                   Taddress mainBlockAddress,
                   int mainBlockLength) const;
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
                    
  int numPatternTokens() const;
  
  LaylaPatternToken& patternToken(int index);
  const LaylaPatternToken& patternToken(int index) const;
	
	int totalSize() const;
	
	int findRealXOfIndex(int index,
	                   LaylaPatternDefinitionTable& patterns);
	
	void insertStandardPatternToken(int position,
	                 LaylaPatternToken* token);
	
	void insertStandardPatternTokens(int position,
	                 LaylaPatternTokenVector tokens);
  
  int findPreviousStandardPatternIndexNext(int startIndex);
  
  int findPreviousStandardPatternIndex(int startIndex);
  int findNextStandardPatternIndex(int startIndex);
  
  int findNearbyStandardPatternIndex(int index);
  
  int findStartingPattern();
  
  void deletePatternsAndNodesAt(int position,
                                int numPatterns);
  
  void deletePatternRange(int startIndex,
                          int endIndex);
  
  void overwritePatternsAndNodesAt(int index,
                   LaylaPatternTokenVector& patterns);
  
  void overwritePatternsAt(int index,
                   LaylaPatternTokenVector& patterns);
  
  void insertPatternsAndNodesAt(int index,
                   LaylaPatternTokenVector& patterns);
  
  void insertPatternsAt(int index,
                   LaylaPatternTokenVector& patterns);
  
  // Insert patterns and nodes, with the guarantee that this insert
  // is not part of an overwrite
  void insertPatternsAndNodesAtPure(int index,
                   int originalPos,
                   LaylaPatternTokenVector& patterns);
  
  void deletePattern(int index);
  
  static int numPatternTokensInCollection(LaylaPatternTokenVector& patterns);
  
  TokenIndexCollection listOfSpawns() const;
  
  int numSpawns();
  int spawnPos(int spawnIndex);
  
  int nodeEntriesAtIndex(int index);
  
  int moveNode(int dstIndex,
                int srcIndex);
  
  int cloneNode(int dstIndex,
                int srcIndex);
  
  int deleteNode(int index);
  
  bool moveNodeItemLeft(int index);
  bool moveNodeItemRight(int index);
  
  int movePattern(int dstIndex,
                  int srcIndex);
  
  int movePatterns(int dstIndex,
                  int srcIndex,
                  int srcEndIndex);
  
  int addSpawn(int index);
  
  void generatePatternAreaTypeGuesses(PatternPreviewAreaTypeCollection& dst,
                                      LaylaPatternDefinitionTable&
                                        patternDefinitions);
  
  void fixReferencesAfterNewPatternAdded(int newPatternIndex);
  
  void fixReferencesAfterPatternRemoved(int patternIndex);
  
protected:
  typedef std::vector<int> TokenPositionCollection;

  // Pattern opcodes
  const static Tbyte opSpawnBoss_      = 0xFA; // spawn boss
  const static Tbyte opSetScrolling_   = 0xFB; // change scrolling
  const static Tbyte opSetArea_        = 0xFC; // change area type
  const static Tbyte opSetMap_         = 0xFD; // change map ID
  const static Tbyte opSetElevators_   = 0xFE; // change elevator destinations
  const static Tbyte opJumpToPosition_ = 0xFF; // jump to new read position
  
  // HACK: Asteroid 3 has what is almost certainly a mistake in a position
  // jump opcode that would cause it to jump into the fixed bank if scrolling
  // was not disabled beforehand. When that address is detected, we replace
  // it with what was probably the intended effect, a jump to the previous
  // pattern.
  const static int invalidJumpComputedOffset = 17404;
  const static int invalidJumpReplacementValue = -4;
  // Upon further inspection, the problem is actually worse: somehow the
  // third byte of the command was completely omitted. It's supposed to
  // be B9, but instead spills over into the next command (FD).

//  Tarray<Tbyte> patterns_;

  const static int numTokensInSpawnAdd = 4;

  LaylaPatternTokenVector patternData_;
  
  TokenPositionCollection spawnTokenPositions_;
  
  void cloneFrom(const LaylaArea& src);
  
  static void tokenize(const Tbyte* src,
                int& pos,
                LaylaPatternTokenVector& dst,
                Taddress startAddress,
                const Tarray<Taddress>& spawnTable);
  
  int countStandardPatterns(int start, int end);
                
  int countSpawns(int start, int end);
                
  int countSpawnsRemoved(int erasePos, int eraseEnd);
                
  int countSpawnsAdded(int insertPos, int insertEnd);
  
  void updateSpawnIndexAfterErase(int erasePos, int eraseEnd);
  
  void updateSpawnIndexAfterInsert(int insertPos, int insertEnd);
  
  void fixTokensAfterInsert(int insertPos,
                           int insertSize);
  
  void fixTokensAfterInsertPure(int insertPos,
                           int insertSize,
                           int origPos);
  
  void fixTokensAfterErase(int erasePos,
                           int eraseSize);
  
  void fixTokensAfterMoveInsert(int insertPos,
                           int insertSize);
  
  void fixTokensAfterMoveErase(int erasePos,
                           int eraseSize);
  
  void fixReferencesAfterEraseRaw(int erasePos,
                           int eraseSize,
                           int numSpawnsRemoved);
  
  void fixReferencesAfterInsertRaw(int insertPos,
                           int insertSize,
                           int numSpawnsAdded);
  
  void fixReferencesAfterInsertPure(int insertPos,
                           int insertSize,
                           int origPos,
                           int numSpawnsAdded);
  
  int correctEraseSpawnNum(int originalNum,
                           int originalPos,
                           int numSpawnsRemoved,
                           int erasePos,
                           int eraseEnd);
  
  int correctInsertSpawnNum(int originalNum,
                           int originalPos,
                           int numSpawnsAdded,
                           int insertPos,
                           int insertEnd);
  
  bool isInRange(int val, int lower, int upper);
  
  void filterStandardPatternTokens(LaylaPatternTokenVector& dst,
                                   LaylaPatternTokenVector& src);
                                   
  void reorderSpawns();
  
  int moveTokens(int dstIndex,
                 int srcIndex,
                 int srcSize);
                 
  void fixReferenceTokenAfterErase(int i,
                                    int erasePos,
                                    int eraseSize,
                                    int numSpawnsRemoved);
                 
  void fixReferenceTokenAfterInsert(int i,
                                    int insertPos,
                                    int insertSize,
                                    int numSpawnsAdded);
  
  void swapTokens(int first, int second);
  
  Tbyte findPreviousAreaType(int index);
  Tbyte findPreviousBackgroundMetatile(int index);
  Tbyte findPreviousKillsToNextFood(int index);
  Tbyte findPreviousYFlag(int index);
  
};


};


#endif
