#ifndef LAYLAOBJECTPATTERNTABLE_H
#define LAYLAOBJECTPATTERNTABLE_H


#include "gamedata/LaylaObjectPatternToken.h"
#include "gamedata/LaylaGraphics.h"
#include "gamedata/LaylaPalettes.h"
#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "structs/TpointerVector.h"
#include "structs/Tstring.h"
#include "structs/Graphic.h"
#include "nes/NesPatternTable.h"
#include "nes/NesPaletteQuad.h"
#include <vector>

namespace Lonely {


class LaylaObjectPatternTable {
public:
  LaylaObjectPatternTable();
  
  LaylaObjectPatternTable(const Tbyte* src,
                          int numEntries,
                          Taddress baseAddress,
                          int dataSize);
  
  ~LaylaObjectPatternTable();
  
  LaylaObjectPatternTable(const LaylaObjectPatternTable& src);
  LaylaObjectPatternTable& operator=(const LaylaObjectPatternTable& src);
  
  void readFromData(const Tbyte* src,
                    int numEntries,
                    Taddress baseAddress,
                    int dataSize);
  
  void writeToData(Tbyte* dst,
                   Taddress baseAddress,
                   int maxSize) const;
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
  
  int numPatternTokens() const;
  
  LaylaObjectPatternToken& patternToken(int index);
  const LaylaObjectPatternToken& patternToken(int index) const;
	
	int totalPatternDataSize() const;
  
  void drawPatternObjectOverlay(Graphic& dst,
                                int x, int y,
                                double zoom,
                                int patternIndex,
                                int patternMetatileWidth,
                                int objectSetNum,
                                const NesPatternTable& patternTable,
                                const NesPaletteQuad& palettes);
  
  void drawBossOverlay(Graphic& dst,
                                int x, int y,
                                double zoom,
                                int bossID,
                                int objectSetNum,
                                const NesPatternTable& patternTable,
                                const NesPaletteQuad& palettes);
  
  Tstring descriptionString(int index) const;
  
  int indexOfPattern(int patternNum) const;
  
  void setIndexOfPattern(int patternNum,
                         int newIndex);
  
  void insertPatternIntoIndex(int index);
  
  void removePatternFromIndex(int index);
  
  int removeObjectPattern(int index);
  
  int addObjectPattern(int index, LaylaObjectPatternToken& token);
  
  void swapItems(int first, int second);
  
  void drawEntryIcon(Graphic& dst,
                     int entryIndex,
                     int objectSetNum,
                     const NesPatternTable& patternTable,
                     const NesPaletteQuad& palettes) const;
  
  void drawEntryIcon(Graphic& dst,
                     int entryIndex,
                     int levelNum,
                     int objectSetNum,
                     const LaylaGraphics& graphics,
                     const LaylaPalettes& palettes) const;
  
  void drawObjectIcon(Graphic& dst,
                     int levelNum,
                     int objectNum,
                     int objectSetNum,
                     const LaylaGraphics& graphics,
                     const LaylaPalettes& palettes) const;
  
  void drawObjectIcon(Graphic& dst,
                     int objectNum,
                     int objectSetNum,
                     const NesPatternTable& patternTable,
                     const NesPaletteQuad& palettes) const;
                         
protected:
  typedef TpointerVector<LaylaObjectPatternToken>
      LaylaObjectPatternTokenVector;
  typedef std::vector<int> ObjectTokenPositionCollection;
  
  const static Tbyte expandedSpawnMask_ = 0x20;
  const static Tbyte delayMask_ = 0x80;
  
  LaylaObjectPatternTokenVector patterns_;
  
  ObjectTokenPositionCollection patternIndices_;
  
  void cloneFrom(const LaylaObjectPatternTable& src);
  
  void drawObjectAndAdvance(Graphic& dst,
                            int& drawX, int& drawY,
                            double zoom,
                            int objectSetNum,
                            Tbyte localObjectID,
                            const NesPatternTable& patternTable,
                            const NesPaletteQuad& palettes,
                            bool continueInterpreting,
                            int& remaining) const;
  
  void drawSprite(Graphic& dst,
                            Graphic& src,
                            int drawX, int drawY,
                            double zoom) const;
                            
};


};


#endif
