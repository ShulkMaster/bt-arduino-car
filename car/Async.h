struct Task {
  bool resolved;
  int resolveTime;
};

template <typename T, typename C>
struct Future {
  unsigned long resolveTime;
  C* obj;
  T (*resolve)(C* object);
};
