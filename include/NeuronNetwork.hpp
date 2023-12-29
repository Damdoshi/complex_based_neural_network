
#ifndef			NEURON_NETWORK_HPP
# define		NEURON_NETWORK_HPP
# include		"NeuronLayer.hpp"
# include		"IdGenerator.hpp"

template <typename	MathValue = std::complex<double>, int MaxAbsValue = 50>
class			NeuronNetwork
{
  IdGenerator		id_generator;
  std::vector<
    NeuronLayer<MathValue, MaxAbsValue>
    >			layers;
  size_t		id;

  void			Build(std::vector<int> const &nn)
  {
    id = id_generator;
    if (nn.size() < 2)
      throw std::exception();
    layers.reserve(nn.size());
    layers.emplace_back(nn[0]);
    for (size_t i = 1; i < nn.size(); ++i)
      layers.emplace_back(layers[i - 1], nn[i]);
  }
  
public:
  size_t		GetId(void) const
  {
    return (id);
  }
  uint64_t		Hash(void) const
  {
    uint64_t		hash = 5381;
    
    for (size_t i = 0; i < layers.size(); ++i)
      hash = ((hash << 5) + hash) + layers[i].Hash();
    return (hash);
  }
  void			PrintGene(std::ostream	&os,
				  bool		binary = false)
  {
    if (binary)
      throw std::exception();
    os << "Network --" << std::endl;
    for (size_t i = 0; i < layers.size(); ++i)
      layers[i].PrintGene(os, binary);
  }
  
  void			Set(size_t		neuron,
			    MathValue		val)
  {
    if (layers.size() < 2)
      throw std::exception();
    NeuronLayer<MathValue, MaxAbsValue> &nl = layers[0];
    
    if (nl.neurons.size() <= neuron)
      throw std::exception();
    nl.Set(neuron, val);
  }
  MathValue		Get(size_t		neuron)
  {
    if (layers.size() < 2)
      throw std::exception();
    NeuronLayer<MathValue, MaxAbsValue> &nl = layers[layers.size() - 1];
    
    if (nl.neurons.size() <= neuron)
      throw std::exception();
    return (nl.Get(neuron));
  }
  void			Reset(void)
  {
    for (size_t i = 0; i < layers.size(); ++i)
      layers[i].Reset();
  }

  void			Mutate(void)
  {
    layers[rand() % layers.size()].Mutate();
  }

  void			Combine(NeuronNetwork<MathValue, MaxAbsValue> const &a,
				NeuronNetwork<MathValue, MaxAbsValue> const &b)
  {
    if (a.layers.size() != b.layers.size() || layers.size() != a.layers.size())
      throw std::exception();
    size_t		i;

    for (i = 0; i < a.layers.size(); ++i)
      layers[i].Combine(a.layers[i], b.layers[i]);
  }

  void			Save(t_bunny_configuration			*cnf,
			     int					y) const
  {
    if (!bunny_configuration_setf_int(cnf, (int)layers.size(), "[%d][0]", y))
      throw std::exception();
    for (size_t i = 0; i < layers.size(); ++i)
      if (!bunny_configuration_setf_int(cnf, (int)layers[i].neurons.size(), "[%d][%d]", y, i + 1))
	throw std::exception();
    int			ret;

    ret = layers.size() + 1;
    for (size_t i = 0; i < layers.size(); ++i)
      ret = layers[i].Save(cnf, y, ret);
  }

  NeuronNetwork(std::vector<int> const	&nn)
  {
    Build(nn);
  }
  NeuronNetwork(t_bunny_configuration	*cnf,
		int			y)
  {
    std::vector<int> v;
    int l;
    int i;

    if (!bunny_configuration_getf_int(cnf, &l, "[%d][0]", y))
      throw std::exception();
    for (i = 0; i < l; ++i)
      {
	int d;
	
	if (!bunny_configuration_getf_int(cnf, &d, "[%d][%d]", y, i + 1))
	  throw std::exception();
	v.push_back(d);
      }
    Build(v);
    int ret = l + 1;

    for (size_t i = 0; i < layers.size(); ++i)
      ret = layers[i].Load(cnf, y, ret);
  }
  ~NeuronNetwork()
  {}
};

#endif	//		NEURON_NETWORK_HPP
