
#ifndef				TRAINER_HPP
# define			TRAINER_HPP
# include			<algorithm>
# include			<iostream>
# include			<map>
# include			"NeuronNetwork.hpp"

template <typename		MathValue = std::complex<double>, int MaxAbsValue = 50>
class				Trainer
{
  std::vector<NeuronNetwork<MathValue, MaxAbsValue>>	nn;
  std::vector<
    std::pair<double, NeuronNetwork<MathValue, MaxAbsValue>*>
    >				nnp;

  virtual double		Test(NeuronNetwork<MathValue, MaxAbsValue> &val) = 0;

  static bool			SortByScore(const std::pair<double, NeuronNetwork<MathValue, MaxAbsValue>*> &a,
					    const std::pair<double, NeuronNetwork<MathValue, MaxAbsValue>*> &b)
  {
    return (a.first < b.first);
  }
 
public:
  void				PrintHash(std::ostream	&os)
  {
    std::map<uint64_t, NeuronNetwork<MathValue, MaxAbsValue>*> entity;

    for (size_t i = 0; i < nnp.size(); ++i)
      {
	uint64_t		hs;
	
	os << "Network with score " << nnp[i].first << " is " << (hs = nnp[i].second->Hash()) << std::endl;
	entity[hs] = nnp[i].second;
      }
    os << "Variation of hashes is " << entity.size() << "." << std::endl;
  }
  void				PrintGene(std::ostream	&os,
					  bool		binary = false)
  {
    if (binary)
      throw std::exception();
    for (size_t i = 0; i < nn.size(); ++i)
      nn[i].PrintGene(os, binary);
  }
  
  void				PrintScore(std::ostream	&os)
  {
    size_t			i;

    for (i = 0; i < nnp.size(); ++i)
      {
	char			buffer[128];

	snprintf(buffer, sizeof(buffer), "%+.0f", round(nnp[i].first));
	os << buffer;
	if (i + 1 < nnp.size())
	  os << " ";
      }
    os << std::endl;
  }
  
  double			Evaluate(void)
  {
    size_t			i;
    double			best;

    for (i = 0; i < nnp.size(); ++i)
      nnp[i].first = Test(*nnp[i].second);
    std::sort(nnp.begin(), nnp.end(), SortByScore);
    best = nnp.rbegin()->first;
    return (best);
  }

  void				Mutate(double		mrate)
  {
    int				mrt = nnp.size() * mrate;

    while (mrt > 0)
      {
	nnp[rand() % nnp.size()].second->Mutate();
	mrt -= 1;
      }
  }
  
  void				Generate(double		keepratio,
					 double		mutation_rate = 0)
  {
    size_t			last = round(nnp.size() * (1.0 - keepratio));
    size_t			i;

    Trainer<MathValue, MaxAbsValue>::Mutate(mutation_rate);
    
    Evaluate();
    if (keepratio < 0 || keepratio > 1.0)
      throw std::exception();
    for (i = 0; i < last; ++i)
      {
	size_t			a;
	size_t			b;

	do
	  {
	    a = nnp.size() - i - 1;
	    b = rand() % (nnp.size() - last) + last;
	  }
	while (a == b);
	nnp[i].second->Combine(*nnp[a].second, *nnp[b].second);
      }
  }

  NeuronNetwork<MathValue, MaxAbsValue>	&GetBest(void)
  {
    return (*nnp.rbegin()->second);
  }
  NeuronNetwork<MathValue, MaxAbsValue> const &GetBest(void) const
  {
    return (*nnp.rbegin()->second);
  }
  double				GetBestScore(void) const
  {
    return (nnp.rbegin()->first);
  }

  bool					Save(const std::string &file,
					     NeuronNetwork<MathValue, MaxAbsValue> const *bnn = NULL) const
  {
    t_bunny_configuration		*cnf;

    if ((cnf = bunny_new_configuration()) == NULL)
      return (false);
    if (bnn)
      bnn->Save(cnf, 0);
    else
      for (size_t i = 0; i < nn.size(); ++i)
	nn[i].Save(cnf, i);
    return (bunny_save_configuration(BC_CSV, file.c_str(), cnf));
  }

  Trainer(const std::string		&file)
  {
    t_bunny_configuration		*cnf;
    int i;
    int j;

    if (!(cnf = bunny_open_configuration(file.c_str(), NULL)))
      throw std::exception();
    for (i = 0; bunny_configuration_getf(cnf, NULL, "[%d]", i); ++i);
    nn.reserve(i);
    nnp.resize(i);
    for (j = 0; j < i; ++j)
      {
	nn.emplace_back(cnf, j);
	nnp[j].first = 0;
	nnp[j].second = &nn[j];
      }
  }
  
  Trainer(std::vector<int> const	&nnd,
	  size_t			ind = 50)
  {
    nn.reserve(ind);
    nnp.resize(ind);
    for (size_t i = 0; i < ind; ++i)
      {
	nn.emplace_back(nnd);
	nnp[i].first = 0;
	nnp[i].second = &nn[i];
      }
  }
  virtual ~Trainer(void) {}
};

#endif	//				TRAINER_HPP

