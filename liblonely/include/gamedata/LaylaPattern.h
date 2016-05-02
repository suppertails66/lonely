#ifndef LAYLAPATTERN_H
#define LAYLAPATTERN_H


#include "structs/Tbyte.h"
#include "structs/Taddress.h"
#include "structs/Tstring.h"
#include "structs/SaveHelper.h"
#include "structs/LoadHelper.h"
#include "structs/Graphic.h"
#include "gamedata/LaylaMetatile.h"
#include "editor/LonelyMetatileCache.h"
#include <vector>

namespace Lonely {


class LaylaPattern {
public:
	const static int metatilesPerColumn = 0x0C;
	
	LaylaPattern();
	
	LaylaPattern(const Tbyte* src);
	
	int readFromData(const Tbyte* src);
	
	int writeToData(Tbyte* dst) const;
  
  int save(Tstring& data) const;
  
  int load(const Tbyte* data);
	
	int safeCompressedDataAllocationSize() const;
	
	int numMetatiles() const;
	int numColumns() const;
	
	void changeWidth(int newWidth);
	
	Tbyte metatileID(int index) const;
	Tbyte metatileID(int column, int row) const;
	
	void setMetatileID(int index, Tbyte value);
	void setMetatileID(int column, int row, Tbyte value);
	
	int realWidth() const;
	int realHeight() const;
	
	void draw(Graphic& dst,
	          int x, int y,
	          const MetatileCacheCollection& metatiles,
	          bool drawMarker = false) const;
  
  void fixAfterMetatileQuantityChange(
    int numMetatiles,
    int index);
protected:
	typedef std::vector<Tbyte> PatternDataCollection;
	
	int positionToIndex(int column, int row) const;
	
	int allocationSizeForColumns(int num);
	
	const static int compressionCheckMask_ = 0x80;
	const static int compressionIDMask_ = 0x7F;
	const static int endOfPatternToken_ = 0xFF;
	
	const static int minRunLength = 3;
	const static int maxEncodableValue = 0x7F;
	const static int maxRunLength = 255;
	
	PatternDataCollection patternData_;
};


};


#endif
