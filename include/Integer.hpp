
#ifndef			INTEGER_HPP
# define		INTEGER_HPP
# define		INTEGER_INPUTS				1

class			Integer : public Trainer<std::complex<double>, 10>
{
  using MathValue = std::complex<double>;

  double		Test(NeuronNetwork<MathValue, 10>	&val)
  {
    val.Reset();
    for (int i = 0; i < INTEGER_INPUTS; ++i)
      val.Set(i, 1);
    return (val.Get(0).real());
  }

public:
  double		operator()(void)
  {
    auto		&best = GetBest();
    
    best.Reset();
    for (int i = 0; i < INTEGER_INPUTS; ++i)
      best.Set(i, 1);
    return (best.Get(0).real());
  }

  Integer(size_t	ind = 100)
    : Trainer({INTEGER_INPUTS, 50, 50, 50, 1}, ind)
  {}
  ~Integer(void)
  {}
};

#endif	//		INTEGER_HPP

