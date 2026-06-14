
#ifndef			ROUND_HPP
# define		ROUND_HPP
# include		<complex>
# include		<stdint.h>
# include		<cmath>
# include		<sstream>
# include		<string>
# include		<lapin.h>

template <typename	T>
T			Zero(void)
{
  return (0);
}

template<> std::complex<double> Zero<std::complex<double>>(void);
template<> std::complex<float> Zero<std::complex<float>>(void);

template <typename	T>
T			One(void)
{
  return (1);
}

template<> inline std::complex<double> One<std::complex<double>>(void)
{
  return (std::complex<double>(1, 1));
}

template<> inline std::complex<float> One<std::complex<float>>(void)
{
  return (std::complex<float>(1, 1));
}

template <typename	T>
T			Scale(T const	&v,
			      double	scale)
{
  return (v * scale);
}

template<> inline std::complex<float> Scale<std::complex<float>>(std::complex<float> const &v,
								 double scale)
{
  return (v * (float)scale);
}

template <typename	T>
T			Conjugate(T const &v)
{
  return (v);
}

template<> inline std::complex<double> Conjugate<std::complex<double>>(std::complex<double> const &v)
{
  return (std::conj(v));
}

template<> inline std::complex<float> Conjugate<std::complex<float>>(std::complex<float> const &v)
{
  return (std::conj(v));
}

template <typename	T>
T			NeuralMultiply(T const &a, T const &b)
{
  return (a * b);
}

template<> inline std::complex<double> NeuralMultiply<std::complex<double>>(std::complex<double> const &a,
								 std::complex<double> const &b)
{
  return (std::complex<double>(a.real() * b.real(), a.imag() * b.imag()));
}

template<> inline std::complex<float> NeuralMultiply<std::complex<float>>(std::complex<float> const &a,
							       std::complex<float> const &b)
{
  return (std::complex<float>(a.real() * b.real(), a.imag() * b.imag()));
}

inline double		Sigmoid(double v)
{
  if (v < -60.0)
    return (0.0);
  if (v > 60.0)
    return (1.0);
  return (1.0 / (1.0 + std::exp(-v)));
}

template <typename	T>
T			Activation(T const &v)
{
  return ((T)Sigmoid((double)v));
}

template<> inline std::complex<double> Activation<std::complex<double>>(std::complex<double> const &v)
{
  return (std::complex<double>(Sigmoid(v.real()), Sigmoid(v.imag())));
}

template<> inline std::complex<float> Activation<std::complex<float>>(std::complex<float> const &v)
{
  return (std::complex<float>((float)Sigmoid(v.real()), (float)Sigmoid(v.imag())));
}

template <typename	T>
T			ActivationDerivativeFromOutput(T const &v)
{
  return (v * (One<T>() - v));
}

template<> inline std::complex<double> ActivationDerivativeFromOutput<std::complex<double>>(std::complex<double> const &v)
{
  return (std::complex<double>(v.real() * (1.0 - v.real()),
			       v.imag() * (1.0 - v.imag())));
}

template<> inline std::complex<float> ActivationDerivativeFromOutput<std::complex<float>>(std::complex<float> const &v)
{
  return (std::complex<float>(v.real() * (1.0f - v.real()),
			      v.imag() * (1.0f - v.imag())));
}

template <typename	T>
double			SquaredMagnitude(T const &v)
{
  return ((double)(v * v));
}

template<> inline double SquaredMagnitude<std::complex<double>>(std::complex<double> const &v)
{
  return (std::norm(v));
}

template<> inline double SquaredMagnitude<std::complex<float>>(std::complex<float> const &v)
{
  return ((double)std::norm(v));
}

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
						    int			y,
						    int			x,
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
