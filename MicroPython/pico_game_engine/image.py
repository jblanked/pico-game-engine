# from https://github.com/harlepengren/PicoGame/blob/main/picogame/image.py
# edited by JBlanked on 2025-01-19
import framebuf
from .vector import Vector

SEEK_CUR = 1


class Image:
    """
    Represents an image.

    The image is stored in a buffer that can be drawn to the screen.
    You can create an image from a file path, a byte array, or a string.
    If you create an image from a file, the image must be in 16-bit BMP format.
    """

    def __init__(self):
        self.size = Vector(0, 0)
        self.buffer = None
        self._image_data = []

    def from_path(self, path: str):
        """Create an image from a file path"""
        self.open_image(path)
        self.create_image_buffer()

    def from_byte_array(self, data, width, height):
        """Create an image from a byte array"""
        if len(data) != width * height * 2:
            raise ValueError("Data length does not match given width and height!")
        self.size = Vector(width, height)
        self.buffer = framebuf.FrameBuffer(data, width, height, framebuf.RGB565)

    def open_image(self, path):
        """Open a 16-bit BMP file and read the image data"""
        with open(path, mode="rb") as f:
            signature = f.read(2)
            fsize = int.from_bytes(f.read(4), "little")
            f.seek(4, SEEK_CUR)
            dataOffset = int.from_bytes(f.read(4), "little")

            infoHeaderSize = int.from_bytes(f.read(4), "little")
            self.size = Vector(
                int.from_bytes(f.read(4), "little"),
                int.from_bytes(f.read(4), "little"),
            )
            f.seek(2, SEEK_CUR)
            depth = int.from_bytes(f.read(2), "little")

            # If compression is 3 then we also need the mask fields to determine whether it is RGB555 or RGB565
            compression = int.from_bytes(f.read(4), "little")
            imageSize = int.from_bytes(f.read(4), "little")

            f.seek(4, SEEK_CUR)

            # colorUsed = struct.unpack('2I', f.read(8))
            f.seek(8, SEEK_CUR)
            masks = (
                int.from_bytes(f.read(4), "little"),
                int.from_bytes(f.read(4), "little"),
                int.from_bytes(f.read(4), "little"),
                int.from_bytes(f.read(4), "little"),
            )

            # Go to end of head
            f.seek(dataOffset)

            rowSize = int(self.size.x * depth / 32 * 4)

            self._image_data = []
            for currentRow in range(0, self.size.y):
                for currentColumn in range(0, self.size.x):
                    currentRead = f.read(2)
                    if len(currentRead) > 0:
                        self._image_data.append(currentRead)

    def create_image_buffer(self):
        """Create a frame buffer from the image data"""
        width = self.size.x
        height = self.size.y
        temp_buffer = bytearray(width * height * 2)
        self.buffer = framebuf.FrameBuffer(temp_buffer, width, height, framebuf.RGB565)

        # We need to flip rows because bmp starts on the bottom left.
        for y in range(0, height):
            for x in range(0, width):
                self.buffer.pixel(
                    x,
                    y,
                    int.from_bytes(
                        self._image_data[(height - 1 - y) * width + x], "little"
                    ),
                )

    def from_string(self, image_str: str):
        """Use a string to create an image. One byte per pixel. Height and width are inferred based on carriage returns.
        The input must have equal length rows."""

        width = 0
        height = 0

        self._image_data = []
        for _char in image_str:
            if _char == "\n":
                if height == 1:
                    # First line, let's get the width
                    width = len(self._image_data)
                height += 1
            elif _char == " ":
                pass
            elif (_char == ".") or (_char == "f"):
                self._image_data.append(0)
            elif _char == "1":
                self._image_data.append(int(0xFFFF))
            elif _char == "2":
                self._image_data.append(int(0xF904))
            elif _char == "3":
                self._image_data.append(int(0xFC98))
            elif _char == "4":
                self._image_data.append(int(0xFC06))
            elif _char == "5":
                self._image_data.append(int(0xFFA1))
            elif _char == "6":
                self._image_data.append(int(0x24F4))
            elif _char == "7":
                self._image_data.append(int(0x7ECA))
            elif _char == "8":
                self._image_data.append(int(0x0215))
            elif _char == "9":
                self._image_data.append(int(0x879F))
            elif _char == "a":
                self._image_data.append(int(0xC05E))
            elif _char == "b":
                self._image_data.append(int(0xFC9F))
            elif _char == "c":
                self._image_data.append(int(0x50CA))
            elif _char == "d":
                self._image_data.append(int(0xACF0))
            elif _char == "e":
                self._image_data.append(int(0x7B07))
            else:
                self._image_data.append(int(0x0020))

        height -= 1

        temp_buffer = bytearray(width * height * 2)
        self.buffer = framebuf.FrameBuffer(temp_buffer, width, height, framebuf.RGB565)

        # We need to flip rows because bmp starts on the bottom left.
        for y in range(0, height):
            for x in range(0, width):
                self.buffer.pixel(x, y, self._image_data[y * width + x])

        self.size = Vector(width, height)
