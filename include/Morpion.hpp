
#ifndef			MORPION_HPP
# define		MORPION_HPP
# include		<array>
# include		<vector>

typedef std::array<std::array<char, 3>, 3> Map;

void			Clear(Map				&map,
			      char				c = 0)
{
  for (size_t i = 0; i < map.size(); ++i)
    map[i].fill(c);
}

class			Morpion : public Trainer<std::complex<double>, 1>
{
  using MathValue = std::complex<double>;
  
  struct		Situation
  {
    Map			map; // Map of +1, 0 and -1.
    Map			score; // Map of score if position is taken
  };
  int			player_token;
  std::vector<
    Situation
    >			situations;
  
public:
  bool			CheckWin(Map				map,
				 t_bunny_position		pos)
  {
    int			tok = map[pos.y][pos.x];
    int			cnt;
    int			i;

    cnt = 0;
    for (i = 0; i < 3; ++i)
      if (map[pos.y][i] == tok)
	cnt += 1;
    if (cnt == 3)
      return (true);

    cnt = 0;
    for (i = 0; i < 3; ++i)
      if (map[i][pos.x] == tok)
	cnt += 1;
    if (cnt == 3)
      return (true);

    if (pos.x == 1 && pos.y == 1)
      {
	if (map[0][0] == tok && map[2][2] == tok)
	  return (true);
	if (map[2][0] == tok && map[0][2] == tok)
	  return (true);
      }
    return (false);
  }

private:
  int			RobotScan(Map				map,
				  t_bunny_position		*pos,
				  int				ptoken)
  {
    std::map<int, std::vector<t_bunny_position>> scores;
    int			i;
    int			j;
    int			k;

    k = 0;
    for (j = 0; j < 3; ++j)
      for (i = 0; i < 3; ++i)
	{
	  if (map[j][i] != 0)
	    continue ;
	  Map		nmap = map;
	  bool		vict;

	  nmap[j][i] = ptoken;
	  scores[RobotScan(nmap, pos, ~ptoken) + 1].push_back({i, j});
	  if ((vict = CheckWin(map, {i, j})) && ptoken == player_token)
	    return (-1);
	  else if (vict)
	    return (+10);
	  k += 1;
	}
    if (k == 0)
      return (0);
    auto		it = scores.rbegin();

    *pos = it->second[rand() % it->second.size()];
    return (1);
  }
  
  t_bunny_position	RobotPlay(Map				map)
  {
    t_bunny_position	fin;

    RobotScan(map, &fin, player_token);
    return (fin);
  }

public:
  t_bunny_position	Play(Map				map,
			     NeuronNetwork<MathValue, 1>	&nn)
  {
    Map			nmap = map;
    int			i;
    int			j;

    nn.Reset();
    for (j = 0; j < 3; ++j)
      for (i = 0; i < 3; ++i)
	{
	  double	val;

	  if ((map[j][i] == player_token))
	    val = 1;
	  else if (map[j][i] == 0)
	    val = 0;
	  else
	    val = -1;
	  nn.Set(i + j * 3, val);
	}

    int			x = nn.Get(0).real();
    int			y = nn.Get(1).imag();

    // Normalement, x et y sont par construction dans [-1; +1]
    // Mais je vérifie tout de même
    if (x < -1 || x > 1 || y < -1 || y > 1 || nmap[y + 1][x + 1] != 0)
      return (t_bunny_position{-1, -1});
    return (t_bunny_position{x + 1, y + 1});
  }

  double		Test(NeuronNetwork<MathValue, 1>	&val)
  {
    double		score = 0;
    bool		cheat = false;
    size_t		i;
    size_t		j;
    
    for (i = 0; i < situations.size(); ++i)
      {
	t_bunny_position pos = Play(situations[i].map, val);

	if (pos.x < 0)
	  {
	    score -= 5;
	    cheat = true;
	  }
	else
	  score += situations[i].score[pos.y][pos.x];
      }
    
    if (cheat == false)
      {
	// Match contre le min max
	for (j = 0; j < 100; ++j)
	  {
	    Map		map;

	    Clear(map);
	    std::cout << "========================== NEW GAME ===========================" << std::endl << std::endl;
	    for (i = 0; i < 10; ++i)
	      {
		t_bunny_position pos;
		bool		turn;
		
		if ((turn = ((j % 2 + i) % 2)))
		  {
		    pos = Play(map, val);
		    if (pos.x < 0)
		      {
			j = INT_MAX;
			break ;
		      }
		    map[pos.y][pos.x] = player_token;
		  }
		else
		  {
		    pos = RobotPlay(map);
		    map[pos.y][pos.x] = 'O';
		  }
		DisplayMap(map, std::cout);
		bool	v = CheckWin(map, pos);

		if (!v)
		  {
		    if (turn)
		      score += 1;
		    continue ;
		  }
		if (map[pos.y][pos.x] == player_token)
		  score += 10; // On a gagné!
		else if (map[pos.y][pos.x] != 0)
		  score += 0; // On a perdu
	      }
	  }
      }
    return (score);
  }

  t_bunny_position	last_move;
  
public:
  Map			operator()(Map				map,
				   bool				minmax = false)
  {
    t_bunny_position	pos;
    
    if (minmax)
      pos = RobotPlay(map);
    else
      {
	auto		&best = GetBest();

	pos = Play(map, GetBest());
      }
    last_move = pos;
    if (pos.x == -1)
      Clear(map, -2);
    else
      map[pos.y][pos.x] = player_token;
    return (map);
  }

  t_bunny_position	GetLastMove(void) const
  {
    return (last_move);
  }
  
  void			DisplayMap(Map				map,
				   std::ostream			&os)
  {
    int			i, j;

    os << "=====" << std::endl;
    for (j = 0; j < 3; ++j)
      {
	for (i = 0; i < 3; ++i)
	  {
	    if (map[j][i])
	      os << map[j][i];
	    else
	      os << " ";
	    if (i + 1 != 3)
	      os << "|";
	  }
	os << std::endl;
	if (j + 1 < 3)
	  os << "-+-+-" << std::endl;
      }
    os << "=====" << std::endl << std::endl;
  }

  void			LoadSituations(const std::string	&str)
  {
    t_bunny_configuration *cnf;
    Situation		sit;
    int			i;
    int			j;

    if ((cnf = bunny_open_configuration(str.c_str(), NULL)) == NULL)
      throw std::exception();
    /*
    j = 0;
    do
      {
	for (i = 0; bunny_configuration_getf_int(cnf, &sit[i / 3][i % 3], "[0][%d]", i); ++i);    
	for (i = 0; bunny_configuration_getf_int(cnf, &sit[i / 3][i % 3], "[1][%d]", i / 3, i % 3); ++i);
    */
  }

  Morpion(const std::string &file)
    : Trainer(file), player_token('X')
  {}
  Morpion(size_t	ind = 100)
    : Trainer({9, 16, 8, 4, 2}, ind), player_token('X')
  {}
  ~Morpion(void)
  {}
};

#endif	//		MORPION_HPP
