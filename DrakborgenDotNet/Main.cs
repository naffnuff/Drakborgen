namespace Drakborgen
{
    internal class Program
    {
        static void Main(string[] args)
        {
	try
            {
                using (Network network = new Network())
                {
                    Game game = new Game(network);
                    Setup.SetUpGame(game);
                    game.Run();
                }
            }
	catch (Exception e)
            {
                Console.Error.WriteLine(e);
                throw;
	}
        }
    }
}
