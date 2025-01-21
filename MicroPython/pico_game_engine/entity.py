from .vector import Vector
from .image import Image


class Entity:
    """
    Represents an entity in the game.

    Parameters:
    - name: str - the name of the entity
    - sprite: Image - the image representing the entity
    - position: Vector - the position of the entity
    - start: function(Entity, Game) - the function called when the entity is created
    - stop: function(Entity, Game) - the function called when the entity is destroyed
    - update: function(Entity, Game) - the function called every frame
    - render: function(Entity, Draw, Game) - the function called every frame to render the entity
    - collision: function(Entity, Entity, Game) - the function called when the entity collides with another entity
    - is_player: bool - whether the entity is the player
    """

    def __init__(
        self,
        name: str,  # name is a string that represents the name of the entity
        sprite: Image,  # path is a string that represents the path to the image or the image data
        position: Vector,  # position is a Vector that represents the x and y coordinates of the entity
        start=None,  # start is a function that is called when the entity is created
        stop=None,  # stop is a function that is called when the entity is destroyed
        update=None,  # update is a function that is called every frame
        render=None,  # render is a function that is called every frame
        collision=None,  # collision is a function that is called when the entity collides with another entity
        is_player: bool = False,  # is_player is a boolean that specifies whether the entity is the player
    ):
        self.name = name
        self.pos = Vector(position.x, position.y)
        self.old_pos = Vector(0, 0)
        self._start = start
        self._stop = stop
        self._update = update
        self._render = render
        self._collision = collision
        self.is_player = is_player
        self.sprite = sprite
        self.pos_did_change = False

        # Set the size of the entity based on the image size
        self.size = self.sprite.size if self.sprite else Vector(0, 0)

    def collision(self, other, game):
        """Called when the entity collides with another entity."""
        if self._collision:
            self._collision(self, other, game)

    @property
    def position(self) -> Vector:
        """Used by the engine to get the position of the entity."""
        return Vector(self.pos.x, self.pos.y)

    @position.setter
    def position(self, value: Vector):
        """Used by the engine to set the position of the entity."""
        self.old_pos = Vector(self.pos.x, self.pos.y)
        self.pos = Vector(value.x, value.y)
        self.pos_did_change = True

    def render(self, draw, game):
        """Called every frame to render the entity."""
        if self._render:
            self._render(self, draw, game)

    def start(self, game):
        """Called when the entity is created."""
        if self._start:
            self._start(self, game)

    def stop(self, game):
        """Called when the entity is destroyed."""
        if self._stop:
            self._stop(self, game)

    def update(self, game):
        """Called every frame."""
        if self._update:
            self._update(self, game)
