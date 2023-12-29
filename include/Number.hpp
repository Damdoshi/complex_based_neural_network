
#ifndef			ROUND_HPP
# define		ROUND_HPP
# include		<complex>
# include		<stdint.h>
# include		<lapin.h>

template <typename	T>
T			Zero(void)
{
  return (0);
}

template<> std::complex<double> Zero<std::complex<double>>(void);
template<> std::complex<float> Zero<std::complex<float>>(void);

template <typename	T>
T			Combine(T const &a, T const &b)
{
  if (rand() % 2)
    return (a);
  return (b);
}

template<>
std::complex<double>	Combine<std::complex<double>>(std::complex<double> const &a, std::complex<double> const &b);
template<>
std::complex<float>	Combine<std::complex<float>>(std::complex<float> const &a, std::complex<float> const &b);

template <typename	T>
T			Random(int		maxabs)
{
  return ((((T)rand() / INT_MAX) - 0.5) * 2 * maxabs);
}

template<>
std::complex<double>	Random<std::complex<double>>(int maxabs);
template<>
std::complex<float>	Random<std::complex<float>>(int maxabs);

template <typename	T>
T			Round(T const &v)
{
  return (floor(v));
}
template<>
std::complex<double>	Round<std::complex<double>>(std::complex<double> const &v);

template<>
std::complex<float>	Round<std::complex<float>>(std::complex<float> const &v);

template <typename	T>
T			Reverse(T const &v, T const &reverse, int max)
{
  if (reverse < 0)
    return (v);
  if (v > 0)
    v = max - v;
  else
    v = -max + v;
  return (v);
}

template<>
std::complex<double>	Reverse<std::complex<double>>(std::complex<double> const &v, std::complex<double> const &r, int max);

template<>
std::complex<float>	Reverse<std::complex<float>>(std::complex<float> const &v, std::complex<float> const &r, int max);

template <typename	T>
std::string		Format(T const &v)
{
  std::stringstream	ss;

  ss << v;
  return (ss.str());
}

template<>
std::string		Format<std::complex<double>>(std::complex<double> const &v);
template<>
std::string		Format<std::complex<float>>(std::complex<float> const &v);

template<typename T>
uint64_t		Hash(T const &v)
{
  return (v);
}

template<>
uint64_t		Hash<std::complex<double>>(std::complex<double> const &v);
template<>
uint64_t		Hash<std::complex<float>>(std::complex<float> const &v);

template<typename T>
int			Retrieve(t_bunny_configuration	*cnf,
				 int			y,
				 int			x,
				 T			&data)
{
  if (!bunny_configuration_getf_int(cnf, &data, "[%d][%d]", y, x))
    throw std::exception();
  return (1);
}

template<>
int			Retrieve<std::complex<double>>(t_bunny_configuration	*cnf,
						       int			y,
						       int			x,
						       std::complex<double>	&data);

template<>
int			Retrieve<std::complex<float>>(t_bunny_configuration	*cnf,
						      int			y,
						      int			x,
						      std::complex<float>	&data);

template<>
int			Retrieve<double>(t_bunny_configuration			*cnf,
					 int					y,
					 int					x,
					 double					&data);

template<>
int			Retrieve<float>(t_bunny_configuration			*cnf,
					 int					y,
					 int					x,
					 float					&data);


template<typename T>
int			Export(t_bunny_configuration				*cnf,
			       int						y,
			       int						x,
			       T						data)
{
  if (!bunny_configuration_setf_int(cnf, data, "[%d][%d]", y, x))
    throw std::exception();
  return (1);
}

template<>
int			Export<std::complex<double>>(t_bunny_configuration	*cnf,
						     int			y,
						     int			x,
						     std::complex<double>	data);

template<>
int			Export<std::complex<float>>(t_bunny_configuration	*cnf,
						    int				y,
						    int				x,
						    std::complex<float>		data);

template<>
int			Export<double>(t_bunny_configuration			*cnf,
				       int					y,
				       int					x,
				       double					data);

template<>
int			Export<float>(t_bunny_configuration			*cnf,
					 int					y,
					 int					x,
					 float					data);


#endif	//		ROUND_HPP
