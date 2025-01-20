from machine import Pin, ADC
from .vector import Vector


class Button:
    """
    Button class to represent the different buttons on the joystick.
    """

    UP = 0
    DOWN = 1
    LEFT = 2
    RIGHT = 3
    CENTER = 4
    BACK = 5
    START = 6


class Orientation:
    """
    Orientation class to represent the different orientations of the joystick.
    """

    NONE = 0  # No orientation
    LEFT = 1  # Rotate 90 degrees left
    RIGHT = 2  # Rotate 90 degrees right
    UP = 3  # Rotate 180 degrees


class Input:
    """
    Input class to handle the button presses and joystick input.

    @param pin: The pin number of the button.
    @param button: The button to be checked.
    @param joystick: The joystick object to be used for input.
    """

    def __init__(self, pin: int, button: Button, joystick: HW504 = None):
        self.pin = Pin(pin, Pin.IN, Pin.PULL_UP)
        self.button = button
        self.elapsed_time = 0
        self.was_pressed = False
        self.joystick = joystick

    def is_pressed(self) -> bool:
        """
        Return True if the button is pressed.
        Since we are pulling up the button, we need to check for a LOW value.
        """
        if self.joystick:
            return self.joystick.value(self.button)
        return self.pin.value() == 0

    def is_held(self, duration: int) -> bool:
        """
        Return True if the button is held for a certain duration.
        """

        return self.elapsed_time >= duration

    def run(self):
        """
        Track the button state and the elapsed time it was pressed.
        This should be looped in the main loop.
        """
        if self.is_pressed():
            self.elapsed_time += 1
            self.was_pressed = True
        else:
            self.elapsed_time = 0
            self.was_pressed = False


class HW504:
    """
    HW504 Joystick class to handle the joystick input.

    Wiring: HW504 -> Pico
    - SW -> GP17 (Pin 22 - TX)
    - VRx -> GP27 (Pin 32 - ADC1)
    - VRy -> GP26 (Pin 31 - ADC0)
    - GND -> GND
    - 5V -> VSYS (Pin 39)

    @param x_pin: The pin number of the x-axis.
    @param y_pin: The pin number of the y-axis.
    @param button_pin: The pin number of the button.
    @param orientation: The orientation of the joystick.
    """

    def __init__(
        self,
        x_pin=27,
        y_pin=26,
        button_pin=17,
        orientation: Orientation = Orientation.NONE,
    ):
        self.x_axis = ADC(Pin(x_pin))
        self.y_axis = ADC(Pin(y_pin))
        self.button = Pin(button_pin, Pin.IN, Pin.PULL_UP)
        self.orientation = orientation

    def _button(self) -> int:
        """Read the button value from the joystick."""
        return self.button.value()

    def get_axes(self) -> Vector:
        """
        Read the raw ADC values and transform them based on orientation.
        For a 90° left rotation:
          - new_x = original y-axis reading
          - new_y = 65535 - original x-axis reading
        (Other orientations can be handled similarly.)
        """
        # Read the original ADC values
        x_raw = self.x_axis.read_u16()
        y_raw = self.y_axis.read_u16()

        if self.orientation == Orientation.LEFT:
            # Apply 90° left rotation transformation
            new_x = y_raw
            new_y = 65535 - x_raw
        elif self.orientation == Orientation.RIGHT:
            # For completeness, here’s a transformation for 90° right rotation:
            # new_x = 65535 - y_raw, new_y = x_raw
            new_x = 65535 - y_raw
            new_y = x_raw
        elif self.orientation == Orientation.UP:
            # For a 180° rotation: both values are flipped.
            new_x = 65535 - x_raw
            new_y = 65535 - y_raw
        else:
            # Orientation.NONE
            new_x = x_raw
            new_y = y_raw

        return Vector(new_x, new_y)

    def value(self, button: Button = Button.CENTER) -> bool:
        """
        Return the state of a button based on the transformed x,y axis values.
        Thresholds:
          - x-axis:
              0 - 10000: left
              20000 - 33000: middle
              40000 - 65000: right
          - y-axis:
              0 - 10000: up
              20000 - 33000: middle
              40000 - 65000: down
        """
        # Get the (possibly transformed) axis values
        vector = self.get_axes()
        x = vector.x
        y = vector.y

        # Now, apply the thresholds to determine the directional press.
        if button == Button.LEFT:
            return x < 10000
        if button == Button.RIGHT:
            return x > 40000
        if button == Button.UP:
            return y < 10000
        if button == Button.DOWN:
            return y > 40000
        if button == Button.CENTER:
            return self._button() == 0

        return False
