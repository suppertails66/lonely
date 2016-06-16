#include "gamedata/LaylaPattern.h"
#include "gamedata/LaylaMetatile.h"
#include "util/StringConversion.h"
#include "util/ByteConversion.h"
#include "exception/OutOfRangeIndexException.h"
#include <iostream>

namespace Lonely {


LaylaPattern::LaylaPattern()
  : inheritPreviousLayout_(false) { };
	
LaylaPattern::LaylaPattern(const Tbyte* src)
  : inheritPreviousLayout_(false) {
	readFromData(src);
}

int LaylaPattern::readFromData(const Tbyte* src) {

  patternData_.clear();

	Tbyte nextByte = *(src++);
  int total = 1;
	
	while (nextByte != endOfPatternToken_) {
		// Check for repeat count
		if ((nextByte & compressionCheckMask_) != 0) {
			Tbyte id = nextByte & compressionIDMask_;
			int repeatCount = *(src++);
			++total;
			
			for (int i = 0; i < repeatCount; i++) {
				patternData_.push_back(id);
			}
		}
		// If not a repeat, single pattern
		else {
			patternData_.push_back(nextByte);
		}
		
		nextByte = *(src++);
	  ++total;
	}
	
/*	for (int i = 0; i < patternData_.size(); i++) {
	  std::cout << StringConversion::intToString(
	                  patternData_[i],
	                  StringConversion::baseHex) << " ";
	}
	std::cout << std::endl; */
	
	return total;
}
	
int LaylaPattern::writeToData(Tbyte* dst) const {
  int i = 0;
  int total = 0;
  int toNextColumn = metatilesPerColumn;
  while (i < patternData_.size()) {
    // Count occurences of next pattern ID
    Tbyte next = patternData_[i++];
    --toNextColumn;
    int runLength = 1;
    
    while ((i < patternData_.size())
           && (toNextColumn > 0)
           && (patternData_[i] == next)
           && (runLength <= maxRunLength)) {
      ++i;
      ++runLength;
      --toNextColumn;
    }
    
    // IDs greater than 0x7F must always be absolute.
    // Note that this means the maximum number of patterns is effectively
    // 255, since 0xFF is the end-of-pattern token and can't be represented
    // in an absolute or encoded run.
    if ((next > maxEncodableValue)
        || (runLength < minRunLength)) {
      // Absolute run
      for (int j = 0; j < runLength; j++) {
        *(dst++) = next;
      }
      
      total += runLength;
    }
    else {
      // Encoded run
      Tbyte encByte = (next | compressionCheckMask_);
      *(dst++) = encByte;
      *(dst++) = (Tbyte)(runLength);
      
      total += 2;
    }
    
    if (toNextColumn == 0) {
      toNextColumn = metatilesPerColumn;
    }
  }
  
  // Write end of pattern token
  *(dst++) = endOfPatternToken_;
  ++total;
  
  return total;
}
  
int LaylaPattern::save(Tstring& data) const {
  int byteCount = 0;
  
  byteCount += SaveHelper::saveInt(data, patternData_.size(),
                                   ByteSizes::uint32Size);
  
  for (int i = 0; i < patternData_.size(); i++) {
    data += (char)(patternData_[i]);
  }
  byteCount += patternData_.size();
  
  return byteCount;
}

int LaylaPattern::load(const Tbyte* data) {
  int byteCount = 0;
  
  patternData_.resize(LoadHelper::loadInt(data, byteCount,
                      ByteSizes::uint32Size));
  
  for (int i = 0; i < patternData_.size(); i++) {
    patternData_[i] = *(data + byteCount + i);
  }
  byteCount += patternData_.size();
  
  return byteCount;
}

int LaylaPattern::safeCompressedDataAllocationSize() const {
  return patternData_.size() * 2;
}
	
int LaylaPattern::numMetatiles() const {
	return patternData_.size();
}

int LaylaPattern::numColumns() const {
	return numMetatiles() / metatilesPerColumn;
}
	
void LaylaPattern::changeWidth(int newWidth) {
  int allocationSize = allocationSizeForColumns(newWidth);
  PatternDataCollection newData(allocationSize, 0);
  
  int copyLength
    = (newWidth > numColumns())
      ? numMetatiles()
      : allocationSize;
  for (int i = 0; i < copyLength; i++) {
    newData[i] = patternData_[i];
  }
  
  patternData_ = newData;
}
	
int LaylaPattern::allocationSizeForColumns(int num) {
  return num * metatilesPerColumn;
}
	
Tbyte LaylaPattern::metatileID(int index) const {
	if (index >= patternData_.size()) {
		throw OutOfRangeIndexException(TALES_SRCANDLINE,
																	 "LaylaPattern::metatileID(int)",
																	 index);
	}
	
	return patternData_[index];
}

Tbyte LaylaPattern::metatileID(int column, int row) const {
	return metatileID(positionToIndex(column, row));
}
	
void LaylaPattern::setMetatileID(int index, Tbyte value) {
  patternData_[index] = value;
}

void LaylaPattern::setMetatileID(int column, int row, Tbyte value) {
  patternData_[positionToIndex(column, row)] = value;
}
	
int LaylaPattern::realWidth() const {
  return numColumns() * LaylaMetatile::width;
}

int LaylaPattern::realHeight() const {
  return metatilesPerColumn * LaylaMetatile::height;
}
	
void LaylaPattern::draw(Graphic& dst,
          int x, int y,
          const MetatileCacheCollection& metatiles,
	        bool drawMarker) const {
  for (int j = 0; j < metatilesPerColumn; j++) {
    for (int i = 0; i < numColumns(); i++) {
      int dstX = x + (i * LaylaMetatile::width);
      int dstY = y + (j * LaylaMetatile::height);
      
      Tbyte id = metatileID(i, j);
      
      // Don't draw invalid metatiles (this can happen e.g. if a pattern from
      // one area is drawn with the metatileset of an area with fewer
      // metatiles)
      if (id >= metatiles.size()) {
        continue;
      }
      
      dst.copy(metatiles[metatileID(i, j)],
               Box(dstX, dstY, 0, 0),
               Graphic::noTransUpdate);
    }
  }
  
  if (drawMarker) {
    dst.drawLine(x, y,
                 x, y + realHeight(),
                 Tcolor(0xFF, 0x00, 0x00, Tcolor::fullAlphaOpacity),
                 3,
                 Graphic::noTransUpdate);
  }
}
	
int LaylaPattern::positionToIndex(int column, int row) const {
	return (column * metatilesPerColumn) + row;
}
  
void LaylaPattern::fixAfterMetatileQuantityChange(
    int numMetatiles,
    int index) {
  for (int i = 0; i < patternData_.size(); i++) {
    // Deletion
    if (numMetatiles < 0) {
      if (patternData_[i] >= index) {
        patternData_[i] += numMetatiles;
      }
    }
    // Insertion
    else if (numMetatiles > 0) {
      if (patternData_[i] >= index) {
        patternData_[i] += numMetatiles;
      }
    }
  }
}
  
bool LaylaPattern::inheritPreviousLayout() const {
  return inheritPreviousLayout_;
}

void LaylaPattern::setInheritPreviousLayout(bool inheritPreviousLayout__) {
  inheritPreviousLayout_ = inheritPreviousLayout__;
}


};
