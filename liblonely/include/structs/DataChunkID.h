#ifndef DATACHUNKID_H
#define DATACHUNKID_H


namespace Lonely {


/**
 * Container for enum of resource chunk IDs.
 */
class DataChunkIDs {
public:
  
  /**
   * Enum of resource chunk IDs.
   * Do NOT insert new entries in the middle of this list -- always append!
   * These are used to identify data during serialization, and changing
   * existing IDs will break save file compatibility between different
   * versions.
   */
  enum DataChunkID {
    null                            = 0,
    fileStart,
    fileEnd,
    LaylaData,
    LaylaLevelSet,
    LaylaLevel,
    LaylaArea,
    LaylaPalettes,
    LaylaGraphics,
    LaylaStaticMetatiles,
    LonelyEditor,
    LaylaObjectSet,
    LonelyEditorData,
    LastExportString
  };
};


};


#endif
