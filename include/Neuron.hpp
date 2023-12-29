
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

  bool			ready;
  MathValue		reverse_output;
  MathValue		toggle_value;

  void			AllRandom(void)
  {
    reverse_output = Random<MathValue>(1);
    for (size_t i = 0; i < inputs.size(); ++i)
      inputs[i].coefficient = Random<MathValue>(MaxAbsValue);
    toggle_value = Random<MathValue>(MaxAbsValue * inputs.size());
    inner_value =  Random<MathValue>(MaxAbsValue * inputs.size());
    output_value = Round(inner_value);
  }
  
public:
  uint64_t		Hash(void) const
  {
    uint64_t		hash = 5381;

    hash = ((hash << 5) + hash) + ::Hash(reverse_output);
    hash = ((hash << 5) + hash) + ::Hash(inner_value);
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
    os << "\033[31m";
    os << "   Rev " << Format(reverse_output) << " ";
    os << "\033[32m";
    os << "   Tog " << Format(toggle_value) << " ";
    os << "\033[33m";
    os << "   Out " << Format(output_value) << " ";
    os << "\033[36m";
    os << "   Inn " << Format(output_value) << " ";
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
    int			rnd = rand() % (inputs.size() + 2);

    if (rnd == 0)
      {
	reverse_output = Random<MathValue>(1);
	return ;
      }
    if (rnd == 1)
      {
	toggle_value = Random<MathValue>(MaxAbsValue * inputs.size());
	return ;
      }
    inputs[rnd - 2].coefficient = Random<MathValue>(MaxAbsValue);
  }
  
  MathValue		Compute(void)
  {
    if (ready)
      return (output_value);
    inner_value = Zero<MathValue>();
    for (size_t i = 0; i < inputs.size(); ++i)
      inner_value += inputs[i].coefficient * inputs[i].neuron->Compute();

    // Si c'est une entrée
    if (toggle_value == Zero<MathValue>())
      return (inner_value);
    // Sinon c'est un intermédiaire
    output_value = ::Round(inner_value / toggle_value);
    output_value = ::Reverse(output_value, reverse_output, MaxAbsValue);
    ready = true;
    return (output_value);
  }
  MathValue		Set(MathValue		val)
  {
    ready = true;
    return (output_value = inner_value = val);
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
    
    reverse_output = ::Combine<MathValue>(a.reverse_output, b.reverse_output);
    toggle_value = ::Combine<MathValue>(a.toggle_value, b.toggle_value);
    for (i = 0; i < inputs.size(); ++i)
      inputs[i].coefficient = ::Combine<MathValue>(a.inputs[i].coefficient, b.inputs[i].coefficient);
  }
  
  int			Load(t_bunny_configuration	*cnf,
			     int			line,
			     int			col)
  {
    col += Retrieve<MathValue>(cnf, line, col, reverse_output);
    col += Retrieve<MathValue>(cnf, line, col, toggle_value);
    for (size_t i = 0; i < inputs.size(); ++i)
      col += Retrieve<MathValue>(cnf, line, col, inputs[i].coefficient);
    return (col);
  }

  int			Save(t_bunny_configuration	*cnf,
			     int			line,
			     int			col) const
  {
    col += Export<MathValue>(cnf, line, col, reverse_output);
    col += Export<MathValue>(cnf, line, col, toggle_value);
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
