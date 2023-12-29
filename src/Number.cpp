
#include <limits.h>
#include <stdio.h>
#include "Number.hpp"

template<>
std::complex<double> Zero<std::complex<double>>(void)
{
  return (std::complex<double>(0, 0));
}

template<>
std::complex<float> Zero<std::complex<float>>(void)
{
  return (std::complex<float>(0, 0));
}

template<>
std::complex<double>	Combine<std::complex<double>>(std::complex<double> const &a, std::complex<double> const &b)
{
  double		rl;
  double		im;
  
  if (rand() % 2)
    rl = a.real();
  else
    rl = b.real();
  if (rand() % 2)
    im = a.imag();
  else
    im = b.imag();
  return (std::complex<double>(rl, im));
}
template<>
std::complex<float>	Combine<std::complex<float>>(std::complex<float> const &a, std::complex<float> const &b)
{
  float			rl;
  float			im;
  
  if (rand() % 2)
    rl = a.real();
  else
    rl = b.real();
  if (rand() % 2)
    im = a.imag();
  else
    im = b.imag();
  return (std::complex<float>(rl, im));
}


template<>
std::complex<double> Random<std::complex<double>>(int maxabs)
{
  return (std::complex<double>(2 * (((double)rand() / INT_MAX) - 0.5) * maxabs,
			       2 * (((double)rand() / INT_MAX) - 0.5) * maxabs
			       ));
}

template<>
std::complex<float> Random<std::complex<float>>(int maxabs)
{
  return (std::complex<float>(2 * (((float)rand() / INT_MAX) - 0.5) * maxabs,
			      2 * (((float)rand() / INT_MAX) - 0.5) * maxabs
			      ));
}

template<>
std::complex<double> Round<std::complex<double>>(std::complex<double> const &v)
{
  return (std::complex<double>(floor(v.real()), floor(v.imag())));
}
template<>
std::complex<float> Round<std::complex<float>>(std::complex<float> const &v)
{
  return (std::complex<float>(floor(v.real()), floor(v.imag())));
}

template<>
std::complex<double>	Reverse(std::complex<double> const &v, std::complex<double> const &r, int max)
{
  double		rl = v.real();
  double		im = v.imag();

  if (r.real() > 0)
    {
      if (rl > 0)
	rl = max - rl;
      else
	rl = -max + rl;
    }
  if (r.imag() > 0)
    {
      if (im > 0)
	im = max - im;
      else
	im = -max + im;
    }
  return (std::complex<double>(rl, im));
}

template<>
std::complex<float>	Reverse(std::complex<float> const &v, std::complex<float> const &r, int max)
{
  float			rl = v.real();
  float			im = v.imag();

  if (r.real() > 0)
    {
      if (rl > 0)
	rl = max - rl;
      else
	rl = -max + rl;
    }
  if (r.imag() > 0)
    {
      if (im > 0)
	im = max - im;
      else
	im = -max + im;
    }
  return (std::complex<float>(rl, im));
}

template<>
std::string		Format<std::complex<double>>(std::complex<double> const &v)
{
  char			buffer[256];

  snprintf(buffer, sizeof(buffer), "(%+.4lf,%+.4lfi)", v.real(), v.imag());
  return (std::string(buffer));
}

template<>
std::string		Format<std::complex<float>>(std::complex<float> const &v)
{
  char			buffer[256];

  snprintf(buffer, sizeof(buffer), "(%+.4f,%+.4fi)", v.real(), v.imag());
  return (std::string(buffer));
}

template<>
uint64_t		Hash<std::complex<double>>(std::complex<double> const &v)
{
  uint64_t		a = 5381;

  a = ((a << 5) + a) + v.real();
  a = ((a << 5) + a) + v.imag();
  return (a);
}

template<>
uint64_t		Hash<std::complex<float>>(std::complex<float> const &v)
{
  uint64_t		a = 5381;

  a = ((a << 5) + a) + v.real();
  a = ((a << 5) + a) + v.imag();
  return (a);
}

template<>
int			Retrieve<std::complex<double>>(t_bunny_configuration	*cnf,
						       int			y,
						       int			x,
						       std::complex<double>	&data)
{
  double		a;
  double		b;

  if (!bunny_configuration_getf_double(cnf, &a, "[%d][%d]", y, x + 0))
    throw std::exception();
  if (!bunny_configuration_getf_double(cnf, &b, "[%d][%d]", y, x + 1))
    throw std::exception();
  data = std::complex<double>(a, b);
  return (2);
}

template<>
int			Retrieve<std::complex<float>>(t_bunny_configuration	*cnf,
						      int			y,
						      int			x,
						      std::complex<float>	&data)
{
  double		a;
  double		b;

  if (!bunny_configuration_getf_double(cnf, &a, "[%d][%d]", y, x + 0))
    throw std::exception();
  if (!bunny_configuration_getf_double(cnf, &b, "[%d][%d]", y, x + 1))
    throw std::exception();
  data = std::complex<float>(a, b);
  return (2);
}

template<>
int			Retrieve<double>(t_bunny_configuration			*cnf,
					 int					y,
					 int					x,
					 double					&data)
{
  if (!bunny_configuration_getf_double(cnf, &data, "[%d][%d]", y, x))
    throw std::exception();
  return (1);
}

template<>
int			Retrieve<float>(t_bunny_configuration			*cnf,
					int					y,
					int					x,
					float					&data)
{
  double		d;
  
  if (!bunny_configuration_getf_double(cnf, &d, "[%d][%d]", y, x))
    throw std::exception();
  data = d;
  return (1);
}


template<>
int			Export<std::complex<double>>(t_bunny_configuration	*cnf,
						     int			y,
						     int			x,
						     std::complex<double>	data)
{
  double		a = data.real();
  double		b = data.imag();

  if (!bunny_configuration_setf_double(cnf, a, "[%d][%d]", y, x + 0))
    throw std::exception();
  if (!bunny_configuration_setf_double(cnf, b, "[%d][%d]", y, x + 1))
    throw std::exception();
  return (2);
}

template<>
int			Export<std::complex<float>>(t_bunny_configuration	*cnf,
						    int				y,
						    int				x,
						    std::complex<float>		data)
{
  double		a = data.real();
  double		b = data.imag();

  if (!bunny_configuration_setf_double(cnf, a, "[%d][%d]", y, x + 0))
    throw std::exception();
  if (!bunny_configuration_setf_double(cnf, b, "[%d][%d]", y, x + 1))
    throw std::exception();
  return (2);
}

template<>
int			Export<double>(t_bunny_configuration			*cnf,
				       int					y,
				       int					x,
				       double					data)
{
  if (!bunny_configuration_setf_double(cnf, data, "[%d][%d]", y, x))
    throw std::exception();
  return (1);
}

template<>
int			Export<float>(t_bunny_configuration			*cnf,
				      int					y,
				      int					x,
				      float					data)
{
  double		d = data;
  
  if (!bunny_configuration_setf_double(cnf, d, "[%d][%d]", y, x))
    throw std::exception();
  return (1);
}

