from .vector import Vector
from .entity import Entity


class Level:
    """
    Represents a level in the game.

    Parameters:
    - name: str - the name of the level
    - size: Vector - the size of the level
    - start: function(Level, GameEngine) - the function called when the level is created
    - stop: function(Level, GameEngine) - the function called when the level is destroyed
    """

    def __init__(
        self,
        name: str,
        size: Vector,
        start=None,  # start is a function that is called when the level is created
        stop=None,  # stop is a function that is called when the level is destroyed
    ):
        self.name = name
        self.size = size
        self.entities = []  # List of entities in the level
        self._start = start
        self._stop = stop

    def clear(self):
        """Clear the level"""
        self.entities = []

    def collision_list(self, entity: Entity) -> list:
        """Return a list of entities that the entity collided with"""
        collided = []
        for other in self.entities:
            if entity != other and self.is_collision(entity, other):
                collided.append(other)
        return collided

    def entity_add(self, entity: Entity):
        """Add an entity to the level"""
        self.entities.append(entity)

    def entity_remove(self, entity: Entity):
        """Remove an entity from the level"""
        self.entities.remove(entity)

    def has_collided(self, entity: Entity) -> bool:
        """Check for collisions with other entities"""
        for other in self.entities:
            if entity != other and self.is_collision(entity, other):
                return True
        return False

    def is_collision(self, entity: Entity, other: Entity) -> bool:
        """Check if two entities collided using AABB logic"""
        entity_pos = entity.position
        other_pos = other.position
        entity_size = entity.size
        other_size = other.size
        return (
            entity_pos.x < other_pos.x + other_size.x
            and entity_pos.x + entity_size.x > other_pos.x
            and entity_pos.y < other_pos.y + other_size.y
            and entity_pos.y + entity_size.y > other_pos.y
        )

    def start(self, engine):
        """Start the level"""
        if self._start:
            self._start(self, engine)

    def stop(self, engine):
        """Stop the level"""
        if self._stop:
            self._stop(self, engine)
