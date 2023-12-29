
#ifndef		EMPTY_HPP
# define	EMPTY_HPP

class		Empty : public Trainer<std::complex<double>, 10>
{
  using MathValue = std::complex<double>;

  double	Test(NeuronNetwork<MathValue, 10>	&val)
  {
    double	score;

    score = 0;
    for (int i = -10; i <= 10; ++i)
      {
	val.Reset();
	val.Set(0, i);
	if (round(val.Get(0).real()) == i)
	  score += 10;
	else
	  score -= abs(val.Get(0).real() - i);
      }
    return (score);
  }

public:
  double	operator()(size_t		val)
  {
    auto	&best = GetBest();

    best.Reset();
    return (best.Get(0).real());
  }

  Empty(size_t	ind = 1)
    : Trainer({1, 1}, ind)
  {}
  ~Empty(void)
  {}
};

#endif	//	EMPTY_HPP
