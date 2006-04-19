#ifndef otbScrollImageView_txx
#define otbScrollImageView_txx

// OTB-FA-00026-CS
#include "otbScrollImageView.h"


namespace otb
{

template <class TPixel>
ScrollImageView<TPixel>::
ScrollImageView() : GLVectorImageViewBase<TPixel>()
  {
  }

template <class TPixel>
ScrollImageView<TPixel>::
~ScrollImageView()
{

}

template <class TPixel>
int
ScrollImageView<TPixel>::handle(int event)
  {

  int key;
  
  switch(event)
    {
    case FL_PUSH:
      {
        int x = Fl::event_x();
        int y = Fl::event_y();
        IndexType lCenter;
        IndexType lCenterPointImage;
        lCenter[0] = x;
        lCenter[1] = y;
        //Position du centre de l'image en coordonnees image
        lCenterPointImage = this->WindowCoord2ImageCoord(lCenter);
        // Mise a jour de la fenetre Zoom
        this->GetViewer()->ScrollAction(lCenterPointImage);
      }
      break;
    case FL_KEYUP:
    break;
    case FL_FOCUS :
                this->update();
                this->GetViewer()->ScrollResize();
                return 1;
                break;
    case FL_HIDE :
                this->GetViewer()->Hide();
                return 0;
                break;
    case FL_KEYBOARD:
    case FL_SHORTCUT:
    key = Fl::event_text()[0];
      switch(key)
        {
        case 'u':
        case 'U':
              this->update();
              return 1;
              break;
    default:
      break;
      }
  }
  
  return 0;
  
}
 
}; //namespace
#endif

