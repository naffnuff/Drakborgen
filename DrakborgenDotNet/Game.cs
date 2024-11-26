using Drakborgen;
using SFML.Graphics;
using SFML.System;
using SFML.Window;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Numerics;
using System.Reflection.Metadata.Ecma335;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using static SFML.Window.Mouse;

namespace Drakborgen
{
    internal class Game
    {
        private delegate void EventCallback();

        private record Player(Hero Hero, Board.MoveSite BoardSite, int Life = 0, int AvatarIndex = 0)
        {
            internal Board.MoveSite BoardSite { get; set; } = BoardSite;
            internal int AvatarIndex { get; set; } = AvatarIndex;
        }

        internal Deck<Tile> Tiles { get; }

        private Network _network;

        private Random _random;
        private RenderWindow _window;
        private Board _board;

        private CardDisplay _cardDisplay;

        private State _state = State.NoState;

        private int _playerCount = 4;

        private bool _xCenteredBoard = false;
        private bool _yCenteredBoard = false;

        private bool _leftMouseButtonDown = false;
        private Vector2f _buttonPressedBoardPosition;
        private Vector2f _buttonPressedMousePosition;
        private Vector2f _buttonReleasedMousePosition;
        private int _capturedItemIndex = 0;
        private bool _preGameSetup = true;

        private List<Hero> _idleHeroes;
        private List<Player> _players;

        private List<Button> _buttons;

        private int _activePlayerIndex = -1;

        private EventCallback[] OnBeginTable {  get; set; }
        private EventCallback[] OnTickTable { get; set; }
        private EventCallback[] OnLeftMouseClickTable { get; set; }
        private EventCallback[] OnEndTable { get; set; }

        private AnimationManager _animations;

        internal Game(Network network)
        {
            _network = network;
            _random = new Random();
            //_window = new RenderWindow(new VideoMode(1024, 768), "Drakborgen");
            _window = new RenderWindow(VideoMode.DesktopMode, "Drakborgen");
            _animations = new AnimationManager();
            _board = new Board(_animations);
            _cardDisplay = new CardDisplay(_window);
            _idleHeroes = new List<Hero>(4);
            _players = new List<Player>();
            _buttons = new List<Button>();

            Tiles = new Deck<Tile>(115, _random);

            _window.SetVerticalSyncEnabled(true);

            Vector2f boardSize = _board.Size;
            Vector2u windowSize = _window.Size;
            _xCenteredBoard = boardSize.X < windowSize.X;
            _yCenteredBoard = boardSize.Y < windowSize.X;
            Vector2f boardPosition = new Vector2f();
            if (_xCenteredBoard)
            {
                boardPosition.X = (float)windowSize.X / 2 - boardSize.X / 2.0f;
            }
            if (_yCenteredBoard)
            {
                boardPosition.Y = (float)windowSize.Y / 2 - boardSize.Y / 2.0f;
            }
            _board.Position = boardPosition;

            _window.Closed += OnWindowClosed;
            _window.MouseButtonPressed += OnMouseButtonPressed;
            _window.MouseButtonReleased += OnMouseButtonReleased;
            _window.MouseMoved += OnMouseMoved;

            CreateStateLogicMap();
        }

        private void OnWindowClosed(object? sender, EventArgs e)
        {
            Console.WriteLine("OnWindowClosed");
            _window.Close();
        }

        private void OnMouseButtonPressed(object? sender, MouseButtonEventArgs e)
        {
            Console.WriteLine("OnMouseButtonPressed");
            if (e.Button == Mouse.Button.Left)
            {
                _leftMouseButtonDown = true;
                _buttonPressedMousePosition.X = (float)e.X;
                _buttonPressedMousePosition.Y = (float)e.Y;
                _buttonPressedBoardPosition = _board.Position;
                _capturedItemIndex = GetMouseOverItemIndex(_buttonPressedMousePosition);
            }
        }

        private void OnMouseButtonReleased(object? sender, MouseButtonEventArgs e)
        {
            Console.WriteLine("OnMouseButtonReleased");
            if (e.Button == Mouse.Button.Left && _leftMouseButtonDown)
            {
                _leftMouseButtonDown = false;
                _buttonReleasedMousePosition.X = (float)e.X;
                _buttonReleasedMousePosition.Y = (float)e.Y;
                int mouseOverItemIndex = GetMouseOverItemIndex(_buttonReleasedMousePosition);
                if (mouseOverItemIndex == _capturedItemIndex)
                {
                    InvokeEventHandler(OnLeftMouseClickTable);
                }
            }
        }

        private void OnMouseMoved(object? sender, MouseMoveEventArgs e)
        {
            if (_leftMouseButtonDown && _capturedItemIndex == -1)
            {
                if (!_xCenteredBoard || !_yCenteredBoard)
                {
                    _animations.Remove(_board);
                    Vector2f mouseMovementSincePressed = new Vector2f((float)(e.X - _buttonPressedMousePosition.X), (float)(e.Y - _buttonPressedMousePosition.Y));
                    Vector2f newBoardPosition = _buttonPressedBoardPosition + mouseMovementSincePressed;
                    Vector2f correctedBoardPosition = CorrectBoardPosition(newBoardPosition);
                    _board.Position = correctedBoardPosition;
                    if (newBoardPosition.X != correctedBoardPosition.X)
                    {
                        _buttonPressedMousePosition.X = (float)e.X;
                        _buttonPressedBoardPosition.X = correctedBoardPosition.X;
                    }
                    if (newBoardPosition.Y != correctedBoardPosition.Y)
                    {
                        _buttonPressedMousePosition.Y = (float)e.Y;
                        _buttonPressedBoardPosition.Y = correctedBoardPosition.Y;
                    }
                }
            }
        }

        internal void Run()
        {
            Clock clock = new Clock();
            float lastTimestamp = 0.0f;
            //tiles.shuffle();
            //bool toogle = false;
            /*for (int row = 0; row < _board.rowCount; ++row)
	{
		for (int column = 0; column < _board.columnCount; ++column)
		{
			if (toogle && !tiles.isEmpty() && !_board.hasTile({ row, column }))
			{
				_board.placeTile(tiles.pullNextItem(), { row, column });
			}
			toogle = !toogle;
		}
	}*/

            SetState(State.SelectNetRole);

            int fpsCount = 0;
            Clock fpsTimer = new Clock();

            while (_window.IsOpen)
            {
                // update the fps text every second
                if (fpsTimer.ElapsedTime.AsSeconds() >= 1)
                {
                    //std.cout << fpsCount << " fps" << std.endl;
                    fpsTimer.Restart();
                    fpsCount = 0;
                }
                ++fpsCount;

                Time time = clock.ElapsedTime;
                float timestamp = time.AsSeconds();
                float timeDelta = timestamp - lastTimestamp;
                lastTimestamp = timestamp;

                if (!_network.IsConnected)
                {
                    //SetState(State.AwaitingConnection);
                }

                InvokeEventHandler(OnTickTable);

                _animations.Update(timestamp, timeDelta);

                _board.Update(timestamp, timeDelta);

                //ProcessSystemEvents();
                _window.DispatchEvents();

                _window.Clear();

                _window.Draw(_board);
                _window.Draw(_cardDisplay);
                if (_preGameSetup)
                {
                    foreach (Hero hero in _idleHeroes)
                    {
                        if (hero.StatsCard != null)
                        {
                            _window.Draw(hero.StatsCard);
                        }
                    }
                }
                if (_activePlayerIndex >= 0 && _players[_activePlayerIndex].Hero.StatsCard != null)
                {
                    _window.Draw(_players[_activePlayerIndex].Hero.StatsCard);
                }
                foreach (Button button in _buttons)
                {
                    _window.Draw(button);
                }

                _window.Display();
            }
        }

        private void InvokeEventHandler(EventCallback[] eventTable)
        {
            EventCallback callback = eventTable[(int)_state];
            callback();
        }

        private void SetState(State newState)
        {
            if (newState != _state)
            {
                InvokeEventHandler(OnEndTable);
                _state = newState;
                InvokeEventHandler(OnBeginTable);
            }
        }

        private Vector2f CorrectBoardPosition(Vector2f boardPostion)
        {
            if (_xCenteredBoard)
            {
                boardPostion.X = (float)(_window.Size.X / 2) - _board.Size.X / 2.0f;
            }
            else if (boardPostion.X > 0.0f)
            {
                boardPostion.X = 0.0f;
            }
            else if (boardPostion.X + _board.Size.X < _window.Size.X)
            {
                boardPostion.X = _window.Size.X - _board.Size.X;
            }
            if (_xCenteredBoard)
            {
                boardPostion.Y = (float)(_window.Size.Y / 2) - _board.Size.Y / 2.0f;
            }
            else if (boardPostion.Y > 0.0f)
            {
                boardPostion.Y = 0.0f;
            }
            else if (boardPostion.Y + _board.Size.Y < _window.Size.Y)
            {
                boardPostion.Y = _window.Size.Y - _board.Size.Y;
            }
            return boardPostion;
        }

        private void OnBegin_SelectNetRole()
        {
            Vector2f buttonSize = new Vector2f(600.0f, 160.0f);
            {
                Vector2f buttonPosition = new Vector2f(_window.Size.X / 2.0f - buttonSize.X / 2.0f, _window.Size.Y * 1.0f / 4.0f - buttonSize.Y / 2.0f);
                _buttons.Add(new Button("Snåla", buttonSize, buttonPosition, 60));
            }
            {
                Vector2f buttonPosition = new Vector2f(_window.Size.X / 2.0f - buttonSize.X / 2.0f, _window.Size.Y * 2.0f / 4.0f - buttonSize.Y / 2.0f);
                _buttons.Add(new Button("Håll gästabud", buttonSize, buttonPosition, 60));
            }
            {
                Vector2f buttonPosition = new Vector2f(_window.Size.X / 2.0f - buttonSize.X / 2.0f, _window.Size.Y * 3.0f / 4.0f - buttonSize.Y / 2.0f);
                _buttons.Add(new Button("Snylta", buttonSize, buttonPosition, 60));
            }
        }

        private void OnLeftMouseClick_SelectNetRole()
        {
            if (_capturedItemIndex == 0)
            {
                SetState(State.SetupGame);
            }
            else if (_capturedItemIndex == 1)
            {
                SetState(State.SetupServer);
            }
            else if (_capturedItemIndex == 2)
            {
                SetState(State.SetupClient);
            }
        }

        private void OnEnd_SelectNetRole()
        {
            _buttons.Clear();
        }

        // Server setup

        private void OnBegin_SetupServer()
        {
            Vector2f buttonSize = new Vector2f(600.0f, 160.0f);
            {
                Vector2f buttonPosition = new Vector2f(_window.Size.X / 2.0f - buttonSize.X / 2.0f, _window.Size.Y * 1.0f / 4.0f - buttonSize.Y / 2.0f);
                _buttons.Add(new Button("Bjud in en gäst", buttonSize, buttonPosition, 60));
            }
            {
                Vector2f buttonPosition = new Vector2f(_window.Size.X / 2.0f - buttonSize.X / 2.0f, _window.Size.Y * 2.0f / 4.0f - buttonSize.Y / 2.0f);
                _buttons.Add(new Button("Bjud in två gäster", buttonSize, buttonPosition, 60));
            }
            {
                Vector2f buttonPosition = new Vector2f(_window.Size.X / 2.0f - buttonSize.X / 2.0f, _window.Size.Y * 3.0f / 4.0f - buttonSize.Y / 2.0f);
                _buttons.Add(new Button("Bjud in tre gäster", buttonSize, buttonPosition, 60));
            }
        }

        private void OnEnd_SetupServer()
        {
            _buttons.Clear();
        }

        private void OnLeftMouseClick_SetupServer()
        {
            if (_capturedItemIndex > -1)
            {
                _network.StartServer(_capturedItemIndex + 1, _random);
                SetState(State.NoState);
            }
        }

        // Client setup

        private void OnBegin_SetupClient()
        {
            {
                // Needs to be at index 0
                Vector2f buttonSize = new Vector2f(600.0f, 120.0f);
                Vector2f buttonPosition = new Vector2f(_window.Size.X / 2.0f - buttonSize.X / 2.0f, _window.Size.Y * 5.0f / 6.0f - buttonSize.Y / 2.0f);
                _buttons.Add(new Button("Låt färden gå!", buttonSize, buttonPosition, 60));
            }
            {
                // Needs to be at index 1
                Vector2f buttonSize = new Vector2f(600.0f, 120.0f);
                Vector2f buttonPosition = new Vector2f(_window.Size.X / 2.0f - buttonSize.X / 2.0f, _window.Size.Y * 2.0f / 5.0f - buttonSize.Y / 2.0f);
                _buttons.Add(new Button("Rasmus", buttonSize, buttonPosition, 60));
            }
            {
                // Needs to be at index 2
                Vector2f buttonSize = new Vector2f(600.0f, 120.0f);
                Vector2f buttonPosition = new Vector2f(_window.Size.X / 2.0f - buttonSize.X / 2.0f, _window.Size.Y * 4.0f / 5.0f - buttonSize.Y / 2.0f);
                _buttons.Add(new Button("192.168.1.121", buttonSize, buttonPosition, 60));
            }
            {
                Vector2f buttonSize = new Vector2f(600.0f, 120.0f);
                Vector2f buttonPosition = new Vector2f(_window.Size.X / 2.0f - buttonSize.X / 2.0f, _window.Size.Y * 1.0f / 8.0f - buttonSize.Y / 2.0f);
                _buttons.Add(new Button("Om vem skall legenden vittna?", buttonSize, buttonPosition, 60));
            }
            {
                Vector2f buttonSize = new Vector2f(600.0f, 120.0f);
                Vector2f buttonPosition = new Vector2f(_window.Size.X / 2.0f - buttonSize.X / 2.0f, _window.Size.Y * 2.0f / 8.0f - buttonSize.Y / 2.0f);
                _buttons.Add(new Button("Varthän går färden?", buttonSize, buttonPosition, 60));
            }
        }

        private void OnLeftMouseClick_SetupClient()
        {
            if (_capturedItemIndex == 0)
            {
                _network.StartClient(/*_buttons[1]->getText(), */_buttons[2].ButtonText);
                SetState(State.NoState);
            }
        }

        private void OnEnd_SetupClient()
        {
            _buttons.Clear();
        }

        private void OnBegin_AwaitingConnection()
        {
            Vector2f buttonSize = new Vector2f(600.0f, 160.0f);
            Vector2f buttonPosition = new Vector2f(_window.Size.X / 2.0f - buttonSize.X / 2.0f, _window.Size.Y * 2.0f / 4.0f - buttonSize.Y / 2.0f);
            _buttons.Add(new Button("Sällskapet samlas...", buttonSize, buttonPosition, 60));
        }

        private void OnTick_AwaitingConnection()
        {
            if (_network.IsConnected)
            {
                SetState(State.NoState);
            }
        }

        private void OnEnd_AwaitingConnection()
        {
            _buttons.Clear();
        }

        private void OnBegin_SetupGame()
        {
            if (_idleHeroes.Count == 0)
            {
                _idleHeroes.Capacity = 4;
                _idleHeroes.Add(new Hero("rohan", "Riddar Rohan", 17));
                _idleHeroes.Add(new Hero("sigeir", "Sigeir Skarpyxe", 16));
                _idleHeroes.Add(new Hero("aelfric", "Aelfric Brunkåpa", 15));
                _idleHeroes.Add(new Hero("bardhor", "Bardhor Bågman", 11));
            }

            _board.SetGameStartMoveSites();

            DisplayCards(GetHeroCards(), () => { });

            SetState(State.PickHero);
        }

        private void OnLeftMouseClick_PickHero()
        {
            if (_capturedItemIndex >= 0)
            {
                _buttons.Clear();
                if (_capturedItemIndex < _cardDisplay.CardCount) // hero card clicked
                {
                    SetState(State.PickStartTower);
                }
                else // begin-game button clicked
                {
                    for (int i = (int)_idleHeroes.Count - 1; i >= 0; --i)
                    {
                        Card card = _cardDisplay.PullCard(i);
                        Action callback = () => { };
                        if (i == _idleHeroes.Count - 1)
                        {
                            callback = () => { _preGameSetup = false; };
                        }
                        MoveOffScreen(card, (float)(i + 1) / (float)(_idleHeroes.Count), callback);
                        _idleHeroes[i].PlaceStatsCard(card);
                    }
                    StartNewGame();
                    SetState(State.PlayerMove);
                }
            }
        }

        private void OnBegin_PickStartTower()
        {
            PanToNextFreeTower();
            CreatePlayer(_capturedItemIndex);
            _board.ShowMoveSites(true);
        }

        private void OnLeftMouseClick_PickStartTower()
        {
            Vector2f mouseBoardPosition = GetMouseBoardPosition();
            if (_board.TestMoveSites(mouseBoardPosition))
            {
                _board.ShowMoveSites(false);
                Board.Site site = _board.GetSite(mouseBoardPosition);
                _board.RemoveMoveSite(site);
                PlaceNewPlayer(site, () => { OnNewPlayerPlaced(); });
                if (_idleHeroes.Count == 0)
                {
                    SetState(State.PlayerMove);
                }
                else
                {
                    SetState(State.PickHero);
                }
            }
        }

        private void OnLeftMouseClick_PlayerMove()
        {
            if (_capturedItemIndex >= _cardDisplay.CardCount)
            {
                SetState(State.ViewStatsCard);
            }
            else
            {
                Vector2f mouseBoardPosition = GetMouseBoardPosition();
                if (_board.TestMoveSites(mouseBoardPosition))
                {
                    Board.MoveSite moveSite = _board.GetMoveSite(mouseBoardPosition);
                    if (!_board.HasTile(moveSite.Site))
                    {
                        Tile tile = Tiles.PullNextItem();
                        _board.PlaceTile(tile, moveSite);
                    }
                    {
                        Tile tile = _board.GetTile(moveSite.Site);
                        MovePlayer(_activePlayerIndex, moveSite, () => { OnPlayerMoved(tile); });
                    }
                    _board.ClearMoveSites();
                    // setting new _state async
                }
            }
        }

        private void OnBegin_ViewStatsCard()
        {
            //_board.showMoveSites(false);
            Card card = _players[_activePlayerIndex].Hero.PullStatsCard();
            if (card == null)
            {
                throw new Exception();
            }
            DisplayCard(card, () => { });
        }

        private void OnLeftMouseClick_ViewStatsCard()
        {
            if (_capturedItemIndex > -1)
            {
                SetState(State.PlayerMove);
            }
        }

        private void OnEnd_ViewStatsCard()
        {
            Card card = _cardDisplay.PullCard();
            MoveOffScreen(card, 0.5f, () => { PlaceAtOrigin(_players[_activePlayerIndex].Hero.StatsCard); });
            _players[_activePlayerIndex].Hero.PlaceStatsCard(card);
            //_board.showMoveSites(true);
        }

        private void OnBegin_TurnContinue()
        {
            StartPlayerRound();
            SetState(State.PlayerMove);
        }

        private void OnBegin_TurnEnd()
        {
            _activePlayerIndex = (_activePlayerIndex + 1) % (int)_players.Count;
            OnBegin_TurnContinue();
        }

        // Helpers

        private void PlaceAtOrigin(Card? card)
        {
            if (card == null)
            {
                throw new Exception();
            }
            Vector2u windowSize = _window.Size;
            Vector2f cardSize = card.GetGlobalBounds().Size;
            card.Position = new Vector2f(-cardSize.X, (float)windowSize.Y);
        }

        private void MoveOffScreen(Card card, float time, Action callback)
        {
            Vector2u windowSize = _window.Size;
            Vector2f cardSize = card.GetGlobalBounds().Size;
            _animations.Add(card, new Vector2f((float)windowSize.X, -cardSize.Y), time, callback);
        }

        private void MoveToCenter(Card card, Action callback)
        {
            Vector2f cardSize = card.GetGlobalBounds().Size;
            Vector2u windowSize = _window.Size;
            Vector2f animationTarget = new Vector2f(windowSize.X / 2.0f - cardSize.X / 2.0f, windowSize.Y / 2.0f - cardSize.Y / 2.0f);
            _animations.Add(card, animationTarget, 0.5f, callback);
        }

        private Vector2f GetMouseBoardPosition()
        {
            Vector2f boardPosition = _board.Position;
            Vector2f mouseBoardPosition = new Vector2f(_buttonReleasedMousePosition.X - boardPosition.X, _buttonReleasedMousePosition.Y - boardPosition.Y);
            return mouseBoardPosition;
        }

        private int GetMouseOverItemIndex(Vector2f mousePosition)
        {
            int index = _cardDisplay.HitTest(mousePosition);
            if (index == -1)
            {
                for (int i = 0; i < _buttons.Count; ++i)
                {
                    if (_buttons[i].HitTest(mousePosition))
                    {
                        index = _cardDisplay.CardCount + i;
                        break;
                    }
                }
            }
            return index;
        }

        private List<Card> GetHeroCards()
        {
            List<Card> heroCards = new List<Card>();
            foreach (Hero hero in _idleHeroes)
            {
                heroCards.Add(hero.PullStatsCard());
            }
            return heroCards;
        }

        private void DisplayCard(Card card, Action callback)
        {
            //placeAtOrigin(card);
            MoveToCenter(card, callback);
            _cardDisplay.PushCard(card);
        }

        private void DisplayCards(List<Card> cards, Action callback)
        {
            Vector2f[] layout = _cardDisplay.GetLayout(cards);
            for (int i = 0; i < cards.Count; ++i)
            {
                PlaceAtOrigin(cards[i]);
                _animations.Add(cards[i], layout[i], (float)(i + 1) / (float)(cards.Count), i == cards.Count - 1 ? callback : () => { });
                _cardDisplay.PushCard(cards[i]);
            }
        }

        private void PanToNextFreeTower()
        {
            Vector2f boardTarget = new Vector2f(0.0f, 0.0f);
            Vector2u windowSize = _window.Size;
            if (_players.Count == 0)
            {
                if (_random.NextBool())
                {
                    boardTarget.Y = -(float)windowSize.Y;
                }
                if (_random.NextBool())
                {
                    boardTarget.X = -(float)windowSize.X;
                }
            }
            else
            {
                Board.Site lastSite = _players.Last().BoardSite.Site;
                int oppositeRow = lastSite.Row == 0 ? Board.RowCount - 1 : 0;
                int oppositeColumn = lastSite.Column == 0 ? Board.ColumnCount - 1 : 0;
                List<Board.Site> candidates = [
                    new Board.Site(oppositeRow, oppositeColumn),
                    new Board.Site(lastSite.Row, oppositeColumn),
                    new Board.Site(oppositeRow, lastSite.Column)
                ];
                //_random.shuffle(candidates);
                foreach (Board.Site candidate in candidates)
                {
                    bool goodCandidate = true;
                    for (int i = 0; i < (int)_players.Count - 1; ++i)
                    {
                        if (_players[i].BoardSite.Site == candidate)
                        {
                            goodCandidate = false;
                            break;
                        }
                    }
                    if (goodCandidate)
                    {
                        if (candidate.Row > 0)
                        {
                            boardTarget.Y = -(float)windowSize.Y;
                        }
                        if (candidate.Column > 0)
                        {
                            boardTarget.X = -(float)windowSize.X;
                        }
                        break;
                    }
                }
            }
            _animations.Add(_board, CorrectBoardPosition(boardTarget), 0.75f, () => { });
        }

        private void CreatePlayer(int heroIndex)
        {
            Card pickedCard = _cardDisplay.PullCard(heroIndex);
            MoveToCenter(pickedCard, () => { });
            _players.Add(new Player(_idleHeroes[heroIndex], Board.InvalidMoveSite, _idleHeroes[heroIndex].MaxLife));
            ++_activePlayerIndex;
            _idleHeroes.RemoveAt(heroIndex);
            for (int i = (int)(_idleHeroes.Count) - 1; i >= 0; --i)
            {
                Card card = _cardDisplay.PullCard(i);
                MoveOffScreen(card, (float)(i + 1) / (float)_idleHeroes.Count, () => { });
                _idleHeroes[i].PlaceStatsCard(card);
            }
            _cardDisplay.PushCard(pickedCard);
        }

        private void PlaceNewPlayer(Board.Site site, Action animationCallback)
        {
            if (_players.Count == 0)
            {
                throw new Exception();
            }
            int index = (int)_players.Count - 1;
            Player player = _players[index];
            player.BoardSite = new Board.MoveSite(site);
            player.AvatarIndex = index;
            if (_cardDisplay.CardCount != 1)
            {
                throw new Exception();
            }
            Card card = _cardDisplay.PullCard(0);
            Action cardAnimationCallback = () =>
            {
                _board.AddPlayer(Setup.MediaPath + "hjaltekort/" + player.Hero.Id + "gubbe.png", player.AvatarIndex);
                _board.SetPlayerSite(player.AvatarIndex, new Board.MoveSite(site), animationCallback);
                PlaceAtOrigin(player.Hero.StatsCard);
            };
            Vector2f target = new Vector2f();
            Vector2f cardSize = card.GetGlobalBounds().Size;
            Vector2u windowSize = _window.Size;
            if (site.Column == 0)
            {
                target.X = -cardSize.X;
            }
            else if (site.Column == Board.ColumnCount - 1)
            {
                target.X = (float)windowSize.X;
            }
            if (site.Row == 0)
            {
                target.Y = -cardSize.Y;
            }
            else if (site.Row == Board.RowCount - 1)
            {
                target.Y = (float)windowSize.Y;
            }
            _animations.Add(card, target, 0.5f, cardAnimationCallback);
            player.Hero.PlaceStatsCard(card);
        }

        private void MovePlayer(int index, Board.MoveSite moveSite, Action animationCallback)
        {
            Player player = _players[index];
            player.BoardSite = moveSite;
            _board.SetPlayerSite(player.AvatarIndex, moveSite, animationCallback);
        }

        private void OnNewPlayerPlaced()
        {
            if (_idleHeroes.Count == 0)
            {
                StartNewGame();
            }
            else
            {
                Vector2f buttonSize = new Vector2f(500.0f, 200.0f);
                Vector2f buttonPosition = new Vector2f(_window.Size.X * 3.0f / 4.0f - buttonSize.X / 2.0f, _window.Size.Y * 3.0f / 4.0f - buttonSize.Y / 2.0f);
                _buttons.Add(new Button("Låt äventyret börja!", buttonSize, buttonPosition, 60));
                DisplayCards(GetHeroCards(), () => { });
            }
        }

        private void OnPlayerMoved(Tile tile)
        {
            SetState(tile.Enter());
        }

        private void StartNewGame()
        {
            _board.ClearMoveSites();
            //_random.shuffle(_players);
            _activePlayerIndex = 0;
            StartPlayerRound();
        }

        private void StartPlayerRound()
        {
            Player player = _players[_activePlayerIndex];
            Vector2f size = new Vector2f(270.0f, 100.0f);
            Vector2f position = new Vector2f(0.0f, 370.0f);
            string heroInfo = player.Hero.Name + "\n" + "Kroppspoäng: " + player.Life + " / " + player.Hero.MaxLife;
            _buttons.Add(new Button(heroInfo, size, position, 30));
            _board.SetPlayerMoveSites(player.BoardSite);
            _board.ShowMoveSites(true);
            Vector2u windowSize = _window.Size;
            Vector2f avatarCenter = _board.GetAvatarCenter(player.AvatarIndex);
            _animations.Add(_board, CorrectBoardPosition(new Vector2f(windowSize.X / 2.0f - avatarCenter.X, windowSize.Y / 2.0f - avatarCenter.Y)), 0.75f, () => { });
        }

        // Setup

        private class StateHandlerInitializer
        {
            internal StateHandlerInitializer(Game game, State state)
            {
                if (state == State.StateCount)
                {
                    int size = (int)State.StateCount;
                    game.OnBeginTable = new EventCallback[size];
                    game.OnTickTable = new EventCallback[size];
                    game.OnLeftMouseClickTable = new EventCallback[size];
                    game.OnEndTable = new EventCallback[size];
                }
                else
                {
                    _next = new StateHandlerInitializer(game, state + 1);

                    game.CreateStateHandler("OnBegin", state);
                    game.CreateStateHandler("OnTick", state);
                    game.CreateStateHandler("OnLeftMouseClick", state);
                    game.CreateStateHandler("OnEnd", state);
                }
            }

            private StateHandlerInitializer? _next;
        }

        private void CreateStateHandler(string eventName, State state)
        {
            Type? type = GetType();
            string fieldName = eventName + "Table";
            PropertyInfo? property = type.GetProperty(fieldName, BindingFlags.NonPublic | BindingFlags.Instance);
            EventCallback[]? table = property?.GetValue(this) as EventCallback[] ?? throw new Exception(fieldName + " not found");

            string methodName = eventName + "_" + Enum.GetName(state);
            if (type.GetMethod(methodName, BindingFlags.NonPublic | BindingFlags.Instance) is MethodInfo method)
            {
                Console.WriteLine("Found " + methodName);
                table[(int)state] = (EventCallback)Delegate.CreateDelegate(typeof(EventCallback), this, method);
            }
            else
            {
                Console.Error.WriteLine("Did not find " + methodName);
                table[(int)state] = () => { };
            }
        }

        private void CreateStateLogicMap()
        {
            new StateHandlerInitializer(this, State.NoState);
        }
    }
}
