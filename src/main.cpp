
#include		"Xor.hpp"
#include		"Morpion.hpp"
#include		"Empty.hpp"
#include		<cstdlib>
#include		<cstring>
#include		<ctime>

int			main(int		argc,
			     char		**argv)
{
  struct timespec	ts;

  clock_gettime(CLOCK_MONOTONIC, &ts);
  srand(ts.tv_sec * 1e9 + ts.tv_nsec);
  //srand(1);

  if (argc < 2)
    {
      puts("./bin\n"
	   "\txor [file]?\n"
	   "\txor-bp [--load file] [epochs=10000] [learning_rate=1.0] [output=xor_bp.csv]?\n"
	   "\toxo [file]?\n"
	   "\n"
	   );
      return (EXIT_FAILURE);
    }
  if (!strcmp(argv[1], "xor"))
    {
      Xor		*xorg;

      if (argc == 3)
	xorg = new Xor(std::string(argv[2]));
      else
	{
	  xorg = new Xor(5000);
	  int		cnt;

	  for (cnt = 0; xorg->GetBestScore() < 8 && cnt < 1000; ++cnt)
	    xorg->Generate(0.8, 0.1);
	  xorg->PrintScore(std::cerr);
	  if (xorg->GetBestScore() == 8)
	    {
	      std::cerr
		<< "Generation "
		<< cnt
		<< " have given birth to the Great One!"
		<< std::endl
		;
	      std::cerr << "Its hash is " << xorg->GetBest().Hash() << "." << std::endl;
	      
	    }
	      xorg->Save("xor.csv");
	}
      Xor	&xorgate = *xorg;
  
      std::cerr << "[X, N]" << std::endl;
      std::cerr << xorgate(0, 0) << std::endl;
      std::cerr << xorgate(0, 1) << std::endl;
      std::cerr << xorgate(1, 0) << std::endl;
      std::cerr << xorgate(1, 1) << std::endl;
    }

  if (!strcmp(argv[1], "xor-bp"))
    {
      Xor		*xorg;
      size_t		epochs = 10000;
      double		learning_rate = 1.0;
      std::string	output_file = "xor_bp.csv";
      bool		initialize = true;

      int argi = 2;

      if (argc >= 4 && !strcmp(argv[2], "--load"))
	{
	  xorg = new Xor(std::string(argv[3]));
	  initialize = false;
	  argi = 4;
	}
      else
	xorg = new Xor({2, 8, 1}, 16);
      if (argc > argi)
	epochs = strtoul(argv[argi], NULL, 10);
      if (argc > argi + 1)
	learning_rate = atof(argv[argi + 1]);
      if (argc > argi + 2)
	output_file = argv[argi + 2];

      double error = xorg->Backpropagate(epochs, learning_rate, 0.001, initialize);

      std::cerr << "Final mean squared error is " << error << "." << std::endl;
      std::cerr << "[X, NX]" << std::endl;
      std::cerr << xorg->BackpropResult(0, 0) << std::endl;
      std::cerr << xorg->BackpropResult(0, 1) << std::endl;
      std::cerr << xorg->BackpropResult(1, 0) << std::endl;
      std::cerr << xorg->BackpropResult(1, 1) << std::endl;
      xorg->SaveBest(output_file);
      delete xorg;
    }

  if (!strcmp(argv[1], "oxo"))
    {
      Morpion		*oxo;

      if (argc == 3)
	oxo = new Morpion(std::string(argv[2]));
      else
	{
	  oxo = new Morpion(5000);
	  int		cnt;

	  for (cnt = 0; cnt < 1000; ++cnt)
	    {
	      oxo->Generate(0.8, 0.1);
	      std::cerr << "Best score is " << oxo->GetBestScore() << "." << std::endl;
	    }
	  oxo->Save("oxo.csv");
	}
      Map		map;
      t_bunny_position	pos;
      int		start = rand() % 2;
      char		c = 'X';

      puts("You are X.");
      Clear(map);
      do
	{
	  if (start % 2)
	    {
	      do
		puts("Type in position (Format is 'x,y' in range [0-2]): ");
	      while (scanf("%d,%d", pos.x, pos.y) != 2 || pos.x < 0 || pos.x > 2 || pos.y < 0 || pos.y > 2);
	      map[pos.y][pos.x] = c;
	    }
	  else
	    {
	      bunny_usleep(1e6 / 4);
	      puts("Thinking...");
	      bunny_usleep(1e6 / 4);
	      (*oxo)(map, true);
	    }
	  oxo->DisplayMap(map, std::cout);
	  start += 1;
	}
      while (oxo->CheckWin(map, oxo->GetLastMove()) == false);
    }
  
  /*
  Integer		integers(15);
  int			cnt;

  puts("With no training");
  printf("Best is %f.\n", integers.Evaluate());
  // integers.PrintHash(std::cout);
  integers.PrintScore(std::cout);
  puts("Start training");
  for (cnt = 0; cnt < 100; ++cnt)
    {
      integers.Generate(0.5, 0.1);
      // integers.PrintHash(std::cout);
      printf("Best of generation %d is is %f with %d.\n", cnt, integers.Evaluate(), integers.GetBest().Hash());
      integers.PrintScore(std::cout);
    }
  */

  /*
  Empty			empty(50000);
  int			cnt;

  for (cnt = 0; cnt < 100; ++cnt)
    {
      empty.Generate(0.8, 0.4);
      std::cout << "Generation " << cnt << "achieved" << std::endl;
    }
  empty.PrintScore(std::cout);
  empty.GetBest().PrintGene(std::cout);
  */
  return (0);
}

