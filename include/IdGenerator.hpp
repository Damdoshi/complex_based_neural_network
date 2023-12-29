
#ifndef			ID_GENERATOR_HPP
# define		ID_GENERATOR_HPP
# include		<sys/types.h>

class			IdGenerator
{
private:
  static size_t		id_generator;

public:
  operator		size_t (void)
  {
    return (id_generator++);
  }
  IdGenerator() {}
  ~IdGenerator() {}
};

#endif	//		ID_GENERATOR_HPP
