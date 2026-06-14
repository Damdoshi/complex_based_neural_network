
#ifndef			NEURON_HPP
# define		NEURON_HPP
# include		<iostream>
# include		<vector>
# include		<complex>
# include		<sys/types.h>
# include		<limits.h>
# include		<float.h>
# include		"Number.hpp"

template <typename MathValue, int MaxAbsValue>
class			NeuronLayer;
template <typename MathValue, int MaxAbsValue>
class			NeuronNetwork;

template <typename MathValue = std::complex<double>, int MaxAbsValue = 50>
class			Neuron
{
  struct		Input
  {
    MathValue		coefficient;
    Neuron		*neuron;

    Input(void) : neuron(NULL) {}
    ~Input(void) {}
  };

  std::vector<Input>	inputs;
  MathValue		inner_value;
  MathValue		output_value;
  MathValue		delta_value;

  bool			ready;
  // Biais du neurone : valeur ajoutée à la somme pondérée avant
  // l'activation. Dans l'ancien modèle, toggle_value était un seuil
  // testé avec "sum > threshold". Le biais exprime la même idée sous
  // une forme plus pratique pour la rétropropagation :
  //   activation(sum - threshold) == activation(sum + bias)
  // donc bias == -threshold. Un biais positif rend le neurone plus
  // facile à activer, un biais négatif le rend plus difficile à activer.
  MathValue		bias;

  void			AllRandom(void)
  {
    for (size_t i = 0; i < inputs.size(); ++i)
      inputs[i].coefficient = Random<MathValue>(MaxAbsValue);
    bias = Random<MathValue>(MaxAbsValue * inputs.size());
    inner_value =  Random<MathValue>(MaxAbsValue * inputs.size());
    output_value = Activation<MathValue>(inner_value);
    delta_value = Zero<MathValue>();
  }

public:
  uint64_t		Hash(void) const
  {
    uint64_t		hash = 5381;

    hash = ((hash << 5) + hash) + ::Hash(bias);
    for (size_t i = 0; i < inputs.size(); ++i)
      hash = ((hash << 5) + hash) + ::Hash(inputs[i].coefficient);
    return (hash);
  }
  void			PrintGene(std::ostream	&os, bool binary = false)
  {
    if (binary)
      throw std::exception();
    os << "\033[35m";
    os << "  Neuron : ";
    os << "\033[32m";
    os << "   Bias " << Format(bias) << " ";
    os << "\033[33m";
    os << "   Out " << Format(output_value) << " ";
    os << "\033[36m";
    os << "   Inn " << Format(inner_value) << " ";
    os << "\033[34m";
    for (size_t i = 0; i < inputs.size(); ++i)
      {
	os << "   Inp " << Format(inputs[i].coefficient);
	if (i + 1 < inputs.size())
	  os << " ";
      }
    os << std::endl;
    os << "\033[00m";
  }

  void			Mutate(void)
  {
    int			rnd = rand() % (inputs.size() + 1);

    if (rnd == 0)
      {
	bias = Random<MathValue>(MaxAbsValue * inputs.size());
	return ;
      }
    inputs[rnd - 1].coefficient = Random<MathValue>(MaxAbsValue);
  }

  void			BackpropInitialize(void)
  {
    if (inputs.size() == 0)
      return ;
    double scale = 1.0 / sqrt((double)inputs.size());

    bias = Scale(Random<MathValue>(1), scale);
    for (size_t i = 0; i < inputs.size(); ++i)
      inputs[i].coefficient = Scale(Random<MathValue>(1), scale);
  }

  MathValue		Compute(void)
  {
    if (ready)
      return (output_value);
    if (inputs.size() == 0)
      return (output_value);
    inner_value = bias;
    for (size_t i = 0; i < inputs.size(); ++i)
      inner_value += NeuralMultiply(inputs[i].coefficient, inputs[i].neuron->Compute());
    output_value = Activation<MathValue>(inner_value);
    ready = true;
    return (output_value);
  }
  MathValue		Set(MathValue		val)
  {
    ready = true;
    return (output_value = inner_value = val);
  }

  void			SetOutputDelta(MathValue const &target)
  {
    delta_value = NeuralMultiply(output_value - target, ActivationDerivativeFromOutput(output_value));
  }

  void			SetHiddenDelta(MathValue const &backward_sum)
  {
    delta_value = NeuralMultiply(backward_sum, ActivationDerivativeFromOutput(output_value));
  }

  void			ApplyBackprop(double learning_rate)
  {
    if (inputs.size() == 0)
      return ;
    for (size_t i = 0; i < inputs.size(); ++i)
      inputs[i].coefficient -= Scale(NeuralMultiply(delta_value, inputs[i].neuron->output_value), learning_rate);
    bias -= Scale(delta_value, learning_rate);
  }
  void			Reset(void)
  {
    if (inputs.size())
      ready = false;
  }

  void			Combine(Neuron<MathValue, MaxAbsValue> const &a,
				Neuron<MathValue, MaxAbsValue> const &b)
  {
    if (inputs.size() != a.inputs.size() || inputs.size() != b.inputs.size())
      throw std::exception();
    if (inputs.size() == 0)
      return ;
    size_t		i;

    bias = ::Combine<MathValue>(a.bias, b.bias);
    for (i = 0; i < inputs.size(); ++i)
      inputs[i].coefficient = ::Combine<MathValue>(a.inputs[i].coefficient, b.inputs[i].coefficient);
  }

  int			Load(t_bunny_configuration	*cnf,
			     int			line,
			     int			col)
  {
    col += Retrieve<MathValue>(cnf, line, col, bias);
    for (size_t i = 0; i < inputs.size(); ++i)
      col += Retrieve<MathValue>(cnf, line, col, inputs[i].coefficient);
    return (col);
  }

  int			Save(t_bunny_configuration	*cnf,
			     int			line,
			     int			col) const
  {
    col += Export<MathValue>(cnf, line, col, bias);
    for (size_t i = 0; i < inputs.size(); ++i)
      col += Export<MathValue>(cnf, line, col, inputs[i].coefficient);
    return (col);
  }

  Neuron(size_t		ninputs = 0)
    : ready(false)
  {
    inputs.resize(ninputs);
    AllRandom();
  }
  ~Neuron(void) {}
  friend class NeuronLayer<MathValue, MaxAbsValue>;
  friend class NeuronNetwork<MathValue, MaxAbsValue>;
};

#endif	       //	NEURON_HPP
