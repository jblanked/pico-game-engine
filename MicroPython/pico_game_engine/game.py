import gc
from .level import Level
from .input import Input, Button
from .vector import Vector
from .draw import Draw
from .ILI9341 import ILI9341, color565


class Game:
    """
    Represents a game.

    Parameters:
    - name: str - the name of the game
    - start: function(Game) - the function called when the game is created
    - stop: function(Game) - the function called when the game is destroyed
    - display: ILI9341 - the display to render the game on
    - foreground_color: int - the color of the foreground
    - background_color: int - the color of the background
    """

    def __init__(
        self,
        name: str,
        start=None,
        stop=None,
        display: ILI9341 = None,
        foreground_color=color565(255, 255, 255),
        background_color=color565(0, 0, 0),
    ):
        self.name = name
        self._start = start
        self._stop = stop
        self.levels = []  # List of levels in the game
        self.current_level: Level = None  # holds the current level
        self.button_up: Input = None
        self.button_down: Input = None
        self.button_left: Input = None
        self.button_right: Input = None
        self.button_center: Input = None
        self.button_back: Input = None
        self.button_start: Input = None
        self.input: int = -1  # last button pressed
        self.draw = Draw(display)
        self.camera = Vector(0, 0)
        self.pos = Vector(0, 0)
        self.size = Vector(display.width, display.height)
        self.world_size = Vector(display.width, display.height)
        self.is_active = False
        self.foreground_color = foreground_color
        self.background_color = background_color

    def clamp(self, value, lower, upper):
        """Clamp a value between a lower and upper bound."""
        return min(max(value, lower), upper)

    @property
    def is_running(self) -> bool:
        """Return the running state of the game"""
        return self.is_active

    @is_running.setter
    def is_running(self, value: bool):
        """Set the running state of the game"""
        self.is_active = value

    def input_add(self, control: Input):
        """Add an input control to the game"""
        if control.button == Button.UP:
            self.button_up = control
        elif control.button == Button.DOWN:
            self.button_down = control
        elif control.button == Button.LEFT:
            self.button_left = control
        elif control.button == Button.RIGHT:
            self.button_right = control
        elif control.button == Button.CENTER:
            self.button_center = control
        elif control.button == Button.BACK:
            self.button_back = control
        elif control.button == Button.START:
            self.button_start = control

        gc.collect()

    def input_remove(self, control: Input):
        """Remove an input control"""
        if control.button == Button.UP:
            self.button_up = None
        elif control.button == Button.DOWN:
            self.button_down = None
        elif control.button == Button.LEFT:
            self.button_left = None
        elif control.button == Button.RIGHT:
            self.button_right = None
        elif control.button == Button.CENTER:
            self.button_center = None
        elif control.button == Button.BACK:
            self.button_back = None
        elif control.button == Button.START:
            self.button_start = None

        gc.collect()

    def level_add(self, level: Level):
        """Add a level to the game"""
        self.levels.append(level)
        gc.collect()

    def level_remove(self, level: Level):
        """Remove a level from the game"""
        self.levels.remove(level)
        gc.collect()

    def level_switch(self, level: Level):
        """Switch to a new level"""
        if not level:
            print("Level is not valid.")
            return
        old_level = self.current_level
        self.current_level = level
        old_level.stop()
        old_level.clear()
        self.current_level.start()
        gc.collect()

    def manage_input(self):
        """Check for input from the user"""
        if self.button_up and self.button_up.is_pressed():
            self.input = Button.UP
        elif self.button_down and self.button_down.is_pressed():
            self.input = Button.DOWN
        elif self.button_left and self.button_left.is_pressed():
            self.input = Button.LEFT
        elif self.button_right and self.button_right.is_pressed():
            self.input = Button.RIGHT
        elif self.button_center and self.button_center.is_pressed():
            self.input = Button.CENTER
        elif self.button_back and self.button_back.is_pressed():
            self.input = Button.BACK
        elif self.button_start and self.button_start.is_pressed():
            self.input = Button.START
        else:
            self.input = -1

        gc.collect()

    def render(self):
        """Render the game entities in a thread-safe manner."""
        # Draw each entity.
        for entity in self.current_level.entities:
            if entity.pos_did_change and entity.old_pos is not entity.pos:
                # delete old pos
                draw_x = entity.old_pos.x - self.camera.x
                draw_y = entity.old_pos.y - self.camera.y

                self.draw.clear(
                    Vector(draw_x, draw_y), entity.size, self.background_color
                )

                entity.old_pos = entity.pos
            # If you need to run any custom rendering function:
            if entity.render:
                entity.render(self.draw, self)

            # Compute on-screen coordinates (using camera offset).
            draw_x = entity.pos.x - self.camera.x
            draw_y = entity.pos.y - self.camera.y

            # Draw the entity's sprite.
            if entity.sprite:
                self.draw.image(entity.sprite, draw_x, draw_y)

    def start(self, engine) -> bool:
        """Start the game"""
        if not self.levels:
            print("The game has no levels.")
            return False
        self.current_level = self.levels[0]
        gc.collect()
        # Clear the screen initially.
        self.draw.clear(color=self.background_color)
        if self._start:
            self._start(self, engine)
        self.current_level.start(engine)
        self.is_active = True
        return True

    def stop(self, engine):
        """Stop the game"""
        gc.collect()

        if not self.is_active:
            return

        if self._stop:
            self._stop(self, engine)

        self.is_active = False

        for level in self.levels:
            if level:
                level.clear()
                level = None
        self.levels = []

        # Clear and release input controls.
        self.button_up = None
        self.button_down = None
        self.button_left = None
        self.button_right = None
        self.button_center = None
        self.button_back = None
        self.button_start = None

        self.draw.clear(color=self.background_color)
        gc.collect()

    def update(self):
        """Update the game input and entity positions in a thread-safe manner."""
        self.manage_input()

        # Run user-defined update functions for each entity.
        for entity in self.current_level.entities:
            if entity.update:
                entity.update(self)

        # Calculate camera offset to center the player.
        self.camera.x = self.pos.x - (self.size.x // 2)
        self.camera.y = self.pos.y - (self.size.y // 2)

        # Clamp camera position to prevent going outside the world.
        self.camera.x = self.clamp(self.camera.x, 0, self.world_size.x - self.size.x)
        self.camera.y = self.clamp(self.camera.y, 0, self.world_size.y - self.size.y)
