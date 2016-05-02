#ifndef INDEXEDPICKERSCENEFUNCTOR_H
#define INDEXEDPICKERSCENEFUNCTOR_H


namespace Lonely {


class IndexedPickerSceneItemPicked {
public:
  IndexedPickerSceneItemPicked();
  
  virtual ~IndexedPickerSceneItemPicked();
  
  virtual void operator()(int index);
protected:
  
};


};


#endif
