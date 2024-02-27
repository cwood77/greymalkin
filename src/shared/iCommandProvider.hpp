#ifndef ___shared_iCommandProvider___
#define ___shared_iCommandProvider___

namespace shared {

class context;

class iCommandProvider {
public:
   virtual ~iCommandProvider() {}
   virtual void provide(context& ctxt) = 0;
};

} // namespace shared

#endif // ___shared_iCommandProvider___
