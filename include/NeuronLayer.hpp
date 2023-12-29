
#ifndef				NEURON_LAYER_HPP
# define			NEURON_LAYER_HPP
# include			"Neuron.hpp"

template <typename		MathValue = std::complex<double>, int MaxAbsValue = 50>
class				NeuronLayer
{
  std::vector<
    Neuron<MathValue, MaxAbsValue>
    >				neurons;

public:
  uint64_t			Hash(void) const
  {
    uint64_t			hash = 5381;
    
    for (size_t i = 0; i < neurons.size(); ++i)
      hash = ((hash << 5) + hash) + neurons[i].Hash();
    return (hash);
  }
  void				PrintGene(std::ostream	&os,
					  bool		binary = false)
  {
    if (binary)
      throw std::exception();
    os << " Layer :" << std::endl;
    for (size_t i = 0; i < neurons.size(); ++i)
      neurons[i].PrintGene(os, binary);
  }
  void				Mutate(void)
  {
    neurons[rand() % neurons.size()].Mutate();
  }
  void				Set(size_t		neuron,
				    MathValue		val)
  {
    if (neurons.size() <= neuron)
      throw std::exception();
    neurons[neuron].Set(val);
  }
  MathValue			Get(size_t		neuron)
  {
    if (neurons.size() <= neuron)
      throw std::exception();
    return (neurons[neuron].Compute());
  }
  void				Reset(void)
  {
    for (size_t i = 0; i < neurons.size(); ++i)
      neurons[i].Reset();
  }

  void				Combine(NeuronLayer<MathValue, MaxAbsValue> const &a,
					NeuronLayer<MathValue, MaxAbsValue> const &b)
  {
    if (neurons.size() != a.neurons.size() || neurons.size() != b.neurons.size())
      throw std::exception();
    size_t			i;

    for (i = 0; i < neurons.size(); ++i)
      neurons[i].Combine(a.neurons[i], b.neurons[i]);
  }

  int				Load(t_bunny_configuration *cnf,
				     int		line,
				     int		col)
  {
    int				ret;

    ret = col;
    for (size_t i = 0; i < neurons.size(); ++i)
      ret = neurons[i].Load(cnf, line, ret);
    return (ret);
  }

  int				Save(t_bunny_configuration *cnf,
				     int		y,
				     int		x) const
  {
    int				ret;

    ret = x;
    for (size_t i = 0; i < neurons.size(); ++i)
      ret = neurons[i].Save(cnf, y, ret);
    return (ret);
  }
  
  NeuronLayer(size_t		nbr_neuron)
  {
    neurons.resize(nbr_neuron);
  }
  NeuronLayer(NeuronLayer	&prev,
	      size_t		nbr_neuron)
  {
    neurons.reserve(nbr_neuron);
    for (size_t i = 0; i < nbr_neuron; ++i)
      {
	neurons.emplace_back(prev.neurons.size());
	for (size_t j = 0; j < prev.neurons.size(); ++j)
	  neurons[i].inputs[j].neuron = &prev.neurons[j];
      }
  }
  ~NeuronLayer(void) {}
  friend class NeuronNetwork<MathValue, MaxAbsValue>;
};

#endif	//			NEURON_LAYER_HPP
