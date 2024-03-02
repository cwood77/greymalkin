#ifndef ___window_api___
#define ___window_api___

#include <cstddef>

namespace cui { class iPort; }
namespace cui { class screenBuffer; }

namespace window {

class iLayout;
class iModelPainter;
class iWindow;

class iManager {
public:
   virtual ~iManager() {}
   virtual iLayout& create(cui::screenBuffer& screen) = 0;
};

// collection of windows such that each addressable spot belongs to a window
class iLayout {
public:
   virtual ~iLayout() {}

   virtual iWindow& getIth(size_t i) = 0;
   /*
   virtual void rearrange();

   virtual iWindow *getLeft(iWindow& w);
   virtual iWindow *getRight(iWindow& w);
   virtual iWindow *getTop(iWindow& w);
   virtual iWindow *getBottom(iWindow& w);

   virtual void swap(iWindow& w1, iWindow& w2);
   */
};

// windows can find each other, know their dims and topRow (scrolling)
class iWindow {
public:
   virtual ~iWindow() {}

   //virtual iLayout& getLayout();
#if 0
   virtual cui::iPort& getPort();
   virtual void bind(windowBuffer& b);
#endif
   virtual void bind(iModelPainter& b) = 0;
   //virtual void maximize();
   virtual void invalidate() = 0;
};

/*
   // implements iPort sorta, but only allows certain usage patterns
   class iVirtualPort : public cui::iPort {
   public:
      virtual void playback(iWindow& w) = 0;
   };
   */

// writes entire model to the (v)port everytime!
class iModelPainter {
public:
   virtual ~iModelPainter() {}
   virtual void draw(cui::iPort& p) = 0;
};

#if 0
// -------------------------------------------------------------------------------------

// infinite logical buffer that's written to a window; survives across resizes
class windowBuffer {
public:
   cui::iPort& getPort();
};

// implements iPort sorta, but only allows certain usage patters
class virtualPort : public cui::iPort {
public:
private:
   fgcol::type m_fillF;
   bgcol::type m_fillB;
   char m_fillC;
};

// -------------------------------------------------------------------------------------

class iLayer {
public:
   virtual void write(
      size_t x, size_t y,
      cui::fgcol::type fg, cui::bgcol::type bg,
      char c) = 0;
};

class iLayerSet {
public:
   virtual void write(
      size_t x, size_t y,
      cui::fgcol::type fg, cui::bgcol::type bg,
      char c) = 0;
};

class baseLayer : public iLayer {
public:
private:
   cui::iPort *m_pPort;
};

class iPainter {
public:
   size_t m_w;
   size_t m_h;
   iLayerSet *m_pLayers;
};

class model {
public:
   std::list<std::string> lines;
};
#endif

} // namespace window

#endif // ___window_api___
