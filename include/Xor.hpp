
#ifndef			XOR_HPP
# define		XOR_HPP
# include		"Trainer.hpp"

class			Xor : public Trainer<std::complex<double>, 1>
{
  using MathValue = std::complex<double>;

  double		Test(NeuronNetwork<MathValue, 1>	&val)
  {
    double		rscore = 0;
    double		iscore = 0;
    int			i;

    for (i = 0; i < 4; ++i)
      {
	int		b0 = (i >> 0) & 1;
	int		b1 = (i >> 1) & 1;
	  
	val.Reset();
	val.Set(0, b0);
	val.Set(1, b1);

	MathValue	ret = val.Get(0);
	bool		bb0 = !!b0;
	bool		bb1 = !!b1;
	bool		nxor = (bb0 == bb1);
	bool		rxor = !nxor;

	rscore += (round(ret.real()) == rxor);
	iscore += (round(ret.imag()) == nxor);
      }
    //std::cout << rscore << " " << iscore << std::endl;
    return (rscore + iscore);
  }

public:
  std::string		operator()(bool				a,
				   bool				b)
  {
    auto		&best = GetBest();
    std::stringstream	ss;

    best.Reset();
    best.Set(0, a);
    best.Set(1, b);
    MathValue		mv = best.Get(0);

    ss << "(" << (int)mv.real() << ", " << (int)mv.imag() << ")";
    return (ss.str());
  }

  std::string		BackpropResult(bool			a,
				       bool			b)
  {
    auto		&best = GetBest();
    std::stringstream	ss;

    best.Reset();
    best.Set(0, MathValue(a, a));
    best.Set(1, MathValue(b, b));
    MathValue		mv = best.Get(0);

    ss << "(" << mv.real() << ", " << mv.imag() << ")";
    return (ss.str());
  }

  double		Backpropagate(size_t		epochs,
			      double		learning_rate = 1.0,
			      double		target_error = 0.001,
			      bool		initialize = true)
  {
    std::vector<std::pair<std::vector<MathValue>, std::vector<MathValue>>> samples;

    samples.push_back({{MathValue(0, 0), MathValue(0, 0)}, {MathValue(0, 1)}});
    samples.push_back({{MathValue(0, 0), MathValue(1, 1)}, {MathValue(1, 0)}});
    samples.push_back({{MathValue(1, 1), MathValue(0, 0)}, {MathValue(1, 0)}});
    samples.push_back({{MathValue(1, 1), MathValue(1, 1)}, {MathValue(0, 1)}});
    return (Trainer<MathValue, 1>::Backpropagate(samples, epochs, learning_rate, target_error, initialize));
  }

  bool			Save(const std::string			&file) const
  {
    if (GetBestScore() == 8)
      return (Trainer<MathValue, 1>::Save(file, &GetBest()));
    return (Trainer<MathValue, 1>::Save(file));
  }

  bool			SaveBest(const std::string		&file) const
  {
    return (Trainer<MathValue, 1>::Save(file, &GetBest()));
  }

  Xor(const std::string	&file)
    : Trainer(file)
  {}
  Xor(std::vector<int> const &topology,
      size_t		ind = 100)
    : Trainer(topology, ind)
  {}
  Xor(size_t		ind = 100)
    : Trainer({2, 4, 1}, ind)
  {}
  ~Xor(void)
  {}
};

#endif	//		XOR_HPP
