"""
Pico Game Engine
Author: JBlanked
Github: https://github.com/jblanked/Pico-Game-Engine
Info: A simple game engine for the Raspberry Pi Pico, TFT display, HW504 joystick, and buttons.
Created: 2025-01-18
Updated: 2025-01-19
"""

# This is just an example of how to use the Pico Game Engine

# import all the modules
from pico_game_engine.game import Game
from pico_game_engine.engine import GameEngine
from pico_game_engine.entity import Entity
from pico_game_engine.level import Level
from pico_game_engine.input import Input, Button, HW504, Orientation
from pico_game_engine.vector import Vector
from pico_game_engine.ILI9341 import ILI9341, color565
from pico_game_engine.image import Image
from pico_game_engine.draw import Draw

# Wiring (TFT -> Pico):
# GNDD -> GND
# VCC -> VSYS (Pin 39)
# CS -> GP9 (Pin 12)
# RESET -> GP10 (Pin 14)
# DC -> GP11 (Pin 15)
# SDI/MOSI -> GP6 (Pin 9)
# SCK -> GP7 (Pin 10)
# LED -> VSYS (Pin 39)

# Wiring (HW504 -> Pico):
# SW -> GP17 (Pin 22 - TX)
# VRx -> GP27 (Pin 32 - ADC1)
# VRy -> GP26 (Pin 31 - ADC0)
# GND -> GND
# 5V -> VSYS (Pin 39)

# Button mapping (used as D-pad for now since we have no A, B, X, Y buttons)
# Controller -> Pico
# Yellow (UP) -> GP16 (Pin 21 - RX)
# Red (RIGHT) -> GP17 (Pin 22 - TX)
# Green (DOWN) -> GP18 (Pin 25)
# Blue (LEFT) -> GP19 (Pin 26)


class PicoGameEngine:
    """
    Pico Game Engine
    """

    def __init__(self):
        # create a new TFT display
        self.screen = ILI9341(
            miso_pin=4,  # not used
            clk_pin=6,
            mosi_pin=7,
            cs_pin=9,
            dc_pin=11,
            rst_pin=10,
            width=320,
            height=240,
            rotation=3,  # 90 degrees to the right
        )

        # create a new game
        self.game: Game = None

        # create a new joystick
        self.joystick = HW504(
            x_pin=27,
            y_pin=26,
            button_pin=17,
            orientation=Orientation.LEFT,  # 90 degrees to the left
        )

        # set the current level to None
        self.current_level: Level = None

        # set the engine to None
        self.engine = None

    def add_input(self):
        """Add input controls"""
        self.game.input_add(Input(16, Button.UP, None))
        self.game.input_add(Input(17, Button.RIGHT, None))
        self.game.input_add(Input(18, Button.DOWN, None))
        self.game.input_add(Input(19, Button.LEFT, None))

    def add_level(self):
        """Add a new level"""
        self.current_level = Level(name="Level 1", size=Vector(320, 240))
        self.game.level_add(self.current_level)

    def add_player(self):
        """Add a new player entity"""
        player_image = Image()
        red_square_byte_array = bytearray(b'\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\xff\xff\x00\x00\xff\xff\x00\x00\x00\x00\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\x00\x00\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\xff\xff\x00\x00\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\x00\x00\x00\x00\xff\xff\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\xff\xff\x00\x00\x00\x00\x00\x00\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00')
                    
        player_image.from_byte_array(red_square_byte_array, 15, 11)

        player_entity = Entity(
            name="Player",
            sprite=player_image,
            position=Vector(160, 120),
            update=self.player_update,
            render=self.player_render,
        )

        self.current_level.entity_add(player_entity)

    def game_input(self, player: Entity, game: Game):
        """Move the player entity based on input"""
        if game.input == Button.UP:
            player.position += Vector(0, -5)
        elif game.input == Button.RIGHT:
            player.position += Vector(5, 0)
        elif game.input == Button.DOWN:
            player.position += Vector(0, 5)
        elif game.input == Button.LEFT:
            player.position += Vector(-5, 0)

    def game_start(self, game: Game, engine: GameEngine):
        """Handle your game start logic here"""

    def game_stop(self, game: Game, engine: GameEngine):
        """Handle your game stop logic here"""

    def player_update(self, player: Entity, game: Game):
        """Update the player entity"""

        # move the player entity based on input
        self.game_input(player, game)

    def player_render(self, player: Entity, draw: Draw, game: Game):
        """Render the player entity"""

        # draw name of the game
        draw.text(game.name, 110, 10)

    def run(self):
        """Run the game engine"""

        # create a new game
        self.game = Game(
            name="Pico-Game",
            start=self.game_start,
            stop=self.game_stop,
            display=self.screen,
            foreground_color=color565(0, 0, 0),
            background_color=color565(255, 255, 255)
        )
        
        self.game.draw.color(self.game.foreground_color, self.game.background_color)

        # add controls, level, and player
        self.add_input()
        self.add_level()
        self.add_player()

        # create a new game engine
        self.engine = GameEngine(
            name="Pico-Game-Engine",
            fps=30,
            game=self.game,
        )

        # run the game engine
        try:
            self.engine.run()
        except KeyboardInterrupt:
            self.game.is_running = False
        finally:
            self.game.stop(self.engine)
            self.screen.display.erase()


# run the main function
if __name__ == "__main__":
    pico_game_engine = PicoGameEngine()
    pico_game_engine.run()

