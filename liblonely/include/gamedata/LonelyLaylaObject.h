#ifndef LONELYLAYLAOBJECT_H
#define LONELYLAYLAOBJECT_H


#include "structs/Tstring.h"

namespace Lonely {


/**
 * Container for Lonely's program-specific enum of game object types.
 * Layla itself does not assign objects unique IDs (each gameplay bank
 * contains 16 different objects), so Lonely has to make some up for keeping
 * track of things.
 *
 * Note that bosses do have consistent IDs, so they use a separate enum.
 * @see LaylaBoss
 */
class LonelyLaylaObjects {
public:
  enum LonelyLaylaObject {
    none = 0,
    // standard objects (defined in all banks)
    unk1,
    unk2,
    fallingWeight,
    keyCard,
    trappedIris,
    undefined,      // entry in init code jump table is always 0000
    // mutants
    purim,          // slime
    ben,            // sinusoidal fly
    spagani,        // spider
    flippedSpagani, // upside-down spider
    friendy,        // grim reaper
    jethro,         // octopus thing
    apara,          // flipping thing
    envy,           // vanishing oval thing
    jondy,          // snake
    mirako,         // big bug alien
    miichin,        // hopping thing
    shiilance,      // straight left fish
    koan,           // sinusoidal crystal
    bilganof,       // sinusoidal fish
    // robots
    soudchil,       // straight left fly
    pogos,          // walk and shoot robot
    vandol,         // fly and chase robot
    dolgadol,       // beheadable flying robot
    buldora,        // up and down robot
    rogin,          // roll and shoot robot
    at3,            // small walking robot?
    torso,          // big walking robot
    usumiil,        // crusher
    valketta,       // sinusoidal lamp thing
    gramzi,         // fly around and shoot jet robot
    // special
    manitokha       // dude hanging around on last boss
  };
  
  static Tstring nameOfObject(LonelyLaylaObject object);
protected:
  
};


};


#endif
