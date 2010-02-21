

template<typename Lang, typename Cxx>
void lang2cxx_array(Lang* tab, size_t len, std::vector<Cxx>& vect)
{
  vect.reserve(len);
  for (size_t i = 0; i < len; ++i)
    vect.push_back( lang2cxx<Lang, Cxx>tab[i] );
}
