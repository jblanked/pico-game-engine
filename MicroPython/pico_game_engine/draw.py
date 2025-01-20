from .ILI9341 import (
    ILI9341,
    color565,
    Font,
    AdafruitGFX5x7Font,
    CMSansSerif2012,
    CMSansSerif201224,
    CMSansSerif201231,
)
from .image import Image
from .vector import Vector


class Draw:
    """
    The Draw class is used to draw images and text on the display.

    :param display: The display object to draw on.
    """

    def __init__(self, display: ILI9341):
        self.s = display

    def clear(
        self,
        pos: Vector = Vector(0, 0),
        size: Vector = Vector(320, 240),
        color=color565(0, 0, 0),
    ):
        """
        Clears the display at the specified position and size with the specified color.
        """
        posi: Vector = Vector.from_val(pos)
        sizi: Vector = Vector.from_val(size)
        self.s.display.fill_rectangle(posi.x, posi.y, sizi.x, sizi.y, color)

    def image(self, img: Image, x: int, y: int):
        """
        Draws an image on the display at the specified position.
        The Image object (or sprite.image) must have its buffer (a framebuf.FrameBuffer)
        and size (width, height) defined.
        """
        self.s.display.blit(img.buffer, x, y, img.size.x, img.size.y)

    def text(self, text: str, x: int, y: int):
        """
        Draws text on the display at the specified position.
        """
        self.s.display.set_pos(x, y)
        self.s.display.print(text)

    def color(
        self,
        foreground=color565(255, 255, 255),
        background=color565(0, 0, 0),
    ):
        """
        Sets the text color and background color.
        """
        self.s.display.set_color(foreground, background)

    def font(self, font: Font):
        """
        Sets the font for text rendering.
        """
        if font == Font.AdafruitGFX5x7Font:
            self.s.display.set_font(AdafruitGFX5x7Font)
        elif font == Font.CMSansSerif2012:
            self.s.display.set_font(CMSansSerif2012)
        elif font == Font.CMSansSerif201224:
            self.s.display.set_font(CMSansSerif201224)
        elif font == Font.CMSansSerif201231:
            self.s.display.set_font(CMSansSerif201231)
