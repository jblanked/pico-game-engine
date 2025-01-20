import gc
from time import sleep
from .game import Game


class GameEngine:
    """
    Represents a game engine.

    Parameters:
    - name: str - the name of the game engine
    - fps: int - the frames per second
    - game: Game - the game to run
    """

    def __init__(self, name: str, fps: int, game: Game):
        self.name = name
        self.fps = fps
        self.game = game

    def run(self):
        """Run the game engine"""

        # start the game
        self.game.start(self)

        # start the game loop
        while True:
            gc.collect()
            self.game.update()  # update positions, input, etc.
            self.game.render()  # update graphics

            # check if the game is over
            if not self.game.is_running:
                break

            sleep(1 / self.fps)

        # stop the game
        self.game.stop(self)

        # clear the screen
        self.game.draw.clear()
        gc.collect()
