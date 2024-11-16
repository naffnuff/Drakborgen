namespace Drakborgen
{
    internal class Program
    {
        static void Main(string[] args)
        {
	try
            {
                Game game = new Game();
                Setup.SetUpGame(game);
                game.Run();
            }
	catch (Exception e)
            {
                Console.Error.WriteLine(e);
                throw;
	}
        }
    }
}
