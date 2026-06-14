
#ifndef			NEURON_NETWORK_HPP
# define		NEURON_NETWORK_HPP
# include		"NeuronLayer.hpp"
# include		"IdGenerator.hpp"
# include		<stdexcept>

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
    if (layers.size() <= 1)
      throw std::exception();
    layers[rand() % (layers.size() - 1) + 1].Mutate();
  }

  void			BackpropInitialize(void)
  {
    for (size_t l = 1; l < layers.size(); ++l)
      for (size_t i = 0; i < layers[l].neurons.size(); ++i)
	layers[l].neurons[i].BackpropInitialize();
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

  double		Backpropagate(std::vector<MathValue> const &input,
			      std::vector<MathValue> const &target,
			      double learning_rate)
  {
    if (layers.size() < 2)
      throw std::exception();
    if (layers[0].neurons.size() != input.size())
      throw std::exception();
    if (layers.rbegin()->neurons.size() != target.size())
      throw std::exception();
    Reset();
    for (size_t i = 0; i < input.size(); ++i)
      layers[0].neurons[i].Set(input[i]);

    NeuronLayer<MathValue, MaxAbsValue> &out = layers[layers.size() - 1];
    double error = 0;

    for (size_t i = 0; i < out.neurons.size(); ++i)
      {
	MathValue output = out.neurons[i].Compute();
	MathValue diff = output - target[i];

	error += SquaredMagnitude(diff);
	out.neurons[i].SetOutputDelta(target[i]);
      }

    for (int l = (int)layers.size() - 2; l > 0; --l)
      {
	NeuronLayer<MathValue, MaxAbsValue> &current = layers[l];
	NeuronLayer<MathValue, MaxAbsValue> &next = layers[l + 1];

	for (size_t i = 0; i < current.neurons.size(); ++i)
	  {
	    MathValue sum = Zero<MathValue>();

	    for (size_t j = 0; j < next.neurons.size(); ++j)
	      for (size_t k = 0; k < next.neurons[j].inputs.size(); ++k)
		if (next.neurons[j].inputs[k].neuron == &current.neurons[i])
		  sum += NeuralMultiply(next.neurons[j].inputs[k].coefficient, next.neurons[j].delta_value);
	    current.neurons[i].SetHiddenDelta(sum);
	  }
      }

    for (size_t l = 1; l < layers.size(); ++l)
      for (size_t i = 0; i < layers[l].neurons.size(); ++i)
	layers[l].neurons[i].ApplyBackprop(learning_rate);
    return (error / target.size());
  }

  double		Backpropagate(std::vector<std::pair<std::vector<MathValue>,
							std::vector<MathValue>>> const &samples,
			      size_t epochs,
			      double learning_rate,
			      double target_error = 0.0)
  {
    double error = 0;

    if (samples.empty())
      throw std::exception();
    for (size_t epoch = 0; epoch < epochs; ++epoch)
      {
	error = 0;
	for (size_t i = 0; i < samples.size(); ++i)
	  error += Backpropagate(samples[i].first, samples[i].second, learning_rate);
	error /= samples.size();
	if (target_error > 0.0 && error <= target_error)
	  break ;
      }
    return (error);
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
